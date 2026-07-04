#include <iostream>
#include "word.h"

int main() {    
    Word w1("apple", "яблоко");
    std::cout <<  w1.getUnknown() << std::endl;
    std::cout << w1.getTranslation() << std::endl;
    std::cout << "+ :" << w1.getRight() 
              << ", - :" << w1.getWrong() << std::endl << std::endl;
    
    w1.incrementRight();
    w1.incrementRight();
    w1.incrementWrong();
    w1.incrementRight();
    
    std::cout << "+ : " << w1.getRight() << std::endl;
    std::cout << "- : " << w1.getWrong() << std::endl;
    std::cout << "успеваемость: " << (w1.getSuccessRate() * 100) << "%" << std::endl;
    
    w1.setUnknown("aple");
    w1.setTranslation("яблако");
    std::cout << "Было: apple -> яблоко" << std::endl;
    std::cout << "Стало: " << w1.getUnknown() << " -> " << w1.getTranslation() << std::endl;
    
    return 0;
}