#ifndef MTOOLKIT_UTILITY_H
#define MTOOLKIT_UTILITY_H

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

}

#endif
