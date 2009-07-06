#include "Menu.h"

using std::cout;    using std::endl;        using std::vector;

/* Nastavení nadpisku */
void Menu::configureCaption (SDL_Rect* _position, Align* align, _TTF_Font** font, SDL_Color* color) {
    captionPosition = _position;
    captionAlign = align;
    captionFont = font;
    captionColor = color;

    /* Povolení ve flags */
    flags |= CAPTION;
}

/* Nastavení scrollbaru */
void Menu::configureScrollbar (SDL_Rect* _position, Align* align, int* arrowHeight, SDL_Surface** arrowUp, SDL_Surface** arrowDown, SDL_Surface** slider) {
    scrollbarPosition = _position;
    scrollbarAlign = align;
    scrollbarArrowUp = arrowUp;
    scrollbarArrowDown = arrowDown;
    scrollbarArrowHeight = arrowHeight;
    scrollbarSlider = slider;

    /* Povolení ve flags */
    flags |= SCROLLBAR;
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

/* Posun dolů */
int Menu::moveDown (void) {
    if((*actualSection).items.size() == 0) return -1;

    if(++(*actualSection).actualItem == (*actualSection).items.end())
        (*actualSection).actualItem = (*actualSection).items.begin();

    return (*(*actualSection).actualItem).flags & DISABLED ? -1 : (*(*actualSection).actualItem).action;
}

/* Posun nahoru */
int Menu::moveUp (void) {
    if((*actualSection).items.size() == 0) return -1;

    if((*actualSection).actualItem == (*actualSection).items.begin())
        (*actualSection).actualItem = (*actualSection).items.end();

    return (*--(*actualSection).actualItem).flags & DISABLED ? -1 : (*(*actualSection).actualItem).action;
}

/* Posun o stránku dolů */
int Menu::scrollDown (void) {
    if((*actualSection).items.size() == 0) return -1;

    /* Půjčeno z Map::view() */
    int _itemHeight = *itemHeight;
    if(_itemHeight == 0) _itemHeight = TTF_FontLineSkip(*itemFont);

    (*actualSection).actualItem += (*itemsPosition).h/_itemHeight-1;

    if((*actualSection).actualItem > (*actualSection).items.end())
        (*actualSection).actualItem = (*actualSection).items.end()-1;

    return (*(*actualSection).actualItem).flags & DISABLED ? -1 : (*(*actualSection).actualItem).action;
}

/* Posun o stránku nahoru */
int Menu::scrollUp (void) {
    if((*actualSection).items.size() == 0) return -1;

    /* Půjčeno z Map::view() */
    int _itemHeight = *itemHeight;
    if(_itemHeight == 0) _itemHeight = TTF_FontLineSkip(*itemFont);

    (*actualSection).actualItem -= (*itemsPosition).h/_itemHeight-1;

    if((*actualSection).actualItem < (*actualSection).items.begin())
        (*actualSection).actualItem = (*actualSection).items.begin();

    return (*(*actualSection).actualItem).flags & DISABLED ? -1 : (*(*actualSection).actualItem).action;
}


/* Reload iterátorů */
void Menu::reloadIterators (int section) {
    /* Reloud jen u jedný sekce (při změně počtu položek menu) */
    if(section != -1) {
        sections[section].actualItem = sections[section].items.begin();
        return;
    }

    /* Reloud u všech sekcí (při přidání sekce) */
    for(vector<Section>::iterator it = sections.begin(); it != sections.end(); ++it) {
        (*it).actualItem = (*it).items.begin();
    }

    actualSection = sections.end()-1;
}

/* Zobrazení menu */
void Menu::view (void) {
    /* Plocha pro vykreslování menu */
    SDL_Rect area = Effects::align(screen, *menuAlign, *position);

    /* Pozadí */
    SDL_BlitSurface(*image, NULL, screen, &area);

    /* Nadpisek menu */
    if(flags & CAPTION) {
        /* Prostor pro napisek */
        SDL_Rect _captionPosition = Effects::align(area, ALIGN_DEFAULT, *captionPosition);
        SDL_Surface* text = (*Effects::textRenderFunction())(*captionFont, (*(*actualSection).caption).c_str(), *captionColor);

        /* Přesná pozice nadpisku, ořezání a vykreslení */
        _captionPosition = Effects::align(_captionPosition, *captionAlign, (*text).w, (*text).h);
        SDL_Rect captionCrop = {0, 0, _captionPosition.w, _captionPosition.h};
        SDL_BlitSurface(text, &captionCrop, screen, &_captionPosition);
        SDL_FreeSurface(text);
    }

    /* Mezera mezi položkami, pokud není striktně definovaná. */
    int _itemHeight = *itemHeight;
    if(_itemHeight == 0) _itemHeight = TTF_FontLineSkip(*itemFont);
    int itemsPerPage = (*itemsPosition).h/_itemHeight;

    /* První a poslední zobrazená položka */
    vector<Item>::const_iterator begin = (*actualSection).items.begin();
    vector<Item>::const_iterator end = (*actualSection).items.end();

    /* Scrollbar */
    if(flags & SCROLLBAR) {
        /* Prostor pro scrollbar */
        SDL_Rect _scrollbarPosition = Effects::align(area, ALIGN_DEFAULT, *scrollbarPosition);

        /* Vrchní šipka, pokud je kam posouvat */
        if((*actualSection).actualItem != (*actualSection).items.begin()) {
            SDL_Rect arrowPosition = Effects::align(_scrollbarPosition, (Align) ((*scrollbarAlign & 0x0F) | ALIGN_TOP), (**scrollbarArrowUp).w, (**scrollbarArrowUp).h);
            SDL_Rect arrowCrop = {0, 0, arrowPosition.w, arrowPosition.h};
            SDL_BlitSurface(*scrollbarArrowUp, &arrowCrop, screen, &arrowPosition);
        }

        /* Spodní šipka, pokud je kam posouvat */
        if((*actualSection).actualItem != (*actualSection).items.end()-1) {
            SDL_Rect arrowPosition = Effects::align(_scrollbarPosition, (Align) ((*scrollbarAlign & 0x0F) | ALIGN_BOTTOM), (**scrollbarArrowDown).w, (**scrollbarArrowDown).h);
            SDL_Rect arrowCrop = {0, 0, arrowPosition.w, arrowPosition.h};
            SDL_BlitSurface(*scrollbarArrowDown, &arrowCrop, screen, &arrowPosition);
        }

        /* Slider (jen při počtu položek > 1, aby se zabránilo dělení nulou) */
        if(end-begin > 1) {
            /* Pozice slideru v nejvyšším bodě */
            SDL_Rect sliderPosition = Effects::align(_scrollbarPosition, (Align) ((*scrollbarAlign & 0x0F) | ALIGN_TOP), (**scrollbarSlider).w, (**scrollbarSlider).h, 0, *scrollbarArrowHeight);

            /* Dráha, po které může slider jet */
            int height = (*scrollbarPosition).h-2*(*scrollbarArrowHeight)-(**scrollbarSlider).h;

            /* Pozice */
            sliderPosition.y += height*((*actualSection).actualItem-begin)/(end-begin-1);

            SDL_Rect sliderCrop = {0, 0, sliderPosition.w, sliderPosition.h};
            SDL_BlitSurface(*scrollbarSlider, &sliderCrop, screen, &sliderPosition);
        }

    }

    /* Pokud je položek více, než se na stránku vejde a aktuální položka není v
       první polovině menu, upravení začátku tak, aby aktuální položka byla v polovině */
    if(end-begin > itemsPerPage && (*actualSection).actualItem-itemsPerPage/2 > begin) {
        begin = (*actualSection).actualItem-itemsPerPage/2;

        /* Pokud je položka blízko u konce, upravení začátku, aby bylo vypsáno
           maximum položek */
        if(end-begin < itemsPerPage)
            begin = end-itemsPerPage;
    }

    end = begin+itemsPerPage;

    /* Plocha pro vykreslení položek. Prvně zarování celé plochy položek, až v ní
       se podle počtu položek na stránku a vertikálního zarovnání (&0xF0) vypočítá
       konečná plocha položek */
    SDL_Rect itemArea = Effects::align(
        Effects::align(area, ALIGN_DEFAULT, *itemsPosition),
        (Align) (*(*actualSection).itemsAlign & 0xF0), (*itemsPosition).w, _itemHeight*(end-begin)
    );
    itemArea.h = _itemHeight;

    /* Vykreslování položek */
    for(vector<Item>::const_iterator it = begin; it != end; ++it) {
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
        SDL_FreeSurface(text);

        /** @todo flags EMPTY, SEPARATOR */

        itemArea.y += _itemHeight; /* Posunutí oblasti další položky */
    }
}
