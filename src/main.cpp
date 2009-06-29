#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "Skin.h"

using std::cout;    using std::cerr;    using std::endl;

/* GP2X button mapping */
enum MAP_KEY {
    VK_UP,          VK_UP_LEFT,     VK_LEFT,        VK_DOWN_LEFT,   /*  0-3  */
    VK_DOWN,        VK_DOWN_RIGHT,  VK_RIGHT,       VK_UP_RIGHT,    /*  4-7  */
    VK_START,       VK_SELECT,      VK_FL,          VK_FR         , /*  8-11 */
    VK_FA,          VK_FB,          VK_FX,          VK_FY         , /* 12-15 */
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

    int done = 0;

    /* Inicializace SDL */
    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_TIMER) < 0) {
        cerr << "Nelze inicializovat SDL: " << SDL_GetError() << endl;
        exit (1);
    }
    atexit (Terminate);

    /* Incializace videa */
    if((screen = SDL_SetVideoMode(320, 240, 16, SDL_SWSURFACE)) == NULL) {
        cerr << "Nelze nastavit video mód 320x240x16: " << SDL_GetError() << endl;
        exit (2);
    }

    /* Inicializace joysticku */
    if (SDL_NumJoysticks() > 0) if(!(joy = SDL_JoystickOpen(0))) {
            cerr << "Nelze otevřít joystick 0: " << SDL_GetError() << endl;
    }

    cout << "Map2X2 -- třetí pokus o nemožné. © Vladimír Vondruš, 21.06.2009" << endl;

    cout << "Načtení skinu." << endl;
    Skin skin("skin.conf");
    int splash = skin.init(Skin::SURFACE, "image", "splash");

    /* Hlavní smyčka programu */
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent (&event)) {
            if(event.type == SDL_KEYDOWN) switch (event.key.keysym.sym) {
                case SDLK_c:
                    if (event.key.keysym.mod& (KMOD_LCTRL|KMOD_RCTRL))
                    done = 1;
                    break;
                case SDLK_s:
                    cout << "Načtení druhého skinu." << endl;
                    skin.load("skin2.conf");
                default:
                    break;
            }
        }

        SDL_BlitSurface(*skin.property<SDL_Surface*>(splash), NULL, screen, NULL);
        SDL_UpdateRect(screen, 0, 0, 0, 0);

       // sleep(1);
    }

    cout << "Ukončuji..." << endl;

    return 0;
}
