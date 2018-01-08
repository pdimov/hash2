
// Copyright 2017 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
//
// Endian-independent test

#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/fnv1a.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/array.hpp>
#include <utility>
#if !defined(BOOST_NO_CXX11_HDR_ARRAY)
# include <array>
#endif
#if !defined(BOOST_NO_CXX11_HDR_TUPLE)
# include <tuple>
#endif

template<class H, class R> void test( R r )
{
    {
        unsigned char v[2] = { 1, 2 };

        H h;

        h.update( v, 2 );

        BOOST_TEST_EQ( h.result(), r );
    }

    {
        unsigned char v[2] = { 1, 2 };

        H h;

        hash_append( h, v[0] );
        hash_append( h, v[1] );

        BOOST_TEST_EQ( h.result(), r );
    }

    {
        unsigned char v[2] = { 1, 2 };

        H h;

        hash_append( h, v );

        BOOST_TEST_EQ( h.result(), r );
    }

    {
        std::pair<unsigned char, unsigned char> v( 1, 2 );

        H h;

        hash_append( h, v );

        BOOST_TEST_EQ( h.result(), r );
    }

    {
        boost::array<unsigned char, 2> v = { { 1, 2 } };

        H h;

        hash_append( h, v );

        BOOST_TEST_EQ( h.result(), r );
    }

#if !defined(BOOST_NO_CXX11_HDR_ARRAY)

    {
        std::array<unsigned char, 2> v = { { 1, 2 } };

        H h;

        hash_append( h, v );

        BOOST_TEST_EQ( h.result(), r );
    }

#endif

#if !defined(BOOST_NO_CXX11_HDR_TUPLE)

    {
        std::tuple<unsigned char, unsigned char> v( 1, 2 );

        H h;

        hash_append( h, v );

        BOOST_TEST_EQ( h.result(), r );
    }

#endif
}

int main()
{
    test<boost::hash2::fnv1a_32>( 3983810698ul );
    test<boost::hash2::fnv1a_64>( 589729691727335466ull );

    return boost::report_errors();
}
