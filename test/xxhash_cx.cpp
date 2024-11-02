// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/xxhash.hpp>
#include <boost/hash2/detail/config.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/config.hpp>
#include <cstring>

#if defined(BOOST_MSVC) && BOOST_MSVC < 1920
# pragma warning(disable: 4307) // integral constant overflow
#endif

#define STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)

template<class H, std::size_t N> BOOST_CXX14_CONSTEXPR typename H::result_type test( std::uint64_t seed, unsigned char const (&v)[ N ] )
{
    H h( seed );

    h.update( v, N / 3 );
    h.update( v, N - N / 3 );

    return h.result();
}

int main()
{
    using namespace boost::hash2;

    constexpr unsigned char v21[ 21 ] = {};
    constexpr unsigned char v45[ 45 ] = {};

    BOOST_TEST_EQ( test<xxhash_32>( 0, v21 ), 86206869 );
    BOOST_TEST_EQ( test<xxhash_32>( 0, v45 ), 747548280 );

    BOOST_TEST_EQ( test<xxhash_64>( 0, v21 ), 8680240691998137788 );
    BOOST_TEST_EQ( test<xxhash_64>( 0, v45 ), 4352694002423811028 );

    BOOST_TEST_EQ( test<xxhash_32>( 7, v21 ), 2135174986 );
    BOOST_TEST_EQ( test<xxhash_32>( 7, v45 ), 1547773082 );

    BOOST_TEST_EQ( test<xxhash_64>( 7, v21 ), 16168826474312362322ull );
    BOOST_TEST_EQ( test<xxhash_64>( 7, v45 ), 14120916949766558435ull );

#if !defined(BOOST_NO_CXX14_CONSTEXPR)

    STATIC_ASSERT( test<xxhash_32>( 0, v21 ) == 86206869 );
    STATIC_ASSERT( test<xxhash_32>( 0, v45 ) == 747548280 );

    STATIC_ASSERT( test<xxhash_64>( 0, v21 ) == 8680240691998137788 );
    STATIC_ASSERT( test<xxhash_64>( 0, v45 ) == 4352694002423811028 );

    STATIC_ASSERT( test<xxhash_32>( 7, v21 ) == 2135174986 );
    STATIC_ASSERT( test<xxhash_32>( 7, v45 ) == 1547773082 );

    STATIC_ASSERT( test<xxhash_64>( 7, v21 ) == 16168826474312362322ull );
    STATIC_ASSERT( test<xxhash_64>( 7, v45 ) == 14120916949766558435ull );

#endif

    return boost::report_errors();
}
