#ifndef Map2X_Sdl_Keyboard_h
#define Map2X_Sdl_Keyboard_h

/**
 * @file Keyboard.h
 * @brief Třída Keyboard
 */

#include <vector>
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "FPS.h"
#include "Matrix.h"
#include "Mouse.h"
#include "utility.h"

namespace Map2X { namespace Sdl {

class Skin;

/**
 * @brief Klávesa klávesnice
 *
 * Struktura patřící do private sekce třídy Keyboard, bohužel kvůli dědění
 * šablonové třídy musí být před deklarací třídy.
 */
struct KeyboardKey {
    int x,                  /**< @brief X-ová pozice ve virtuální mřížce (viz Matrix) */
        y;                  /**< @brief Y-ová pozice ve virtuální mřížce (viz Matrix) */
    int flags;              /**< @brief Flags (viz Keyboard::Flags) */
    SDL_Rect position;      /**< @brief Pozice klávesy */
    SDL_Surface** image;    /**< @brief Pozadí klávesy */
    SDL_Surface** activeImage;  /**<  @brief Pozadí stisknuté klávesy */
    std::string name;       /**< @brief Popisek klávesy */

    /**
     * @brief Hodnoty klávesy
     *
     * Pokud se jedná o normální klávesu, jsou na zde tyto hodnoty:<br>
     * Pozice 0 - normální hodnota klávesy<br>
     * Pozice 1 - hodnota klávesy se Shiftem<br>
     * Pozice 2 - hodnota klávesy v kombinaci s 1. spec. klávesou<br>
     * Pozice 3 - hodnota klávesy v kombinaci s 1. spec. klávesou a Shiftem<br>
     * Pozice 4 - hodnota klávesy v kombinaci s Shift + 1. spec. klávesou<br>
     * Pozice 5 - hodnota klávesy v kombinaci s Shift + 1. spec. a Shiftem<br>
     * Pozice 6 - hodnota klávesy v kombinaci s 2. spec. klávesou<br>
     * Pozice 7 - hodnota klávesy v kombinaci s 2. spec. klávesou a Shiftem<br>
     * ...
     */
    std::vector<std::string> values;
};

/**
 * @brief Psaní textu pomocí klávesnice
 *
 * Plná konfigurovatelnost pomocí conf osuborů, podpora UTF-8 a skinů, dotykové
 * ovládání. Podrobný popis konfiguračních souborů a ovládání v samostatné sekci
 * @ref Keyboard.
 * @todo Přepínání jiné klávesnice a dalšího textu ke zpracování
 * @todo Nadpisek klávesnice
 * @todo Propojení klávesnice a skinu!
 * @todo Duplicitní ConfParser <=> Skin
 */
class Keyboard: public Matrix<KeyboardKey>, public Mouse {
    public:
        /** @brief Flags */
        enum Flags {
            HIDDEN = 0x01,      /**< @brief Klávesnice je schovaná */

            /**
             * @brief Zda ukazovat kurzor
             *
             * Touto flag lze určit, zda kurzor bude na počátku vykreslování
             * viditelný, nebo ne. Tato flag je ale spíše jen pro vnitřní
             * použití - po uplynutí doby stanovené v parametru
             * @ref KeyboardCursorInterval "cursorInterval" v příslušné sekci
             * skinu se viditelnost přepne do druhé polohy.
             * @sa Keyboard::cursorBlink
             */
            SHOW_CURSOR = 0x02
        };

        /**
         * @brief Konstruktor
         *
         * Načte klávesnici z conf souboru
         * @param   screen  Displejová surface
         * @param   _skin   Reference na otevřenou skin
         * @param   file    Soubor s daty klávesnice
         * @param   _text   Ukazatel na proměnnou s textem ke zpracování
         * @param   _flags  Flags (viz Keyboard::Flags)
         */
        Keyboard(SDL_Surface* screen, Skin& _skin, std::string file, std::string& _text, int _flags = 0);

        /**
         * @brief Stisk klávesy
         *
         * Zapíše znak klávesy do proměnné
         */
        void select(void);

        /**
         * @brief Kliknutí
         *
         * Vyvolá akci způsobenou kliknutím myši na příslušné souřadnice
         * @param   x       X-ová souřadnice
         * @param   y       Y-ová souřadnice
         * @param   action  Pokud kliknutí spustilo nějakou akci, do této
         *  proměnné se uloží její číslo (-1 pokud se žádná akce nespustila)
         * @return  Zda bylo klinutí v oblasti objektu
         * @todo Rozdělit do mouseDown a mouseUp
         */
        bool click(int x, int y, int& action);

        /** @brief Schování toolbaru */
        inline void hide(void) { flags |= HIDDEN; }

        /** @brief Povolení zobrazení toolbaru */
        inline void show(void) { flags &= ~HIDDEN; }

