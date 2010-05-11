#include "Effects.h"

using namespace MToolkit;

namespace MInterface {

/* Vyhlazení textu je defaultně zapnuto */
bool Effects::smoothText = true;

/* Zarovnání objektu */
SDL_Rect Effects::align (const SDL_Rect& area, Align _align, int objectW, int objectH, int moveX, int moveY, SDL_Rect* crop) {
    /* Ořezový obdélník */
    if(crop != NULL) {
        (*crop).x = 0;
        (*crop).y = 0;
    }

    /* Objekt má záporný posun - zmenšení velikosti objektu */
    if(moveX < 0) {
        objectW -= moveX;
        if(crop != NULL) (*crop).x = -moveX;
    }
    if(moveY < 0) {
        objectH -= moveY;
        if(crop != NULL) (*crop).y = -moveY;
    }

    /* Ořez objektu do velikosti rámce */
    if(area.w < objectW+moveX) objectW = area.w-moveX;
    if(area.h < objectH+moveY) objectH = area.h-moveY;

    /** @todo Kompletně celé přepsat */
    if(crop != NULL) {
        (*crop).w = objectW;
        (*crop).h = objectH;
    }

    /* Default je zarovnání vlevo nahoru */
    SDL_Rect ret = {area.x+moveX, area.y+moveY, objectW, objectH};

    /** @todo Při zarovnání doprava osekávat crop zleva, při zarovnání na střed
        osekávat z obou stran... */
    if(_align & ALIGN_CENTER)    ret.x += (area.w-objectW)/2;
    if(_align & ALIGN_RIGHT)     ret.x += area.w-objectW;
    if(_align & ALIGN_MIDDLE)    ret.y += (area.h-objectH)/2;
    if(_align & ALIGN_BOTTOM)    ret.y += area.h-objectH;

    return ret;
}

}
