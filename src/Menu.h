#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "Effects.h"
#include "utility.h"

/**
 * @brief Textové menu s případnými ikonami
 *
 * Více sekcí s různým obsahem a možnost přepínání mezi nimi, rolování při velkém
 * počtu položek. Plná podpora Skin.
 * @todo Zvýraznění položek změnou pozadí
 * @todo Definice scrollbaru
 * @todo Vícesloupcové menu + názvy sloupců (např. souborový manažer)
 */
class Menu {
    public:
        /**
         * @brief Flags pro celé menu
         *
         * Tyto flags by neměly být používány přímo, jsou nastavovány automaticky
         * při spuštění funkcí Menu::configureCaption a Menu::configureScrollbar
         */
        enum Flags { CAPTION = 0x01, SCROLLBAR = 0x02 };

        /** @brief Flags pro sekci */
        enum SectionFlags { ICONS_LEFT = 0x01, ICONS_RIGHT = 0x02 };

        /** @brief Flags pro položku menu */
        enum ItemFlags { EMPTY = 0x01, SEPARATOR = 0x02, DISABLED = 0x04 };

        /** @brief Typ pro ID menu */
        typedef unsigned int sectionId;
        typedef unsigned int itemId;

        /**
         * @brief Konstruktor
         *
         * @param   _screen     Displejová surface
         * @param   _image      Pozadí menu
         * @param   _x          X-ové posunutí menu
         * @param   _y          Y-ové posunutí menu
         * @param   _w          Šířka menu
         * @param   _h          Výška menu
         * @param   _menuAlign  Zarovnání menu vůči stránce
         * @param   _itemsX     X-ová pozice položek v menu
         * @param   _itemsY     Y-ová pozice položek v menu
         * @param   _itemsW     Šířka položek
         * @param   _itemsH     Výška (všech) položek
         * @param   _itemHeight Výška položky (0 pro autodetekci z výšky textu)
         * @param   _iconWidth  Šířka ikony (může být NULL, pokud se ikony nebudou používat)
         * @param   _itemFont   Font položek
         * @param   _itemColor  Barva položky
         * @param   _activeItemColor    Barva aktivní položky
         * @param   _disabledItemColor  Barva zakázané položky (může být NULL,
         *  pokud nebudou nikde zakázané položky)
         * @param   _activeDisabledItemColor  Barva aktivní zakázané položky
         *  (může být NULL, pokud nebudou nikde zakázané položky)
         * @param   _flags      Flags
         * @todo No to je příšernost! -- převést na SDL_Rect
         */
        Menu(SDL_Surface* _screen, SDL_Surface** _image, int* _x, int* _y, int* _w, int* _h, Align* _menuAign, int* _itemsX, int* _itemsY, int* _itemsW, int* _itemsH, int* _itemHeight, int* _iconWidth, TTF_Font** _itemFont, SDL_Color* _itemColor, SDL_Color* _activeItemColor, SDL_Color* _disabledItemColor, SDL_Color* _activeDisabledItemColor, int _flags):
            screen(_screen), image(_image), x(_x), y(_y), w(_w), h(_h),
            menuAlign(_menuAign), itemsX(_itemsX), itemsY(_itemsY), itemsW(_itemsW),
            itemsH(_itemsH), itemHeight(_itemHeight), iconWidth(_iconWidth),
            itemFont(_itemFont), itemColor(_itemColor), activeItemColor(_activeItemColor),
            disabledItemColor(_disabledItemColor),
            activeDisabledItemColor(_activeDisabledItemColor), flags(_flags) {}

        /**
         * @brief Nastavení nadpisku
         *
         * Nastaví a povolí vykreslování nadpisku sekce menu
         * @param   _x      X-ová pozice
         * @param   _y      Y-ová pozice
         * @param   _w      Šířka
         * @param   _h      Výška
         * @param   align   Zarovnání
         * @param   font    Font
         * @param   color   Barva
         */
        void configureCaption(int* _x, int* _y, int* _w, int* _h, Align* align, TTF_Font** font, SDL_Color* color);

        /**
         * @brief Nastavení scrollbaru
         *
         * Nastaví a povolí scrollbar. Šipky se umístí do vrchu a spodku oblasti
         * dle vodorovného zarovnání, mezi nimi se bude pohybovat slider
         * @param   _x          X-ová pozice
         * @param   _y          Y-ová pozice
         * @param   _w          Šířka
         * @param   _h          Výška
         * @param   align       Vodorovné zarovnání položek (vertikální ignorováno)
         * @param   topArrow    Surface s vrchní šipkou
         * @param   bottomArrow Surface se spodní šipkou
         * @param   arrowHeight Výška šipky (pro odsazení slideru od šipek)
         * @param   slider      Slider
         */
        void configureScrollbar(int* _x, int* _y, int* _w, int* _h, Align* align, int* arrowHeight, SDL_Surface** topArrow, SDL_Surface** bottomArrow, SDL_Surface** slider);

        /**
         * @brief Vytvoření sekce menu
         *
         * @param   parent      Rodičovská sekce
         * @param   caption     Nadpisek sekce (může být NULL, pokud nejsou nadpisky nastaveny)
         * @param   iconAlign   Zarovnání ikony (může být NULL, pokud nejsou ikony povoleny ve flags)
         * @param   itemsAlign  Zarovnání položek
         * @param   flags       Flags (viz Menu::SectionFlags)
         * @return  ID sekce, použitelné při přidávání a mazání položek menu v této sekci
         */
        sectionId addSection(sectionId parent, std::string* caption, Align* iconAlign, Align* itemsAlign, int flags);

