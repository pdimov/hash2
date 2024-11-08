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

namespace detail
{

/*BOOST_CXX14_CONSTEXPR*/ BOOST_FORCEINLINE std::uint64_t mulx( std::uint64_t x, std::uint64_t y ) noexcept
{
#if defined(_MSC_VER) && defined(_M_X64) && !defined(__clang__)

    std::uint64_t r2 = 0;
    std::uint64_t r = _umul128( x, y, &r2 );
    return r ^ r2;

#elif defined(__SIZEOF_INT128__)

    __uint128_t r = (__uint128_t)x * y;
    return (std::uint64_t)r ^ (std::uint64_t)( r >> 64 );

#else

    std::uint64_t x1 = (std::uint32_t)x;
    std::uint64_t x2 = x >> 32;

    std::uint64_t y1 = (std::uint32_t)y;
    std::uint64_t y2 = y >> 32;

    std::uint64_t r3 = x2 * y2;

    std::uint64_t r2a = x1 * y2;

    r3 += r2a >> 32;

    std::uint64_t r2b = x2 * y1;

    r3 += r2b >> 32;

    std::uint64_t r1 = x1 * y1;

    std::uint64_t r2 = (r1 >> 32) + (std::uint32_t)r2a + (std::uint32_t)r2b;

    r1 = (r2 << 32) + (std::uint32_t)r1;
    r3 += r2 >> 32;

    return r1 ^ r3;

#endif
}

BOOST_CXX14_CONSTEXPR BOOST_FORCEINLINE std::uint64_t mul32( std::uint32_t x, std::uint32_t y )
{
    return (std::uint64_t)x * y;
}

} // namespace detail

// mulxp1_64

class mulxp1_64
{
private:

    static constexpr std::uint64_t const Q = 0x9e3779b97f4a7c15ULL;
    static constexpr std::uint64_t const K = Q * Q;

    std::uint64_t w_;
    std::uint64_t h_;

    unsigned char buffer_[ 8 ] = {};
    std::size_t m_ = 0; // == n_ % 8

    std::uint64_t n_ = 0;

private:

    /*BOOST_CXX14_CONSTEXPR*/ void init( std::uint64_t seed ) noexcept
    {
        w_ = detail::mulx( seed + Q, K );
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
                std::uint64_t v1 = detail::read64le( p );

                w += Q;
                h1 ^= detail::mulx( v1 + w, K );

                p += 8;
                --k;
            }
            {
                std::uint64_t v1 = detail::read64le( p );

                w += Q;
                h2 ^= detail::mulx( v1 + w, K );

                p += 8;
                --k;
            }
            {
                std::uint64_t v1 = detail::read64le( p );

                w += Q;
                h3 ^= detail::mulx( v1 + w, K );

                p += 8;
                --k;
            }
        }

        std::uint64_t h = h_ ^ h1 ^ h2 ^ h3;

        while( k > 0 )
        {
            std::uint64_t v1 = detail::read64le( p );

            w += Q;
            h ^= detail::mulx( v1 + w, K );

            p += 8;
            --k;
        }

        w_ = w;
        h_ = h;
   }

public:

    typedef std::uint64_t result_type;

    /*BOOST_CXX14_CONSTEXPR*/ mulxp1_64() noexcept
    {
        init( 0 );
    }

    /*BOOST_CXX14_CONSTEXPR*/ explicit mulxp1_64( std::uint64_t seed ) noexcept
    {
        init( seed );
    }

    /*BOOST_CXX14_CONSTEXPR*/ mulxp1_64( unsigned char const * p, std::size_t n )
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

            detail::memcpy( buffer_ + m_, p, k );

            p += k;
            n -= k;
            m_ += k;

            if( m_ < 8 ) return;

            BOOST_ASSERT( m_ == 8 );

            update_( buffer_, 1 );
            m_ = 0;
        }

        BOOST_ASSERT( m_ == 0 );

        {
            std::size_t k = n / 8;

            update_( p, k );

            p += 8 * k;
            n -= 8 * k;
        }

        BOOST_ASSERT( n < 8 );

        if( n > 0 )
        {
            detail::memcpy( buffer_, p, n );
            m_ = n;
        }

        BOOST_ASSERT( m_ == n_ % 8 );
    }

    void update( void const* pv, std::size_t n )
    {
        unsigned char const* p = static_cast<unsigned char const*>( pv );
        update( p, n );
    }

    /*BOOST_CXX14_CONSTEXPR*/ std::uint64_t result()
    {
        BOOST_ASSERT( m_ == n_ % 8 );

        {
            unsigned char const* const p = buffer_;
            std::size_t const n = m_;

            std::uint64_t v1 = 0;

            if( n >= 4 )
            {
                v1 = (std::uint64_t)detail::read32le( p + n - 4 ) << ( n - 4 ) * 8 | detail::read32le( p );
            }
            else if( n >= 1 )
            {
                std::size_t const x1 = ( n - 1 ) & 2; // 1: 0, 2: 0, 3: 2
                std::size_t const x2 = n >> 1;        // 1: 0, 2: 1, 3: 1

                v1 = (std::uint64_t)p[ x1 ] << x1 * 8 | (std::uint64_t)p[ x2 ] << x2 * 8 | (std::uint64_t)p[ 0 ];
            }

            w_ += Q;
            h_ ^= detail::mulx( v1 + w_, K );
        }

        h_ ^= n_;

        n_ += 8 - m_;
        m_ = 0;

        // clear buffered plaintext
        detail::memset( buffer_, 0, 8 );

        return detail::mulx( h_ + w_, K );
    }
};

