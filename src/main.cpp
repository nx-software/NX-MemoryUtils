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

#include "memoryEngine.h"

#define VERSION_MAJOR_NUM 0
#define VERSION_MINOR_NUM 1
#define COPYRIGHT_YEARS "2025"

MemoryEngine* nxMemory;

void commandLineHelp();

// Loops
void commandInputLoop();
void processSelectLoop();

void cleanUp();

int main(int args, char* argv[]){
    printf("================= NX-MemoryUtils =================\n");
    printf("Version %d.%d - (c) %s NX-Software {@Electro-Corp}\n", VERSION_MAJOR_NUM, VERSION_MINOR_NUM, COPYRIGHT_YEARS);
    printf("==================================================\n");
    printf("Starting NXMemoryEngine....\n");
    nxMemory = new MemoryEngine();
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
            case 'h':
                commandLineHelp();
                break;
            case 'p':
                processSelectLoop();
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
            }
        }
    }
}

void commandLineHelp(){
    printf("NX-MemUtils commands:\n");
    printf("h - Show this help\n");
    printf("p - Select process\n");
    printf("a - Show about\n");
    printf("q - Quit program\n");
}

void cleanUp(){
    delete nxMemory;
}