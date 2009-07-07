#ifndef SPLASH_H
#define SPLASH_H

#include <iostream>
#include <vector>
#include <string>
#include <SDL/SDL.h>
#include "Skin.h"
#include "Effects.h"

/**
 * @brief Zobrazení splashe
 *
 * Zobrazí obrázek na pozadí + texty. Plná podpora Skin a Localize
 */
class Splash {
    public:
        /**
         * @brief Konstruktor
         *
         * @param   _screen     Displejová surface
         * @param   _image      Obrázek
         * @param   _position   Pozice
         * @param   _align      Zarovnání rámce vůči displeji
         */
        inline Splash(SDL_Surface* _screen, SDL_Surface** _image, SDL_Rect* _position, Align* _align):
            screen(_screen), image(_image), position(_position), align(_align) {}

        /**
         * @brief Vložení textu
         *
         * @param   font        Font
         * @param   color       Barva textu
         * @param   _position   Pozice textu
         * @param   _align      Zarovnání textu
         * @param   text        Text
         */
        void addText(TTF_Font** font, SDL_Color* color, SDL_Rect* _position, Align* _align, std::string* text);

        /**
         * @brief Zobrazení splashe
         *
         * @todo Vyplnit pozadí černou barvou, aby nebyly artefakty
         */
        void view(void);
    private:
        /**
         * @brief Displejová surface, obrázek splashe
         * @todo Možné problémy při resize screen (ztráta cíle ukazatele) => dvojitý?
         */
        SDL_Surface *screen, **image;

        /** @brief Pozice splashe */
        SDL_Rect* position;

        /** @brief Zarování splashe vůči displeji */
        Align* align;

        /** @brief Struktura pro text */
        struct Text {
            TTF_Font** font;
            SDL_Color* color;
            SDL_Rect* position;
            Align* align;
            std::string* text;
        };

        /** @brief Vektor s texty */
        std::vector<Text> texts;
};

#endif
