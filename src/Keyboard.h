#ifndef MINTERFACE_KEYBOARD_H
#define MINTERFACE_KEYBOARD_H

/**
 * @file Keyboard.h
 * @brief Třída Keyboard
 */

#include <vector>
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "Matrix.h"
#include "Mouse.h"
#include "utility.h"

namespace MInterface {

class Skin;

/**
 * @page Keyboard Klávesnice na obrazovce
 * Klávesnice je určena pro editaci textu. Plně konfigurovatelná pomocí conf
 * souborů, skinovatelnost a plná podpora UTF-8.
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

\# Pozadí mezerníku, enteru, shiftu a jejich pozadí, když jsou vybrané
spaceImage=gfx/keySpace.png
spaceActiveImage=gfx/keySpaceActive.png
enterImage=gfx/keyEnter.png
enterActiveImage=gfx/keyEnterActive.png
shiftImage=gfx/keyEnter.png
shiftActiveImage=gfx/keyEnterActive.png
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
 * Conf soubor pokračuje nastavením funkčních kláves - kláves Space, Shift a
 * Enter. Jejich účel je jasný z názvů. Tyto klávesy mají většinou jiné rozměry
 * než ostatní klávesy, proto má každá své parametry pro určení velikosti.
 * Jejich pozadí je specifikováno v příslušné sekci skinu. Parametr <tt>name</tt>
 * není povinný, pokud je z pozadí klávesy dostatečně jasná její funkce.
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

\# Klávesa enter
[enter]
x=264
y=146
w=48
h=24
posX=6
posY=4
name=Enter

\# Klávesa shift
[shift]
x=0
y=146
w=48
h=24
posX=0
posY=4
name=Shift
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
 */

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
 * ovládání.
 * @todo Přepínání jiné klávesnice a dalšího textu ke zpracování
 * @todo Nadpisek klávesnice
 * @todo Propojení klávesnice a skinu!
 * @todo Duplicitní ConfParser <=> Skin
 */
class Keyboard: public MToolkit::Matrix<KeyboardKey>, public MInterface::Mouse {
    public:
        /**
         * @brief Konstruktor
         *
         * Načte klávesnici z conf souboru
         * @param   screen  Displejová surface
         * @param   _skin   Reference na otevřenou skin
         * @param   file    Soubor s daty klávesnice
         * @param   _text   Ukazatel na proměnnou s textem ke zpracování
         */
        Keyboard(SDL_Surface* screen, Skin& _skin, std::string file, std::string& _text);

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

        /** @brief Zobrazení klávesnice */
        void view(void);
    private:
        /**
         * @brief Flags klávesy
         */
        enum KeyFlags {
            DISABLED = 0x01,    /**< @brief Klávesa je zakázána (nutné pro Matrix) */
            SPECIAL = 0x04,     /**< @brief Klávesa je speciální */
            SHIFT = 0x08,       /**< @brief Klávesa je Shift */
            ENTER = 0x10        /**< @brief Klávesa je Enter */
        };

        SDL_Surface* screen;    /**< @brief Displejová surface */
        Skin& skin;             /**< @brief Globální skin */
        std::string& text;      /**< @brief Text ke zpracování */

        int keyboardW,          /**< @brief Šířka klávesnice (z konfiguráku) */
            keyboardH;          /**< @brief Výška klávesnice (z konfiguráku) */
        SDL_Rect textPosition;  /**< @brief Pozice zpracovávaného textu (z konfiguráku) */
        MToolkit::Align textAlign;  /**< @brief Zarovnání textu (z konfiguráku) */

        int *keyboardX,         /**< @brief X-ová pozice klávesnice (ze skinu) */
            *keyboardY;         /**< @brief Y-ová pozice klávesnice (ze skinu) */
        MToolkit::Align *align; /**< @brief Zarovnání klávesnice (ze skinu) */
        SDL_Surface **image;    /**< @brief Pozadí klávesnice (ze skinu) */

        TTF_Font **textFont;    /**< @brief Barva textu (ze skinu) */
        SDL_Color *textColor;   /**< @brief Barva textu (ze skinu) */

        MToolkit::Align *keyAlign;  /**< @brief Zarovnání popisku klávesy (ze skinu) */
        TTF_Font **keyFont;     /**< @brief Font popisků kláves (ze skinu) */
        SDL_Color *keyColor,    /**< @brief Barva popisků kláves (ze skinu) */
            *keyActiveColor,    /**< @brief Barva popisku aktivní klávesy (ze skinu) */
            *keySpecialActiveColor; /**< @brief Barva popisku stlačené spec. klávesy (ze skinu) */

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

}

#endif
