// Copyright 2017, 2018, 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/get_integral_result.hpp>
#include <boost/core/lightweight_test.hpp>
#include <set>
#include <limits>
#include <cstdint>

template<class T> void test_identity()
{
    using boost::hash2::get_integral_result;

    using R = T;

    for( unsigned i = 0; i <= std::numeric_limits<T>::max(); ++i )
    {
        R r = static_cast<R>( i );
        T t = get_integral_result<T>( r );

        BOOST_TEST_EQ( t, r );
    }
}

template<class T, class R> std::size_t test_permutation( int shift )
{
    using boost::hash2::get_integral_result;

    std::set<T> dist;

    for( unsigned i = 0; i <= std::numeric_limits<T>::max(); ++i )
    {
        R r = static_cast<R>( i << shift );
        T t = get_integral_result<T>( r );

        dist.insert( t );
    }

    return dist.size();
}

template<class T, class R> std::size_t test_roundtrip()
{
    using boost::hash2::get_integral_result;

    std::set<T> dist;

    for( unsigned i = 0; i <= std::numeric_limits<T>::max(); ++i )
    {
        T t1 = static_cast<T>( i );
        R r = get_integral_result<R>( t1 );
        T t2 = get_integral_result<T>( r );

        dist.insert( t2 );
    }

    return dist.size();
}

int main()
{
    // 1 -> 1

    test_identity<std::uint8_t>();

    // 1 -> 2

    BOOST_TEST_EQ( (test_permutation<std::uint8_t, std::uint16_t>( 0 )), 256u );
    BOOST_TEST_EQ( (test_permutation<std::uint8_t, std::uint16_t>( 8 )), 256u );

    BOOST_TEST_GE( (test_roundtrip<std::uint8_t, std::uint16_t>()), 255u ); // !

    // 1 -> 4

    BOOST_TEST_EQ( (test_permutation<std::uint8_t, std::uint32_t>(  0 )), 256u );
    BOOST_TEST_EQ( (test_permutation<std::uint8_t, std::uint32_t>(  8 )), 256u );
    BOOST_TEST_GE( (test_permutation<std::uint8_t, std::uint32_t>( 16 )), 255u ); // !
    BOOST_TEST_EQ( (test_permutation<std::uint8_t, std::uint32_t>( 24 )), 256u );

    BOOST_TEST_EQ( (test_roundtrip<std::uint8_t, std::uint32_t>()), 256u );

    // 1 -> 8

    BOOST_TEST_EQ( (test_permutation<std::uint8_t, std::uint64_t>(  0 )), 256u );
    BOOST_TEST_EQ( (test_permutation<std::uint8_t, std::uint64_t>(  8 )), 256u );
    BOOST_TEST_EQ( (test_permutation<std::uint8_t, std::uint64_t>( 16 )), 256u );
    BOOST_TEST_EQ( (test_permutation<std::uint8_t, std::uint64_t>( 24 )), 256u );
    BOOST_TEST_EQ( (test_permutation<std::uint8_t, std::uint64_t>( 32 )), 256u );
    BOOST_TEST_EQ( (test_permutation<std::uint8_t, std::uint64_t>( 40 )), 256u );
    BOOST_TEST_EQ( (test_permutation<std::uint8_t, std::uint64_t>( 48 )), 256u );
    BOOST_TEST_EQ( (test_permutation<std::uint8_t, std::uint64_t>( 56 )), 256u );

    BOOST_TEST_EQ( (test_roundtrip<std::uint8_t, std::uint64_t>()), 256u );

    // 2 -> 2

    test_identity<std::uint16_t>();

    // 2 -> 4

    BOOST_TEST_EQ( (test_permutation<std::uint16_t, std::uint32_t>(  0 )), 65536u );
    BOOST_TEST_EQ( (test_permutation<std::uint16_t, std::uint32_t>( 16 )), 65536u );

    BOOST_TEST_GE( (test_roundtrip<std::uint16_t, std::uint32_t>()), 65535u ); // !

    // 2 -> 8

    BOOST_TEST_EQ( (test_permutation<std::uint16_t, std::uint64_t>(  0 )), 65536u );
    BOOST_TEST_EQ( (test_permutation<std::uint16_t, std::uint64_t>( 16 )), 65536u );
    BOOST_TEST_EQ( (test_permutation<std::uint16_t, std::uint64_t>( 32 )), 65536u );
    BOOST_TEST_EQ( (test_permutation<std::uint16_t, std::uint64_t>( 48 )), 65536u );

    BOOST_TEST_EQ( (test_roundtrip<std::uint16_t, std::uint64_t>()), 65536u );

    //

    return boost::report_errors();
}
