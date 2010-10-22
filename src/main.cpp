#include <iostream>
#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "FPS.h"
#include "Keyboard.h"
#include "Localize.h"
#include "Menu.h"
#include "Map.h"
#include "Skin.h"
#include "Splash.h"
#include "Toolbar.h"

using std::cout;    using std::cerr;    using std::endl;        using std::string;
using namespace Map2X::Sdl;

/* GP2X button mapping */
enum MAP_KEY {
    VK_UP,          VK_UP_LEFT,     VK_LEFT,        VK_DOWN_LEFT,   /*  0-3  */
    VK_DOWN,        VK_DOWN_RIGHT,  VK_RIGHT,       VK_UP_RIGHT,    /*  4-7  */
    VK_START,       VK_SELECT,      VK_FL,          VK_FR,          /*  8-11 */
    VK_FA,          VK_FB,          VK_FX,          VK_FY,          /* 12-15 */
    VK_VOL_UP,      VK_VOL_DOWN,    VK_TAT
};

/* The screen surface, joystick device */
/**
 * @todo Udělat nějakou třídu
 */
SDL_Surface *screen = NULL;
SDL_Joystick *joy = NULL;

/**
 * Správné ukončení SDL a návrat do GP2X menu
 */
void Terminate(void) {
    SDL_Quit();

    #ifdef GP2X
    chdir("/usr/gp2x");
    execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);
    #endif
}

