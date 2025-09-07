/*
	NX-Software Memory Utilities
	main.cpp - Entry point for MemUtil
    Copyright (C) 2025 Electro-Corp

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>

#include "memoryEngine.h"
#include "graph.h"

#define VERSION_MAJOR_NUM 0
#define VERSION_MINOR_NUM 1
#define COPYRIGHT_YEARS "2025"

MemoryEngine* nxMemory;
Graph* graph;

void commandLineHelp();

// Loops
void commandInputLoop();
void processSelectLoop();
void memoryAnalyzeLoop();
void processSnapshotLoop(bool graph);

// Signals
void segfaultSignal(int);
void interruptSignal(int);
void setupSignals();

void cleanUp();

int main(int args, char* argv[]){
    printf("================= NX-MemoryUtils =================\n");
    printf("Version %d.%d - (c) %s NX-Software {@Electro-Corp}\n", VERSION_MAJOR_NUM, VERSION_MINOR_NUM, COPYRIGHT_YEARS);
    printf("==================================================\n");
    printf("Starting NXMemoryEngine....\n");
    nxMemory = new MemoryEngine();
    printf("Setup signals...\n");
    setupSignals();
    printf("Entering command line now, type 'h' for help.\n");

    commandInputLoop();

    return 0;
}

// Loops

void commandInputLoop(){
    while(/* some thing */ 1){
        printf("NX> ");
        std::string c;
        std::cin >> c;

        switch(c.c_str()[0]){
            case '\n':
                break;
            //
            // Process commmands
            //
            case 'p':
                processSelectLoop();
                break;
            case 's':
                if(nxMemory->getPID() == -1){
                    printf("Error: No process selected.\n");
                }else{
                    nxMemory->UpdateMemory();
                    printf("%d -> %ld mB\n", nxMemory->getPID(), nxMemory->getLastSnap().memKB / 1024);
                }
                break;
            case 'c':
                if(nxMemory->getPID() == -1){
                    printf("Error: No process selected.\n");
                }else{
                    processSnapshotLoop(false);
                }
                break;
            case 'g':
                if(nxMemory->getPID() == -1){
                    printf("Error: No process selected.\n");
                }else{
                    processSnapshotLoop(true);
                }
                break;
            //
            // General commands
            //
            case 'h':
                commandLineHelp();
                break;
            case 'q':
                exit(0);
                break;
            case 'a':
                printf("NX-MemoryUtil - (c) NX-Software {@ElectroCorp}\n");
                break;
            default:
                printf("Unrecognized command. Type 'h' for help.\n");
        }
    }
}

void processSelectLoop(){
    printf("Select process by name or PID? ([N]ame/[P]id): ");
    std::string c;
    std::cin >> c;
    
    int isName = c.c_str()[0] == 'n' || c.c_str()[0] == 'N';

    std::string prompt = "Name: ";

    if(isName){
        printf("Selecting process by name. (q to exit to commands)\n");
    }else{
        prompt = "PID: ";
        printf("Selecting process by PID. (q to exit to commands)\n");
    }

    while(1){
        printf("%s", prompt.c_str());
        std::cin >> c;

        if(c.c_str()[0] == 'q' && c.length() < 2){
            printf("Exiting process select mode.\n");
            commandInputLoop();
        }else{
            // Do we recognize this
            bool result = false;
            if(!isName){ 
                try{
                    result = nxMemory->addProgram(std::stoi(c));
                } catch(std::invalid_argument& e){
                    printf("Error: PID contained something other than numbers!\n");
                    result = false;
                } catch(std::out_of_range& e){
                    printf("Error: PID too large!\n");
                    result = false;
                }
            }
            else result = nxMemory->addProgram(c);

            if(!result){
                printf("Process select failed!\n");
            }else{
                printf("Process selected.\n");
                break;
            }
        }
    }
    commandInputLoop();
}

void processSnapshotLoop(bool graphEnabled){
    printf("Process %d: (Refreshing every one second, CTRL-C to return to command loop)\n", nxMemory->getPID());
    nxMemory->UpdateMemory();

    if(graphEnabled){
        graph = new Graph(std::string{"Memory Usage"}, "Time", "Memory (mB)", (int)(nxMemory->getLastSnap().memKB / 1024));
    }

    //long int cur = time(NULL), past = time(NULL);

    auto s_t = std::chrono::steady_clock::now();

    for(;;){
        //cur = time(NULL);
        //if((cur - past) > 0){
            //past = cur;
            nxMemory->UpdateMemory();
            // print
            if(graphEnabled){
                graph->addPoint((int)nxMemory->getLastSnap().memKB / 1024);
                graph->render();
            }else{
                printf("%ld mB\r", nxMemory->getLastSnap().memKB / 1024);
            }
            fflush(stdout);
        //}
        std::this_thread::sleep_until(s_t + std::chrono::seconds(1));
        s_t = std::chrono::steady_clock::now();
    }
}

void segfaultSignal(int sig){
    printf("Segmentation fault, cleaning up...\n");
    cleanUp();
    exit(-1);
}

void interruptSignal(int sig){
    printf("Exiting current loop, returning to commands.\n");
    if(graph) delete graph;
    signal(SIGINT, interruptSignal);
    commandInputLoop();
}

void setupSignals(){
    signal(SIGINT, interruptSignal);
    signal(SIGSEGV, segfaultSignal);
}

void commandLineHelp(){
    printf("== NX-MemUtils commands: ==\n");
    printf("> Process commands: \n");
    printf("p - Select process\n");
    printf("s - Snapshot of process memory\n");
    printf("c - Constantly refreshing process memory\n");
    printf("g - Constantly refreshing process memory with graph\n");
    printf("> General commands: \n");
    printf("h - Show this help\n");
    printf("a - Show about\n");
    printf("q - Quit program\n");
}

void cleanUp(){
    if(graph) delete graph;
    delete nxMemory;
}