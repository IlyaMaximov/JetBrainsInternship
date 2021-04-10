#include "DiskSearcher.h"

int main() {

    DiskSearcher searcher("../data/prepared_words.txt", "../data/words.txt");
    std::string match, command = "NOT STATED";

    while (true) {
        std::cout << "Write your substring for search: ";
        std::cin >> match;

        std::cout << "Matching strings: " << std::endl;
        searcher.printAllOccurrences(match);

        while (command != "Y" && command != "N" && command != "y" && command != "n") {
            std::cout << "Do you want to continue (Y / N): ";
            std::cin >> command;
        }

        if (command == "N" || command == "n") {
            return 0;
        } else {
            command = "NOT STATED";
        }

        std::cout << std::endl;
    }
}