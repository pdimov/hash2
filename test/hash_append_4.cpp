// Copyright 2017 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
//
// Endian-independent test

#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/fnv1a.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/array.hpp>
#include <utility>
#include <array>
#include <tuple>

template<class Hash, class Flavor, class R> void test( R r )
{
    Flavor f;

    {
        unsigned char v[2] = { 1, 2 };

        Hash h;

        h.update( v, 2 );

        BOOST_TEST_EQ( h.result(), r );
    }

    {
        unsigned char v[2] = { 1, 2 };

        Hash h;

        hash_append( h, f, v[0] );
        hash_append( h, f, v[1] );

        BOOST_TEST_EQ( h.result(), r );
    }

    {
        unsigned char v[2] = { 1, 2 };

        Hash h;

        hash_append( h, f, v );

        BOOST_TEST_EQ( h.result(), r );
    }

    {
        std::pair<unsigned char, unsigned char> v( static_cast<unsigned char>( 1 ), static_cast<unsigned char>( 2 ) );

        Hash h;

        hash_append( h, f, v );

        BOOST_TEST_EQ( h.result(), r );
    }

    {
        boost::array<unsigned char, 2> v = { { 1, 2 } };

        Hash h;

        hash_append( h, f, v );

        BOOST_TEST_EQ( h.result(), r );
    }

    {
        std::array<unsigned char, 2> v = { { 1, 2 } };

        Hash h;

        hash_append( h, f, v );

        BOOST_TEST_EQ( h.result(), r );
    }

    {
        std::tuple<unsigned char, unsigned char> v( static_cast<unsigned char>( 1 ), static_cast<unsigned char>( 2 ) );

        Hash h;

        hash_append( h, f, v );

        BOOST_TEST_EQ( h.result(), r );
    }
}

int main()
{
    using namespace boost::hash2;

    test<fnv1a_32, default_flavor>( 3983810698ul );
    test<fnv1a_64, default_flavor>( 589729691727335466ull );

    return boost::report_errors();
}
