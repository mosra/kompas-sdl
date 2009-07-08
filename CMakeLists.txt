cmake_minimum_required(VERSION 2.6)

project(Map2X)

#Verze ze SVN
find_package(Subversion)
if(Subversion_FOUND)
    Subversion_WC_INFO(${CMAKE_HOME_DIRECTORY} Map2X)
    set(SVN_VERSION "r${Map2X_WC_REVISION}")
else(Subversion_FOUND)
    set(SVN_VERSION "unknown")
endif(Subversion_FOUND)

message(STATUS "Verze: SVN ${SVN_VERSION}")

#Zdrojáky programu
add_subdirectory(src)

#Konfigurák pro Doxygen
configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.version.cmake ${CMAKE_BINARY_DIR}/Doxyfile.version)

#Doxygen dokumentace
find_package(Doxygen)
if(DOXYGEN_EXECUTABLE AND UNIX)

    # Adresář doc
    file(REMOVE_RECURSE ${CMAKE_SOURCE_DIR}/doc)
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/doc)

    # Příkaz pro vytvoření dokumentace
    add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/doc/latex/ ${CMAKE_SOURCE_DIR}/doc/html/ ${CMAKE_SOURCE_DIR}/doc/
    COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile > /dev/null
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

    # Příkaz pro vytvoření PDF dokumentace
    add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/doc/manual-${SVN_VERSION}.pdf
    COMMAND make pdf > /dev/null
    COMMAND mv refman.pdf ../manual-${SVN_VERSION}.pdf && rm -rf ../latex
    DEPENDS ${CMAKE_SOURCE_DIR}/doc/latex/
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/doc/latex
    )

    # Custom target, který se postará o vytvoření dokumentace
    add_custom_target(
    Dokumentace ALL
    DEPENDS ${CMAKE_SOURCE_DIR}/doc/html/
    DEPENDS ${CMAKE_SOURCE_DIR}/doc/manual-${SVN_VERSION}.pdf
    )

#Doxygen nenalezen
else(DOXYGEN_EXECUTABLE AND UNIX)
    message(STATUS "Doxygen není přítomen, žádná dokumentace nebude vytvořena.")
endif(DOXYGEN_EXECUTABLE AND UNIX)
