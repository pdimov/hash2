// Copyright 2017 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
//
// Endian-independent test

#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/fnv1a.hpp>
#include <boost/core/lightweight_test.hpp>
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <forward_list>

template<class H, class T, class R> void test_unsized_range( R r )
{
    unsigned char w[] = { 1, 2, 3, 4 };
    T v( w, w + sizeof(w) / sizeof(w[0]) );

    H h;

    hash_append_range( h, v.begin(), v.end() );

    BOOST_TEST_EQ( h.result(), r );
}

template<class H, class R> void test( R r )
{
    test_unsized_range< H, std::string >( r );
    test_unsized_range< H, std::vector<unsigned char> >( r );
    test_unsized_range< H, std::list<unsigned char> >( r );
    test_unsized_range< H, std::deque<unsigned char> >( r );
    test_unsized_range< H, std::set<unsigned char> >( r );
    test_unsized_range< H, std::multiset<unsigned char> >( r );
    test_unsized_range< H, std::forward_list<unsigned char> >( r );

    {
        unsigned char v[2][2] = { { 1, 2 }, { 3, 4 } };

        H h;

        hash_append_range( h, v, v + 2 );

        BOOST_TEST_EQ( h.result(), r );
    }
}

int main()
{
    test<boost::hash2::fnv1a_32>( 1463068797ul );
    test<boost::hash2::fnv1a_64>( 13725386680924731485ull );

    return boost::report_errors();
}
