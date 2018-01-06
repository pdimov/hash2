
// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hash2/underlying_type.hpp>
#include <boost/core/lightweight_test_trait.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/type_traits/is_enum.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/config.hpp>
#include <climits>

enum E1
{
    e1
};

enum E2
{
    e2 = SCHAR_MAX
};

enum E3
{
    e3 = SCHAR_MIN
};

enum E4
{
    e4 = UCHAR_MAX
};

enum E5
{
    e5 = SHRT_MAX
};

enum E6
{
    e6 = SHRT_MIN
};

enum E7
{
    e7 = USHRT_MAX
};

enum E8
{
    e8 = INT_MAX
};

enum E9
{
    e9 = INT_MIN
};

enum E10
{
    e10 = UINT_MAX
};

enum E11
{
    e11 = LONG_MAX
};

enum E12
{
    e12 = LONG_MIN
};

enum E13
{
    e13 = ULONG_MAX
};

enum E14
{
    e14 = LLONG_MAX
};

enum E15
{
    e15 = LLONG_MIN
};

enum E16
{
    e16 = ULLONG_MAX
};

template<class E> void test( E e )
{
    typedef typename boost::hash2::underlying_type<E>::type U;

    BOOST_TEST_TRAIT_TRUE(( boost::is_enum<E> ));
    BOOST_TEST_TRAIT_TRUE(( boost::is_integral<U> ));

    BOOST_TEST_EQ( sizeof(E), sizeof(U) );

    U u = static_cast<U>( e );

    BOOST_TEST_EQ( u, e );

    unsigned char const * pe = reinterpret_cast<unsigned char const*>( &e );
    unsigned char const * pu = reinterpret_cast<unsigned char const*>( &u );

    BOOST_TEST_ALL_EQ( pe, pe + sizeof( e ), pu, pu + sizeof( u ) );
}

int main()
{
    test( e1 );
    test( e2 );
    test( e3 );
    test( e4 );
    test( e5 );
    test( e6 );
    test( e7 );
    test( e8 );
    test( e9 );
    test( e10 );
    test( e11 );
    test( e12 );
    test( e13 );
    test( e14 );
    test( e15 );
    test( e16 );

    return boost::report_errors();
}
