// Copyright 2017-2019 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

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
#include <boost/unordered/unordered_flat_set.hpp>
#include <boost/core/type_name.hpp>
#include <boost/cstdint.hpp>
#include <random>
#include <chrono>
#include <typeinfo>
#include <cstddef>
#include <cstdio>
#include <string>
#include <vector>

#define STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)

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

template<class H> class hasher1
{
private:

    std::size_t seed_;

public:

    explicit hasher1( std::size_t seed ): seed_( seed )
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

template<class H> class hasher2
{
private:

    H h_;

public:

    explicit hasher2( std::size_t seed ): h_( seed )
    {
    }

    template<class T> std::size_t operator()( T const& v ) const
    {
        H h( h_ );

        using boost::hash2::hash_append;
        hash_append( h, v );

        using boost::hash2::get_integral_result;
        return get_integral_result<std::size_t>( h.result() );
    }
};

template<class H> class hasher3
{
private:

    std::size_t seed_;

public:

    explicit hasher3( std::size_t seed ): seed_( seed )
    {
    }

    template<class T> std::size_t operator()( T const& v ) const
    {
        H h( seed_ );

        STATIC_ASSERT( boost::hash2::is_contiguous_range<T>::value );
        STATIC_ASSERT( boost::hash2::is_contiguously_hashable<typename T::value_type, H>::value );

        typename T::value_type const * p = v.data();
        typename T::size_type n = v.size();

        h.update( reinterpret_cast<boost::hash2::byte_type const*>( p ), n );

        using boost::hash2::get_integral_result;
        return get_integral_result<std::size_t>( h.result() );
    }
};

template<class H> class hasher4
{
private:

    H h_;

public:

    explicit hasher4( std::size_t seed ): h_( seed )
    {
    }

    template<class T> std::size_t operator()( T const& v ) const
    {
        H h( h_ );

        STATIC_ASSERT( boost::hash2::is_contiguous_range<T>::value );
        STATIC_ASSERT( boost::hash2::is_contiguously_hashable<typename T::value_type, H>::value );

        typename T::value_type const * p = v.data();
        typename T::size_type n = v.size();

        h.update( reinterpret_cast<boost::hash2::byte_type const*>( p ), n );

        using boost::hash2::get_integral_result;
        return get_integral_result<std::size_t>( h.result() );
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

#if defined( _MSC_VER )

    std::printf( "%s: n=%Iu, q=%Iu, %lld + %lld ms\n", hash, n, q, ms1, ms2 );

#else

    std::printf( "%s: n=%zu, q=%zu, %lld + %lld ms\n", hash, n, q, ms1, ms2 );

#endif
}

template<class K, class H, class V> void test3( int N, V const& v, std::size_t seed )
{
    boost::unordered_flat_set<K, H> s( 0, H( seed ) );
    test4( N, v, boost::core::type_name<H>().c_str(), s );
}

template<class K, class H, class V> void test2( int N, V const& v )
{
    test3< K, hasher1<H> >( N, v, 0x9e3779b9 );
    test3< K, hasher2<H> >( N, v, 0x9e3779b9 );
    test3< K, hasher3<H> >( N, v, 0x9e3779b9 );
    test3< K, hasher4<H> >( N, v, 0x9e3779b9 );
    std::puts( "" );
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
                sprintf( buffer, "prefix_%llu_suffix", k );
            }
            else
            {
                sprintf( buffer, "{%u}", static_cast<unsigned>( k ) );
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

    test2<K, mul31_32>( N, v );
    test2<K, mul31_64>( N, v );
    test2<K, boost::hash2::fnv1a_32>( N, v );
    test2<K, boost::hash2::fnv1a_64>( N, v );
    test2<K, boost::hash2::murmur3_32>( N, v );
    test2<K, boost::hash2::murmur3_128>( N, v );
    test2<K, boost::hash2::xxhash_32>( N, v );
    test2<K, boost::hash2::xxhash_64>( N, v );
    test2<K, boost::hash2::spooky2_128>( N, v );
    test2<K, boost::hash2::siphash_32>( N, v );
    test2<K, boost::hash2::siphash_64>( N, v );
    test2<K, boost::hash2::md5_128>( N, v );
    test2<K, boost::hash2::sha1_160>( N, v );

    std::puts( "" );
}
