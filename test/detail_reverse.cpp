// Copyright 2024 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/detail/reverse.hpp>
#include <boost/core/lightweight_test.hpp>

template<std::size_t N> void test()
{
    unsigned char v[ N ] = {};
    for( std::size_t i = 0; i < N; ++i ) v[ i ] = static_cast<unsigned char>( i + 1 );

    unsigned char w[ N ] = {};
    boost::hash2::detail::reverse( w, v );

    for( std::size_t i = 0; i < N; ++i ) BOOST_TEST_EQ( w[ i ], v[ N - 1 - i ] );
}

int main()
{
    test<1>();
    test<2>();
    test<3>();
    test<4>();
    test<5>();
    test<6>();
    test<7>();
    test<8>();
    test<9>();
    test<10>();
    test<11>();
    test<12>();
    test<13>();
    test<14>();
    test<15>();
    test<16>();
    test<17>();

    return boost::report_errors();
}
