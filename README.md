Kompas is an portable navigation system with goal to be as multiplatform and
extensible as possible. Its core library is written in pure C++ with no
external dependencies, on top of it is Qt and SDL GUI. The application is
licensed under LGPLv3.

This is Kompas SDL GUI. It is currently unmaintained, but the goal is to make
fast and efficient interface for low-powered embedded devices and platforms
where Qt has not yet been ported.

INSTALLATION
============

You can either use packaging scripts, which are stored in package/ subdirectory,
or compile and install everything manually.

Dependencies
------------

 * CMake    - for building
 * SDL
 * Qt       - optionally, for unit tests

Compilation, installation
-------------------------

    mkdir -p build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=/usr .. && make
    make install

If you want to build also unit tests (which are not built by default),
pass -DBUILD_TESTS=True to CMake. Unit tests use QtTest framework.

CONTACT
=======

Want to learn more about the application? Found a bug or want to tell me an
awesome idea? Feel free to visit project website or contact me at:

 * Website - http://mosra.cz/blog/kompas.php
 * GitHub - http://github.com/mosra
 * E-mail - mosra@centrum.cz
 * Jabber - mosra@jabbim.cz
