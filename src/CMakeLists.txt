# src/CMakeLists.txt

#Konfigurák (pro zapsání SVN verze)
configure_file(${CMAKE_SOURCE_DIR}/src/configure.h.cmake ${CMAKE_BINARY_DIR}/configure.h)

#Přilinkování SDL
find_package(SDL)
find_package(SDL_image)
find_package(SDL_ttf)
include_directories(${CMAKE_BINARY_DIR} ${SDL_INCLUDE_DIR} ${SDLIMAGE_INCLUDE_DIR} ${SDLTTF_INCLUDE_DIR})

#Knihovny
add_library(ConfParser STATIC ConfParser.cpp)
add_library(Effects STATIC Effects.cpp)
add_library(Localize STATIC Localize.cpp)
add_library(Menu STATIC Menu.cpp)
add_library(Skin STATIC Skin.cpp)
add_library(Splash STATIC Splash.cpp)
add_library(utility STATIC utility.cpp)

#Meziknihovní závislosti
target_link_libraries(ConfParser utility)
target_link_libraries(Effects utility ${SDL_LIBRARY})
target_link_libraries(Localize ConfParser)
target_link_libraries(Menu utility Effects ${SDL_LIBRARY} ${SDLTTF_LIBRARY})
target_link_libraries(Skin utility ConfParser Effects ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY})
target_link_libraries(Splash utility Effects ${SDL_LIBRARY} ${SDLTTF_LIBRARY})

#Hlavní program
add_executable(map2x main.cpp)
target_link_libraries(map2x Skin Splash Menu Localize ${SDL_LIBRARY})
