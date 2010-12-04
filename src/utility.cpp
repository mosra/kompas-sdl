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

#include "utility.h"

#include <iostream>

using namespace std;

namespace Kompas { namespace Sdl {

string::iterator nextUTF8Character(string* str, string::iterator position) {
    if((*str).empty() || position < (*str).begin() || position >= (*str).end())
        return (*str).end();

    /* Regular ASCII character */
    if((unsigned char) *position < 0x80)
        return ++position;

    /* Leading byte for UTF-8 sequence, get sequence length */
    int bytes = 0;
         if((unsigned char) *position >= 0xC2 && (unsigned char) *position <= 0xDF) bytes = 2;
    else if((unsigned char) *position >= 0xE0 && (unsigned char) *position <= 0xEF) bytes = 3;
    else if((unsigned char) *position >= 0xF0 && (unsigned char) *position <= 0xF4) bytes = 4;

    /* Overlong encoding - begin of two-byte sequence, but second byte belongs
       to ASCII - still tolerating */
    else if((unsigned char) *position == 0xC0 || (unsigned char) *position == 0xC1) {
        /* Unexpected end */
        if(++position == (*str).end()) {
            cerr << "UTF-8 error: overlong encoding: unexpected end of string."
                 << endl;
            return (*str).end();
        }

        /* Second byte doesn't belong to ASCII! */
        else if((unsigned char) *position >= 0x80) {
            cerr << "UTF-8 error: overlong encoding: invalid byte " << std::hex
                 << (int) (unsigned char) *position << std::dec
                 << " on position " << position - (*str).begin() << "." << endl;
            return position;

        /* All OK */
        } else return ++position;
    }

    /* Reserved and invalid values, return position of next character */
    else {
        cerr << "UTF-8 error: invalid leading byte of UTF-8 sequence "
             << std::hex << (int) (unsigned char) *position << std::dec
             << " on position " << position - (*str).begin() << "." << endl;
        return ++position;
    }

    /* Check whether second and next bytes are in valid range */
    for(string::const_iterator it = position+1; it != position+bytes; ++it) {
        /* Unexpected end */
        if(it == (*str).end()) {
            cerr << "UTF-8 error: unexpected end of string." << endl;
            return (*str).end();
        }

        /* Wrong range */
        else if((unsigned char) *it < 0x80 || (unsigned char) *it > 0xBF) {
            cerr << "UTF-8 error: invalid second or next byte of UTF-8 sequence "
                 << std::hex << (int) (unsigned char) *it << std::dec
                 << " on position " << position - (*str).begin() << "." << endl;
            return ++position;
        }
    }

    /* All OK, return position of first byte after the UTF-8 sequence */
    return position+bytes;
}

string::iterator prevUTF8Character(string* str, string::iterator position) {
    if((*str).empty() || position == (*str).begin() || position > (*str).end())
        return (*str).begin();

    /* Previous byte is regular ASCII */
    if((unsigned char) *(position-1) < 0x80) {
        /* Overlong encoding */
        if((unsigned char) *(position-2) == 0xC0 || (unsigned char) *(position-2) == 0xC1)
            return position-2;

        return --position;
    }

    /* Invalid character */
    if((unsigned char) *(position-1) < 0x80 || (unsigned char) *(position-1) > 0xBF) {
        cerr << "UTF-8 error: invalid second or next byte of UTF-8 sequence "
             << std::hex << (int) (unsigned char) *--position << std::dec
             << " on position " << position - (*str).begin() << "." << endl;
        return position;

    /* Previous byte is second or next byte of UTF-8 sequence */
    } else {
        /* Foreach previous three bytes to get length of UTF-8 sequence */
        int bytes = 0;
        for(string::const_iterator it = position-1; it != position-5; --it) {
            bytes++;

            /* Begin of the string or there is no second or next byte of UTF-8 sequence */
            if(it == (*str).begin() || (unsigned char) *it < 0x80 || (unsigned char) *it > 0xBF) break;
        }

        /* Check if leading byte of UTF-8 sequence corresponds with sequence
           length, if not, return position of previous byte */
        if((bytes == 2 && ((unsigned char) *(position-bytes) < 0xC2 || (unsigned char) *(position-bytes) > 0xDF)) ||
           (bytes == 3 && ((unsigned char) *(position-bytes) < 0xE0 || (unsigned char) *(position-bytes) > 0xEF)) ||
           (bytes == 4 && ((unsigned char) *(position-bytes) < 0xF0 || (unsigned char) *(position-bytes) > 0xF4))) {
            cerr << "UTF-8 error: invalid second or next byte of UTF-8 sequence "
                 << std::hex << (int) (unsigned char) *(position-bytes)
                 << std::dec << " in " << bytes << "-byte sequence on position "
                 << position - (*str).begin() << "." << endl;
            return position-1;

        /* All OK, return position of leading byte of previous UTF-8 sequence */
        } else return position-bytes;
    }
}

}}
