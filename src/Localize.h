#ifndef Map2X_Sdl_Localize_h
#define Map2X_Sdl_Localize_h
/*
    Copyright © 2007, 2008, 2009, 2010 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Map2X.

    Map2X is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Map2X is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

/** @file
 * @brief Class Map2X::Sdl::Localize
 */

#include <string>
#include <vector>

#include "ConfParser.h"

namespace Map2X { namespace Sdl {

/**
 * @brief Localizations
 *
 * Allows application localization via configuration files. Features:
 *  - switching application language on-the-fly (without restart)
 *  - fallback language for not-yet-translated strings
 *
 * Localized strings are provided via pointers, so no overhead is needed for
 * localization usage.
 */
class Localize {
    public:
        /**
         * @brief Constructor
         * @param file          Language file
         * @param fallback      Fallback language
         */
        Localize(const std::string& file, const std::string& fallback = "")
            { load(file, fallback); }

        /** @brief Destructor */
        ~Localize(void);

        /**
         * @brief Load a language
         * @param file          Language file
         * @param _fallback     Fallback language
         *
         * Use for loading another language. All strings which were requested
         * before via get() are gotten from new file or from fallback, if
         * needed.
         */
        void load(const std::string& file, const std::string& _fallback = "");

        /**
         * @brief Get localized string
         * @param key           Key
         * @param section       Section
         */
        std::string* get(const std::string& parameter, const std::string& section = ConfParser::DEFAULT_SECTION);

    private:
        struct Localization {
            std::string parameter;
            std::string section;
            std::string* text;
        };

        ConfParser lang;
        ConfParser fallback;
        std::vector<Localization> localizations;
};

}}

#endif
