#include "Localize.h"

using std::string;      using std::vector;      using std::cout;    using std::endl;

/* Destruktor */
Localize::~Localize (void) {
    for(vector<Localization>::const_iterator it = localizations.begin(); it != localizations.end(); ++it) {
        delete (*it).text;
    }
}

/* Načtení lokalizace */
void Localize::load (const string& file, const string& _fallback) {
    /* Pokud načítáme jinou lokalizaci než tu, která už je načtena */
    if(lang.getFilename() != file || fallback.getFilename() != file) {
        if(lang.getFilename() != file) lang = ConfParser(file);
        if(fallback.getFilename() != _fallback) fallback = ConfParser(fallback);
    }

    /* Načtení lokalizovaných textů z nového souboru */
    for(vector<Localization>::const_iterator it = localizations.begin(); it != localizations.end(); ++it) {
        (*(*it).text).clear();

        /* Pokus o načtení z primární lokalizace, pak fallback */
        if(lang.value((*it).parameter, *(*it).text, lang.section((*it).section)) == lang.parameterNotFound())
            if(fallback.value((*it).parameter, *(*it).text, fallback.section((*it).section)) == fallback.parameterNotFound())
                *(*it).text = "localizeMe{ [" + (*it).section + "] " + (*it).parameter + " }";
    }
}

/* Získání lokalizace */
string* Localize::get (const string& parameter, const string& section) {
    string* text = new string;
    if(lang.value(parameter, *text, lang.section(section)) == lang.parameterNotFound())
        if(fallback.value(parameter, *text, fallback.section(section)) == fallback.parameterNotFound())
            *text = "localizeMe{ [" + section + "] " + parameter + " }";

    Localization loc;
    loc.parameter = parameter;
    loc.section = section;
    loc.text = text;
    localizations.push_back(loc);

    return text;
}

