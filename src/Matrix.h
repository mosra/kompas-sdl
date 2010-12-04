#ifndef Kompas_Sdl_Matrix_h
#define Kompas_Sdl_Matrix_h
/*
    Copyright © 2007, 2008, 2009, 2010 Vladimír Vondruš <mosra@centrum.cz>

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
 * @file Matrix.h
 * @brief Třída Matrix
 */

#include <vector>

namespace Kompas { namespace Sdl {

/**
 * @brief Matice položek s možností efektivního pohybu mezi nimi
 *
 * Třída slouží k uchování jakéhokoli počtu položek v dvourozměrné mřížce, z
 * nichž každá má své dané souřadnice (nemusí být zaplněny všechny body mřížky)
 * a může být zakázána, čímž nebude možné se na ní přesunout. Šablonová položka
 * Matrix::Item musí mít tedy alespoň souřadnice a položku flags, která může být
 * nastavena na DISABLED (viz Matrix::Flags):
 * @code
struct Item {
    int x, y;
    int flags;
}
 * @endcode
 * Odvozené třídy můžou do této struktury přidávat jakékoli vlastnosti (a funkce),
 * flags jsou rozšiřitelné i na další položky (spojované binárním OR), ovšem
 * položka Matrix::DISABLED (<tt>0x01</tt>) musí zůstat.
 *
 * Posun mezi položkami není bezchybný, při malém počtu povolených položek může
 * dojít k tomu, že některé položky budou pomocí kurzorových kláves nedostupné.
 * <strong>První systém</strong> (který je zde implementovaný) hledá nejbližší
 * sloupec (při horizontálním posunu) či řádek (při vertikálním posunu) a v něm
 * položku nejblíže té aktuální. Bohužel při tomto rozvržení je položka dole
 * nedostupná:
<pre>
00000
0   0
0 X 0
</pre>
 * <strong>Druhý systém</strong> hledá nejbližší položku ve svém sloupci či
 * řádku, při nenalezení hledá v jiných. Při tomto rozvržení je nedostupná
 * položka uprostřed:
<pre>
0 0
 X
0 0
</pre>
 */
template <class Item> class Matrix {
    public:
        /** @brief Flags */
        enum Flags {
            DISABLED = 0x01         /**< @brief Položka je zakázaná (nelze se na ni přesunout) */
        };

        /** @brief Typ pro ID položky */
        typedef typename std::vector<Item>::size_type itemId;

        /**
         * @brief Posun nahoru
         *
         * @return Zda jsme se někam pohnuli
         */
        bool moveUp(void);

        /**
         * @brief Posun dolů
         *
         * @return Zda jsme se někam pohnuli
         */
        bool moveDown(void);

        /**
         * @brief Posun vlevo
         *
         * @return Zda jsme se někam pohnuli
         */
        bool moveLeft(void);

        /**
         * @brief Posun vpravo
         *
         * @return Zda jsme se někam pohnuli
         */
        bool moveRight(void);

        /**
         * @brief Zakázání položky
         * @param   item    ID položky
         * @todo Např. u klávesnice je tohle k ničemu => protected?
         */
        void disableItem(itemId item);

        /**
         * @brief Povolení položky
         * @param   item    ID položky
         * @todo Např. u klávesnice je tohle k ničemu => protected?
         */
        void enableItem(itemId item);

    protected:

        typename std::vector<Item>::const_iterator actualItem;   /**< @brief Ukazatel na aktuální položku */
        std::vector<Item> items;                        /**< @brief Položky */

        /**
         * @brief Reload položek pro účely posunu
         *
         * Spouštěno při přidání nové položky či při zakázání nebo povolení
         * položky. Znova vytvoří seřazené vektory Toolbar::sortedHorizontal a
         * Toolbar::sortedVertical z aktivních položek a první povolenou položku
         * označí jako aktuální.
         */
        void reloadItems();

    private:

        /**
         * @brief Vertikálně řazené ukazatele na aktivní položky
         *
         * Pro účely horizontálního posunu - rychlé nalezení položky ve sloupci
         * vpravo či vlevo nejblíže od aktuální položky.
         */
        typename std::vector<typename std::vector<Item>::const_iterator> sortedVertical;

        /**
         * @brief Horizontálně řazené ukazatele na aktivní položky
         *
         * Pro účely vertikálního posunu - rychlé nalezení položky v řádku nad
         * či pod aktuální položkou.
         */
        typename std::vector<typename std::vector<Item>::const_iterator> sortedHorizontal;

        /**
         * @brief Predikát pro vertikální řazení položek
         *
         * @param   a       První položka k porovnání
         * @param   b       Druhá položka k porovnání
         * @return  Jestli a patří před b
         */
        inline static bool verticalCompare(typename std::vector<Item>::const_iterator a, typename std::vector<Item>::const_iterator b) {
            return (*a).x == (*b).x ? (*a).y < (*b).y : (*a).x < (*b).x;
        }

        /**
         * @brief Predikát pro horizontální řazení položek
         *
         * @param   a       První položka k porovnání
         * @param   b       Druhá položka k porovnání
         * @return  Jestli a patří před b
         */
        inline static bool horizontalCompare(typename std::vector<Item>::const_iterator a, typename std::vector<Item>::const_iterator b) {
            return (*a).y == (*b).y ? (*a).x < (*b).x : (*a).y < (*b).y;
        }
};

}}

#endif
