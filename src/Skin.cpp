#include "Skin.h"

#include <iostream>
#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "ConfParser.h"

using std::string;  using std::cout;    using std::endl;    using std::cerr;
using std::vector;

/* Načtení skinu */
void Skin::load (const string& file) {
    /* Pokud načítáme jiný skin, než ten, který už je načten */
    if(conf.getFilename() != file) {
        conf = ConfParser(file);
    }

    /* Uvolnění čehokoli předtím */
    free();

    /* Načtení surfaců z nového skinu */
    for(vector<Skin::Property<SDL_Surface*> >::iterator it = surfaces.begin(); it != surfaces.end(); ++it) {
        string file;
        conf.value((*it).parameter, file, conf.section((*it).section));

        SDL_Surface* temp = IMG_Load(file.c_str());
        (*it).property = SDL_DisplayFormatAlpha(temp);
        SDL_FreeSurface(temp);
    }



    /* Načtení fontů z nového skinu */
    for(vector<Skin::Property<TTF_Font*> >::iterator it = fonts.begin(); it != fonts.end(); ++it) {
        string file;
        conf.value((*it).parameter, file, conf.section((*it).section));
        int fontSize = 0;
        conf.value((*it).parameter + "Size", fontSize, conf.section((*it).section));

        (*it).property = TTF_OpenFont(file.c_str(), fontSize);
    }

    /* Načtení čísel z nového skinu */
    for(vector<Skin::Property<int> >::iterator it = numbers.begin(); it != numbers.end(); ++it) {
        (*it).property = 0; /* inicializace, aby nedocházelo k nedefinovaným jevům */
        conf.value((*it).parameter, (*it).property, conf.section((*it).section));
    }

    /* Načtení textu z nového skinu */
    for(vector<Skin::Property<string> >::iterator it = texts.begin(); it != texts.end(); ++it) {
        conf.value((*it).parameter, (*it).property, conf.section((*it).section));
    }
}

/* Uvolnění starého skinu */
void Skin::free(void) {
    /* Uvolnění surfaců */
    for(vector<Skin::Property<SDL_Surface*> >::iterator it = surfaces.begin(); it != surfaces.end(); ++it) {
        if((*it).property != NULL) SDL_FreeSurface((*it).property);
    }

    /* Uvolnění fontů */
    for(vector<Skin::Property<TTF_Font*> >::iterator it = fonts.begin(); it != fonts.end(); ++it) {
        if((*it).property != NULL) TTF_CloseFont((*it).property);
    }

    /* Smazání textů */
    for(vector<Skin::Property<string> >::iterator it = texts.begin(); it != texts.end(); ++it) {
        (*it).property.clear();
    }
}

/* Inicializace vlastnosti */
int Skin::set(Skin::propertyType type, const string& parameter, string section) {
    /* Surface */
    if(type == Skin::SURFACE) {
        string file;
        conf.value(parameter, file, conf.section(section));

        Skin::Property<SDL_Surface*> property;
        property.parameter = parameter;
        property.section = section;
        property.property = IMG_Load(file.c_str());

        surfaces.push_back(property);

        return surfaces.size()-1;
    }

    /* Font */
    if(type == Skin::FONT) {
        string file;
        conf.value(parameter, file, conf.section(section));

        int fontSize = 0;
        conf.value(parameter + "Size", fontSize, conf.section(section));

        Skin::Property<TTF_Font*> property;
        property.parameter = parameter;
        property.section = section;
        property.property = TTF_OpenFont(file.c_str(), fontSize);

        fonts.push_back(property);

        return fonts.size()-1;
    }

    /* Číslo */
    if(type == Skin::NUMBER) {
        int i = 0;
        conf.value(parameter, i, conf.section(section));

        Skin::Property<int> property;
        property.parameter = parameter;
        property.section = section;
        property.property = i;

        numbers.push_back(property);

        return numbers.size()-1;
    }

    /* Text (defaultní) */
    string text;
    conf.value(parameter, text, conf.section(section));

    Skin::Property<string> property;
    property.parameter = parameter;
    property.section = section;
    property.property = text;

    texts.push_back(property);

    return texts.size()-1;
}

/* Získání ukazatele na vlastnost */
/** @todo Není zde kontrola správnosti id kvůli bloatu a rychlosti inline */
template<> SDL_Surface** Skin::get(int id) {
    return &surfaces[id].property;
}
template<> TTF_Font** Skin::get(int id) {
    return &fonts[id].property;
}
template<> string* Skin::get(int id) {
    return &texts[id].property;
}
template<> int* Skin::get(int id) {
    return &numbers[id].property;
}
