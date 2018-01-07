#ifndef BOOST_HASH2_HMAC_HPP_INCLUDED
#define BOOST_HASH2_HMAC_HPP_INCLUDED

// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
//
// HMAC message authentication algorithm, https://tools.ietf.org/html/rfc2104

#include <boost/hash2/byte_type.hpp>
#include <boost/hash2/detail/write.hpp>
#include <boost/cstdint.hpp>
#include <boost/assert.hpp>
#include <cstring>

namespace boost
{
namespace hash2
{

template<class H> class hmac
{
public:

    typedef typename H::result_type result_type;
    typedef typename H::size_type size_type;

    static const int block_size = H::block_size;

private:

    H outer_;
    H inner_;

private:

    void init( byte_type const * p, std::ptrdiff_t n )
    {
        int const m = block_size;

        byte_type key[ m ] = {};

        if( n == 0 )
        {
            // memcpy from (NULL, 0) is undefined
        }
        else if( n <= m )
        {
            std::memcpy( key, p, n );
        }
        else
        {
            H h;

            h.update( p, n );

            result_type r = h.result();

            std::memcpy( key, &r[0], r.size() );
        }

        for( int i = 0; i < m; ++i )
        {
            key[ i ] = static_cast<byte_type>( key[ i ] ^ 0x36 );
        }

        inner_.update( key, m );

        for( int i = 0; i < m; ++i )
        {
            key[ i ] = static_cast<byte_type>( key[ i ] ^ 0x36 ^ 0x5C );
        }

        outer_.update( key, m );
    }

public:

    hmac()
    {
        init( 0, 0 );
    }

    explicit hmac( boost::uint64_t seed )
    {
        if( seed == 0 )
        {
            init( 0, 0 );
        }
        else
        {
            byte_type tmp[ 8 ];
            detail::write64le( tmp, seed );

            init( tmp, 8 );
        }
    }

    hmac( byte_type const * p, std::ptrdiff_t n )
    {
        BOOST_ASSERT( n >= 0 );
        init( p, n );
    }

    void update( byte_type const * p, std::ptrdiff_t n )
    {
        BOOST_ASSERT( n >= 0 );
        inner_.update( p, n );
    }

    result_type result()
    {
        result_type r = inner_.result();

        outer_.update( &r[0], r.size() );

        return outer_.result();
    }
};

} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_HMAC_HPP_INCLUDED
