#ifndef WORDLIST_H
#define WORDLIST_H

#include <vector>
#include <string> 
#include "word.h"

class WordList {
private:
    std::vector<Word> words;
public:
    WordList();
    //добавление 
    void addWord(const Word& word);
    //удаление
    void removeWord(int index);
    //редактирование
    void editWord(int index, const Word& word);
    //получение по индексу
    Word getWord(int index) const;
    //кол-во слов
    int getSize() const;
    //загрузка из файла
    void loadFile(const std::string& filename);
    //сохранение в файл
    void saveFile(const std::string& filename);
    //случайное слово
    Word getRandomWord() const;
};
#endif