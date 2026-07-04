# Компилятор
CXX = g++

# Флаги компиляции
CXXFLAGS = -std=c++17 -Wall

# Имя итоговой программы
TARGET = flashcard.exe

# Исходные файлы
SOURCES = src/main.cpp src/word.cpp

# Объектные файлы
OBJECTS = main.o word.o

# Сборка по умолчанию
all: $(TARGET)

# Сборка программы
$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^

# Компиляция main.cpp
main.o: src/main.cpp src/word.h
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o main.o

# Компиляция word.cpp
word.o: src/word.cpp src/word.h
	$(CXX) $(CXXFLAGS) -c src/word.cpp -o word.o

# Очистка (удаляем всё, кроме исходников)
clean:
	del /f $(OBJECTS) $(TARGET) test.exe 2>nul || exit 0

# Запуск программы (ИСПРАВЛЕНО!)
run: $(TARGET)
	.\$(TARGET)

# Пересборка с нуля
rebuild: clean all

# Показать список файлов
list:
	dir /b

# Помощь
help:
	@echo Доступные команды:
	@echo   make        - собрать проект
	@echo   make run    - собрать и запустить
	@echo   make clean  - удалить .o и .exe файлы
	@echo   make rebuild - пересобрать с нуля
	@echo   make list   - показать список файлов