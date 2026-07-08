#include <iostream>
#include <string>
#include <windows.h>
#include "wordlist.h"

void clear() { system("cls"); }
void pause() { std::cout << "\nНажмите Enter..."; std::cin.ignore(); std::cin.get(); }

void showAll(WordList& list) {
    clear();
    std::cout << "=== ВСЕ СЛОВА ===\n\n";
    if (list.getSize() == 0) {
        std::cout << "Словарь пуст.\n";
    } else {
        for (int i = 0; i < list.getSize(); i++) {
            Word w = list.getWord(i);
            std::cout << i + 1 << ". " << w.getUnknown() << " -> " << w.getTranslation()
                      << " (+" << w.getRight() << "/-" << w.getWrong() << ")\n";
        }
    }
    pause();
}

void addWord(WordList& list) {
    clear();
    std::cout << "=== ДОБАВЛЕНИЕ СЛОВА ===\n\n";
    std::string unknown, translation;
    std::cout << "Слово: "; std::getline(std::cin, unknown);
    std::cout << "Перевод: "; std::getline(std::cin, translation);
    if (unknown.empty() || translation.empty()) {
        std::cout << "Ошибка: пустые поля!\n";
        pause();
        return;
    }
    list.addWord(Word(unknown, translation));
    list.saveFile("resources/words.txt");
    std::cout << "\nДобавлено!\n";
    pause();
}

int main() {
    SetConsoleOutputCP(65001);
    WordList list;
    list.loadFile("resources/words.txt");

    while (true) {
        clear();
        std::cout << "=== КАРТОЧКИ ДЛЯ ЗАУЧИВАНИЯ СЛОВ ===\n\n";
        std::cout << "1. Добавить слово\n2. Показать все слова\n3. Выйти\n\nВыберите: ";
        int choice; std::cin >> choice; std::cin.ignore();

        if (choice == 1) addWord(list);
        else if (choice == 2) showAll(list);
        else if (choice == 3) { std::cout << "\nДо свидания!\n"; break; }
        else { std::cout << "Неверный выбор!\n"; pause(); }
    }
    return 0;
}