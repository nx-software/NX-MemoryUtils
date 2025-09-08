#include "memoryEngine.h"

MemoryEngine::MemoryEngine(){

}

bool MemoryEngine::addProgram(std::string name){
#ifdef __linux__
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(std::string{"pidof -s " + name}.c_str(), "r"), pclose);
    if (!pipe) {
        printf("Failed to execute pidof!\n");
        return false;
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    if(result.size() < 1){
        return false;
    }
    return addProgram(std::stoi(result));
#endif
    return false;
}

bool MemoryEngine::addProgram(int pid){
    // check if its a program
    if(!doesPIDExist(pid)){
        this->pid = -1;
        return false;
    }
    // get name
    printf("Selecting process %d - %s", pid, getNameFromPID(pid).c_str());
    // set pid as ours
    this->pid = pid;
    return true;
}

void MemoryEngine::UpdateMemory(){
#ifdef __linux__
    this->lastSnap.memKB = getMemoryKBFromPID(this->pid);
#endif
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

// Get memory usage from pid
long MemoryEngine::getMemoryKBFromPID(int pid){
#ifdef __linux__
    std::string procPath = "/proc/" + std::to_string(pid) + "/status";
    std::ifstream proc(procPath);
    std::string curLine;

    while(std::getline(proc, curLine)){
        if(curLine.rfind("VmRSS:", 0) == 0){
            long rss_kb = 0;
            std::sscanf(curLine.c_str(), "VmRSS: %ld kB", &rss_kb);
            return rss_kb;
        }
    }

    return -1;
#endif
}