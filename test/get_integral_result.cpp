// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/get_integral_result.hpp>
#include <boost/hash2/byte_type.hpp>
#include <boost/cstdint.hpp>
#include <boost/core/lightweight_test.hpp>
#include <array>
#include <cstddef>

int main()
{
    using boost::hash2::get_integral_result;

    {
        boost::uint32_t u32 = 0x12345678;

        BOOST_TEST_EQ( get_integral_result<boost::uint8_t>( u32 ), 0x78 );
        BOOST_TEST_EQ( get_integral_result<boost::int8_t>( u32 ), 0x78 );
        BOOST_TEST_EQ( get_integral_result<boost::uint16_t>( u32 ), 0x5678 );
        BOOST_TEST_EQ( get_integral_result<boost::int16_t>( u32 ), 0x5678 );
        BOOST_TEST_EQ( get_integral_result<boost::uint32_t>( u32 ), 0x12345678 );
        BOOST_TEST_EQ( get_integral_result<boost::int32_t>( u32 ), 0x12345678 );
        BOOST_TEST_EQ( get_integral_result<boost::uint64_t>( u32 ), 0x1234567812345678ull );
        BOOST_TEST_EQ( get_integral_result<boost::int64_t>( u32 ), 0x1234567812345678ull );
    }

    {
        boost::uint32_t s32 = 0xFFFFFFFFu;

        BOOST_TEST_EQ( get_integral_result<boost::uint8_t>( s32 ), 0xFF );
        BOOST_TEST_EQ( get_integral_result<boost::int8_t>( s32 ), -1 );
        BOOST_TEST_EQ( get_integral_result<boost::uint16_t>( s32 ), 0xFFFF );
        BOOST_TEST_EQ( get_integral_result<boost::int16_t>( s32 ), -1 );
        BOOST_TEST_EQ( get_integral_result<boost::uint32_t>( s32 ), 0xFFFFFFFFu );
        BOOST_TEST_EQ( get_integral_result<boost::int32_t>( s32 ), -1 );
        BOOST_TEST_EQ( get_integral_result<boost::uint64_t>( s32 ), 0xFFFFFFFFFFFFFFFFull );
        BOOST_TEST_EQ( get_integral_result<boost::int64_t>( s32 ), -1 );
    }

    {
        boost::uint64_t u64 = 0x0123456789ABCDEFull;

        BOOST_TEST_EQ( get_integral_result<boost::uint8_t>( u64 ), 0xEF );
        BOOST_TEST_EQ( get_integral_result<boost::int8_t>( u64 ), -0x11 );
        BOOST_TEST_EQ( get_integral_result<boost::uint16_t>( u64 ), 0xCDEF );
        BOOST_TEST_EQ( get_integral_result<boost::int16_t>( u64 ), -0x3211 );
        BOOST_TEST_EQ( get_integral_result<boost::uint32_t>( u64 ), 0x89ABCDEF );
        BOOST_TEST_EQ( get_integral_result<boost::int32_t>( u64 ), 0x89ABCDEF );
        BOOST_TEST_EQ( get_integral_result<boost::uint64_t>( u64 ), 0x0123456789ABCDEFull );
        BOOST_TEST_EQ( get_integral_result<boost::int64_t>( u64 ), 0x0123456789ABCDEFull );
    }

    {
        boost::uint64_t s64 = 0xFFFFFFFFFFFFFFFFull;

        BOOST_TEST_EQ( get_integral_result<boost::uint8_t>( s64 ), 0xFF );
        BOOST_TEST_EQ( get_integral_result<boost::int8_t>( s64 ), -1 );
        BOOST_TEST_EQ( get_integral_result<boost::uint16_t>( s64 ), 0xFFFF );
        BOOST_TEST_EQ( get_integral_result<boost::int16_t>( s64 ), -1 );
        BOOST_TEST_EQ( get_integral_result<boost::uint32_t>( s64 ), 0xFFFFFFFFu );
        BOOST_TEST_EQ( get_integral_result<boost::int32_t>( s64 ), -1 );
        BOOST_TEST_EQ( get_integral_result<boost::uint64_t>( s64 ), 0xFFFFFFFFFFFFFFFFull );
        BOOST_TEST_EQ( get_integral_result<boost::int64_t>( s64 ), -1 );
    }

    using boost::hash2::byte_type;

    {
        std::array<byte_type, 8> a64 = {{ 0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01 }};

        BOOST_TEST_EQ( get_integral_result<boost::uint8_t>( a64 ), 0xEF );
        BOOST_TEST_EQ( get_integral_result<boost::int8_t>( a64 ), -0x11 );
        BOOST_TEST_EQ( get_integral_result<boost::uint16_t>( a64 ), 0xCDEF );
        BOOST_TEST_EQ( get_integral_result<boost::int16_t>( a64 ), -0x3211 );
        BOOST_TEST_EQ( get_integral_result<boost::uint32_t>( a64 ), 0x89ABCDEF );
        BOOST_TEST_EQ( get_integral_result<boost::int32_t>( a64 ), 0x89ABCDEF );
        BOOST_TEST_EQ( get_integral_result<boost::uint64_t>( a64 ), 0x0123456789ABCDEFull );
        BOOST_TEST_EQ( get_integral_result<boost::int64_t>( a64 ), 0x0123456789ABCDEFull );
    }

    {
        std::array<byte_type, 8> b64 = {{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }};

        BOOST_TEST_EQ( get_integral_result<boost::uint8_t>( b64 ), 0xFF );
        BOOST_TEST_EQ( get_integral_result<boost::int8_t>( b64 ), -1 );
        BOOST_TEST_EQ( get_integral_result<boost::uint16_t>( b64 ), 0xFFFF );
        BOOST_TEST_EQ( get_integral_result<boost::int16_t>( b64 ), -1 );
        BOOST_TEST_EQ( get_integral_result<boost::uint32_t>( b64 ), 0xFFFFFFFFu );
        BOOST_TEST_EQ( get_integral_result<boost::int32_t>( b64 ), -1 );
        BOOST_TEST_EQ( get_integral_result<boost::uint64_t>( b64 ), 0xFFFFFFFFFFFFFFFFull );
        BOOST_TEST_EQ( get_integral_result<boost::int64_t>( b64 ), -1 );
    }

    return boost::report_errors();
}
