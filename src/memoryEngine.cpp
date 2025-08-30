#include "memoryEngine.h"

MemoryEngine::MemoryEngine(){

}

bool MemoryEngine::addProgram(std::string name){
    
    return false;
}

bool MemoryEngine::addProgram(int pid){
    if(!doesPIDExist(pid)) return false;
    printf("Selecting process %d - %s", pid, getNameFromPID(pid).c_str());
    return true;
}




//
// Internal
//

// Check if PID exists

bool MemoryEngine::doesPIDExist(int pid){
#ifdef __linux__
    return !kill(pid, 0);
#endif
#ifdef WIN32_
    return false;
#endif
}

// Get name from PID
std::string MemoryEngine::getNameFromPID(int pid){
#ifdef __linux__
    std::string procPath = "/proc/" + std::to_string(pid) + "/comm";
    std::ifstream proc(procPath);
    std::string procName;
    std::getline(proc, procName, '\0');
    return procName;
#endif
#ifdef WIN32_
    return "Win32 Not implemented";
#endif
}