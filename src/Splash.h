#ifndef Kompas_Sdl_Splash_h
#define Kompas_Sdl_Splash_h
/*
    Copyright © 2007, 2008, 2009, 2010, 2011 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Kompas.

    Kompas is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Kompas is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

/**
 * @file Splash.h
 * @brief Třída Splash
 */

#include <vector>
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "utility.h"

namespace Kompas { namespace Sdl {

/**
 * @brief Zobrazení splashe
 *
 * Zobrazí obrázek a texty na pozadí. Plná podpora Skin a Localize.
 */
class Splash {
    public:
        /**
         * @brief Konstruktor
         *
         * @param   _screen     Displejová surface
         * @param   _image      Obrázek
         * @param   _position   Pozice
         * @param   _align      Zarovnání rámce vůči displeji
         * @todo Možné problémy při resize screen (ztráta cíle ukazatele) => dvojitý?
         */
        inline Splash(SDL_Surface* _screen, SDL_Surface** _image, SDL_Rect* _position, Align* _align):
            screen(_screen), image(_image), position(_position), align(_align) {}

        /**
         * @brief Vložení textu
         *
         * @param   font        Font
         * @param   color       Barva textu
         * @param   _position   Pozice textu
         * @param   _align      Zarovnání textu
         * @param   text        Text
         */
        void addText(TTF_Font** font, SDL_Color* color, SDL_Rect* _position, Align* _align, std::string* text);

        /** @brief Zobrazení splashe */
        void view(void);
    private:
        /** @brief Struktura pro text */
        struct Text {
            TTF_Font** font;        /**< @brief Font textu */
            SDL_Color* color;       /**< @brief Barva textu */
            SDL_Rect* position;     /**< @brief Pozice textu */
            Align* align; /**< @brief Zarovnání textu */
            std::string* text;      /**< @brief Text */
        };

        SDL_Surface *screen,        /**< @brief Displejová surface */
                    **image;        /**< @brief Obrázek splashe */
        SDL_Rect* position;         /**< @brief Pozice splashe */
        Align* align;     /**< @brief Zarování splashe vůči displeji */
        std::vector<Text> texts;    /**< @brief Vektor s texty */
};

}}

#endif
