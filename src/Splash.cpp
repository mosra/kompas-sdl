#include "Splash.h"

#include "SDL/SDL.h"

/* Zobrazení splashe */
void Splash::view(void){
    /* Zobrazení obrázku */
    SDL_Rect src = {0, 0, *w, *h};    /** @todo Srát se s šířkou obrázku větší než šířka displeje / rámce? */

    /* Zarovnání rámce vůči displeji, poté ořezání na velikost obrázku */
    SDL_Rect dst = Effects::move(
        Effects::align((*screen).w, (*screen).h, *align, *w, *h),
        *image_x, *image_y, (**image).w, (**image).h
    );

    SDL_BlitSurface(*image, &src, screen, &dst);
}
