#ifndef LOCALIZE_H
#define LOCALIZE_H

#include <iostream>
#include <string>
#include <vector>

#include "ConfParser.h"

/**
 * @brief Jazykové lokalizace
 *
 * Umožňuje jazykovou lokalizaci programu z conf souborů, možnost změny jazyka
 * za běhu, fallback jazyk pro dosud nepřeložené části programu
 * @todo Fallback jen volitelně!
 */
class Localize {
    public:
        /**
         * @brief Konstruktor
         *
         * @param   file        Soubor s jazykem
         * @param   fallback    Fallback soubor (použije se, pokud některé
         *  parametry v hlavním souboru chybí)
         */
        Localize(const std::string& file, const std::string& fallback):
            lang(file), fallback(fallback) { load(file, fallback); }

        /**
         * @brief Destruktor
         */
        ~Localize(void);

        /**
         * @brief Načtení jazyka
         *
         * @param   file        Soubor s jazykem
         * @param   fallback    Fallback soubor (použije se, pokud některé
         *  parametry v hlavním souboru chybí)
         */
        void load(const std::string& file, const std::string& fallback);

        /**
         * @brief Získání lokalizovaného textu
         *
         * @param   parameter   Parametr
         * @param   section     Sekce
         */
        std::string* get(const std::string& parameter, const std::string& section = ConfParser::DEFAULT_SECTION);

    private:
        /** Konfigurák s jazykem */
        ConfParser lang;

        /** Fallback jazyk */
        ConfParser fallback;

        /** Lokalizovaný text */
        struct Localization {
            std::string parameter;
            std::string section;
            std::string* text;
        };

        /** Vektor s lokalizovanými texty */
        std::vector<Localization> localizations;
};

#endif