#ifndef BOOST_HASH2_DETAIL_READ_HPP_INCLUDED
#define BOOST_HASH2_DETAIL_READ_HPP_INCLUDED

// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/config.hpp>
#include <cstdint>
#include <cstring>

namespace boost
{
namespace hash2
{
namespace detail
{

BOOST_FORCEINLINE std::uint32_t read32le( unsigned char const * p )
{
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386) || defined(_M_IX86)

    std::uint32_t v;
    std::memcpy( &v, p, sizeof(v) );
    return v;

#else

    return
        static_cast<std::uint32_t>( p[0] ) |
        ( static_cast<std::uint32_t>( p[1] ) <<  8 ) |
        ( static_cast<std::uint32_t>( p[2] ) << 16 ) |
        ( static_cast<std::uint32_t>( p[3] ) << 24 );

#endif
}

BOOST_FORCEINLINE std::uint64_t read64le( unsigned char const * p )
{
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386) || defined(_M_IX86)

    std::uint64_t v;
    std::memcpy( &v, p, sizeof(v) );
    return v;

#else

    return
        static_cast<std::uint64_t>( p[0] ) |
        ( static_cast<std::uint64_t>( p[1] ) <<  8 ) |
        ( static_cast<std::uint64_t>( p[2] ) << 16 ) |
        ( static_cast<std::uint64_t>( p[3] ) << 24 ) |
        ( static_cast<std::uint64_t>( p[4] ) << 32 ) |
        ( static_cast<std::uint64_t>( p[5] ) << 40 ) |
        ( static_cast<std::uint64_t>( p[6] ) << 48 ) |
        ( static_cast<std::uint64_t>( p[7] ) << 56 );

#endif
}

BOOST_FORCEINLINE std::uint32_t read32be( unsigned char const * p )
{
    return
        static_cast<std::uint32_t>( p[3] ) |
        ( static_cast<std::uint32_t>( p[2] ) <<  8 ) |
        ( static_cast<std::uint32_t>( p[1] ) << 16 ) |
        ( static_cast<std::uint32_t>( p[0] ) << 24 );
}

BOOST_FORCEINLINE std::uint64_t read64be( unsigned char const * p )
{
    return
        static_cast<std::uint64_t>( p[7] ) |
        ( static_cast<std::uint64_t>( p[6] ) <<  8 ) |
        ( static_cast<std::uint64_t>( p[5] ) << 16 ) |
        ( static_cast<std::uint64_t>( p[4] ) << 24 ) |
        ( static_cast<std::uint64_t>( p[3] ) << 32 ) |
        ( static_cast<std::uint64_t>( p[2] ) << 40 ) |
        ( static_cast<std::uint64_t>( p[1] ) << 48 ) |
        ( static_cast<std::uint64_t>( p[0] ) << 56 );
}

} // namespace detail
} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_DETAIL_READ_HPP_INCLUDED
