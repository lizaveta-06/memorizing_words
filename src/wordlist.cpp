#include "wordlist.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>

WordList::WordList() {
    srand(static_cast<unsigned>(time(nullptr)));
}

void WordList::addWord(const Word& word) {
    words.push_back(word);
}

void WordList::removeWord(int index) {
    if (index >= 0 && index < (int)words.size()) {
        words.erase(words.begin() + index);
    }
}

void WordList::editWord(int index, const Word& word) {
    if (index >= 0 && index < (int)words.size()) {
        words[index] = word;
    }
}

Word WordList::getWord(int index) const {
    if (index >= 0 && index < (int)words.size()) {
        return words[index];
    }
    return Word();
}

int WordList::getSize() const {
    return words.size();
}

void WordList::loadFile(const std::string& filename) {
    words.clear();
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::string unknown, translation;
        int right = 0, wrong = 0;

        size_t pos1 = line.find('\t');
        size_t pos2 = line.find('\t', pos1 + 1);
        size_t pos3 = line.find('\t', pos2 + 1);

        if (pos1 != std::string::npos && pos2 != std::string::npos) {
            unknown = line.substr(0, pos1);
            translation = line.substr(pos1 + 1, pos2 - pos1 - 1);

            if (pos3 != std::string::npos) {
                right = std::stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
                wrong = std::stoi(line.substr(pos3 + 1));
            }

            Word w(unknown, translation);
            for (int i = 0; i < right; i++) w.incrementRight();
            for (int i = 0; i < wrong; i++) w.incrementWrong();
            words.push_back(w);
        }
    }
    file.close();
}

void WordList::saveFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    for (const Word& w : words) {
        file << w.getUnknown() << "\t"
             << w.getTranslation() << "\t"
             << w.getRight() << "\t"
             << w.getWrong() << "\n";
    }
    file.close();
}

Word WordList::getRandomWord() const {
    if (words.empty()) return Word();
    int index = rand() % words.size();
    return words[index];
}