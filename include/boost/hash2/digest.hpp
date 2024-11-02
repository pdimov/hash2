#ifndef BOOST_HASH2_DIGEST_HPP_INCLUDED
#define BOOST_HASH2_DIGEST_HPP_INCLUDED

// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/detail/memcpy.hpp>
#include <boost/hash2/detail/config.hpp>
#include <boost/assert.hpp>
#include <boost/config.hpp>
#include <string>
#include <iosfwd>
#include <cstddef>

namespace boost
{
namespace hash2
{

template<std::size_t N> class digest
{
private:

    unsigned char data_[ N ] = {};

public:

    // constructors

    digest() = default;

#if defined(BOOST_NO_CXX14_CONSTEXPR)

    digest( unsigned char const (&v)[ N ] ) noexcept
    {
        std::memcpy( data_, v, N );
    }

#elif defined(BOOST_HASH2_HAS_BUILTIN_IS_CONSTANT_EVALUATED)

    BOOST_CXX14_CONSTEXPR digest( unsigned char const (&v)[ N ] ) noexcept
    {
        detail::memcpy( data_, v, N );
    }

#else

    BOOST_CXX14_CONSTEXPR digest( unsigned char const (&v)[ N ] ) noexcept
    {
        for( std::size_t i = 0; i < N; ++i )
        {
            data_[ i ] = v[ i ];
        }
    }

#endif

    // iteration

    using value_type = unsigned char;
    using reference = unsigned char&;
    using const_reference = unsigned char const&;
    using iterator = unsigned char*;
    using const_iterator = unsigned char const*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    BOOST_CXX14_CONSTEXPR iterator begin() noexcept { return data_; }
    constexpr const_iterator begin() const noexcept { return data_; }

    BOOST_CXX14_CONSTEXPR iterator end() noexcept { return data_ + N; }
    constexpr const_iterator end() const noexcept { return data_ + N; }

    // data, size

    BOOST_CXX14_CONSTEXPR unsigned char* data() noexcept { return data_; }
    constexpr unsigned char const* data() const noexcept { return data_; }

    constexpr size_type size() const noexcept { return N; }
    constexpr size_type max_size() const noexcept { return N; }

    // element access

    BOOST_CXX14_CONSTEXPR reference operator[]( std::size_t i ) { BOOST_ASSERT( i < N ); return data_[ i ]; }
    BOOST_CXX14_CONSTEXPR const_reference operator[]( std::size_t i ) const { BOOST_ASSERT( i < N ); return data_[ i ]; }
};

// comparisons

#if defined(BOOST_NO_CXX14_CONSTEXPR)

template<std::size_t N> bool operator==( digest<N> const& a, digest<N> const& b ) noexcept
{
    return std::memcmp( a.data(), b.data(), N ) == 0;
}

#elif defined(BOOST_HASH2_HAS_BUILTIN_IS_CONSTANT_EVALUATED)

template<std::size_t N> BOOST_CXX14_CONSTEXPR bool operator==( digest<N> const& a, digest<N> const& b ) noexcept
{
    if( !detail::is_constant_evaluated() )
    {
        return std::memcmp( a.data(), b.data(), N ) == 0;
    }
    else
    {
        for( std::size_t i = 0; i < N; ++i )
        {
            if( a[ i ] != b[ i ] ) return false;
        }

        return true;
    }
}

#else

template<std::size_t N> BOOST_CXX14_CONSTEXPR bool operator==( digest<N> const& a, digest<N> const& b ) noexcept
{
    for( std::size_t i = 0; i < N; ++i )
    {
        if( a[ i ] != b[ i ] ) return false;
    }

    return true;
}

#endif

template<std::size_t N> BOOST_CXX14_CONSTEXPR bool operator!=( digest<N> const& a, digest<N> const& b ) noexcept
{
    return !( a == b );
}

// to_chars

template<std::size_t N> BOOST_CXX14_CONSTEXPR char* to_chars( digest<N> const& v, char* first, char* last ) noexcept
{
    if( last - first < static_cast<std::ptrdiff_t>( 2 * N ) )
    {
        return nullptr;
    }

    constexpr char digits[] = "0123456789abcdef";

    for( std::size_t i = 0; i < N; ++i )
    {
        first[ i*2 + 0 ] = digits[ v[i] >> 4 ];
        first[ i*2 + 1 ] = digits[ v[i] & 0x0F ];
    }

    return first + N * 2;
}

template<std::size_t N, std::size_t M> BOOST_CXX14_CONSTEXPR void to_chars( digest<N> const& v, char (&w)[ M ] ) noexcept
{
    static_assert( M >= 2 * N + 1, "Output buffer not large enough" );
    *to_chars( v, w, w + M ) = 0;
}

// operator<<

template<std::size_t N> std::ostream& operator<<( std::ostream& os, digest<N> const& v )
{
    char tmp[ 2*N+1 ];
    to_chars( v, tmp );

    os << tmp;
    return os;
}

// to_string

template<std::size_t N> std::string to_string( digest<N> const& v )
{
    char tmp[ 2*N+1 ];
    to_chars( v, tmp );

    return std::string( tmp, 2*N );
}

} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_DIGEST_HPP_INCLUDED
