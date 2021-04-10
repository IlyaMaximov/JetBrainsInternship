cmake_minimum_required(VERSION 3.19)
project(JetBrainsInternship)

set(CMAKE_CXX_STANDARD 17)

add_executable(JetBrainsInternship main.cpp indexer/SuffixAutomaton.cpp indexer/SuffixAutomaton.h)
target_link_libraries(JetBrainsInternship stdc++fs)