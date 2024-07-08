// Copyright 2017, 2018 Peter Dimov.
// Copyright 2024 Christian Mazakas.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#define _CRT_SECURE_NO_WARNINGS

#include <boost/hash2/ripemd.hpp>
#include <boost/core/lightweight_test.hpp>
#include <cstdint>
#include <string>
#include <cstddef>
#include <cstdio>

template<std::size_t N> std::string to_string( std::array<unsigned char, N> const & v )
{
    std::string r;

    for( std::size_t i = 0; i < N; ++i )
    {
        char buffer[ 8 ];

        std::snprintf( buffer, sizeof( buffer ), "%02x", static_cast<int>( v[ i ] ) );

        r += buffer;
    }

    return r;
}

template<class H> std::string digest( std::string const & s )
{
    H h;

    h.update( s.data(), s.size() );

    return to_string( h.result() );
}

using boost::hash2::ripemd_160;

int main()
{
    // Test vectors from https://en.wikipedia.org/wiki/RIPEMD

    BOOST_TEST_EQ( digest<ripemd_160>( "" ), std::string( "9c1185a5c5e9fc54612808977ee8f548b2258d31" ) );
    BOOST_TEST_EQ( digest<ripemd_160>( "The quick brown fox jumps over the lazy dog" ), std::string( "37f332f68db77bd9d7edd4969571ad671cf9dd3b" ) );
    BOOST_TEST_EQ( digest<ripemd_160>( "The quick brown fox jumps over the lazy cog" ), std::string( "132072df690933835eb8b6ad0b77e7b6f14acad7" ) );

    // Test vectors from https://homes.esat.kuleuven.be/~bosselae/ripemd160/pdf/AB-9601/AB-9601.pdf

    std::string repeating_digits;
    for( int i = 0; i < 8; ++i )
    {
        repeating_digits += "1234567890";
    }

    BOOST_TEST_EQ( digest<ripemd_160>( "a" ), std::string( "0bdc9d2d256b3ee9daae347be6f4dc835a467ffe" ) );
    BOOST_TEST_EQ( digest<ripemd_160>( "abc" ), std::string( "8eb208f7e05d987a9b044a8e98c6b087f15a0bfc" ) );
    BOOST_TEST_EQ( digest<ripemd_160>( "message digest" ), std::string( "5d0689ef49d2fae572b881b123a85ffa21595f36" ) );
    BOOST_TEST_EQ( digest<ripemd_160>( "abcdefghijklmnopqrstuvwxyz" ), std::string( "f71c27109c692c1b56bbdceb5b9d2865b3708dbc" ) );
    BOOST_TEST_EQ( digest<ripemd_160>( "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq" ), std::string( "12a053384a9c0c88e405a06c27dcf49ada62eb2b" ) );
    BOOST_TEST_EQ( digest<ripemd_160>( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" ), std::string( "b0e20b6e3116640286ed3a87a5713079b21f5189" ) );
    BOOST_TEST_EQ( digest<ripemd_160>( repeating_digits ), std::string( "9b752e45573d4b39f4dbd3323cab82bf63326bfb" ) );
    BOOST_TEST_EQ( digest<ripemd_160>( std::string( 1000000, 'a' ) ), std::string( "52783243c1697bdbe16d37f97f68f08325dc1528" ) );

    return boost::report_errors();
}
