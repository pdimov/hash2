#ifndef BOOST_HASH2_DETAIL_ROT_HPP_INCLUDED
#define BOOST_HASH2_DETAIL_ROT_HPP_INCLUDED

// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/config.hpp>
#include <cstdint>
#include <cstring>
#include <cstdlib>

namespace boost
{
namespace hash2
{
namespace detail
{

#if defined( _MSC_VER )

BOOST_FORCEINLINE std::uint32_t rotl( std::uint32_t v, int k )
{
    return _rotl( v, k );
}

BOOST_FORCEINLINE std::uint64_t rotl( std::uint64_t v, int k )
{
    return _rotl64( v, k );
}

BOOST_FORCEINLINE std::uint32_t rotr( std::uint32_t v, int k )
{
    return _rotr( v, k );
}

BOOST_FORCEINLINE std::uint64_t rotr( std::uint64_t v, int k )
{
    return _rotr64( v, k );
}

#else

// k must not be 0
BOOST_FORCEINLINE std::uint32_t rotl( std::uint32_t v, int k )
{
    return ( v << k ) + ( v >> ( 32 - k ) );
}

// k must not be 0
BOOST_FORCEINLINE std::uint64_t rotl( std::uint64_t v, int k )
{
    return ( v << k ) + ( v >> ( 64 - k ) );
}

// k must not be 0
BOOST_FORCEINLINE std::uint32_t rotr( std::uint32_t v, int k )
{
    return ( v >> k ) | ( v << ( 32 - k ) );
}

BOOST_FORCEINLINE std::uint64_t rotr( std::uint64_t v, int k )
{
    return ( v >> k ) | ( v << ( 64 - k ) );
}

#endif

} // namespace detail
} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_DETAIL_ROT_HPP_INCLUDED
