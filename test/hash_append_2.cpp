
// Copyright 2017 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
//
// Endian-dependent test

#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/fnv1a.hpp>
#include <boost/core/lightweight_test.hpp>

template<class H, class R> void test( R r )
{
    {
        int v[4] = { 1, 2, 3, 4 };

        H h;

        hash_append( h, v[0] );
        hash_append( h, v[1] );
        hash_append( h, v[2] );
        hash_append( h, v[3] );

        BOOST_TEST_EQ( h.result(), r );
    }

    {
        int v[4] = { 1, 2, 3, 4 };

        H h;

        hash_append( h, v );

        BOOST_TEST_EQ( h.result(), r );
    }

    {
        int v[2][2] = { { 1, 2 }, { 3, 4 } };

        H h;

        hash_append( h, v );

        BOOST_TEST_EQ( h.result(), r );
    }
}

int main()
{
    test<boost::hash2::fnv1a_32>( 1041505217ul );
    test<boost::hash2::fnv1a_64>( 9566659391000707361ull );

    return boost::report_errors();
}
