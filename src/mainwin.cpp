#include "mainwin.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QHeaderView>
#include <QInputDialog>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), editingIndex(-1), sessionCorrect(0), sessionIncorrect(0), isAnswerShown(false) {

    setWindowTitle("Карточки для запоминания слов");
    setWindowFlags(Qt::WindowStaysOnTopHint);
    resize(600, 500);

    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);

    wordList.loadFile("../resources/words.txt");

    // Создаём центральный виджет
    recreateMainWidget();
    showMainMenu();
}

MainWindow::~MainWindow() {
    wordList.saveFile("../resources/words.txt");
}

void MainWindow::recreateMainWidget() {
    // Удаляем старый центральный виджет
    if (mainWidget) {
        delete mainWidget;
        mainWidget = nullptr;
    }
    // Создаём новый
    mainWidget = new QWidget(this);
    mainWidget->setStyleSheet("background-color: white;");
    setCentralWidget(mainWidget);
    mainLayout = new QVBoxLayout(mainWidget);
}

void MainWindow::clearLayout() {
    // Просто пересоздаём виджет
    recreateMainWidget();
}

void MainWindow::showMainMenu() {
    clearLayout();

    QLabel* title = new QLabel("Главное меню", mainWidget);
    title->setAlignment(Qt::AlignCenter);
    QFont font = title->font();
    font.setPointSize(18);
    title->setFont(font);
    mainLayout->addWidget(title);

    btnAdd = new QPushButton("1. Добавить слово", mainWidget);
    btnShow = new QPushButton("2. Показать все слова", mainWidget);
    btnEdit = new QPushButton("3. Редактировать слово", mainWidget);
    btnDelete = new QPushButton("4. Удалить слово", mainWidget);
    btnTrain = new QPushButton("5. Тренировка", mainWidget);
    btnExit = new QPushButton("6. Выход", mainWidget);

    for (QPushButton* btn : {btnAdd, btnShow, btnEdit, btnDelete, btnTrain, btnExit}) {
        btn->setFixedHeight(40);
        mainLayout->addWidget(btn);
    }

    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::showAddEditMenu);
    connect(btnShow, &QPushButton::clicked, this, &MainWindow::showWordList);
    connect(btnEdit, &QPushButton::clicked, this, &MainWindow::editWord);
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::deleteWord);
    connect(btnTrain, &QPushButton::clicked, this, &MainWindow::startTraining);
    connect(btnExit, &QPushButton::clicked, this, &MainWindow::close);
}

void MainWindow::showAddEditMenu() {
    bool isEdit = (editingIndex >= 0);
    clearLayout();

    QLabel* title = new QLabel(isEdit ? "Редактировать слово" : "Добавить слово", mainWidget);
    title->setAlignment(Qt::AlignCenter);
    QFont font = title->font();
    font.setPointSize(16);
    title->setFont(font);
    mainLayout->addWidget(title);

    QHBoxLayout* row1 = new QHBoxLayout();
    row1->addWidget(new QLabel("Слово:", mainWidget));
    editUnknown = new QLineEdit(mainWidget);
    row1->addWidget(editUnknown);
    mainLayout->addLayout(row1);

    QHBoxLayout* row2 = new QHBoxLayout();
    row2->addWidget(new QLabel("Перевод:", mainWidget));
    editTranslation = new QLineEdit(mainWidget);
    row2->addWidget(editTranslation);
    mainLayout->addLayout(row2);

    if (isEdit) {
        Word w = wordList.getWord(editingIndex);
        editUnknown->setText(QString::fromStdString(w.getUnknown()));
        editTranslation->setText(QString::fromStdString(w.getTranslation()));
    }

    QHBoxLayout* buttons = new QHBoxLayout();
    btnSave = new QPushButton(isEdit ? "Сохранить" : "Добавить", mainWidget);
    btnCancel = new QPushButton("Отмена", mainWidget);
    buttons->addWidget(btnSave);
    buttons->addWidget(btnCancel);
    mainLayout->addLayout(buttons);

    connect(btnSave, &QPushButton::clicked, this, &MainWindow::saveWord);
    connect(btnCancel, &QPushButton::clicked, this, &MainWindow::showMainMenu);
}

