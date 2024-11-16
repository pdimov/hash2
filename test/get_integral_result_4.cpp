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

template<class T, class R> void test_permutation( R offset = 0, R scale = 1 )
{
    using boost::hash2::get_integral_result;

    std::set<T> dist;

    for( unsigned i = 0; i <= std::numeric_limits<T>::max(); ++i )
    {
        R r = static_cast<R>( i * scale + offset );
        T t = get_integral_result<T>( r );

        dist.insert( t );
    }

    BOOST_TEST_EQ( dist.size(), std::numeric_limits<T>::max() + 1u );
}

template<class T, class R> void test_roundtrip()
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

    BOOST_TEST_EQ( dist.size(), std::numeric_limits<T>::max() + 1u );
}

int main()
{
    test_identity<std::uint8_t>();

    test_permutation<std::uint8_t, std::uint8_t>();
    test_permutation<std::uint8_t, std::uint16_t>();
    test_permutation<std::uint8_t, std::uint32_t>();
    test_permutation<std::uint8_t, std::uint64_t>();

    test_roundtrip<std::uint8_t, std::uint8_t>();
    test_roundtrip<std::uint8_t, std::uint16_t>();
    test_roundtrip<std::uint8_t, std::uint32_t>();
    test_roundtrip<std::uint8_t, std::uint64_t>();

    test_identity<std::uint16_t>();

    test_permutation<std::uint16_t, std::uint16_t>();
    test_permutation<std::uint16_t, std::uint32_t>();
    test_permutation<std::uint16_t, std::uint64_t>();

    test_roundtrip<std::uint16_t, std::uint16_t>();
    test_roundtrip<std::uint16_t, std::uint32_t>();
    test_roundtrip<std::uint16_t, std::uint64_t>();

    return boost::report_errors();
}
