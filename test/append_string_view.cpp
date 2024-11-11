// Copyright 2017, 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/hash_append.hpp>
#include <boost/utility/string_view.hpp>
#include <boost/core/detail/string_view.hpp>
#include <boost/hash2/fnv1a.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/config.hpp>
#include <string>
#if !defined(BOOST_NO_CXX17_HDR_STRING_VIEW)
#include <string_view>
#endif

template<class Hash, class Flavor, class T, class R> void test( R r )
{
    std::basic_string<T> str{ 1, 2, 3, 4 };

    Flavor f;

    {
        Hash h;
        boost::hash2::hash_append( h, f, str );

        BOOST_TEST_EQ( h.result(), r );
    }

    {
        Hash h;
        boost::hash2::hash_append( h, f, boost::basic_string_view<T>( str ) );

        BOOST_TEST_EQ( h.result(), r );
    }

    {
        Hash h;
        boost::hash2::hash_append( h, f, boost::core::basic_string_view<T>( str ) );

        BOOST_TEST_EQ( h.result(), r );
    }

#if !defined(BOOST_NO_CXX17_HDR_STRING_VIEW)
    {
        Hash h;
        boost::hash2::hash_append( h, f, std::basic_string_view<T>( str ) );

        BOOST_TEST_EQ( h.result(), r );
    }
#endif
}

int main()
{
    using namespace boost::hash2;

    test<fnv1a_32, little_endian_flavor, char>( 2468847257 );
    test<fnv1a_32, big_endian_flavor, char>( 78451921 );

#if defined(__cpp_char8_t) && __cpp_char8_t >= 201811L

    test<fnv1a_32, little_endian_flavor, char8_t>( 2468847257 );
    test<fnv1a_32, big_endian_flavor, char8_t>( 78451921 );

#endif

    test<fnv1a_32, little_endian_flavor, char16_t>( 634283301 );
    test<fnv1a_32, big_endian_flavor, char16_t>( 1469224165 );

    test<fnv1a_32, little_endian_flavor, char32_t>( 2374676325 );
    test<fnv1a_32, big_endian_flavor, char32_t>( 2877134693 );

    return boost::report_errors();
}
