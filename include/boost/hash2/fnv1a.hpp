#ifndef BOOST_HASH2_FNV1A_HPP_INCLUDED
#define BOOST_HASH2_FNV1A_HPP_INCLUDED

// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
//
// FNV-1a
//
// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function

#include <boost/hash2/byte_type.hpp>
#include <boost/hash2/detail/write.hpp>
#include <boost/cstdint.hpp>
#include <boost/assert.hpp>
#include <boost/config.hpp>
#include <cstddef>

namespace boost
{
namespace hash2
{

namespace detail
{

template<class T> struct fnv1a_const;

template<> struct fnv1a_const<boost::uint32_t>
{
    BOOST_STATIC_CONSTEXPR boost::uint32_t basis = 0x811C9DC5ul;
    BOOST_STATIC_CONSTEXPR boost::uint32_t prime = 0x01000193ul;
};

template<> struct fnv1a_const<boost::uint64_t>
{
    BOOST_STATIC_CONSTEXPR boost::uint64_t basis = 0xCBF29CE484222325ull;
    BOOST_STATIC_CONSTEXPR boost::uint64_t prime = 0x00000100000001B3ull;
};

template<class T> class fnv1a
{
private:

    T st_;

public:

    typedef T result_type;
    typedef T size_type;

    fnv1a(): st_( fnv1a_const<T>::basis )
    {
    }

    explicit fnv1a( boost::uint64_t seed ): st_( fnv1a_const<T>::basis )
    {
        if( seed )
        {
            if( seed >> 32 )
            {
                byte_type tmp[ 8 ];
                detail::write64le( tmp, seed );
                update( tmp, 8 );
            }
            else
            {
                byte_type tmp[ 4 ];
                detail::write32le( tmp, static_cast<boost::uint32_t>( seed ) );
                update( tmp, 4 );
            }
        }
    }

    fnv1a( byte_type const * p, std::ptrdiff_t n ): st_( fnv1a_const<T>::basis )
    {
        BOOST_ASSERT( n >= 0 );

        if( n != 0 )
        {
            update( p, n );
        }
    }

    void update( byte_type const * p, std::ptrdiff_t n )
    {
        BOOST_ASSERT( n >= 0 );

        T h = st_;

        for( std::ptrdiff_t i = 0; i < n; ++i )
        {
            h ^= static_cast<T>( p[i] );
            h *= fnv1a_const<T>::prime;
        }

        st_ = h;
    }

    T result()
    {
        T r = st_;

        // advance as if by update( "\xFF", 1 ), to allow
        // multiple result() calls to generate a sequence
        // of distinct values

        st_ = ( st_ ^ 0xFF ) * fnv1a_const<T>::prime;

        return r;
    }
};

} // namespace detail

class fnv1a_32: public detail::fnv1a<boost::uint32_t>
{
public:

    fnv1a_32(): detail::fnv1a<boost::uint32_t>()
    {
    }

    explicit fnv1a_32( boost::uint64_t seed ): detail::fnv1a<boost::uint32_t>( seed )
    {
    }

    fnv1a_32( byte_type const * p, std::ptrdiff_t n ): detail::fnv1a<boost::uint32_t>( p, n )
    {
    }
};

class fnv1a_64: public detail::fnv1a<boost::uint64_t>
{
public:

    fnv1a_64(): detail::fnv1a<boost::uint64_t>()
    {
    }

    explicit fnv1a_64( boost::uint64_t seed ): detail::fnv1a<boost::uint64_t>( seed )
    {
    }

    fnv1a_64( byte_type const * p, std::ptrdiff_t n ): detail::fnv1a<boost::uint64_t>( p, n )
    {
    }
};

} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_FNV1A_HPP_INCLUDED
