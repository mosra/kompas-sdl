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
         * Zarovnání objektu známých rozměrů do dané oblasti
         * @param   areaW       Šířka oblasti
         * @param   areaH       Výška oblasti
         * @param   align       Zarovnání objektu
         * @param   objectW     Šířka objektu
         * @param   objectH     Výška objektu
         * @return  Pozice objektu
         */
        static SDL_Rect align(int areaW, int areaH, Align align, int objectW, int objectH);

        /**
         * Zarovnání objektu proti posunuté oblasti
         * @param   areaX       X-ová pozice oblasti
         * @param   areaY       Y-ová pozice oblasti
         * @param   areaW       Šířka oblasti
         * @param   areaH       Výška oblasti
         * @param   align       Zarovnání objektu
         * @param   objectW     Šířka objektu
         * @param   objectH     Výška objektu
         * @return  Pozice objektu
         */
        inline static SDL_Rect alignMoved(int areaX, int areaY, int areaW, int areaH, Align align, int objectW, int objectH) {
            SDL_Rect ret = Effects::align(areaW, areaH, align, objectW, objectH);
            ret.x += areaX; ret.y += areaY;
            return ret;
        }

        /**
         * Vložení objektu do oblasti
         * @param   area        Pozice oblasti
         * @param   objectX     X-ová pozice objektu
         * @param   objectY     Y-ová pozice objektu
         * @param   objectW     Šířka objektu
         * @param   objectH     Výška objektu
         * @return  Pozice objektu
         */
        inline static SDL_Rect move(SDL_Rect area, int objectX, int objectY, int objectW, int objectH) {
            area.x += objectX; area.y += objectY;
            area.w = objectW; area.h = objectH;
            return area;
        }
};

#endif