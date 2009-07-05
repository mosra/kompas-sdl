#ifndef CONFPARSER_H
#define CONFPARSER_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>      /* std::istringstream */
#include <cstring>      /** @todo zbavit se toho strcmp() */

#include "utility.h"

/**
 * Parser konfiguračních souborů
 * @todo Typy hodnot (+ double, align)
 * @todo Testovat parametry jen na alfanumerické znaky
 * @todo Ošetřit escape znaky
 */
class ConfParser {
    private:
        /**
         * Parametr konfiguračního souboru
         */
        struct Parameter {
            std::string parameter, value;
        };

        /**
         * Sekce konfiguračního souboru
         */
        struct Section {
            std::string section;
            std::vector<Parameter>::const_iterator begin;
        };
    public:
        /**
         * Maximální délka řádku
         */
        static const int MAX_LINE_LENGTH;

        /**
         * Název hlavní sekce v konfiguráku
         */
        static const std::string DEFAULT_SECTION;

        /**
         * Typ dat
         */
        enum flags { HEX = 0x01, COLOR = 0x02 };

        /**
         * Ukazatel na sekci
         */
        typedef std::vector<Section>::const_iterator sectionPointer;

        /**
         * Ukazatel na parametr
         */
        typedef std::vector<Parameter>::const_iterator parameterPointer;

        /**
         * Získání názvu konfiguračního souboru
         * @return  Název souboru
         */
        inline std::string getFilename(void) const { return filename; }

        /**
         * Žádný parametr nenalezen
         */
        inline parameterPointer parameterNotFound() const { return parameters.end(); }

        /**
         * Žádná sekce nenalezena
         */
        inline sectionPointer sectionNotFound() const { return sections.end(); }

        /**
         * Konstruktor
         * @param   _file   Konfigurační soubor
         */
        ConfParser(std::string _file);

        /**
         * Destruktor
         */
        inline ~ConfParser(void) { destroy(); }

        /**
         * Kopírovací konstruktor
         *
         * Při kopírování do čistého objektu se musí reloadovat iterátory
         * ukazující na sekce, aby při zrušení původce dat nedošlo k segfault
         */
        inline ConfParser(const ConfParser& conf): filename(conf.filename), parameters(conf.parameters) {
            file.open(filename.c_str());
            reloadSections();
        }

        /**
         * Operátor přiřazení
         *
         * Při přepisování stávajícího objektu se musí korektně zničit předek
         * a reloadovat iterátory původce nových dat
         */
        ConfParser& operator= (const ConfParser& conf);

        /**
         * Nalezení hodnoty parametru
         * @param   parameter   Název parametru
         * @param   value       Reference na proměnnou pro uložení hodnoty
         * @param   section     Sekce, ve které se bude hledat
         * @param   begin       Počáteční parametr, od kterého se bude hledat
         */
        template<class Value> parameterPointer value(const std::string& parameter, Value& _value, sectionPointer section, parameterPointer begin, int flags = 0);

        template<class Value> inline parameterPointer value(const std::string& parameter, Value& _value, sectionPointer section, int flags = 0) {
            if(section == sections.end()) return parameters.end();
            return value(parameter, _value, section, (*section).begin, flags);
        }

        template<class Value> inline parameterPointer value(const std::string& parameter, Value& _value, parameterPointer begin, int flags = 0) {
            return value(parameter, _value, sections.begin(), begin, flags);
        }

        template<class Value> inline parameterPointer value(const std::string& parameter, Value& _value, int flags = 0) {
            return value(parameter, _value, sections.begin(), parameters.begin(), flags);
        }

        /**
         * Nalezení sekce
         * @param   section     Název sekce
         * @param   begin       Počátek hledání
         */
        sectionPointer section(const std::string& name, sectionPointer begin);

        inline sectionPointer section(const std::string& name) {
            return section(name, sections.begin());
        }
    private:
        /**
         * Soubor s konfigurákem
         */
        std::ifstream file;

        /**
         * Jméno souboru s konfigurákem
         */
        std::string filename;

        /**
         * Vektor s parametry a komentáři
         */
        std::vector<Parameter> parameters;

        /**
         * Vektor s názvy a pozicemi sekcí
         */
        std::vector<Section> sections;

        /**
         * Zničení objektu
         *
         * Uvolnění všech dat a uzavření souboru
         */
        inline void destroy(void) { file.close(); }

        /**
         * (Znovu)nalezení sekcí v konfiguráku
         *
         * Interně to používá iterátory, takže při přidání / změně parametru je
         * to rozhází
         */
        void reloadSections(void);
};

#endif