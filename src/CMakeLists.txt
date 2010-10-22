if(TOOLCHAIN_GP2X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
else(TOOLCHAIN_GP2X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
endif(TOOLCHAIN_GP2X)

find_package(SDL)
find_package(SDL_image)
find_package(SDL_ttf)

find_package(Map2XCore REQUIRED)

include_directories(${MAP2X_CORE_INCLUDE_DIR})

set(Map2X_Sdl_SRCS
    ConfParser.cpp
    Effects.cpp
    FPS.cpp
    Keyboard.cpp
    Localize.cpp
    main.cpp
    Map.cpp
    Matrix.cpp
    Menu.cpp
    Mouse.cpp
    Skin.cpp
    Splash.cpp
    Toolbar.cpp
    utility.cpp
)

add_executable(map2x-sdl ${Map2X_Sdl_SRCS})
target_link_libraries(map2x-sdl ${MAP2X_CORE_LIBRARY} ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY})
