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

#include "ConfParser.h"

#include <fstream>
#include <iostream>
#include <sstream>      /* std::istringstream */
#include <cstring>      /* strcmp() */

#include "Utility/utilities.h"

using namespace std;
using namespace Kompas::Utility;

namespace Kompas { namespace Sdl {

const int ConfParser::MAX_LINE_LENGTH = 256;
const std::string ConfParser::DEFAULT_SECTION = "default";

/* Konstruktor */
ConfParser::ConfParser(std::string _file): filename(_file) {
    std::ifstream file(_file.c_str());
    if(!file.good()) {
        cerr << "Nelze otevřít soubor " << _file << "." << endl;
        return;
    }

    char buffer[ConfParser::MAX_LINE_LENGTH];

    /* BOM u UTF-8 */
    if(file.peek() == 0xEF) {

        char bom[4] = {
            (unsigned char) 0xEF, (unsigned char) 0xBB,
            (unsigned char) 0xBF, '\0'
        };
        file.get(buffer, 4);

        /* Tak to BOM nebyl */
        /** @todo zbavit se toho strcmp() */
        if(strcmp(buffer, bom) != 0) {
            file.seekg(0, std::ios::beg);
        }
    }

    /* Naparsování */
    while(file.good()) {
        /* Komentář - musí být na začátku řádku bez mezer */
        if(file.peek() == ';' || file.peek() == '#') {
            file.ignore(1);
            file.getline(buffer, ConfParser::MAX_LINE_LENGTH-1);

            /** @todo Error při nadlimitní velikosti */

            Parameter parameter;
            parameter.parameter = "#";
            parameter.value = buffer;
            parameters.push_back(parameter);

            continue;
        }

        /* Název sekce */
        if(file.peek() == '[') {
            file.ignore(1);
            file.get(buffer, ConfParser::MAX_LINE_LENGTH-1, ']');
            if(file.peek() != ']') {
                cerr << "Neplatný název sekce " << buffer << " v souboru " << filename << "." << endl;
                return;
            }
            file.ignore(ConfParser::MAX_LINE_LENGTH, '\n');

            Parameter parameter;
            parameter.parameter = "[";
            parameter.value = buffer;
            parameters.push_back(parameter);
            continue;
        }

        /* Prázdný řádek nebo jiná píčovina */
        if(file.peek() == '\r' || file.peek() == '\n') {
            file.ignore(1);
            continue;
        }

        /* Odentrování těsně před koncem souboru */
        if(!file.good()) continue;

        /* Získání parametru */
        file.get(buffer, ConfParser::MAX_LINE_LENGTH-1, '=');
        if(file.peek() != '=') {
            cerr << "Neplatný prázdný parametr " << buffer << " v souboru " << filename << " na pozici " << file.tellg() << "." << endl;
            return;
        }
        file.ignore(1);

        /* Inicializace parametru */
        Parameter parameter;
        parameter.parameter = trim(buffer);

        /* Uvozovky */
        if(file.peek() == '"' || file.peek() == '\'') {
            char delimiter = file.peek();
            file.ignore(1);
            file.get(buffer, ConfParser::MAX_LINE_LENGTH-1, delimiter);

            /* Chybějící koncové uvozovky! */
            if(file.peek() != delimiter) {
                cerr << "Neukončené uvozovky v souboru " << filename << " u parametru "
                     << parameter.parameter << " (pozice " << file.tellg() << ")." << endl;
                return;
            } else {
                file.ignore(ConfParser::MAX_LINE_LENGTH, '\n');

                /* U parametru v uvozovkách se mezery neosekávají */
                parameter.value = buffer;
            }
        }

        /* Hodnota bez uvozovek */
        else {
            file.getline(buffer, ConfParser::MAX_LINE_LENGTH-1);
            parameter.value = trim(buffer);
        }

        /* Doplnění hodnoty do parametru */
        parameters.push_back(parameter);

    }

    /* Uzavření souboru */
    file.close();

    /* Rozdělení konfiguráku na sekce */
    reloadSections();
}

/* Operátor přiřazení */
ConfParser& ConfParser::operator= (const ConfParser& conf) {
        /* Ne, vopravdu nebudu kopírovat sama do sebe,
            abych si před sebou zavřel svůj soubor */
        if(&conf != this) {
            destroy();
            filename = conf.filename;
            parameters = conf.parameters;
            reloadSections();
        }

        return *this;
}

/* Uvolnění všech dat */
void ConfParser::destroy(void) {
    parameters.clear();
    sections.clear();
}

/* Nalezení sekcí v konfiguráku */
void ConfParser::reloadSections(void) {
    /* Vyčištění sekcí (aby tam nezůstávaly staré pointery) */
    sections.clear();

    /* Default sekce */
    Section section;
    section.section = ConfParser::DEFAULT_SECTION;
    section.begin = parameters.begin();
    sections.push_back(section);

    for(vector<Parameter>::const_iterator it = parameters.begin(); it != parameters.end(); ++it) {
        if((*it).parameter == "[") {
            Section section;
            section.section = (*it).value;
            section.begin = it+1;
            sections.push_back(section);
        }
    }
}

/* Nalezení sekce */
ConfParser::sectionPointer ConfParser::section(const string& name, ConfParser::sectionPointer begin, int flags) const {
    for(ConfParser::sectionPointer it = begin; it != sections.end(); ++it) {
        if((*it).section == name) return it;
    }

    /* Nic nenalezeno, pokud hledáme poprvé, chybové hlášení */
    if(begin == sections.begin() && !(flags & SUPPRESS_ERRORS))
        cerr << "Sekce '" << name << "' nebyla v souboru '" << filename << "' nalezena." << endl;

    return sections.end();
}

/* Nalezení textové hodnoty parametru */
template<> ConfParser::parameterPointer ConfParser::value(const string& parameter, string& _value, ConfParser::sectionPointer section, ConfParser::parameterPointer begin, int flags) const {
    /* Nenalezená sekce */
    if(section == sections.end()) return parameters.end();

    /* Spočtení konce sekce */
    ConfParser::parameterPointer end = (section == sections.end()-1) ? parameters.end() : (*(section+1)).begin;

    /* Procházení sekce */
    for(ConfParser::parameterPointer it = begin; it != end; ++it) {
        if((*it).parameter == parameter) {
            _value = (*it).value;
            return it;
        }
    }

    /* Nic nenalezeno, pokud hledáme poprvé, vyhození hlášky */
    if(begin == (*section).begin && !(flags & SUPPRESS_ERRORS))
        cerr << "Hodnota parametru '" << parameter << "' nebyla v sekci [" << (*section).section << "] souboru '" << filename << "' nalezena." << endl;

    return parameters.end();
}

/* Nalezení zarovnání */
template<> ConfParser::parameterPointer ConfParser::value(const string& parameter, Align& _value, ConfParser::sectionPointer section, ConfParser::parameterPointer begin, int flags) const {
    string __value;
    ConfParser::parameterPointer position = value(parameter, __value, section, begin);

    /* pokud bylo něco nalezeno */
    if(position != parameters.end()) {
        istringstream str(__value);
        string keyword;
        _value = (Align) 0;
        while(str >> keyword) {
                 if(keyword == "left")      _value = (Align) (_value | ALIGN_LEFT);
            else if(keyword == "center")    _value = (Align) (_value | ALIGN_CENTER);
            else if(keyword == "right")     _value = (Align) (_value | ALIGN_RIGHT);
            else if(keyword == "top")       _value = (Align) (_value | ALIGN_TOP);
            else if(keyword == "middle")    _value = (Align) (_value | ALIGN_MIDDLE);
            else if(keyword == "bottom")    _value = (Align) (_value | ALIGN_BOTTOM);
        }
    }

    return position;
}

/* Nalezení int / double hodnoty parametru */
template<class Value> ConfParser::parameterPointer ConfParser::value(const string& parameter, Value& _value, ConfParser::sectionPointer section, ConfParser::parameterPointer begin, int flags) const {
    string __value;
    ConfParser::parameterPointer position = value(parameter, __value, section, begin);

    /* pokud bylo něco nalezeno */
    if(position != parameters.end()) {
        istringstream str(__value);

        /* Hexadecimální hodnota */
        if(flags == ConfParser::HEX)    str >> std::hex >> _value;

        /* Barva ve formátu #FF3366 / FF3366 */
        else if(flags == ConfParser::COLOR) {
            if(str.peek() == '#') str.ignore(1);
            str >> std::hex >> _value;
            /** @todo Barvy ve formátu 255,255,0 */
        }

        /* int nebo double */
        else                            str >> _value;
    }

    return position;
}

#ifndef GENERATING_DOXYGEN_OUTPUT
/* Předdefinování určitě používaných template, aby linker neházel chyby o tom,
    že v knihovně taková template nejsou instancovaná */
template ConfParser::parameterPointer ConfParser::value<int>(const string&, int&, ConfParser::sectionPointer, ConfParser::parameterPointer, int) const;
template ConfParser::parameterPointer ConfParser::value<short>(const string&, short&, ConfParser::sectionPointer, ConfParser::parameterPointer, int) const;
template ConfParser::parameterPointer ConfParser::value<unsigned short>(const string&, unsigned short&, ConfParser::sectionPointer, ConfParser::parameterPointer, int) const;
template ConfParser::parameterPointer ConfParser::value<double>(const string&, double&, ConfParser::sectionPointer, ConfParser::parameterPointer, int) const;
#endif

/* Nalezení bool hodnoty parametru */
template<> ConfParser::parameterPointer ConfParser::value(const string& parameter, bool& _value, ConfParser::sectionPointer section, ConfParser::parameterPointer begin, int flags) const {
    string __value;
    ConfParser::parameterPointer position = value(parameter, __value, section, begin);

    /* pokud bylo něco nalezeno */
    if(position != parameters.end()) {
        if(__value == "true" || __value == "yes" || __value == "1" ||
           __value == "y"    || __value == "t") _value = true;
        else _value = false;
    }

    return position;
}

}}
