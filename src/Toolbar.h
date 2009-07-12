#ifndef TOOLBAR_H
#define TOOLBAR_H

/**
 * @file Toolbar.h
 * @brief Třída Toolbar
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>    /* sort() */
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "utility.h"
#include "Effects.h"

/**
 * @brief Nástrojová lišta
 *
 * Víceřádková nástrojová lišta tvořená ikonami a / nebo jejich popisky. Popisky
 * mohou být také na centralizovaném místě a zobrazí se jen při aktivaci položky.
 * Položky jsou uspořádány do virtuální mřížky, aby bylo usnadněno jejich
 * procházení pomocí kurzorových kláves. Zakázané položky jsou při procházení
 * automaticky přeskakovány, pokud nastane stav, že budou zakázané všechny, nebude
 * možné jejich procházení vůbec. Procházení je nekonečné, tj. pokud již ve směru
 * posunu není žádná aktivní ikona, začne posun znovu od začátku.
 *
 * Při velkém počtu zakázaných položek je posun implementován takto: Při posunu
 * například vpravo v horizontálním směru se ve sloupci položek vpravo od aktuální
 * hledá co položka co nejblíže této. Pokud v tomto sloupci žádné aktivní položky
 * nejsou, hledá se v dalším sloupci. Analogický postup je ve vertikálním směru.
 * @todo Vypnutí "nekonečného procházení" ve flags
 */
class Toolbar {
    public:
        /**
         * @brief Flags pro toolbar
         *
         * Při uvedení jedné z flags Toolbar::CAPTION_UNDER_ICON,
         * Toolbar::CAPTION_BESIDE_ICON nebo Toolbar::CAPTION_IN_PLACE se povolí
         * zobrazování popisku ikony. Při uvedení Toolbar::NO_ICON je popisek
         * povinný a flagy CAPTION_* ztrácejí smysl.
         */
        enum Flags {
            NO_ICON = 0x01,             /**< @brief Vypnutí ikon */
            CAPTION_UNDER_ICON = 0x02,  /**< @brief Popisek ikony bude pod ní */
            CAPTION_BESIDE_ICON = 0x04, /**< @brief Popisek ikony bude vedle ní vpravo */

            /**
             * @brief Popisek ikony se objeví na centrálním definovaném místě
             * @warning Tato flag se nastaví automaticky při volání funkce
             * Toolbar::configureCaptionPlace, ručně by se nastavovat neměla!
             */
            CAPTION_IN_PLACE = 0x08
        };

        /**
         * @brief Flags pro položku
         */
        enum ItemFlags {
            DISABLED = 0x01             /**< @brief Položka je zakázaná */
        };

        /** @brief Typ pro ID položky */
        typedef int itemId;

        /**
         * @brief Konstruktor
         *
         * @param   _screen         Displejová surface
         * @param   _position       Pozice toolbaru
         * @param   _align          Zarovnání toolbaru
         * @param   _itemAlign      Zarovnání položky. Pokud nejsou povoleny
         *  nadpisky nebo je nastaven flag Toolbar::CAPTION_IN_PLACE, je toto
         *  zarovnání ikony, pokud je nastaven Toolbar::CAPTION_UNDER_ICON,
         *  vertikální zarovnání platí jen pro text (ikona je ve vršku oblasti),
         *  pokud je nastaven Toolbar::CAPTION_BESIDE_ICON, platí horizontální
         *  zarovnání jen pro text, ikona je co nejvíce vlevo. Pokud je nastaven
         *  flag Toolbar::NO_ICON, je to zarovnání nadpisku položky.
         * @param   _iconSize       Velikost ikony. Pokud je nastaven flag
         *  Toolbar::CAPTION_UNDER_ICON, je toto výška ikony i s odsazením,
         *  pokud je nastaven Toolbar::CAPTION_BESIDE_ICON, jedná se o šířku
         *  ikony včetně odsazení od textu. Může být NULL, pokud nadpisky u
         *  ikony nejsou nebo je nastaven flag Toolbar::NO_ICON.
         * @param   _captionFont    Font popisků položek (může být NULL, pokud
         *  nebudou nadpisky povoleny)
         * @param   _captionColor   Barva popisků položek (může být NULL, pokud
         *  nebudou nadpisky povoleny)
         * @param   _captionActiveColor Barva popisku vybrané položky (může být
         *  NULL, pokud nebudou nadpisky u ikon)
         * @param   _captionDisabledColor Barva popisku zakázané položky (může být
         *  NULL, pokud nebudou nadpisky u ikon)
         * @param   _flags          Flags (viz Toolbar::Flags)
         */
        Toolbar(SDL_Surface* _screen, SDL_Rect* _position, Align* _align, Align* _itemAlign, int* _iconSize, TTF_Font** _captionFont, SDL_Color* _captionColor, SDL_Color* _captionActiveColor, SDL_Color* _captionDisabledColor, int _flags):
            screen(_screen), position(_position), align(_align), itemAlign(_itemAlign),
            iconSize(_iconSize), captionFont(_captionFont), captionColor(_captionColor),
            captionDisabledColor(_captionDisabledColor) {}

