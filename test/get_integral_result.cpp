// Copyright 2017, 2018, 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/get_integral_result.hpp>
#include <boost/hash2/digest.hpp>
#include <boost/array.hpp>
#include <array>

template<class R> void test()
{
    using boost::hash2::get_integral_result;

    R r = {};

    get_integral_result<signed char>( r );
    get_integral_result<unsigned char>( r );
    get_integral_result<short>( r );
    get_integral_result<unsigned short>( r );
    get_integral_result<int>( r );
    get_integral_result<unsigned int>( r );
    get_integral_result<long>( r );
    get_integral_result<unsigned long>( r );
    get_integral_result<long long>( r );
    get_integral_result<unsigned long long>( r );

    get_integral_result<char>( r );
    get_integral_result<char16_t>( r );
    get_integral_result<char32_t>( r );
}

int main()
{
    using boost::hash2::get_integral_result;

    test<unsigned char>();
    test<unsigned short>();
    test<unsigned int>();
    test<unsigned long>();
    test<unsigned long long>();

    test< std::array<unsigned char, 8> >();
    test< std::array<unsigned char, 16> >();
    test< std::array<unsigned char, 20> >();
    test< std::array<unsigned char, 28> >();
    test< std::array<unsigned char, 32> >();
    test< std::array<unsigned char, 48> >();
    test< std::array<unsigned char, 64> >();

    test< boost::array<unsigned char, 8> >();
    test< boost::array<unsigned char, 16> >();
    test< boost::array<unsigned char, 20> >();
    test< boost::array<unsigned char, 28> >();
    test< boost::array<unsigned char, 32> >();
    test< boost::array<unsigned char, 48> >();
    test< boost::array<unsigned char, 64> >();

    test< boost::hash2::digest<8> >();
    test< boost::hash2::digest<16> >();
    test< boost::hash2::digest<20> >();
    test< boost::hash2::digest<28> >();
    test< boost::hash2::digest<32> >();
    test< boost::hash2::digest<48> >();
    test< boost::hash2::digest<64> >();
}
