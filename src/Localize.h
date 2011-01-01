#ifndef Kompas_Sdl_Localize_h
#define Kompas_Sdl_Localize_h
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

/** @file
 * @brief Class Kompas::Sdl::Localize
 */

#include <string>
#include <vector>

#include "Utility/Configuration.h"

namespace Kompas { namespace Sdl {

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
         * @param _fallback     Fallback language
         */
        Localize(const std::string& file, const std::string& _fallback = ""):
            lang(file), fallback(_fallback) {}

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
         * @param group         Group
         */
        std::string* get(const std::string& key, const std::string& group = "");

    private:
        struct Localization {
            std::string key;
            std::string group;

            /* This must be via pointer because vector automatically rearranges
               itself */
            std::string* text;
        };

        Utility::Configuration lang;
        Utility::Configuration fallback;
        std::vector<Localization> localizations;

        bool get(const std::string& key, const std::string& group, std::string* text, const Kompas::Utility::Configuration* file) const;
};

}}

#endif