        /**
         * @brief Nastavení nadpisku na centrálním definovaném místě
         *
         * Zruší flags Toolbar::CAPTION_UNDER_ICON nebo
         * Toolbar::CAPTION_BESIDE_ICON a nastaví flag Toolbar::CAPTION_IN_PLACE.
         * @param   _position       Pozice
         * @param   _align          Zarovnání textu
         */
        void configureCaptionPlace(SDL_Rect* _position, Align* _align);

        /**
         * @brief Přidání obrázku do toolbaru
         *
         * Tato funkce slouží i ke specifikaci pozadí toolbaru.
         * @param   _position       Pozice obrázku (pokud je NULL, použije se pozice celého toolbaru)
         * @param   _image          Obrázek
         */
        void addImage(SDL_Rect* _position, SDL_Surface** _image);

        /**
         * @brief Přidání položky do toolbaru
         *
         * @param   _position       Pozice položky
         * @param   x               X-ová pozice ve virtuální mřížce
         * @param   y               Y-ová pozice ve virtuální mřížce
         * @param   action          Akce (vrácená při vybrání položky)
         * @param   icon            Ikona položky (může být NULL, pokud je
         *  nastaven flag Toolbar::NO_ICON)
         * @param   activeIcon      Ikona položky při aktivaci (může být NULL,
         *  pokud je nastaven flag Toolbar::NO_ICON)
         * @param   disabledIcon    Ikona zakázané položky (může být NULL,
         *  pokud je nastaven flag Toolbar::NO_ICON nebo ikona nebude nikdy zakázaná)
         * @param   caption         Nadpisek položky (může být NULL, pokud nejsou
         *  nadpisky povoleny)
         * @param   flags           Flags (viz Toolbar::ItemFlags)
         * @return  ID položky
         * @todo Vyřešit položky obsazující více polí mřížky (ukazatele?)
         * @todo Pozice ve virtuální mřížce závislá celkem dost na skinu!
         */
        itemId addItem(SDL_Rect* _position, int x, int y, int action, SDL_Surface** icon, SDL_Surface** activeIcon, SDL_Surface** disabledIcon, std::string* caption, int flags = 0);

        /**
         * @brief Přidání textu do toolbaru
         *
         * @param   _position       Pozice textu
         * @param   _align          Zarovnání textu
         * @param   font            Font textu
         * @param   color           Barva textu
         * @param   _text           Text
         */
        void addText(SDL_Rect* _position, Align* _align, TTF_Font** font, SDL_Color* color, std::string* _text);

        /**
         * @brief Zakázání položky
         *
         * @param   item            ID položky (viz Toolbar::addItem)
         * @todo Posunout aktuální položku, pokud se nyní stala zakázanou
         */
        inline void disableItem(itemId item);

        /**
         * @brief Povolení položky
         *
         * @param   item            ID položky (viz Toolbar::addItem)
         * @todo Posunout aktuální položku na tuto, pokud předtím neexistovala
         *  žádná povolená položka
         */
        inline void enableItem(itemId item);

        /**
         * @brief Posun nahoru
         * @return  Akce položky, pokud položka není zakázaná
         */
        int moveUp(void);

        /**
         * @brief Posun dolů
         * @return  Akce položky, pokud položka není zakázaná
         */
        int moveDown(void);

        /**
         * @brief Posun vlevo
         * @return  Akce položky, pokud položka není zakázaná
         */
        int moveLeft(void);

        /**
         * @brief Posun vpravo
         * @return  Akce položky, pokud položka není zakázaná
         */
        int moveRight(void);

        /**
         * @brief Vybrání položky
         * @return  Akce položky, pokud položka není zakázaná
         * @todo Žádná zakázaná položka nemůže být aktivní!
         */
        inline int select(void) {
            return (*actualItem).flags & DISABLED ? -1 : (*actualItem).action;
        }

