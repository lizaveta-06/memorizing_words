#ifndef WORD_H
#define WORD_H

#include <string>

class Word {
private:
    std::string unknown;
    std::string translation;
    int right;
    int wrong;

public:
    Word();
    Word(const std::string& unknown, const std::string& translation);

    std::string getUnknown() const;
    std::string getTranslation() const;
    int getRight() const;
    int getWrong() const;

    void setUnknown(const std::string& unknown);
    void setTranslation(const std::string& translation);
    void incrementRight();
    void incrementWrong();

    double getSuccessRate() const;
};

#endif