// Copyright 2017 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
//
// Endian-dependent test

// lib1.hpp

#include <string>

// forward declaration of boost::hash2::hash_append

namespace boost
{
namespace hash2
{

template<class Hash, class Flavor, class T> void hash_append( Hash& h, Flavor const& f, T const& v );

} // namespace hash2
} // namespace boost

// lib1::X

namespace lib1
{

class X
{
private:

    std::string s1_;
    std::string s2_;

    template<class Hash, class Flavor> friend void do_hash_append( Hash& h, Flavor const& f, X const& x )
    {
        boost::hash2::hash_append( h, f, x.s1_ );
        boost::hash2::hash_append( h, f, x.s2_ );
    }

public:

    X(): s1_( "s1" ), s2_( "s2" )
    {
    }
};

} // namespace lib1

// lib2.hpp

#include <cstdint>
#include <cstddef>

namespace lib2
{

class fnv1a
{
private:

    std::uint32_t st_;

public:

    typedef std::uint32_t result_type;

    fnv1a(): st_( 0x811C9DC5ul )
    {
    }

    void update( void const* pv, std::size_t n )
    {
        unsigned char const* p = static_cast<unsigned char const*>( pv );

        std::uint32_t h = st_;

        for( std::size_t i = 0; i < n; ++i )
        {
            h ^= static_cast<std::uint32_t>( p[i] );
            h *= 0x01000193ul;
        }

        st_ = h;
    }

    std::uint32_t result()
    {
        std::uint32_t r = st_;

        st_ = ( st_ ^ 0xFF ) * 0x01000193ul;

        return r;
    }
};

} // namespace lib2

// test

#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/fnv1a.hpp>
#include <boost/core/lightweight_test.hpp>
#include <vector>
#include <list>

template<class Hash, class Flavor, class R> void test( R r )
{
    {
        Hash h;
        Flavor f;

        boost::hash2::hash_append( h, f, std::vector<lib1::X>( 3 ) );

        BOOST_TEST_EQ( h.result(), r );
    }

    {
        Hash h;
        Flavor f;

        boost::hash2::hash_append( h, f, std::list<lib1::X>( 3 ) );

        BOOST_TEST_EQ( h.result(), r );
    }
}

int main()
{
    test<boost::hash2::fnv1a_32, boost::hash2::default_flavor>( 2425039999ul );
    test<lib2::fnv1a, boost::hash2::default_flavor>( 2425039999ul );

    return boost::report_errors();
}
