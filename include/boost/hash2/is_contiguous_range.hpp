
// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.

#ifndef BOOST_HASH2_IS_CONTIGUOUS_RANGE_HPP_INCLUDED
#define BOOST_HASH2_IS_CONTIGUOUS_RANGE_HPP_INCLUDED

#include <boost/type_traits/integral_constant.hpp>
#include <boost/config.hpp>

#if !defined(BOOST_NO_CXX11_DECLTYPE) && !defined(BOOST_NO_SFINAE_EXPR)

#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/declval.hpp>
#include <boost/type_traits/is_same.hpp>
#include <iterator>

namespace boost
{
namespace hash2
{

namespace detail
{

template<class It, class T, class S>
    integral_constant< bool, is_same<typename std::iterator_traits<It>::value_type, T>::value && is_integral<S>::value >
        is_contiguous_range_check( It first, It last, T const*, T const*, S );

template<class T> decltype( is_contiguous_range_check( declval<T&>().begin(), declval<T&>().end(), declval<T&>().data(), declval<T&>().data() + declval<T&>().size(), declval<T&>().size() ) ) is_contiguous_range_( int );
template<class T> false_type is_contiguous_range_( ... );

} // namespace detail

template<class T> struct is_contiguous_range: decltype( detail::is_contiguous_range_<T>( 0 ) )
{
};

} // namespace hash2
} // namespace boost

#else // !defined(BOOST_NO_CXX11_DECLTYPE) && !defined(BOOST_NO_SFINAE_EXPR)

#include <boost/array.hpp>
#include <cstddef>
#include <vector>
#include <string>
#if !defined(BOOST_NO_CXX11_HDR_ARRAY)
#include <array>
#endif

namespace boost
{
namespace hash2
{

template<class T> struct is_contiguous_range: false_type
{
};

template<class T, std::size_t N> struct is_contiguous_range< boost::array<T, N> >: true_type
{
};

template<class T, std::size_t N> struct is_contiguous_range< boost::array<T, N> const >: true_type
{
};

template<class T, class A> struct is_contiguous_range< std::vector<T, A> >: true_type
{
};

template<class T, class A> struct is_contiguous_range< std::vector<T, A> const >: true_type
{
};

template<class E, class T, class A> struct is_contiguous_range< std::basic_string<E, T, A> >: true_type
{
};

template<class E, class T, class A> struct is_contiguous_range< std::basic_string<E, T, A> const >: true_type
{
};

#if !defined(BOOST_NO_CXX11_HDR_ARRAY)

template<class T, std::size_t N> struct is_contiguous_range< std::array<T, N> >: true_type
{
};

template<class T, std::size_t N> struct is_contiguous_range< std::array<T, N> const >: true_type
{
};

#endif

} // namespace hash2
} // namespace boost

#endif // !defined(BOOST_NO_CXX11_DECLTYPE) && !defined(BOOST_NO_SFINAE_EXPR)

#endif // #ifndef BOOST_HASH2_IS_CONTIGUOUS_RANGE_HPP_INCLUDED
