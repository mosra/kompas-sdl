#ifndef Map2X_Sdl_Utility_h
#define Map2X_Sdl_Utility_h
/*
    Copyright © 2007, 2008, 2009, 2010 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Map2X.

    Map2X is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Map2X is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

/**
 * @file utility.h
 * @brief Utility nezávislé na SDL sloužící k triviálním operacím nad C++ knihovnami
 */

#include <string>

/**
 * @brief Nízkoúrovňové věci
 *
 * Funkce a třídy nezávislé na žádné externí knihovně, pro správu konfigurace,
 * lokalizace atd.
 */
namespace Map2X { namespace Sdl {

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
 * @brief Nalezení pozice dalšího znaku v UTF-8 řetězci
 *
 * Pokud je na aktuální pozici neplatný uvozovací znak UTF-8 sekvence, vrací
 * s chybou pozici následujícího znaku, pokud nesouhlasí hodnota druhého či
 * dalšího znaku UTF-8 sekvence, vrací pozici prvního znaku, na kterém nastala
 * chyba. Pokud je řetězec neočekávaně ukončen nebo předaná pozice odpovídá
 * poslednímu znaku, vrací pozici za koncem.
 * @sa http://en.wikipedia.org/wiki/UTF-8#Description
 * @param   str         String
 * @param   position    Startovní pozice
 * @return  Pozice dalšího znaku
 * @todo Proč nejde const string&? a const_iterator?
 */
std::string::iterator nextUTF8Character(std::string* str, std::string::iterator position);

/**
 * @brief Nalezení pozice předchozího znaku v UTF-8 řetězci
 *
 * Pokud je na předchozí pozici neplatný druhý či další znak UTF-8 sekvence,
 * vrací s chybou tuto pozici, pokud je před těmito znaky neplatný uvozovací
 * znak (jeho hodnota se neshoduje s délkou sekvence či jiná chyba), vrací
 * předchozí pozici, než která byla předána.
 * @sa http://en.wikipedia.org/wiki/UTF-8#Description
 * @param   str         String
 * @param   position    Startovní pozice
 * @return  Pozice předchozího znaku
 * @todo Proč nejde const string&? a const_iterator?
 */
std::string::iterator prevUTF8Character(std::string* str, std::string::iterator position);

}}

#endif