        /** @brief Zjištění, jestli je povoleno zobrazení toolbaru */
        inline operator bool(void) { return !(flags & HIDDEN); }

        /** @brief Zobrazení klávesnice */
        void view(void);
    private:
        /**
         * @brief Flags klávesy
         */
        enum KeyFlags {
            DISABLED = 0x001,   /**< @brief Klávesa je zakázána (nutné pro Matrix) */
            SPECIAL = 0x002,    /**< @brief Klávesa je speciální */

            ENTER = 0x004,      /**< @brief Enter */
            SHIFT = 0x008,      /**< @brief Shift */
            BACKSPACE = 0x010,  /**< @brief Backspace */
            DELETE = 0x020,     /**< @brief Delete */
            LEFT_ARROW = 0x040, /**< @brief Šipka doleva */
            RIGHT_ARROW = 0x080 /**< @brief Šipka doprava */
        };


        SDL_Surface* screen;    /**< @brief Displejová surface */
        Skin& skin;             /**< @brief Globální skin */
        std::string& text;      /**< @brief Text ke zpracování */
        std::string::iterator cursor;   /**< @brief Pozice kurzoru */

        int keyboardW,          /**< @brief Šířka klávesnice (z konfiguráku) */
            keyboardH;          /**< @brief Výška klávesnice (z konfiguráku) */
        SDL_Rect textPosition;  /**< @brief Pozice zpracovávaného textu (z konfiguráku) */
        Align textAlign;  /**< @brief Zarovnání textu (z konfiguráku) */

        int *keyboardX,         /**< @brief X-ová pozice klávesnice (ze skinu) */
            *keyboardY;         /**< @brief Y-ová pozice klávesnice (ze skinu) */
        Align *align; /**< @brief Zarovnání klávesnice (ze skinu) */
        SDL_Surface **image;    /**< @brief Pozadí klávesnice (ze skinu) */

        TTF_Font **textFont;    /**< @brief Barva textu (ze skinu) */
        SDL_Color *textColor;   /**< @brief Barva textu (ze skinu) */

        Align *keyAlign;  /**< @brief Zarovnání popisku klávesy (ze skinu) */
        TTF_Font **keyFont;     /**< @brief Font popisků kláves (ze skinu) */
        SDL_Color *keyColor,    /**< @brief Barva popisků kláves (ze skinu) */
            *keyActiveColor,    /**< @brief Barva popisku aktivní klávesy (ze skinu) */
            *keySpecialActiveColor; /**< @brief Barva popisku stlačené spec. klávesy (ze skinu) */

        SDL_Surface **cursorImage; /**< @brief Obrázek kurzoru (ze skinu) */
        int *cursorX,           /**< @brief X-ové posunutí kurzoru (ze skinu) */
            *cursorY;           /**< @brief Y-ové posunutí kurzoru (ze skinu) */
        Align* cursorAlign;   /**< @brief Vertikální zarovnání kurzoru (ze skinu) */
        int *cursorInterval;    /**< @brief Interval blikání kurzoru (ze skinu) */

        FPS::Data cursorBlink; /**< @brief Čas dalšího přebliknutí kurzoru */
        int flags;              /**< @brief Flags (viz Toolbar::Flags) */

        /**
         * @brief Ukazatel na stlačenou speciální klávesu
         *
         * Pokud je rovno items.end(), žádná speciální klávesa nebyla stlačena.
         */
        std::vector<KeyboardKey>::const_iterator specialPushed;

        /**
         * @brief Ukazatel na stlačenou spec. klávesu se Shiftem
         *
         * Toto znamená, že uživatel stlačil kombinaci Shift a pak spec. klávesa.
         * Pokud je rovno items.end(), tato kombinace nebyla stlačena.
         */
        std::vector<KeyboardKey>::const_iterator specialShiftPushed;

