# Target OS
SET(CMAKE_SYSTEM_NAME Linux)

set(TOOLCHAIN_GP2X)

# C a C++ kompilátor
SET(CMAKE_C_COMPILER /usr/bin/arm-gp2x-linux-gcc)
SET(CMAKE_CXX_COMPILER /usr/bin/arm-gp2x-linux-g++)

# Cesta k knihovnám a programům zkompilovaným pro GP2X
SET(CMAKE_FIND_ROOT_PATH /usr/arm-gp2x-linux/ )

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
