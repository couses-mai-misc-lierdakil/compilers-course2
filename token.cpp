#include "token.h"
#include <sstream>
#include <type_traits>

namespace Tok {

std::wstring toString(const Token &tok) {
    return std::visit([](const auto& v) {
        std::wstringstream s;
        s << "Token " << v.name;
        if constexpr(!std::is_same_v<decltype(v.value), std::monostate>) {
            s << " with value " << v.value;
        }
        return s.str();
    }, tok);
}

std::wostream &operator<<(std::wostream &s, const Token &tok)
{
    return s << toString(tok);
}

}
