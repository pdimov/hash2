#ifndef BOOST_HASH2_IS_UNIQUELY_REPRESENTED_HPP_INCLUDED
#define BOOST_HASH2_IS_UNIQUELY_REPRESENTED_HPP_INCLUDED

// Copyright 2017, 2019 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <type_traits>
#include <cstddef>

namespace boost
{
namespace hash2
{

template<class T> struct is_uniquely_represented:
    std::integral_constant< bool, std::is_integral<T>::value || std::is_enum<T>::value || std::is_pointer<T>::value >
{
};

template<class T> struct is_uniquely_represented<T const>:
    is_uniquely_represented<T>
{
};

template<class T, std::size_t N> struct is_uniquely_represented<T[N]>:
    is_uniquely_represented<T>
{
};

template<class T, std::size_t N> struct is_uniquely_represented<T const[N]>:
    is_uniquely_represented<T>
{
};

} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_IS_UNIQUELY_REPRESENTED_HPP_INCLUDED
