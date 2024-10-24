#ifndef BOOST_HASH2_IS_CONTIGUOUSLY_HASHABLE_HPP_INCLUDED
#define BOOST_HASH2_IS_CONTIGUOUSLY_HASHABLE_HPP_INCLUDED

// Copyright 2017, 2023 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/endian.hpp>
#include <type_traits>
#include <cstddef>

namespace boost
{
namespace hash2
{

// is_trivially_equality_comparable

template<class T> struct is_trivially_equality_comparable:
    std::integral_constant< bool, std::is_integral<T>::value || std::is_enum<T>::value || std::is_pointer<T>::value >
{
};

template<class T> struct is_trivially_equality_comparable<T const>:
    is_trivially_equality_comparable<T>
{
};

// is_endian_independent

template<class T> struct is_endian_independent:
    std::integral_constant< bool, sizeof(T) == 1 >
{
};

template<class T> struct is_endian_independent<T const>:
    is_endian_independent<T>
{
};

// is_contiguously_hashable

template<class T, endian E> struct is_contiguously_hashable:
    std::integral_constant<bool, is_trivially_equality_comparable<T>::value && (E == endian::native || is_endian_independent<T>::value)>
{
};

template<class T, std::size_t N, endian E> struct is_contiguously_hashable<T[N], E>:
    is_contiguously_hashable<T, E>
{
};

} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_IS_CONTIGUOUSLY_HASHABLE_HPP_INCLUDED
