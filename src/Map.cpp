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

#include "Map.h"

#include <iostream>
#include <sstream>

#include "Effects.h"
#include "utility.h"

using namespace std;

namespace Kompas { namespace Sdl {

/* Konstruktor */
Map::Map(SDL_Surface* _screen, SDL_Surface** _tileLoading, SDL_Surface** _tileNotFound):
screen(_screen), tileLoading(_tileLoading), tileNotFound(_tileNotFound), tileW(256),
tileH(256), tileMatrixW(0), tileMatrixH(0), beginX(0), beginY(0), endX(0xFFFFFFFF),
endY(0xFFFFFFFF), moveX(0), moveY(0) {
    resizeMatrix();
    loadTiles();
}

/* Změna velikosti matice */
bool Map::resizeMatrix(void) {
    vector<Tile>::size_type oldTileMatrixW = tileMatrixW,
                            oldTileMatrixH = tileMatrixH;
    tileMatrixW = (*screen).w/tileW + ((*screen).w%tileW > 1 ? 2 : 1);
    tileMatrixH = (*screen).h/tileH + ((*screen).h%tileH > 1 ? 2 : 1);

    if(tileMatrixH == oldTileMatrixH && tileMatrixW == oldTileMatrixW) return false;

    /* Rezervace kapacity pro zvětšení vektoru */
    if(tileMatrixH*tileMatrixW > oldTileMatrixH*oldTileMatrixW)
        tiles.reserve(tileMatrixH*tileMatrixW);

    /* Pokud máme prázdno, normální naplnění */
    if(tiles.empty()) {
        for(vector<Tile>::size_type row = 0; row != tileMatrixH; ++row) {
            for(vector<Tile>::size_type col = 0; col != tileMatrixW; ++col) {
                Tile tile;
                tile.x = 100+col;
                tile.y = 100+row;
                tile.isLoaded = false;
                tiles.push_back(tile);
            }
        }

        cout << tiles.size() << endl;

        return true;
    }

    /* Přidávání chybějících sloupců doprava (zatím zachováváme starou výšku) */
    if(oldTileMatrixW < tileMatrixW) {
        for(vector<Tile>::size_type row = 0; row != oldTileMatrixH; ++row) {
            for(vector<Tile>::size_type col = oldTileMatrixW; col != tileMatrixW; ++col) {
                /* Pozice: všechny řádky nad tímto již mají plný počet položek,
                   proto se násobí novým (větším) počtem položek */
                vector<Tile>::iterator position = tiles.begin()+row*tileMatrixW+col;

                /* Zjištění souřadnic z dlaždice vlevo od aktuální */
                Tile tile;
                tile.x = (*(position-1)).x+1;
                tile.y = (*(position-1)).y;
                tile.isLoaded = false;

                /* Přidáváme PŘED pozici */
                tiles.insert(position, tile);
            }
        }

    /* Odebírání přebývajících sloupců zprava (pokud nějaké přebývají) */
    } else if(oldTileMatrixW != tileMatrixW) {
        for(vector<Tile>::size_type row = 0; row != oldTileMatrixH; ++row) {
            /* Ve všech předchozích řádcích jsou již odstraněny sloupce, proto
               je pozice počítána z nové šířky řádku */
            tiles.erase(tiles.begin()+row*tileMatrixW+tileMatrixW,
                        tiles.begin()+row*tileMatrixW+oldTileMatrixW);
        }
    }

    /* Přidávání chybějících řádků dolů */
    if(oldTileMatrixH < tileMatrixH) {
        /* Počáteční pozice matice */
        unsigned int x = tiles.front().x,
                     y = tiles.front().y;

        for(vector<Tile>::size_type row = oldTileMatrixH; row != tileMatrixH; ++row) {
            for(vector<Tile>::size_type col = 0; col != tileMatrixW; ++col) {
                Tile tile;
                tile.x = x+col;
                tile.y = y+row;
                tile.isLoaded = false;
                tiles.push_back(tile);
            }
        }

    /* Odstranění přebývajícíh řádků dole */
    } else if(oldTileMatrixH != tileMatrixH)
        tiles.erase(tiles.begin()+tileMatrixW*tileMatrixH, tiles.end());

    /* Kontrola, jestli je vše ok */
    /** @todo Po debugu tohle smazat! */
    if(tileMatrixH*tileMatrixW != tiles.size())
        cerr << "Chyba: množství dlaždic je " << tiles.size() << " místo očekávaných "
             << tileMatrixW << "x" << tileMatrixH << "!" << endl;

    cout << tiles.size() << endl;

    return true;
}

/* Načtení dlaždic */
void Map::loadTiles(void) {
    for(vector<Tile>::iterator it = tiles.begin(); it != tiles.end(); ++it) {
        if(!(*it).isLoaded) {
            /** @todo A co takhle freeSurface? */
            (*it).image = tileNotFound;
            (*it).isLoaded = true;
        }
    }
}

/* Posunutí mapy nahoru */
void Map::moveUp(unsigned int pixels) {
    /* Posun mimo načtenou oblast mapy = přidání dalšího řádku dlaždic nahoru */
    if(pixels > moveY) {
        unsigned int x = tiles.front().x,
                     y = tiles.front().y;

        /* Už jsme na hranici mapy */
        if(y-- == beginY) { moveY = 0; return; }

        /* Smazání posledního řádku dole */
        tiles.erase(tiles.end()-tileMatrixW, tiles.end());

        /* Naplnění nového řádku nahoře */
        vector<Tile>::iterator insertPos = tiles.begin();
        for(unsigned int pos = x; pos != x+tileMatrixW; ++pos) {
            Tile tile;
            tile.x = pos;
            tile.y = y;
            tile.isLoaded = false;
            insertPos = ++tiles.insert(insertPos, tile);
        }

        loadTiles();

        pixels -= moveY; moveY = tileH;
        return moveUp(pixels);
    }

    moveY -= pixels;
}

/* Posunutí mapy dolů */
void Map::moveDown(unsigned int pixels) {
    /* Posun mimo načtenou oblast mapy = přidání dalšího řádku dlaždic dolů */
    if(tileMatrixH*tileH-(pixels+moveY) < (unsigned int) (*screen).h) {
        unsigned int x = tiles.front().x,
                     y = tiles.back().y;

        /* Už jsme na hranici mapy */
        if(++y == endY) { moveY = tileMatrixH*tileH-(*screen).h; return; }

        /* Smazání prvního řádku */
        tiles.erase(tiles.begin(), tiles.begin()+tileMatrixW);

        /* Naplnění nového řádku dole */
        for(unsigned int pos = x; pos != x+tileMatrixW; ++pos) {
            Tile tile;
            tile.x = pos;
            tile.y = y;
            tile.isLoaded = false;
            tiles.push_back(tile);
        }

        loadTiles();
        pixels -= tileMatrixH*tileH-((unsigned int) (*screen).h+moveY);
        /** @bug To mě přivádí k ověření důkazu, jestli je tileMatrixH*tileH
            vždy alespoň o tileH větší než (*screen).h? Při sudém rozměru screen
            by to mělo být dodrženo, při lichém ne */
        moveY -= tileH;
        return moveDown(pixels);
    }

    moveY += pixels;
}

/* Posunutí mapy doleva */
void Map::moveLeft(unsigned int pixels) {
    /* Posun mimo oblast mapy - přidání dalšího řádku dlaždic doleva */
    if(pixels > moveX) {

        /* Už jsme na hranici mapy */
        if(tiles.front().x == beginX) { moveX = 0; return; }

        /* Nyní se nemusí složitě mazat žádný sloupec, na začátek se přidá
           položka, která bude představovat první dlaždici nového sloupce. Tím
           se každá dlaždice na konci řádku stane dlaždicí na začátku řádku
           dalšího - takže stačí už jen přepsat souřadnice. Poslední dlaždice
           v poli se smaže, protože je zde navíc (nemá už řádek, do kterého by
           se přesunula). */

        /* Smazání poslední a přidání první položky */
        tiles.erase(tiles.end()-1);
        Tile tile;
        tiles.insert(tiles.begin(), tile);

        /* Přepisování souřadnic */
        for(vector<Tile>::iterator it = tiles.begin(); it != tiles.end(); ++it) {
            (*it).x = (*(it+1)).x-1;
            (*it).y = (*(it+1)).y;
            (*it).isLoaded = false;
        }

        loadTiles();

        pixels -= moveX; moveX = tileW;
        return moveLeft(pixels);
    }

    moveX -= pixels;
}

/* Posunutí mapy doprava */
void Map::moveRight(unsigned int pixels) {
    /* Posun mimo oblast mapy - přidání dalšího řádku dlaždic doleva */
    if(tileMatrixW*tileW-(pixels+moveX) < (unsigned int) (*screen).w) {

        /* Už jsme na hranici mapy */
        if(tiles.back().x == endX-1) { moveX = tileMatrixW*tileW-(*screen).w; return; }

        /* Nyní se nemusí složitě mazat žádný sloupec, na konec se přidá
        položka, která bude představovat první dlaždici nového sloupce. Tím
        se každá dlaždice na začátku řádku stane dlaždicí na konci řádku
        předchozího - takže stačí už jen přepsat souřadnice. První dlaždice
        v poli se smaže, protože je zde navíc (nemá už řádek, do kterého by
        se přesunula). */

        /* Smazání první a přidání poslední položky */
        tiles.erase(tiles.begin());
        Tile tile;
        tiles.push_back(tile);

        /* Přepisování souřadnic */
        for(vector<Tile>::iterator it = tiles.begin()+tileMatrixW-1; it != tiles.end(); ++it) {
            (*it).x = (*(it-1)).x+1;
            (*it).y = (*(it-1)).y;
            (*it).isLoaded = false;
        }

        loadTiles();

        pixels -= tileMatrixW*tileW-((unsigned int) (*screen).w+moveX);
        /** @bug To mě přivádí k ověření důkazu, jestli je tileMatrixW*tileW
        vždy alespoň o tileW větší než (*screen).w? Při sudém rozměru screen
        by to mělo být dodrženo, při lichém ne */
        moveX -= tileW;
        return moveLeft(pixels);
    }

    moveX += pixels;
}

/* Zobrazení mapy */
void Map::view(TTF_Font** font, SDL_Color* color) {
    /* Co kdyby náhodou někdo změnil velilkost okna */
    if(resizeMatrix()) loadTiles();

    /* Počáteční x a y souřadnice (předpokládá, že je vektor správně seřazený) */
    unsigned int x = tiles.front().x;
    unsigned int y = tiles.front().y;

    /* Zobrazování jednotlivých dlaždic */
    for (vector<Tile>::const_iterator it = tiles.begin(); it != tiles.end(); ++it) {
        SDL_Rect tileCrop;
        SDL_Rect tilePosition = Effects::align(screen, ALIGN_DEFAULT, tileW, tileH,
            ((*it).x-x)*tileW-moveX, ((*it).y-y)*tileH-moveY, &tileCrop);

        SDL_BlitSurface(*(*it).image, &tileCrop, screen, &tilePosition);

        /* Text */
        /** <<< debug */
        std::ostringstream title;
        title << "[" << (*it).x << ":" << (*it).y << "]";
        SDL_Surface* text = (*Effects::textRenderFunction())(*font, title.str().c_str(), *color);
        tilePosition = Effects::align(tilePosition, (Align) (ALIGN_CENTER|ALIGN_MIDDLE), (*text).w, (*text).h, 0, 64, &tileCrop);
        SDL_BlitSurface(text, &tileCrop, screen, &tilePosition);
        SDL_FreeSurface(text);
        /** >>> debug */
    }
}

}}
