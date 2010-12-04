#ifndef Kompas_Sdl_Effects_h
#define Kompas_Sdl_Effects_h
/*
    Copyright © 2007, 2008, 2009, 2010 Vladimír Vondruš <mosra@centrum.cz>

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
 * @file Effects.h
 * @brief Třída Effects
 */

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "utility.h"

/**
 * @brief Uživatelské rozhraní
 *
 * Třídy tvořící uživatelské rozhraní programu (menu, toolbary, klávesnice...)
 * závislé na SDL.
 */
namespace Kompas { namespace Sdl {

/**
 * @brief Funkce pro zarovnávání, posouvání textu a další fičurky
 *
 * Funkce a vlastnosti jsou statické, aby byly použitelné globálně bez nutnosti
 * instantace třídy.
 */
class Effects {
    public:
        /**
         * @brief Zarovnání objektu do dané oblasti
         *
         * Zarovná objekt do dané oblasti podle předaného zarovnání a poté jej
         * případně posune. Šířka a výška vrácené pozice se dá poté použít k
         * dodatečnému ořezání objektu.
         * @param   area        Oblast
         * @param   _align      Zarovnání objektu
         * @param   objectW     Šířka objektu
         * @param   objectH     Výška objektu
         * @param   moveX       Dodatečné X-posunutí objektu po zarovnání
         * @param   moveY       Dodatečné Y-posunutí objektu po zarovnání
         * @param   crop        Ořezový obdélník
         * @return  Pozice objektu
         */
        static SDL_Rect align(const SDL_Rect& area, Align _align, int objectW, int objectH, int moveX = 0, int moveY = 0, SDL_Rect* crop = NULL);

        /**
         * @brief Zarovnání objektu do dané oblasti
         *
         * Souřadnice objektu ve struktuře SDL_Rect.
         * @param   area        Oblast
         * @param   _align      Zarovnání objektu
         * @param   object      Objekt
         * @param   crop        Ořezový obdélník
         * @return  Pozice objektu
         */
        inline static SDL_Rect align(const SDL_Rect& area, Align _align, const SDL_Rect& object, SDL_Rect* crop = NULL) {
            return align(area, _align, object.w, object.h, object.x, object.y, crop);
        }


        /**
         * @brief Zarovnání objektu vůči displeji
         *
         * @param   screen      Displejová surface
         * @param   _align      Zarovnání objektu
         * @param   objectW     Šířka objektu
         * @param   objectH     Výška objektu
         * @param   moveX       Dodatečné X-posunutí objektu po zarovnání
         * @param   moveY       Dodatečné Y-posunutí objektu po zarovnání
         * @param   crop        Ořezový obdélník
         * @return  Pozice objektu
         */
        inline static SDL_Rect align(SDL_Surface* screen, Align _align, int objectW, int objectH, int moveX = 0, int moveY = 0, SDL_Rect* crop = NULL) {
            SDL_Rect area = {0, 0, (*screen).w, (*screen).h};
            return align(area, _align, objectW, objectH, moveX, moveY, crop);
        }

        /**
         * @brief Zarovnání objektu vůči displeji
         *
         * Souřadnice objektu ve struktuře SDL_Rect.
         * @param   screen      Displejová surface
         * @param   _align      Zarovnání objektu
         * @param   object      Objekt
         * @param   crop        Ořezový obdélník
         * @return  Pozice objektu
         */
        inline static SDL_Rect align(SDL_Surface* screen, Align _align, const SDL_Rect& object, SDL_Rect* crop = NULL) {
            SDL_Rect area = {0, 0, (*screen).w, (*screen).h};
            return align(area, _align, object.w, object.h, object.x, object.y, crop);
        }

        /**
         * @brief Zda se má vyhlazovat text
         *
         * Zde lze nastavit, zda se má vyhlazovat text. Třídy vypisující text
         * potom volají funkci Effects::textRenderFunction.
         */
        static bool smoothText;

        /**
         * @brief Ukazatel na funkci na vypsání textu
         *
         * Vysvětlivka: po dereferencování výsledku funkce dostaneme funkci s
         * parametry (...) vracející SDL_Surface.
         * @return Podle hodnoty Effects::smoothText funkce vrací ukazatel na
         *  funkci TTF_RenderUTF8_Solid nebo TTF_RenderUTF8_Blended
         */
         inline static SDL_Surface* (*textRenderFunction(void))(TTF_Font*, const char*, SDL_Color) {
            return Effects::smoothText ? TTF_RenderUTF8_Blended : TTF_RenderUTF8_Solid;
         }
};

}}

#endif