void MainWindow::saveWord() {
    std::string unknown = editUnknown->text().toStdString();
    std::string translation = editTranslation->text().toStdString();

    if (unknown.empty() || translation.empty()) {
        QMessageBox::warning(this, "Ошибка", "Поля не могут быть пустыми!");
        return;
    }

    Word w(unknown, translation);
    if (editingIndex >= 0) {
        wordList.editWord(editingIndex, w);
        editingIndex = -1;
    } else {
        wordList.addWord(w);
    }
    wordList.saveFile("resources/words.txt");
    QMessageBox::information(this, "Успех", "Слово сохранено!");
    showMainMenu();
}

void MainWindow::showWordList() {
    clearLayout();

    QLabel* title = new QLabel("Список слов", mainWidget);
    title->setAlignment(Qt::AlignCenter);
    QFont font = title->font();
    font.setPointSize(16);
    title->setFont(font);
    mainLayout->addWidget(title);

    wordListWidget = new QListWidget(mainWidget);
    mainLayout->addWidget(wordListWidget);
    updateWordList();

    btnCancel = new QPushButton("Назад", mainWidget);
    mainLayout->addWidget(btnCancel);
    connect(btnCancel, &QPushButton::clicked, this, &MainWindow::showMainMenu);
}

void MainWindow::updateWordList() {
    wordListWidget->clear();
    for (int i = 0; i < wordList.getSize(); i++) {
        Word w = wordList.getWord(i);
        QString text = QString("%1. %2 -> %3 (+%4/-%5)")
            .arg(i + 1)
            .arg(QString::fromStdString(w.getUnknown()))
            .arg(QString::fromStdString(w.getTranslation()))
            .arg(w.getRight())
            .arg(w.getWrong());
        wordListWidget->addItem(text);
    }
}

void MainWindow::editWord() {
    if (wordList.getSize() == 0) {
        QMessageBox::information(this, "Инфо", "Словарь пуст!");
        return;
    }
    bool ok;
    int index = QInputDialog::getInt(this, "Редактировать слово",
                                      "Введите номер слова для редактирования:",
                                      1, 1, wordList.getSize(), 1, &ok);
    if (!ok) return;

    editingIndex = index - 1;
    showAddEditMenu();
}

void MainWindow::deleteWord() {
    if (wordList.getSize() == 0) {
        QMessageBox::information(this, "Инфо", "Словарь пуст!");
        return;
    }
    bool ok;
    int index = QInputDialog::getInt(this, "Удалить слово",
                                      "Введите номер слова для удаления:",
                                      1, 1, wordList.getSize(), 1, &ok);
    if (!ok) return;

    Word w = wordList.getWord(index - 1);
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение",
                                  QString("Удалить слово \"%1\"?").arg(QString::fromStdString(w.getUnknown())),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        wordList.removeWord(index - 1);
        wordList.saveFile("resources/words.txt");
        QMessageBox::information(this, "Успех", "Слово удалено!");
    }
}

void MainWindow::startTraining() {
    if (wordList.getSize() == 0) {
        QMessageBox::information(this, "Инфо", "Словарь пуст! Добавьте слова.");
        return;
    }

    sessionCorrect = 0;
    sessionIncorrect = 0;
    isAnswerShown = false;
    showTrainingMenu();
}

