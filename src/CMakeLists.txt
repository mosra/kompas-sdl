# src/CMakeLists.txt

#Masochismus
if(TOOLCHAIN_GP2X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
else(TOOLCHAIN_GP2X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
endif(TOOLCHAIN_GP2X)

#Konfigurák (pro zapsání SVN verze)
configure_file(${CMAKE_SOURCE_DIR}/src/configure.h.cmake ${CMAKE_BINARY_DIR}/configure.h)

#Přilinkování SDL
find_package(SDL)
find_package(SDL_image)
find_package(SDL_ttf)
include_directories(${CMAKE_BINARY_DIR})

#Knihovny
# MToolkit
add_library(MToolkit STATIC ConfParser.cpp Localize.cpp Matrix.cpp utility.cpp)

# MInterface
add_library(MInterface STATIC Effects.cpp FPS.cpp Keyboard.cpp Menu.cpp Mouse.cpp Skin.cpp Splash.cpp Toolbar.cpp)
target_link_libraries(MInterface MToolkit ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY})

#Hlavní program
add_executable(map2x main.cpp)
target_link_libraries(map2x MToolkit MInterface)
