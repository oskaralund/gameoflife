cmake_minimum_required(VERSION 3.8)
project(gameoflife LANGUAGES CXX C)

#add_library(fontcache STATIC src/SDL_FontCache.c)
#target_include_directories(fontcache PUBLIC include)

#add_library(pbd2d STATIC src/point_cloud.cpp
#                         src/length_constraint.cpp
#                         src/bend_constraint.cpp
#                         src/pbd_system.cpp
#                         src/pbd_factory.cpp
#                         src/collisions.cpp
#                         src/geometry.cpp)
#target_include_directories(pbd2d PUBLIC include)

#add_library(console STATIC src/console.cpp)
#target_include_directories(console PUBLIC include)
#target_link_libraries(console PUBLIC fontcache)

add_executable(main src/main.cpp
                    src/renderer.cpp
                    src/controller.cpp
                    src/individual.cpp
                    src/ant.cpp
                    src/game_of_life.cpp)
#target_include_directories(main PUBLIC include)
target_link_libraries(main PUBLIC -lsfml-graphics -lsfml-window -lsfml-system)
target_compile_features(main PUBLIC cxx_std_17)
