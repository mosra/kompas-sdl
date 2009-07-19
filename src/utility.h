#ifndef UTIL_H
#define UTIL_H

/**
 * @file utility.h
 * @brief Utility nezávislé na SDL sloužící k triviálním operacím nad C++ knihovnami
 */

#include <string>

namespace MToolkit {

/**
 * @brief Zarovnání
 *
 * Slouží ke specifikaci zarovnání objektu (spojením vertikální a horizontální
 * hodnoty pomocí binárního OR).
 */
enum Align {
    ALIGN_DEFAULT = 0,      /**< @brief Default zarovnání (vlevo nahoru) */
    ALIGN_LEFT = 0x01,      /**< @brief Doleva */
    ALIGN_CENTER = 0x02,    /**< @brief Doprostřed (horizontálně) */
    ALIGN_RIGHT  = 0x04,    /**< @brief Doprava */
    ALIGN_TOP  = 0x10,      /**< @brief Nahoru */
    ALIGN_MIDDLE = 0x20,    /**< @brief Doprostřed (vertikálně) */
    ALIGN_BOTTOM = 0x40     /**< @brief Dolů */
};

/**
 * @brief Oseknutí mezer obalujících string
 *
 * @param   str         String k oseknutí
 * @return  Oseknutý string
 */
std::string trim (const std::string& str);

}

#endif
