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

/* Destruktor */
Skin::~Skin(void) {
    /* Uvolnění surfaců a ukazatelů na ně */
    for(vector<Skin::Property<SDL_Surface**> >::iterator it = surfaces.begin(); it != surfaces.end(); ++it) {
        if((*(*it).property) != NULL) SDL_FreeSurface((*(*it).property));
        delete (*it).property;
    }

    /* Uvolnění fontů a ukazatelů na ně */
    for(vector<Skin::Property<TTF_Font**> >::iterator it = fonts.begin(); it != fonts.end(); ++it) {
        if((*(*it).property) != NULL) TTF_CloseFont((*(*it).property));
        delete (*it).property;
    }

    /* Smazání ukazatelů na texty */
    for(vector<Skin::Property<string*> >::iterator it = texts.begin(); it != texts.end(); ++it) {
        delete (*it).property;
    }

    /* Smazání ukazatelů na čísla */
    for(vector<Skin::Property<int*> >::iterator it = numbers.begin(); it != numbers.end(); ++it) {
        delete (*it).property;
    }
}

/* Načtení skinu */
void Skin::load (const string& file) {
    /* Pokud načítáme jiný skin, než ten, který už je načten */
    if(conf.getFilename() != file) {
        conf = ConfParser(file);
    }

    /* Načtení surfaců z nového skinu */
    for(vector<Skin::Property<SDL_Surface**> >::iterator it = surfaces.begin(); it != surfaces.end(); ++it) {
        /* Uvolnění starého PŘED načtením nového, aby nevznikla neúnosná špička obsazení paměti */
        if((*(*it).property) != NULL) SDL_FreeSurface((*(*it).property));

        string file;
        conf.value((*it).parameter, file, conf.section((*it).section));

        SDL_Surface* temp = IMG_Load(file.c_str());
        (*(*it).property) = SDL_DisplayFormatAlpha(temp);
        SDL_FreeSurface(temp);
    }

    /* Načtení fontů z nového skinu */
    for(vector<Skin::Property<TTF_Font**> >::iterator it = fonts.begin(); it != fonts.end(); ++it) {
        /* Uvolnění starého PŘED načtením nového, aby nevznikla neúnosná špička obsazení paměti */
        if((*(*it).property) != NULL) TTF_CloseFont((*(*it).property));

        string file;
        conf.value((*it).parameter, file, conf.section((*it).section));
        int fontSize = 0;
        conf.value((*it).parameter + "Size", fontSize, conf.section((*it).section));

        (*(*it).property) = TTF_OpenFont(file.c_str(), fontSize);
    }

    /* Načtení textů z nového skinu */
    for(vector<Skin::Property<string*> >::iterator it = texts.begin(); it != texts.end(); ++it) {
        (*(*it).property).clear();  /* reset, aby nedocházelo k nedefinovaným jevům */
        conf.value((*it).parameter, (*(*it).property), conf.section((*it).section));
    }

    /* Načtení čísel z nového skinu */
    for(vector<Skin::Property<int*> >::iterator it = numbers.begin(); it != numbers.end(); ++it) {
        (*(*it).property) = 0; /* reset, aby nedocházelo k nedefinovaným jevům */
        conf.value((*it).parameter, (*(*it).property), conf.section((*it).section));
    }
}

/* Inicializace a získání ukazatele na surface */
template<> SDL_Surface** Skin::set(const string& parameter, string section) {
    string file;
    conf.value(parameter, file, conf.section(section));

    /* Ukazatel musí být dvojitý, protože funkce IMG_Load() si samy určují, kam
        umístí data a proto by se jednoduchý ukazatel při reloadu surface zničil */
    SDL_Surface** surface = new SDL_Surface*;

    /* Konverze do formátu displeje */
    SDL_Surface* temp = IMG_Load(file.c_str());
    *surface = SDL_DisplayFormatAlpha(temp);
    SDL_FreeSurface(temp);

    Skin::Property<SDL_Surface**> property;
    property.parameter = parameter;
    property.section = section;
    property.property = surface;

    surfaces.push_back(property);

    return surface;
}

/* Inicializace a získání ukazatele na font */
template<> TTF_Font** Skin::set(const string& parameter, string section) {
    string file;
    conf.value(parameter, file, conf.section(section));

    int fontSize = 0;
    conf.value(parameter + "Size", fontSize, conf.section(section));

    TTF_Font** font = new TTF_Font*;
    *font = TTF_OpenFont(file.c_str(), fontSize);

    Skin::Property<TTF_Font**> property;
    property.parameter = parameter;
    property.section = section;
    property.property = font;

    fonts.push_back(property);

    return font;
}

/* Inicalizace a získání ukazatele na string */
template<> string* Skin::set(const string& parameter, string section) {
    string *text = new string;
    conf.value(parameter, *text, conf.section(section));

    Skin::Property<string*> property;
    property.parameter = parameter;
    property.section = section;
    property.property = text;

    texts.push_back(property);

    return text;
}

/* Inicializace a získání ukazatele na číslo */
template<> int* Skin::set(const string& parameter, string section) {
    int *i = new int; *i = 0;
    conf.value(parameter, *i, conf.section(section));

    Skin::Property<int*> property;
    property.parameter = parameter;
    property.section = section;
    property.property = i;

    numbers.push_back(property);

    return i;
}
