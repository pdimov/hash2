// Copyright 2024 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/fnv1a.hpp>
#include <boost/config.hpp>
#include <boost/config/pragma_message.hpp>

#if defined(BOOST_NO_CXX14_CONSTEXPR)

BOOST_PRAGMA_MESSAGE( "Test skipped, because BOOST_NO_CXX14_CONSTEXPR is defined" )
int main() {}

#elif defined(BOOST_GCC) && BOOST_GCC < 60000

BOOST_PRAGMA_MESSAGE( "Test skipped, because BOOST_GCC is < 60000" )
int main() {}

#else

#if defined(BOOST_MSVC) && BOOST_MSVC < 1920
# pragma warning(disable: 4307) // integral constant overflow
#endif

#define STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)

using namespace boost::hash2;

template<class T> BOOST_CXX14_CONSTEXPR std::uint32_t test()
{
    T v[ 95 ] = {};

    fnv1a_32 h;
    hash_append( h, {}, v );

    return h.result();
}

enum E: unsigned char {};

int main()
{
    constexpr std::uint32_t r = 409807047;

    STATIC_ASSERT( test<bool>() == r );
    STATIC_ASSERT( test<char>() == r );
    STATIC_ASSERT( test<signed char>() == r );
    STATIC_ASSERT( test<unsigned char>() == r );
    STATIC_ASSERT( test<E>() == r );

#if defined(__cpp_lib_byte) && __cpp_lib_byte >= 201603L
    STATIC_ASSERT( test<std::byte>() == r );
#endif

#if defined(__cpp_char8_t) && __cpp_char8_t >= 201811L
    STATIC_ASSERT( test<char8_t>() == r );
#endif
}

#endif
