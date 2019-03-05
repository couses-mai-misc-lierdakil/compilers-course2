#include <iostream>
#include <list>
#include <string>
#include "lexer.h"
#include "token.h"
#include <map>
#include <cmath>
#include "parser.h"

/* Notable changes:
 * - Lexer EOF handling
 * - Grammar
 * - Reset lexer on newline
 * - Wstring
 */

int main()
{
    std::ios::sync_with_stdio(false);
    std::locale::global(std::locale("en_US.UTF8"));
    std::wcin.imbue(std::locale());
    std::wcout.imbue(std::locale());
    std::wcerr.imbue(std::locale());
    Lexer lexer(std::wcin);
    Parser parser(lexer);
    for (;;) {
        try {
            auto result = parser.parse();
            std::wcout << "Result: " << result << std::endl;
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
            if (std::wcin.eof()) break;
        }
    }
    return 0;
}
