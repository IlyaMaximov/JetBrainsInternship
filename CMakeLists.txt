cmake_minimum_required(VERSION 3.19)
project(JetBrainsInternship)

set(CMAKE_CXX_STANDARD 17)
find_package (Threads REQUIRED)

set(SOURCE_INDEXER
    indexer/Indexer.cpp
    indexer/SuffixAutomaton.cpp
)

set(SOURCE_INDEX_SUBSTRING_SEARCH
   index_searcher/DiskSearcher.cpp
   index_searcher/IndexSearch.cpp
)

set(SOURCE_PARALLEL_SUBSTRING_SEARCH
    parallel_search/ParallelSearcher.cpp
    parallel_search/StrSearch.cpp
)

set(SOURCE_PARALLEL_SUBSEQUENCE_SEARCH
    parallel_search/ParallelSearcher.cpp
    parallel_search/SeqSearch.cpp
)

add_executable(indexer ${SOURCE_INDEXER})
add_executable(substring-index-search ${SOURCE_INDEX_SUBSTRING_SEARCH})
add_executable(substring-parallel-search ${SOURCE_PARALLEL_SUBSTRING_SEARCH})
add_executable(subsequence-parallel-search ${SOURCE_PARALLEL_SUBSEQUENCE_SEARCH})

target_link_libraries(indexer stdc++fs)
target_link_libraries(substring-index-search stdc++fs)
target_link_libraries(substring-parallel-search ${CMAKE_THREAD_LIBS_INIT} stdc++fs)
target_link_libraries(subsequence-parallel-search ${CMAKE_THREAD_LIBS_INIT} stdc++fs)