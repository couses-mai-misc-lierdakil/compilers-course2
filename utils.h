#ifndef UTILS_H
#define UTILS_H

#include <string>

std::string wstr_to_str(std::wstring s);

template <typename T>
inline constexpr std::false_type::value_type always_false =
    std::false_type::value;

#endif // UTILS_H
