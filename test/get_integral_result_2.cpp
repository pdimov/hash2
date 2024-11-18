// Copyright 2017, 2018, 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/get_integral_result.hpp>
#include <boost/hash2/digest.hpp>
#include <boost/array.hpp>
#include <boost/core/lightweight_test.hpp>
#include <array>
#include <cstdint>

template<class R> void test( R const& r )
{
    using boost::hash2::get_integral_result;

    {
        auto t1 = get_integral_result<signed char>( r );
        auto t2 = get_integral_result<unsigned char>( r );

        BOOST_TEST_EQ( static_cast<unsigned char>( t1 ), t2 );
    }

    {
        auto t1 = get_integral_result<signed short>( r );
        auto t2 = get_integral_result<unsigned short>( r );

        BOOST_TEST_EQ( static_cast<unsigned short>( t1 ), t2 );
    }

    {
        auto t1 = get_integral_result<signed int>( r );
        auto t2 = get_integral_result<unsigned int>( r );

        BOOST_TEST_EQ( static_cast<unsigned int>( t1 ), t2 );
    }

    {
        auto t1 = get_integral_result<signed long>( r );
        auto t2 = get_integral_result<unsigned long>( r );

        BOOST_TEST_EQ( static_cast<unsigned long>( t1 ), t2 );
    }

    {
        auto t1 = get_integral_result<signed long long>( r );
        auto t2 = get_integral_result<unsigned long long>( r );

        BOOST_TEST_EQ( static_cast<unsigned long long>( t1 ), t2 );
    }
}

int main()
{
    using boost::hash2::get_integral_result;

    test<std::uint8_t>( 0x1E );
    test<std::uint8_t>( 0xE1 );

    test<std::uint16_t>( 0x1E1E );
    test<std::uint16_t>( 0xE1E1 );

    test<std::uint32_t>( 0x1E1E1E1Eu );
    test<std::uint32_t>( 0xE1E1E1E1u );

    test<std::uint64_t>( 0x1E1E1E1E1E1E1E1Eull );
    test<std::uint64_t>( 0xE1E1E1E1E1E1E1E1ull );

    test< std::array<unsigned char, 8> >( {{ 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E }} );
    test< std::array<unsigned char, 8> >( {{ 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1 }} );

    test< boost::array<unsigned char, 8> >( {{ 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E }} );
    test< boost::array<unsigned char, 8> >( {{ 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1 }} );

    test< boost::hash2::digest<8> >( {{ 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E }} );
    test< boost::hash2::digest<8> >( {{ 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1 }} );

    return boost::report_errors();
}
