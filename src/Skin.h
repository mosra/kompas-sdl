#ifndef SKIN_H
#define SKIN_H

#include <iostream>
#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "ConfParser.h"
#include "Effects.h"

/**
 * Skiny
 *
 * Umožňuje úplnou konfigurovatelnost skinů v conf souborech a jejich změnu bez
 * nutnosti restartu aplikace
 *
 * @todo Přepsat z ukazatelů na reference (asi nepude?)
 * @todo Skiny podle velikosti displeje (větší menu pro větší atd.)
 */
class Skin {
    public:

        /**
         * Konstruktor
         * @param   skinFile    Soubor se skinem
         */
        inline Skin(SDL_Surface* _screen, const std::string& file): screen(_screen) { load(file); }

        /**
         * Destruktor
         */
        ~Skin(void);

        /**
         * Načtení skinu
         * @param   skinFile    Soubor se skinem
         */
        void load(const std::string& file);

        /**
         * @brief Inicializace a získání ukazatele na vlastnost
         *
         * Pro možné typy vlastností viz Skin::surfaces, Skin::fonts, Skin::colors,
         * Skin::positions, Skin::numbers, Skin::aligns, Skin::texts
         * @param   parameter   Parametr
         * @param   section     Sekce
         * @return  Ukazatel na vlastnost
         */
        template<class T> T get(const std::string& parameter, std::string section = ConfParser::DEFAULT_SECTION);

    private:
        /**
         * Displejová surface
         * @todo Možné problémy při resize (ztráta cíle ukazatele) => dvojitý?
         */
        SDL_Surface* screen;

        /**
         * Konfigurák skinu
         */
        ConfParser conf;

        /**
        * Vlastnost skinu
        */
        template<class T> struct Property {
            std::string parameter;
            std::string section;
            T property;
        };

        /**
         * Vektor se surfacy
         */
        std::vector<Property<SDL_Surface**> > surfaces;

        /**
         * @brief Vektor s fonty
         *
         * Font je získáván z conf souboru ze dvou parametrů - názvu souboru
         * (např. v parametru font) a velikosti písma (v parametru fontSize). Oba
         * paramtery musí být v conf souboru přítomné!
         */
        std::vector<Property<TTF_Font**> > fonts;

        /**
         * Vektor s barvami
         */
        std::vector<Property<SDL_Color*> > colors;

        /**
         * @brief Vektor s pozicemi
         *
         * Pozice je získávána z conf souboru ze čtyř parametrů. Pokud není ve
         * funkci Skin::set parametr zadán, získávají se parametry x, y, w, h
         * z příslušné sekce. Pokud je parametr zadán, získávají se parametry
         * parametrX, parametrY, parametrW, parametrH.
         */
        std::vector<Property<SDL_Rect*> > positions;

        /**
         * Vektor s čísly
         */
        std::vector<Property<int*> > numbers;

        /**
         * @brief Vektor se zarovnáními
         *
         * Zarovnání je v conf souboru specifikováno dvěma slovy oddělenými
         * libovolným počtem bílých znaků. Všechna písmena musí být malá. Řetězce
         * top, middle a bottom platí pro vertikální zarovnání, řetězce left,
         * center a right pro horizontální. Default hodnota je vlevo nahoře, takže
         * např. při neuvedení horizontálního směru je tento nastaven na left.
         * Při uvedení více než dvou slov se postupuje zleva shora, když je tedy
         * uvedeno zarovnání left right, platí left.
         */
        std::vector<Property<Align*> > aligns;

        /**
         * Vektor s texty
         */
        std::vector<Property<std::string*> > texts;
};

#endif
