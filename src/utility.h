#ifndef Map2X_Sdl_Utility_h
#define Map2X_Sdl_Utility_h
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
 * @brief Basic utilities
 */

#include <string>

namespace Map2X { namespace Sdl {

/**
 * @brief Align
 *
 * Specifies align of an object. Combine values with binary OR.
 */
enum Align {
    ALIGN_DEFAULT = 0,      /**< @brief Default align (top left) */
    ALIGN_LEFT = 0x01,      /**< @brief Left */
    ALIGN_CENTER = 0x02,    /**< @brief Center (horizontally) */
    ALIGN_RIGHT  = 0x04,    /**< @brief Right */
    ALIGN_TOP  = 0x10,      /**< @brief Top */
    ALIGN_MIDDLE = 0x20,    /**< @brief Middle (vertically) */
    ALIGN_BOTTOM = 0x40     /**< @brief Bottom */
};

/**
 * @brief Look up position of next character in UTF-8 string
 * @param str           UTF-8 string
 * @param position      Current position
 * @return Position of next UTF-8 character (if everything went fine), or:
 *  - If current byte is invalid, prints error to stderr and returns position
 *    of next byte.
 *  - If any of next byte is invalid, prints error to stderr and returns
 *    position of first invalid byte.
 *  - If the string has unexpected end or the position is after end, returns
 *    one position after end.
 *
 * @sa http://en.wikipedia.org/wiki/UTF-8#Description
 * @todo Proč nejde const string&? a const_iterator?
 */
std::string::iterator nextUTF8Character(std::string* str, std::string::iterator position);

/**
 * @brief Look up position of previous character in UTF-8 string
 * @param str           UTF-8 string
 * @param position      Current position
 * @return  Position of previous UTF-8 character (if everything went fine), or:
 *  - If previous byte is invalid, prints error to stderr and returns that
 *    position.
 *  - If leading byte of UTF-8 sequence is invalid, returns position of
 *    previous byte.
 *
 * @sa http://en.wikipedia.org/wiki/UTF-8#Description
 * @todo Proč nejde const string&? a const_iterator?
 */
std::string::iterator prevUTF8Character(std::string* str, std::string::iterator position);

}}

#endif
