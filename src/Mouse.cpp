#include "Mouse.h"

namespace Map2X { namespace Sdl {

/* Zjištění, zda byl klik v oblasti */
bool Mouse::inArea(int x, int y, const SDL_Rect& area) {
    if((x >= area.x && x < area.x+area.w) &&
       (y >= area.y && y < area.y+area.h))
        return true;
    return false;
}

}}
