
// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hash2/endian.hpp>
#include <boost/hash2/byte_type.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/static_assert.hpp>
#include <boost/cstdint.hpp>
#include <cstring>

int main()
{
    using namespace boost::hash2;

    BOOST_TEST_NE( static_cast<int>( endian::little ), static_cast<int>( endian::big ) );

    byte_type const v[ 4 ] = { 0x01, 0x02, 0x03, 0x04 };

    boost::uint32_t w;

    BOOST_STATIC_ASSERT( sizeof( w ) == 4 );

    std::memcpy( &w, v, 4 );

    if( endian::native == endian::little )
    {
        BOOST_TEST_EQ( w, 0x04030201 );
    }
    else if( endian::native == endian::big )
    {
        BOOST_TEST_EQ( w, 0x01020304 );
    }
    else
    {
        BOOST_TEST_NE( w, 0x04030201 );
        BOOST_TEST_NE( w, 0x01020304 );
    }

    return boost::report_errors();
}
