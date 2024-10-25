#ifndef BOOST_HASH2_DETAIL_REVERSE_HPP_INCLUDED
#define BOOST_HASH2_DETAIL_REVERSE_HPP_INCLUDED

// Copyright 2024 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <cstdint>
#include <cstring>
#include <cstdlib>

namespace boost
{
namespace hash2
{
namespace detail
{

template<std::size_t N> void reverse( unsigned char (&d)[ N ], void const* s )
{
    unsigned char const* s2 = static_cast<unsigned char const*>( s );

    for( std::size_t i = 0; i < N; ++i )
    {
        d[ i ] = s2[ N-1-i ];
    }
}

#if defined(__GNUC__) || defined(__clang__)

inline void reverse( unsigned char (&d)[ 2 ], void const* s )
{
    std::uint16_t tmp;
    std::memcpy( &tmp, s, 2 );
    tmp = __builtin_bswap16( tmp );
    std::memcpy( d, &tmp, 2 );
}

inline void reverse( unsigned char (&d)[ 4 ], void const* s )
{
    std::uint32_t tmp;
    std::memcpy( &tmp, s, 4 );
    tmp = __builtin_bswap32( tmp );
    std::memcpy( d, &tmp, 4 );
}

inline void reverse( unsigned char (&d)[ 8 ], void const* s )
{
    std::uint64_t tmp;
    std::memcpy( &tmp, s, 8 );
    tmp = __builtin_bswap64( tmp );
    std::memcpy( d, &tmp, 8 );
}

inline void reverse( unsigned char (&d)[ 16 ], void const* s )
{
    std::uint64_t tmp[ 2 ];
    std::memcpy( tmp, s, 16 );

    std::uint64_t tmp2[ 2 ] = { __builtin_bswap64( tmp[1] ), __builtin_bswap64( tmp[0] ) };
    std::memcpy( d, tmp2, 16 );
}

#elif defined(_MSC_VER)

inline void reverse( unsigned char (&d)[ 2 ], void const* s )
{
    unsigned short tmp;
    std::memcpy( &tmp, s, 2 );
    tmp = _byteswap_ushort( tmp );
    std::memcpy( d, &tmp, 2 );
}

inline void reverse( unsigned char (&d)[ 4 ], void const* s )
{
    unsigned long tmp;
    std::memcpy( &tmp, s, 4 );
    tmp = _byteswap_ulong( tmp );
    std::memcpy( d, &tmp, 4 );
}

inline void reverse( unsigned char (&d)[ 8 ], void const* s )
{
    unsigned long long tmp;
    std::memcpy( &tmp, s, 8 );
    tmp = _byteswap_uint64( tmp );
    std::memcpy( d, &tmp, 8 );
}

inline void reverse( unsigned char (&d)[ 16 ], void const* s )
{
    unsigned long long tmp[ 2 ];
    std::memcpy( tmp, s, 16 );

    unsigned long long tmp2[ 2 ] = { _byteswap_uint64( tmp[1] ), _byteswap_uint64( tmp[0] ) };
    std::memcpy( d, tmp2, 16 );
}

#endif

} // namespace detail
} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_DETAIL_REVERSE_HPP_INCLUDED
