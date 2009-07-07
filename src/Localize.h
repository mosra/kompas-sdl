#ifndef LOCALIZE_H
#define LOCALIZE_H

/**
 * @file Localize.h
 * @brief Třída Localize
 */

#include <iostream>
#include <string>
#include <vector>

#include "ConfParser.h"

/**
 * @brief Jazykové lokalizace
 *
 * Umožňuje jazykovou lokalizaci programu z conf souborů, možnost změny jazyka
 * za běhu, fallback jazyk pro dosud nepřeložené části programu. Funkce, které
 * používají tyto lokalizace, pracují s ukazateli, takže může být jazyk změněn
 * bez jejich spoluúčasti.
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
        Localize(const std::string& file, const std::string& fallback = "")
            { load(file, fallback); }

        /** @brief Destruktor */
        ~Localize(void);

        /**
         * @brief Načtení jazyka
         *
         * Všechny texty získané přetím pomocí Localize::get se naleznou v novém
         * souboru, případně ve fallbacku.
         * @param   file        Soubor s jazykem
         * @param   _fallback   Fallback soubor (použije se, pokud některé
         *  parametry v hlavním souboru chybí)
         */
        void load(const std::string& file, const std::string& _fallback = "");

        /**
         * @brief Získání lokalizovaného textu
         *
         * @param   parameter   Parametr
         * @param   section     Sekce
         */
        std::string* get(const std::string& parameter, const std::string& section = ConfParser::DEFAULT_SECTION);

    private:
        /** @brief Lokalizovaný text */
        struct Localization {
            std::string parameter;  /**< @brief Název parametru z conf souboru */
            std::string section;    /**< @brief Název sekce z conf souboru */
            std::string* text;      /**< @brief Ukazatel na lokalizovaný text */
        };

        ConfParser lang;            /**< @brief Konfigurák s jazykem */
        ConfParser fallback;        /**< @brief Fallback jazyk */
        std::vector<Localization> localizations;    /**< @brief Vektor s lokalizovanými texty */
};

#endif