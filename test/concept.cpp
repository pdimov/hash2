
// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hash2/fnv1a.hpp>
#include <boost/hash2/siphash.hpp>
#include <boost/hash2/xxhash.hpp>
#include <boost/hash2/spooky2.hpp>
#include <boost/hash2/murmur3.hpp>
#include <boost/hash2/md5.hpp>
#include <boost/hash2/sha1.hpp>
#include <boost/hash2/byte_type.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/core/lightweight_test_trait.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/array.hpp>
#include <limits>
#include <cstddef>

using boost::hash2::byte_type;

template<class R> struct is_valid_result:
    boost::integral_constant<bool,
        boost::is_integral<R>::value && !boost::is_signed<R>::value
    >
{
};

template<std::size_t N> struct is_valid_result< boost::array<byte_type, N> >:
    boost::integral_constant<bool, N >= 8>
{
};

template<class H> void test_result_type()
{
    BOOST_TEST_TRAIT_TRUE((is_valid_result<typename H::result_type>));
}

template<class H> void test_size_type()
{
    BOOST_TEST_TRAIT_TRUE((boost::is_integral<typename H::size_type>));

    BOOST_TEST_EQ( std::numeric_limits<typename H::size_type>::min(), 0 );
    BOOST_TEST_GE( std::numeric_limits<typename H::size_type>::max(), 0xFFFFFFFFu );
}

template<class H> void test_default_constructible()
{
    H h1;
    typename H::result_type r1 = h1.result();

    H h2;
    typename H::result_type r2 = h2.result();

    BOOST_TEST( r1 == r2 );
}

template<class H> void test_seed_constructible()
{
    byte_type const seed[ 3 ] = { 0x01, 0x02, 0x03 };

    {
        H h1;
        H h2( static_cast<byte_type const*>( 0 ), 0 );

        BOOST_TEST( h1.result() == h2.result() );
    }

    {
        H h1;
        H h2( seed, 0 );

        BOOST_TEST( h1.result() == h2.result() );
    }

    {
        H h1;
        H h2( seed, 3 );

        BOOST_TEST( h1.result() != h2.result() );
    }

    {
        H h1( seed, 3 );
        H h2( seed, 3 );

        BOOST_TEST( h1.result() == h2.result() );
    }

    {
        H h1( seed, 2 );
        H h2( seed, 3 );

        BOOST_TEST( h1.result() != h2.result() );
    }
}

template<class H> void test_copy_constructible()
{
    byte_type const seed[ 3 ] = { 0x01, 0x02, 0x03 };

    {
        H h1;
        H h2( h1 );

        BOOST_TEST( h1.result() == h2.result() );
    }

    {
        H h1( seed, 3 );
        H h2( h1 );

        BOOST_TEST( h1.result() == h2.result() );
    }

    {
        H h1;
        h1.update( seed, 3 );

        H h2( h1 );

        BOOST_TEST( h1.result() == h2.result() );
    }
}

template<class H> void test_update()
{
    byte_type const data[ 3 ] = {};

    {
        H h1;

        H h2;
        h2.update( data, 3 );

        BOOST_TEST( h1.result() != h2.result() );
    }

    {
        H h1;
        h1.update( data, 3 );

        H h2;
        h2.update( data, 3 );

        BOOST_TEST( h1.result() == h2.result() );
    }

    {
        H h1;
        h1.update( data, 3 );

        H h2;
        h2.update( data, 2 );

        BOOST_TEST( h1.result() != h2.result() );
    }

    {
        H h1;
        h1.update( data, 3 );

        H h2( h1 );

        h1.update( data, 3 );
        h2.update( data, 3 );

        BOOST_TEST( h1.result() == h2.result() );
    }
}

template<class H> void test()
{
    test_result_type<H>();
    test_size_type<H>();
    test_default_constructible<H>();
    test_seed_constructible<H>();
    test_copy_constructible<H>();
    test_update<H>();
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
