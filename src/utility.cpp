#include "utility.h"

#include <iostream>

using std::string;  using std::cout;    using std::endl;    using std::cerr;

namespace MToolkit {

/* Oseknutí mezer */
string trim(const string& str) {
    size_t begin = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");

    /* Nenalezen žádný smysluplný znak */
    if((begin == string::npos) || (end == string::npos)) return "";

    return str.substr(begin, end-begin+1);
}

/* Doxygen neví, že std::string::iterator == string::iterator */
#ifndef GENERATING_DOXYGEN_OUTPUT

/* Pozice dalšího znaku v UTF-8 sekvenci */
string::iterator nextUTF8Character(string* str, string::iterator position) {
    /* Prázdný string, pozice je na konci nebo úplně v háji, konec */
    if((*str).empty() || position < (*str).begin() || position >= (*str).end())
        return (*str).end();

    /* Normální ASCII znak */
    if((unsigned char) *position < 0x80)
        return ++position;

    /* Jinak je to znak patřící do vícebajtové sekvence UTF-8. Zjistíme počet
       bajtů UTF-8 sekvence */
    int bytes = 0;
         if((unsigned char) *position >= 0xC2 && (unsigned char) *position <= 0xDF) bytes = 2;
    else if((unsigned char) *position >= 0xE0 && (unsigned char) *position <= 0xEF) bytes = 3;
    else if((unsigned char) *position >= 0xF0 && (unsigned char) *position <= 0xF4) bytes = 4;

    /* Overlong encoding - začátek dvoubajtové sekvence, ale druhý znak
       patří do normální ASCII - ještě tolerujeme */
    else if((unsigned char) *position == 0xC0 || (unsigned char) *position == 0xC1) {
        /* Neočekávaný konec */
        if(++position == (*str).end()) {
            cerr << "Chyba při zpracování UTF-8 řetězce: Overlong encoding:"
                    << " Neočekávaný konec řetězce." << endl;
            return (*str).end();
        }

        /* V pořádku, vrácení pozice za těmito dvěma znaky */
        else if((unsigned char) *position < 0x80) return ++position;

        /* Druhý znak musí být z rozsahu 0 - 127! */
        else {
            cerr << "Chyba při zpracování UTF-8 řetězce: Overlong encoding:"
                    << " Neplatný znak " << std::hex
                    << (int) (unsigned char) *position << std::dec
                    << " na pozici " << position - (*str).begin() << "." << endl;
            return position;
        }
    }

    /* Rezervované hodnoty a chybové hodnoty - vrácení pozice hned dalšího
       znaku */
    else {
        cerr << "Chyba při zpracování UTF-8 řetězce: Neplatný uvozovací znak "
             << std::hex << (int) (unsigned char) *position << std::dec
             << " na pozici " << position - (*str).begin() << "." << endl;
        return ++position;
    }

    /* Ověření druhého a dalších bytů, zda jsou ve správném rozsahu,
       pokud ne, vrácení pozice prvního špatného znaku */
    for(string::const_iterator it = position+1; it != position+bytes; ++it) {
        /* Neočekávaný konec */
        if(it == (*str).end()) {
            cerr << "Chyba při zpracování UTF-8 řetězce: Neočekávaný konec "
                 << "řetězce." << endl;
            return (*str).end();
        }

        /* Ve správném rozsahu */
        else if((unsigned char) *it >= 0x80 && (unsigned char) *it <= 0xBF) continue;

        /* Ve špatném rozsahu */
        cerr << "Chyba při zpracování UTF-8 řetězce: Neplatný druhý či "
             << "další znak v UTF-8 sekvenci: " << std::hex
             << (int) (unsigned char) *it << std::dec
             << " na pozici " << position - (*str).begin() << "." << endl;
        return ++position;
    }

    /* Vše prošlo v pořádku, vracíme znak za UTF-8 sekvencí */
    return position+bytes;
}

/* Pozice předchozího znaku UTF-8 sekvence */
string::iterator prevUTF8Character(string* str, string::iterator position) {
    /* Prázdný string, pozice je úplně na začátku nebo za koncem */
    if((*str).empty() || position == (*str).begin() || position > (*str).end())
        return (*str).begin();

    /* Předchozí znak je normální ASCII znak */
    if((unsigned char) *(position-1) < 0x80) {
        /* Může to být ale i druhý znak u Overlong encoding */
        if((unsigned char) *(position-2) == 0xC0 || (unsigned char) *(position-2) == 0xC1)
            return position-2;

        return --position;
    }

    /* Předchozí znak je druhý nebo další znak UTF-8 sekvence */
    if((unsigned char) *(position-1) >= 0x80 && (unsigned char) *(position-1) <= 0xBF) {

        /* Procházení předchozích 3 znaků, abychom zjistili délku UTF-8 sekvence */
        int bytes = 0;
        for(string::const_iterator it = position-1; it != position-5; --it) {
            bytes++;

            /* Pokud jsme na začátku či už zde není druhý či další znak UTF-8 sekvence */
            if(it == (*str).begin() || (unsigned char) *it < 0x80 || (unsigned char) *it > 0xBF) break;
        }

        /* Ověření, jestli je před 1 bytem uvozovací znak dvoubytové sekvence
           nebo před 2 byty uvozovací znak tříbytové sekvence nebo před 3 byty
           uvozovací znak čtyřbytové sekvence */
        if((bytes == 2 && (unsigned char) *(position-bytes) >= 0xC2 && (unsigned char) *(position-bytes) <= 0xDF) ||
           (bytes == 3 && (unsigned char) *(position-bytes) >= 0xE0 && (unsigned char) *(position-bytes) <= 0xEF) ||
           (bytes == 4 && (unsigned char) *(position-bytes) >= 0xF0 && (unsigned char) *(position-bytes) <= 0xF4))
            return position-bytes;

        /* Nebylo tomu tak, vrací předchozí pozici, než která byla předána */
        else {
            cerr << "Chyba při zpracování UTF-8 řetězce: Neplatný uvozovací znak "
                 << std::hex << (int) (unsigned char) *(position-bytes)
                 << std::dec << " u " << bytes << "-bytové sekvence"
                 << " na pozici " << position - (*str).begin() << "." << endl;
            return position-1;
        }
    }

    /* Jiný znak - chyba! */
    cerr << "Chyba při zpracování UTF-8 řetězce: Neplatný druhý či "
         << "další znak v UTF-8 sekvenci: " << std::hex
         << (int) (unsigned char) *--position << std::dec
         << " na pozici " << position - (*str).begin() << "." << endl;
    return position;
}

#endif

}
