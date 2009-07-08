#ifndef CONFPARSER_H
#define CONFPARSER_H

/**
 * @file ConfParser.h
 * @brief Třída ConfParser
 */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>      /* std::istringstream */
#include <cstring>      /* strcmp() */

#include "utility.h"

/**
@page ConfParser Formát conf souborů
Konfigurační soubory mají jendoduchou sytaxi, která je pohodlná pro uživatele i
pro parser. Konfigurační soubor je možné dělit do sekcí a libovolně opatřovat
komentáři.
@section ConfSyntax Syntaxe conf souboru
<pre>
parametr=hodnota
[názevSekce]
\#Komentář na více
\#řádků

;Komentář
parametr=hodnota

[jináSekce]
jinýParametr=" hodnota "
dalšíParametr='hodnota'
jinýParametr=hodnota
</pre>
Na řádku může být samostatně buď @ref ConfSyntaxComments "komentář", název
@ref ConfSyntaxSections "sekce" nebo @ref ConfSyntaxParameters "parametr" s
hodnotou. V souboru může být libovolný počet prázdných řádků, mezery na začátku
a konci řádku nehrají roli. Pokud je na začátku souboru nalezena UTF8 signatura
<tt>EF BB BF</tt> (například při uložení z widláckého Notepadu), je automaticky
detekována a přeskočena.
@subsection ConfSyntaxComments Komentáře
Komentář musí vždy začínat znakem <tt>\#</tt> nebo <tt>;</tt> na začátku řádku.
Komentář může být maximálně dlouhý ConfParser::MAX_LINE_LENGTH znaků.
@subsection ConfSyntaxSections Sekce
Název sekce je ohraničen znaky <tt>[</tt> a <tt>]</tt>, vše co následuje za
tímto řádkem, patří do této sekce, dokud se nedosáhne řádku s názvem další
sekce. Pokud jsou před první sekcí uvedeny parametry, patří do sekce pojmenované
ConfParser::DEFAULT_SECTION. Maximální délka názvu sekce je
ConfParser::MAX_LINE_LENGTH znaků.
@subsection ConfSyntaxParameters Parametry a hodnoty (tj. vlastní data)
Název parametru je vše co je před znakem <tt>=</tt> (kromě úvodních mezer na
řádku), jeho délka je omezena na ConfParser::MAX_LINE_LENGTH znaků. Hodnota může
být uzavřena do uvozovek <tt>"</tt> nebo apostrofů <tt>'</tt>, pokud není, jsou
u ní osekány počáteční a koncové mezery. Maximální délka hodnoty je taktéž
ConfParser::MAX_LINE_LENGTH znaků.
@section ConfDataTypes Druhy hodnot
V conf souboru můžou být parametry typu @ref ConfTypeInt, @ref ConfTypeDouble,
@ref ConfTypeBool, @ref ConfTypeString a @ref ConfTypeAlign. Je možné jakýkoli
typ extrahovat jako text pomocí <tt>ConfParser::value(string, string&)</tt>
například pro účely přímého výpisu.
@subsection ConfTypeInt Integer
Normální číselná hodnota. Extrahovatelné pomocí
<tt>ConfParser::value(string, int&)</tt>. Příklad:
<pre>param=3475</pre>
U hexadecimálních hodnot nezáleží na velikosti písmen, jsou extrahovatelné pomocí
<tt>ConfParser::value(string, int&, ConfParser::HEX)</tt>. Příklad:
<pre>param=8FC680
param=c368ab</pre>
Hexadecimální barevné reprezentace ve formátu RGB mohou, ale nemusí mít na
začátku znak <tt>\#</tt>. Extrahovatelné pomocí
<tt>ConfParser::value(string, int&, ConfParser::COLOR)</tt>. Výsledkem je číslo,
z kterého lze pomocí binárního AND a bitových posunů extrahovat jednotlivé
barevné složky. Příklad:
<pre>
param=\#ff3366
param=00CC3F
</pre>
@subsection ConfTypeDouble Double
Desetinná čísla. Je nutné uvést tečku, ne čárku. Extrahovatelné pomocí
<tt>ConfParser::value(string, double&)</tt>. Příklad:
<pre>param=3.1415</pre>
@subsection ConfTypeBool Boolean
Pokud je jako hodnota parametru uvedeno <tt>true</tt>, <tt>t</tt>, <tt>yes</tt>,
<tt>y</tt> nebo <tt>1</tt>, je to bráno jako pravda, ostatní hodnoty jsou brány
jako nepravda. Extrahovatelné pomocí <tt>ConfParser::value(string, bool&)</tt>.
Příklad:
<pre>param=true</pre>
@subsection ConfTypeString String
Pokud je uvedený v uvozovkách nebo apostrofech, nebudou mu osekány počáteční a
koncové mezery. Extrahovatelný pomocí <tt>ConfParser::value(string, string&)</tt>.
Příklad:
<pre>param="text blah blah blah blah"
param=Jedno slovo
</pre>
@subsection ConfTypeAlign Align
Zarovnání je v conf souboru specifikováno dvěma slovy oddělenými libovolným
počtem bílých znaků. Všechna písmena musí být malá. Řetězce <tt>top</tt>,
<tt>middle</tt> a <tt>bottom</tt> platí pro vertikální zarovnání, řetězce
<tt>left</tt>, <tt>center</tt> a <tt>right</tt> pro horizontální. Default
hodnota je vlevo nahoře, takže např. při neuvedení horizontálního směru
je tento nastaven na left. Při uvedení více než dvou slov se postupuje zleva
shora, když je tedy uvedeno zarovnání <tt>left right</tt>, platí <tt>left</tt>.
Extrahovatelné pomocí <tt>ConfParser::value(string, Align&)</tt>.
Příklad:
<pre>\# Zarovnání doprava dolů
param=right bottom
\# Také zarovnání doprava dolů
param="bottom right"
\# Zarovnání doleva nahoru (doleva je dosazeno jako default hodnota)
param=top
\# Zarovnání doprostřed nahoru (vynechány parametry více vpravo a dole)
param="center right top middle bottom"
</pre>
@section duplicateNames Sekce a hodnoty s duplicitními názvy
Conf soubory mohou obsahovat více sekcí nebo parametrů se stejným názvem a jejich
výběr není omezen.
@todo Dopsat!
 */

