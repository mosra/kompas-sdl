#ifndef Kompas_Sdl_Skin_h
#define Kompas_Sdl_Skin_h
/*
    Copyright © 2007, 2008, 2009, 2010 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Kompas.

    Kompas is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Kompas is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

/**
 * @file Skin.h
 * @brief Třída Skin
 */

#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "ConfParser.h"
#include "utility.h"

namespace Kompas { namespace Sdl {

/**
 * @brief Skiny
 *
 * Umožňuje úplnou konfigurovatelnost skinů v conf souborech a jejich změnu bez
 * nutnosti restartu aplikace. Založeno na ukazatelích, takže je (téměř) nulová
 * režie při vykreslování. Pro specifikaci skin souborů a podporovaných datových
 * typů viz @ref Skin.
 * @attention Pro správnou funkci této třídy je nutné zavolat TTF_Init()!
 * @todo Přepsat z ukazatelů na reference (asi nepude?)
 * @todo Skiny podle velikosti displeje (větší menu pro větší atd.)
 * @todo Vynechávat načítání stejných věcí vícekrát
 */
class Skin {
    public:

        /**
         * @brief Konstruktor
         *
         * @param   _screen     Displejová surface
         * @param   file        Soubor se skinem
         * @todo Možné problémy při resize (ztráta cíle ukazatele) => dvojitý?
         */
        inline Skin(SDL_Surface* _screen, const std::string& file): screen(_screen) { load(file); }

        /** @brief Destruktor */
        ~Skin(void);

        /**
         * @brief Načtení skinu
         *
         * @param   file        Soubor se skinem
         */
        void load(const std::string& file);

        /**
         * @brief Získání ukazatele na vlastnost
         *
         * Protože toto je šablonová funkce, je nutné ji volat s explicitní
         * specifikací návratové hodnoty (viz @ref SkinDataTypes).
         * @param   parameter   Parametr
         * @param   section     Sekce
         * @return  Ukazatel na vlastnost
         */
        template<class T> T get(const std::string& parameter, std::string section = ConfParser::DEFAULT_SECTION);

    private:
        /** @brief Vlastnost skinu */
        template<class T> struct Property {
            std::string parameter;  /**< @brief Název parametru z conf souboru */
            std::string section;    /**< @brief Název sekce z conf souboru */
            T property;             /**< @brief Vlastnost */
        };

        SDL_Surface* screen;    /**< @brief Displejová surface */
        ConfParser conf;        /**< @brief Konfigurák skinu */