void MainWindow::showTrainingMenu() {
    clearLayout();

    QLabel* title = new QLabel("Тренировка", mainWidget);
    title->setAlignment(Qt::AlignCenter);
    QFont font = title->font();
    font.setPointSize(18);
    title->setFont(font);
    mainLayout->addWidget(title);

    lblStats = new QLabel(mainWidget);
    lblStats->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(lblStats);

    lblUnknown = new QLabel(mainWidget);
    lblUnknown->setAlignment(Qt::AlignCenter);
    font = lblUnknown->font();
    font.setPointSize(28);
    lblUnknown->setFont(font);
    mainLayout->addWidget(lblUnknown);

    lblTranslation = new QLabel(mainWidget);
    lblTranslation->setAlignment(Qt::AlignCenter);
    font = lblTranslation->font();
    font.setPointSize(24);
    lblTranslation->setFont(font);
    lblTranslation->setStyleSheet("color: blue;");
    lblTranslation->hide();
    mainLayout->addWidget(lblTranslation);

    QHBoxLayout* buttons = new QHBoxLayout();
    btnCheck = new QPushButton("Проверить", mainWidget);
    btnCorrect = new QPushButton("Верно", mainWidget);
    btnIncorrect = new QPushButton("Неверно", mainWidget);
    btnFinish = new QPushButton("Финиш", mainWidget);

    btnCorrect->hide();
    btnIncorrect->hide();

    buttons->addWidget(btnCheck);
    buttons->addWidget(btnCorrect);
    buttons->addWidget(btnIncorrect);
    buttons->addWidget(btnFinish);
    mainLayout->addLayout(buttons);

    connect(btnCheck, &QPushButton::clicked, this, &MainWindow::checkWord);
    connect(btnCorrect, &QPushButton::clicked, this, &MainWindow::markCorrect);
    connect(btnIncorrect, &QPushButton::clicked, this, &MainWindow::markIncorrect);
    connect(btnFinish, &QPushButton::clicked, this, &MainWindow::finishTraining);

    currentWord = wordList.getSmartRandomWord();
    lblUnknown->setText(QString::fromStdString(currentWord.getUnknown()));
    lblTranslation->setText(QString::fromStdString(currentWord.getTranslation()));
    lblTranslation->hide();
    btnCheck->setEnabled(true);
    btnCheck->show();
    btnCorrect->hide();
    btnIncorrect->hide();
    isAnswerShown = false;
    updateStats();
}

void MainWindow::updateStats() {
    lblStats->setText(QString("Сессия: верно = %1, неверно = %2")
        .arg(sessionCorrect)
        .arg(sessionIncorrect));
}

void MainWindow::checkWord() {
    if (isAnswerShown) return;
    lblTranslation->show();
    btnCheck->setEnabled(false);
    btnCheck->hide();
    btnCorrect->show();
    btnIncorrect->show();
    isAnswerShown = true;
}

void MainWindow::markCorrect() {
    for (int i = 0; i < wordList.getSize(); i++) {
        Word w = wordList.getWord(i);
        if (w.getUnknown() == currentWord.getUnknown() &&
            w.getTranslation() == currentWord.getTranslation()) {
            Word updated = w;
            updated.incrementRight();
            wordList.editWord(i, updated);
            break;
        }
    }
    sessionCorrect++;
    wordList.saveFile("resources/words.txt");
    showTrainingMenu();
}

void MainWindow::markIncorrect() {
    for (int i = 0; i < wordList.getSize(); i++) {
        Word w = wordList.getWord(i);
        if (w.getUnknown() == currentWord.getUnknown() &&
            w.getTranslation() == currentWord.getTranslation()) {
            Word updated = w;
            updated.incrementWrong();
            wordList.editWord(i, updated);
            break;
        }
    }
    sessionIncorrect++;
    wordList.saveFile("resources/words.txt");
    showTrainingMenu();
}

void MainWindow::finishTraining() {
    QMessageBox::information(this, "Итоги тренировки",
                             QString("Всего слов: %1\nВерно: %2\nНеверно: %3")
                             .arg(sessionCorrect + sessionIncorrect)
                             .arg(sessionCorrect)
                             .arg(sessionIncorrect));
    showMainMenu();
}