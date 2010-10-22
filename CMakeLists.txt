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
