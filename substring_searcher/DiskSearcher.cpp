#include "DiskSearcher.h"

#define logInfo(msg) std::cerr << "[INFO] " << msg << '\n';
#define logError(msg) std::cerr << "[ERROR] " << msg << '\n';

DiskSearcher::DiskSearcher(fs::path data_path, fs::path source_data_path):
    data_path_(std::move(data_path)),
    source_data_path_(std::move(source_data_path)) {

    if (!fs::exists(data_path_)) {
        logError("Prepared data file does not exist")
        exit(1);
    }

    std::ifstream in(data_path_, std::ifstream::ate | std::ifstream::binary);
    u_int32_t data_size = in.tellg();
    in.close();

    int file_descriptor = open(data_path_.c_str(), O_RDONLY, S_IRUSR|S_IRGRP|S_IROTH);
    void* file_data = mmap(nullptr, data_size, PROT_READ, MAP_SHARED, file_descriptor, 0);
    data_view_ = DataView(file_data);

    close(file_descriptor);
}

DiskSearcher::StateView::StateView(void* data) {
    auto* file_data = static_cast<uint32_t *>(data);

    first_line = *file_data;
    uint32_t edges_cnt = *(file_data + 1);

    char* edges_ptr = reinterpret_cast<char *>(file_data + 2);
    for (uint32_t edges_num = 0; edges_num < edges_cnt; ++edges_num, edges_ptr += 5) {
        char symbol = *edges_ptr;
        uint32_t state_num = *reinterpret_cast<uint32_t *>(edges_ptr + 1);
        edges[symbol] = state_num;
    }

    links_cnt = *reinterpret_cast<uint32_t *>(edges_ptr);
    inv_suf_links = reinterpret_cast<uint32_t *>(edges_ptr) + 1;
}

DiskSearcher::StateView DiskSearcher::StateView::getNext() const {
    return StateView(inv_suf_links + links_cnt);
}

DiskSearcher::DataView::DataView(void* data) {
    shifts_cnt = *(static_cast<uint32_t *>(data));
    shifts = static_cast<uint32_t *>(data) + 1;
    file_data = static_cast<char *>(data);
}

DiskSearcher::StateView DiskSearcher::DataView::getStateView(uint32_t state_num) const {
    uint32_t current_shift = shifts[state_num / 1000];

    StateView state_view(file_data + current_shift);
    for (uint32_t i = 0; i < state_num % 1000; ++i) {
        state_view = state_view.getNext();
    }

    return state_view;
}

void DiskSearcher::printAllOccurrences(const std::string &word) const {
    uint32_t tmp_state = 0;

    for (char next_char: word) {
        StateView stateView = data_view_.getStateView(tmp_state);
        if (stateView.edges.count(next_char)) {
            tmp_state = stateView.edges.at(next_char);
        } else {
            return;
        }
    }

    std::set<uint32_t> all_occurrences;
    std::ifstream fin(source_data_path_);
    printOccurrences(tmp_state, all_occurrences, fin);

    fin.close();
}

void DiskSearcher::printOccurrences(u_int tmp_state, std::set<u_int> &occurrences, std::ifstream& fin) const {
    StateView state_view = data_view_.getStateView(tmp_state);
    std::string line;

    if (!occurrences.count(state_view.first_line)) {
        fin.seekg(state_view.first_line);
        fin >> line;
        std::cout << line << std::endl;
        occurrences.insert(state_view.first_line);
    }
    for (uint32_t link_num = 0; link_num < state_view.links_cnt; ++link_num) {
        printOccurrences(state_view.inv_suf_links[link_num], occurrences, fin);
    }
}
