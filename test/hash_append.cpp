
// Copyright 2017 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
//
// Endian-independent test

#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/fnv1a.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/array.hpp>
#if !defined(BOOST_NO_CXX11_HDR_ARRAY)
# include <array>
#endif

template<class H, class R> void test( R r )
{
    {
        unsigned char v[4] = { 1, 2, 3, 4 };

        H h;

        hash_append( h, v[0] );
        hash_append( h, v[1] );
        hash_append( h, v[2] );
        hash_append( h, v[3] );

        BOOST_TEST_EQ( h.result(), r );
    }

    {
        unsigned char v[4] = { 1, 2, 3, 4 };

        H h;

        hash_append( h, v );

        BOOST_TEST_EQ( h.result(), r );
    }

    {
        unsigned char v[2][2] = { { 1, 2 }, { 3, 4 } };

        H h;

        hash_append( h, v );

        BOOST_TEST_EQ( h.result(), r );
    }

    {
        boost::array<unsigned char, 4> v = {{ 1, 2, 3, 4 }};

        H h;

        hash_append( h, v );

        BOOST_TEST_EQ( h.result(), r );
    }

#if !defined(BOOST_NO_CXX11_HDR_ARRAY)

    {
        std::array<unsigned char, 4> v = {{ 1, 2, 3, 4 }};

        H h;

        hash_append( h, v );

        BOOST_TEST_EQ( h.result(), r );
    }

#endif
}

int main()
{
    test<boost::hash2::fnv1a_32>( 1463068797ul );
    test<boost::hash2::fnv1a_64>( 13725386680924731485ull );

    return boost::report_errors();
}
