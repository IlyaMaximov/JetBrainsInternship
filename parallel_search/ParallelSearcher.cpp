#include <future>

#include "ParallelSearcher.h"

std::mutex ParallelSearcher::mutex_{};

ParallelSearcher::ParallelSearcher(fs::path data_path):
    data_path_(std::move(data_path)) {}


void ParallelSearcher::printAllOccurrences(const std::string &match, SearchType search_type) const {
    std::ifstream fin(data_path_);

    // get length of file
    fin.seekg(0, std::ios::end);
    uint32_t file_size = fin.tellg();
    fin.seekg(0, std::ios::beg);

    fin.close();

    std::vector<uint32_t> shifts(MAX_THREADS_CNT);
    for (uint32_t thread_num = 0; thread_num < MAX_THREADS_CNT; ++thread_num) {
        shifts[thread_num] = thread_num * (file_size / MAX_THREADS_CNT);
    }

    alignShifts(shifts);
    shifts.push_back(file_size - 1);

    std::vector<std::future<void>> tasks(MAX_THREADS_CNT);
    for (uint32_t thread_num = 0; thread_num < MAX_THREADS_CNT; ++thread_num) {
        tasks[thread_num] = std::async(std::launch::async,
                                       &ParallelSearcher::printOccurrences,
                                       this,
                                       shifts[thread_num],
                                       shifts[thread_num + 1],
                                       match,
                                       search_type
                                       );
    }

    for (uint32_t thread_num = 0; thread_num < MAX_THREADS_CNT; ++thread_num) {
        tasks[thread_num].wait();
    }
}

void ParallelSearcher::alignShifts(std::vector<uint32_t> &shifts) const {
    std::ifstream fin(data_path_);

    for (u_int& shift: shifts) {

        fin.seekg(shift);
        while (fin.get() != '\n') {
            ++shift;
        }

        ++shift;
    }
    shifts[0] = 0;

    fin.close();
}

void ParallelSearcher::printOccurrences(uint32_t start_shift, uint32_t finish_shift,
                                        const std::string &match, SearchType search_type) const {

    std::ifstream fin(data_path_);

    fin.seekg(start_shift);
    std::string word;

    while (fin.tellg() < finish_shift) {
        fin >> word;

        if (match.size() > word.size()) {
            continue;
        }

        bool is_good_word = false;

        if (search_type == SearchType::SUBSEQUENCE) {

            uint32_t match_i = 0;
            uint32_t word_i = 0;
            while (match_i < match.size() && word_i < word.size()) {

                if (match[match_i] == word[word_i]) {
                    ++match_i;
                    ++word_i;
                } else {
                    ++word_i;
                }
            }

            if (match_i == match.size()) {
                is_good_word = true;
            }

        } else {

            std::string tmp_string(match);
            tmp_string.append("#").append(word);
            size_t n = tmp_string.length();
            std::vector<uint32_t> pref_values(n);

            for (uint32_t i = 1; i < n; ++i) {
                uint32_t j = pref_values[i - 1];
                while (j > 0 && tmp_string[i] != tmp_string[j]) {
                    j = pref_values[j - 1];
                }
                if (tmp_string[i] == tmp_string[j]) {
                    ++j;
                }
                pref_values[i] = j;
            }

            for (uint32_t pref_value : pref_values) {
                if (pref_value == match.size()) {
                    is_good_word = true;
                }
            }
        }

        if (is_good_word) {
            std::lock_guard<std::mutex> guard(mutex_);
            std::cout << word << '\n';
        }
    }

    fin.close();
}
