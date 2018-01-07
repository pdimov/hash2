
// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.

#ifndef BOOST_HASH2_ENDIAN_HPP_INCLUDED
#define BOOST_HASH2_ENDIAN_HPP_INCLUDED

#include <boost/config.hpp>
#include <boost/predef/other/endian.h>

namespace boost
{
namespace hash2
{

#if !defined(BOOST_NO_CXX11_SCOPED_ENUMS)

enum class endian
{
    little,
    big,
    native
#if BOOST_ENDIAN_LITTLE_BYTE
        = little
#elif BOOST_ENDIAN_BIG_BYTE
        = big
#endif
};

typedef endian endian_type;

#else

struct endian
{

enum endian_type
{
    little,
    big,
    native
#if BOOST_ENDIAN_LITTLE_BYTE
        = little
#elif BOOST_ENDIAN_BIG_BYTE
        = big
#endif
};

};

typedef endian::endian_type endian_type;

#endif // !defined(BOOST_NO_CXX11_SCOPED_ENUMS)

} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_ENDIAN_HPP_INCLUDED
