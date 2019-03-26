#include "utils.h"
#include <codecvt>
#include <locale>

using converter = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>;
static converter conv;

std::string wstr_to_str(std::wstring s) { return conv.to_bytes(s); }
