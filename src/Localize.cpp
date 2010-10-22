/*
    Copyright © 2007, 2008, 2009, 2010 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Map2X.

    Map2X is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Map2X is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include "Localize.h"

#include <iostream>

using std::string;      using std::vector;      using std::cout;    using std::endl;

namespace Map2X { namespace Sdl {

/* Destruktor */
Localize::~Localize (void) {
    for(vector<Localization>::const_iterator it = localizations.begin(); it != localizations.end(); ++it) {
        delete (*it).text;
    }
}

/* Načtení lokalizace */
void Localize::load (const string& file, const string& _fallback) {
    /* Pokud načítáme jinou lokalizaci než tu, která už je načtena */
    lang = ConfParser(file);

    /* Fallback jen když ho uživatel specifikoval */
    if(_fallback != "") fallback = ConfParser(_fallback);

    /* Načtení lokalizovaných textů z nového souboru */
    for(vector<Localization>::const_iterator it = localizations.begin(); it != localizations.end(); ++it) {
        (*(*it).text).clear();

        /* Pokus o načtení z primární lokalizace, pak fallback */
        if(lang.value((*it).parameter, *(*it).text, lang.section((*it).section)) == lang.parameterNotFound())
            if(!fallback || fallback.value((*it).parameter, *(*it).text, fallback.section((*it).section)) == fallback.parameterNotFound())
                *(*it).text = "localizeMe{ [" + (*it).section + "] " + (*it).parameter + " }";
    }
}

/* Získání lokalizace */
string* Localize::get (const string& parameter, const string& section) {
    string* text = new string;
    if(lang.value(parameter, *text, lang.section(section)) == lang.parameterNotFound())
        if(!fallback || fallback.value(parameter, *text, fallback.section(section)) == fallback.parameterNotFound())
            *text = "localizeMe{ [" + section + "] " + parameter + " }";

    Localization loc;
    loc.parameter = parameter;
    loc.section = section;
    loc.text = text;
    localizations.push_back(loc);

    return text;
}

}}
