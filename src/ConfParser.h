#ifndef CONFPARSER_H
#define CONFPARSER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream> //debug

/**
 * Parametr konfiguračního souboru
 */
struct ConfParameter {
    std::string parameter, value;
};

/**
 * Sekce konfiguračního souboru
 */
struct ConfSection {
    std::string section;
    std::vector<ConfParameter>::const_iterator begin;
};

/**
 * Parser konfiguračních souborů
 * @todo Typy hodnot (+ double, align)
 * @todo Jaký sou výhody ukazatelů při vybírání hodnot?
 * @todo Testovat parametry jen na alfanumerické znaky
 */
class ConfParser {
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
        enum dataType {
            BOOL = 0x01, ALIGN = 0x02, INT = 0x04, DOUBLE = 0x08, STRING = 0x10,
            HEX = 0x20
        };

        /**
         * Ukazatel na sekci
         */
        typedef std::vector<ConfSection>::const_iterator sectionPointer;

        /**
         * Ukazatel na parametr
         */
        typedef std::vector<ConfParameter>::const_iterator parameterPointer;

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
            return value(parameter, _value, section, (*section).begin);
        }

        template<class Value> inline parameterPointer value(const std::string& parameter, Value& _value, parameterPointer begin, int flags = 0) {
            return value(parameter, _value, sections.begin(), begin);
        }

        template<class Value> inline parameterPointer value(const std::string& parameter, Value& _value, int flags = 0) {
            return value(parameter, _value, sections.begin(), parameters.begin());
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
        std::vector<ConfParameter> parameters;

        /**
         * Vektor s názvy a pozicemi sekcí
         */
        std::vector<ConfSection> sections;

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