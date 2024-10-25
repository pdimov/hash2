// Copyright 2024 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/fnv1a.hpp>
#include <boost/core/lightweight_test.hpp>
#include <cstdint>

template<class Hash, class Flavor, class T> void test( T const& v, unsigned char const (&ref)[ sizeof(T) ] )
{
    {
        typename Hash::result_type r1, r2;

        {
            Hash h;
            Flavor f;

            hash_append( h, f, v );

            r1 = h.result();
        }

        {
            Hash h;

            h.update( ref, sizeof(ref) );

            r2 = h.result();
        }

        BOOST_TEST_EQ( r1, r2 );
    }
}

int main()
{
    using namespace boost::hash2;

    // float

    test<fnv1a_32, default_flavor>( +0.0f, { 0x00, 0x00, 0x00, 0x00 } );
    test<fnv1a_32, default_flavor>( -0.0f, { 0x00, 0x00, 0x00, 0x00 } );

    test<fnv1a_32, little_endian_flavor>( +1.0f, { 0x00, 0x00, 0x80, 0x3F } );
    test<fnv1a_32, little_endian_flavor>( -1.0f, { 0x00, 0x00, 0x80, 0xBF } );

    // test<fnv1a_32, big_endian_flavor>( +1.0f, { 0x3F, 0x80, 0x00, 0x00 } );
    // test<fnv1a_32, big_endian_flavor>( -1.0f, { 0xBF, 0x80, 0x00, 0x00 } );

    // double

    test<fnv1a_32, default_flavor>( +0.0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } );
    test<fnv1a_32, default_flavor>( -0.0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } );

    test<fnv1a_32, little_endian_flavor>( +1.0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F } );
    test<fnv1a_32, little_endian_flavor>( -1.0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xBF } );

    // test<fnv1a_32, big_endian_flavor>( +1.0, { 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } );
    // test<fnv1a_32, big_endian_flavor>( -1.0, { 0x3B, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } );

    // long double

    /* ... */

    return boost::report_errors();
}