        /**
         * @brief Vykreslení toolbaru
         *
         * Prvně jsou vykreslovány obrázky, následně položky (ikony a jejich
         * popisky) a nakonec texty.
         */
        void view(void);
    private:
        /** @brief Struktura položky toolbaru */
        struct Item {
            SDL_Rect* position;     /**< @brief Pozice položky */
            int x,                  /**< @brief X-ová pozice položky ve virtuální mřížce (viz Toolbar::addItem) */
                y,                  /**< @brief Y-ová pozice položky ve virtuální mřížce (viz Toolbar::addItem) */
                action;             /**< @brief Akce (vrácena při vybrání položky) */
            SDL_Surface **icon,     /**< @brief Ikona položky (může být NULL) */
                **activeIcon,       /**< @brief Ikona vybrané položky (může být NULL) */
                **disabledIcon;     /**< @brief Ikona zakázané položky (může být NULL) */
            std::string* caption;   /**< @brief Nadpisek ikony (může být NULL) */
            int flags;              /**< @brief Flags (Viz Toolbar::ItemFlags) */
        };

        /** @brief Struktura obrázku */
        struct Image {
            SDL_Rect* position;     /**< @brief Pozice obrázku */
            SDL_Surface** image;    /**< @brief Obrázek */
        };

        /** @brief Struktura textu */
        struct Text {
            SDL_Rect* position;     /**< @brief Pozice textu */
            Align* align;           /**< @brief Zarovnání textu */
            TTF_Font** font;        /**< @brief Font */
            SDL_Color* color;       /**< @brief Barva textu */
            std::string* text;      /**< @brief Text */
        };

        SDL_Surface* screen;        /**< @brief Displejová surface */
        SDL_Rect* position;         /**< @brief Pozice toolbaru */
        Align* align;               /**< @brief Zarovnání toolbaru */
        Align* itemAlign;           /**< @brief Zarovnání položky (viz Toolbar::Toolbar) */
        int* iconSize;              /**< @brief Velikost ikony (viz Toolbar::Toolbar) */
        SDL_Rect* captionPosition;  /**< @brief Pozice centralizovaného popisku (jen při flagu Toolbar::CAPTION_IN_PLACE) */
        Align* captionAlign;        /**< @brief Zarovnání centralizovaného popisku (jen při flagu Toolbar::CAPTION_IN_PLACE) */
        TTF_Font** captionFont;     /**< @brief Font popisků položek (může být NULL) */
        SDL_Color *captionColor,    /**< @brief Barva popisků položek (může být NULL) */
            *captionActiveColor,    /**< @brief Barva popisku aktivní položky (může být NULL) */
            *captionDisabledColor;  /**< @brief Barva popisku zakázané položky (může být NULL) */
        int flags;                  /**< @brief Flags (viz Toolbar::Flags) */

        std::vector<Image> images;  /**< @brief Vektor s obrázky */
        std::vector<Text> texts;    /**< @brief Vektor s texty */
        std::vector<Item> items;    /**< @brief Vektor s položkami toolbaru */
        std::vector<Item>::const_iterator actualItem;   /** @brief Aktuální (vybraná) položka */

        /**
         * @brief Vertikálně řazené ukazatele na aktivní položky
         *
         * Pro účely horizontálního posunu - rychlé nalezení položky ve sloupci
         * vpravo či vlevo nejblíže od aktuální položky.
         */
        std::vector<std::vector<Item>::const_iterator> sortedVertical;

        /**
         * @brief Horizontálně řazené ukazatele na aktivní položky
         *
         * Pro účely vertikálního posunu - rychlé nalezení položky v řádku nad
         * či pod aktuální položkou.
         */
        std::vector<std::vector<Item>::const_iterator> sortedHorizontal;

        /**
         * @brief Predikát pro vertikální řazení položek
         *
         * @param   a       První položka k porovnání
         * @param   b       Druhá položka k porovnání
         * @return  Jestli a patří před b
         */
        static bool verticalCompare(std::vector<Item>::const_iterator a, std::vector<Item>::const_iterator b) {
            return (*a).x == (*b).x ? (*a).y < (*b).y : (*a).x < (*b).x;
        }

        /**
         * @brief Predikát pro horizontální řazení položek
         *
         * @param   a       První položka k porovnání
         * @param   b       Druhá položka k porovnání
         * @return  Jestli a patří před b
         */
        static bool horizontalCompare(std::vector<Item>::const_iterator a, std::vector<Item>::const_iterator b) {
            return (*a).y == (*b).y ? (*a).x < (*b).x : (*a).y < (*b).y;
        }

        /**
         * @brief Reload položek pro účely posunu
         *
         * Spouštěno při přidání nové položky či při zakázání nebo povolení
         * položky. Znova vytvoří seřazené vektory Toolbar::sortedHorizontal a
         * Toolbar::sortedVertical z aktivních položek a první povolenou položku
         * označí jako aktuální.
         */
        void reloadItems(void);
};

#endif