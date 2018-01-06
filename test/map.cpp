
// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
//
// Endian-dependent test

#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/fnv1a.hpp>
#include <boost/core/lightweight_test.hpp>
#include <map>
#if !defined(BOOST_NO_CXX11_HDR_UNORDERED_MAP)
# include <unordered_map>
#endif

template<class H, class M, class R> void test( R r )
{
    {
        M m;

        for( int i = 0; i < 64; ++i )
        {
            m.insert( typename M::value_type( i, 3 * i + 1 ) );
        }

        H h;

        hash_append( h, m );

        BOOST_TEST_EQ( h.result(), r );
    }

    {
        M m;

        for( int i = 0; i < 64; ++i )
        {
            int j = 63 - i;
            m.insert( typename M::value_type( j, 3 * j + 1 ) );
        }

        H h;

        hash_append( h, m );

        BOOST_TEST_EQ( h.result(), r );
    }

    {
        M m;

        for( int i = 0; i < 64; ++i )
        {
            int j = ( i * 17 ) % 64;
            m.insert( typename M::value_type( j, 3 * j + 1 ) );
        }

        H h;

        hash_append( h, m );

        BOOST_TEST_EQ( h.result(), r );
    }
}

int main()
{
    test< boost::hash2::fnv1a_32, std::map<int, int> >( 3152726101ul );
    test< boost::hash2::fnv1a_64, std::map<int, int> >( 12051529320333828229ull );

    test< boost::hash2::fnv1a_32, std::multimap<int, int> >( 3152726101ul );
    test< boost::hash2::fnv1a_64, std::multimap<int, int> >( 12051529320333828229ull );

#if !defined(BOOST_NO_CXX11_HDR_UNORDERED_MAP)

    test< boost::hash2::fnv1a_32, std::unordered_map<int, int> >( 2742410178ul );
    test< boost::hash2::fnv1a_64, std::unordered_map<int, int> >( 2617313294186790738ull );

    test< boost::hash2::fnv1a_32, std::unordered_multimap<int, int> >( 2742410178ul );
    test< boost::hash2::fnv1a_64, std::unordered_multimap<int, int> >( 2617313294186790738ull );

#endif

    return boost::report_errors();
}
