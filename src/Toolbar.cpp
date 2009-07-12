#include "Toolbar.h"

using std::vector;      using std::sort;    using std::cout;    using std::endl;

/* Nastavení pozice nadpisku */
void Toolbar::configureCaptionPlace (SDL_Rect* _position, Align* _align) {
    flags = (flags & !(CAPTION_BESIDE_ICON | CAPTION_UNDER_ICON)) | CAPTION_IN_PLACE;
    captionPosition = _position;
    captionAlign = _align;
}

/* Přidání obrázku do toolbaru */
void Toolbar::addImage (SDL_Rect* _position, SDL_Surface** _image) {
    /* Pokud je pozice NULL, bude obrázek na pozici toolbaru */
    if(_position == NULL) _position = position;
    Image image;
    image.position = _position;
    image.image = _image;
    images.push_back(image);
}

/* Přidání položky do toolbaru */
Toolbar::itemId Toolbar::addItem (SDL_Rect* _position, int x, int y, int action, SDL_Surface** icon, SDL_Surface** activeIcon, SDL_Surface** disabledIcon, std::string* caption, int flags) {
    Item item;
    item.position = _position;
    item.x = x;
    item.y = y;
    item.action = action;
    item.icon = icon;
    item.activeIcon = activeIcon;
    item.disabledIcon = disabledIcon;
    item.caption = caption;
    item.flags = flags;
    items.push_back(item);

    reloadItems();
    return items.size()-1;
}

/* Přidání textu do toolbaru */
void Toolbar::addText(SDL_Rect* _position, Align* _align, TTF_Font** font, SDL_Color* color, std::string* _text) {
    Text text;
    text.position = _position;
    text.align = _align;
    text.font = font;
    text.color = color;
    text.text = _text;
    texts.push_back(text);
}

/* Zakázání položky */
void Toolbar::disableItem(Toolbar::itemId item) {
    items[item].flags |= DISABLED;
    reloadItems();
}

/* Povolení položky */
void Toolbar::enableItem(Toolbar::itemId item) {
    items[item].flags &= !DISABLED;
    reloadItems();
}

