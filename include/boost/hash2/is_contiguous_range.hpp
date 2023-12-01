#ifndef BOOST_HASH2_IS_CONTIGUOUS_RANGE_HPP_INCLUDED
#define BOOST_HASH2_IS_CONTIGUOUS_RANGE_HPP_INCLUDED

// Copyright 2017, 2018 Peter Dimov.
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

template<class It, class T, class S>
    std::integral_constant< bool, std::is_same<typename std::iterator_traits<It>::value_type, T>::value && std::is_integral<S>::value >
        is_contiguous_range_check( It first, It last, T const*, T const*, S );

template<class T> decltype( is_contiguous_range_check( std::declval<T&>().begin(), std::declval<T&>().end(), std::declval<T&>().data(), std::declval<T&>().data() + std::declval<T&>().size(), std::declval<T&>().size() ) ) is_contiguous_range_( int );
template<class T> std::false_type is_contiguous_range_( ... );

} // namespace detail

template<class T> struct is_contiguous_range: decltype( detail::is_contiguous_range_<T>( 0 ) )
{
};

} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_IS_CONTIGUOUS_RANGE_HPP_INCLUDED
