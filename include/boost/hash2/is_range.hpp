#ifndef BOOST_HASH2_IS_RANGE_HPP_INCLUDED
#define BOOST_HASH2_IS_RANGE_HPP_INCLUDED

// Copyright 2017 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <type_traits>
#include <iterator>

namespace boost
{
namespace hash2
{

namespace detail
{

template<class It> std::true_type is_range_check( It first, It last, typename std::iterator_traits<It>::difference_type* = 0 );

template<class T> decltype( is_range_check( std::declval<T&>().begin(), std::declval<T&>().end() ) ) is_range_( int );
template<class T> std::false_type is_range_( ... );

} // namespace detail

template<class T> struct is_range: decltype( detail::is_range_<T>( 0 ) )
{
};

} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_IS_RANGE_HPP_INCLUDED
