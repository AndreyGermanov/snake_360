cmake_minimum_required(VERSION 3.28)
project(                                        
    snake                                       # название проекта
    VERSION 1.0                                 # номер версии проекта
    DESCRIPTION "Snake"                         # описание
    LANGUAGES CXX                               # язык программирования
)

# Сборка в режиме отладки
set (CMAKE_BUILD_TYPE Debug)
set(CMAKE_AUTOMOC ON)

# Используемый стандарт C++
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# Файлы исходного кода
set (
    source 
    src/window.h 
    src/window.cpp 
    src/main.cpp 
    src/random.h 
    src/random.cpp
    src/matrix.h
    src/matrix.cpp
)

# Определение исполняемого файла
add_executable(snake ${source})

# Привязка Qt 6
find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)
target_link_libraries(snake PRIVATE Qt6::Core Qt6::Widgets ${LINK_FLAGS})

# Копируем изображения в папку сборки
file(COPY img/body.png DESTINATION img/)
file(COPY img/apple.png DESTINATION img/)
file(COPY img/bg.png DESTINATION img/)
file(COPY img/field.png DESTINATION img/)
file(COPY img/head.png DESTINATION img/)
