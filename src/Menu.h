#ifndef MENU_H
#define MENU_H

/**
 * @file Menu.h
 * @brief Třída Menu
 */

#include <iostream>
#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "Effects.h"
#include "utility.h"

namespace MInterface {

/**
 * @brief Textové menu s případnými ikonami
 *
 * Více sekcí s různým obsahem a možnost přepínání mezi nimi, rolování při velkém
 * počtu položek. Plná podpora Skin a Localize.
 * @todo Zvýraznění položek změnou pozadí
 * @todo Vícesloupcové menu + názvy sloupců (např. souborový manažer)
 * @todo Flags pro vypnutí nekonečného procházení
 * @todo Inline funkce jen tam, kde to je potřeba (kde budou často volané), zbytek
 *   přesunout do Menu.cpp
 */
class Menu {
    public:
        /**
         * @brief Flags pro celé menu
         *
         * @warning Tyto flags by neměly být používány přímo, jsou nastavovány
         * automaticky při spuštění funkcí Menu::configureCaption nebo
         * Menu::configureScrollbar!
         */
        enum Flags {
            CAPTION = 0x01,     /**< @brief Povolení zobrazení nadpisku sekce */
            SCROLLBAR = 0x02    /**< @brief Povolení zobrazení scrollbaru */
        };

        /**
         * @brief Flags pro sekci
         *
         * Při uvedení jedné z flags ICON_LEFT nebo ICON_RIGHT se povolí
         * zobrazování ikon u položek menu.
         */
        enum SectionFlags {
            ICONS_LEFT = 0x01,  /**< @brief Zobrazení ikon položek vpravo */
            ICONS_RIGHT = 0x02  /**< @brief Zobrazení ikon položek vlevo */
        };

        /** @brief Flags pro položku menu */
        enum ItemFlags {
            EMPTY = 0x01,       /**< @brief Položka je prázdná (mezera) */
            SEPARATOR = 0x02,   /**< @brief Položka je oddělovač (čára) */
            DISABLED = 0x04     /**< @brief Položka je zakázaná */
        };

        /** @brief Typ pro ID sekce */
        typedef unsigned int sectionId;

        /** @brief Typ pro ID položky */
        typedef unsigned int itemId;

        /**
         * @brief Konstruktor
         *
         * @param   _screen             Displejová surface
         * @param   _image              Pozadí menu
         * @param   _position           Pozice menu
         * @param   _menuAlign          Zarovnání menu vůči stránce
         * @param   _itemsPosition      Pozice položek
         * @param   _itemHeight         Výška položky (0 pro autodetekci z výšky textu)
         * @param   _iconWidth          Šířka ikony (může být NULL, pokud se ikony nebudou používat)
         * @param   _itemFont           Font položek
         * @param   _itemColor          Barva položky
         * @param   _activeItemColor    Barva aktivní položky
         * @param   _disabledItemColor  Barva zakázané položky (může být NULL,
         *  pokud nebudou nikde zakázané položky)
         * @param   _activeDisabledItemColor  Barva aktivní zakázané položky
         *  (může být NULL, pokud nebudou nikde zakázané položky)
         * @param   _flags              Flags
         */
        Menu(SDL_Surface* _screen, SDL_Surface** _image, SDL_Rect* _position, MToolkit::Align* _menuAlign, SDL_Rect* _itemsPosition, int* _itemHeight, int* _iconWidth, TTF_Font** _itemFont, SDL_Color* _itemColor, SDL_Color* _activeItemColor, SDL_Color* _disabledItemColor, SDL_Color* _activeDisabledItemColor, int _flags):
            screen(_screen), image(_image), position(_position), menuAlign(_menuAlign),
            itemsPosition(_itemsPosition), itemHeight(_itemHeight), iconWidth(_iconWidth),
            itemFont(_itemFont), itemColor(_itemColor), activeItemColor(_activeItemColor),
            disabledItemColor(_disabledItemColor),
            activeDisabledItemColor(_activeDisabledItemColor), flags(_flags) {}

        /**
         * @brief Nastavení nadpisku
         *
         * Nastaví a povolí vykreslování nadpisku sekce menu.
         * @param   _position   Pozice
         * @param   align       Zarovnání
         * @param   font        Font
         * @param   color       Barva
         */
        void configureCaption(SDL_Rect* _position, MToolkit::Align* align, TTF_Font** font, SDL_Color* color);

        /**
         * @brief Nastavení scrollbaru
         *
         * Nastaví a povolí scrollbar. Šipky se umístí do vrchu a spodku oblasti
         * dle vodorovného zarovnání, mezi nimi se bude pohybovat slider.
         * @param   _position   Pozice
         * @param   align       Vodorovné zarovnání položek (vertikální ignorováno)
         * @param   topArrow    Surface s vrchní šipkou
         * @param   bottomArrow Surface se spodní šipkou
         * @param   arrowHeight Výška šipky (pro odsazení slideru od šipek)
         * @param   slider      Slider
         */
        void configureScrollbar(SDL_Rect* _position, MToolkit::Align* align, int* arrowHeight, SDL_Surface** topArrow, SDL_Surface** bottomArrow, SDL_Surface** slider);

