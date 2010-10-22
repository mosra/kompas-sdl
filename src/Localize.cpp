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

using namespace std;

namespace Map2X { namespace Sdl {

Localize::~Localize (void) {
    for(vector<Localization>::const_iterator it = localizations.begin(); it != localizations.end(); ++it) {
        delete (*it).text;
    }
}

void Localize::load (const string& file, const string& _fallback) {
    /* Load new language file and fallback, if specified */
    lang = ConfParser(file);
    if(_fallback != "") fallback = ConfParser(_fallback);

    /* Reload all localizations from new files */
    for(vector<Localization>::const_iterator it = localizations.begin(); it != localizations.end(); ++it) {
        (*(*it).text).clear();

        /* If not found in primary language file, get from fallback */
        if(lang.value((*it).parameter, *(*it).text, lang.section((*it).section)) == lang.parameterNotFound())
            if(!fallback || fallback.value((*it).parameter, *(*it).text, fallback.section((*it).section)) == fallback.parameterNotFound())
                *(*it).text = "localizeMe{ [" + (*it).section + "] " + (*it).parameter + " }";
    }
}

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
