cmake_minimum_required(VERSION 2.6)

project(Map2X)

#Verze ze SVN
find_package(Subversion)
if(Subversion_FOUND)
    Subversion_WC_INFO(${CMAKE_HOME_DIRECTORY} Map2X)
    set(SVN_VERSION "SVN r${Map2X_WC_REVISION}")
else(Subversion_FOUND)
    set(SVN_VERSION "SVN unknown")
endif(Subversion_FOUND)

message(STATUS "Verze: ${SVN_VERSION}")

#Konfigurák (pro zapsání SVN verze)
configure_file(${CMAKE_SOURCE_DIR}/src/configure.h.cmake ${CMAKE_BINARY_DIR}/configure.h)
configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.version.cmake ${CMAKE_BINARY_DIR}/Doxyfile.version)

#Přilinkování SDL
find_package(SDL)
find_package(SDL_image)
find_package(SDL_ttf)
include_directories(${CMAKE_BINARY_DIR} ${SDL_INCLUDE_DIR} ${SDLIMAGE_INCLUDE_DIR} ${SDLTTF_INCLUDE_DIR})

#Knihovny
add_library(ConfParser STATIC src/ConfParser.cpp)
add_library(Effects STATIC src/Effects.cpp)
add_library(Localize STATIC src/Localize.cpp)
add_library(Menu STATIC src/Menu.cpp)
add_library(Skin STATIC src/Skin.cpp)
add_library(Splash STATIC src/Splash.cpp)
add_library(utility STATIC src/utility.cpp)

#Meziknihovní závislosti
target_link_libraries(ConfParser utility)
target_link_libraries(Effects utility ${SDL_LIBRARY})
target_link_libraries(Localize ConfParser)
target_link_libraries(Menu utility Effects ${SDL_LIBRARY})
target_link_libraries(Skin ConfParser Effects ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY})
target_link_libraries(Splash Effects ${SDL_LIBRARY})

#Hlavní program
add_executable(map2x src/main.cpp)
target_link_libraries(map2x Skin Splash Menu Localize ${SDL_LIBRARY})

#Doxygen dokumentace
find_package(Doxygen)
if(DOXYGEN_EXECUTABLE AND UNIX)

    # Adresář doc
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/doc)

    # Příkaz pro vytvoření dokumentace
    add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/doc/wtf ${CMAKE_SOURCE_DIR}/doc/latex/refman.tex
    COMMAND rm -rf ${CMAKE_SOURCE_DIR}/doc/*
    COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile > /dev/null
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

    # Příkaz pro vytvoření PDF dokumentace
    add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/doc/refman.pdf
    COMMAND make pdf > /dev/null
    COMMAND mv refman.pdf ../ && rm -r ../latex
    DEPENDS ${CMAKE_SOURCE_DIR}/doc/latex/refman.tex
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/doc/latex
    )

    # Custom target, který se postará o vytvoření dokumentace
    add_custom_target(
    documentation ALL
    DEPENDS ${CMAKE_SOURCE_DIR}/doc/wtf
    DEPENDS ${CMAKE_SOURCE_DIR}/doc/refman.pdf
    )

#Doxygen nenalezen
else(DOXYGEN_EXECUTABLE AND UNIX)
    message(STATUS "Doxygen není přítomen, žádná dokumentace nebude vytvořena.")
endif(DOXYGEN_EXECUTABLE AND UNIX)