        /**
         * @brief Vytvoření sekce menu
         *
         * @param   parent      Rodičovská sekce
         * @param   caption     Nadpisek sekce (může být NULL, pokud nejsou nadpisky nastaveny)
         * @param   iconAlign   Zarovnání ikony (může být NULL, pokud nejsou ikony povoleny ve flags)
         * @param   itemsAlign  Zarovnání položek
         * @param   flags       Flags (viz Menu::SectionFlags)
         * @return  ID sekce, použitelné při přidávání a mazání položek menu v této sekci
         * @todo Jiné barvy a font pro každou sekci menu?
         */
        sectionId addSection(sectionId parent, std::string* caption, MToolkit::Align* iconAlign, MToolkit::Align* itemsAlign, int flags = 0);

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
        itemId addItem(sectionId section, int action, std::string* caption, SDL_Surface** icon = NULL, int flags = 0);

        /**
         * @brief Odstranění položky menu
         *
         * @param   section     ID sekce (viz Menu::addSection)
         * @param   item        ID položky (viz Menu::addItem)
         * @bug Po smazání položky už nebudou chodit dobře funkce Menu::disableItem a Menu::enableItem
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
         * @brief Přepnutí na nadřazenou sekci
         *
         * @todo Vrátit akci vybrané položky
         */
        inline void parentSection(void) {
            actualSection = sections.begin()+(*actualSection).parent;
        }

        /**
         * @brief Posun na další položku
         *
         * @return  Akce aktuální položky, pokud není zakázaná
         */
        int moveDown(void);

        /**
         * @brief Posun na předchozí položku
         *
         * @return  Akce aktuální položky, pokud není zakázaná
         */
        int moveUp(void);

        /**
         * @brief Rolování o stránku dolů
         *
         * @return  Akce aktuální položky, pokud není zakázaná
         */
        int scrollDown(void);

        /**
         * @brief Rolování o stránku nahoru
         *
         * @return  Akce aktuální položky, pokud není zakázaná
         */
        int scrollUp(void);

        /** @brief Zobrazení menu */
        void view(void);

    private:
        /** @brief Struktura položky menu */
        struct Item {
            std::string* caption;   /**< @brief Text položky */
            SDL_Surface** icon;     /**< @brief Ikona položky (může být NULL) */
            int action;             /**< @brief Číselná specifikace akce položky */
            int flags;              /**< @brief Flags (viz Menu::ItemFlags) */
        };

        /** @brief Sekce menu */
        struct Section {
            sectionId parent;       /**< @brief ID nadřazené sekce */
            std::string* caption;   /**< @brief Nadpisek sekce (může být NULL) */
            MToolkit::Align* iconAlign;     /**< @brief Zarovnání ikon (může být NULL) */
            MToolkit::Align* itemsAlign;    /**< @brief Zarovnání položek */
            int flags;              /**< @brief Flags (viz Menu::SectionFlags) */

            std::vector<Item> items;    /**< @brief Vektor s jednotlivými položkami */
            std::vector<Item>::const_iterator actualItem;   /**< @brief Aktuální položka */
        };

        SDL_Surface* screen;        /**< @brief Displejová surface */
        SDL_Surface** image;        /**< @brief Pozadí menu */
        SDL_Rect* position;         /**< @brief Pozice menu */
        MToolkit::Align *menuAlign; /**< @brief Zarovnání menu */

        SDL_Rect* captionPosition;      /**< @brief Pozice nadpisku */
        MToolkit::Align* captionAlign;  /**< @brief Zarovnání nadpisku */
        TTF_Font** captionFont;         /**< @brief Font nadpisku */
        SDL_Color* captionColor;        /**< @brief Barva nadpisku */

        SDL_Rect* scrollbarPosition;    /**< @brief Pozice scrollbaru */
        MToolkit::Align *scrollbarAlign;    /**< @brief Zarovnání scrollbaru */
        SDL_Surface **scrollbarArrowUp, /**< @brief Horní šipka scrollbaru */
            **scrollbarArrowDown,       /**< @brief Spodní šipka scrollbaru */
            **scrollbarSlider;          /**< @brief Posuvník scrollbaru */
        int *scrollbarArrowHeight;      /**< @brief Výška šipek scrollbaru
                                             @brief (=> odsazení šipek od posuvníku) */

        SDL_Rect* itemsPosition;    /**< @brief Pozice oblasti položek menu */
        int *itemHeight,            /**< @brief Výška položky menu (0 pro autodetekci) */
            *iconWidth;             /**< @brief Šířka ikony menu */
        TTF_Font** itemFont;        /**< @brief Font položek menu */
        SDL_Color *itemColor,       /**< @brief Barva normální položky */
            *activeItemColor,       /**< @brief Barva aktivní (právě vybrané) položky */
            *disabledItemColor,     /**< @brief Barva zakázané položky */
            *activeDisabledItemColor;   /**< @brief Barva právě vybrané zakázané položky */

        int flags;                  /**< @brief Flags pro celé menu (viz Menu::Flags) */
        std::vector<Section> sections;                  /**< @brief Jednotlivé sekce */
        std::vector<Section>::iterator actualSection;   /**< @brief Aktuální sekce */

        /**
         * @brief Obnovení iterátorů ukazujících na položky
         *
         * @param   section     Obnovit jen iterátory v dané sekci, pokud neuvedeno,
         *  obnoví iterátory ve všech sekcích + ukazatel na aktuální sekci
         */
        void reloadIterators(int section = -1);
};

}

#endif
