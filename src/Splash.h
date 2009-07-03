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
         * @param   _x      X-ové posunutí rámce
         * @param   _y      Y-ové posunutí rámce
         * @param   _w      Šířka ohraničujícího rámce
         * @param   _h      Výška ohraničujícího rámce
         * @param   _align  Zarovnání rámce vůči displeji
         */
        inline Splash(SDL_Surface* _screen, SDL_Surface** _image, int* _x, int* _y, int* _w, int* _h, Align* _align):
            screen(_screen), image(_image), w(_w), h(_h), x(_x), y(_y), align(_align) {}

        /**
         * Vložení textu
         */
        void addText(TTF_Font** font, SDL_Color* color, int* _x, int* _y, int* _w, int *_h, Align* align, std::string* text);

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
         * Posunutí a velikost ohraničujícího rámce
         */
        int *x, *y, *w, *h;

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
