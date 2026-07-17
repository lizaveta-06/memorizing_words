#include <iostream>
#include <string>
#include <windows.h>
#include "wordlist.h"

void clear() { system("cls"); }
void pause() { std::cout << "\nНажмите Enter"; std::cin.ignore(); std::cin.get(); }

//показ списка слов
void showAll(WordList& list) {
    clear();
    std::cout << "ВСЕ СЛОВА\n\n";
    if (list.getSize() == 0) {
        std::cout << "Словарь пуст\n";
    } else {
        for (int i = 0; i < list.getSize(); i++) {
            Word w = list.getWord(i);
            std::cout << i + 1 << ". " << w.getUnknown() << " -> " << w.getTranslation()
                      << " (+" << w.getRight() << "/-" << w.getWrong() << ")\n";
        }
    }
    pause();
}
//добавление
void addWord(WordList& list) {
    clear();
    std::cout << "ДОБАВЛЕНИЕ СЛОВА\n\n";
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
//редактирование
void editWord(WordList& list) {
    clear();
    std::cout << "РЕДАКТИРОВАНИЕ СЛОВА\n\n";
    if (list.getSize() == 0) {
        std::cout << "Словарь пуст. Нечего редактировать.\n";
        pause();
        return;
    }
   showAll(list); 
    
    std::cout << "Введите номер слова для редактирования: ";
    int index;
    std::cin >> index;
    std::cin.ignore();
    
    if (index < 1 || index > list.getSize()) {
        std::cout << "Неверный номер!\n";
        pause();
        return;
    }
    
    Word w = list.getWord(index - 1);
    std::cout << "\nТекущее слово: " << w.getUnknown() << " -> " << w.getTranslation() << "\n";
    std::string newUnknown, newTranslation;
    std::cout << "Новое слово: ";
    std::getline(std::cin, newUnknown);
    std::cout << "Новый перевод: ";
    std::getline(std::cin, newTranslation);
    
    if (!newUnknown.empty()) w.setUnknown(newUnknown);
    if (!newTranslation.empty()) w.setTranslation(newTranslation);
    
    list.editWord(index - 1, w);
    list.saveFile("resources/words.txt");
    std::cout << "\nСлово обновлено!\n";
    pause();
}

//удаление
void deleteWord(WordList& list) {
    clear();
    std::cout << "УДАЛЕНИЕ СЛОВА\n\n";
    if (list.getSize() == 0) {
        std::cout << "Словарь пуст. Нечего удалять.\n";
        pause();
        return;
    }
    
    showAll(list);
    
    std::cout << "Введите номер слова для удаления: ";
    int index;
    std::cin >> index;
    std::cin.ignore();
    
    if (index < 1 || index > list.getSize()) {
        std::cout << "Неверный номер!\n";
        pause();
        return;
    }
    list.removeWord(index - 1);
    list.saveFile("resources/words.txt");
    std::cout << "\nСлово удалено!\n";
    pause();
}

//тренировка
void train(WordList& list) {
    clear();
    std::cout << "ТРЕНИРОВКА\n\n";
    
    if (list.getSize() == 0) {
        std::cout << "Словарь пуст! Добавьте слова перед тренировкой.\n";
        pause();
        return;
    }
    
    int total = 0;
    int correct = 0;
    int incorrect = 0;
    
    while (true) {
        clear();
        std::cout << "ТРЕНИРОВКА\n\n";
        std::cout << "Сессия верно:" << correct << ", неверно:" << incorrect 
                  << ", всего:" << total << "\n\n";
        
        Word w = list.getSmartRandomWord();
        std::cout << "Слово: " << w.getUnknown() << "\n\n";
        std::cout << "Enter";
        std::cin.get();
        
        std::cout << "\nПеревод: " << w.getTranslation() << "\n\n";
        std::cout << "Ваш ответ:\n";
        std::cout << "  1 - Верно\n";
        std::cout << "  2 - Не верно\n";
        std::cout << "  0 - Выйти из тренировки\n\n";
        std::cout << "Выберите: ";
        
        int answer;
        std::cin >> answer;
        std::cin.ignore();
        
        if (answer == 0) {
            break;
        }
        
        if (answer == 1) {
            // находим слово и обновляем статистику
            for (int i = 0; i < list.getSize(); i++) {
                Word current = list.getWord(i);
                if (current.getUnknown() == w.getUnknown() && 
                    current.getTranslation() == w.getTranslation()) {
                    Word updated = current;
                    updated.incrementRight();
                    list.editWord(i, updated);
                    break;
                }
            }
            correct++;
        } else if (answer == 2) {
            for (int i = 0; i < list.getSize(); i++) {
                Word current = list.getWord(i);
                if (current.getUnknown() == w.getUnknown() && 
                    current.getTranslation() == w.getTranslation()) {
                    Word updated = current;
                    updated.incrementWrong();
                    list.editWord(i, updated);
                    break;
                }
            }
            incorrect++;
        } else {
            std::cout << "\nНеверный ввод! Нажмите 1, 2 или 0.\n";
            pause();
            continue;
        }
        
        total++;
        list.saveFile("resources/words.txt");
    }
    
    std::cout << "\nИТОГИ ТРЕНИРОВКИ\n";
    std::cout << "Всего слов: " << total << "\n";
    std::cout << "Верно: " << correct << "\n";
    std::cout << "Неверно: " << incorrect << "\n";
    if (total > 0) {
        std::cout << "Процент: " << (correct * 100 / total) << "%\n";
    }
    pause();
}

int main() {
    SetConsoleOutputCP(65001);
    WordList list;
    list.loadFile("resources/words.txt");

    while (true) {
        clear();
        std::cout << "КАРТОЧКИ ДЛЯ ЗАУЧИВАНИЯ СЛОВ\n\n";
        std::cout << "1. Добавить слово\n";
        std::cout << "2. Показать все слова\n";
        std::cout << "3. Редактировать слово\n";
        std::cout << "4. Удалить слово\n";
        std::cout << "5. Тренировка\n";
        std::cout << "6. Выйти\n\n";
        std::cout << "Выберите: ";
        int choice; std::cin >> choice; std::cin.ignore();

        if (choice == 1) addWord(list);
        else if (choice == 2) showAll(list);
        else if (choice == 3) editWord(list);
        else if (choice == 4) deleteWord(list);
        else if (choice == 5) train(list);
        else if (choice == 6) { std::cout << "\nПока!\n"; break; }
        else { std::cout << "Неверный выбор!\n"; pause(); }
    }
    return 0;
}