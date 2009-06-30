#include "Effects.h"

#include <SDL/SDL.h>
#include "utility.h"

/* Zarovnání objektu */
SDL_Rect Effects::align (int areaW, int areaH, Align align, int objectW, int objectH) {
    /* Default je zarovnání vlevo nahoru */
    SDL_Rect ret = {0, 0, objectW, objectH};

    if(align & ALIGN_CENTER)    ret.x = (areaW-objectW)/2;
    if(align & ALIGN_RIGHT)     ret.x = areaW-objectW;
    if(align & ALIGN_MIDDLE)    ret.y = (areaH-objectH)/2;
    if(align & ALIGN_BOTTOM)    ret.y = areaH-objectH;

    return ret;
}