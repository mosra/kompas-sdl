#ifndef EFFECTS_H
#define EFFECTS_H

#include <SDL/SDL.h>
#include "utility.h"

/**
 * Funkce pro zarovnávání, posouvání textu a další fičurky
 */
class Effects {
    public:
        /**
         * Zarovnání objektu do dané oblasti
         * @param   area        Oblast
         * @param   _align      Zarovnání objektu
         * @param   objectW     Šířka objektu
         * @param   objectH     Výška objektu
         * @param   moveX       Dodatečné X-posunutí objektu po zarovnání
         * @param   moveY       Dodatečné Y-posunutí objektu po zarovnání
         * @return  Pozice objektu
         */
        static SDL_Rect align(SDL_Rect area, Align _align, int objectW, int objectH, int moveX = 0, int moveY = 0);

        /**
         * Zarovnání objektu vůči displeji
         * @param   screen      Displejová surface
         * @param   _align      Zarovnání objektu
         * @param   objectW     Šířka objektu
         * @param   objectH     Výška objektu
         * @param   moveX       Dodatečné X-posunutí objektu po zarovnání
         * @param   moveY       Dodatečné Y-posunutí objektu po zarovnání
         * @return  Pozice objektu
         */
        inline static SDL_Rect align(SDL_Surface* screen, Align _align, int objectW, int objectH, int moveX = 0, int moveY = 0) {
            SDL_Rect area = {0, 0, (*screen).w, (*screen).h};
            return align(area, _align, objectW, objectH, moveX, moveY);
        }
};

#endif