#ifndef BOOST_HASH2_SIPHASH_HPP_INCLUDED
#define BOOST_HASH2_SIPHASH_HPP_INCLUDED

// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
//
// SipHash, https://131002.net/siphash/

#include <boost/hash2/detail/read.hpp>
#include <boost/hash2/detail/rot.hpp>
#include <boost/assert.hpp>
#include <cstdint>
#include <cstring>
#include <cstddef>

namespace boost
{
namespace hash2
{

class siphash_64
{
private:

    std::uint64_t v0, v1, v2, v3;

    unsigned char buffer_[ 8 ];
    std::size_t m_; // == n_ % 8

    std::uint64_t n_;

private:

    void sipround()
    {
        v0 += v1;
        v1 = detail::rotl(v1, 13);
        v1 ^= v0;
        v0 = detail::rotl(v0, 32);
        v2 += v3;
        v3 = detail::rotl(v3, 16);
        v3 ^= v2;
        v0 += v3;
        v3 = detail::rotl(v3, 21);
        v3 ^= v0;
        v2 += v1;
        v1 = detail::rotl(v1, 17);
        v1 ^= v2;
        v2 = detail::rotl(v2, 32);
    }

    void update_( unsigned char const * p )
    {
        std::uint64_t m = detail::read64le( p );

        v3 ^= m;

        sipround();
        sipround();

        v0 ^= m;
    }

    void init( std::uint64_t k0, std::uint64_t k1 )
    {
        v0 = 0x736f6d6570736575ULL;
        v1 = 0x646f72616e646f6dULL;
        v2 = 0x6c7967656e657261ULL;
        v3 = 0x7465646279746573ULL;

        v3 ^= k1;
        v2 ^= k0;
        v1 ^= k1;
        v0 ^= k0;
    }

public:

    typedef std::uint64_t result_type;
    typedef std::uint64_t size_type;

    siphash_64(): m_( 0 ), n_( 0 )
    {
        init( 0, 0 );
    }

    explicit siphash_64( std::uint64_t k0, std::uint64_t k1 = 0 ): m_( 0 ), n_( 0 )
    {
        init( k0, k1 );
    }

    siphash_64( unsigned char const * p, std::size_t n ): m_( 0 ), n_( 0 )
    {
        if( n == 0 )
        {
            init( 0, 0 );
        }
        else if( n <= 16 )
        {
            unsigned char q[ 16 ] = { 0 };
            std::memcpy( q, p, n );

            std::uint64_t k0 = detail::read64le( q + 0 );
            std::uint64_t k1 = detail::read64le( q + 8 );

            init( k0, k1 );
        }
        else
        {
            std::uint64_t k0 = detail::read64le( p + 0 );
            std::uint64_t k1 = detail::read64le( p + 8 );

            init( k0, k1 );

            p += 16;
            n -= 16;

            update( p, n );

            result();
        }
    }

    void update( void const * pv, std::size_t n )
    {
        unsigned char const* p = static_cast<unsigned char const*>( pv );

        BOOST_ASSERT( m_ == n_ % 8 );

        if( n == 0 ) return;

        n_ += n;

        if( m_ > 0 )
        {
            std::size_t k = 8 - m_;

            if( n < k )
            {
                k = n;
            }

            std::memcpy( buffer_ + m_, p, k );

            p += k;
            n -= k;
            m_ += k;

            if( m_ < 8 ) return;

            BOOST_ASSERT( m_ == 8 );

            update_( buffer_ );
            m_ = 0;

            // clear buffered plaintext
            std::memset( buffer_, 0, 8 );
        }

        BOOST_ASSERT( m_ == 0 );

        while( n >= 8 )
        {
            update_( p );

            p += 8;
            n -= 8;
        }

        BOOST_ASSERT( n < 8 );

        if( n > 0 )
        {
            std::memcpy( buffer_, p, n );
            m_ = n;
        }

        BOOST_ASSERT( m_ == n_ % 8 );
    }

    std::uint64_t result()
    {
        BOOST_ASSERT( m_ == n_ % 8 );

        std::memset( buffer_ + m_, 0, 8 - m_ );

        buffer_[ 7 ] = static_cast<unsigned char>( n_ & 0xFF );

        update_( buffer_ );

        v2 ^= 0xFF;

        sipround();
        sipround();
        sipround();
        sipround();

        n_ += 8 - m_;
        m_ = 0;

        // clear buffered plaintext
        std::memset( buffer_, 0, 8 );

        return v0 ^ v1 ^ v2 ^ v3;
    }
};

class siphash_32
{
private:

