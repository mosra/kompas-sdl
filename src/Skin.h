#ifndef SKIN_H
#define SKIN_H

/**
 * @file Skin.h
 * @brief Třída Skin
 */

#include <iostream>
#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "ConfParser.h"
#include "Effects.h"
#include "utility.h"

/**
 * @brief Skiny
 *
 * Umožňuje úplnou konfigurovatelnost skinů v conf souborech a jejich změnu bez
 * nutnosti restartu aplikace. Založeno na ukazatelích, takže je (téměř) nulová
 * režie při vykreslování.
 * @todo Přepsat z ukazatelů na reference (asi nepude?)
 * @todo Skiny podle velikosti displeje (větší menu pro větší atd.)
 */
class Skin {
    public:

        /**
         * @brief Konstruktor
         *
         * @param   _screen     Displejová surface
         * @param   file        Soubor se skinem
         * @todo Možné problémy při resize (ztráta cíle ukazatele) => dvojitý?
         */
        inline Skin(SDL_Surface* _screen, const std::string& file): screen(_screen) { load(file); }

        /** @brief Destruktor */
        ~Skin(void);

        /**
         * @brief Načtení skinu
         *
         * @param   file        Soubor se skinem
         */
        void load(const std::string& file);

        /**
         * @brief Získání ukazatele na vlastnost
         *
         * @param   parameter   Parametr
         * @param   section     Sekce
         * @return  Ukazatel na vlastnost
         */
        template<class T> T get(const std::string& parameter, std::string section = ConfParser::DEFAULT_SECTION);

        /**
         * @fn template<> SDL_Surface** Skin::get(const std::string& parameter, std::string section)
         * @brief Získání ukazatele na surface
         *
         * @param   parameter   Parametr
         * @param   section     Sekce
         * @return  Ukazatel na vlastnost
         */

        /**
         * @fn template<> TTF_Font** Skin::get(const std::string& parameter, std::string section)
         * @brief Získání ukazatele na font
         *
         * Font je získáván z conf souboru ze dvou parametrů - názvu souboru
         * (např. v parametru font) a velikosti písma (v parametru fontSize). Oba
         * paramtery musí být v conf souboru přítomné!
         * @param   parameter   Parametr
         * @param   section     Sekce
         * @return  Ukazatel na vlastnost
         */
    private:
        /** @brief Vlastnost skinu */
        template<class T> struct Property {
            std::string parameter;  /**< @brief Název parametru z conf souboru */
            std::string section;    /**< @brief Název sekce z conf souboru */
            T property;             /**< @brief Vlastnost */
        };

        SDL_Surface* screen;    /**< @brief Displejová surface */
        ConfParser conf;        /**< @brief Konfigurák skinu */

        /**
         * @brief Vektor s fonty
         *
         * Font je získáván z conf souboru ze dvou parametrů - názvu souboru
         * (např. v parametru font) a velikosti písma (v parametru fontSize). Oba
         * paramtery musí být v conf souboru přítomné!
         */
        std::vector<Property<TTF_Font**> > fonts;

        /**
         * @brief Vektor s pozciemi
         *
         * Pozice je získávána z conf souboru ze čtyř parametrů. Pokud není ve
         * funkci Skin::set parametr zadán, získávají se parametry x, y, w, h
         * z příslušné sekce. Pokud je parametr zadán, získávají se parametry
         * parametrX, parametrY, parametrW, parametrH.
         * @todo Získávat jen z jednoho parametru v conf souboru (x y w h oddělené
         *  jen mezerami)
         */
        std::vector<Property<SDL_Rect*> > positions;

        std::vector<Property<SDL_Surface**> > surfaces; /**< @brief Vektor se surfacy */
        std::vector<Property<SDL_Color*> > colors;      /**< @brief Vektor s barvami */
        std::vector<Property<int*> > numbers;           /**< @brief Vektor s čísly */
        std::vector<Property<Align*> > aligns;          /**< @brief Vektor se zarovnáními */
        std::vector<Property<std::string*> > texts;     /**< @brief Vektor s texty */
};

#endif
