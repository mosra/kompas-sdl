#include "Effects.h"

/* Vyhlazení textu je defaultně zapnuto */
bool Effects::smoothText = true;

/* Zarovnání objektu */
SDL_Rect Effects::align (SDL_Rect area, Align _align, int objectW, int objectH, int moveX, int moveY) {
    /* Ořez objektu do velikosti rámce */
    if(area.w < objectW) objectW = area.w;
    if(area.h < objectH) objectH = area.h;

    /* Default je zarovnání vlevo nahoru */
    SDL_Rect ret = {area.x+moveX, area.y+moveY, objectW, objectH};

    if(_align & ALIGN_CENTER)    ret.x += (area.w-objectW)/2;
    if(_align & ALIGN_RIGHT)     ret.x += area.w-objectW;
    if(_align & ALIGN_MIDDLE)    ret.y += (area.h-objectH)/2;
    if(_align & ALIGN_BOTTOM)    ret.y += area.h-objectH;

    return ret;
}
