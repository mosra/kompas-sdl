#ifndef EFFECTS_H
#define EFFECTS_H

/**
 * @file Effects.h
 * @brief Třída Effects
 */

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "utility.h"

namespace MInterface {

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
         * Zarovná objekt do oblasti a ořízne jej, aby nepřesahoval hranice
         * oblasti. Z výsledku se šířka a výška dá poté použít k ořezu.
         * @param   area        Oblast
         * @param   _align      Zarovnání objektu
         * @param   objectW     Šířka objektu
         * @param   objectH     Výška objektu
         * @param   moveX       Dodatečné X-posunutí objektu po zarovnání
         * @param   moveY       Dodatečné Y-posunutí objektu po zarovnání
         * @return  Pozice objektu
         */
        static SDL_Rect align(const SDL_Rect& area, MToolkit::Align _align, int objectW, int objectH, int moveX = 0, int moveY = 0);

        /**
         * @brief Zarovnání objektu do dané oblasti
         *
         * Souřadnice objektu ve struktuře SDL_Rect.
         * @param   area        Oblast
         * @param   _align      Zarovnání objektu
         * @param   object      Objekt
         * @return  Pozice objektu
         */
        inline static SDL_Rect align(const SDL_Rect& area, MToolkit::Align _align, const SDL_Rect& object) {
            return align(area, _align, object.w, object.h, object.x, object.y);
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
         * @return  Pozice objektu
         */
        inline static SDL_Rect align(SDL_Surface* screen, MToolkit::Align _align, int objectW, int objectH, int moveX = 0, int moveY = 0) {
            SDL_Rect area = {0, 0, (*screen).w, (*screen).h};
            return align(area, _align, objectW, objectH, moveX, moveY);
        }

        /**
         * @brief Zarovnání objektu vůči displeji
         *
         * Souřadnice objektu ve struktuře SDL_Rect.
         * @param   screen      Displejová surface
         * @param   _align      Zarovnání objektu
         * @param   object      Objekt
         * @return  Pozice objektu
         */
        inline static SDL_Rect align(SDL_Surface* screen, MToolkit::Align _align, const SDL_Rect& object) {
            SDL_Rect area = {0, 0, (*screen).w, (*screen).h};
            return align(area, _align, object.w, object.h, object.x, object.y);
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

}

#endif
