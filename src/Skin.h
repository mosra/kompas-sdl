#ifndef SKIN_H
#define SKIN_H

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "ConfParser.h"
#include "utility.h"

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
         * Konstruktor
         * @param   skinFile    Soubor se skinem
         */
        inline Skin(SDL_Surface* _screen, const std::string& file): screen(_screen), conf(file) { load(file); }

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
         * Inicializace a získání ukazatele na vlastnost
         * @param   parameter   Parametr
         * @param   section     Sekce
         * @return  ID surface
         */
        template<class T> T set(const std::string& parameter, std::string section = ConfParser::DEFAULT_SECTION);

    private:
        /**
         * Displejová surface
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
         * Vektor s fonty
         */
        std::vector<Property<TTF_Font**> > fonts;

        /**
         * Vektor s čísly
         */
        std::vector<Property<int*> > numbers;

        /**
         * Vektor se zarovnáními
         */
        std::vector<Property<Align*> > aligns;

        /**
         * Vektor s texty
         */
        std::vector<Property<std::string*> > texts;
};

#endif
