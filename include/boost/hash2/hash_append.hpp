#ifndef BOOST_HASH2_HASH_APPEND_HPP_INCLUDED
#define BOOST_HASH2_HASH_APPEND_HPP_INCLUDED

// Copyright 2017, 2018, 2023 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
//
// Based on
//
// Types Don't Know #
// Howard E. Hinnant, Vinnie Falco, John Bytheway
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3980.html

#include <boost/hash2/is_contiguously_hashable.hpp>
#include <boost/hash2/get_integral_result.hpp>
#include <boost/container_hash/is_range.hpp>
#include <boost/container_hash/is_contiguous_range.hpp>
#include <boost/container_hash/is_unordered_range.hpp>
#include <boost/container_hash/is_tuple_like.hpp>
#include <boost/mp11/integer_sequence.hpp>
#include <cstdint>
#include <type_traits>
#include <iterator>

namespace boost
{
namespace hash2
{

// forward declarations

template<class H, class T> void hash_append( H & h, T const & v );
template<class H, class It> void hash_append_range( H & h, It first, It last );
template<class H, class T> void hash_append_size( H & h, T const & v );
template<class H, class It> void hash_append_sized_range( H & h, It first, It last );

// hash_append_range

namespace detail
{

template<class H, class It> void hash_append_range_( H & h, It first, It last )
{
    for( ; first != last; ++first )
    {
        hash_append( h, *first );
    }
}

template<class H> void hash_append_range_( H & h, unsigned char * first, unsigned char * last )
{
    h.update( first, last - first );
}

template<class H> void hash_append_range_( H & h, unsigned char const * first, unsigned char const * last )
{
    h.update( first, last - first );
}

template<class H, class T>
    typename std::enable_if<
        is_contiguously_hashable<T, endian::native>::value, void >::type
    hash_append_range_( H & h, T * first, T * last )
{
    unsigned char const * f2 = reinterpret_cast<unsigned char const*>( first );
    unsigned char const * l2 = reinterpret_cast<unsigned char const*>( last );

    h.update( f2, l2 - f2 );
}

} // namespace detail

template<class H, class It> void hash_append_range( H & h, It first, It last )
{
    detail::hash_append_range_( h, first, last );
}

// hash_append_size

template<class H, class T> void hash_append_size( H & h, T const & v )
{
    hash_append( h, static_cast<typename H::size_type>( v ) );
}

// hash_append_sized_range

namespace detail
{

template<class H, class It> void hash_append_sized_range_( H & h, It first, It last, std::input_iterator_tag )
{
    typename std::iterator_traits<It>::difference_type m = 0;

    for( ; first != last; ++first, ++m )
    {
        hash_append( h, *first );
    }

    hash_append_size( h, m );
}

template<class H, class It> void hash_append_sized_range_( H & h, It first, It last, std::random_access_iterator_tag )
{
    hash_append_range( h, first, last );
    hash_append_size( h, last - first );
}

} // namespace detail

template<class H, class It> void hash_append_sized_range( H & h, It first, It last )
{
    detail::hash_append_sized_range_( h, first, last, typename std::iterator_traits<It>::iterator_category() );
}

// do_hash_append

// contiguously hashable (this includes unsigned char const&)

template<class H, class T>
    typename std::enable_if<
        is_contiguously_hashable<T, endian::native>::value, void >::type
    do_hash_append( H & h, T const & v )
{
    unsigned char const * p = reinterpret_cast<unsigned char const*>( &v );
    hash_append_range( h, p, p + sizeof(T) );
}

// floating point

template<class H, class T>
    typename std::enable_if< std::is_floating_point<T>::value, void >::type
    do_hash_append( H & h, T const & v )
{
    T w = v == 0? 0: v;

    unsigned char const * p = reinterpret_cast<unsigned char const*>( &w );
    hash_append_range( h, p, p + sizeof(T) );
}

// C arrays

template<class H, class T, std::size_t N> void do_hash_append( H & h, T const (&v)[ N ] )
{
    hash_append_range( h, v + 0, v + N );
}

// contiguous containers and ranges, w/ size

template<class H, class T>
    typename std::enable_if< container_hash::is_contiguous_range<T>::value && !container_hash::is_tuple_like<T>::value, void >::type
    do_hash_append( H & h, T const & v )
{
    hash_append_range( h, v.data(), v.data() + v.size() );
    hash_append_size( h, v.size() );
}

// containers and ranges, w/ size

template<class H, class T>
	typename std::enable_if< container_hash::is_range<T>::value && !container_hash::is_tuple_like<T>::value && !container_hash::is_contiguous_range<T>::value && !container_hash::is_unordered_range<T>::value, void >::type
    do_hash_append( H & h, T const & v )
{
    hash_append_sized_range( h, v.begin(), v.end() );
}

// std::array (both range and tuple-like)

template<class H, class T>
    typename std::enable_if< container_hash::is_range<T>::value && container_hash::is_tuple_like<T>::value, void >::type
    do_hash_append( H & h, T const & v )
{
    hash_append_range( h, v.begin(), v.end() );
}

// unordered containers (is_unordered_range implies is_range)

namespace detail
{

template<class H, class It> void hash_append_unordered_range_( H & h, It first, It last )
{
    typename std::iterator_traits<It>::difference_type m = 0;

    std::uint64_t w = 0;

    for( ; first != last; ++first, ++m )
    {
        H h2( h );

        hash_append( h2, *first );

        w += get_integral_result<std::uint64_t>( h2.result() );
    }

    hash_append( h, w );
    hash_append_size( h, m );
}

} // namespace detail

template<class H, class T>
    typename std::enable_if< container_hash::is_unordered_range<T>::value, void >::type
    do_hash_append( H & h, T const & v )
{
    detail::hash_append_unordered_range_( h, v.begin(), v.end() );
}

// tuple-likes

namespace detail
{

template<class H, class T, std::size_t... J> void hash_append_tuple( H & h, T const& v, boost::mp11::integer_sequence<std::size_t, J...> )
{
    using std::get;
    int a[] = { 0, ((void)hash_append( h, get<J>(v) ), 0)... };
    (void)a;
}

} // namespace detail

template<class H, class T>
    typename std::enable_if< !container_hash::is_range<T>::value && container_hash::is_tuple_like<T>::value, void >::type
    do_hash_append( H & h, T const & v )
{
    typedef boost::mp11::make_index_sequence<std::tuple_size<T>::value> seq;
    detail::hash_append_tuple( h, v, seq() );
}

// hash_append

template<class H, class T> void hash_append( H & h, T const & v )
{
    do_hash_append( h, v );
}

} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_HASH_APPEND_HPP_INCLUDED
