#ifndef SPLASH_H
#define SPLASH_H

#include <vector>
#include <string>
#include <SDL/SDL.h>
#include "Skin.h"
#include "Effects.h"

/**
 * Zobrazení splashe
 *
 * Zobrazí obrázek na pozadí + texty. Spolupracuje se Skin, takže je vše
 * změnitelné v reálném čase.
 */
class Splash {
    public:
        /**
         * Konstruktor
         * @param   _image  Obrázek
         * @param   _w      Šířka ohraničujícího rámce
         * @param   _h      Výška ohraničujícího rámce
         * @param   _align  Zarovnání rámce vůči displeji
         * @param   x       X-ová pozice obrázku v rámci
         * @param   y       Y-ová pozice obrázku v rámci
         */
        inline Splash(SDL_Surface* _screen, SDL_Surface** _image, int* _w, int* _h, Align* _align, int* x, int* y):
            screen(_screen), image(_image), w(_w), h(_h), image_x(x), image_y(y), align(_align) {}

        /**
         * Zobrazení splashe
         */
        void view(void);
    private:
        /**
         * Displejová surface, obrázek splashe
         */
        SDL_Surface *screen, **image;

        /**
         * Šířka a výška ohraničujícího rámce, pozice obrázku vůči němu
         */
        int *w, *h, *image_x, *image_y;

        /**
         * Zarování rámce vůči displeji
         */
        Align* align;
};

#endif
