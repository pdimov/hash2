// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/siphash.hpp>
#include <boost/hash2/detail/config.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/config.hpp>
#include <cstring>

#if defined(BOOST_MSVC) && BOOST_MSVC < 1920
# pragma warning(disable: 4307) // integral constant overflow
#endif

#define STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)

template<class H, std::size_t N> BOOST_CXX14_CONSTEXPR typename H::result_type test( unsigned char const (&seed)[ 16 ], unsigned char const (&v)[ N ] )
{
    H h( seed, 16 );

    h.update( v, N / 3 );
    h.update( v, N - N / 3 );

    return h.result();
}

int main()
{
    using namespace boost::hash2;

    constexpr unsigned char seed[ 16 ] = {};

    constexpr unsigned char v21[ 21 ] = {};
    constexpr unsigned char v45[ 45 ] = {};

    BOOST_TEST_EQ( test<siphash_32>( seed, v21 ), 3273912247 );
    BOOST_TEST_EQ( test<siphash_32>( seed, v45 ), 3389005632 );

    BOOST_TEST_EQ( test<siphash_64>( seed, v21 ), 17634937937087799533ull );
    BOOST_TEST_EQ( test<siphash_64>( seed, v45 ), 7083435387605517692 );

#if !defined(BOOST_NO_CXX14_CONSTEXPR)

    STATIC_ASSERT( test<siphash_32>( seed, v21 ) == 3273912247 );
    STATIC_ASSERT( test<siphash_32>( seed, v45 ) == 3389005632 );

    STATIC_ASSERT( test<siphash_64>( seed, v21 ) == 17634937937087799533ull );
    STATIC_ASSERT( test<siphash_64>( seed, v45 ) == 7083435387605517692 );

#endif

    return boost::report_errors();
}
