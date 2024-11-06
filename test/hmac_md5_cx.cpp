// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/md5.hpp>
#include <boost/hash2/detail/config.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/config.hpp>
#include <array>

#if defined(BOOST_MSVC) && BOOST_MSVC < 1920
# pragma warning(disable: 4307) // integral constant overflow
#endif

#define STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)

template<class H, std::size_t N> BOOST_CXX14_CONSTEXPR typename H::result_type test( std::uint64_t seed, unsigned char const (&v)[ N ] )
{
    H h( seed );

    h.update( v, N / 3 );
    h.update( v, N - N / 3 );

    return h.result();
}

int main()
{
    using namespace boost::hash2;

    constexpr unsigned char v[ 95 ] = {};

    BOOST_CXX14_CONSTEXPR digest<16> r1 = {{ 0x6F, 0x37, 0xD7, 0x4E, 0xD7, 0x97, 0xC2, 0x55, 0xEA, 0x1E, 0x4A, 0x46, 0xC3, 0x52, 0x39, 0xD7 }};
    BOOST_CXX14_CONSTEXPR digest<16> r2 = {{ 0x68, 0xD4, 0xDF, 0x36, 0x73, 0x64, 0x42, 0xC0, 0xED, 0xE2, 0xA6, 0x15, 0x85, 0x10, 0xA4, 0x5E }};

    BOOST_TEST_EQ( test<hmac_md5_128>( 0, v ), r1 );
    BOOST_TEST_EQ( test<hmac_md5_128>( 7, v ), r2 );

#if !defined(BOOST_NO_CXX14_CONSTEXPR) && !( defined(BOOST_GCC) && BOOST_GCC < 60000 )

    STATIC_ASSERT( test<hmac_md5_128>( 0, v ) == r1 );
    STATIC_ASSERT( test<hmac_md5_128>( 7, v ) == r2 );

#endif

    return boost::report_errors();
}
