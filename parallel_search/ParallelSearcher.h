#ifndef JETBRAINSINTERNSHIP_PARALLELSEARCHER_H
#define JETBRAINSINTERNSHIP_PARALLELSEARCHER_H

#include <filesystem>
#include <iostream>
#include <fstream>
#include <mutex>

#define MAX_THREADS_CNT 8

namespace fs = std::filesystem;

enum SearchType {
    SUBSTRING,
    SUBSEQUENCE
};

class ParallelSearcher {
public:

    explicit ParallelSearcher(fs::path data_path);

    void printAllOccurrences(const std::string& word, SearchType ) const;

private:

    void alignShifts(std::vector<uint32_t>& shifts) const;

    void printOccurrences(uint32_t start_shift, uint32_t finish_shift, const std::string &match, SearchType ) const;

    fs::path data_path_{};
    static std::mutex mutex_;
};

#endif //JETBRAINSINTERNSHIP_PARALLELSEARCHER_H
