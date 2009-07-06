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

    Skin skin(screen, "skin.conf");
    string* author = skin.set<string*>("author");

    int dummy = 0;

    Splash splash(screen,
        skin.set<SDL_Surface**>("image", "splash"),
        &dummy, &dummy,
        skin.set<int*>("width", "splash"),
        skin.set<int*>("height", "splash"),
        skin.set<Align*>("align", "splash"));

    string text;
    splash.addText(
        skin.set<TTF_Font**>("font", "splashAuthor"),
        skin.set<SDL_Color*>("color", "splashAuthor"),
        skin.set<int*>("x", "splashAuthor"), skin.set<int*>("y", "splashAuthor"),
        skin.set<int*>("w", "splashAuthor"), skin.set<int*>("h", "splashAuthor"),
        skin.set<Align*>("align", "splashAuthor"),
        &text
    );
    string version = SVN_VERSION;
    splash.addText(
        skin.set<TTF_Font**>("font", "splashVersion"),
        skin.set<SDL_Color*>("color", "splashVersion"),
        skin.set<int*>("x", "splashVersion"), skin.set<int*>("y", "splashVersion"),
        skin.set<int*>("w", "splashVersion"), skin.set<int*>("h", "splashVersion"),
        skin.set<Align*>("align", "splashVersion"),
        &version
    );

    Menu menu(
        screen, skin.set<SDL_Surface**>("image", "menu"),
        skin.set<int*>("x", "menu"), skin.set<int*>("y", "menu"),
        skin.set<int*>("w", "menu"), skin.set<int*>("h", "menu"), skin.set<Align*>("align", "menu"),
        skin.set<int*>("itemsX", "menu"), skin.set<int*>("itemsY", "menu"),
        skin.set<int*>("itemsW", "menu"), skin.set<int*>("itemsH", "menu"),
        &dummy, &dummy, skin.set<TTF_Font**>("itemFont", "menu"),
        skin.set<SDL_Color*>("itemColor", "menu"),
        skin.set<SDL_Color*>("activeItemColor", "menu"),
        skin.set<SDL_Color*>("disabledItemColor", "menu"),
        skin.set<SDL_Color*>("activeDisabledItemColor", "menu"),
        0
    );

    string caption = "blahblahblahblahblahblahblahblahblahblahblahblah";
    menu.configureCaption(
        skin.set<int*>("captionX", "menu"), skin.set<int*>("captionY", "menu"),
        skin.set<int*>("captionW", "menu"), skin.set<int*>("captionH", "menu"),
        skin.set<Align*>("captionAlign", "menu"),
        skin.set<TTF_Font**>("captionFont", "menu"),
        skin.set<SDL_Color*>("captionColor", "menu")
    );

    Menu::sectionId section = menu.addSection(0, &caption, NULL, skin.set<Align*>("itemsAlign", "menu"), 0);
    menu.addItem(section, 0, &caption, NULL, 0);
    menu.addItem(section, 0, &caption, NULL, Menu::DISABLED);
    menu.addItem(section, 0, &caption, NULL, 0);
    menu.addItem(section, 0, &caption, NULL, 0);

    TTF_Font** font = skin.set<TTF_Font**>("font", "splashAuthor");

    cout << "Načtení skinu, autor: " << *author << endl;

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

        text = "Autor skinu: " + *author;
        splash.view();
        menu.view();
        SDL_UpdateRect(screen, 0, 0, 0, 0);

        //sleep(1);
    }

    cout << "Ukončuji..." << endl;

    return 0;
}
