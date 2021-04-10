#include "indexer/SuffixAutomaton.h"

#include <iostream>

int main() {
    SuffixAutomaton suffix_automation;
    std::string math;

    std::cout << "Wait while the file is indexing ..." << std::endl;
    suffix_automation.build("../data/words.txt");

    std::cout << "Write math: ";
    std::cin >> math;
    std::cout << std::endl;
    suffix_automation.printAllOccurrences(math);

//    std::cout << "LOL" << std::endl;
//    std::cout << suffix_automation.wordIn("11-point") << std::endl;
//    std::cout << suffix_automation.wordIn("12-point") << std::endl;
//    std::cout << suffix_automation.wordIn("abacate") << std::endl;
//    std::cout << suffix_automation.wordIn("13-point") << std::endl;
//    std::cout << suffix_automation.wordIn("abacatu") << std::endl;
//
//    std::cout << std::endl << std::endl;

//    std::cout << suffix_automation.firstLineEnteringWord("11-point") << std::endl;
//    std::cout << suffix_automation.firstLineEnteringWord("12-point") << std::endl;
//    std::cout << suffix_automation.firstLineEnteringWord("abacate") << std::endl;
//    std::cout << suffix_automation.firstLineEnteringWord("13-point") << std::endl;
//    std::cout << suffix_automation.firstLineEnteringWord("abacatu") << std::endl;
//    std::cout << suffix_automation.firstLineEnteringWord("ZZZ") << std::endl;

//    std::cout << std::endl << std::endl;

//    suffix_automation.printAllOccurrences("cat");

    return 0;
}