/**
 * @brief Parser konfiguračních souborů
 *
 * @ref ConfParser
 * @todo Specifikace conf souborů do nějaké spec. stránky
 * @todo Testovat parametry jen na alfanumerické znaky
 * @todo Ošetřit escape znaky
 * @todo Default sekce jen když obsahuje nějaké parametry
 */
class ConfParser {
    private:
        /** @brief Parametr konfiguračního souboru */
        struct Parameter {
            std::string parameter,  /**< @brief Parametr */
                        value;      /**< @brief Hodnota */
        };

        /** @brief Sekce konfiguračního souboru */
        struct Section {
            std::string section;    /**< @brief Název sekce */

            /** @brief Ukazatel na první parametr v sekci */
            std::vector<Parameter>::const_iterator begin;
        };
    public:
        /** @brief Flags při výběru hodnoty int */
        enum flags {
            HEX = 0x01,     /**< @brief Číslo je v hexadecimálním formátu (např. ffa806, na velikosti písem nezáleží). */
            COLOR = 0x02    /**< @brief Číslo reprezentuje barvu (ff3366 i s # na začátku, na velikosti písem nezáleží). */
        };

        /** @brief Maximální délka komentáře, názvu sekce, parametru nebo hodnoty parametru */
        static const int MAX_LINE_LENGTH;

        /** @brief Název první sekce v konfiguráku */
        static const std::string DEFAULT_SECTION;

