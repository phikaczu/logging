#pragma once
#include "Header.h"

class Header2
{
    // dummy
};

template<typename T>
HeaderImpl<T> operator<< (Header2&& h, T value)
{
    return HeaderImpl<T>(value);
}

template<typename... Args,typename T>
HeaderImpl<T, Args...> operator<< (HeaderImpl<Args...>&& h, T value)
{
    return HeaderImpl<T, Args...>(value, std::move(h));
}
