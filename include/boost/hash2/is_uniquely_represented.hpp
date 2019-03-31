#ifndef BOOST_HASH2_IS_UNIQUELY_REPRESENTED_HPP_INCLUDED
#define BOOST_HASH2_IS_UNIQUELY_REPRESENTED_HPP_INCLUDED

// Copyright 2017, 2019 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_enum.hpp>
#include <boost/type_traits/is_pointer.hpp>

namespace boost
{
namespace hash2
{

template<class T> struct is_uniquely_represented:
    integral_constant< bool, is_integral<T>::value || is_enum<T>::value || is_pointer<T>::value >
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
