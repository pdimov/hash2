// Copyright 2023 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/detail/has_constant_size.hpp>
#include <boost/core/lightweight_test_trait.hpp>
#include <boost/array.hpp>
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <array>
#include <forward_list>
#include <unordered_set>
#include <unordered_map>

struct X
{
};

int main()
{
    using boost::hash2::detail::has_constant_size;

    BOOST_TEST_TRAIT_FALSE((has_constant_size<void>));
    BOOST_TEST_TRAIT_FALSE((has_constant_size<void const>));

    BOOST_TEST_TRAIT_FALSE((has_constant_size<int>));
    BOOST_TEST_TRAIT_FALSE((has_constant_size<int const>));

    BOOST_TEST_TRAIT_FALSE((has_constant_size<X>));
    BOOST_TEST_TRAIT_FALSE((has_constant_size<X const>));

    BOOST_TEST_TRAIT_FALSE((has_constant_size<int[2]>));
    BOOST_TEST_TRAIT_FALSE((has_constant_size<int const[2]>));

    BOOST_TEST_TRAIT_FALSE((has_constant_size<std::string>));
    BOOST_TEST_TRAIT_FALSE((has_constant_size<std::string const>));

    BOOST_TEST_TRAIT_FALSE((has_constant_size<std::wstring>));
    BOOST_TEST_TRAIT_FALSE((has_constant_size<std::wstring const>));

    BOOST_TEST_TRAIT_FALSE((has_constant_size< std::vector<X> >));
    BOOST_TEST_TRAIT_FALSE((has_constant_size< std::vector<X> const >));

    BOOST_TEST_TRAIT_FALSE((has_constant_size< std::deque<X> >));
    BOOST_TEST_TRAIT_FALSE((has_constant_size< std::deque<X> const >));

    BOOST_TEST_TRAIT_FALSE((has_constant_size< std::set<int> >));
    BOOST_TEST_TRAIT_FALSE((has_constant_size< std::set<int> const >));

    BOOST_TEST_TRAIT_FALSE((has_constant_size< std::multiset<int> >));
    BOOST_TEST_TRAIT_FALSE((has_constant_size< std::multiset<int> const >));

    BOOST_TEST_TRAIT_FALSE((has_constant_size< std::map<int, X> >));
    BOOST_TEST_TRAIT_FALSE((has_constant_size< std::map<int, X> const >));

    BOOST_TEST_TRAIT_FALSE((has_constant_size< std::multimap<int, X> >));
    BOOST_TEST_TRAIT_FALSE((has_constant_size< std::multimap<int, X> const >));

    BOOST_TEST_TRAIT_TRUE((has_constant_size< std::array<X, 2> >));
    BOOST_TEST_TRAIT_TRUE((has_constant_size< std::array<X, 2> const >));

    BOOST_TEST_TRAIT_TRUE((has_constant_size< std::array<X, 0> >));
    BOOST_TEST_TRAIT_TRUE((has_constant_size< std::array<X, 0> const >));

    BOOST_TEST_TRAIT_FALSE((has_constant_size< std::forward_list<X> >));
    BOOST_TEST_TRAIT_FALSE((has_constant_size< std::forward_list<X> const >));

    BOOST_TEST_TRAIT_FALSE((has_constant_size< std::unordered_set<int> >));
    BOOST_TEST_TRAIT_FALSE((has_constant_size< std::unordered_set<int> const >));

    BOOST_TEST_TRAIT_FALSE((has_constant_size< std::unordered_multiset<int> >));
    BOOST_TEST_TRAIT_FALSE((has_constant_size< std::unordered_multiset<int> const >));

    BOOST_TEST_TRAIT_FALSE((has_constant_size< std::unordered_map<int, X> >));
    BOOST_TEST_TRAIT_FALSE((has_constant_size< std::unordered_map<int, X> const >));

    BOOST_TEST_TRAIT_FALSE((has_constant_size< std::unordered_multimap<int, X> >));
    BOOST_TEST_TRAIT_FALSE((has_constant_size< std::unordered_multimap<int, X> const >));

    BOOST_TEST_TRAIT_TRUE((has_constant_size< boost::array<X, 2> >));
    BOOST_TEST_TRAIT_TRUE((has_constant_size< boost::array<X, 2> const >));

    BOOST_TEST_TRAIT_TRUE((has_constant_size< boost::array<X, 0> >));
    BOOST_TEST_TRAIT_TRUE((has_constant_size< boost::array<X, 0> const >));

    return boost::report_errors();
}
