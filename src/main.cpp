#include <iostream>
#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "Skin.h"
#include "Splash.h"
#include "utility.h"

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
    Skin skin(screen, "skin.conf");

    int dummy = 0;

    Splash splash(screen,
        skin.set<SDL_Surface**>("image", "splash"),
        skin.set<int*>("width", "splash"),
        skin.set<int*>("height", "splash"),
        skin.set<Align*>("align", "splash"),
        &dummy, &dummy);

    string*  author = skin.set<string*>("author");
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
                            cout << "Načtení druhého skinu, autor: " << *author << endl;
                            break;
                        case SDLK_d:
                            skin.load("skin.conf");
                            cout << "Načtení prvního skinu, autor: " << *author << endl;
                            break;
                        default:
                            break;
                    } break;
                case SDL_VIDEORESIZE:
                    screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 16, SDL_SWSURFACE|SDL_RESIZABLE);
                    break;
            }
        }

        splash.view();
        SDL_UpdateRect(screen, 0, 0, 0, 0);

       // sleep(1);
    }

    cout << "Ukončuji..." << endl;

    return 0;
}
