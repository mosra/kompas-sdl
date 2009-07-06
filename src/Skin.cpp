#include "Skin.h"

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
    for(vector<Skin::Property<string*> >::iterator it = texts.begin(); it != texts.end(); ++it)
        delete (*it).property;

    /* Smazání ukazatelů na barvy */
    for(vector<Skin::Property<SDL_Color*> >::iterator it = colors.begin(); it != colors.end(); ++it)
        delete (*it).property;

    /* Smazání ukazatelů na čísla */
    for(vector<Skin::Property<int*> >::iterator it = numbers.begin(); it != numbers.end(); ++it)
        delete (*it).property;

    /* Smazání ukazatelů na zarovnání */
    for(vector<Skin::Property<Align*> >::iterator it = aligns.begin(); it != aligns.end(); ++it)
        delete (*it).property;
}

/* Načtení skinu */
void Skin::load (const string& file) {
    /* Pokud načítáme jiný skin, než ten, který už je načten */
    if(conf.getFilename() != file) {
        conf = ConfParser(file);
    }

    /* Vypnění černou barvou, aby nezůstávaly artefakty */
    SDL_FillRect(screen, NULL, SDL_MapRGB((*screen).format, 0, 0, 0));

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

    /* Načtení barev z nového skinu */
    for(vector<Skin::Property<SDL_Color*> >::iterator it = colors.begin(); it != colors.end(); ++it) {
        int col = 0;
        conf.value((*it).parameter, col, conf.section((*it).section), ConfParser::COLOR);

        (*(*it).property).r = (col & 0xFF0000) >> 16;
        (*(*it).property).g = (col & 0x00FF00) >> 8;
        (*(*it).property).b =  col & 0x0000FF;
    }

    /* Načtení čísel z nového skinu */
    for(vector<Skin::Property<int*> >::iterator it = numbers.begin(); it != numbers.end(); ++it) {
        (*(*it).property) = 0; /* reset, aby nedocházelo k nedefinovaným jevům */
        conf.value((*it).parameter, (*(*it).property), conf.section((*it).section));
    }

    /* Načtení zarovnání z nového skinu */
    for(vector<Skin::Property<Align*> >::iterator it = aligns.begin(); it != aligns.end(); ++it) {
        (*(*it).property) = (Align) 0; /* reset, aby nedocházelo k nedefinovaným jevům */
        conf.value((*it).parameter, (*(*it).property), conf.section((*it).section));
    }
}

/* Inicializace a získání ukazatele na surface */
template<> SDL_Surface** Skin::get(const string& parameter, string section) {
    string file;
    conf.value(parameter, file, conf.section(section));

    /* Ukazatel musí být dvojitý, protože funkce IMG_Load() si samy určují, kam
        umístí data a proto by se jednoduchý ukazatel při reloadu surface zničil */
    SDL_Surface** surface = new SDL_Surface*;

    /* Konverze do formátu displeje */
    SDL_Surface* temp = IMG_Load(file.c_str());
    if(temp == NULL)
        cerr << "Nepodařilo se načíst obrázek \"" << file << "\"." << endl;

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
template<> TTF_Font** Skin::get(const string& parameter, string section) {
    string file;
    conf.value(parameter, file, conf.section(section));

    int fontSize = 0;
    conf.value(parameter + "Size", fontSize, conf.section(section));

    TTF_Font** font = new TTF_Font*;
    *font = TTF_OpenFont(file.c_str(), fontSize);

    if(*font == NULL)
        cerr << "Nepodařilo se načíst font \"" << file << "\"." << endl;

    Skin::Property<TTF_Font**> property;
    property.parameter = parameter;
    property.section = section;
    property.property = font;

    fonts.push_back(property);

    return font;
}

/* Inicalizace a získání ukazatele na string */
template<> string* Skin::get(const string& parameter, string section) {
    string *text = new string;
    conf.value(parameter, *text, conf.section(section));

    Skin::Property<string*> property;
    property.parameter = parameter;
    property.section = section;
    property.property = text;

    texts.push_back(property);

    return text;
}

/* Inicializace a získání ukazatele na barvu */
template<> SDL_Color* Skin::get(const string& parameter, string section) {
    int col = 0;
    conf.value(parameter, col, conf.section(section), ConfParser::COLOR);

    SDL_Color* color = new SDL_Color;
    (*color).r = (col & 0xFF0000) >> 16;
    (*color).g = (col & 0x00FF00) >> 8;
    (*color).b =  col & 0x0000FF;

    Skin::Property<SDL_Color*> property;
    property.parameter = parameter;
    property.section = section;
    property.property = color;

    colors.push_back(property);

    return color;
}

/* Inicializace a získání ukazatele na SDL_Rect */
template<> SDL_Rect* Skin::get(const string& parameter, string section) {
    SDL_Rect* position = new SDL_Rect;
    int x = 0;  int y = 0; int w = 0; int h = 0;
    if(parameter == "") {
        conf.value("x", x, conf.section(section));
        conf.value("y", y, conf.section(section));
        conf.value("w", w, conf.section(section));
        conf.value("h", h, conf.section(section));
    }
    else {
        conf.value(parameter + "X", x, conf.section(section));
        conf.value(parameter + "Y", y, conf.section(section));
        conf.value(parameter + "W", w, conf.section(section));
        conf.value(parameter + "H", h, conf.section(section));
    }

    (*position).x = x;  (*position).y = y;
    (*position).w = w;  (*position).h = h;

    Skin::Property<SDL_Rect*> property;
    property.parameter = parameter;
    property.section = section;
    property.property = position;

    positions.push_back(property);

    return position;
}

/* Inicializace a získání ukazatele na číslo */
template<> int* Skin::get(const string& parameter, string section) {
    int *i = new int; *i = 0;
    conf.value(parameter, *i, conf.section(section));

    Skin::Property<int*> property;
    property.parameter = parameter;
    property.section = section;
    property.property = i;

    numbers.push_back(property);

    return i;
}

/* Inicializace a získání ukazatele na zarovnání */
template<> Align* Skin::get(const string& parameter, string section) {
    Align *i = new Align; *i = (Align) 0;
    conf.value(parameter, *i, conf.section(section));

    Skin::Property<Align*> property;
    property.parameter = parameter;
    property.section = section;
    property.property = i;

    aligns.push_back(property);

    return i;
}
