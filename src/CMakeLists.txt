# src/CMakeLists.txt

#Masochismus
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")

#Konfigurák (pro zapsání SVN verze)
configure_file(${CMAKE_SOURCE_DIR}/src/configure.h.cmake ${CMAKE_BINARY_DIR}/configure.h)

#Přilinkování SDL
find_package(SDL)
find_package(SDL_image)
find_package(SDL_ttf)
include_directories(${CMAKE_BINARY_DIR} ${SDL_INCLUDE_DIR} ${SDLIMAGE_INCLUDE_DIR}
    ${SDLTTF_INCLUDE_DIR} ${SDLTIMER_INCLUDE_DIR})

#Knihovny
add_library(libConfParser STATIC ConfParser.cpp)
add_library(libEffects STATIC Effects.cpp)
add_library(libFPS STATIC FPS.cpp)
add_library(libLocalize STATIC Localize.cpp)
add_library(libMenu STATIC Menu.cpp)
add_library(libSkin STATIC Skin.cpp)
add_library(libSplash STATIC Splash.cpp)
add_library(libToolbar STATIC Toolbar.cpp)
add_library(libutility STATIC utility.cpp)

#Meziknihovní závislosti
target_link_libraries(libConfParser libutility)
target_link_libraries(libEffects libutility ${SDL_LIBRARY})
target_link_libraries(libFPS ${SDL_LIBRARY} ${SDLTIMER_LIBRARY})
target_link_libraries(libLocalize libConfParser)
target_link_libraries(libMenu libEffects libutility ${SDL_LIBRARY} ${SDLTTF_LIBRARY})
target_link_libraries(libSkin libConfParser libEffects libutility ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY})
target_link_libraries(libSplash libEffects libutility ${SDL_LIBRARY} ${SDLTTF_LIBRARY})
target_link_libraries(libToolbar libEffects libutility ${SDL_LIBRARY} ${SDLTTF_LIBRARY})

#Hlavní program
add_executable(map2x main.cpp)
target_link_libraries(map2x libFPS libLocalize libMenu libSkin libSplash libToolbar libutility ${SDL_LIBRARY})
