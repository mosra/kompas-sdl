# Target OS
SET(CMAKE_SYSTEM_NAME Windows)

# C a C++ kompilátor
SET(CMAKE_C_COMPILER i486-mingw32-gcc)
SET(CMAKE_CXX_COMPILER i486-mingw32-g++)

# Cesta k knihovnám a programům zkompilovaným pro Windows
SET(CMAKE_FIND_ROOT_PATH /usr/i486-mingw32 )

# Tam hledat jen knihovny (kompilátor je v /usr/bin)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
