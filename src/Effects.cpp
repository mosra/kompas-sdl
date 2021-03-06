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

#include "Effects.h"

namespace Kompas { namespace Sdl {

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

}}
