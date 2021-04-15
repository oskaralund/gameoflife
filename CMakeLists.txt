cmake_minimum_required(VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(gameoflife LANGUAGES CXX C)

add_executable(main src/main.cpp
                    src/renderer.cpp
                    src/controller.cpp
                    src/individual.cpp
                    src/ant.cpp
                    src/game_of_life.cpp
                    src/adjacent_tiles.cpp)
target_include_directories(main PUBLIC include)
target_link_libraries(main PUBLIC -lsfml-graphics -lsfml-window -lsfml-system)
target_compile_features(main PUBLIC cxx_std_17)