        /**
         * @brief Přidání položky do menu
         *
         * @param   section     ID sekce, do které má být položka vložena
         * @param   action      Číselná specifikace akce (vrácena při aktivaci položky)
         * @param   caption     Nadpisek položky
         * @param   icon        Ikona (může být NULL, pokud nejsou ikony povoleny ve flags sekce)
         * @param   flags       Flags (viz Menu::ItemFlags)
         * @return  ID položky, použitelné při jejím mazání
         */
        itemId addItem(sectionId section, int action, std::string* caption, SDL_Surface** icon, int flags);

        /**
         * @brief Odstranění položky menu
         *
         * @param   section     ID sekce (viz Menu::addSection)
         * @param   item        ID položky (viz Menu::addItem)
         */
        inline void deleteItem(sectionId section, itemId item) {
            sections[section].items.erase(sections[section].items.begin()+item);
            reloadIterators(section);
        }

        /** @brief Zakázání položky menu
         *
         * @param   section     ID sekce (viz Menu::addSection)
         * @param   item        ID položky (viz Menu::addItem)
         */
        inline void disableItem(sectionId section, itemId item) {
            sections[section].items[item].flags |= DISABLED;
        }

        /** @brief Povolení položky menu
         *
         * @param   section     ID sekce (viz Menu::addSection)
         * @param   item        ID položky (viz Menu::addItem)
         */
        inline void enableItem(sectionId section, itemId item) {
            sections[section].items[item].flags &= !DISABLED;
        }

        /**
         * @brief Vybrání položky menu
         *
         * @return  Akce položky (viz Menu::addItem), pokud je položka neaktivní
         *  (Menu::disableItem nebo Menu::DISABLED ve flags), vrací -1
         * @todo    Kliknutí na dosud nevybranou položku
         */
        inline int select(void) {
            if((*(*actualSection).actualItem).flags & DISABLED) return -1;
            return (*(*actualSection).actualItem).action;
        }

        /**
         * @brief Přepnutí na jinou sekci
         *
         * @param   section     ID sekce (viz Menu::addSection)
         * @todo Vrátit akci vybrané položky
         */
        inline void changeSection(sectionId section) {
            actualSection = sections.begin()+section;
        }

        /**
         * @brief Posun na další položku
         * @return  Akce aktuální položky, pokud není zakázaná
         */
        int moveDown(void);

        /**
         * @brief Posun na předchozí položku
         * @return  Akce aktuální položky, pokud není zakázaná
         */
        int moveUp(void);

        /**
         * @brief Rolování o stránku dolů
         * @return  Akce aktuální položky, pokud není zakázaná
         */
        int scrollDown(void);

        /**
         * @brief Rolování o stránku nahoru
         * @return  Akce aktuální položky, pokud není zakázaná
         */
        int scrollUp(void);

        /**
         * @brief Zobrazení menu
         */
        void view(void);

    private:
        /** Flags pro celé menu */
        int flags;

        /** Displejová surface */
        SDL_Surface* screen;

        /** Pozadí menu */
        SDL_Surface** image;

        /** Posunutí a velikost menu */
        int *x, *y, *w, *h;

        /** Zarovnání menu */
        Align *menuAlign;

        /** Pozice nadpisku */
        int *captionX, *captionY, *captionW, *captionH;

        /** Zarovnání nadpisku */
        Align *captionAlign;

        /** Font nadpisku */
        TTF_Font** captionFont;

        /** Barva nadpisku */
        SDL_Color* captionColor;

        /** Pozice scrollbaru */
        int *scrollbarX, *scrollbarY, *scrollbarW, *scrollbarH;

        /** Zarovnání scrollbaru */
        Align *scrollbarAlign;

        /** Šipky a posuvník */
        SDL_Surface **scrollbarArrowUp, **scrollbarArrowDown, **scrollbarSlider;

        /** Výška šipek (=> odsazení šipek od posuvníku) */
        int *scrollbarArrowHeight;

        /** Velikost a pozice oblasti položek menu */
        int *itemsX, *itemsY, *itemsW, *itemsH;

        /** Výška položek, šířka ikony */
        int *itemHeight, *iconWidth;

        /**
         * Font položek menu
         * @todo Jiný font, barvu pro každou sekci menu?
         */
        TTF_Font** itemFont;

        /**
         * Barvy položek
         * @todo Jiné barvy pro každou sekci menu?
         */
        SDL_Color *itemColor, *activeItemColor, *disabledItemColor, *activeDisabledItemColor;

        /** Struktura položky menu */
        struct Item {
            std::string* caption;
            SDL_Surface** icon;
            int action;
            int flags;
        };

        /** Sekce menu */
        struct Section {
            sectionId parent;
            std::string* caption;
            Align* iconAlign;
            Align* itemsAlign;
            int flags;

            std::vector<Item> items;
            std::vector<Item>::const_iterator actualItem;
        };

        /** Jednotlivé sekce */
        std::vector<Section> sections;

        /** Aktuální sekce */
        std::vector<Section>::iterator actualSection;

        /**
         * @brief Obnovení iterátorů ukazujících na položky
         *
         * @param   section     Obnovit jen iterátory v dané sekci, pokud neuvedeno,
         *  obnoví iterátory ve všech sekcích + ukazatel na aktuální sekci
         */
        void reloadIterators(int section = -1);
};

#endif
