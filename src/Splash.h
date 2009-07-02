#ifndef SPLASH_H
#define SPLASH_H

#include <iostream>
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
         * Vložení textu
         */
        void addText(TTF_Font** font, SDL_Color* color, int* x, int* y, int* w, int *h, Align* align, std::string* text);

        /**
         * Zobrazení splashe
         */
        void view(void);
    private:
        /**
         * Displejová surface, obrázek splashe
         * @todo Možné problémy při resize screen (ztráta cíle ukazatele) => dvojitý?
         */
        SDL_Surface *screen, **image;

        /**
         * Šířka a výška ohraničujícího rámce
         */
        int *w, *h;

        /**
         * Pozice obrázku vůči rámci
         */
        int *image_x, *image_y;

        /**
         * Zarování rámce vůči displeji
         */
        Align* align;

        /**
         * Struktura pro text
         */
        struct Text {
            TTF_Font** font;
            SDL_Color* color;
            int *x, *y;
            int *w, *h;
            Align* align;
            std::string* text;
        };

        /**
         * Vektor s texty
         */
        std::vector<Text> texts;
};

#endif
