cmake_minimum_required(VERSION 3.16)
project(TelegramOpener)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Автоматические системы Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Поиск Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Создание исполняемого файла
add_executable(TelegramOpener main.cpp)

# Подключение Qt библиотек
target_link_libraries(TelegramOpener Qt6::Widgets)

# Добавление include директорий
target_include_directories(TelegramOpener PRIVATE ${Qt6Widgets_INCLUDE_DIRS})
