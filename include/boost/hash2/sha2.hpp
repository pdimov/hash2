#ifndef SHA2_HPP_INCLUDED
#define SHA2_HPP_INCLUDED

// Copyright 2024 Peter Dimov.
// Copyright 2024 Christian Mazakas.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
//
// SHA2 message digest algorithm, https://csrc.nist.gov/pubs/fips/180-4/upd1/final, https://www.rfc-editor.org/rfc/rfc6234

#include <boost/hash2/detail/read.hpp>
#include <boost/hash2/detail/rot.hpp>
#include <boost/hash2/detail/write.hpp>
#include <boost/assert.hpp>
#include <boost/cstdint.hpp>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace boost
{
namespace hash2
{

class sha2_256
{
private:

    std::uint32_t state_[ 8 ];

    static const int N = 64;

    unsigned char buffer_[ N ];
    std::size_t m_; // == n_ % N

    std::uint64_t n_;

private:

    void init()
    {
        state_[ 0 ] = 0x6a09e667;
        state_[ 1 ] = 0xbb67ae85;
        state_[ 2 ] = 0x3c6ef372;
        state_[ 3 ] = 0xa54ff53a;
        state_[ 4 ] = 0x510e527f;
        state_[ 5 ] = 0x9b05688c;
        state_[ 6 ] = 0x1f83d9ab;
        state_[ 7 ] = 0x5be0cd19;
    }

    static std::uint32_t Sigma0( std::uint32_t x ) noexcept
    {
        return detail::rotr( x, 2 ) ^ detail::rotr( x, 13 ) ^ detail::rotr( x, 22 );
    }

    static std::uint32_t Sigma1( std::uint32_t x ) noexcept
    {
        return detail::rotr( x, 6 ) ^ detail::rotr( x, 11 ) ^ detail::rotr( x, 25 );
    }

    static std::uint32_t sigma0( std::uint32_t x ) noexcept
    {
        return detail::rotr( x, 7 ) ^ detail::rotr( x, 18 ) ^ ( x >> 3 );
    }

    static std::uint32_t sigma1( std::uint32_t x ) noexcept
    {
        return detail::rotr( x, 17 ) ^ detail::rotr( x, 19 ) ^ ( x >> 10 );
    }

    static std::uint32_t Ch( std::uint32_t x, std::uint32_t y, std::uint32_t z ) noexcept
    {
        return ( x & y ) ^ ( ~x & z );
    }

    static std::uint32_t Maj( std::uint32_t x, std::uint32_t y, std::uint32_t z ) noexcept
    {
        return ( x & y ) ^ ( x & z ) ^ ( y & z );
    }

    void transform( unsigned char const block[ 64 ] )
    {
        constexpr static std::uint32_t const K[ 64 ] =
        {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
            0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
            0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
            0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
            0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
            0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
            0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
        };

        std::uint32_t W[ 64 ];

        for( int t = 0; t < 16; ++t )
        {
            W[ t ] = detail::read32be( block + t * 4 );
        }

        for( int t = 16; t < 64; ++t )
        {
            W[ t ] = ( sigma1( W[ t - 2 ] ) + W[ t - 7] + sigma0( W[ t - 15 ] ) + W[ t - 16 ] );
        }

        std::uint32_t a = state_[ 0 ];
        std::uint32_t b = state_[ 1 ];
        std::uint32_t c = state_[ 2 ];
        std::uint32_t d = state_[ 3 ];
        std::uint32_t e = state_[ 4 ];
        std::uint32_t f = state_[ 5 ];
        std::uint32_t g = state_[ 6 ];
        std::uint32_t h = state_[ 7 ];

        for( int t = 0; t < 64; ++t )
        {
            std::uint32_t T1 = h + Sigma1( e ) + Ch( e, f, g ) + K[ t ] + W[ t ];
            std::uint32_t T2 = Sigma0( a ) + Maj( a, b, c );

            h = g;
            g = f;
            f = e;
            e = (d + T1);
            d = c;
            c = b;
            b = a;
            a = (T1 + T2);
        }

        state_[0] += a;
        state_[1] += b;
        state_[2] += c;
        state_[3] += d;
        state_[4] += e;
        state_[5] += f;
        state_[6] += g;
        state_[7] += h;
    }

public:
    using result_type = std::array<unsigned char, 32>;

    sha2_256(): m_( 0 ), n_( 0 )
    {
        init();
    }

    void update( void const * pv, std::size_t n )
    {
        unsigned char const* p = static_cast<unsigned char const*>( pv );

        BOOST_ASSERT( m_ == n_ % N );

        n_ += n;

        if( m_ > 0 )
        {
            std::size_t k = N - m_;

            if( n < k )
            {
                k = n;
            }

            std::memcpy( buffer_ + m_, p, k );

            p += k;
            n -= k;
            m_ += k;

            if( m_ < N ) return;

            BOOST_ASSERT( m_ == N );

            transform( buffer_ );
            m_ = 0;

            std::memset( buffer_, 0, N );
        }

        BOOST_ASSERT( m_ == 0 );

        while( n >= N )
        {
            transform( p );

            p += N;
            n -= N;
        }

        BOOST_ASSERT( n < N );

        if( n > 0 )
        {
            std::memcpy( buffer_, p, n );
            m_ = n;
        }

        BOOST_ASSERT( m_ == n_ % N );
    }

    result_type result()
    {
        unsigned char bits[ 8 ];
        detail::write64be( bits, n_ * 8 );

        std::size_t k = m_ < 56 ? 56 - m_ : 64 + 56 - m_;
        unsigned char padding[ 64 ] = { 0x80 };

        update( padding, k );
        update( bits, 8 );
        BOOST_ASSERT( m_ == 0 );

        result_type digest;
        for( int i = 0; i < 8; ++i ) {
            detail::write32be( &digest[ i * 4 ], state_[ i ] );
        }

        return digest;
    }
};

} // namespace boost
} // namespace hash2

#endif // #ifndef SHA2_HPP_INCLUDED
