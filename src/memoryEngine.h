/*
	NX-Software Memory Utilities
	memoryEngine.h - Main memory analysis engine
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
#pragma once

#include <string>
#include <fstream>
#include <sys/types.h>
#include <signal.h>
#include <sys/resource.h>

typedef struct{
    long memKB;
} MemSnapshot;

class MemoryEngine{
private:
    // PID
    int pid = -1;

    // Last snap
    MemSnapshot lastSnap;

    // Settings
    bool logToFile, saveLog;

    // does pid exist
    bool doesPIDExist(int pid);
    // get name from pid
    std::string getNameFromPID(int pid);
    // get memory from pid
    long getMemoryKBFromPID(int pid);
public:
    MemoryEngine();

    // Attempt to add an executable
    bool addProgram(std::string name);
    bool addProgram(int pid);

    // Grab proc memory at this time
    void UpdateMemory();

    // Getters
    int getPID(){
        return pid;
    }

    MemSnapshot getLastSnap(){
        return lastSnap;
    }
};