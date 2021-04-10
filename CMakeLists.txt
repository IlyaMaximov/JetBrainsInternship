cmake_minimum_required(VERSION 3.19)
project(JetBrainsInternship)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_INDEXER
    indexer/Indexer.cpp
    indexer/SuffixAutomaton.cpp
)

set(SOURCE
    indexer/SuffixAutomaton.cpp
    searcher/DiskSearcher.cpp
    searcher/Search.cpp
)

add_executable(indexer ${SOURCE_INDEXER})
add_executable(search ${SOURCE})

target_link_libraries(indexer stdc++fs)
target_link_libraries(search stdc++fs)