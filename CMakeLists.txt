cmake_minimum_required(VERSION 3.9)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(gameoflife LANGUAGES CXX)

add_executable(main src/main.cpp
                    src/renderer.cpp
                    src/ant_renderer.cpp
                    src/controller.cpp
                    src/ant_controller.cpp
                    src/agent.cpp
                    src/ant.cpp
                    src/game_of_life.cpp
                    src/adjacent_tiles.cpp)
target_include_directories(main PUBLIC include)
target_link_libraries(main PUBLIC -lsfml-graphics -lsfml-window -lsfml-system)

#find_package(OpenMP REQUIRED)
#if(OpenMP_CXX_FOUND)
#    target_link_libraries(main PUBLIC OpenMP::OpenMP_CXX)
#endif()

target_compile_features(main PUBLIC cxx_std_20)
