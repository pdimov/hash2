#ifndef BOOST_HASH2_XXHASH_HPP_INCLUDED
#define BOOST_HASH2_XXHASH_HPP_INCLUDED

// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
//
// xxHash, https://cyan4973.github.io/xxHash/

#include <boost/hash2/byte_type.hpp>
#include <boost/hash2/detail/read.hpp>
#include <boost/hash2/detail/rot.hpp>
#include <boost/cstdint.hpp>
#include <boost/assert.hpp>
#include <cstring>
#include <cstddef>

namespace boost
{
namespace hash2
{

class xxhash_32
{
private:

    boost::uint32_t v1_, v2_, v3_, v4_;

    byte_type buffer_[ 16 ];
    int m_;

    std::size_t n_;

private:

    static const boost::uint32_t P1 = 2654435761U;
    static const boost::uint32_t P2 = 2246822519U;
    static const boost::uint32_t P3 = 3266489917U;
    static const boost::uint32_t P4 =  668265263U;
    static const boost::uint32_t P5 =  374761393U;

private:

    static boost::uint32_t round( boost::uint32_t seed, boost::uint32_t input )
    {
        seed += input * P2;
        seed = detail::rotl( seed, 13 );
        seed *= P1;
        return seed;
    }

    void update_( byte_type const * p, std::ptrdiff_t k )
    {
        boost::uint32_t v1 = v1_;
        boost::uint32_t v2 = v2_;
        boost::uint32_t v3 = v3_;
        boost::uint32_t v4 = v4_;

        for( std::ptrdiff_t i = 0; i < k; ++i, p += 16 )
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

    void init( boost::uint32_t seed )
    {
        v1_ = seed + P1 + P2;
        v2_ = seed + P2;
        v3_ = seed;
        v4_ = seed - P1;
    }

public:

    typedef boost::uint32_t result_type;
    typedef boost::uint32_t size_type;

    explicit xxhash_32( boost::uint32_t seed = 0 ): m_( 0 ), n_( 0 )
    {
        init( seed );
    }

    explicit xxhash_32( byte_type const * p, std::ptrdiff_t n ): m_( 0 ), n_( 0 )
    {
        BOOST_ASSERT( n >= 0 );

        if( n <= 4 )
        {
            byte_type q[ 4 ] = {};
            std::memcpy( q, p, n );

            boost::uint32_t seed = detail::read32le( q );
            init( seed );
        }
        else
        {
            boost::uint32_t seed = detail::read32le( p );
            init( seed );

            p += 4;
            n -= 4;

            update( p, n );
            result();
        }
    }

    void update( byte_type const * p, std::ptrdiff_t n )
    {
        BOOST_ASSERT( n >= 0 );

        BOOST_ASSERT( m_ == static_cast<int>( n_ & 15 ) );

        if( n == 0 ) return;

        n_ += n;

        if( m_ > 0 )
        {
            int k = 16 - m_;

            if( n < k )
            {
                k = static_cast<int>( n );
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
            std::ptrdiff_t k = n / 16;

            update_( p, k );

            p += 16 * k;
            n -= 16 * k;
        }

        BOOST_ASSERT( n < 16 );

        if( n > 0 )
        {
            std::memcpy( buffer_, p, n );
            m_ = static_cast<int>( n );
        }

        BOOST_ASSERT( m_ == static_cast<int>( n_ & 15 ) );
    }

    boost::uint32_t result()
    {
        BOOST_ASSERT( m_ == static_cast<int>( n_ & 15 ) );

        boost::uint32_t h;

        if( n_ >= 16 )
        {
            h = detail::rotl( v1_, 1 ) + detail::rotl( v2_, 7 ) + detail::rotl( v3_, 12 ) + detail::rotl( v4_, 18 );
        }
        else
        {
            h = v3_ + P5;
        }

        h += static_cast<boost::uint32_t>( n_ );

        byte_type const * p = buffer_;

        int m = m_;

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

    boost::uint64_t v1_, v2_, v3_, v4_;

    byte_type buffer_[ 32 ];
    int m_;

    boost::uint64_t n_;

private:

    static const boost::uint64_t P1 = 11400714785074694791ULL;
    static const boost::uint64_t P2 = 14029467366897019727ULL;
    static const boost::uint64_t P3 =  1609587929392839161ULL;
    static const boost::uint64_t P4 =  9650029242287828579ULL;
    static const boost::uint64_t P5 =  2870177450012600261ULL;

private:

    static boost::uint64_t round( boost::uint64_t seed, boost::uint64_t input )
    {
        seed += input * P2;
        seed = detail::rotl( seed, 31 );
        seed *= P1;
        return seed;
    }

    static boost::uint64_t merge_round( boost::uint64_t acc, boost::uint64_t val )
    {
        val = round( 0, val );
        acc ^= val;
        acc = acc * P1 + P4;
        return acc;
    }

    void update_( byte_type const * p, std::ptrdiff_t k )
    {
        boost::uint64_t v1 = v1_;
        boost::uint64_t v2 = v2_;
        boost::uint64_t v3 = v3_;
        boost::uint64_t v4 = v4_;

        for( int i = 0; i < k; ++i, p += 32 )
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

    void init( boost::uint64_t seed )
    {
        v1_ = seed + P1 + P2;
        v2_ = seed + P2;
        v3_ = seed;
        v4_ = seed - P1;
    }

public:

    typedef boost::uint64_t result_type;
    typedef boost::uint64_t size_type;

    explicit xxhash_64( boost::uint64_t seed = 0 ): m_( 0 ), n_( 0 )
    {
        init( seed );
    }

    explicit xxhash_64( byte_type const * p, std::ptrdiff_t n ): m_( 0 ), n_( 0 )
    {
        BOOST_ASSERT( n >= 0 );

        if( n <= 8 )
        {
            byte_type q[ 8 ] = {};
            std::memcpy( q, p, n );

            boost::uint64_t seed = detail::read64le( q );
            init( seed );
        }
        else
        {
            boost::uint64_t seed = detail::read64le( p );
            init( seed );

            p += 8;
            n -= 8;

            update( p, n );
            result();
        }
    }

    void update( byte_type const * p, std::ptrdiff_t n )
    {
        BOOST_ASSERT( n >= 0 );

        BOOST_ASSERT( m_ == static_cast<int>( n_ & 31 ) );

        if( n == 0 ) return;

        n_ += n;

        if( m_ > 0 )
        {
            int k = 32 - m_;

            if( n < k )
            {
                k = static_cast<int>( n );
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
            std::ptrdiff_t k = n / 32;

            update_( p, k );

            p += 32 * k;
            n -= 32 * k;
        }

        BOOST_ASSERT( n < 32 );

        if( n > 0 )
        {
            std::memcpy( buffer_, p, n );
            m_ = static_cast<int>( n );
        }

        BOOST_ASSERT( m_ == static_cast<int>( n_ & 31 ) );
    }

    boost::uint64_t result()
    {
        BOOST_ASSERT( m_ == static_cast<int>( n_ & 31 ) );

        boost::uint64_t h;

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

        byte_type const * p = buffer_;

        int m = m_;

        while( m >= 8 )
        {
            boost::uint64_t k1 = round( 0, detail::read64le( p ) );

            h ^= k1;
            h = detail::rotl( h, 27 ) * P1 + P4;

            p += 8;
            m -= 8;
        }

        while( m >= 4 )
        {
            h ^= static_cast<boost::uint64_t>( detail::read32le( p ) ) * P1;
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
