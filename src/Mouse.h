#ifndef Kompas_Sdl_Mouse_h
#define Kompas_Sdl_Mouse_h
/*
    Copyright © 2007, 2008, 2009, 2010, 2011 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Kompas.

    Kompas is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Kompas is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

/**
 * @file Mouse.h
 * @brief Třída Mouse
 */

#include <SDL/SDL.h>

namespace Kompas { namespace Sdl {

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
