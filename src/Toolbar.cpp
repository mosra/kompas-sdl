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
    findActualItem();
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

    /* Pokud jsme si zakázali aktuální položku, nalezení jiné */
    if(actualItem-items.begin() == item) findActualItem();
}

/* Povolení položky */
void Toolbar::enableItem(Toolbar::itemId item) {
    items[item].flags &= !DISABLED;

    /* Pokud je tato položka první povolená, označení této jako aktuální */
    if(actualItem == items.end()) actualItem = items.begin()+item;
}

/* Reload položek */
void Toolbar::reloadItems (void) {
    sortedHorizontal.clear();   sortedHorizontal.reserve(items.size());
    sortedVertical.clear();     sortedVertical.reserve(items.size());

    for(vector<Item>::const_iterator it = items.begin(); it != items.end(); ++it) {
        sortedHorizontal.push_back(it);
        sortedVertical.push_back(it);
    }

    sort(sortedHorizontal.begin(), sortedHorizontal.end(), horizontalCompare);
    sort(sortedVertical.begin(), sortedVertical.end(), verticalCompare);
}

/* Nalezení aktuální položky */
void Toolbar::findActualItem(void) {
    for(vector<Item>::const_iterator it = items.begin(); it != items.end(); ++it) {
        /* Pokud položka není zakázaná, označení jako aktuální */
        if(!((*it).flags & DISABLED)) {
            actualItem = it;
            return;
        }
    }

    /* Všechny položky zakázané */
    actualItem = items.end();
}

/* Posun doprava */
int Toolbar::moveRight(void) {
    /** @todo Kompletně předělat, tohle je příšernost! */

    /* Nalezení aktuální položky */
    vector<vector<Item>::const_iterator>::const_iterator actual = sortedVertical.end();
    for(vector<vector<Item>::const_iterator>::const_iterator it = sortedVertical.begin(); it != sortedVertical.end(); ++it) {
        if(*it == actualItem) {
            actual = it;
            break;
        }
    }

    /* Aktuální položka neexistuje */
    if(actual == sortedVertical.end()) return -1;

    /* Kandidát na položku posunutou vpravo */
    vector<Item>::const_iterator candidate = items.end();

    /* Hledání položky ve sloupci co nejblíže vpravo */
    vector<vector<Item>::const_iterator>::const_iterator it = actual+1;
    while(it != actual) {

        /* Dostali jsme se nakonec */
        if(it == sortedVertical.end()) it = sortedVertical.begin();

        /* Už máme kandidáta a jsme v dalším sloupci */
        if(candidate != items.end() && (**it).x != (*candidate).x) break;

        /* Nejsme ve sloupci aktuální položky a položka je povolená */
        if((**it).x != (*actualItem).x && !((**it).flags & DISABLED)) {

            /* Pokud je položka blíže aktuální než předešlý kandidát, přiřazení */
            if(candidate == items.end() ||
             ((**it).y <= (*actualItem).y && (*candidate).y < (**it).y) ||
             ((**it).y > (*actualItem).y && (*candidate).y > (**it).y)
            ) candidate = *it;

            /* Položka je dále než předešlý kandidát, konec */
            else break;
        }

        it++;
    }

    if(candidate != items.end()) actualItem = candidate;
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