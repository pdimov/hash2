
// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hash2/byte_type.hpp>
#include <boost/hash2/fnv1a.hpp>
#include <boost/hash2/siphash.hpp>
#include <boost/hash2/xxhash.hpp>
#include <boost/hash2/spooky2.hpp>
#include <boost/hash2/murmur3.hpp>
#include <boost/hash2/md5.hpp>
#include <boost/hash2/sha1.hpp>
#include <boost/core/lightweight_test.hpp>
#include <algorithm>
#include <cstddef>

using boost::hash2::byte_type;

template<class H> void test()
{
    char const * s = "xxxx";

    {
        H h;

        h.update( reinterpret_cast<byte_type const*>( s ), 4 );

        h.result();

        unsigned char const * p = reinterpret_cast<unsigned char const*>( &h );
        std::size_t n = sizeof( h );

        BOOST_TEST_EQ( std::search( p, p + n, s, s + 4 ) - p, n );
    }

    {
        H h;

        byte_type buffer[ 1024 ] = {};
        h.update( buffer, 1024 );

        h.update( reinterpret_cast<byte_type const*>( s ), 4 );

        h.result();

        unsigned char const * p = reinterpret_cast<unsigned char const*>( &h );
        std::size_t n = sizeof( h );

        BOOST_TEST_EQ( std::search( p, p + n, s, s + 4 ) - p, n );
    }
}

int main()
{
    test<boost::hash2::fnv1a_32>();
    test<boost::hash2::fnv1a_64>();
    test<boost::hash2::murmur3_32>();
    test<boost::hash2::murmur3_128>();
    test<boost::hash2::xxhash_32>();
    test<boost::hash2::xxhash_64>();
    test<boost::hash2::spooky2_128>();
    test<boost::hash2::siphash_32>();
    test<boost::hash2::siphash_64>();
    test<boost::hash2::md5_128>();
    test<boost::hash2::sha1_160>();
    test<boost::hash2::hmac_md5_128>();
    test<boost::hash2::hmac_sha1_160>();

    return boost::report_errors();
}