// mulxp1_32

class mulxp1_32
{
private:

    static constexpr std::uint32_t const Q = 0x9e3779b9U;
    static constexpr std::uint32_t const K = Q * Q;

    std::uint64_t h_;
    std::uint32_t w_;

    unsigned char buffer_[ 4 ] = {};
    std::size_t m_ = 0; // == n_ % 4

    std::size_t n_ = 0;

private:

    /*BOOST_CXX14_CONSTEXPR*/ void init( std::uint64_t seed )
    {
        h_ = ( seed + Q ) * K;
        w_ = static_cast<std::uint32_t>( h_ );
    }

    /*BOOST_CXX14_CONSTEXPR*/ void update_( unsigned char const * p, std::size_t k )
    {
        std::uint32_t w = w_;
        std::uint64_t h = h_;

        while( k > 0 )
        {
            std::uint32_t v1 = detail::read32le( p );

            w += Q;
            h ^= detail::mul32( v1 + w, K );

            p += 4;
            --k;
        }

        w_ = w;
        h_ = h;
    }

public:

    using result_type = std::uint32_t;

    /*BOOST_CXX14_CONSTEXPR*/ mulxp1_32() noexcept
    {
        init( 0 );
    }

    /*BOOST_CXX14_CONSTEXPR*/ explicit mulxp1_32( std::uint64_t seed ) noexcept
    {
        init( seed );
    }

    /*BOOST_CXX14_CONSTEXPR*/ mulxp1_32( unsigned char const * p, std::size_t n )
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
        BOOST_ASSERT( m_ == n_ % 4 );

        if( n == 0 ) return;

        n_ += n;

        if( m_ > 0 )
        {
            std::size_t k = 4 - m_;

            if( n < k )
            {
                k = n;
            }

            detail::memcpy( buffer_ + m_, p, k );

            p += k;
            n -= k;
            m_ += k;

            if( m_ < 4 ) return;

            BOOST_ASSERT( m_ == 4 );

            update_( buffer_, 1 );
            m_ = 0;
        }

        BOOST_ASSERT( m_ == 0 );

        {
            std::size_t k = n / 4;

            update_( p, k );

            p += 4 * k;
            n -= 4 * k;
        }

        BOOST_ASSERT( n < 4 );

        if( n > 0 )
        {
            detail::memcpy( buffer_, p, n );
            m_ = n;
        }

        BOOST_ASSERT( m_ == n_ % 4 );
    }

    void update( void const* pv, std::size_t n )
    {
        unsigned char const* p = static_cast<unsigned char const*>( pv );
        update( p, n );
    }

    /*BOOST_CXX14_CONSTEXPR*/ std::uint32_t result()
    {
        BOOST_ASSERT( m_ == n_ % 4 );

        {
            unsigned char const* const p = buffer_;
            std::size_t const n = m_;

            std::uint32_t v1 = 0;

            if( n >= 1 )
            {
                std::size_t const x1 = ( n - 1 ) & 2; // 1: 0, 2: 0, 3: 2
                std::size_t const x2 = n >> 1;        // 1: 0, 2: 1, 3: 1

                v1 = (std::uint32_t)p[ x1 ] << x1 * 8 | (std::uint32_t)p[ x2 ] << x2 * 8 | (std::uint32_t)p[ 0 ];
            }

            w_ += Q;
            h_ ^= detail::mul32( v1 + w_, K );
        }

        h_ ^= n_;

        w_ += Q;
        h_ ^= detail::mul32( (std::uint32_t)h_ + w_, (std::uint32_t)(h_ >> 32) + w_ + K );

        n_ += 4 - m_;
        m_ = 0;

        // clear buffered plaintext
        detail::memset( buffer_, 0, 4 );

        return (std::uint32_t)h_ ^ (std::uint32_t)(h_ >> 32);
    }
};

