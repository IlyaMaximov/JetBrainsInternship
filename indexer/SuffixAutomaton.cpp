#include "SuffixAutomaton.h"

SuffixAutomaton::SuffixAutomaton() {
    states_.emplace_back();
}

void SuffixAutomaton::build(fs::path data_path) {
    data_path_ = std::move(data_path);

    std::ifstream fin(data_path_);
    if (!fin.is_open()) {
        std::cout << "Incorrect path to data: " + std::string(data_path_) << std::endl;
        std::cout << "SuffixAutomaton was not built" << std::endl;
        return;
    }

    // get length of file for states_ reserve
    fin.seekg(0, std::ios::end);
    states_.reserve(2 * fin.tellg());
    fin.seekg(0, std::ios::beg);

    char next_symbol = ' ';
    std::ifstream::pos_type tmp_pos = 0;

    while (fin.get(next_symbol)) {
        automationExtent(next_symbol);
        tmp_pos += 1;

        if (next_symbol == '\n') {
            last_line_begin_ = tmp_pos;
        }
    }

    for (uint32_t state_num = 1; state_num < states_.size(); ++state_num) {
        states_[states_[state_num].suf_link].inv_suf_links.push_back(state_num);
    }

    fin.close();
}

void SuffixAutomaton::automationExtent(char next_char) {
    size_t curr_state_num = states_.size();
    states_.emplace_back();
    states_[curr_state_num].max_len = states_[last_state_].max_len + 1;
    states_[curr_state_num].first_line = last_line_begin_;

    uint32_t tmp_state = last_state_;
    while (tmp_state != -1 && !states_[tmp_state].edges.count(next_char)) {
        states_[tmp_state].edges[next_char] = curr_state_num;
        tmp_state = states_[tmp_state].suf_link;
    }

    if (tmp_state == -1)
        states_[curr_state_num].suf_link = 0;
    else {
        uint32_t q_state = states_[tmp_state].edges[next_char];

        if (states_[tmp_state].max_len + 1 == states_[q_state].max_len) {
            states_[curr_state_num].suf_link = q_state;
        } else {
            uint32_t clone_state = states_.size();
            states_.emplace_back(states_[tmp_state].max_len + 1,
                                 states_[q_state].first_line,
                                 states_[q_state].suf_link,
                                 states_[q_state].edges);

            while (tmp_state != -1 && states_[tmp_state].edges[next_char] == q_state) {
                states_[tmp_state].edges[next_char] = clone_state;
                tmp_state = states_[tmp_state].suf_link;
            }
            states_[q_state].suf_link = clone_state;
            states_[curr_state_num].suf_link = clone_state;
        }
    }
    last_state_ = curr_state_num;
}

bool SuffixAutomaton::wordIn(const std::string& word) const {
    uint32_t tmp_state = 0;

    for (char next_char: word) {
        if (states_[tmp_state].edges.count(next_char)) {
            tmp_state = states_[tmp_state].edges.at(next_char);
        } else {
            return false;
        }
    }

    return true;
}

int32_t SuffixAutomaton::firstLineEnteringWord(const std::string &word) const {
    uint32_t tmp_state = 0;

    for (char next_char: word) {
        if (states_[tmp_state].edges.count(next_char)) {
            tmp_state = states_[tmp_state].edges.at(next_char);
        } else {
            return -1;
        }
    }

    return static_cast<int32_t>(states_[tmp_state].first_line);
}

void SuffixAutomaton::printAllOccurrences(const std::string &word) const {
    uint32_t tmp_state = 0;

    for (char next_char: word) {
        if (states_[tmp_state].edges.count(next_char)) {
            tmp_state = states_[tmp_state].edges.at(next_char);
        } else {
            return ;
        }
    }

    std::set<uint32_t> all_occurrences;
    getAllOccurrences(tmp_state, all_occurrences);

    std::ifstream fin(data_path_);
    std::string line;
    for (uint32_t occurrence_pos: all_occurrences) {
        fin.seekg(occurrence_pos);
        fin >> line;
        std::cout << line << '\n';
    }
    fin.close();
}

void SuffixAutomaton::getAllOccurrences(u_int tmp_state, std::set<u_int> &occurrences) const {
    occurrences.insert(states_[tmp_state].first_line);

    for (uint32_t new_state: states_[tmp_state].inv_suf_links) {
        getAllOccurrences(new_state, occurrences);
    }
}

void SuffixAutomaton::loadDataToDisk(const fs::path& data_path) const {
    // записываю мой автомат в файл в бинарном формате
    std::ofstream out(data_path, std::ios::out | std::ios::binary);

    // здесь я буду хранить сдвиги для каждого 1000 state
    uint32_t word_shift_size = 1000;
    uint32_t words_shifts_cnt = (states_.size() + word_shift_size - 1) / word_shift_size;
    std::vector<uint32_t> words_shifts(words_shifts_cnt);

    out.write((char *) &words_shifts_cnt, sizeof(words_shifts_cnt));
    out.seekp(sizeof(uint32_t) + words_shifts_cnt * sizeof(uint32_t));

    uint32_t char_num = 0;
    uint32_t shift_num = 0;
    for (const auto& state: states_) {

        uint32_t edges_size = state.edges.size();
        uint32_t suf_links_size = state.inv_suf_links.size();

        if (char_num % 1000 == 0) {
            words_shifts[shift_num] = out.tellp();
            ++shift_num;
        }

        out.write((char *) &state.first_line, sizeof(state.first_line));

        out.write((char *) &edges_size, sizeof(edges_size));
        for (const auto [symbol, state_num]: state.edges) {
            out.write((char *) &symbol, sizeof(symbol));
            out.write((char *) &state_num, sizeof(state_num));
        }

        out.write((char *) &suf_links_size, sizeof (suf_links_size));
        out.write((char *) state.inv_suf_links.data(), suf_links_size * sizeof(uint32_t));

        ++char_num;
    }
    assert(words_shifts.size() == words_shifts_cnt);
    assert(words_shifts.size() == shift_num);

    out.seekp(sizeof(uint32_t));
    out.write((char *) words_shifts.data(), words_shifts.size() * sizeof(words_shifts.front()));

    out.close();
}

