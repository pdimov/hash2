// Copyright 2017 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/fnv1a.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/config.hpp>
#include <cstring>

#define STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)

template<class H, std::size_t N> BOOST_CXX14_CONSTEXPR typename H::result_type test( std::uint64_t seed, unsigned char const (&v)[ N ] )
{
    H h( seed );

    h.update( v, N );

    return h.result();
}

int main()
{
    using namespace boost::hash2;

    BOOST_TEST_EQ( test<fnv1a_32>( 0, { 0 } ), 84696351 );
    BOOST_TEST_EQ( test<fnv1a_32>( 0, { 0, 1, 2, 3 } ), 3282719153 );

    BOOST_TEST_EQ( test<fnv1a_64>( 0, { 0 } ), 12638153115695167455ull );
    BOOST_TEST_EQ( test<fnv1a_64>( 0, { 0, 1, 2, 3 } ), 4932904490461320209 );

#if !defined(BOOST_NO_CXX14_CONSTEXPR)

    STATIC_ASSERT( test<fnv1a_32>( 0, { 0 } ) == 84696351 );
    STATIC_ASSERT( test<fnv1a_32>( 0, { 0, 1, 2, 3 } ) == 3282719153 );

    STATIC_ASSERT( test<fnv1a_64>( 0, { 0 } ) == 12638153115695167455ull );
    STATIC_ASSERT( test<fnv1a_64>( 0, { 0, 1, 2, 3 } ) == 4932904490461320209 );

#endif

    return boost::report_errors();
}
