#ifndef BOOST_HASH2_DETAIL_ROT_HPP_INCLUDED
#define BOOST_HASH2_DETAIL_ROT_HPP_INCLUDED

// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hash2/byte_type.hpp>
#include <boost/cstdint.hpp>
#include <boost/config.hpp>
#include <cstring>
#include <cstdlib>

namespace boost
{
namespace hash2
{
namespace detail
{

#if defined( _MSC_VER )

BOOST_FORCEINLINE boost::uint32_t rotl( boost::uint32_t v, int k )
{
    return _rotl( v, k );
}

BOOST_FORCEINLINE boost::uint64_t rotl( boost::uint64_t v, int k )
{
    return _rotl64( v, k );
}

#else

// k must not be 0
BOOST_FORCEINLINE boost::uint32_t rotl( boost::uint32_t v, int k )
{
    return ( v << k ) + ( v >> ( 32 - k ) );
}

// k must not be 0
BOOST_FORCEINLINE boost::uint64_t rotl( boost::uint64_t v, int k )
{
    return ( v << k ) + ( v >> ( 64 - k ) );
}

#endif

} // namespace detail
} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_DETAIL_ROT_HPP_INCLUDED
