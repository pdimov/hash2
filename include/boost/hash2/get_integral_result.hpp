#ifndef BOOST_HASH2_GET_INTEGRAL_RESULT_HPP_INCLUDED
#define BOOST_HASH2_GET_INTEGRAL_RESULT_HPP_INCLUDED

// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/detail/read.hpp>
#include <type_traits>
#include <limits>
#include <cstddef>

namespace boost
{
namespace hash2
{

template<class T, class R>
    typename std::enable_if<std::is_integral<R>::value && (sizeof(R) >= sizeof(T)), T>::type
    get_integral_result( R const & r )
{
    static_assert( std::is_integral<T>::value, "T must be integral" );
    static_assert( !std::is_same<typename std::remove_cv<T>::type, bool>::value, "T must not be bool" );

    static_assert( std::is_unsigned<R>::value, "R must be unsigned" );

    typedef typename std::make_unsigned<T>::type U;

    return static_cast<T>( static_cast<U>( r ) );
}

template<class T, class R>
    typename std::enable_if<std::is_integral<R>::value && (sizeof(R) < sizeof(T)), T>::type
    get_integral_result( R const & r )
{
    static_assert( std::is_integral<T>::value, "T must be integral" );
    static_assert( !std::is_same<typename std::remove_cv<T>::type, bool>::value, "T must not be bool" );

    static_assert( std::is_unsigned<R>::value, "R must be unsigned" );

    typedef typename std::make_unsigned<T>::type U;

    constexpr U m = std::numeric_limits<U>::max() / std::numeric_limits<R>::max();

    return static_cast<T>( r * m );
}

template<class T, class R>
    typename std::enable_if< !std::is_integral<R>::value, T >::type
    get_integral_result( R const & r )
{
    static_assert( std::is_integral<T>::value, "T must be integral" );
    static_assert( !std::is_same<typename std::remove_cv<T>::type, bool>::value, "T must not be bool" );

    static_assert( R().size() >= 8, "Array-like result type is too short" );

    return static_cast<T>( detail::read64le( r.data() ) );
}

} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_GET_INTEGRAL_RESULT_HPP_INCLUDED
