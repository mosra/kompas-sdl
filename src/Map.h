#ifndef Map2X_Sdl_Map_h
#define Map2X_Sdl_Map_h
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
 * @file Map.h
 * @brief Třída Map
 */

#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "FPS.h"

namespace Map2X { namespace Sdl {

/**
 * @brief Zobrazení mapy
 *
 * Base třída umožňující zobrazování mapových dlaždic na obrazovku
 */
class Map {
    public:
        /**
         * @brief Konstruktor
         *
         * @note Pro správnou funkčnost posouvání musí být inicializována třída FPS.
         */
        Map(SDL_Surface* _screen, SDL_Surface** _tileLoading, SDL_Surface** _tileNotFound);

        /** @brief Posun nahoru */
        void moveUp(unsigned int pixels);

        /** @brief Posun dolů */
        void moveDown(unsigned int pixels);

        /** @brief Posun doleva */
        void moveLeft(unsigned int pixels);

        /** @brief Posun doprava */
        void moveRight(unsigned int pixels);

        /**
         * @brief Zobrazení mapy
         */
        void view(TTF_Font** font, SDL_Color* color);

    private:
        /**
         * @brief Mapová dlaždice
         * @todo Bacha, jsme limitováni velikostí 32bit int (tj. ani teoreticky
         * nepůjde zobrazit celý svět na Googlu při max. přiblížení)
         */
        struct Tile {
            unsigned int x,         /** @brief X-ová souřadnice dlaždice */
                         y;         /** @brief Y-ová souřadnice dlaždice */
            SDL_Surface** image;    /** @brief Obrázek dlaždice */
            bool isLoaded;          /** @brief Zda je dlaždice načtena */
        };

        SDL_Surface* screen;        /** @brief Displejová surface */

        SDL_Surface** tileLoading,  /** @brief Obrázek použitý místo dlaždice při jejím načítání */
            **tileNotFound;         /** @brief Obrázek použitý místo dlaždice, pokud se nepodařila načíst */

        unsigned int tileW,         /** @brief Šířka dlaždice */
                     tileH;         /** @brief Výška dlaždice */
        std::vector<Tile>::size_type
            tileMatrixW,            /** @brief Šířka matice s dlaždicemi */
            tileMatrixH;            /** @brief Výška matice s dlaždicemi */
        std::vector<Tile> tiles;    /** @brief Vektor s dlaždicemi */

        unsigned int beginX,        /** @brief Počáteční x-ová souřadnice mapy */
                     beginY,        /** @brief Počáteční y-ová souřadnice mapy */
                     endX,          /** @brief Koncová+1 x-ová souřadnice mapy */
                     endY;          /** @brief Koncová+1 y-ová souřadnice mapy */

        unsigned int moveX,         /** @brief X-ové posunutí zobrazení matice */
                     moveY;         /** @brief Y-ové posunutí zobrazení matice */
        FPS::Data
            moveXData,              /** @brief Data pro X-ové posunutí */
            moveYData;              /** @brief Data pro Y-ové posunutí */

        /**
        * @brief Změna velikosti matice
        *
        * @note Funkce se spouští v konstruktoru nebo při resize okna.
        * <strong>Výpočet velikosti matice:</strong> Kromě dlaždic, které se
        * vejdou na displej v daném rozměru celé, musíme ještě počítat se
        * zbytkem prostoru - pokud je větší než 1 pixel, mohou být na displeji
        * 2 další řady (na každém konci jedna v jednopixelovém proužku). Toto
        * funguje i v případě, že daný rozměr je menší než rozměr dlaždice.
        *
        * <strong>Přidávání nových dlaždic:</strong> Nové dlaždice se přidávají
        * k existujícím (ty se netvoří znova, škoda procesorového času) -
        * doplňují se prázdné sloupce vpravo a řádky dole, aby se zachovala
        * návaznost původních dlaždic. Pokud je okno zmenšováno, jsou
        * odstraněny řádky a sloupce vpravo a dole.
        * @return Zda se velikost matice změnila
        *
        * @todo Přidávat a odebírat s ohledem na zachování vycentrované pozice
        * souměrně vlevo a vpravo, dolů a nahoru (aby pak nedocházelo ke
        * znovunačítání smazaných dlaždic)
        * @todo Priority načítání, multithread, semafory...
        */
        bool resizeMatrix(void);

        /**
         * @brief Načtení mapových dlaždic
         *
         * Najde ve vektoru dlaždic nenačtené a načte je
         */
        virtual void loadTiles(void);
};

}}

#endif
