#ifndef BOOST_HASH2_EXPERIMENTAL_MULXP_HPP_INCLUDED
#define BOOST_HASH2_EXPERIMENTAL_MULXP_HPP_INCLUDED

// Copyright 2017-2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/detail/read.hpp>
#include <boost/hash2/detail/rot.hpp>
#include <boost/hash2/detail/memset.hpp>
#include <boost/hash2/detail/memcpy.hpp>
#include <boost/assert.hpp>
#include <boost/config.hpp>
#include <cstdint>
#include <cstring>
#include <cstddef>

#if defined(BOOST_MSVC) && BOOST_MSVC < 1920
# pragma warning(push)
# pragma warning(disable: 4307) // '+': integral constant overflow
#endif

namespace boost
{
namespace hash2
{

/*
class mulxp3_32
{
private:

    std::uint32_t v1_ = P1 + P2;
    std::uint32_t v2_ = P2;
    std::uint32_t v3_ = 0;
    std::uint32_t v4_ = static_cast<std::uint32_t>( 0 ) - P1;

    unsigned char buffer_[ 16 ] = {};
    std::size_t m_ = 0; // == n_ % 16

    std::size_t n_ = 0;

private:

    BOOST_CXX14_CONSTEXPR void init( std::uint32_t seed )
    {
        v1_ = seed + P1 + P2;
        v2_ = seed + P2;
        v3_ = seed;
        v4_ = seed - P1;
    }

    BOOST_CXX14_CONSTEXPR static std::uint32_t round( std::uint32_t seed, std::uint32_t input )
    {
        seed += input * P2;
        seed = detail::rotl( seed, 13 );
        seed *= P1;
        return seed;
    }

    BOOST_CXX14_CONSTEXPR void update_( unsigned char const * p, std::size_t k )
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

public:

    using result_type = std::uint32_t;

    xxhash_32() = default;

    BOOST_CXX14_CONSTEXPR explicit xxhash_32( std::uint64_t seed )
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

    BOOST_CXX14_CONSTEXPR xxhash_32( unsigned char const * p, std::size_t n )
    {
        if( n != 0 )
        {
            update( p, n );
            result();
        }
    }

    BOOST_CXX14_CONSTEXPR void update( unsigned char const* p, std::size_t n )
    {
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

            detail::memcpy( buffer_ + m_, p, k );

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
            detail::memcpy( buffer_, p, n );
            m_ = n;
        }

        BOOST_ASSERT( m_ == n_ % 16 );
    }

    void update( void const* pv, std::size_t n )
    {
        unsigned char const* p = static_cast<unsigned char const*>( pv );
        update( p, n );
    }

    BOOST_CXX14_CONSTEXPR std::uint32_t result()
    {
        BOOST_ASSERT( m_ == n_ % 16 );

        std::uint32_t h = 0;

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
        detail::memset( buffer_, 0, 16 );

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
*/

class mulxp3_64
{
private:

	static constexpr std::uint64_t const Q = 0x9e3779b97f4a7c15ULL;
	static constexpr std::uint64_t const K = Q * Q;

    std::uint64_t w_;
    std::uint64_t h_;

    unsigned char buffer_[ 16 ] = {};
    std::size_t m_ = 0; // == n_ % 16

    std::uint64_t n_ = 0;

private:

    static /*BOOST_CXX14_CONSTEXPR*/ std::uint64_t mulx( std::uint64_t x, std::uint64_t y ) noexcept
    {
#if defined(_MSC_VER) && defined(_M_X64) && !defined(__clang__)

        std::uint64_t r2 = 0;
        std::uint64_t r = _umul128( x, y, &r2 );
        return r ^ r2;

#else

        __uint128_t r = (__uint128_t)x * y;
        return (std::uint64_t)r ^ (std::uint64_t)( r >> 64 );

#endif
    }

    /*BOOST_CXX14_CONSTEXPR*/ void init( std::uint64_t seed ) noexcept
    {
		w_ = mulx( seed + Q, K );
		h_ = w_;
    }

