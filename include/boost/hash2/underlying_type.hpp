
// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.

#ifndef BOOST_HASH2_UNDERLYING_TYPE_HPP_INCLUDED
#define BOOST_HASH2_UNDERLYING_TYPE_HPP_INCLUDED

#include <boost/type_traits/conditional.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/make_signed.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/config.hpp>

#if !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)

#include <type_traits>

namespace boost
{
namespace hash2
{

using std::underlying_type;

} // namespace hash2
} // namespace boost

#else

namespace boost
{
namespace hash2
{

template<class T> struct underlying_type:
    conditional< is_signed<T>::value, make_signed<T>, make_unsigned<T> >::type
{
};

} // namespace hash2
} // namespace boost

#endif // !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)

#endif // #ifndef BOOST_HASH2_UNDERLYING_TYPE_HPP_INCLUDED
