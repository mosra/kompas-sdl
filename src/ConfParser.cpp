#include "ConfParser.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>      /* std::istringstream */
#include <cstring>      /** @todo zbavit se toho strcmp() */
#include <cstdlib>

#include "utility.h"

using std::string;      using std::cout;            using std::cerr;
using std::vector;      using std::istringstream;   using std::endl;
using std::hex;

const int ConfParser::MAX_LINE_LENGTH = 256;
const std::string ConfParser::DEFAULT_SECTION = "default";

/* Konstruktor */
ConfParser::ConfParser(std::string _file): filename(_file) {
    file.open(_file.c_str());
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
                cerr << "Neplatná sekce " << buffer << " v souboru " << filename << "." << endl;
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
            cerr << "Neplatný parametr " << buffer << " v souboru " << filename << " na pozici " << file.tellg() << "." << endl;
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
            file.ignore(ConfParser::MAX_LINE_LENGTH, '\n');
        }

        /* Hodnota bez uvozovek */
        else file.getline(buffer, ConfParser::MAX_LINE_LENGTH-1);

        /* Doplnění hodnoty do parametru */
        parameter.value = trim(buffer);
        parameters.push_back(parameter);

    }

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
            file.open(filename.c_str());
            parameters = conf.parameters;
            reloadSections();
        }

        return *this;
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

    /* Koncová sekce */
    section.section = ConfParser::DEFAULT_SECTION;
    section.begin = parameters.end();
    sections.push_back(section);
}

/* Nalezení sekce */
ConfParser::sectionPointer ConfParser::section(const string& name, ConfParser::sectionPointer begin) {
    for(ConfParser::sectionPointer it = begin; it != sections.end(); ++it) {
        if((*it).section == name) return it;
    }

    return sections.end();
}

/* Nalezení textové hodnoty parametru */
template<> ConfParser::parameterPointer ConfParser::value(const string& parameter, string& _value, ConfParser::sectionPointer section, ConfParser::parameterPointer begin, int flags) {
    /* Nenalezená sekce */
    if(section == sections.end()) return parameters.end();

    /* Spočtení konce sekce */
    ConfParser::parameterPointer end = (*++section).begin;


    /* Procházení sekce */
    for(ConfParser::parameterPointer it = begin; it != end; ++it) {
        if((*it).parameter == parameter) {
            _value = (*it).value;
            return it;
        }
    }

    return parameters.end();
}

/* Nalezení int / double hodnoty parametru */
template<class Value> ConfParser::parameterPointer ConfParser::value(const string& parameter, Value& _value, ConfParser::sectionPointer section, ConfParser::parameterPointer begin, int flags) {
    string __value;
    ConfParser::parameterPointer position = value(parameter, __value, section, begin);

    /* pokud bylo něco nalezeno */
    if(position != parameters.end()) {
        istringstream str(__value);
       /* if(flags == ConfParser::HEX)    str >> std::hex >> _value;
        else    */                        str >> _value;
    }

    return position;
}

/* Předdefinování určitě používaných template, aby linker neházel chyby o tom,
    že v knihovně taková template nejsou instanciovaná */
template ConfParser::parameterPointer ConfParser::value<int>(const string&, int&, ConfParser::sectionPointer, ConfParser::parameterPointer, int);
template ConfParser::parameterPointer ConfParser::value<double>(const string&, double&, ConfParser::sectionPointer, ConfParser::parameterPointer, int);

/* Nalezení bool hodnoty parametru */
template<> ConfParser::parameterPointer ConfParser::value(const string& parameter, bool& _value, ConfParser::sectionPointer section, ConfParser::parameterPointer begin, int flags) {
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