#ifndef BOOST_HASH2_GET_INTEGRAL_RESULT_HPP_INCLUDED
#define BOOST_HASH2_GET_INTEGRAL_RESULT_HPP_INCLUDED

// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hash2/byte_type.hpp>
#include <boost/hash2/detail/read.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/core/enable_if.hpp>
#include <boost/array.hpp>
#include <boost/static_assert.hpp>
#include <cstddef>

namespace boost
{
namespace hash2
{

template<class T, class R>
    typename boost::enable_if_c<boost::is_integral<R>::value && (sizeof(R) >= sizeof(T)), T>::type
    get_integral_result( R const & r )
{
    typedef typename boost::make_unsigned<T>::type U;
    return static_cast<T>( static_cast<U>( r ) );
}

template<class T, class R>
    typename boost::enable_if_c<boost::is_integral<R>::value && sizeof(R) == 4 && sizeof(T) == 8, T>::type
    get_integral_result( R const & r )
{
    typedef typename boost::make_unsigned<T>::type U;
    return static_cast<T>( ( static_cast<U>( r ) << 32 ) + r );
}

template<class T, std::size_t N>
    T get_integral_result( boost::array<byte_type, N> const & r )
{
    BOOST_STATIC_ASSERT( N >= 8 );
    return static_cast<T>( detail::read64le( &r[0] ) );
}

} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_GET_INTEGRAL_RESULT_HPP_INCLUDED
