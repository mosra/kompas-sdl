#ifndef UTIL_H
#define UTIL_H

#include <string>

/**
 * Utility nezávislé na SDL sloužící k triviálním operacím nad C++ knihovnami
 */

/**
 * Zarovnání
 */
enum Align {
    ALIGN_DEFAULT = 0,
    ALIGN_LEFT = 0x01,  ALIGN_CENTER = 0x02,    ALIGN_RIGHT  = 0x04,
    ALIGN_TOP  = 0x10,  ALIGN_MIDDLE = 0x20,    ALIGN_BOTTOM = 0x40
};

/**
 * Oseknutí mezer obalujících string
 * @param   string      String k oseknutí
 * @return  Oseknutý string
 */
std::string trim (const std::string& str);

#endif
