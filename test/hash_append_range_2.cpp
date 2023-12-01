// Copyright 2017 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
//
// Endian-dependent test

#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/fnv1a.hpp>
#include <boost/core/lightweight_test.hpp>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <forward_list>

template<class H, class T, class R> void test_unsized_range( R r )
{
    int w[] = { 1, 2, 3, 4 };
    T v( w, w + sizeof(w) / sizeof(w[0]) );

    H h;

    hash_append_range( h, v.begin(), v.end() );

    BOOST_TEST_EQ( h.result(), r );
}

template<class H, class R> void test( R r )
{
    test_unsized_range< H, std::vector<int> >( r );
    test_unsized_range< H, std::list<int> >( r );
    test_unsized_range< H, std::deque<int> >( r );
    test_unsized_range< H, std::set<int> >( r );
    test_unsized_range< H, std::multiset<int> >( r );
    test_unsized_range< H, std::forward_list<int> >( r );

    {
        int v[2][2] = { { 1, 2 }, { 3, 4 } };

        H h;

        hash_append_range( h, v, v + 2 );

        BOOST_TEST_EQ( h.result(), r );
    }
}

int main()
{
    test<boost::hash2::fnv1a_32>( 1041505217ul );
    test<boost::hash2::fnv1a_64>( 9566659391000707361ull );

    return boost::report_errors();
}
