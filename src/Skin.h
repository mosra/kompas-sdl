#ifndef SKIN_H
#define SKIN_H

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "ConfParser.h"

template<class T> struct Property {
    std::string parameter;
    std::string section;
    T property;
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
         * Typ
         */
        enum propertyType { TEXT, FONT, NUMBER, SURFACE };

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
        int init(propertyType type, const std::string& parameter, std::string section = ConfParser::DEFAULT_SECTION);

        /**
         * Získání nějaké vlastnosti
         * @param   id          ID vlastnosti
         * @param   property    Kam uložit ukazatel na vlastnost
         */
        template<class T> inline T* property(int id) { return NULL; }

    private:
        /**
         * Konfigurák skinu
         */
        ConfParser conf;

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