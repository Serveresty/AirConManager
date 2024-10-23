cmake_minimum_required(VERSION 3.31.0)

# Название проекта
project(AirConManager)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/mingw81_64")

# Найти необходимые пакеты Qt5
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5 REQUIRED COMPONENTS Core)
find_package(Qt5 REQUIRED COMPONENTS Xml)
find_package(Qt5 REQUIRED COMPONENTS Gui)

# Пути к исходникам и заголовкам
set(SOURCES
    src/main.cpp
    src/coolwindow.cpp
    src/coolinputwindow.cpp
    src/settings.cpp
    includes/coolwindow.h
    includes/coolinputwindow.h
    includes/settings.h
)

# Создаем исполняемый файл
add_executable(AirConManager WIN32 ${SOURCES})

# Линкуем с библиотеками Qt
target_link_libraries(AirConManager Qt5::Widgets Qt5::Core Qt5::Xml Qt5::Gui)