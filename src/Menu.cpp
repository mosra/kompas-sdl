#include "Menu.h"

using std::cout;    using std::endl;        using std::vector;

/* Nastavení nadpisku */
void Menu::configureCaption (int* _x, int* _y, int* _w, int* _h, Align* align, _TTF_Font** font, SDL_Color* color) {
    captionX = _x;  captionY = _y;
    captionW = _w;  captionH = _h;
    captionAlign = align;
    captionFont = font;
    captionColor = color;

    /* Povolení ve flags */
    flags |= CAPTION;
}

/* Vytvoření sekce menu */
Menu::sectionId Menu::addSection (Menu::sectionId parent, std::string* caption, Align* iconAlign, Align* itemsAlign, int flags) {
    Menu::Section section;
    section.parent = parent;
    section.caption = caption;
    section.iconAlign = iconAlign;
    section.itemsAlign = itemsAlign;
    section.flags = flags;
    sections.push_back(section);

    reloadIterators();

    return sections.size()-1;
}

/* Přidání položky do menu */
Menu::itemId Menu::addItem (Menu::sectionId section, int action, std::string* caption, SDL_Surface** icon, int flags) {
    Menu::Item item;
    item.action = action;
    item.caption = caption;
    item.icon = icon;
    item.flags = flags;
    sections[section].items.push_back(item);

    reloadIterators(section);

    return sections[section].items.size()-1;
}

/* Reload iterátorů */
void Menu::reloadIterators (int section) {
    /* Reloud jen u jedný sekce (při změně počtu položek menu) */
    if(section != -1) {
        sections[section].first = sections[section].items.begin();
        sections[section].actualItem = sections[section].items.begin();
        return;
    }

    /* Reloud u všech sekcí (při přidání sekce) */
    for(vector<Section>::iterator it = sections.begin(); it != sections.end(); ++it) {
        (*it).first = (*it).items.begin();
        (*it).actualItem = (*it).items.begin();
    }

    actualSection = sections.end()-1;
}

/* Zobrazení menu */
void Menu::view (void) {
    /* Plocha pro vykreslování menu */
    SDL_Rect area = Effects::align(screen, *menuAlign, *w, *h, *x, *y);

    /* Pozadí */
    SDL_BlitSurface(*image, NULL, screen, &area);

    /* Nadpisek menu */
    if(flags & CAPTION) {
        /* Prostor pro napisek */
        SDL_Rect captionPosition = Effects::align(area, ALIGN_DEFAULT, *captionW, *captionH, *captionX, *captionY);
        SDL_Surface* text = (*Effects::textRenderFunction())(*captionFont, (*(*actualSection).caption).c_str(), *captionColor);

        /* Přesná pozice nadpisku, ořezání a vykreslení */
        captionPosition = Effects::align(captionPosition, *captionAlign, (*text).w, (*text).h);
        SDL_Rect captionCrop = {0, 0, captionPosition.w, captionPosition.h};
        SDL_BlitSurface(text, &captionCrop, screen, &captionPosition);
    }

    /** @todo Posuvníky */

    /* Mezera mezi položkami, pokud není striktně definovaná. */
    int _itemHeight = *itemHeight;
    if(_itemHeight == 0) _itemHeight = TTF_FontLineSkip(*itemFont);

    /* Počet položek na stránku (spočítání koncového iterétoru) */
    vector<Item>::const_iterator end;
    if((*actualSection).items.end()-(*actualSection).first < *itemsH/_itemHeight)
        end = (*actualSection).items.end();
    else
        end = (*actualSection).first+(*itemsH/_itemHeight)+1;

    /* Plocha pro vykreslení položek. Prvně zarování celé plochy položek, až v ní
       se podle počtu položek na stránku a vertikálního zarovnání (&0xF0) vypočítá
       konečná plocha položek */
    SDL_Rect itemArea = Effects::align(
        Effects::align(area, ALIGN_DEFAULT, *itemsW, *itemsH, *itemsX, *itemsY),
        (Align) (*(*actualSection).itemsAlign & 0xF0), *itemsW, _itemHeight*(end-(*actualSection).first)
    );
    itemArea.h = _itemHeight;

    /* Vykreslování položek */
    for(vector<Item>::const_iterator it = (*actualSection).first; it != end; ++it) {
        SDL_Rect textPosition = itemArea;

        /* Ikona */
        if((*actualSection).flags & (ICONS_LEFT | ICONS_RIGHT)) {
            /* Prostor pro ikonu */
            SDL_Rect iconPosition = {itemArea.x, itemArea.y, *iconWidth, _itemHeight};
            if((*actualSection).flags & ICONS_RIGHT) iconPosition.x = itemArea.x+itemArea.w-*iconWidth;

            /* Přesná pozice ikony, ořezání a vykreslení */
            iconPosition = Effects::align(iconPosition, *(*actualSection).iconAlign, (**(*it).icon).w, (**(*it).icon).h);
            SDL_Rect iconCrop = {0, 0, iconPosition.w, iconPosition.h};
            SDL_BlitSurface(*(*it).icon, &iconCrop, screen, &iconPosition);

            /* Odečtení prostoru zabraného ikonou od prostoru textu */
            textPosition.x += *iconWidth; textPosition.y -= *iconWidth;
            if((*actualSection).flags & ICONS_RIGHT) textPosition.x = itemArea.x;
        }

        /* Barva textu */
        SDL_Color* color = itemColor;
        if((*actualSection).actualItem == it) {
            if((*it).flags == DISABLED) color = activeDisabledItemColor;
            else color = activeItemColor;
        }
        else if((*it).flags == DISABLED) color = disabledItemColor;

        /* Vykreslení oříznutého textu */
        SDL_Surface* text = (*Effects::textRenderFunction())(*itemFont, (*(*it).caption).c_str(), *color);
        textPosition = Effects::align(textPosition, (Align) ((*(*actualSection).itemsAlign & 0x0F) | ALIGN_MIDDLE), (*text).w, (*text).h);
        SDL_Rect textCrop = {0, 0, textPosition.w, textPosition.h};
        SDL_BlitSurface(text, &textCrop, screen, &textPosition);

        /** @todo flags EMPTY, SEPARATOR */

        itemArea.y += _itemHeight; /* Posunutí oblasti další položky */
    }
}
