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
         * @param   _image      Obrázek
         * @param   _position   Pozice
         * @param   _align      Zarovnání rámce vůči displeji
         */
        inline Splash(SDL_Surface* _screen, SDL_Surface** _image, SDL_Rect* _position, Align* _align):
            screen(_screen), image(_image), position(_position), align(_align) {}

        /**
         * Vložení textu
         */
        void addText(TTF_Font** font, SDL_Color* color, SDL_Rect* _position, Align* align, std::string* text);

        /**
         * Zobrazení splashe
         * @todo Vyplnit pozadí černou barvou, aby nebyly artefakty
         */
        void view(void);
    private:
        /**
         * Displejová surface, obrázek splashe
         * @todo Možné problémy při resize screen (ztráta cíle ukazatele) => dvojitý?
         */
        SDL_Surface *screen, **image;

        /**
         * Pozice splashe
         */
        SDL_Rect* position;

        /**
         * Zarování splashe vůči displeji
         */
        Align* align;

        /**
         * Struktura pro text
         */
        struct Text {
            TTF_Font** font;
            SDL_Color* color;
            SDL_Rect* position;
            Align* align;
            std::string* text;
        };

        /**
         * Vektor s texty
         */
        std::vector<Text> texts;
};

#endif
