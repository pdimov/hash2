#ifndef BOOST_HASH2_IS_CONTIGUOUSLY_HASHABLE_HPP_INCLUDED
#define BOOST_HASH2_IS_CONTIGUOUSLY_HASHABLE_HPP_INCLUDED

// Copyright 2017 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_enum.hpp>
#include <boost/type_traits/is_pointer.hpp>

namespace boost
{
namespace hash2
{

template<class T, class H> struct is_contiguously_hashable:
    integral_constant< bool, is_integral<T>::value || is_enum<T>::value || is_pointer<T>::value >
{
};

template<class T, class H> struct is_contiguously_hashable<T const, H>:
    is_contiguously_hashable<T, H>
{
};

template<class T, std::size_t N, class H> struct is_contiguously_hashable<T[N], H>:
    is_contiguously_hashable<T, H>
{
};

template<class T, std::size_t N, class H> struct is_contiguously_hashable<T const[N], H>:
    is_contiguously_hashable<T, H>
{
};

} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_IS_CONTIGUOUSLY_HASHABLE_HPP_INCLUDED
