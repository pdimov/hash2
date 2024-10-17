// Copyright 2017-2020 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/fnv1a.hpp>
#include <boost/hash2/siphash.hpp>
#include <boost/hash2/xxhash.hpp>
#include <boost/hash2/md5.hpp>
#include <boost/hash2/sha1.hpp>
#include <boost/hash2/sha2.hpp>
#include <boost/hash2/ripemd.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/get_integral_result.hpp>
#include <boost/core/type_name.hpp>
#include <chrono>
#include <vector>
#include <cstdio>
#include <typeinfo>

template<class H> void test_( unsigned char const * p, int N, int M )
{
    H h;

    typedef std::chrono::steady_clock clock_type;

    clock_type::time_point t1 = clock_type::now();

    for( int i = 0; i < M; ++i )
    {
        hash_append_range( h, p, p + N );
    }

    clock_type::time_point t2 = clock_type::now();

    long long ms = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();

    using boost::hash2::get_integral_result;
    unsigned r = get_integral_result<unsigned>( h.result() );

    std::printf( "%s (N=%d): %u: %lld ms, %.2f MB/s\n", boost::core::type_name<H>().c_str(), N, r, ms, 1000.0 * N * M / ms / 1048576 );
}

extern unsigned char data[];

void test( int N, int M )
{
    test_<boost::hash2::fnv1a_32>( data, N, M );
    test_<boost::hash2::fnv1a_64>( data, N, M );
    test_<boost::hash2::xxhash_32>( data, N, M );
    test_<boost::hash2::xxhash_64>( data, N, M );
    test_<boost::hash2::siphash_32>( data, N, M );
    test_<boost::hash2::siphash_64>( data, N, M );
    test_<boost::hash2::md5_128>( data, N, M );
    test_<boost::hash2::sha1_160>( data, N, M );
    test_<boost::hash2::sha2_256>( data, N, M );
    test_<boost::hash2::sha2_224>( data, N, M );
    test_<boost::hash2::sha2_512>( data, N, M );
    test_<boost::hash2::sha2_384>( data, N, M );
    test_<boost::hash2::sha2_512_224>( data, N, M );
    test_<boost::hash2::sha2_512_256>( data, N, M );
    test_<boost::hash2::ripemd_160>( data, N, M );

    puts( "--" );
}

unsigned const N1 = 65536;
unsigned const M1 = 65535;

unsigned const N2 = 15;
unsigned const M2 = M1 * N1 / N2;

unsigned const N3 = 4;
unsigned const M3 = M1 * N1 / N3;

int main()
{
    test( N1, M1 );
    test( N2, M2 );
    test( N3, M3 );
}

unsigned char data[ N1 ];
