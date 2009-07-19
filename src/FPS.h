#ifndef FPS_H
#define FPS_H

/**
 * @file FPS.h
 * @brief Třída FPS
 */

#include <SDL/SDL.h>
#include <SDL/SDL_timer.h>

namespace MInterface {

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
        inline static void pause(int ms, Data* object) {
            *object = 0-(SDL_GetTicks()+ms);
        }

    private:
        static unsigned int timer;          /**< @brief Čas při konci posledního snímku */
        static unsigned int lastFrameTime;  /**< @brief Čas zpracování posledního snímku */
};

}

#endif