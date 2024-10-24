#ifndef BOOST_HASH2_HASH_APPEND_HPP_INCLUDED
#define BOOST_HASH2_HASH_APPEND_HPP_INCLUDED

// Copyright 2017, 2018, 2023, 2024 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
//
// Based on
//
// Types Don't Know #
// Howard E. Hinnant, Vinnie Falco, John Bytheway
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3980.html

#include <boost/hash2/hash_append_fwd.hpp>
#include <boost/hash2/is_contiguously_hashable.hpp>
#include <boost/hash2/get_integral_result.hpp>
#include <boost/hash2/endian.hpp>
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

template<class T, std::size_t N> class array;

namespace hash2
{

struct default_flavor;

// hash_append_range

namespace detail
{

template<class Hash, class Flavor, class It> void hash_append_range_( Hash& h, Flavor const& f, It first, It last )
{
    for( ; first != last; ++first )
    {
        hash2::hash_append( h, f, *first );
    }
}

template<class Hash, class Flavor> void hash_append_range_( Hash& h, Flavor const& /*f*/, unsigned char* first, unsigned char* last )
{
    h.update( first, last - first );
}

template<class Hash, class Flavor> void hash_append_range_( Hash& h, Flavor const& /*f*/, unsigned char const* first, unsigned char const* last )
{
    h.update( first, last - first );
}

template<class Hash, class Flavor, class T>
    typename std::enable_if<
        is_contiguously_hashable<T, Flavor::byte_order>::value, void >::type
    hash_append_range_( Hash& h, Flavor const& /*f*/, T* first, T* last )
{
    h.update( first, (last - first) * sizeof(T) );
}

} // namespace detail

template<class Hash, class Flavor = default_flavor, class It> void hash_append_range( Hash& h, Flavor const& f, It first, It last )
{
    detail::hash_append_range_( h, f, first, last );
}

// hash_append_size

template<class Hash, class Flavor = default_flavor, class T> void hash_append_size( Hash& h, Flavor const& f, T const& v )
{
    hash2::hash_append( h, f, static_cast<typename Flavor::size_type>( v ) );
}

// hash_append_sized_range

namespace detail
{

template<class Hash, class Flavor, class It> void hash_append_sized_range_( Hash& h, Flavor const& f, It first, It last, std::input_iterator_tag )
{
    typename std::iterator_traits<It>::difference_type m = 0;

    for( ; first != last; ++first, ++m )
    {
        hash2::hash_append( h, f, *first );
    }

    hash2::hash_append_size( h, f, m );
}

template<class Hash, class Flavor, class It> void hash_append_sized_range_( Hash& h, Flavor const& f, It first, It last, std::random_access_iterator_tag )
{
    hash2::hash_append_range( h, f, first, last );
    hash2::hash_append_size( h, f, last - first );
}

} // namespace detail

template<class Hash, class Flavor = default_flavor, class It> void hash_append_sized_range( Hash& h, Flavor const& f, It first, It last )
{
    detail::hash_append_sized_range_( h, f, first, last, typename std::iterator_traits<It>::iterator_category() );
}

// hash_append_unordered_range

template<class Hash, class Flavor = default_flavor, class It> void hash_append_unordered_range( Hash& h, Flavor const& f, It first, It last )
{
    typename std::iterator_traits<It>::difference_type m = 0;

    std::uint64_t w = 0;

    for( ; first != last; ++first, ++m )
    {
        Hash h2( h );
        hash2::hash_append( h2, f, *first );

        w += hash2::get_integral_result<std::uint64_t>( h2.result() );
    }

    hash2::hash_append( h, f, w );
    hash2::hash_append_size( h, f, m );
}

// do_hash_append

// contiguously hashable (this includes unsigned char const&)

template<class Hash, class Flavor, class T>
    typename std::enable_if<
        is_contiguously_hashable<T, Flavor::byte_order>::value, void >::type
    do_hash_append( Hash& h, Flavor const& f, T const& v )
{
    unsigned char const * p = reinterpret_cast<unsigned char const*>( &v );
    hash2::hash_append_range( h, f, p, p + sizeof(T) );
}

// floating point

template<class Hash, class Flavor, class T>
    typename std::enable_if< std::is_floating_point<T>::value, void >::type
    do_hash_append( Hash& h, Flavor const& f, T const& v )
{
    T w = v == 0? 0: v;

    unsigned char const * p = reinterpret_cast<unsigned char const*>( &w );
    hash2::hash_append_range( h, f, p, p + sizeof(T) );
}

// C arrays

template<class Hash, class Flavor, class T, std::size_t N> void do_hash_append( Hash& h, Flavor const& f, T const (&v)[ N ] )
{
    hash2::hash_append_range( h, f, v + 0, v + N );
}

// contiguous containers and ranges, w/ size

template<class Hash, class Flavor, class T>
    typename std::enable_if< container_hash::is_contiguous_range<T>::value && !container_hash::is_tuple_like<T>::value, void >::type
    do_hash_append( Hash& h, Flavor const& f, T const& v )
{
    hash2::hash_append_range( h, f, v.data(), v.data() + v.size() );
    hash2::hash_append_size( h, f, v.size() );
}

// containers and ranges, w/ size

template<class Hash, class Flavor, class T>
    typename std::enable_if< container_hash::is_range<T>::value && !container_hash::is_tuple_like<T>::value && !container_hash::is_contiguous_range<T>::value && !container_hash::is_unordered_range<T>::value, void >::type
    do_hash_append( Hash& h, Flavor const& f, T const& v )
{
    hash2::hash_append_sized_range( h, f, v.begin(), v.end() );
}

// std::array (both range and tuple-like)

template<class Hash, class Flavor, class T>
    typename std::enable_if< container_hash::is_range<T>::value && container_hash::is_tuple_like<T>::value, void >::type
    do_hash_append( Hash& h, Flavor const& f, T const& v )
{
    hash2::hash_append_range( h, f, v.begin(), v.end() );
}

// boost::array (constant size, but not tuple-like)

template<class Hash, class Flavor, class T, std::size_t N> void do_hash_append( Hash& h, Flavor const& f, boost::array<T, N> const& v )
{
    hash2::hash_append_range( h, f, v.begin(), v.end() );
}

// unordered containers (is_unordered_range implies is_range)

template<class Hash, class Flavor, class T>
    typename std::enable_if< container_hash::is_unordered_range<T>::value, void >::type
    do_hash_append( Hash& h, Flavor const& f, T const& v )
{
    hash2::hash_append_unordered_range( h, f, v.begin(), v.end() );
}

// tuple-likes

namespace detail
{

template<class Hash, class Flavor, class T, std::size_t... J> void hash_append_tuple( Hash& h, Flavor const& f, T const& v, boost::mp11::integer_sequence<std::size_t, J...> )
{
    using std::get;
    int a[] = { 0, ((void)hash2::hash_append( h, f, get<J>(v) ), 0)... };
    (void)a;
}

} // namespace detail

template<class Hash, class Flavor, class T>
    typename std::enable_if< !container_hash::is_range<T>::value && container_hash::is_tuple_like<T>::value, void >::type
    do_hash_append( Hash& h, Flavor const& f, T const& v )
{
    typedef boost::mp11::make_index_sequence<std::tuple_size<T>::value> seq;
    detail::hash_append_tuple( h, f, v, seq() );
}

// flavors

struct default_flavor
{
    using size_type = std::uint32_t;
    static constexpr auto byte_order = endian::native;
};

// hash_append

template<class Hash, class Flavor = default_flavor, class T> void hash_append( Hash& h, Flavor const& f, T const& v )
{
    do_hash_append( h, f, v );
}

} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_HASH_APPEND_HPP_INCLUDED
