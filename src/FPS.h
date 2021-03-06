#ifndef Kompas_Sdl_FPS_h
#define Kompas_Sdl_FPS_h
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
 * @file FPS.h
 * @brief Třída FPS
 */

#include <SDL/SDL.h>
#include <SDL/SDL_timer.h>

namespace Kompas { namespace Sdl {

/**
 * @brief Omezování %FPS a vypočítávání rychlosti
 *
 * Slouží k omezování počtu snímků za sekundu a vypočítávání plynulé rychlosti
 * posunu objektů při proměnlivém %FPS.
 *
 * @attention Pro funkci této třídy je nutná inicializace SDL_INIT_TIMER v SDL_Init()!
 */
class FPS {
    public:
        /**
         * @brief Typ pro uchovávání dat o posunu objektu
         *
         * Protože žádný posun nebude nikdy přesně o celý počet pixelů, musí se
         * někam ukládat neceločíselný zbytek posunu a připočítat do dalšího
         * posunu, aby byl posun plynulý. Tato data si volající třída uchovává
         * ve své členské proměnné.
         *
         * Hodnota <strong>větší než nula</strong> je neceločíselný zbytek
         * posunu vynásobený 1000 (aby se nemuselo používat na GP2X pomalé
         * double, takovou přesnost nepotřebujeme). Viz FPS::move.
         *
         * <strong>Záporná hodnota</strong> značí, že objekt je zrovna
         * pozastaven do doby, než časovač SDL_GetTicks() dosáhne této hodnoty
         * (samozřejmě převedené na kladnou). Viz FPS::pause.
         */
        typedef int Data;

        /**
         * @brief Limitní hodnota FPS
         *
         * Pokud je nastaveno na nulu, FPS se neomezují.
         */
        static unsigned int limit;

        /**
         * @brief "Konstruktor"
         *
         * Inicializuje čas před prvním snímkem, takže je dobré konstruktor volat
         * těsně před hlavním programovým cyklem, aby čas prvního snímku nebyl
         * zatížen inicializací programu. Protože tato třída nemá žádné nestatické
         * členy, není nutné její instanci ukládat do proměnné.
         */
        FPS(void) {
            timer = SDL_GetTicks();
        }

        /**
         * @brief Refresh
         *
         * Spouštět na konci každé smyčky programového cyklu. Pozastaví program,
         * pokud byl čas vykonání smyčky kratší, než kolik dovoluje limit FPS a
         * z celkového času smyčky umožní výpočet rychlosti pohybu objektů.
         * @note Může být inline, protože je volaná jen na jednom místě v kódu.
         * @return %FPS aktuálního snímku
         */
        static double refresh(void);

        /**
         * @brief Spočítání délky posunu v aktuálním snímku
         *
         * Délka posunu je vypočítána z času posledního snímku, což se může
         * projevit "skokem" objektu, pokud předchozí snímek trval neúměrně
         * dlouhou dobu.
         * @param   pps     Rychlost v pixelech za sekundu
         * @param   object  Ukazatel na objekt pro uložení dat (viz FPS::Data)
         * @return  Délka posunu objektu
         * @bug Skok posouvajícího se objektu při neúměrně dlouhém času předchozího snímku
         */
        static int move(unsigned int pps, Data* object);

        /**
         * @brief Pozastavení posunu objektu
         *
         * Pozastaví na určitý čas posun objektu, takže funkce FPS::move bude
         * vracet nulu.
         * @param   ms      Doba pozastavení v milisekundách
         * @param   object  Ukazatel na objekt pro uložení dat (viz FPS::Data)
         */
        inline static void pause(int ms, Data& object) {
            object = 0-(SDL_GetTicks()+ms);
        }

        /**
         * @brief Zjištění, zda je objekt pozastaven
         *
         * @param   object  Ukazatel na objekt s daty (viz FPS::Data)
         * @return True, když je objekt pozastaven
         */
        inline static bool paused(Data object) {
            return (unsigned int) (0-object) > SDL_GetTicks();
        }

    private:
        static unsigned int timer;          /**< @brief Čas při konci posledního snímku */
        static unsigned int lastFrameTime;  /**< @brief Čas zpracování posledního snímku */
};

}}

#endif
