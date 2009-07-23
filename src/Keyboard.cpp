#include "Keyboard.h"

#include <iostream>
#include <sstream>

#include "ConfParser.h"
#include "Effects.h"
#include "Skin.h"

using std::string;  using std::vector;  using std::cout;    using std::endl;
using std::cerr;
using namespace MToolkit;

#ifndef GENERATING_DOXYGEN_OUTPUT
/* Předdefinování určitě používaných template, aby linker neházel chyby o tom,
    že v knihovně taková template nejsou instancovaná */
#include "Matrix.cpp"
template class Matrix<MInterface::KeyboardKey>;
#endif

namespace MInterface {

/* Konstruktor */
Keyboard::Keyboard(SDL_Surface* _screen, Skin& _skin, std::string file, std::string& _text): screen(_screen), skin(_skin), text(_text), shiftPushed(false) {
    /* Otevření conf souboru */
    ConfParser keyboard(file);

    /* Sekce skinu, ve které hledat věci pro tuto klávesnici */
    string skinSection;
    keyboard.value("skinSection", skinSection);

    /* Načtení věcí ze skinu */
    keyboardX = skin.get<int*>("x", skinSection);
    keyboardY = skin.get<int*>("x", skinSection);
    align = skin.get<Align*>("align", skinSection);
    image = skin.get<SDL_Surface**>("image", skinSection);

    textFont = skin.get<TTF_Font**>("textFont", skinSection);
    textColor = skin.get<SDL_Color*>("textColor", skinSection);

    keyAlign = skin.get<Align*>("keyAlign", skinSection);
    keyFont = skin.get<TTF_Font**>("keyFont", skinSection);
    keyColor = skin.get<SDL_Color*>("keyColor", skinSection);
    keyActiveColor = skin.get<SDL_Color*>("keyActiveColor", skinSection);
    keySpecialActiveColor = skin.get<SDL_Color*>("keySpecialActiveColor", skinSection);
    SDL_Surface** keyImage = skin.get<SDL_Surface**>("keyImage", skinSection);
    SDL_Surface** keyActiveImage = skin.get<SDL_Surface**>("keyActiveImage", skinSection);
    SDL_Surface** keySpecialImage = skin.get<SDL_Surface**>("keySpecialImage", skinSection);
    SDL_Surface** keySpecialActiveImage = skin.get<SDL_Surface**>("keySpecialActiveImage", skinSection);

    /* Načtení globálních věcí z konfiguráku klávesnice */
    keyboard.value("w", keyboardW);
    keyboard.value("h", keyboardH);

    keyboard.value("textX", textPosition.x);
    keyboard.value("textY", textPosition.y);
    keyboard.value("textW", textPosition.w);
    keyboard.value("textH", textPosition.h);
    keyboard.value("textAlign", textAlign);

    /* keyW a keyH budu doplňovat do každé klávesy, proto nejsou v žádné vlastnosti třídy */
    int keyW;   keyboard.value("keyW", keyW);
    int keyH;   keyboard.value("keyH", keyH);

    /* Speciální klávesy */
    ConfParser::sectionPointer section = keyboard.section("specialKey");
    while(section != keyboard.sectionNotFound()) {
        KeyboardKey key;
        keyboard.value("x", key.position.x, section);
        keyboard.value("y", key.position.y, section);
        key.position.w = keyW;
        key.position.h = keyH;

        keyboard.value("posX", key.x, section);
        keyboard.value("posY", key.y, section);
        keyboard.value("name", key.name, section, ConfParser::SUPPRESS_ERRORS);

        string temp;
        keyboard.value("val", temp, section, ConfParser::SUPPRESS_ERRORS);
        key.values.push_back(temp);
        keyboard.value("shiftVal", temp, section, ConfParser::SUPPRESS_ERRORS);
        key.values.push_back(temp);

        /* Pokud není parametr name ani val, chyba */
        if(key.name == "" && key.values[0] == "")
            cerr << "U speciální klávesy na pozici [" << key.x << ";" << key.y << "] chybí popisek i hodnota!" << endl;

        key.image = keySpecialImage;
        key.activeImage = keySpecialActiveImage;
        key.flags = SPECIAL;

        items.push_back(key);
        section = keyboard.section("specialKey", section+1);
    }

    /* Počet speciálních kláves */
    vector<KeyboardKey>::size_type specialKeyCount = items.size();

    /* Mezerník */
    section = keyboard.section("space");
    KeyboardKey space;
    keyboard.value("x", space.position.x, section);
    keyboard.value("y", space.position.y, section);
    keyboard.value("w", space.position.w, section);
    keyboard.value("h", space.position.h, section);
    keyboard.value("posX", space.x, section);
    keyboard.value("posY", space.y, section);
    keyboard.value("name", space.name, section, ConfParser::SUPPRESS_ERRORS);
    space.image = skin.get<SDL_Surface**>("spaceImage", skinSection);
    space.activeImage = skin.get<SDL_Surface**>("spaceActiveImage", skinSection);
    for(vector<KeyboardKey>::size_type i = 0; i != specialKeyCount*4; ++i)
        space.values.push_back(" ");
    space.flags = 0;
    items.push_back(space);

    /* Enter */
    section = keyboard.section("enter");
    KeyboardKey enter;
    keyboard.value("x", enter.position.x, section);
    keyboard.value("y", enter.position.y, section);
    keyboard.value("w", enter.position.w, section);
    keyboard.value("h", enter.position.h, section);
    keyboard.value("posX", enter.x, section);
    keyboard.value("posY", enter.y, section);
    keyboard.value("name", enter.name, section, ConfParser::SUPPRESS_ERRORS);
    enter.image = skin.get<SDL_Surface**>("enterImage", skinSection);
    enter.activeImage = skin.get<SDL_Surface**>("enterActiveImage", skinSection);
    enter.flags = ENTER;
    items.push_back(enter);

    /* Shift */
    section = keyboard.section("shift");
    KeyboardKey shift;
    keyboard.value("x", shift.position.x, section);
    keyboard.value("y", shift.position.y, section);
    keyboard.value("w", shift.position.w, section);
    keyboard.value("h", shift.position.h, section);
    keyboard.value("posX", shift.x, section);
    keyboard.value("posY", shift.y, section);
    keyboard.value("name", shift.name, section, ConfParser::SUPPRESS_ERRORS);
    shift.image = skin.get<SDL_Surface**>("shiftImage", skinSection);
    shift.activeImage = skin.get<SDL_Surface**>("shiftActiveImage", skinSection);
    shift.flags = SHIFT;
    items.push_back(shift);

    /* Běžné klávesy */
    section = keyboard.section("key");
    while(section != keyboard.sectionNotFound()) {
        KeyboardKey key;
        keyboard.value("x", key.position.x, section);
        keyboard.value("y", key.position.y, section);
        key.position.w = keyW;
        key.position.h = keyH;

        keyboard.value("posX", key.x, section);
        keyboard.value("posY", key.y, section);
        keyboard.value("name", key.name, section, ConfParser::SUPPRESS_ERRORS);

        /*
        Hodnoty klávesy:
        - pokud není definována shift hodnota, je stejná jako normální
        - pokud není definována special normální hodnota, je stejná jako normální
        - pokud není definována shiftSpecial normální hodnota, je stejná jako normální
        - pokud není definována special shift hodnota, je stejná jako shift
        - pokud není definována shiftSpecial shift hodnota, je stejná jako shift
        */
        string temp;
        keyboard.value("val", temp, section);
        key.values.push_back(temp);
        keyboard.value("shiftVal", temp, section, ConfParser::SUPPRESS_ERRORS);
        key.values.push_back(temp);

        /* Speciální klávesy (je jich items.size()-3 (enter, space a shift) */
        for(vector<KeyboardKey>::size_type i = 0; i != specialKeyCount; ++i) {
            std::ostringstream number;
            number << i;

            temp = key.values[0];
            keyboard.value("special" + number.str() + "Val", temp, section, ConfParser::SUPPRESS_ERRORS);
            key.values.push_back(temp);
            temp = key.values[1];
            keyboard.value("special" + number.str() + "ShiftVal", temp, section, ConfParser::SUPPRESS_ERRORS);
            key.values.push_back(temp);

            temp = key.values[0];
            keyboard.value("shiftSpecial" + number.str() + "Val", temp, section, ConfParser::SUPPRESS_ERRORS);
            key.values.push_back(temp);
            temp = key.values[1];
            keyboard.value("shiftSpecial" + number.str() + "ShiftVal", temp, section, ConfParser::SUPPRESS_ERRORS);
            key.values.push_back(temp);
        }

        key.image = keyImage;
        key.activeImage = keyActiveImage;
        key.flags = 0;

        items.push_back(key);
        section = keyboard.section("key", section+1);
    }

    /* Inicializace ukazatelů na stisknuté spec. klávesy */
    specialPushed = items.end();
    specialShiftPushed = items.end();

    /* Reload položek */
    reloadItems();
}

/* Kliknutí myší */
bool Keyboard::click(int x, int y, int& action) {
    /* Oblast klávesnice */
    SDL_Rect area = Effects::align(screen, *align, keyboardW, keyboardH, *keyboardX, *keyboardY);

    if(!inArea(x, y, area)) return false;

    /* Procházení jednotlivých kláves */
    for(vector<KeyboardKey>::const_iterator it = items.begin(); it != items.end(); it++) {
        /* Nalezeno */
        if(!((*it).flags & DISABLED) && inArea(x, y, Effects::align(area, ALIGN_DEFAULT, (*it).position))) {
            actualItem = it;
            select();
            return true;
        }
    }

    return true;
}

/* Vybrání znaku */
void Keyboard::select(void) {
    /* Shift */
    if((*actualItem).flags & SHIFT) {
        /* Cyklické zapínání / vypínání shiftu */
        shiftPushed = shiftPushed ? false : true;

    /* Speciální klávesy */
    } else if((*actualItem).flags & SPECIAL) {
        /* Cyklické zapínání / vypínání */
        if(specialPushed == actualItem) specialPushed = items.end();
        else if(specialShiftPushed == actualItem) specialShiftPushed = items.end();

        else {
            /* Vypnutí již stisknuté spec. klávesy */
            if(specialPushed != items.end() || specialShiftPushed != items.end()) {
                specialPushed = items.end();
                specialShiftPushed = items.end();
            }

            /* Stlačeno v kombinaci se shiftem, vypnutí shiftu */
            if(shiftPushed) {
                specialShiftPushed = actualItem;
                shiftPushed = false;
            }

            /* Stlačeno samostatně */
            else specialPushed = actualItem;
        }

    /* Enter */
    } else if((*actualItem).flags & ENTER) {

    /* Běžné klávesy */
    } else {
        int valuePosition = 0;

        /* Stisknutá spec. klávesa */
        if(specialPushed != items.end())
            valuePosition = 2+4*(specialPushed-items.begin());
        if(specialShiftPushed != items.end())
            valuePosition = 4+4*(specialShiftPushed-items.begin());

        /* Shift */
        if(shiftPushed) valuePosition++;

        text += (*actualItem).values[valuePosition];

        /* Reset modofikátorů */
        shiftPushed = false;
        specialPushed = items.end();
        specialShiftPushed = items.end();
    }
}

/* Zobrazení klávesnice */
void Keyboard::view(void) {
    /* Plocha pro vykreslování klávesnice */
    SDL_Rect area = Effects::align(screen, *align, keyboardW, keyboardH, *keyboardX, *keyboardY);

    /* Pozadí */
    SDL_BlitSurface(*image, NULL, screen, &area);

    /* Editovaný text */
    SDL_Rect _textPosition = Effects::align(area, ALIGN_DEFAULT, textPosition);
    SDL_Surface* _text = (*Effects::textRenderFunction())(*textFont, text.c_str(), *textColor);

    _textPosition = Effects::align(_textPosition, textAlign, (*_text).w, (*_text).h);
    /** @todo Ořezy tak, aby bylo vidět co píšu */
    SDL_Rect textCrop = {0, 0, _textPosition.w, _textPosition.h};
    SDL_BlitSurface(_text, &textCrop, screen, &_textPosition);
    SDL_FreeSurface(_text);

    /* Jednotlivé klávesy */
    for(vector<KeyboardKey>::const_iterator it = items.begin(); it != items.end(); ++it) {
        /* Plocha pro klávesu */
        SDL_Rect keyArea = Effects::align(area, ALIGN_DEFAULT, (*it).position);

        /* Pozadí */
        SDL_Surface* image = *(*it).image;
        if(it == actualItem) image = *(*it).activeImage;
        SDL_BlitSurface(image, NULL, screen, &keyArea);

        /* Popisek - Pokud je nastaveno jméno klávesy, bude vypsáno vždy */
        string label;
        if((*it).name != "") label = (*it).name;

        /* Jinak se vypisuje podle toho, jaké modifikátory jsou aktivní a jestli existuje nějaká hodnota */
        else {
            vector<string>::size_type valuePosition = 0;

            /* Stisknutá spec. klávesa */
            if(specialPushed != items.end())
                valuePosition = 2+4*(specialPushed-items.begin());
            if(specialShiftPushed != items.end())
                valuePosition = 4+4*(specialShiftPushed-items.begin());

            /* Shift */
            if(shiftPushed) valuePosition++;

            label = (*it).values[valuePosition];
        }

        /* Pokud je vůbec nějaký popisek */
        if(label != "") {
            /* Barva popisku */
            SDL_Color color;

            /* Stlačený modifikátor */
            if(((*it).flags & SHIFT && shiftPushed) ||
               it == specialPushed || it == specialShiftPushed)
                color = *keySpecialActiveColor;

            /* Aktuální klávesa */
            else if(it == actualItem) color = *keyActiveColor;

            /* Běžná klávesa */
            else color = *keyColor;

            _text = (*Effects::textRenderFunction())(*keyFont, label.c_str(), color);

            _textPosition = Effects::align(keyArea, *keyAlign, (*_text).w, (*_text).h);
            SDL_Rect _textCrop = {0, 0, _textPosition.w, _textPosition.h};
            SDL_BlitSurface(_text, &_textCrop, screen, &_textPosition);
            SDL_FreeSurface(_text);
        }
    }
}

}
