cmake_minimum_required(VERSION 3.10)
project(PlatformerGame)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Находим SFML
# Указываем путь к SFML (при необходимости)
set(SFML_DIR "C:/SFML/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Добавляем папки с заголовочными файлами
include_directories(${CMAKE_SOURCE_DIR}/include)

# Проверяем наличие папки src
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/src")
    message(FATAL_ERROR "Папка src не найдена! Создайте папку src с исходными файлами.")
endif()

# Явно указываем исходные файлы
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Player.cpp
    src/Platform.cpp
    src/Coin.cpp
    src/Button.cpp
    src/GamePlayState.cpp
    src/MenuState.cpp
)

# Проверяем существование каждого файла
foreach(SOURCE_FILE ${SOURCES})
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/${SOURCE_FILE}")
        message(WARNING "Файл ${SOURCE_FILE} не найден!")
    endif()
endforeach()

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

# Связываем с SFML
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

# Копируем ресурсы в выходную директорию (для Windows)
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    )
endif()