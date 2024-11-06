// Copyright 2024 Christian Mazakas
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/ripemd.hpp>
#include <boost/hash2/digest.hpp>
#include <boost/hash2/detail/config.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/config.hpp>

#if defined(BOOST_MSVC) && BOOST_MSVC < 1920
# pragma warning(disable: 4307) // integral constant overflow
#endif

#define STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)

#if defined(BOOST_NO_CXX14_CONSTEXPR)
# define TEST_EQ(x1, x2) BOOST_TEST_EQ(x1, x2)
#else
# define TEST_EQ(x1, x2) BOOST_TEST_EQ(x1, x2); STATIC_ASSERT(x1 == x2)
#endif

template<class H, std::size_t N> BOOST_CXX14_CONSTEXPR typename H::result_type test( std::uint64_t seed, boost::hash2::digest<N> const& dgst )
{
    H h( seed );

    h.update( dgst.data(), N / 3 );
    h.update( dgst.data() + N / 3, N - N / 3 );

    return h.result();
}

BOOST_CXX14_CONSTEXPR unsigned char to_byte( char c )
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0xff;
}

BOOST_CXX14_CONSTEXPR boost::hash2::digest<20> digest_from_hex( char const* str )
{
    boost::hash2::digest<20> dgst = {};
    auto* p = dgst.data();
    for( int i = 0; i < 20; ++i ) {
        auto c1 = to_byte( str[ 2 * i ] );
        auto c2 = to_byte( str[ 2 * i + 1 ] );
        p[ i ] = (c1 << 4) | c2;
    }
    return dgst;
}

template<std::size_t N> BOOST_CXX14_CONSTEXPR boost::hash2::digest<N - 1> digest_from_str( char const (&str)[N] )
{
    boost::hash2::digest<N - 1> dgst;
    for( std::size_t i = 0; i < N - 1; ++i ) {
        dgst.data()[ i ] = static_cast<unsigned char>( str[ i ] );
    }
    return dgst;
}

int main()
{
    using namespace boost::hash2;

    TEST_EQ( test<ripemd_160>( 0, digest_from_str( "a" ) ), digest_from_hex( "0bdc9d2d256b3ee9daae347be6f4dc835a467ffe" ) );
    TEST_EQ( test<ripemd_160>( 0, digest_from_str( "abc" ) ), digest_from_hex( "8eb208f7e05d987a9b044a8e98c6b087f15a0bfc" ) );
    TEST_EQ( test<ripemd_160>( 0, digest_from_str( "message digest" ) ), digest_from_hex( "5d0689ef49d2fae572b881b123a85ffa21595f36" ) );
    TEST_EQ( test<ripemd_160>( 0, digest_from_str( "abcdefghijklmnopqrstuvwxyz" ) ), digest_from_hex( "f71c27109c692c1b56bbdceb5b9d2865b3708dbc" ) );
    TEST_EQ( test<ripemd_160>( 0, digest_from_str( "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq" ) ), digest_from_hex( "12a053384a9c0c88e405a06c27dcf49ada62eb2b" ) );
    TEST_EQ( test<ripemd_160>( 0, digest_from_str( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" ) ), digest_from_hex( "b0e20b6e3116640286ed3a87a5713079b21f5189" ) );

    TEST_EQ( test<ripemd_160>( 7, digest_from_str( "a" ) ), digest_from_hex( "7580d81d5ed1467317ce3ab9199088764a518b9d" ) );
    TEST_EQ( test<ripemd_160>( 7, digest_from_str( "abc" ) ), digest_from_hex( "f5a724fe56b405db4edcb1ea46707ad78753a2e4" ) );
    TEST_EQ( test<ripemd_160>( 7, digest_from_str( "message digest" ) ), digest_from_hex( "da6a32ce82c07de14ac5c2be1be3775201f07941" ) );
    TEST_EQ( test<ripemd_160>( 7, digest_from_str( "abcdefghijklmnopqrstuvwxyz" ) ), digest_from_hex( "221e3b3af0671febf816397ded6f05c8a5c540d3" ) );
    TEST_EQ( test<ripemd_160>( 7, digest_from_str( "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq" ) ), digest_from_hex( "4578ce76d4ddec5a75288306da83f65eb963973c" ) );
    TEST_EQ( test<ripemd_160>( 7, digest_from_str( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" ) ), digest_from_hex( "868f80dffc5c55612afdd5f8c7c4948410134dc5" ) );

    return boost::report_errors();
}
