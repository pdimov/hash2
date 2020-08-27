
// Copyright 2017-2020 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.

#define _CRT_SECURE_NO_WARNINGS

#include <boost/hash2/fnv1a.hpp>
#include <boost/hash2/siphash.hpp>
#include <boost/hash2/xxhash.hpp>
#include <boost/hash2/spooky2.hpp>
#include <boost/hash2/md5.hpp>
#include <boost/hash2/sha1.hpp>
#include <boost/hash2/murmur3.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/get_integral_result.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/chrono.hpp>
#include <boost/core/demangle.hpp>
#include <boost/config.hpp>
#include <typeinfo>
#include <cstddef>
#include <cstdio>
#include <string>
#include <vector>

class mul31_32
{
private:

    boost::uint32_t st_;

public:

    typedef boost::uint32_t result_type;
    typedef boost::uint32_t size_type;

    mul31_32(): st_( 0x811C9DC5ul )
    {
    }

    explicit mul31_32( boost::uint64_t seed ): st_( 0x811C9DC5ul ^ ( seed & 0xFFFFFFFFu ) ^ ( seed >> 32 ) )
    {
    }

    void update( boost::hash2::byte_type const * p, std::ptrdiff_t n )
    {
        boost::uint32_t h = st_;

        for( std::ptrdiff_t i = 0; i < n; ++i )
        {
            h = h * 31 + static_cast<boost::uint32_t>( p[i] );
        }

        st_ = h;
    }

    boost::uint32_t result()
    {
        boost::uint32_t r = st_;
        st_ = st_ * 31 + 0xFF;
        return r;
    }
};

class mul31_64
{
private:

    boost::uint64_t st_;

public:

    typedef boost::uint64_t result_type;
    typedef boost::uint64_t size_type;

    mul31_64(): st_( 0xCBF29CE484222325ull )
    {
    }

    explicit mul31_64( boost::uint64_t seed ): st_( 0xCBF29CE484222325ull ^ seed )
    {
    }

    void update( boost::hash2::byte_type const * p, std::ptrdiff_t n )
    {
        boost::uint64_t h = st_;

        for( std::ptrdiff_t i = 0; i < n; ++i )
        {
            h = h * 31 + static_cast<boost::uint64_t>( p[i] );
        }

        st_ = h;
    }

    boost::uint64_t result()
    {
        boost::uint64_t r = st_;
        st_ = st_ * 31 + 0xFF;
        return r;
    }
};

template<class H> class hasher
{
private:

    std::size_t seed_;

public:

    explicit hasher( std::size_t seed ): seed_( seed )
    {
    }

    template<class T> std::size_t operator()( T const& v ) const
    {
        H h( seed_ );

        using boost::hash2::hash_append;
        hash_append( h, v );

        using boost::hash2::get_integral_result;
        return get_integral_result<std::size_t>( h.result() );
    }
};

template<class H, class V> void test3( int N, V const& v, char const* seed_text, std::size_t seed )
{
    // test4( N, v, boost::core::demangle( typeid(H).name() ).c_str(), seed_text );

    typedef boost::chrono::steady_clock clock_type;

    clock_type::time_point t1 = clock_type::now();

    std::size_t q = 0;

    for( int i = 0; i < N; ++i )
    {
        q += hasher<H>( seed )( v[i] );
    }

    clock_type::time_point t2 = clock_type::now();

    long long ms1 = boost::chrono::duration_cast<boost::chrono::milliseconds>( t2 - t1 ).count();

    std::string hash = boost::core::demangle( typeid(H).name() );

#if defined( _MSC_VER )

    std::printf( "%s(%s): q=%Iu, %lld ms\n", hash.c_str(), seed_text, q, ms1 );

#else

    std::printf( "%s(%s): q=%zu, %lld ms\n", hash.c_str(), seed_text, q, ms1 );

#endif
}

template<class H, class V> void test2( int N, V const& v )
{
    test3<H>( N, v, "0", 0 );
    test3<H>( N, v, "1", 1 );
    test3<H>( N, v, "~0", ~static_cast<std::size_t>( 0 ) );
    std::puts( "" );
}

int main()
{
    int const N = 16 * 1048576;

    std::vector<std::string> v;

    {
        v.reserve( N );

        boost::mt19937 rnd;

        for( int i = 0; i < N; ++i )
        {
            unsigned k = rnd();

            char buffer[ 64 ];
            sprintf( buffer, "{%u}", k );

            v.push_back( buffer );
        }
    }

    test2<mul31_32>( N, v );
    test2<mul31_64>( N, v );
    test2<boost::hash2::fnv1a_32>( N, v );
    test2<boost::hash2::fnv1a_64>( N, v );
    test2<boost::hash2::murmur3_32>( N, v );
    test2<boost::hash2::murmur3_128>( N, v );
    test2<boost::hash2::xxhash_32>( N, v );
    test2<boost::hash2::xxhash_64>( N, v );
    test2<boost::hash2::spooky2_128>( N, v );
    test2<boost::hash2::siphash_32>( N, v );
    test2<boost::hash2::siphash_64>( N, v );

    std::puts( "" );
}
