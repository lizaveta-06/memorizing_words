cmake_minimum_required(VERSION 3.16)
project(FlashcardApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(SOURCES
    src/main.cpp
    src/word.cpp
    src/wordlist.cpp
    src/mainwin.cpp
)

set(HEADERS
    src/word.h
    src/wordlist.h
    src/mainwin.h
)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")

add_executable(FlashcardApp ${SOURCES} ${HEADERS})

target_link_libraries(FlashcardApp Qt5::Widgets)