        std::vector<Property<SDL_Surface**> > surfaces; /**< @brief Vektor se surfacy */
        std::vector<Property<TTF_Font**> > fonts;       /**< @brief Vektor s fonty */
        std::vector<Property<SDL_Rect*> > positions;    /**< @brief Vektor s pozicemi */
        std::vector<Property<SDL_Color*> > colors;      /**< @brief Vektor s barvami */
        std::vector<Property<Align*> > aligns;          /**< @brief Vektor se zarovnáními */
        std::vector<Property<std::string*> > texts;     /**< @brief Vektor s texty */
        std::vector<Property<int*> > numbers;           /**< @brief Vektor s čísly */
};

/**
@page Skin Skiny
Většina tříd z MInterface podporuje skinovatelnost a změnu vzhledu za běhu, bez
nutnosti restartu aplikace. Skiny jsou založeny na ukazatelích, takže třídy
využívající tuto funkčnost většinou nepřijdou s touto třídou vůbec do styku.
%Skin je definován v @ref ConfParser "conf souboru".
@sa Skin
@section SkinDataTypes Podporované datové typy
Skin podporuje škálu datových typů, které je možné definovat v conf souboru a
následně načíst pomocí funkcí Skin::get.
@subsection SkinSurface SDL_Surface**
V conf souboru uložen jako cesta k obrázku, lze načíst pomocí
Skin::get<SDL_Surface**>. Pokud se povede obrázek načíst, je automaticky
zkonvertován do formátu displejové surface pomocí SDL_DisplayFormatAlpha.
@note Při nenalezení obrázku je vypsáno chybové hlášení a vrácen ukazatel na
NULL. Segfault při normálním blittingu nehrozí.

<pre>\# skin.conf
[images]
splash=gfx/splash.png
</pre>
@code
Skin skin("skin.conf");
SDL_Surface** splashImage = skin.get<SDL_Surface**>("image", "splash");
@endcode
@subsection SkinFont TTF_Font**
V conf souboru uložen ve dvou parametrech. V prvním je cesta k TTF fontu, druhý
parametr udává velikost fontu. Druhý parametr musí mít stejný název jako první,
navíc se <tt>Size</tt> na konci. Font lze načíst pomocí Skin::get<TTF_Font**>.
@warning Pokud se font nepovede otevřít, vypíše se sice chybové hlášení, ale
první pokus o vypsání textu skončí segfaultem.

<pre>\# skin.conf
[menu]
captionFont=gfx/DejaVuSans.ttf
captionFontSize=12
</pre>
@code
Skin skin("skin.conf");
TTF_Font** font = skin.get<TTF_Font**>("captionFont", "menu");
@endcode
@subsection SkinPosition SDL_Rect*
Pozice je získávána z conf souboru ze čtyř parametrů. Pokud je ve funkci
Skin::get zadán prázdný parametr, získávají se parametry <tt>x</tt>, <tt>y</tt>,
<tt>w</tt>, <tt>h</tt> z příslušné sekce. Pokud je parametr zadán, získávají se
parametry <tt>parametrX</tt>, <tt>parametrY</tt>, <tt>parametrW</tt>,
<tt>parametrH</tt>. Pozici lze získat pomocí Skin::get<SDL_Rect*>.
@todo Získávat pozici jen z jednoho parametru (hodnoty oddělené mezerami)
<pre>\# skin.conf
[toolbar]
x=0
y=0
w=320
h=32
exitIconX=4
exitIconY=4
exitIconW=24
exitIconH=24
</pre>
@code
Skin skin("skin.conf");
SDL_Rect* toolbarPosition = skin.get<SDL_Rect*>("", "toolbar");
SDL_Rect* exitIconPosition = skin.get<SDL_Rect*>("exitIcon", "toolbar");
@endcode
@subsection SkinColor SDL_Color*
Pro syntaxi ukládání tohoto typu viz @ref ConfTypeInt. Barvu lze načíst
pomocí Skin::get<SDL_Color*>.
<pre>\# skin.conf
[colors]
blood=#910503
</pre>
@code
Skin skin("skin.conf");
SDL_Color* bloodColor = skin.get<SDL_Color*>("blood", "colors");
@endcode
@subsection SkinAlign Align*
Pro syntaxi ukládání a získávání tohoto typu z conf souboru viz
@ref ConfTypeAlign. Zarovnání lze načíst pomocí Skin::get<Align*>.
<pre>\# skin.conf
killSwitchAlign="right bottom"
</pre>
@code
Skin skin("skin.conf");
Align* killSwitchAlign = skin.get<Align*>("killSwitchAlign");
@endcode
@subsection SkinString std::string*
Lze načíst pomocí Skin::get<std::string*>.
<pre>\# skin.conf
author="Vladimír Vondruš"
</pre>
@code
Skin skin("skin.conf");
string* skinAuthor = skin.get<string*>("author");
@endcode
@subsection SkinInt int*
Lze načíst pomocí Skin::get<int*>.
<pre>\# skin.conf
emergency=911
</pre>
@code
Skin skin("skin.conf");
int* skinAuthor = skin.get<int*>("emergency");
@endcode
@section SkinTips Tipy a triky při tvoření skinu
Aby byl skin co nejvíce flexibilní, musí být připraven i na změny velikosti
okna. Z tohoto důvodu není dobré používat absolutní pozicování, protože pak by
při změně velikosti okna zůstalo vše vlevo nahoře a zbytek prostoru zůstal
nevyužitý. Tento problém lze vyřešit použitím zarovnání a poté případného
jemného posunu objektu na určenou pozici: Určí se zarovnání objektu (například
dole uprostřed) a tento objekt se poté posune o 5px nahoru, aby dole vznikla
mezera. Prvně je tedy objekt zarovnán a až poté posunut (většinou pomocí funkce
Effects::align). Ukázkový příklad:
<pre>
\# Spodní toolbar
[toolbar]
w=320
h=32
x=0
y=-5
align="bottom center"
</pre>
 */

}}

#endif
