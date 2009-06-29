cmake_minimum_required(VERSION 2.6)

project(map2x2)

#Přilinkování SDL
find_package(SDL)
find_package(SDL_image)
find_package(SDL_ttf)
include_directories(${SDL_INCLUDE_DIR} ${SDLIMAGE_INCLUDE_DIR} ${SDLTTF_INCLUDE_DIR})

#Knihovny
add_library(utility STATIC src/utility.cpp)
add_library(ConfParser STATIC src/ConfParser.cpp)
add_library(Skin STATIC src/Skin.cpp)

#Meziknihovní závislosti
target_link_libraries(ConfParser utility)
target_link_libraries(Skin ConfParser ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY})

#Hlavní program
add_executable(map2x2 src/main.cpp)
target_link_libraries(map2x2 ConfParser Skin ${SDL_LIBRARY})