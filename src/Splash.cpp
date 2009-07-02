#include "Splash.h"

using std::vector;      using std::string;      using std::cout;    using std::endl;

/* Zobrazení splashe */
void Splash::view(void){
    /* Plocha pro vykreslování splashe */
    SDL_Rect area = Effects::align(screen, *align, *w, *h);

    /* Zobrazení obrázku */
    SDL_Rect dst = Effects::align(area, ALIGN_DEFAULT, (**image).w, (**image).h, *image_x, *image_y);

    SDL_BlitSurface(*image, NULL, screen, &dst);

    /* Zobrazení textů */
    for(vector<Text>::const_iterator it = texts.begin(); it != texts.end(); ++it) {
        /* Oblast textu */
        SDL_Rect textArea = Effects::align(area, ALIGN_DEFAULT, *(*it).w, *(*it).h, *(*it).x, *(*it).y);

        /** @todo Přepínač vyhlazeného textu */
        SDL_Surface* text = TTF_RenderUTF8_Blended(*(*it).font, (*(*it).text).c_str(), *(*it).color);

        SDL_Rect src = {0, 0,
            (*text).w > *(*it).w ? *(*it).w : (*text).w,
            (*text).h > *(*it).h ? *(*it).h : (*text).h};
        SDL_Rect dst = Effects::align(textArea, *(*it).align, src.w, src.h);
        SDL_BlitSurface(text, NULL, screen, &dst);
    }
}

/* Přidání textu */
void Splash::addText (TTF_Font** font, SDL_Color* color, int* x, int* y, int* w, int* h, Align* align, std::string* text) {
    Splash::Text _text;
    _text.font = font;
    _text.color = color;
    _text.x = x;
    _text.y = y;
    _text.w = w;
    _text.h = h;
    _text.align = align;
    _text.text = text;
    texts.push_back(_text);
}