        /** @brief Zda byl stlačen Shift */
        bool shiftPushed;
};

/**
 * @page Keyboard Klávesnice na obrazovce
 * Klávesnice je určena pro editaci textu. Plně konfigurovatelná pomocí conf
 * souborů, skinovatelnost a plná podpora UTF-8.
 * @sa Keyboard
 * @section KeyboardModifiers Modifikátory
 * Klávesnice má kromě běžných kláves také modifikátory, které přidávají
 * klávesám další hodnoty. Klávesa <strong>Shift</strong> přidává klávesám druhou
 * hodnotu, po jejím stisku a následném stisku klávesy se vypíše shift hodnota
 * dané klávesy a pro další psaní už je shift zase vypnutý. Při opětovném stisku
 * Shiftu se modifikátor vypne.
 *
 * Dalšími modifikátory jsou tzv. <strong>speciální klávesy</strong>, primárně
 * určené k přidávání diakritiky k písmenným klávesám. Speciální klávesy lze
 * kombinovat se shiftem, takže každá speciální klávesa přidává každé klávese až
 * 4 hodnoty:
 * <ul>
 * <li>speciální + klávesa</li>
 * <li>speciální + shift + klávesa</li>
 * <li>shift + speciální + klávesa</li>
 * <li>shift + speciální + shift + klávesa</li>
 * </ul>
 * Při opětovném stisku speciální klávesy se modifikátor vypne, při stisku dvou
 * speciálních kláves po sobě se jako modifikátor nastaví ta druhá.
 * @subsection KeyboardFeaturesMissing Chybějící hodnoty kláves
 * Každá klávesa nemusí mít uvedené všechny hodnoty kláves, povinná je akorát
 * normální hodnota. Pokud nějaká hodnota chybí, je doplněna takto:
 * <ul>
 * <li>pokud není definována shift, je stejná jako normální</li>
 * <li>pokud není definována special + normální, je stejná jako normální</li>
 * <li>pokud není definována shift + special + normální, je stejná jako normální</li>
 * <li>pokud není definována special + shift, je stejná jako shift</li>
 * <li>pokud není definována shift + special + shift, je stejná jako shift</li>
 * </ul>
 * @section KeyboardConf Konfigurační soubory
 * Klávesnice je plně uživatelsky konfigurovatelná pomocí conf souborů, čímž lze
 * vytvořit specializované klávesnice pro různé abecedy a různé funkce (např.
 * klávesnice přímo se shellovými příkazy). Konfigurace je rozdělena do skin
 * souboru (který je určen pro to, aby byl společný více klávesnicím) a samotné
 * konfigurace jednotlivých kláves.
 * @subsection KeyboardConfSkin Skin klávesnice
 * Protože klávesnice je úzce spojena se skinem a může dojít ke stavu, že stejná
 * klávesnice bude načtená ve více různých skinech, či v jednom skinu bude více
 * různých klávesnic, jsou obrázky, fonty a další věci úzce související
 * se skinem přesunuty do odpovídající sekce skinu. Název této sekce je potom
 * specifikován v konfiguračním souboru klávesnice.
 *
 * Sekce odpovídající této ukázkové klávesnici v souboru skinu:
<pre>\# Soubor skinu

[smallKeyboard]

\# Posunutí a zarovnání klávesnice vzhledem k displeji
x=0
y=0
align="middle center"

\# Pozadí klávesnice
image="gfx/keyboard.png"

\# Font a barva upravovaného textu
textFont="gfx/DejaVuSans.ttf"
textFontSize=12
textColor=\#ffffff

\# Posunutí, zarovnání a obrázek kurzoru
cursorX=0
cursorY=0
cursorAlign="middle"
cursorImage=gfx/cursor.png

@anchor KeyboardCursorInterval
\# Interval blikání kurzoru (= 300 ms viditelný, 300 ms ne)
cursorInterval=300

\# Zarovnání popisku uvnitř klávesy, font
keyAlign="center middle"
keyFont="gfx/DejaVuSans.ttf"
keyFontSize=11

\# Barva klávesy, vybrané klávesy a stisknuté speciální klávesy
keyColor=\#000000
keyActiveColor=\#0000ff
keySpecialActiveColor=\#ff0000

\# Pozadí běžné klávesy a vybrané běžné klávesy
keyImage=gfx/key.png
keyActiveImage=gfx/keyActive.png
keySpecialImage=gfx/keySpecial.png
keySpecialActiveImage=gfx/keySpecialActive.png

\# Pozadí mezerníku, enteru, shiftu, backspace a jejich pozadí, když jsou vybrané
spaceImage=gfx/keySpace.png
spaceActiveImage=gfx/keySpaceActive.png
enterImage=gfx/keyEnter.png
enterActiveImage=gfx/keyEnterActive.png
shiftImage=gfx/keyShift.png
shiftActiveImage=gfx/keyShiftActive.png
backspaceImage=gfx/keyBackspace.png
backspaceActiveImage=gfx/keyBackspaceActive.png
deleteImage=gfx/keyDelete.png
deleteActiveImage=gfx/keyDeleteActive.png
leftArrowImage=gfx/leftArrowImage.png
leftArrowActiveImage=gfx/leftArrowActiveImage.png
rightArrowImage=gfx/rightArrowImage.png
rightArrowActiveImage=gfx/rightArrowActiveImage.png
</pre>
 * @subsection KeyboardConfGlobal Globální nastavení klávesnice
 * Na začátku conf souboru klávesnice se nastavují globální parametry společné
 * všem klávesám.
<pre>\# Soubor klávesnice
\# Verze konfiguračního souboru klávesnice = 1
\# (pro dodržení pozdější zpětné kompatibility)
version=1

\# Stručný popis klávesnce, autor
keyboard="Malá klávesnice s málo klávesami"
author="Vladimír Vondruš"

\# Sekce glob. skinu, ve které se budou hledat obrázky, fonty a další věci
\# spojené se skinem.
skinSection=smallKeyboard

\# Velikost klávesnice
w=312
h=146

\# Pozice a zarovnání zpracovávaného textu
textX=10
textY=7
textW=292
textH=36
textAlign="middle left"

\# Šířka a výška běžných a speciálních kláves
keyW=24
keyH=24
</pre>
 * @subsection KeyboardConfSpecialKeys Speciální klávesy
 * Dále jsou v souboru uvedeny speciální klávesy. Při jejich stisku se nic
 * nezapisuje do textu, ale změní se hodnoty některých kláves. Speciální klávesy
 * musí být uvedeny alespoň popisek (parametr <tt>name</tt>) nebo hodnotu
 * (<tt>val</tt>), případně shift hodnotu (<tt>shiftVal</tt>).
<pre>
# Spec. klávesa číslo 0
[specialKey]

# Pozice klávesy
x=240
y=50

# Pozice klávesy ve virtuální mřížce
posX=10
posY=0

# Popisek klávesy - tato klávesa přidává přehlásku či kroužek k písmenům
# (při stisku shift + tato spec. klávesa)
name=¨°

# Spec. klávesa číslo 1 - přidává čárku nebo zpětnou čárku (se shiftem) k písmenům
[specialKey]
x=264
y=50
posX=11
posY=0
name="'`"

# Spec. klávesa číslo 2 - přidává háček nebo stříšku (se shiftem) k písmenům
[specialKey]
x=264
y=74
posX=11
posY=1
name=ˇ^
</pre>
 * @subsection KeyboardConfFunctionKeys Funkční klávesy
 * Conf soubor pokračuje nastavením funkčních kláves - kláves Space, Shift,
 * Backspace, Delete, kurzorové klávesy a Enter. Jejich účel je jasný z názvů.
 * Tyto klávesy mají většinou jiné rozměry než ostatní klávesy, proto má každá
 * své parametry pro určení velikosti. Jejich pozadí je specifikováno v
 * příslušné sekci skinu. Parametr <tt>name</tt> není povinný, pokud je z pozadí
 * klávesy dostatečně jasná její funkce.
<pre>\# Funkční klávesy

\# Mezerník
[space]

\# Pozice a velikost klávesy
x=48
y=146
w=96
h=24

\# Pozice ve virtuální mřížce
posX=1
posY=4

\# Název klávesy
name=Space

[enter]
\# ...

[shift]
\# ...

[backspace]
\# ...

[delete]
\# ...

[leftArrow]
\# ...

[rightArrow]
\# ...
</pre>
 * @subsection KeyboardConfKeys Běžné klávesy
 * Na konci conf souboru jsou uvedeny všechny běžné klávesy. Při neuvedení
 * parametru <tt>name</tt> se pro jejich popis použijí jejich hodnoty (podle
 * aktuálně stisknutých modifikátorů). Hodnoty se zapisují do těchto parametrů:
 * <ul>
 * <li><tt>val</tt> - běžná hodnota</li>
 * <li><tt>shiftVal</tt> - při stisku shift + klávesa</li>
 * <li><tt>special0Val</tt> - 0. speciální + klávesa</li>
 * <li><tt>special0ShiftVal</tt> - 0. speciální + shift + klávesa</li>
 * <li><tt>shiftSpecial0Val</tt> - shift + 0. speciální + klávesa</li>
 * <li><tt>shiftSpecial0ShiftVal</tt> - shift + 0. speciální + shift + klávesa</li>
 * <li><tt>special1Val</tt> - 1. speciální + klávesa</li>
 * <li>...</li>
 * </ul>
<pre>\# Běžné klávesy

[key]

\# Pozice klávesy
x=0
y=74

\# Pozice klávesy ve virtuální mřížce
posX=0
posY=1

\# Běžná hodnota
val=q

\# Shift hodnota
shiftVal=Q

[key]
x=24
y=74
posX=1
posY=1
val=w
shiftVal=W

[key]
x=48
y=74
posX=2
posY=1
val=e
shiftVal=E

\# Hodnoty se stisknutou 1. spec. klávesou (s popiskem '`)
special1Val=é
special1ShiftVal=É

\# Hodnoty se shift + stisknutou 1. spec. klávesou
shiftSpecial1Val=è
shiftSpecial1ShiftVal=È

\# Hodnoty se stisknutou 2. spec. klávesou (s popiskem ˇ^)
special2Val=ě
special2ShiftVal=Ě

\# Hodnoty se shift + stisknutou 2. spec. klávesou
shiftSpecial2Val=ê
shiftSpecial2ShiftVal=Ê

# ... další klávesy ...
</pre>
 * @todo Posunout pod definici třídy (všude - Conf, Skin...)
 */

}}

#endif
