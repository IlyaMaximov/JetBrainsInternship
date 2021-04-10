#ifndef JETBRAINSINTERNSHIP_DISKSEARCHER_H
#define JETBRAINSINTERNSHIP_DISKSEARCHER_H

#include <iostream>
#include <filesystem>
#include <utility>
#include <fstream>
#include <map>

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <set>

namespace fs = std::filesystem;

class DiskSearcher {
public:

    explicit DiskSearcher(fs::path data_path, fs::path source_data_path);

    void printAllOccurrences(const std::string& word) const;

private:

    void getAllOccurrences(u_int tmp_state, std::set<u_int> &occurrences, std::ifstream& fin) const;

    struct StateView {
        explicit StateView(void* data);
        [[nodiscard]] StateView getNext() const;

        uint32_t first_line = -1;          // первая строка вхождения match в автомат
        std::map<char, uint32_t> edges{};  // ребра переходов в другие состояния
        uint32_t links_cnt = 0;            // количество инвертированных суффиксных ссылок
        uint32_t* inv_suf_links = nullptr; // инвертированные суффиксные ссылки ведущие в это состояние
    };

    struct DataView {
        DataView() = default;
        explicit DataView(void* data);
        [[nodiscard]] StateView getStateView(uint32_t state_num) const;

        uint32_t shifts_cnt = 0;
        uint32_t* shifts = nullptr;
        char* file_data = nullptr;
    };


    fs::path data_path_{};
    fs::path source_data_path_{};
    DataView data_view_;
};

#endif //JETBRAINSINTERNSHIP_DISKSEARCHER_H
