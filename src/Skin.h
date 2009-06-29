#ifndef SKIN_H
#define SKIN_H

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "ConfParser.h"

/**
 * Zarovnání
 */
enum Align {
    left = 0x01,    center = 0x02,      right =  0x04,
    top =  0x08,    middle = 0x10,      bottom = 0x02
};

/**
 * Skiny
 *
 * Umožňuje úplnou konfigurovatelnost skinů v conf souborech a jejich změnu bez
 * nutnosti restartu aplikace
 *
 * @todo Property typu font
 */
class Skin {
    public:
        /**
         * Typ vlastnosti
         */
        enum propertyType { TEXT, FONT, NUMBER, SURFACE };

        /**
         * Alias pro lepší pochopení typu vracejícího
         */
        typedef int propertyId;

        /**
         * Konstruktor
         * @param   skinFile    Soubor se skinem
         */
        inline Skin(const std::string& file): conf(file) { load(file); }

        /**
         * Destruktor
         */
        inline ~Skin(void) { free(); }

        /**
         * Načtení skinu
         * @param   skinFile    Soubor se skinem
         */
        void load(const std::string& file);

        /**
         * Inicializace vlastnosti
         * @param   parameter   Parametr
         * @param   section     Sekce
         * @return  ID surface
         */
        propertyId set(propertyType type, const std::string& parameter, std::string section = ConfParser::DEFAULT_SECTION);

        /**
         * Získání nějaké vlastnosti
         * @param   id          ID vlastnosti
         * @param   property    Kam uložit ukazatel na vlastnost
         */
        template<class T> inline T* get(propertyId id) { return NULL; }

    private:
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
        std::vector<Property<SDL_Surface*> > surfaces;

        /**
         * Vektor s fonty
         */
        std::vector<Property<TTF_Font*> > fonts;

        /**
         * Vektor s čísly
         */
        std::vector<Property<int> > numbers;

        /**
         * Vektor s texty
         */
        std::vector<Property<std::string> > texts;

        /**
         * Uvolnění prostředků starého skinu
         */
        void free(void);
};

#endif