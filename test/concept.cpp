// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

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
#include <array>
#include <type_traits>
#include <limits>
#include <cstddef>
#include <cstdint>

using boost::hash2::byte_type;

template<class R> struct is_valid_result:
    std::integral_constant<bool,
        std::is_integral<R>::value && !std::is_signed<R>::value
    >
{
};

template<std::size_t N> struct is_valid_result< std::array<byte_type, N> >:
    std::integral_constant<bool, N >= 8>
{
};

template<class H> void test_result_type()
{
    BOOST_TEST_TRAIT_TRUE((is_valid_result<typename H::result_type>));
}

template<class H> void test_size_type()
{
    BOOST_TEST_TRAIT_TRUE((std::is_integral<typename H::size_type>));

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

template<class H> void test_byte_seed_constructible()
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

template<class H> void test_integral_seed_constructible()
{
    typename H::result_type r0, r1, r2, r3;

    {
        H h( 0 );
        r0 = h.result();
    }

    {
        H h( 1 );
        r1 = h.result();
    }

    BOOST_TEST( r0 != r1 );

    {
        H h( 2 );
        r2 = h.result();
    }

    BOOST_TEST( r0 != r2 );
    BOOST_TEST( r1 != r2 );

    {
        H h( 1LL << 32 );
        r3 = h.result();
    }

    BOOST_TEST( r0 != r3 );
    BOOST_TEST( r1 != r3 );
    BOOST_TEST( r2 != r3 );

    {
        H h;
        BOOST_TEST( h.result() == r0 );
    }

    {
        H h( 0L );
        BOOST_TEST( h.result() == r0 );
    }

    {
        H h( 1L );
        BOOST_TEST( h.result() == r1 );
    }

    {
        H h( 2L );
        BOOST_TEST( h.result() == r2 );
    }

    {
        H h( 0LL );
        BOOST_TEST( h.result() == r0 );
    }

    {
        H h( 1LL );
        BOOST_TEST( h.result() == r1 );
    }

    {
        H h( 2LL );
        BOOST_TEST( h.result() == r2 );
    }

    {
        H h( static_cast<std::uint32_t>( 0 ) );
        BOOST_TEST( h.result() == r0 );
    }

    {
        H h( static_cast<std::uint32_t>( 1 ) );
        BOOST_TEST( h.result() == r1 );
    }

    {
        H h( static_cast<std::uint32_t>( 2 ) );
        BOOST_TEST( h.result() == r2 );
    }

    {
        H h( static_cast<std::size_t>( 0 ) );
        BOOST_TEST( h.result() == r0 );
    }

    {
        H h( static_cast<std::size_t>( 1 ) );
        BOOST_TEST( h.result() == r1 );
    }

    {
        H h( static_cast<std::size_t>( 2 ) );
        BOOST_TEST( h.result() == r2 );
    }

    {
        H h( static_cast<std::uint64_t>( 0 ) );
        BOOST_TEST( h.result() == r0 );
    }

    {
        H h( static_cast<std::uint64_t>( 1 ) );
        BOOST_TEST( h.result() == r1 );
    }

    {
        H h( static_cast<std::uint64_t>( 2 ) );
        BOOST_TEST( h.result() == r2 );
    }

    {
        H h( static_cast<std::uint64_t>( 1 ) << 32 );
        BOOST_TEST( h.result() == r3 );
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

template<class H> void test_assignable()
{
    byte_type const seed[ 3 ] = { 0x01, 0x02, 0x03 };

    {
        H h1;
        H h2( h1 );

        typename H::result_type r1 = h1.result();

        BOOST_TEST( h1.result() != r1 );

        h1 = h2;
        BOOST_TEST( h1.result() == r1 );
    }

    {
        H h1( seed, 3 );
        H h2( h1 );

        typename H::result_type r1 = h1.result();

        BOOST_TEST( h1.result() != r1 );

        h1 = h2;
        BOOST_TEST( h1.result() == r1 );
    }

    {
        H h1;
        h1.update( seed, 3 );

        H h2( h1 );

        typename H::result_type r1 = h1.result();

        BOOST_TEST( h1.result() != r1 );

        h1 = h2;
        BOOST_TEST( h1.result() == r1 );
    }
}

template<class H> void test()
{
    test_result_type<H>();
    test_size_type<H>();
    test_default_constructible<H>();
    test_byte_seed_constructible<H>();
    test_integral_seed_constructible<H>();
    test_copy_constructible<H>();
    test_update<H>();
    test_assignable<H>();
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
