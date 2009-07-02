#include "utility.h"

using std::string;

/* Oseknutí mezer */
string trim(const string& str) {
    size_t begin = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");

    /* Nenalezen žádný smysluplný znak */
    if((begin == string::npos) || (end == string::npos)) return "";

    return str.substr(begin, end-begin+1);
}