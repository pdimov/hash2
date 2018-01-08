
// Copyright 2017 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.

#ifndef BOOST_HASH2_IS_RANGE_HPP_INCLUDED
#define BOOST_HASH2_IS_RANGE_HPP_INCLUDED

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/declval.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/core/enable_if.hpp>
#include <boost/config.hpp>
#include <boost/config/workaround.hpp>
#include <iterator>

namespace boost
{
namespace hash2
{

#if !defined(BOOST_NO_CXX11_DECLTYPE) && !defined(BOOST_NO_SFINAE_EXPR) && !BOOST_WORKAROUND(BOOST_GCC, < 40700)

namespace detail
{

template<class It> true_type is_range_check( It first, It last, typename std::iterator_traits<It>::difference_type* = 0 );

template<class T> decltype( is_range_check( declval<T&>().begin(), declval<T&>().end() ) ) is_range_( int );
template<class T> false_type is_range_( ... );

} // namespace detail

template<class T> struct is_range: decltype( detail::is_range_<T>( 0 ) )
{
};

#else

namespace detail
{

template<class T, class E = true_type> struct is_range_: false_type
{
};

template<class T> struct is_range_< T, integral_constant< bool,
        is_same<typename T::value_type, typename std::iterator_traits<typename T::iterator>::value_type>::value &&
        is_same<typename T::value_type, typename std::iterator_traits<typename T::const_iterator>::value_type>::value &&
        is_integral<typename T::size_type>::value
    > >: true_type
{
};

} // namespace detail

template<class T> struct is_range: detail::is_range_<T>
{
};

#endif // !defined(BOOST_NO_CXX11_DECLTYPE) && !defined(BOOST_NO_SFINAE_EXPR)

} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_IS_RANGE_HPP_INCLUDED