/* Reload položek */
void Toolbar::reloadItems (void) {
    sortedHorizontal.clear();   sortedHorizontal.reserve(items.size());
    sortedVertical.clear();     sortedVertical.reserve(items.size());

    /* Naplnění tříděných jen aktivními položkami */
    for(vector<Item>::const_iterator it = items.begin(); it != items.end(); ++it) {
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

/* Posun dolů */
int Toolbar::moveDown(void) {
    if(sortedHorizontal.size() == 0) return -1;

    /* Nalezení aktuální položky */
    vector<vector<Item>::const_iterator>::const_iterator it = sortedHorizontal.begin();
    for( ; it != sortedHorizontal.end(); ++it) {
        if(*it == actualItem) break;
    }

    /* Hledání jiného řádku (nekonečné procházení, dokud se nevrátíme zpět na počáteční položku) */
    do {
        if(++it == sortedHorizontal.end()) it = sortedHorizontal.begin();
    } while(*it != actualItem && (**it).y == (*actualItem).y);

    /* Žádný jiný řádek neexistuje, jsme zpět na aktuální položce */
    if(*it == actualItem) return (*actualItem).action;

    /* Hledání lepšího kandidáta v daném řádku (y) - blíže k x než předešlý */
    int y = (**it).y;   int x = (*actualItem).x;    actualItem = *it;
    while(++it != sortedHorizontal.end() && (**it).y == y) {

        /* Pokud je položka blíže aktuální než předešlý kandidát, přiřazení */
        if( ((**it).x < x && (*actualItem).x < (**it).x) || /* vlevo od aktuální */
            ((**it).x > x && (*actualItem).x > (**it).x)    /* vpravo od aktuální */
        ) actualItem = *it;

        /* Položka je dále než předešlý kandidát, konec */
        else break;
    }

    return (*actualItem).action;
}

/* Posun nahoru */
int Toolbar::moveUp(void) {
    if(sortedHorizontal.size() == 0) return -1;

    /* Nalezení aktuální položky */
    vector<vector<Item>::const_iterator>::const_iterator it = sortedHorizontal.begin();
    for( ; it != sortedHorizontal.end(); ++it) {
        if(*it == actualItem) break;
    }

    /* Hledání jiného řádku (nekonečné procházení, dokud se nevrátíme zpět na počáteční položku) */
    do {
        if(it-- == sortedHorizontal.begin()) it = sortedHorizontal.end()-1;
    } while(*it != actualItem && (**it).y == (*actualItem).y);

    /* Žádný jiný řádek neexistuje, jsme zpět na aktuální položce */
    if(*it == actualItem) return (*actualItem).action;

    /* Hledání lepšího kandidáta v daném řádku (y) - blíže k x než předešlý */
    int y = (**it).y;   int x = (*actualItem).x;    actualItem = *it;
    while(it-- != sortedHorizontal.begin() && (**it).y == y) {

        /* Pokud je položka blíže aktuální než předešlý kandidát, přiřazení */
        if( ((**it).x < x && (*actualItem).x < (**it).x) || /* vlevo od aktuální */
            ((**it).x > x && (*actualItem).x > (**it).x)    /* vpravo od aktuální */
        ) actualItem = *it;

        /* Položka je dále než předešlý kandidát, konec */
        else break;
    }

    return (*actualItem).action;
}

/* Posun doleva */
int Toolbar::moveLeft(void) {
    if(sortedVertical.size() == 0) return -1;

    /* Nalezení aktuální položky */
    vector<vector<Item>::const_iterator>::const_iterator it = sortedVertical.begin();
    for( ; it != sortedVertical.end(); ++it) {
        if(*it == actualItem) break;
    }

    /* Hledání jiného sloupce (nekonečné procházení, dokud se nevrátíme zpět na počáteční položku) */
    do {
        if(it-- == sortedVertical.begin()) it = sortedVertical.end()-1;
    } while(*it != actualItem && (**it).x == (*actualItem).x);

    /* Žádný jiný sloupec neexistuje, jsme zpět na aktuální položce */
    if(*it == actualItem) return (*actualItem).action;

    /* Hledání lepšího kandidáta v daném sloupci (x) - blíže k y než předešlý */
    int x = (**it).x;   int y = (*actualItem).y;    actualItem = *it;
    while(it-- != sortedVertical.begin() && (**it).x == x) {

        /* Pokud je položka blíže aktuální než předešlý kandidát, přiřazení */
        if( ((**it).y < y && (*actualItem).y < (**it).y) || /* výše než aktuální */
            ((**it).y > y && (*actualItem).y > (**it).y)    /* níže než aktuální */
        ) actualItem = *it;

        /* Položka je dále než předešlý kandidát, konec */
        else break;
    }

    return (*actualItem).action;
}

/* Posun doprava */
int Toolbar::moveRight(void) {
    if(sortedVertical.size() == 0) return -1;

    /* Nalezení aktuální položky */
    vector<vector<Item>::const_iterator>::const_iterator it = sortedVertical.begin();
    for( ; it != sortedVertical.end(); ++it) {
        if(*it == actualItem) break;
    }

    /* Hledání jiného sloupce (nekonečné procházení, dokud se nevrátíme zpět na počáteční položku) */
    do {
        if(++it == sortedVertical.end()) it = sortedVertical.begin();
    } while(*it != actualItem && (**it).x == (*actualItem).x);

    /* Žádný jiný sloupec neexistuje, jsme zpět na aktuální položce */
    if(*it == actualItem) return (*actualItem).action;

    /* Hledání lepšího kandidáta v daném sloupci (x) - blíže k y než předešlý */
    int x = (**it).x;   int y = (*actualItem).y;    actualItem = *it;
    while(++it != sortedVertical.end() && (**it).x == x) {

        /* Pokud je položka blíže aktuální než předešlý kandidát, přiřazení */
        if( ((**it).y < y && (*actualItem).y < (**it).y) || /* výše než aktuální */
            ((**it).y > y && (*actualItem).y > (**it).y)    /* níže než aktuální */
        ) actualItem = *it;

        /* Položka je dále než předešlý kandidát (už lepší nenajdeme), konec */
        else break;
    }

    return (*actualItem).action;
}

/* Zobrazení toolbaru */
void Toolbar::view (void) {
    /* Pozice toolbaru */
    SDL_Rect area = Effects::align(screen, *align, *position);

    /* Obrázky */
    for(vector<Image>::const_iterator it = images.begin(); it != images.end(); ++it) {
        SDL_Rect imageArea = Effects::align(area, ALIGN_DEFAULT, *(*it).position);
        SDL_Rect imageCrop = {0, 0, imageArea.w, imageArea.y};
        SDL_BlitSurface(*(*it).image, &imageCrop, screen, &imageArea);
    }

    /* Položky toolbaru */
    for(vector<Item>::const_iterator it = items.begin(); it != items.end(); ++it) {
        SDL_Rect itemPosition = Effects::align(area, ALIGN_DEFAULT, *(*it).position);

        /* Ukazatel na správnou ikonu */
        SDL_Surface** icon;
        if((*it).flags & DISABLED) icon = (*it).disabledIcon;
        else if(actualItem == it) icon = (*it).activeIcon;
        else icon = (*it).icon;

        /* Pokud jsou povoleny ikony */
        if(!(flags & NO_ICON)) {
            SDL_Rect iconPosition;

            /* Nadpisek pod ikonou (zachování horizontálního zarovnání pro ikonu) */
            if(flags & CAPTION_UNDER_ICON) {
                iconPosition = Effects::align(itemPosition, (Align) (*itemAlign & 0x0F), (**icon).w, (**icon).h);

                /* Od prostoru položky odečtení výšky ikony */
                itemPosition.y += *iconSize; itemPosition.h -= *iconSize;
            }

            /* Nadpisek vedle ikony (zachování vertikálního zarovnání pro ikonu) */
            else if(flags & CAPTION_BESIDE_ICON) {
                iconPosition = Effects::align(itemPosition, (Align) (*itemAlign & 0xF0), (**icon).w, (**icon).h);

                /* Od prostoru položky oddečtení šířky ikony */
                itemPosition.x += *iconSize; itemPosition.w -= *iconSize;
            }

            /* Žádný nadpisek nebo nadpisek na centrálním místě */
            else iconPosition = Effects::align(itemPosition, *itemAlign, (**icon).w, (**icon).h);

            SDL_Rect iconCrop = {0, 0, iconPosition.w, iconPosition.h};
            SDL_BlitSurface(*icon, &iconCrop, screen, &iconPosition);
        }

        /* Vypsání textu položky. Pokud je povolena ikona, souřadnice byly upraveny dříve. */
        if(flags & (NO_ICON | CAPTION_BESIDE_ICON | CAPTION_UNDER_ICON) || (flags & CAPTION_IN_PLACE && it == actualItem)) {
            SDL_Surface* text = (*Effects::textRenderFunction())(*captionFont, (*(*it).caption).c_str(), *captionColor);
            SDL_Rect textPosition;

            /* Popisek na centralizovaném místě */
            if(flags & CAPTION_IN_PLACE) {
                textPosition = Effects::align(
                    Effects::align(area, ALIGN_DEFAULT, *captionPosition),
                    *captionAlign, (*text).w, (*text).h
                );
            }

            /* Popisek u ikony */
            else
                textPosition = Effects::align(itemPosition, *itemAlign, (*text).w, (*text).h);

            SDL_Rect textCrop = {0, 0, textPosition.w, textPosition.h};
            SDL_BlitSurface(text, &textCrop, screen, &textPosition);
            SDL_FreeSurface(text);
        }
    }

    /* Text toolbaru */
    for(vector<Text>::const_iterator it = texts.begin(); it != texts.end(); ++it) {
        SDL_Surface* text = (*Effects::textRenderFunction())(*(*it).font, (*(*it).text).c_str(), *(*it).color);
        SDL_Rect textPosition = Effects::align(*(*it).position, *(*it).align, (*text).w, (*text).h);
        SDL_Rect textCrop = {0, 0, textPosition.w, textPosition.h};
        SDL_BlitSurface(text, &textCrop, screen, &textPosition);
        SDL_FreeSurface(text);
    }
}