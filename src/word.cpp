#include "word.h"

Word::Word() : unknown(""), translation(""), right(0), wrong(0) {}

Word::Word(const std::string& unknown, const std::string& translation)
    : unknown(unknown), translation(translation), right(0), wrong(0) {}

std::string Word::getUnknown() const { return unknown; }
std::string Word::getTranslation() const { return translation; }
int Word::getRight() const { return right; }   
int Word::getWrong() const { return wrong; }  

void Word::setUnknown(const std::string& u) { unknown = u; }
void Word::setTranslation(const std::string& t) { translation = t; }
void Word::incrementRight() { right++; }      
void Word::incrementWrong() { wrong++; }      

double Word::getSuccessRate() const {
    int total = right + wrong;
    if (total == 0) return 0.5;
    return (double)right / total;
}