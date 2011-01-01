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

#include "Matrix.h"

#include <algorithm>    /* sort() */

using namespace std;

namespace Kompas { namespace Sdl {

/* Zakázání položky */
template<class Item> void Matrix<Item>::disableItem(itemId item) {
    items[item].flags |= DISABLED;
    reloadItems();
}

/* Povolení položky */
template<class Item> void Matrix<Item>::enableItem(itemId item) {
    items[item].flags &= ~DISABLED;
    reloadItems();
}

/* Obnovení položek */
template<class Item> void Matrix<Item>::reloadItems() {
    sortedHorizontal.clear();   sortedHorizontal.reserve(items.size());
    sortedVertical.clear();     sortedVertical.reserve(items.size());

    /* Naplnění tříděných jen aktivními položkami */
    for(typename vector<Item>::const_iterator it = items.begin(); it != items.end(); ++it) {
        if(!((*it).flags & DISABLED)) {
            sortedHorizontal.push_back(it);
            sortedVertical.push_back(it);
        }
    }

    sort(sortedHorizontal.begin(), sortedHorizontal.end(), horizontalCompare);
    sort(sortedVertical.begin(), sortedVertical.end(), verticalCompare);

    if(sortedVertical.size() != 0) actualItem = sortedVertical.front();
    else actualItem = items.end();
}

/* Posun nahoru */
template<class Item> bool Matrix<Item>::moveUp(void) {
    if(sortedHorizontal.size() == 0) return false;

    /* Nalezení aktuální položky */
    typename vector<typename vector<Item>::const_iterator>::const_iterator it = sortedHorizontal.begin();
    for( ; it != sortedHorizontal.end(); ++it) {
        if(*it == actualItem) break;
    }

    /* Hledání jiného řádku (nekonečné procházení, dokud se nevrátíme zpět na počáteční položku) */
    do {
        if(it-- == sortedHorizontal.begin()) it = sortedHorizontal.end()-1;
    } while(*it != actualItem && (**it).y == (*actualItem).y);

    /* Žádný jiný řádek neexistuje, jsme zpět na aktuální položce */
    if(*it == actualItem) return false;

    /* Hledání lepšího kandidáta v daném řádku (y) - blíže k x než předešlý */
    int y = (**it).y;   int x = (*actualItem).x;    actualItem = *it;
    while(it-- != sortedHorizontal.begin() && (**it).y == y) {

        /* Pokud je položka blíže aktuální než předešlý kandidát, přiřazení */
        if( ((**it).x <= x && (*actualItem).x < (**it).x) || /* vlevo od aktuální */
            ((**it).x >= x && (*actualItem).x > (**it).x)    /* vpravo od aktuální */
        ) actualItem = *it;

        /* Položka je dále než předešlý kandidát, konec */
        else break;
    }

    return true;
}

/* Posun dolů */
template<class Item> bool Matrix<Item>::moveDown(void) {
    if(sortedHorizontal.size() == 0) return false;

    /* Nalezení aktuální položky */
    typename vector<typename vector<Item>::const_iterator>::const_iterator it = sortedHorizontal.begin();
    for( ; it != sortedHorizontal.end(); ++it) {
        if(*it == actualItem) break;
    }

    /* Hledání jiného řádku (nekonečné procházení, dokud se nevrátíme zpět na počáteční položku) */
    do {
        if(++it == sortedHorizontal.end()) it = sortedHorizontal.begin();
    } while(*it != actualItem && (**it).y == (*actualItem).y);

    /* Žádný jiný řádek neexistuje, jsme zpět na aktuální položce */
    if(*it == actualItem) return false;

    /* Hledání lepšího kandidáta v daném řádku (y) - blíže k x než předešlý */
    int y = (**it).y;   int x = (*actualItem).x;    actualItem = *it;
    while(++it != sortedHorizontal.end() && (**it).y == y) {

        /* Pokud je položka blíže aktuální než předešlý kandidát, přiřazení */
        if( ((**it).x <= x && (*actualItem).x < (**it).x) || /* vlevo od aktuální */
            ((**it).x >= x && (*actualItem).x > (**it).x)    /* vpravo od aktuální */
        ) actualItem = *it;

        /* Položka je dále než předešlý kandidát, konec */
        else break;
    }

    return true;
}

/* Posun doleva */
template<class Item> bool Matrix<Item>::moveLeft(void) {
    if(sortedVertical.size() == 0) return false;

    /* Nalezení aktuální položky */
    typename vector<typename vector<Item>::const_iterator>::const_iterator it = sortedVertical.begin();
    for( ; it != sortedVertical.end(); ++it) {
        if(*it == actualItem) break;
    }

    /* Hledání jiného sloupce (nekonečné procházení, dokud se nevrátíme zpět na počáteční položku) */
    do {
        if(it-- == sortedVertical.begin()) it = sortedVertical.end()-1;
    } while(*it != actualItem && (**it).x == (*actualItem).x);

    /* Žádný jiný sloupec neexistuje, jsme zpět na aktuální položce */
    if(*it == actualItem) return false;

    /* Hledání lepšího kandidáta v daném sloupci (x) - blíže k y než předešlý */
    int x = (**it).x;   int y = (*actualItem).y;    actualItem = *it;
    while(it-- != sortedVertical.begin() && (**it).x == x) {

        /* Pokud je položka blíže aktuální než předešlý kandidát, přiřazení */
        if( ((**it).y <= y && (*actualItem).y < (**it).y) || /* výše než aktuální */
            ((**it).y >= y && (*actualItem).y > (**it).y)    /* níže než aktuální */
        ) actualItem = *it;

        /* Položka je dále než předešlý kandidát, konec */
        else break;
    }

    return true;
}

/* Posun doprava */
template<class Item> bool Matrix<Item>::moveRight(void) {
    if(sortedVertical.size() == 0) return false;

    /* Nalezení aktuální položky */
    typename vector<typename vector<Item>::const_iterator>::const_iterator it = sortedVertical.begin();
    for( ; it != sortedVertical.end(); ++it) {
        if(*it == actualItem) break;
    }

    /* Hledání jiného sloupce (nekonečné procházení, dokud se nevrátíme zpět na počáteční položku) */
    do {
        if(++it == sortedVertical.end()) it = sortedVertical.begin();
    } while(*it != actualItem && (**it).x == (*actualItem).x);

    /* Žádný jiný sloupec neexistuje, jsme zpět na aktuální položce */
    if(*it == actualItem) return false;

    /* Hledání lepšího kandidáta v daném sloupci (x) - blíže k y než předešlý */
    int x = (**it).x;   int y = (*actualItem).y;    actualItem = *it;
    while(++it != sortedVertical.end() && (**it).x == x) {

        /* Pokud je položka blíže aktuální než předešlý kandidát, přiřazení */
        if( ((**it).y <= y && (*actualItem).y < (**it).y) || /* výše než aktuální */
            ((**it).y >= y && (*actualItem).y > (**it).y)    /* níže než aktuální */
        ) actualItem = *it;

        /* Položka je dále než předešlý kandidát (už lepší nenajdeme), konec */
        else break;
    }

    return true;
}

}}
