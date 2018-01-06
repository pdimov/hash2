
// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hash2/fnv1a.hpp>
#include <boost/hash2/siphash.hpp>
#include <boost/hash2/xxhash.hpp>
#include <boost/hash2/spooky2.hpp>
#include <boost/hash2/md5.hpp>
#include <boost/hash2/sha1.hpp>
#include <boost/hash2/murmur3.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/get_integral_result.hpp>
#include <boost/chrono.hpp>
#include <boost/core/demangle.hpp>
#include <typeinfo>
#include <cstddef>
#include <cstdio>
#include <string>
#include <limits>
#include <cmath>

template<class R, class H> void test_( int N )
{
    typedef boost::chrono::steady_clock clock_type;

    clock_type::time_point t1 = clock_type::now();

    double r = 0;

    H h;

    for( int i = 0; i < N; ++i )
    {
        using boost::hash2::get_integral_result;
        r += get_integral_result<R>( h.result() );
        r += 0.5;
    }

    clock_type::time_point t2 = clock_type::now();

    long long ms = boost::chrono::duration_cast<boost::chrono::milliseconds>( t2 - t1 ).count();

    r /= N;

    // Standard deviation of Bates distribution
    double stddev = static_cast<double>( std::numeric_limits<R>::max() ) - static_cast<double>( std::numeric_limits<R>::min() ) / std::sqrt( 12.0 * N );

    r /= stddev;

    printf( "%s: r=%e, %lld ms\n", boost::core::demangle( typeid(H).name() ).c_str(), r, ms );
}

template<class R> void test( int N )
{
    printf( "Integral result type `%s`:\n\n", boost::core::demangle( typeid(R).name() ).c_str() );

    test_<R, boost::hash2::fnv1a_32>( N );
    test_<R, boost::hash2::fnv1a_64>( N );
    test_<R, boost::hash2::murmur3_32>( N );
    test_<R, boost::hash2::murmur3_128>( N );
    test_<R, boost::hash2::xxhash_32>( N );
    test_<R, boost::hash2::xxhash_64>( N );
    test_<R, boost::hash2::spooky2_128>( N );
    test_<R, boost::hash2::siphash_32>( N );
    test_<R, boost::hash2::siphash_64>( N );
    test_<R, boost::hash2::md5_128>( N );
    test_<R, boost::hash2::sha1_160>( N );
    test_<R, boost::hash2::hmac_md5_128>( N );
    test_<R, boost::hash2::hmac_sha1_160>( N );

    puts( "" );
}

int main()
{
    int const N = 64 * 1024 * 1024;

    test<signed char>( N );
    test<short>( N );
    test<int>( N );
    test<long long>( N );
}
