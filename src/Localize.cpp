/*
    Copyright © 2007, 2008, 2009, 2010, 2011 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Kompas.

    Kompas is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Kompas is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include "Localize.h"

#include <iostream>

using namespace std;
using namespace Kompas::Utility;

namespace Kompas { namespace Sdl {

Localize::~Localize (void) {
    for(vector<Localization>::const_iterator it = localizations.begin(); it != localizations.end(); ++it) {
        delete (*it).text;
    }
}

void Localize::load (const string& file, const string& _fallback) {
    /* Load new language file and fallback, if specified */
    lang = Configuration(file, Configuration::ReadOnly);
    if(_fallback != "") fallback = Configuration(_fallback, Configuration::ReadOnly);

    /* Reload all localizations from new files */
    for(vector<Localization>::const_iterator it = localizations.begin(); it != localizations.end(); ++it) {
        (*(*it).text).clear();

        /* If not found in primary language or in fallback, return error text */
        get(it->key, it->group, it->text, &lang);
    }
}

string* Localize::get (const string& key, const string& group) {
    string* text = new string;
    get(key, group, text, &lang);

    Localization loc;
    loc.key = key;
    loc.group = group;
    loc.text = text;
    localizations.push_back(loc);

    return text;
}

bool Localize::get(const string& key, const string& group, string* text, const Configuration* file) const {
    const ConfigurationGroup* g;
    if(group.empty()) g = file;
    else g = file->group(group);

    /* Value not found */
    if(!g->value(key, text)) {
        /* Try fallback */
        if(file == &lang) return get(key, group, text, &fallback);

        /* If not found in fallback, return error text */
        *text = "localizeMe{ [" + group + "] " + key + " }";
        return false;
    }

    return true;
}

}}
