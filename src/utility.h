#ifndef UTIL_H
#define UTIL_H

#include <string>

/**
 * Utility nezávislé na SDL sloužící k triviálním operacím nad C++ knihovnami
 */

/**
 * Oseknutí mezer obalujících string
 * @param   string      String k oseknutí
 * @return  Oseknutý string
 */
std::string trim (const std::string& str);

#endif
