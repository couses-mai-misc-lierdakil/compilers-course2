#include <locale>
#include <codecvt>
#include "utils.h"

using converter = std::wstring_convert<std::codecvt_utf8<wchar_t>,wchar_t>;
static converter conv;

std::runtime_error runtime_error(std::wstring what)
{
    return std::runtime_error(conv.to_bytes(what));
}
