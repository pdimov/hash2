#ifndef BOOST_HASH2_XXHASH_HPP_INCLUDED
#define BOOST_HASH2_XXHASH_HPP_INCLUDED

// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
//
// xxHash, https://cyan4973.github.io/xxHash/

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

class xxhash_32
{
private:

    std::uint32_t v1_, v2_, v3_, v4_;

    unsigned char buffer_[ 16 ];
    std::size_t m_; // == n_ % 16

    std::size_t n_;

private:

    static const std::uint32_t P1 = 2654435761U;
    static const std::uint32_t P2 = 2246822519U;
    static const std::uint32_t P3 = 3266489917U;
    static const std::uint32_t P4 =  668265263U;
    static const std::uint32_t P5 =  374761393U;

private:

    static std::uint32_t round( std::uint32_t seed, std::uint32_t input )
    {
        seed += input * P2;
        seed = detail::rotl( seed, 13 );
        seed *= P1;
        return seed;
    }

    void update_( unsigned char const * p, std::size_t k )
    {
        std::uint32_t v1 = v1_;
        std::uint32_t v2 = v2_;
        std::uint32_t v3 = v3_;
        std::uint32_t v4 = v4_;

        for( std::size_t i = 0; i < k; ++i, p += 16 )
        {
            v1 = round( v1, detail::read32le( p +  0 ) );
            v2 = round( v2, detail::read32le( p +  4 ) );
            v3 = round( v3, detail::read32le( p +  8 ) );
            v4 = round( v4, detail::read32le( p + 12 ) );
        }

        v1_ = v1; 
        v2_ = v2; 
        v3_ = v3; 
        v4_ = v4; 
    }

    void init( std::uint32_t seed )
    {
        v1_ = seed + P1 + P2;
        v2_ = seed + P2;
        v3_ = seed;
        v4_ = seed - P1;
    }

public:

    typedef std::uint32_t result_type;
    typedef std::uint32_t size_type;

    xxhash_32(): m_( 0 ), n_( 0 )
    {
        init( 0 );
    }

    explicit xxhash_32( std::uint64_t seed ): m_( 0 ), n_( 0 )
    {
        std::uint32_t s0 = static_cast<std::uint32_t>( seed );
        std::uint32_t s1 = static_cast<std::uint32_t>( seed >> 32 );

        init( s0 );

        if( s1 != 0 )
        {
            v1_ = round( v1_, s1 );
            v2_ = round( v2_, s1 );
            v3_ = round( v3_, s1 );
            v4_ = round( v4_, s1 );
        }
    }

    xxhash_32( unsigned char const * p, std::size_t n ): m_( 0 ), n_( 0 )
    {
        init( 0 );

        if( n != 0 )
        {
            update( p, n );
            result();
        }
    }

    void update( void const * pv, std::size_t n )
    {
        unsigned char const* p = static_cast<unsigned char const*>( pv );

        BOOST_ASSERT( m_ == n_ % 16 );

        if( n == 0 ) return;

        n_ += n;

        if( m_ > 0 )
        {
            std::size_t k = 16 - m_;

            if( n < k )
            {
                k = n;
            }

            std::memcpy( buffer_ + m_, p, k );

            p += k;
            n -= k;
            m_ += k;

            if( m_ < 16 ) return;

            BOOST_ASSERT( m_ == 16 );

            update_( buffer_, 1 );
            m_ = 0;
        }

        BOOST_ASSERT( m_ == 0 );

        {
            std::size_t k = n / 16;

            update_( p, k );

            p += 16 * k;
            n -= 16 * k;
        }

        BOOST_ASSERT( n < 16 );

        if( n > 0 )
        {
            std::memcpy( buffer_, p, n );
            m_ = n;
        }

        BOOST_ASSERT( m_ == n_ % 16 );
    }

    std::uint32_t result()
    {
        BOOST_ASSERT( m_ == n_ % 16 );

        std::uint32_t h;

        if( n_ >= 16 )
        {
            h = detail::rotl( v1_, 1 ) + detail::rotl( v2_, 7 ) + detail::rotl( v3_, 12 ) + detail::rotl( v4_, 18 );
        }
        else
        {
            h = v3_ + P5;
        }

        h += static_cast<std::uint32_t>( n_ );

        unsigned char const * p = buffer_;

        std::size_t m = m_;

        while( m >= 4 )
        {
            h += detail::read32le( p ) * P3;
            h = detail::rotl( h, 17 ) * P4;

            p += 4;
            m -= 4;
        }

        while( m > 0 )
        {
            h += p[0] * P5;
            h = detail::rotl( h, 11 ) * P1;

            ++p;
            --m;
        }

        n_ += 16 - m_;
        m_ = 0;

        // clear buffered plaintext
        std::memset( buffer_, 0, 16 );

        // advance state
        v1_ = round( v1_, h );
        v2_ = round( v2_, h );
        v3_ = round( v3_, h );
        v4_ = round( v4_, h );

        // apply final mix
        h ^= h >> 15;
        h *= P2;
        h ^= h >> 13;
        h *= P3;
        h ^= h >> 16;

        return h;
    }
};

class xxhash_64
{
private:

