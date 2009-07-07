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
 * @brief Parser konfiguračních souborů
 *
 * Konfigurační soubory jsou v tomto formátu:
 * [názevSekce]
 * #Komentář
 * ;Komentář
 * parametr=hodnota
 * jinýParametr="hodnota"
 * dalšíParametr='hodnota'
 *
 * Komentář musí vždy začínat na novém řádku, v souboru může být libovolný počet
 * prázdných řádků, mezery na začátku a konci řádku nehrají roli. Pokud není před
 * prvním parametrem v souboru uveden název sekce, její název je zvolen podle
 * ConfParser::DEFAULT_SECTION. Sekce a parametry se stejným názvem se mohou
 * libovolně opakovat, jejich výběr také není omezen. UTF8 signatura na začátku
 * souboru (EF BB BF) je automaticky detekována a přeskočena. Maximální délka
 * komentáře, názvu sekce, parametru nebo hodnoty parametru je nastavena na
 * ConfParser::MAX_LINE_LENGTH. Pokud není hodnota v uvozovkách (apostrofech),
 * jsou u ní osekány počáteční a koncové mezery.
 * @todo Testovat parametry jen na alfanumerické znaky
 * @todo Ošetřit escape znaky
 * @todo Default sekce jen když obsahuje nějaké parametry
 */
class ConfParser {
    private:
        /** Parametr konfiguračního souboru */
        struct Parameter {
            std::string parameter, value;
        };

        /** Sekce konfiguračního souboru */
        struct Section {
            std::string section;
            std::vector<Parameter>::const_iterator begin;
        };
    public:
        /** Maximální délka komentáře, názvu sekce, parametru nebo hodnoty parametru */
        static const int MAX_LINE_LENGTH;

        /** Název první sekce v konfiguráku */
        static const std::string DEFAULT_SECTION;

        /** Flags při výběru hodnoty int */
        enum flags { HEX = 0x01, COLOR = 0x02 };

        /** Ukazatel na sekci */
        typedef std::vector<Section>::const_iterator sectionPointer;

        /** Ukazatel na parametr */
        typedef std::vector<Parameter>::const_iterator parameterPointer;

        /**
         * @brief Získání názvu konfiguračního souboru
         * @return  Název souboru
         */
        inline std::string getFilename(void) const { return filename; }

        /**
         * @brief Žádný parametr nenalezen
         *
         * Použitelné pro porovnávání s návratovou hodnotou ConfParser::value
         * @return Iterátor parameters.end()
         */
        inline parameterPointer parameterNotFound() const { return parameters.end(); }

        /**
         * @brief Žádná sekce nenalezena
         *
         * Použitelné pro porovnávání s návratovou hodnotou ConfParser::section
         * @return Iterátor sections.end()
         */
        inline sectionPointer sectionNotFound() const { return sections.end(); }

        /**
         * @brief Implicitní konstruktor
         */
        ConfParser(void) {}

        /**
         * @brief Konstruktor
         * @param   _file   Konfigurační soubor
         */
        ConfParser(std::string _file);

        /**
         * @brief Destruktor
         */
        inline ~ConfParser(void) { destroy(); }

        /**
         * @brief Přeypování na bool
         *
         * Vrací true, pokud byl načten alespoň jeden parametr z konfiguračního souboru
         */
        inline operator bool(void) { return !(parameters.size() == 0); }

        /**
         * @brief Kopírovací konstruktor
         *
         * Při kopírování do čistého objektu se musí reloadovat iterátory
         * ukazující na sekce, aby při zrušení původce dat nedošlo k segfault
         */
        inline ConfParser(const ConfParser& conf): filename(conf.filename), parameters(conf.parameters) {
            file.open(filename.c_str());
            reloadSections();
        }

        /**
         * @brief Operátor přiřazení
         *
         * Při přepisování stávajícího objektu se musí korektně zničit předek
         * a reloadovat iterátory původce nových dat
         */
        ConfParser& operator= (const ConfParser& conf);

