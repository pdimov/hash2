#ifndef BOOST_HASH2_IS_TUPLE_LIKE_HPP_INCLUDED
#define BOOST_HASH2_IS_TUPLE_LIKE_HPP_INCLUDED

// Copyright 2017 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/config.hpp>
#include <boost/config/workaround.hpp>
#include <utility>

#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES) && !defined(BOOST_NO_CXX11_HDR_TUPLE) && !BOOST_WORKAROUND(BOOST_MSVC, == 1800)

namespace boost
{
namespace hash2
{

namespace detail
{

template<class T, class E = true_type> struct is_tuple_like_: false_type
{
};

template<class T> struct is_tuple_like_<T, integral_constant<bool, std::tuple_size<T>::value == std::tuple_size<T>::value> >: true_type
{
};

} // namespace detail

template<class T> struct is_tuple_like: detail::is_tuple_like_<typename remove_cv<T>::type>
{
};

} // namespace hash2
} // namespace boost

#else

#if !defined(BOOST_NO_CXX11_HDR_ARRAY)
# include <array>
#endif

#if !defined(BOOST_NO_CXX11_HDR_TUPLE)
# include <tuple>
#endif

namespace boost
{
namespace hash2
{

template<class T> struct is_tuple_like: false_type
{
};

template<class T> struct is_tuple_like<T const>: is_tuple_like<T>
{
};

template<class T1, class T2> struct is_tuple_like< std::pair<T1, T2> >: true_type
{
};

#if !defined(BOOST_NO_CXX11_HDR_ARRAY)

template<class T, std::size_t N> struct is_tuple_like< std::array<T, N> >: true_type
{
};

#endif // !defined(BOOST_NO_CXX11_HDR_ARRAY)

#if !defined(BOOST_NO_CXX11_HDR_TUPLE)

#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

template<class... T> struct is_tuple_like< std::tuple<T...> >: true_type
{
};

#else

template<> struct is_tuple_like< std::tuple<> >: true_type
{
};

template<class T1> struct is_tuple_like< std::tuple<T1> >: true_type
{
};

template<class T1, class T2> struct is_tuple_like< std::tuple<T1, T2> >: true_type
{
};

template<class T1, class T2, class T3> struct is_tuple_like< std::tuple<T1, T2, T3> >: true_type
{
};

template<class T1, class T2, class T3, class T4> struct is_tuple_like< std::tuple<T1, T2, T3, T4> >: true_type
{
};

template<class T1, class T2, class T3, class T4, class T5> struct is_tuple_like< std::tuple<T1, T2, T3, T4, T5> >: true_type
{
};

template<class T1, class T2, class T3, class T4, class T5, class T6> struct is_tuple_like< std::tuple<T1, T2, T3, T4, T5, T6> >: true_type
{
};

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7> struct is_tuple_like< std::tuple<T1, T2, T3, T4, T5, T6, T7> >: true_type
{
};

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8> struct is_tuple_like< std::tuple<T1, T2, T3, T4, T5, T6, T7, T8> >: true_type
{
};

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

#endif // !defined(BOOST_NO_CXX11_HDR_TUPLE)

} // namespace hash2
} // namespace boost

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES) && !defined(BOOST_NO_CXX11_HDR_TUPLE) && !BOOST_WORKAROUND(BOOST_MSVC, == 1800)

#endif // #ifndef BOOST_HASH2_IS_TUPLE_LIKE_HPP_INCLUDED
