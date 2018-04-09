#pragma once

#include <ostream>

template <typename... Args>
class HeaderImpl;

template<typename T>
class HeaderImpl<T>
{
public:
    HeaderImpl(T value);

    void write(std::ostream& os) const;

private:
    T m_value;
};

template<typename T, typename U, typename... Args>
class HeaderImpl<T, U, Args...> : private HeaderImpl<U, Args...>
{
public:
    HeaderImpl(T value, U othervalue, Args ... arg);
    HeaderImpl(T value, HeaderImpl<U, Args...>&& h);

    void write(std::ostream& os) const;

private:
    T m_value;
};

class Header
{
public:
    template<typename... Args>
    static auto GetHeader(Args&&... arg)
    {
        return HeaderImpl<Args...>(std::forward<Args>(arg)...);
    }
};

template<typename... Args>
std::ostream& operator<< (std::ostream& os, const HeaderImpl<Args...>& value)
{
    value.write(os);
    return os;
}

template<typename T>
HeaderImpl<T>::HeaderImpl(T value)
    : m_value(value)
{}

template<typename T>
void HeaderImpl<T>::write(std::ostream& os) const
{
    os << m_value;
};

template<typename T, typename U, typename ...Args>
HeaderImpl<T, U, Args...>::HeaderImpl(T value, U othervalue, Args ...arg)
    : HeaderImpl<U, Args...>(othervalue, arg...)
    , m_value(value)
{}

template<typename T, typename U, typename ...Args>
inline HeaderImpl<T, U, Args...>::HeaderImpl(T value, HeaderImpl<U, Args...>&& h)
    : HeaderImpl<U, Args...>(std::move(h))
    , m_value(value)
{
}

template<typename T, typename U, typename ...Args>
void HeaderImpl<T, U, Args...>::write(std::ostream& os) const
{
    // Switch these calls to get the proper order for the streaming constructor
    os << m_value;
    HeaderImpl<U, Args...>::write(os);
}
