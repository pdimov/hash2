// Copyright 2017, 2018, 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/get_integral_result.hpp>
#include <boost/core/lightweight_test.hpp>
#include <set>
#include <limits>
#include <cstdint>

using T = std::uint32_t;

void test_identity()
{
    using boost::hash2::get_integral_result;

    using R = T;

    for( unsigned i = 0; i < 65536; ++i )
    {
        R r = static_cast<R>( i * 0x9E3779B9u );
        T t = get_integral_result<T>( r );

        BOOST_TEST_EQ( t, r );
    }
}

template<class R> void test_permutation( R offset = 0, R scale = 1 )
{
    using boost::hash2::get_integral_result;

    std::set<T> dist;

    for( unsigned i = 0; i < 65536; ++i )
    {
        R r = static_cast<R>( i * 0x9E3779B9u * scale + offset );
        T t = get_integral_result<T>( r );

        dist.insert( t );
    }

    BOOST_TEST_EQ( dist.size(), 65536u );
}

template<class R> void test_roundtrip()
{
    using boost::hash2::get_integral_result;

    std::set<T> dist;

    for( unsigned i = 0; i < 65536; ++i )
    {
        T t1 = static_cast<T>( i * 0x9E3779B9u );
        R r = get_integral_result<R>( t1 );
        T t2 = get_integral_result<T>( r );

        dist.insert( t2 );
    }

    BOOST_TEST_EQ( dist.size(), 65536u );
}

int main()
{
    test_identity();

    test_permutation<std::uint32_t>();
    test_permutation<std::uint64_t>();

    test_roundtrip<std::uint32_t>();
    test_roundtrip<std::uint64_t>();

    return boost::report_errors();
}
