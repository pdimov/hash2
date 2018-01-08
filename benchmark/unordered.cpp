
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
#include <boost/config.hpp>
#include <typeinfo>
#include <cstddef>
#include <cstdio>
#include <string>

#if defined(BOOST_NO_CXX11_HDR_UNORDERED_SET)

#include <boost/unordered_set.hpp>
using boost::unordered_set;

#else

#include <unordered_set>
using std::unordered_set;

#endif

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

template<class K, class H> void test3( int N, char const* seed_text, std::size_t seed )
{
    unordered_set< K, hasher<H> > s( 0, hasher<H>( seed ) );

    typedef boost::chrono::steady_clock clock_type;

    clock_type::time_point t1 = clock_type::now();

    for( int i = 0; i < N; ++i )
    {
        s.insert( i );
    }

    std::size_t q = 0;

    for( int j = 0; j < 16; ++j )
    {
        for( int i = N * 15 / 16; i < 4 * N; ++i )
        {
            q += s.count( i );
        }
    }

    clock_type::time_point t2 = clock_type::now();

    long long ms = boost::chrono::duration_cast<boost::chrono::milliseconds>( t2 - t1 ).count();

    std::size_t n = s.bucket_count();
    std::size_t m = 0;

    for( std::size_t i = 0; i < n; ++i )
    {
        std::size_t k = s.bucket_size( i );

        if( k > m )
        {
            m = k;
        }
    }

#if defined( _MSC_VER )

    std::printf( "%s(%s): n=%Iu, m=%Iu, q=%Iu, %lld ms\n", boost::core::demangle( typeid(H).name() ).c_str(), seed_text, n, m, q, ms );

#else

    std::printf( "%s(%s): n=%zu, m=%zu, q=%zu, %lld ms\n", boost::core::demangle( typeid(H).name() ).c_str(), seed_text, n, m, q, ms );

#endif
}

template<class K, class H> void test2( int N )
{
    test3<K, H>( N, "0", 0 );
    test3<K, H>( N, "1", 1 );
    test3<K, H>( N, "~0", ~static_cast<std::size_t>( 0 ) );
    std::puts( "" );
}

template<class K> void test( int N )
{
    std::printf( "Key type `%s`:\n\n", boost::core::demangle( typeid(K).name() ).c_str() );

    test2<K, boost::hash2::fnv1a_32>( N );
    test2<K, boost::hash2::fnv1a_64>( N );
    test2<K, boost::hash2::murmur3_32>( N );
    test2<K, boost::hash2::murmur3_128>( N );
    test2<K, boost::hash2::xxhash_32>( N );
    test2<K, boost::hash2::xxhash_64>( N );
    test2<K, boost::hash2::spooky2_128>( N );
    test2<K, boost::hash2::siphash_32>( N );
    test2<K, boost::hash2::siphash_64>( N );
    test2<K, boost::hash2::md5_128>( N );
    test2<K, boost::hash2::sha1_160>( N );
    test2<K, boost::hash2::hmac_md5_128>( N );
    test2<K, boost::hash2::hmac_sha1_160>( N );

    std::puts( "" );
}

class X
{
private:

    int key_;
    std::string value_;

public:

    X( int k ): key_( k ), value_( "0123456789ABCDE" )
    {
    }

    int key() const
    {
        return key_;
    }

    std::string value() const
    {
        return value_;
    }
};

inline bool operator==( X const & x1, X const & x2 )
{
    return x1.key() == x2.key() && x1.value() == x2.value();
}

template<class H> inline void hash_append( H & h, X const & x )
{
    using boost::hash2::hash_append;

    hash_append( h, x.key() );
    hash_append( h, x.value() );
}

int main()
{
#if defined(BOOST_NO_CXX11_HDR_UNORDERED_SET)

    std::puts( "Using boost::unordered_set for lack of <unordered_set>:\n" );

#else

    std::puts( "Using std::unordered_set:\n" );

#endif

    int const N = 1048576;

    test<int>( N );
    test<long long>( N );
    test<X>( N );
}
