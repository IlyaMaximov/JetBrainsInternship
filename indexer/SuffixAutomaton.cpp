#include "SuffixAutomaton.h"

SuffixAutomaton::SuffixAutomaton() {
    states_.emplace_back();
}

void SuffixAutomaton::build(fs::path sDataPath) {
    data_path_ = std::move(sDataPath);

    std::ifstream fin(data_path_);
    if (!fin.is_open()) {
        std::cout << "Incorrect path to data: " + std::string(data_path_) << std::endl;
        std::cout << "SuffixAutomaton was not built" << std::endl;
        return;
    }

    // get length of file for states_ reserve
    fin.seekg(0, std::ios::end);
    states_.reserve(fin.tellg());
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
}

void SuffixAutomaton::getAllOccurrences(u_int tmp_state, std::set<u_int> &occurrences) const {
    occurrences.insert(states_[tmp_state].first_line);

    for (uint32_t new_state: states_[tmp_state].inv_suf_links) {
        getAllOccurrences(new_state, occurrences);
    }
}