        /**
         * @brief Nalezení hodnoty parametru
         *
         * @param   parameter   Název parametru
         * @param   value       Reference na proměnnou pro uložení hodnoty
         * @param   section     Sekce, ve které se bude hledat
         * @param   begin       Počáteční parametr, od kterého se bude hledat
         *  (použitelné v souborech s více stejně nazvanými parametry v jedné sekci)
         * @param   flags       Viz ConfParser::flags, použitelné při získávání int
         * @return  Ukazatel na nalezený parametr, pokud nebyl nalezen, vrací to
         *  samé jako ConfParser::parameterNotFound
         */
        template<class Value> parameterPointer value(const std::string& parameter, Value& _value, sectionPointer section, parameterPointer begin, int flags = 0);

        /**
         * @brief Nalezení hodnoty parametru
         *
         * Počátek hledávní nastaven na počátek dané sekce
         * @param   parameter   Název parametru
         * @param   value       Reference na proměnnou pro uložení hodnoty
         * @param   section     Sekce, ve které se bude hledat
         * @param   flags       Viz ConfParser::flags, použitelné při získávání int
         * @return  Ukazatel na nalezený parametr, pokud nebyl nalezen, vrací to
         *  samé jako ConfParser::parameterNotFound
         */
        template<class Value> inline parameterPointer value(const std::string& parameter, Value& _value, sectionPointer section, int flags = 0) {
            if(section == sections.end()) return parameters.end();
            return value(parameter, _value, section, (*section).begin, flags);
        }

        /**
         * @brief Nalezení hodnoty parametru
         *
         * Hledá se v implicitní sekci (použitelné v conf souborech bez sekcí)
         * @param   parameter   Název parametru
         * @param   value       Reference na proměnnou pro uložení hodnoty
         * @param   begin       Počáteční parametr, od kterého se bude hledat
         *  (použitelné v souborech s více stejně nazvanými parametry v jedné sekci)
         * @param   flags       Viz ConfParser::flags, použitelné při získávání int
         * @return  Ukazatel na nalezený parametr, pokud nebyl nalezen, vrací to
         *  samé jako ConfParser::parameterNotFound
         */
        template<class Value> inline parameterPointer value(const std::string& parameter, Value& _value, parameterPointer begin, int flags = 0) {
            return value(parameter, _value, sections.begin(), begin, flags);
        }

        /**
         * @brief Nalezení hodnoty parametru
         *
         * Hledá se od počátku se v implicitní sekci (použitelné v conf souborech bez sekcí)
         * @param   parameter   Název parametru
         * @param   value       Reference na proměnnou pro uložení hodnoty
         * @param   flags       Viz ConfParser::flags, použitelné při získávání int
         * @return  Ukazatel na nalezený parametr, pokud nebyl nalezen, vrací to
         *  samé jako ConfParser::parameterNotFound
         */
        template<class Value> inline parameterPointer value(const std::string& parameter, Value& _value, int flags = 0) {
            return value(parameter, _value, sections.begin(), parameters.begin(), flags);
        }

        /**
         * @brief Nalezení sekce
         *
         * @param   section     Název sekce
         * @param   begin       Počátek hledání (použitelné v souborech s více stejnými sekcemi)
         * @return  Ukazatel na nalezenou sekci použitelný ve funkcích ConfParser::value,
         *  pokud nebyla nalezena, vrací to samé jako ConfParser::sectionNotFound
         */
        sectionPointer section(const std::string& name, sectionPointer begin);

        /**
         * @brief Nalezení sekce
         *
         * Hledá se od počátku souboru
         * @param   section     Název sekce
         * @return  Ukazatel na nalezenou sekci použitelný ve funkcích ConfParser::value,
         *  pokud nebyla nalezena, vrací to samé jako ConfParser::sectionNotFound
         */
        inline sectionPointer section(const std::string& name) {
            return section(name, sections.begin());
        }
    private:
        /** Soubor s konfigurákem */
        std::ifstream file;

        /** Jméno souboru s konfigurákem */
        std::string filename;

        /** Vektor s parametry a komentáři */
        std::vector<Parameter> parameters;

        /** Vektor s názvy a pozicemi sekcí */
        std::vector<Section> sections;

        /**
         * @brief Zničení objektu
         *
         * Uvolnění všech dat a uzavření souboru
         */
        inline void destroy(void) { file.close(); }

        /**
         * @brief (Znovu)nalezení sekcí v konfiguráku
         *
         * Interně jsou na pozice sekcí používány iterátory, při změně dat (načtení
         * souboru) jsou rozházeny a musí se znovu nalézt
         */
        void reloadSections(void);
};

#endif