    std::uint32_t v0, v1, v2, v3;

    unsigned char buffer_[ 4 ];
    std::uint32_t m_; // == n_ % 4

    std::uint32_t n_;

private:

    void sipround()
    {
        v0 += v1;
        v1 = detail::rotl(v1, 5);
        v1 ^= v0;
        v0 = detail::rotl(v0, 16);
        v2 += v3;
        v3 = detail::rotl(v3, 8);
        v3 ^= v2;
        v0 += v3;
        v3 = detail::rotl(v3, 7);
        v3 ^= v0;
        v2 += v1;
        v1 = detail::rotl(v1, 13);
        v1 ^= v2;
        v2 = detail::rotl(v2, 16);
    }

    void update_( unsigned char const * p )
    {
        std::uint32_t m = detail::read32le( p );

        v3 ^= m;

        sipround();
        sipround();

        v0 ^= m;
    }

    void init( std::uint32_t k0, std::uint32_t k1 )
    {
        v0 = 0;
        v1 = 0;
        v2 = 0x6c796765;
        v3 = 0x74656462;

        v3 ^= k1;
        v2 ^= k0;
        v1 ^= k1;
        v0 ^= k0;
    }

public:

    typedef std::uint32_t result_type;
    typedef std::uint32_t size_type;

    siphash_32(): m_( 0 ), n_( 0 )
    {
        init( 0, 0 );
    }

    explicit siphash_32( std::uint64_t seed ): m_( 0 ), n_( 0 )
    {
        std::uint32_t k0 = static_cast<std::uint32_t>( seed );
        std::uint32_t k1 = static_cast<std::uint32_t>( seed >> 32 );

        init( k0, k1 );
    }

    siphash_32( std::uint32_t k0, std::uint32_t k1 ): m_( 0 ), n_( 0 )
    {
        init( k0, k1 );
    }

    siphash_32( unsigned char const * p, std::size_t n ): m_( 0 ), n_( 0 )
    {
        if( n == 0 )
        {
            init( 0, 0 );
        }
        else if( n <= 8 )
        {
            unsigned char q[ 8 ] = { 0 };
            std::memcpy( q, p, n );

            std::uint32_t k0 = detail::read32le( q + 0 );
            std::uint32_t k1 = detail::read32le( q + 4 );

            init( k0, k1 );
        }
        else
        {
            std::uint32_t k0 = detail::read32le( p + 0 );
            std::uint32_t k1 = detail::read32le( p + 4 );

            init( k0, k1 );

            p += 8;
            n -= 8;

            update( p, n );

            result();
        }
    }

    void update( void const * pv, std::size_t n )
    {
        unsigned char const* p = static_cast<unsigned char const*>( pv );

        BOOST_ASSERT( m_ == n_ % 4 );

        if( n == 0 ) return;

        n_ += static_cast<std::uint32_t>( n );

        if( m_ > 0 )
        {
            std::uint32_t k = 4 - m_;

            if( n < k )
            {
                k = static_cast<std::uint32_t>( n );
            }

            std::memcpy( buffer_ + m_, p, k );

            p += k;
            n -= k;
            m_ += k;

            if( m_ < 4 ) return;

            BOOST_ASSERT( m_ == 4 );

            update_( buffer_ );
            m_ = 0;

            // clear buffered plaintext
            std::memset( buffer_, 0, 4 );
        }

        BOOST_ASSERT( m_ == 0 );

        while( n >= 4 )
        {
            update_( p );

            p += 4;
            n -= 4;
        }

        BOOST_ASSERT( n < 4 );

        if( n > 0 )
        {
            std::memcpy( buffer_, p, n );
            m_ = static_cast<std::uint32_t>( n );
        }

        BOOST_ASSERT( m_ == n_ % 4 );
    }

    std::uint32_t result()
    {
        BOOST_ASSERT( m_ == n_ % 4 );

        std::memset( buffer_ + m_, 0, 4 - m_ );

        buffer_[ 3 ] = static_cast<unsigned char>( n_ & 0xFF );

        update_( buffer_ );

        v2 ^= 0xFF;

        sipround();
        sipround();
        sipround();
        sipround();

        n_ += 4 - m_;
        m_ = 0;

        // clear buffered plaintext
        std::memset( buffer_, 0, 4 );

        return v1 ^ v3;
    }
};

} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_SIPHASH_HPP_INCLUDED
