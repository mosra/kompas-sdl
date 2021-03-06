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

#include "FPS.h"

namespace Kompas { namespace Sdl {

/* Default limit FPS */
unsigned int FPS::limit = 100;
unsigned int FPS::lastFrameTime = 10;
unsigned int FPS::timer = 0;

/* Refresh FPS */
double FPS::refresh (void) {
    unsigned int frameTime = SDL_GetTicks() - timer;

    /* Můžem si dát pauzu, pokud máme limit */
    if(limit != 0 && frameTime < 1000/limit)
        SDL_Delay(1000/limit-frameTime);

    /* Aktualizace hodnot */
    lastFrameTime = SDL_GetTicks() - timer;
    timer = SDL_GetTicks();

    return ((double) 1000)/((double) lastFrameTime);
}

/* Pohyb objektu */
int FPS::move (unsigned int pps, FPS::Data* object) {
    /* Pokud je objekt ještě pozastaven, pohyb se nekoná */
    if(*object < 0) {
        if((unsigned int) (0-*object) < SDL_GetTicks()) *object = 0;
        else return 0;
    }

    /* Posun */
    unsigned int move = pps*lastFrameTime/1000;

    /* Přičtení zbytku (vynásobený 1000), jestli dal již dohromady pixel, inkrementace */
    if((*object += pps*lastFrameTime-(move*1000)) > 1000) {
        *object -= 1000;
        return ++move;
    }

    return move;
}

}}
