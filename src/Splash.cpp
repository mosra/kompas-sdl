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

#include "Splash.h"

#include <iostream>

#include "Effects.h"

using namespace std;

namespace Kompas { namespace Sdl {

/* Zobrazení splashe */
void Splash::view(void){
    /* Vyplnění pozadí černou barvou */
    SDL_FillRect(screen, NULL, SDL_MapRGB((*screen).format, 0, 0, 0));

    /* Plocha pro vykreslování splashe */
    SDL_Rect area = Effects::align(screen, *align, *position);

    /* Zobrazení obrázku */
    SDL_BlitSurface(*image, NULL, screen, &area);

    /* Zobrazení textů */
    for(vector<Text>::const_iterator it = texts.begin(); it != texts.end(); ++it) {
        /* Oblast textu */
        SDL_Rect textArea = Effects::align(area, ALIGN_DEFAULT, *(*it).position);
        SDL_Surface* text = (*Effects::textRenderFunction())(*(*it).font, (*(*it).text).c_str(), *(*it).color);

        SDL_Rect dst = Effects::align(textArea, *(*it).align, (*text).w, (*text).h);
        SDL_Rect textCrop = {0, 0, dst.w, dst.h};
        SDL_BlitSurface(text, &textCrop, screen, &dst);
        SDL_FreeSurface(text);
    }
}

/* Přidání textu */
void Splash::addText (TTF_Font** font, SDL_Color* color, SDL_Rect* _position, Align* _align, std::string* text) {
    Splash::Text _text;
    _text.font = font;
    _text.color = color;
    _text.position = _position;
    _text.align = _align;
    _text.text = text;
    texts.push_back(_text);
}

}}
