#ifndef Map2X_Sdl_Mouse_h
#define Map2X_Sdl_Mouse_h

/**
 * @file Mouse.h
 * @brief Třída Mouse
 */

#include <SDL/SDL.h>

namespace Map2X { namespace Sdl {

/**
 * @brief Ovládání objektu myší
 *
 * Od tohoto základu se odvozují třídy umožňující ovládání pomocí myši a
 * reimplementují funkce Mouse::click, Mouse::mouseDown a Mouse::mouseUp.
 */
class Mouse {
    public:
        /**
         * @brief Kliknutí
         *
         * Vyvolá akci způsobenou kliknutím myši na příslušné souřadnice
         * @param   x       X-ová souřadnice
         * @param   y       Y-ová souřadnice
         * @param   action  Pokud kliknutí spustilo nějakou akci, do této
         *  proměnné se uloží její číslo (-1 pokud se žádná akce nespustila)
         * @return  Zda bylo klinutí v oblasti objektu
         */
        virtual bool click(int x, int y, int& action) { return false; }

        /**
         * @brief Stisknutí tlačítka myši
         *
         * Vyvolá akci způsobenou stisknutím tlačítka myěi na příslušné souřadnici
         * @param   x       X-ová souřadnice
         * @param   y       Y-ová souřadnice
         * @param   action  Pokud kliknutí spustilo nějakou akci, do této
         *  proměnné se uloží její číslo (-1 pokud se žádná akce nespustila)
         * @return  Zda bylo klinutí v oblasti objektu
         */
        virtual bool mouseDown(int x, int y, int& action) { return false; }

        /**
         * @brief Uvolnění tlačítka myši
         *
         * Vyvolá akci způsobenou uvolněním tlačítka myši na příslušné souřadnici
         * @param   x       X-ová souřadnice
         * @param   y       Y-ová souřadnice
         * @param   action  Pokud kliknutí spustilo nějakou akci, do této
         *  proměnné se uloží její číslo (-1 pokud se žádná akce nespustila)
         * @return  Zda bylo klinutí v oblasti objektu
         */
        virtual bool mouseUp(int x, int y, int& action) { return false; }
    protected:

        /**
         * @brief Zjištění, zda bylo kliknutí v oblasti
         *
         * @param   x       X-ová souřadnice
         * @param   y       Y-ová souřadnice
         * @param   area    Oblast
         * @return  Zda bylo klinutí v oblasti
         */
        bool inArea(int x, int y, const SDL_Rect& area);
};

}}

#endif
