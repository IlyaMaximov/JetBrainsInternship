#ifndef JETBRAINSINTERNSHIP_SUFFIXAUTOMATON_H
#define JETBRAINSINTERNSHIP_SUFFIXAUTOMATON_H

#include <filesystem>
#include <iostream>
#include <map>
#include <utility>
#include <fstream>
#include <set>

namespace fs = std::filesystem;

class SuffixAutomaton {
private:

    struct State {

        State() = default;

        State(uint32_t len, uint32_t line, uint32_t link, std::map<char, uint32_t> edges):
            max_len(len),
            first_line(line),
            suf_link(link),
            edges(std::move(edges)) {}

        uint32_t max_len = 0; // длина самой длинной строки принимаемой этим состоянием
        uint32_t first_line = -1; // первая строка вхождения match в автомат
        uint32_t suf_link = -1; // суффиксная ссылка, хранит state_num
        std::map<char, uint32_t> edges{}; // ребра переходов в другие состояния
        std::vector<uint32_t> inv_suf_links{}; // инвертированные суффиксные ссылки ведущие в это состояние
    };

public:
    explicit SuffixAutomaton();

    void build(fs::path sDataPath);

    [[nodiscard]] bool wordIn(const std::string& word) const;

    [[nodiscard]] int32_t firstLineEnteringWord(const std::string& word) const;

    void printAllOccurrences(const std::string& word) const;

    void loadDataToDisk(const fs::path& sDataPath) const;

private:

    void automationExtent(char next_char);

    void getAllOccurrences(u_int tmp_state, std::set<u_int> &occurrences) const;

    std::vector<State> states_{};
    uint32_t last_state_ = 0;
    std::ifstream::pos_type last_line_begin_ = 0;
    fs::path data_path_{};
};


#endif //JETBRAINSINTERNSHIP_SUFFIXAUTOMATON_H
