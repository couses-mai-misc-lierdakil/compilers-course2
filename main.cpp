#include <iostream>
#include <sstream>
#include "lexer.h"
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
    std::wstring line;
    while (!std::getline(std::wcin, line).eof()) {
      std::wstringstream ss(line);
      Lexer lexer(ss);
      Parser parser(lexer);
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
