#include <iostream>
#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "utility.h"
#include "ConfParser.h"
#include "Skin.h"
#include "Splash.h"
#include "Menu.h"
#include "Localize.h"
#include "configure.h"

using std::cout;    using std::cerr;    using std::endl;        using std::string;

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
    Localize localize("lang/en.conf", "lang/cz.conf");
    string* skinAuthor = localize.get("skinAuthor", "splash");
    string* author = skin.get<string*>("author");

    /* Nastavení splashe */
    Splash splash(screen,
        skin.get<SDL_Surface**>("image", "splash"),
        skin.get<SDL_Rect*>("", "splash"),
        skin.get<Align*>("align", "splash"));
    string text;
    splash.addText(
        skin.get<TTF_Font**>("font", "splashAuthor"),
        skin.get<SDL_Color*>("color", "splashAuthor"),
        skin.get<SDL_Rect*>("", "splashAuthor"),
        skin.get<Align*>("align", "splashAuthor"),
        &text
    );
    string version = SVN_VERSION;
    splash.addText(
        skin.get<TTF_Font**>("font", "splashVersion"),
        skin.get<SDL_Color*>("color", "splashVersion"),
        skin.get<SDL_Rect*>("", "splashVersion"),
        skin.get<Align*>("align", "splashVersion"),
        &version
    );

    /* Menu */
    Menu menu(
        screen, skin.get<SDL_Surface**>("image", "menu"),
        skin.get<SDL_Rect*>("", "menu"),
        skin.get<Align*>("align", "menu"),
        skin.get<SDL_Rect*>("items", "menu"),
        &dummy, &dummy, skin.get<TTF_Font**>("itemFont", "menu"),
        skin.get<SDL_Color*>("itemColor", "menu"),
        skin.get<SDL_Color*>("activeItemColor", "menu"),
        skin.get<SDL_Color*>("disabledItemColor", "menu"),
        skin.get<SDL_Color*>("activeDisabledItemColor", "menu"),
        0
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
    Menu::sectionId section = menu.addSection(0, localize.get("caption", "menu"), NULL, skin.get<Align*>("itemsAlign", "menu"), 0);
    menu.addItem(section, 0, localize.get("openPackage", "menu"), NULL, Menu::DISABLED);
    menu.addItem(section, 0, localize.get("playGame", "menu"));
    menu.addItem(section, 0, localize.get("options", "menu"));
    menu.addItem(section, 0, localize.get("about", "menu"));
    menu.addItem(section, 0, localize.get("quit", "menu"));

    /* Hlavní smyčka programu */
    int done = 0;
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent (&event)) {
            switch(event.type) {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_c:
                            if (event.key.keysym.mod& (KMOD_LCTRL|KMOD_RCTRL))
                            done = 1;
                            break;
                        case SDLK_s:
                            skin.load("skin2.conf");
                            break;
                        case SDLK_d:
                            skin.load("skin.conf");
                            break;
                        case SDLK_UP:
                            menu.moveUp();
                            break;
                        case SDLK_DOWN:
                            menu.moveDown();
                            break;
                        case SDLK_PAGEUP:
                            menu.scrollUp();
                            break;
                        case SDLK_PAGEDOWN:
                            menu.scrollDown();
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

        text = *skinAuthor + *author;
        splash.view();
        menu.view();
        SDL_UpdateRect(screen, 0, 0, 0, 0);

        //sleep(1);
    }

    return 0;
}
