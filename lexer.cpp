#include <string>
#include "lexer.h"
#include "token.h"
#include "utils.h"
#include <locale>

/* Lexer based on DFA; transition table is:
 * (0,[*+/-]) = 1
 * (0,[:alpha:_]) = 2
 * (0,[:digit:]) = 3
 * (0,[$\n]) = 5
 * (0,[:space:]) = 0
 * (2,[:alpha:_]) = 2
 * (3,[:digit:]) = 3
 * (3,[.]) = 4
 * (4,[:digit:]) = 4
 * all other cases = -1
 * All states except 0 are accepting;
 * 0 corresponds to whitespace;
 * 1 corresponds to operator
 * 2 corresponds to identifier
 * 3 corresponds to integer
 * 4 corresponds to number with fractional part
 * 5 corresponds to EOF or newline
 */

Lexer::Lexer(std::wistream &inputStream) : inputStream(inputStream) {}

static int dtable(int s, wint_t c) {
    static std::wstring operatorChars(L"=+-*/");
    wchar_t ch = static_cast<wchar_t>(c);
    if (s == 0 && operatorChars.find(ch) != operatorChars.npos) return 1;
    else if (s == 0 && (std::isalpha(ch, std::locale()) || ch == '_')) return 2;
    else if (s == 0 && std::isdigit(ch, std::locale())) return 3;
    else if (s == 2 && (std::isalnum(ch, std::locale()) ||  ch == '_')) return 2;
    else if (s == 3 && std::isdigit(ch, std::locale())) return 3;
    else if (s == 3 && c == '.') return 4;
    else if (s == 4 && std::isdigit(ch, std::locale())) return 4;
    else if (s == 0 && (c == WEOF || ch == '\n')) return 5;
    else if (s == 0 && std::isspace(ch, std::locale())) return 0;
    else return -1;
}

wint_t Lexer::getNextChar() {
    if (ret_buf.empty()) {
        return inputStream.get();
    } else {
      wchar_t ch = ret_buf.front();
      ret_buf.pop_front();
      return static_cast<wint_t>(ch);
    }
}

Tok::Token Lexer::getNextToken()
{
    int state = 0;
    wint_t curch_;
    std::list<wchar_t> buf;
    std::list<wchar_t> acc_buf;
    int la_state = -1;
    for (;;) {
        curch_ = getNextChar();
        state = dtable(state, curch_);
        wchar_t curch = static_cast<wchar_t>(curch_);
        if (curch_ != WEOF) buf.push_back(curch);
        if (state > 0) {
            la_state = state;
            acc_buf.splice(acc_buf.end(), buf);
        }
        if (state == -1 || state == 5) {
            ret_buf.splice(ret_buf.begin(), buf);
            if (la_state == 1) {
                std::wstring op(acc_buf.begin(), acc_buf.end());
                return Tok::Op{op[0]};
            } else if (la_state == 2) {
                std::wstring var(acc_buf.begin(), acc_buf.end());
                return Tok::Var{var};
            } else if (la_state == 3 || la_state == 4) {
                std::wstring num(acc_buf.begin(), acc_buf.end());
                return Tok::Num{std::stod(num)};
            } else if (la_state == 5) {
                return Tok::Eof{};
            } else {
                // there was no accepted state
                getNextChar();
                throw runtime_error(
                                L"Lexer error: invalid character "
                                + std::wstring({curch}));
            }
            state = 0;
            la_state = -1;
            acc_buf.clear();
        }
        if (state == 0) {
            buf.clear();
        }
    }
}
