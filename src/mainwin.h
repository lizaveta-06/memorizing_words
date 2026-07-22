#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QInputDialog>
#include "wordlist.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void showMainMenu();
    void showAddEditMenu();
    void saveWord();
    void showWordList();
    void updateWordList();
    void editWord();
    void deleteWord();
    void startTraining();
    void checkWord();
    void markCorrect();
    void markIncorrect();
    void finishTraining();

private:
    WordList wordList;

    QWidget* mainWidget;
    QVBoxLayout* mainLayout;

    QPushButton* btnAdd;
    QPushButton* btnShow;
    QPushButton* btnEdit;
    QPushButton* btnDelete;
    QPushButton* btnTrain;
    QPushButton* btnExit;

    QLineEdit* editUnknown;
    QLineEdit* editTranslation;
    QPushButton* btnSave;
    QPushButton* btnCancel;
    int editingIndex;

    QListWidget* wordListWidget;

    QLabel* lblUnknown;
    QLabel* lblTranslation;
    QLabel* lblStats;
    QPushButton* btnCheck;
    QPushButton* btnCorrect;
    QPushButton* btnIncorrect;
    QPushButton* btnFinish;
    Word currentWord;
    int sessionCorrect;
    int sessionIncorrect;
    bool isAnswerShown;

    void clearLayout();
    void recreateMainWidget();   // ← НОВАЯ ФУНКЦИЯ
    void showTrainingMenu();
    void updateStats();
};

#endif