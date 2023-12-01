#ifndef BOOST_HASH2_IS_TUPLE_LIKE_HPP_INCLUDED
#define BOOST_HASH2_IS_TUPLE_LIKE_HPP_INCLUDED

// Copyright 2017 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <type_traits>
#include <utility>
#include <cstddef>

namespace boost
{

template<class T, std::size_t N> class array;
	
namespace hash2
{

namespace detail
{

template<class T, class E = std::true_type> struct is_tuple_like_: std::false_type
{
};

template<class T> struct is_tuple_like_<T, std::integral_constant<bool, std::tuple_size<T>::value == std::tuple_size<T>::value> >: std::true_type
{
};

template<class T, std::size_t N> struct is_tuple_like_< boost::array<T, N> >: std::true_type
{
};

} // namespace detail

template<class T> struct is_tuple_like: detail::is_tuple_like_<typename std::remove_cv<T>::type>
{
};

} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_IS_TUPLE_LIKE_HPP_INCLUDED