    std::uint64_t v1_, v2_, v3_, v4_;

    unsigned char buffer_[ 32 ];
    std::size_t m_; // == n_ % 32

    std::uint64_t n_;

private:

    static const std::uint64_t P1 = 11400714785074694791ULL;
    static const std::uint64_t P2 = 14029467366897019727ULL;
    static const std::uint64_t P3 =  1609587929392839161ULL;
    static const std::uint64_t P4 =  9650029242287828579ULL;
    static const std::uint64_t P5 =  2870177450012600261ULL;

private:

    static std::uint64_t round( std::uint64_t seed, std::uint64_t input )
    {
        seed += input * P2;
        seed = detail::rotl( seed, 31 );
        seed *= P1;
        return seed;
    }

    static std::uint64_t merge_round( std::uint64_t acc, std::uint64_t val )
    {
        val = round( 0, val );
        acc ^= val;
        acc = acc * P1 + P4;
        return acc;
    }

    void update_( unsigned char const * p, std::size_t k )
    {
        std::uint64_t v1 = v1_;
        std::uint64_t v2 = v2_;
        std::uint64_t v3 = v3_;
        std::uint64_t v4 = v4_;

        for( std::size_t i = 0; i < k; ++i, p += 32 )
        {
            v1 = round( v1, detail::read64le( p +  0 ) );
            v2 = round( v2, detail::read64le( p +  8 ) );
            v3 = round( v3, detail::read64le( p + 16 ) );
            v4 = round( v4, detail::read64le( p + 24 ) );
        }

        v1_ = v1; 
        v2_ = v2; 
        v3_ = v3; 
        v4_ = v4; 
    }

    void init( std::uint64_t seed )
    {
        v1_ = seed + P1 + P2;
        v2_ = seed + P2;
        v3_ = seed;
        v4_ = seed - P1;
    }

public:

    typedef std::uint64_t result_type;
    typedef std::uint64_t size_type;

    xxhash_64(): m_( 0 ), n_( 0 )
    {
        init( 0 );
    }

    explicit xxhash_64( std::uint64_t seed ): m_( 0 ), n_( 0 )
    {
        init( seed );
    }

    xxhash_64( unsigned char const * p, std::size_t n ): m_( 0 ), n_( 0 )
    {
        init( 0 );

        if( n != 0 )
        {
            update( p, n );
            result();
        }
    }

    void update( void const * pv, std::size_t n )
    {
        unsigned char const* p = static_cast<unsigned char const*>( pv );

        BOOST_ASSERT( m_ == n_ % 32 );

        if( n == 0 ) return;

        n_ += n;

        if( m_ > 0 )
        {
            std::size_t k = 32 - m_;

            if( n < k )
            {
                k = n;
            }

            std::memcpy( buffer_ + m_, p, k );

            p += k;
            n -= k;
            m_ += k;

            if( m_ < 32 ) return;

            BOOST_ASSERT( m_ == 32 );

            update_( buffer_, 1 );
            m_ = 0;
        }

        BOOST_ASSERT( m_ == 0 );

        {
            std::size_t k = n / 32;

            update_( p, k );

            p += 32 * k;
            n -= 32 * k;
        }

        BOOST_ASSERT( n < 32 );

        if( n > 0 )
        {
            std::memcpy( buffer_, p, n );
            m_ = n;
        }

        BOOST_ASSERT( m_ == n_ % 32 );
    }

    std::uint64_t result()
    {
        BOOST_ASSERT( m_ == n_ % 32 );

        std::uint64_t h;

        if( n_ >= 32 )
        {
            h = detail::rotl( v1_, 1 ) + detail::rotl( v2_, 7 ) + detail::rotl( v3_, 12 ) + detail::rotl( v4_, 18 );

            h = merge_round( h, v1_ );
            h = merge_round( h, v2_ );
            h = merge_round( h, v3_ );
            h = merge_round( h, v4_ );
        }
        else
        {
            h = v3_ + P5;
        }

        h += n_;

        unsigned char const * p = buffer_;

        std::size_t m = m_;

        while( m >= 8 )
        {
            std::uint64_t k1 = round( 0, detail::read64le( p ) );

            h ^= k1;
            h = detail::rotl( h, 27 ) * P1 + P4;

            p += 8;
            m -= 8;
        }

        while( m >= 4 )
        {
            h ^= static_cast<std::uint64_t>( detail::read32le( p ) ) * P1;
            h = detail::rotl( h, 23 ) * P2 + P3;

            p += 4;
            m -= 4;
        }

        while( m > 0 )
        {
            h ^= p[0] * P5;
            h = detail::rotl( h, 11 ) * P1;

            ++p;
            --m;
        }

        n_ += 32 - m_;
        m_ = 0;

        // clear buffered plaintext
        std::memset( buffer_, 0, 32 );

        // advance state
        v1_ = round( v1_, h );
        v2_ = round( v2_, h );
        v3_ = round( v3_, h );
        v4_ = round( v4_, h );

        // apply final mix
        h ^= h >> 33;
        h *= P2;
        h ^= h >> 29;
        h *= P3;
        h ^= h >> 32;

        return h;
    }
};

} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_XXHASH_HPP_INCLUDED