        /** @brief Ukazatel na sekci */
        typedef std::vector<Section>::const_iterator sectionPointer;

        /** @brief Ukazatel na parametr */
        typedef std::vector<Parameter>::const_iterator parameterPointer;

        /**
         * @brief Získání názvu konfiguračního souboru
         * @return  Název souboru
         */
        inline std::string getFilename(void) const { return filename; }

        /**
         * @brief Žádný parametr nenalezen
         *
         * Použitelné pro porovnávání s návratovou hodnotou ConfParser::value.
         * @return Iterátor parameters.end()
         */
        inline parameterPointer parameterNotFound() const { return parameters.end(); }

        /**
         * @brief Žádná sekce nenalezena
         *
         * Použitelné pro porovnávání s návratovou hodnotou ConfParser::section.
         * @return Iterátor sections.end()
         */
        inline sectionPointer sectionNotFound() const { return sections.end(); }

        /** @brief Implicitní konstruktor */
        ConfParser(void) {}

        /**
         * @brief Konstruktor
         * @param   _file   Konfigurační soubor
         */
        ConfParser(std::string _file);

        /** @brief Destruktor */
        inline ~ConfParser(void) { destroy(); }

        /**
         * @brief Přeypování na bool
         *
         * Vrací true, pokud byl načten alespoň jeden parametr z konfiguračního souboru.
         */
        inline operator bool(void) { return !(parameters.size() == 0); }

        /**
         * @brief Kopírovací konstruktor
         *
         * Při kopírování do čistého objektu se musí reloadovat iterátory
         * ukazující na sekce, aby při zrušení původce dat nedošlo k segfault.
         * @param   conf    Kopírovaný objekt
         */
        inline ConfParser(const ConfParser& conf): filename(conf.filename), parameters(conf.parameters) {
            file.open(filename.c_str());
            reloadSections();
        }

        /**
         * @brief Operátor přiřazení
         *
         * Při přepisování stávajícího objektu se musí korektně zničit předek
         * a reloadovat iterátory původce nových dat.
         * @param   conf        Přiřazovaný objekt
         * @return  Přiřazený objekt
         */
        ConfParser& operator= (const ConfParser& conf);

        /**
         * @brief Nalezení hodnoty parametru
         *
         * Pro informace o číselných hodnotách viz @ref ConfTypeInt a
         * @ref ConfTypeDouble.
         * @param   parameter   Název parametru
         * @param   _value      Reference na proměnnou pro uložení hodnoty
         * @param   section     Sekce, ve které se bude hledat
         * @param   begin       Počáteční parametr, od kterého se bude hledat
         *  (použitelné v souborech s více stejně nazvanými parametry v jedné sekci)
         * @param   flags       Viz ConfParser::flags, použitelné při získávání int
         * @return  Ukazatel na nalezený parametr, pokud nebyl nalezen, vrací to
         *  samé jako ConfParser::parameterNotFound
         */
        template<class Value> parameterPointer value(const std::string& parameter, Value& _value, sectionPointer section, parameterPointer begin, int flags = 0);

        /** @overload */
        template<class Value> inline parameterPointer value(const std::string& parameter, Value& _value, sectionPointer section, int flags = 0) {
            if(section == sections.end()) return parameters.end();
            return value(parameter, _value, section, (*section).begin, flags);
        }

        /** @overload */
        template<class Value> inline parameterPointer value(const std::string& parameter, Value& _value, parameterPointer begin, int flags = 0) {
            return value(parameter, _value, sections.begin(), begin, flags);
        }

        /** @overload */
        template<class Value> inline parameterPointer value(const std::string& parameter, Value& _value, int flags = 0) {
            return value(parameter, _value, sections.begin(), parameters.begin(), flags);
        }

