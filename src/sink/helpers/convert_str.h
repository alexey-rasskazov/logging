#pragma once

#include <type_traits>

template<class T, class F>
std::enable_if_t<!std::is_same_v<T, F>, T>
convert_str(const F &from)
{
    return {from.begin(), from.end()};
}

template<class T>
T&& convert_str(T &&from)
{
    return static_cast<T&&>(from);
}
