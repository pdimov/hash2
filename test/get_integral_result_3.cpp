// Copyright 2017, 2018, 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/get_integral_result.hpp>
#include <boost/hash2/digest.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/array.hpp>
#include <cstdint>
#include <array>

template<class R> void test()
{
    using boost::hash2::get_integral_result;

    {
        R a64 = {{ 0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01 }};

        BOOST_TEST_EQ( get_integral_result<std::uint8_t>( a64 ), 0xEF );
        BOOST_TEST_EQ( get_integral_result<std::int8_t>( a64 ), -0x11 );
        BOOST_TEST_EQ( get_integral_result<std::uint16_t>( a64 ), 0xCDEF );
        BOOST_TEST_EQ( get_integral_result<std::int16_t>( a64 ), -0x3211 );
        BOOST_TEST_EQ( get_integral_result<std::uint32_t>( a64 ), 0x89ABCDEF );
        BOOST_TEST_EQ( get_integral_result<std::int32_t>( a64 ), 0x89ABCDEF );
        BOOST_TEST_EQ( get_integral_result<std::uint64_t>( a64 ), 0x0123456789ABCDEFull );
        BOOST_TEST_EQ( get_integral_result<std::int64_t>( a64 ), 0x0123456789ABCDEFull );
    }

    {
        R b64 = {{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }};

        BOOST_TEST_EQ( get_integral_result<std::uint8_t>( b64 ), 0xFF );
        BOOST_TEST_EQ( get_integral_result<std::int8_t>( b64 ), -1 );
        BOOST_TEST_EQ( get_integral_result<std::uint16_t>( b64 ), 0xFFFF );
        BOOST_TEST_EQ( get_integral_result<std::int16_t>( b64 ), -1 );
        BOOST_TEST_EQ( get_integral_result<std::uint32_t>( b64 ), 0xFFFFFFFFu );
        BOOST_TEST_EQ( get_integral_result<std::int32_t>( b64 ), -1 );
        BOOST_TEST_EQ( get_integral_result<std::uint64_t>( b64 ), 0xFFFFFFFFFFFFFFFFull );
        BOOST_TEST_EQ( get_integral_result<std::int64_t>( b64 ), -1 );
    }
}

int main()
{
    test< std::array<unsigned char, 8> >();
    test< boost::array<unsigned char, 8> >();
    test< boost::hash2::digest<8> >();

    return boost::report_errors();
}