// mulxp3_64

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

    /*BOOST_CXX14_CONSTEXPR*/ void init( std::uint64_t seed ) noexcept
    {
        w_ = detail::mulx( seed + Q, K );
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
                h1 ^= detail::mulx( v1 + w, v2 + w + K );

                p += 16;
                --k;
            }
            {
                std::uint64_t v1 = detail::read64le( p + 0 );
                std::uint64_t v2 = detail::read64le( p + 8 );

                w += Q;
                h2 ^= detail::mulx( v1 + w, v2 + w + K );

                p += 16;
                --k;
            }
            {
                std::uint64_t v1 = detail::read64le( p + 0 );
                std::uint64_t v2 = detail::read64le( p + 8 );

                w += Q;
                h3 ^= detail::mulx( v1 + w, v2 + w + K );

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
            h ^= detail::mulx( v1 + w, v2 + w + K );

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
            h_ ^= detail::mulx( v1 + w_, v2 + w_ + K );
        }

        h_ ^= n_;

        n_ += 16 - m_;
        m_ = 0;

        // clear buffered plaintext
        detail::memset( buffer_, 0, 16 );

        return detail::mulx( h_, K );
    }
};

// mulxp3_32

class mulxp3_32
{
private:

    static constexpr std::uint32_t const Q = 0x9e3779b9U;
    static constexpr std::uint32_t const K = Q * Q;

    std::uint64_t h_;
    std::uint32_t w_;

    unsigned char buffer_[ 8 ] = {};
    std::size_t m_ = 0; // == n_ % 8

    std::size_t n_ = 0;

private:

    /*BOOST_CXX14_CONSTEXPR*/ void init( std::uint64_t seed )
    {
        h_ = ( seed + Q ) * K;
        w_ = static_cast<std::uint32_t>( h_ );
    }

    /*BOOST_CXX14_CONSTEXPR*/ void update_( unsigned char const * p, std::size_t k )
    {
        std::uint32_t w = w_;
        std::uint64_t h = h_;

        while( k > 0 )
        {
            std::uint32_t v1 = detail::read32le( p + 0 );
            std::uint32_t v2 = detail::read32le( p + 4 );

            w += Q;
            h ^= detail::mul32( v1 + w, v2 + w + K );

            p += 8;
            --k;
        }

        w_ = w;
        h_ = h;
    }

public:

    using result_type = std::uint32_t;

    /*BOOST_CXX14_CONSTEXPR*/ mulxp3_32() noexcept
    {
        init( 0 );
    }

    /*BOOST_CXX14_CONSTEXPR*/ explicit mulxp3_32( std::uint64_t seed ) noexcept
    {
        init( seed );
    }

    /*BOOST_CXX14_CONSTEXPR*/ mulxp3_32( unsigned char const * p, std::size_t n )
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

            detail::memcpy( buffer_ + m_, p, k );

            p += k;
            n -= k;
            m_ += k;

            if( m_ < 8 ) return;

            BOOST_ASSERT( m_ == 8 );

            update_( buffer_, 1 );
            m_ = 0;
        }

        BOOST_ASSERT( m_ == 0 );

        {
            std::size_t k = n / 8;

            update_( p, k );

            p += 8 * k;
            n -= 8 * k;
        }

        BOOST_ASSERT( n < 8 );

        if( n > 0 )
        {
            detail::memcpy( buffer_, p, n );
            m_ = n;
        }

        BOOST_ASSERT( m_ == n_ % 8 );
    }

    void update( void const* pv, std::size_t n )
    {
        unsigned char const* p = static_cast<unsigned char const*>( pv );
        update( p, n );
    }

    /*BOOST_CXX14_CONSTEXPR*/ std::uint32_t result()
    {
        BOOST_ASSERT( m_ == n_ % 8 );

        {
            unsigned char const* const p = buffer_;
            std::size_t const n = m_;

            std::uint32_t v1 = 0;
            std::uint32_t v2 = 0;

            if( n >= 4 )
            {
                v1 = detail::read32le( p );
                v2 = ((std::uint64_t)detail::read32le( p + n - 4 ) << ( n - 4 ) * 8) >> 32;
            }
            else if( n >= 1 )
            {
                std::size_t const x1 = ( n - 1 ) & 2; // 1: 0, 2: 0, 3: 2
                std::size_t const x2 = n >> 1;        // 1: 0, 2: 1, 3: 1

                v1 = (std::uint32_t)p[ x1 ] << x1 * 8 | (std::uint32_t)p[ x2 ] << x2 * 8 | (std::uint32_t)p[ 0 ];
            }

            w_ += Q;
            h_ ^= detail::mul32( v1 + w_, v2 + w_ + K );
        }

        h_ ^= n_;

        w_ += Q;
        h_ ^= detail::mul32( (std::uint32_t)h_ + w_, (std::uint32_t)(h_ >> 32) + w_ + K );

        n_ += 8 - m_;
        m_ = 0;

        // clear buffered plaintext
        detail::memset( buffer_, 0, 8 );

        return (std::uint32_t)h_ ^ (std::uint32_t)(h_ >> 32);
    }
};

} // namespace hash2
} // namespace boost

#if defined(BOOST_MSVC) && BOOST_MSVC < 1920
# pragma warning(pop)
#endif

#endif // #ifndef BOOST_HASH2_EXPERIMENTAL_MULXP_HPP_INCLUDED