        /**
         * @fn template<> ConfParser::parameterPointer ConfParser::value(const std::string& parameter, std::string& _value, ConfParser::sectionPointer section, ConfParser::parameterPointer begin, int flags)
         * @brief Nalezení textové hodnoty parametru
         *
         * Pro informace o textové hodnotě viz @ref ConfTypeString.
         * @param   parameter   Název parametru
         * @param   _value      Reference na proměnnou pro uložení hodnoty
         * @param   section     Sekce, ve které se bude hledat
         * @param   begin       Počáteční parametr, od kterého se bude hledat
         *  (použitelné v souborech s více stejně nazvanými parametry v jedné sekci)
         * @param   flags       Viz ConfParser::flags, použitelné při získávání int
         * @return  Ukazatel na nalezený parametr, pokud nebyl nalezen, vrací to
         *  samé jako ConfParser::parameterNotFound
         */

        /**
         * @fn template<> ConfParser::parameterPointer ConfParser::value(const std::string& parameter, Align& _value, ConfParser::sectionPointer section, ConfParser::parameterPointer begin, int flags)
         * @brief Nalezení zarovnání
         *
         * Pro informace o zarovnání viz @ref ConfTypeAlign.
         * @param   parameter   Název parametru
         * @param   _value      Reference na proměnnou pro uložení hodnoty
         * @param   section     Sekce, ve které se bude hledat
         * @param   begin       Počáteční parametr, od kterého se bude hledat
         *  (použitelné v souborech s více stejně nazvanými parametry v jedné sekci)
         * @param   flags       Viz ConfParser::flags, použitelné při získávání int
         * @return  Ukazatel na nalezený parametr, pokud nebyl nalezen, vrací to
         *  samé jako ConfParser::parameterNotFound
         */

        /**
         * @fn template<> ConfParser::parameterPointer ConfParser::value(const std::string& parameter, bool& _value, ConfParser::sectionPointer section, ConfParser::parameterPointer begin, int flags)
         * @brief Nalezení bool hodnoty
         *
         * Pro informace o bool viz @ref ConfTypeBool.
         * @param   parameter   Název parametru
         * @param   _value      Reference na proměnnou pro uložení hodnoty
         * @param   section     Sekce, ve které se bude hledat
         * @param   begin       Počáteční parametr, od kterého se bude hledat
         *  (použitelné v souborech s více stejně nazvanými parametry v jedné sekci)
         * @param   flags       Viz ConfParser::flags, použitelné při získávání int
         * @return  Ukazatel na nalezený parametr, pokud nebyl nalezen, vrací to
         *  samé jako ConfParser::parameterNotFound
         */

        /**
         * @brief Nalezení sekce
         *
         * @param   name        Název sekce
         * @param   begin       Počátek hledání (použitelné v souborech s více stejnými sekcemi)
         * @return  Ukazatel na nalezenou sekci použitelný ve funkcích ConfParser::value,
         *  pokud nebyla nalezena, vrací to samé jako ConfParser::sectionNotFound
         */
        sectionPointer section(const std::string& name, sectionPointer begin);

        /** @overload */
        inline sectionPointer section(const std::string& name) {
            return section(name, sections.begin());
        }
    private:
        std::ifstream file;                 /**< @brief Soubor s konfigurákem */
        std::string filename;               /**< @brief Jméno souboru s konfigurákem */
        std::vector<Parameter> parameters;  /**< @brief Vektor s parametry a komentáři */
        std::vector<Section> sections;      /**< @brief Vektor s názvy a pozicemi sekcí */

        /**
         * @brief Zničení objektu
         *
         * Uvolnění všech dat a uzavření souboru.
         */
        inline void destroy(void) { file.close(); }

        /**
         * @brief (Znovu)nalezení sekcí v konfiguráku
         *
         * Interně jsou na pozice sekcí používány iterátory, při změně dat (načtení
         * souboru) jsou rozházeny a musí se znovu nalézt.
         */
        void reloadSections(void);
};

#endif