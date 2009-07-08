# src/CMakeLists.txt

#Masochismus
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")

#Konfigurák (pro zapsání SVN verze)
configure_file(${CMAKE_SOURCE_DIR}/src/configure.h.cmake ${CMAKE_BINARY_DIR}/configure.h)

#Přilinkování SDL
find_package(SDL)
find_package(SDL_image)
find_package(SDL_ttf)
#find_package(SDL_timer)
include_directories(${CMAKE_BINARY_DIR} ${SDL_INCLUDE_DIR} ${SDLIMAGE_INCLUDE_DIR}
    ${SDLTTF_INCLUDE_DIR} ${SDLTIMER_INCLUDE_DIR})

#Knihovny
add_library(ConfParser STATIC ConfParser.cpp)
add_library(Effects STATIC Effects.cpp)
add_library(FPS STATIC FPS.cpp)
add_library(Localize STATIC Localize.cpp)
add_library(Menu STATIC Menu.cpp)
add_library(Skin STATIC Skin.cpp)
add_library(Splash STATIC Splash.cpp)
add_library(utility STATIC utility.cpp)

#Meziknihovní závislosti
target_link_libraries(ConfParser utility)
target_link_libraries(Effects utility ${SDL_LIBRARY})
target_link_libraries(FPS ${SDL_LIBRARY} ${SDLTIMER_LIBRARY})
target_link_libraries(Localize ConfParser)
target_link_libraries(Menu utility Effects ${SDL_LIBRARY} ${SDLTTF_LIBRARY})
target_link_libraries(Skin utility ConfParser Effects ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY})
target_link_libraries(Splash utility Effects ${SDL_LIBRARY} ${SDLTTF_LIBRARY})

#Hlavní program
add_executable(map2x main.cpp)
target_link_libraries(map2x FPS Localize Menu Skin Splash ${SDL_LIBRARY})