    /*BOOST_CXX14_CONSTEXPR*/ void update_( unsigned char const* p, std::size_t k ) noexcept
    {
		std::uint64_t w = w_;

		std::uint64_t h1 = 0;
		std::uint64_t h2 = 0;
		std::uint64_t h3 = 0;

		while( k >= 3 )
		{
			{
				std::uint64_t v1 = detail::read64le( p + 0 );
				std::uint64_t v2 = detail::read64le( p + 8 );

				w += Q;
				h1 ^= mulx( v1 + w, v2 + w + K );

				p += 16;
				--k;
			}
			{
				std::uint64_t v1 = detail::read64le( p + 0 );
				std::uint64_t v2 = detail::read64le( p + 8 );

				w += Q;
				h2 ^= mulx( v1 + w, v2 + w + K );

				p += 16;
				--k;
			}
			{
				std::uint64_t v1 = detail::read64le( p + 0 );
				std::uint64_t v2 = detail::read64le( p + 8 );

				w += Q;
				h3 ^= mulx( v1 + w, v2 + w + K );

				p += 16;
				--k;
			}
		}

		std::uint64_t h = h_ ^ h1 ^ h2 ^ h3;

		while( k > 0 )
		{
			std::uint64_t v1 = detail::read64le( p + 0 );
			std::uint64_t v2 = detail::read64le( p + 8 );

			w += Q;
			h ^= mulx( v1 + w, v2 + w + K );

			p += 16;
			--k;
		}

		w_ = w;
		h_ = h;
   }

public:

    typedef std::uint64_t result_type;

    /*BOOST_CXX14_CONSTEXPR*/ mulxp3_64() noexcept
    {
        init( 0 );
    }

    /*BOOST_CXX14_CONSTEXPR*/ explicit mulxp3_64( std::uint64_t seed ) noexcept
    {
        init( seed );
    }

    /*BOOST_CXX14_CONSTEXPR*/ mulxp3_64( unsigned char const * p, std::size_t n )
    {
		init( 0 );

        if( n != 0 )
        {
            update( p, n );
            result();
        }
    }

    /*BOOST_CXX14_CONSTEXPR*/ void update( unsigned char const* p, std::size_t n )
    {
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

            detail::memcpy( buffer_ + m_, p, k );

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
            detail::memcpy( buffer_, p, n );
            m_ = n;
        }

        BOOST_ASSERT( m_ == n_ % 16 );
    }

    void update( void const* pv, std::size_t n )
    {
        unsigned char const* p = static_cast<unsigned char const*>( pv );
        update( p, n );
    }

    /*BOOST_CXX14_CONSTEXPR*/ std::uint64_t result()
    {
        BOOST_ASSERT( m_ == n_ % 16 );

		{
			unsigned char const* const p = buffer_;
			std::size_t const n = m_;

			std::uint64_t v1 = 0;
			std::uint64_t v2 = 0;

			if( n > 8 )
			{
				v1 = detail::read64le( p );
				v2 = detail::read64le( p + n - 8 ) >> ( 16 - n ) * 8;
			}
			else if( n_ >= 4 )
			{
				v1 = (std::uint64_t)detail::read32le( p + n - 4 ) << ( n - 4 ) * 8 | detail::read32le( p );
			}
			else if( n_ >= 1 )
			{
				std::size_t const x1 = ( n - 1 ) & 2; // 1: 0, 2: 0, 3: 2
				std::size_t const x2 = n >> 1;        // 1: 0, 2: 1, 3: 1

				v1 = (std::uint64_t)p[ x1 ] << x1 * 8 | (std::uint64_t)p[ x2 ] << x2 * 8 | (std::uint64_t)p[ 0 ];
			}

			w_ += Q;
			h_ ^= mulx( v1 + w_, v2 + w_ + K );
		}

		h_ ^= n_;

        n_ += 16 - m_;
        m_ = 0;

        // clear buffered plaintext
        detail::memset( buffer_, 0, 16 );

        return mulx( h_, K );
    }
};

} // namespace hash2
} // namespace boost

#if defined(BOOST_MSVC) && BOOST_MSVC < 1920
# pragma warning(pop)
#endif

#endif // #ifndef BOOST_HASH2_EXPERIMENTAL_MULXP_HPP_INCLUDED
