#ifndef EFFECTS_H
#define EFFECTS_H

#include <SDL/SDL.h>

/**
 * Zarovnání
 */
enum Align {
    ALIGN_LEFT = 0x01,  ALIGN_CENTER = 0x02,    ALIGN_RIGHT  = 0x04,
    ALIGN_TOP  = 0x08,  ALIGN_MIDDLE = 0x10,    ALIGN_BOTTOM = 0x02
};

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