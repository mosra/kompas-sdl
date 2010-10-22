#include "Toolbar.h"

#include <iostream>

#include "Effects.h"
#include "Matrix.cpp"

using std::vector;      using std::cout;    using std::endl;

namespace Map2X { namespace Sdl {

#ifndef GENERATING_DOXYGEN_OUTPUT
/* Předdefinování určitě používaných template, aby linker neházel chyby o tom,
    že v knihovně taková template nejsou instancovaná */
template class Matrix<ToolbarItem>;
#endif

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
    ToolbarItem item;
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

/* Posun nahoru */
int Toolbar::moveUp(void) {
    if(Matrix<ToolbarItem>::moveUp()) return (*actualItem).action;
    else return -1;
}

/* Posun dolů */
int Toolbar::moveDown(void) {
    if(Matrix<ToolbarItem>::moveDown()) return (*actualItem).action;
    else return -1;
}

/* Posun doleva */
int Toolbar::moveLeft(void) {
    if(Matrix<ToolbarItem>::moveLeft()) return (*actualItem).action;
    else return -1;
}

/* Posun doprava */
int Toolbar::moveRight(void) {
    if(Matrix<ToolbarItem>::moveRight()) return (*actualItem).action;
    else return -1;
}

/* Kliknutí */
bool Toolbar::click(int x, int y, int& action) {
    /* Pokud je toolbar schovaný, konec */
    if(flags & HIDDEN) return false;

    /* Oblast toolbaru */
    SDL_Rect area = Effects::align(screen, *align, *position);

    if(!inArea(x, y, area)) return false;

    /* Procházení jednotlivých ikon */
    for(vector<ToolbarItem>::const_iterator it = items.begin(); it != items.end(); ++it) {
        if(!((*it).flags & DISABLED) && inArea(x, y, Effects::align(area, ALIGN_DEFAULT, *(*it).position))) {
            actualItem = it;
            action = select();
            return true;
        }
    }

    return true;
}

/* Zobrazení toolbaru */
void Toolbar::view (void) {
    /* Pokud je toolbar schovaný, konec */
    if(flags & HIDDEN) return;

    /* Pozice toolbaru */
    SDL_Rect area = Effects::align(screen, *align, *position);

    /* Obrázky */
    for(vector<Image>::const_iterator it = images.begin(); it != images.end(); ++it) {
        SDL_Rect imageArea = Effects::align(area, ALIGN_DEFAULT, *(*it).position);
        SDL_Rect imageCrop = {0, 0, imageArea.w, imageArea.y};
        SDL_BlitSurface(*(*it).image, &imageCrop, screen, &imageArea);
    }

    /* Položky toolbaru */
    for(vector<ToolbarItem>::const_iterator it = items.begin(); it != items.end(); ++it) {
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

}}