int main(int argc, char **argv) {

    cout << "Map2X2 -- třetí pokus o nemožné. © Vladimír Vondruš, 21.06.2009" << endl;

    /* Inicializace SDL */
    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_TIMER) < 0) {
        cerr << "Nelze inicializovat SDL: " << SDL_GetError() << endl;
        exit (1);
    }
    atexit (Terminate);

    /* Incializace videa */
    if((screen = SDL_SetVideoMode(320, 240, 16, SDL_SWSURFACE|SDL_RESIZABLE)) == NULL) {
        cerr << "Nelze nastavit video mód 320x240x16: " << SDL_GetError() << endl;
        exit (2);
    }

    /* Inicializace joysticku */
    if (SDL_NumJoysticks() > 0) if(!(joy = SDL_JoystickOpen(0)))
        cerr << "Nelze otevřít joystick 0: " << SDL_GetError() << endl;

    /* Inicializace TTF */
    if(TTF_Init() != 0) {
        cerr << "Nelze inicializovat TTF: " << SDL_GetError() << endl;
        exit(3);
    }

    /* Nastavení skinu */
    Skin skin(screen, "skin.conf");
    int dummy = 0;

    /* Lokalizace */
    Localize lang("lang/cz.conf", "lang/en.conf");
    string* skinAuthor = lang.get("skinAuthor", "splash");
    string* author = skin.get<string*>("author");

    /* Nastavení splashe */
    Splash splash(screen,
        skin.get<SDL_Surface**>("image", "splash"),
        skin.get<SDL_Rect*>("", "splash"),
        skin.get<Align*>("align", "splash"));
    string skinText;
    splash.addText(
        skin.get<TTF_Font**>("authorFont", "splash"),
        skin.get<SDL_Color*>("authorColor", "splash"),
        skin.get<SDL_Rect*>("author", "splash"),
        skin.get<Align*>("authorAlign", "splash"),
        &skinText
    );
    string version = "Git";
    splash.addText(
        skin.get<TTF_Font**>("versionFont", "splash"),
        skin.get<SDL_Color*>("versionColor", "splash"),
        skin.get<SDL_Rect*>("version", "splash"),
        skin.get<Align*>("versionAlign", "splash"),
        &version
    );

    /* Menu */
    Menu menu(screen,
        skin.get<SDL_Surface**>("image", "menu"),
        skin.get<SDL_Rect*>("", "menu"),
        skin.get<Align*>("align", "menu"),
        skin.get<SDL_Rect*>("items", "menu"),
        &dummy, &dummy, skin.get<TTF_Font**>("itemFont", "menu"),
        skin.get<SDL_Color*>("itemColor", "menu"),
        skin.get<SDL_Color*>("activeItemColor", "menu"),
        skin.get<SDL_Color*>("disabledItemColor", "menu"),
        skin.get<SDL_Color*>("activeDisabledItemColor", "menu"),
        Menu::HIDDEN
    );
    menu.configureCaption(
        skin.get<SDL_Rect*>("caption", "menu"),
        skin.get<Align*>("captionAlign", "menu"),
        skin.get<TTF_Font**>("captionFont", "menu"),
        skin.get<SDL_Color*>("captionColor", "menu")
    );
    menu.configureScrollbar(
        skin.get<SDL_Rect*>("scrollbar", "menu"),
        skin.get<Align*>("scrollbarAlign", "menu"),
        skin.get<int*>("scrollbarArrowHeight", "menu"),
        skin.get<SDL_Surface**>("scrollbarArrowUp", "menu"),
        skin.get<SDL_Surface**>("scrollbarArrowDown", "menu"),
        skin.get<SDL_Surface**>("scrollbarSlider", "menu")
    );

    enum Actions {
        ZOOMIN, ZOOMOUT, OPEN, SAVE, OPTIONS, MAPOPTIONS, EXIT,
        OPTIONS_FPS, OPTIONS_LANGUAGE, OPTIONS_SMOOTHTEXT, OPTIONS_ABOUT,
        OPTIONS_CHANGEABLE_TEXT
    };

    Align* menuItemsAlign = skin.get<Align*>("itemsAlign", "menu");

    /* Sekce nastavení */
    Menu::sectionId optionsSection = menu.addSection(0, lang.get("options", "toolbar"), NULL, menuItemsAlign, 0);
    menu.addItem(optionsSection, OPTIONS_FPS, lang.get("maxFPS", "optionsMenu"), NULL, Menu::DISABLED);
    menu.addItem(optionsSection, OPTIONS_LANGUAGE, lang.get("language", "optionsMenu"), NULL, Menu::DISABLED);
    menu.addItem(optionsSection, OPTIONS_SMOOTHTEXT, lang.get("smoothText", "optionsMenu"), NULL, Menu::DISABLED);
    string text = "Měnitelný text položky";
    menu.addItem(optionsSection, OPTIONS_CHANGEABLE_TEXT, &text);
    menu.addItem(optionsSection, OPTIONS_ABOUT, lang.get("about", "optionsMenu"), NULL, Menu::DISABLED);

    /* Sekce otevření mapového balíčku */
    Menu::sectionId openSection = menu.addSection(1, lang.get("open", "toolbar"), NULL, menuItemsAlign, 0);

    /* Toolbar */
    Toolbar toolbar(screen,
        skin.get<SDL_Rect*>("", "toolbar"),
        skin.get<Align*>("align", "toolbar"),
        (Align*) &dummy, &dummy,
        skin.get<TTF_Font**>("captionFont", "toolbar"),
        skin.get<SDL_Color*>("captionColor", "toolbar"),
        NULL, NULL, 0
    );
    toolbar.configureCaptionPlace(
        skin.get<SDL_Rect*>("captionPosition", "toolbar"),
        skin.get<Align*>("captionAlign", "toolbar")
    );
    toolbar.addImage(NULL, skin.get<SDL_Surface**>("image", "toolbar"));
    toolbar.addItem(
        skin.get<SDL_Rect*>("zoomInPosition", "toolbar"),
        0, 0, ZOOMIN,
        skin.get<SDL_Surface**>("zoomInIcon", "toolbar"),
        skin.get<SDL_Surface**>("zoomInIconActive", "toolbar"),
        skin.get<SDL_Surface**>("zoomInIconDisabled", "toolbar"),
        lang.get("zoomIn", "toolbar"), Toolbar::DISABLED
    );
    toolbar.addItem(
        skin.get<SDL_Rect*>("zoomOutPosition", "toolbar"),
        1, 0, ZOOMOUT,
        skin.get<SDL_Surface**>("zoomOutIcon", "toolbar"),
        skin.get<SDL_Surface**>("zoomOutIconActive", "toolbar"),
        skin.get<SDL_Surface**>("zoomOutIconDisabled", "toolbar"),
        lang.get("zoomOut", "toolbar"), Toolbar::DISABLED
    );
    toolbar.addItem(
        skin.get<SDL_Rect*>("openPosition", "toolbar"),
        2, 0, OPEN,
        skin.get<SDL_Surface**>("openIcon", "toolbar"),
        skin.get<SDL_Surface**>("openIconActive", "toolbar"),
        NULL, lang.get("open", "toolbar")
    );
    toolbar.addItem(
        skin.get<SDL_Rect*>("savePosition", "toolbar"),
        3, 0, SAVE,
        skin.get<SDL_Surface**>("saveIcon", "toolbar"),
        skin.get<SDL_Surface**>("saveIconActive", "toolbar"),
        skin.get<SDL_Surface**>("saveIconDisabled", "toolbar"),
        lang.get("save", "toolbar"), Toolbar::DISABLED
    );
    toolbar.addItem(
        skin.get<SDL_Rect*>("optionsPosition", "toolbar"),
        4, 0, OPTIONS,
        skin.get<SDL_Surface**>("optionsIcon", "toolbar"),
        skin.get<SDL_Surface**>("optionsIconActive", "toolbar"),
        NULL, lang.get("options", "toolbar")
    );
    toolbar.addItem(
        skin.get<SDL_Rect*>("mapOptionsPosition", "toolbar"),
        5, 0, MAPOPTIONS,
        skin.get<SDL_Surface**>("mapOptionsIcon", "toolbar"),
        skin.get<SDL_Surface**>("mapOptionsIconActive", "toolbar"),
        skin.get<SDL_Surface**>("mapOptionsIconDisabled", "toolbar"),
        lang.get("mapOptions", "toolbar"), Toolbar::DISABLED
    );
    toolbar.addItem(
        skin.get<SDL_Rect*>("exitPosition", "toolbar"),
        5, 0, EXIT,
        skin.get<SDL_Surface**>("exitIcon", "toolbar"),
        skin.get<SDL_Surface**>("exitIconActive", "toolbar"),
        NULL, lang.get("exit", "toolbar")
    );

    TTF_Font** mFont = skin.get<TTF_Font**>("captionFont", "toolbar");
    SDL_Color* mColor = skin.get<SDL_Color*>("captionColor", "toolbar");

    /* Klávesnice */
    Keyboard keyboard(screen, skin, "keyboard/cz.conf", text, Keyboard::HIDDEN);

    /* Mapa */
    Map map(screen, NULL,
        skin.get<SDL_Surface**>("tileNotFound", "map"));

    /* Hlavní smyčka programu */
    FPS(); FPS::limit = 50;
    int done = 0;
    while (!done) {
        int action = -1;

        /* Projití událostí */
        SDL_Event event;
        while (SDL_PollEvent (&event)) {
            switch(event.type) {
                case SDL_MOUSEBUTTONDOWN:
                    if(!keyboard.click(event.button.x, event.button.y, action))
                        if(!menu.click(event.button.x, event.button.y, action))
                            toolbar.click(event.button.x, event.button.y, action);
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_c:
                            if (event.key.keysym.mod& (KMOD_LCTRL|KMOD_RCTRL))
                            done = 1;
                            break;
                        case SDLK_UP:
                            if(keyboard) keyboard.moveUp();
                            else if(menu) menu.moveUp();
                            else if(toolbar) toolbar.moveUp();
                            else map.moveUp(10);
                            break;
                        case SDLK_DOWN:
                            if(keyboard) keyboard.moveDown();
                            else if(menu) menu.moveDown();
                            else if(toolbar) toolbar.moveDown();
                            else map.moveDown(10);
                            break;
                        case SDLK_RIGHT:
                            if(keyboard) keyboard.moveRight();
                            else if(!menu && toolbar) toolbar.moveRight();
                            else map.moveRight(10);
                            break;
                        case SDLK_LEFT:
                            if(keyboard) keyboard.moveLeft();
                            else if(!menu && toolbar) toolbar.moveLeft();
                            else map.moveLeft(10);
                            break;
                        case SDLK_RETURN:
                            if(keyboard) keyboard.select();
                            else if(menu) action = menu.select();
                            else if(toolbar) action = toolbar.select();
                            break;
                        case SDLK_PAGEUP:
                            if(menu) menu.scrollUp();
                            break;
                        case SDLK_PAGEDOWN:
                            if(menu) menu.scrollDown();
                            break;
                        case SDLK_ESCAPE:
                            if(keyboard) {
                                keyboard.hide();
                                menu.show();
                            }
                            else if(menu) menu.hide();
                            else if(toolbar) toolbar.hide();
                            else toolbar.show();
                            break;
                        default:
                            break;
                    } break;
                case SDL_VIDEORESIZE:
                    screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 16, SDL_SWSURFACE|SDL_RESIZABLE);
                    break;
                case SDL_QUIT:
                    done = 1;
                    break;
            }
        }

        /* Spuštěné akce */
        switch(action) {
            case OPEN:
                /* Pokud přepínáme z jiné sekce, neschovávání menu */
                if(menu.changeSection(openSection)) menu.show();
                else menu ? menu.hide() : menu.show();
                break;
            case OPTIONS:
                /* Pokud přepínáme z jiné sekce, neschovávání menu */
                if(menu.changeSection(optionsSection)) menu.show();
                else menu ? menu.hide() : menu.show();
                break;
            case EXIT:
                done = 1;
                break;
            case OPTIONS_CHANGEABLE_TEXT:
                menu.hide();
                keyboard.show();
                break;
            default:
                break;
        }

        skinText = *skinAuthor + *author;
        splash.view();
        map.view(mFont, mColor);
        toolbar.view();
        menu.view();
        keyboard.view();
        SDL_UpdateRect(screen, 0, 0, 0, 0);
        FPS::refresh();
    }

    return 0;
}
