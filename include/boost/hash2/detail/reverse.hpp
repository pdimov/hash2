#ifndef BOOST_HASH2_DETAIL_REVERSE_HPP_INCLUDED
#define BOOST_HASH2_DETAIL_REVERSE_HPP_INCLUDED

// Copyright 2024 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <cstdint>
#include <cstring>

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

} // namespace detail
} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_DETAIL_REVERSE_HPP_INCLUDED
