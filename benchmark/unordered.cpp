// Copyright 2017-2019 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#define _CRT_SECURE_NO_WARNINGS

#include <boost/hash2/fnv1a.hpp>
#include <boost/hash2/siphash.hpp>
#include <boost/hash2/xxhash.hpp>
#include <boost/hash2/md5.hpp>
#include <boost/hash2/sha1.hpp>
#include <boost/hash2/sha2.hpp>
#include <boost/hash2/ripemd.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/get_integral_result.hpp>
#include <boost/unordered/unordered_flat_set.hpp>
#include <boost/core/type_name.hpp>
#include <cstdint>
#include <random>
#include <chrono>
#include <typeinfo>
#include <cstddef>
#include <cstdio>
#include <string>
#include <vector>

#define STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)

template<class T, class H> class hasher
{
private:

    H h_;

private:

    template<class = void> static void hash_append_impl( H& h, T const& v, std::false_type )
    {
        boost::hash2::hash_append( h, v );
    }

    template<class = void> static void hash_append_impl( H& h, T const& v, std::true_type )
    {
        boost::hash2::hash_append_range( h, v.data(), v.data() + v.size() );
    }

public:

    hasher(): h_()
    {
    }

    explicit hasher( std::uint64_t seed ): h_( seed )
    {
    }

    hasher( unsigned char const* seed, std::size_t n ): h_( seed, n )
    {
    }

    std::size_t operator()( T const& v ) const
    {
        H h( h_ );

        hash_append_impl( h, v, boost::container_hash::is_contiguous_range<T>() );

        return boost::hash2::get_integral_result<std::size_t>( h.result() );
    }
};

template<class V, class S> void test4( int N, V const& v, char const * hash, S s )
{
    typedef std::chrono::steady_clock clock_type;

    clock_type::time_point t1 = clock_type::now();

    for( int i = 0; i < N; ++i )
    {
        s.insert( v[ i * 16 ] );
    }

    clock_type::time_point t2 = clock_type::now();

    std::size_t q = 0;

    for( int i = 0; i < 16 * N; ++i )
    {
        q += s.count( v[ i ] );
    }

    clock_type::time_point t3 = clock_type::now();

    long long ms1 = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    long long ms2 = std::chrono::duration_cast<std::chrono::milliseconds>( t3 - t2 ).count();

    std::size_t n = s.bucket_count();

    std::printf( "%s: n=%zu, q=%zu, %lld + %lld ms\n", hash, n, q, ms1, ms2 );
}

template<class K, class H, class V> void test3( int N, V const& v, std::size_t seed )
{
    using hasher = ::hasher<K, H>;
    boost::unordered_flat_set<K, hasher> s( 0, hasher( seed ) );
    test4( N, v, boost::core::type_name<H>().c_str(), s );
}

template<class K, class H, class V> void test2( int N, V const& v )
{
    test3< K, H >( N, v, 0x9e3779b9 );
}

int main()
{
    int const N = 1048576;

    std::vector<std::string> v;

    {
        v.reserve( N * 16 );

        std::mt19937_64 rnd;

        for( int i = 0; i < 16 * N; ++i )
        {
            char buffer[ 64 ];

            unsigned long long k = rnd();

            if( k & 1 )
            {
                std::snprintf( buffer, sizeof( buffer ), "prefix_%llu_suffix", k );
            }
            else
            {
                std::snprintf( buffer, sizeof( buffer ), "{%u}", static_cast<unsigned>( k ) );
            }

            v.push_back( buffer );
        }
    }

    typedef std::string K;

    {
        boost::unordered_flat_set<K> s;
        test4( N, v, "default", s );
        std::puts( "" );
    }

    test2<K, boost::hash2::fnv1a_32>( N, v );
    test2<K, boost::hash2::fnv1a_64>( N, v );
    test2<K, boost::hash2::xxhash_32>( N, v );
    test2<K, boost::hash2::xxhash_64>( N, v );
    test2<K, boost::hash2::siphash_32>( N, v );
    test2<K, boost::hash2::siphash_64>( N, v );
    test2<K, boost::hash2::md5_128>( N, v );
    test2<K, boost::hash2::sha1_160>( N, v );
    test2<K, boost::hash2::sha2_256>( N, v );
    test2<K, boost::hash2::sha2_224>( N, v );
    test2<K, boost::hash2::sha2_512>( N, v );
    test2<K, boost::hash2::sha2_384>( N, v );
    test2<K, boost::hash2::sha2_512_224>( N, v );
    test2<K, boost::hash2::sha2_512_256>( N, v );
    test2<K, boost::hash2::ripemd_160>( N, v );

    std::puts( "" );
}
