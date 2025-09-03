cmake_minimum_required(VERSION 3.10)

project(NX-MemDetect
  VERSION 0.1
  LANGUAGES CXX)

# Add an executable target
add_executable(NX-MemDetect
    src/graph.cpp
    src/memoryEngine.cpp
    src/main.cpp
)