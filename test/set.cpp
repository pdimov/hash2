
// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
//
// Endian-dependent test

#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/fnv1a.hpp>
#include <boost/core/lightweight_test.hpp>
#include <set>
#if !defined(BOOST_NO_CXX11_HDR_UNORDERED_SET)
# include <unordered_set>
#endif

template<class H, class S, class R> void test( R r )
{
    {
        S s;

        for( int i = 0; i < 64; ++i )
        {
            s.insert( i );
        }

        H h;

        hash_append( h, s );

        BOOST_TEST_EQ( h.result(), r );
    }

    {
        S s;

        for( int i = 0; i < 64; ++i )
        {
            s.insert( 63 - i );
        }

        H h;

        hash_append( h, s );

        BOOST_TEST_EQ( h.result(), r );
    }

    {
        S s;

        for( int i = 0; i < 64; ++i )
        {
            s.insert( ( i * 17 ) % 64 );
        }

        H h;

        hash_append( h, s );

        BOOST_TEST_EQ( h.result(), r );
    }
}

int main()
{
    test< boost::hash2::fnv1a_32, std::set<int> >( 2078558933ul );
    test< boost::hash2::fnv1a_64, std::set<int> >( 17046016161958689285ull );

    test< boost::hash2::fnv1a_32, std::multiset<int> >( 2078558933ul );
    test< boost::hash2::fnv1a_64, std::multiset<int> >( 17046016161958689285ull );

#if !defined(BOOST_NO_CXX11_HDR_UNORDERED_SET)

    test< boost::hash2::fnv1a_32, std::unordered_set<int> >( 2270492092ul );
    test< boost::hash2::fnv1a_64, std::unordered_set<int> >( 3232503781718511241ull );

    test< boost::hash2::fnv1a_32, std::unordered_multiset<int> >( 2270492092ul );
    test< boost::hash2::fnv1a_64, std::unordered_multiset<int> >( 3232503781718511241ull );

#endif

    return boost::report_errors();
}
