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

#include "Mouse.h"

namespace Kompas { namespace Sdl {

/* Zjištění, zda byl klik v oblasti */
bool Mouse::inArea(int x, int y, const SDL_Rect& area) {
    if((x >= area.x && x < area.x+area.w) &&
       (y >= area.y && y < area.y+area.h))
        return true;
    return false;
}

}}
