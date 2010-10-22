#ifndef Map2X_Sdl_Splash_h
#define Map2X_Sdl_Splash_h

/**
 * @file Splash.h
 * @brief Třída Splash
 */

#include <vector>
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "utility.h"

namespace Map2X { namespace Sdl {

/**
 * @brief Zobrazení splashe
 *
 * Zobrazí obrázek a texty na pozadí. Plná podpora Skin a Localize.
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
         * @todo Možné problémy při resize screen (ztráta cíle ukazatele) => dvojitý?
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

        /** @brief Zobrazení splashe */
        void view(void);
    private:
        /** @brief Struktura pro text */
        struct Text {
            TTF_Font** font;        /**< @brief Font textu */
            SDL_Color* color;       /**< @brief Barva textu */
            SDL_Rect* position;     /**< @brief Pozice textu */
            Align* align; /**< @brief Zarovnání textu */
            std::string* text;      /**< @brief Text */
        };

        SDL_Surface *screen,        /**< @brief Displejová surface */
                    **image;        /**< @brief Obrázek splashe */
        SDL_Rect* position;         /**< @brief Pozice splashe */
        Align* align;     /**< @brief Zarování splashe vůči displeji */
        std::vector<Text> texts;    /**< @brief Vektor s texty */
};

}}

#endif
