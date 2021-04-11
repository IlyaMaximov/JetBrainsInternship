#include "SuffixAutomaton.h"

#include <iostream>

#define logInfo(msg) std::cerr << "[INFO] " << msg << '\n';

int main() {
    SuffixAutomaton suffix_automation;
    std::string math;

    logInfo("Wait while the file is indexing ...")
    suffix_automation.build("../data/words.txt");

    logInfo("Loading data to the file  ...")
    suffix_automation.loadDataToDisk("../data/prepared_words.bin");

    logInfo("Indexing is finished")

    return 0;
}