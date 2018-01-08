#ifndef BOOST_HASH2_HASH_APPEND_HPP_INCLUDED
#define BOOST_HASH2_HASH_APPEND_HPP_INCLUDED

// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
//
// Based on
//
// Types Don't Know #
// Howard E. Hinnant, Vinnie Falco, John Bytheway
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3980.html

#include <boost/hash2/is_contiguously_hashable.hpp>
#include <boost/hash2/is_range.hpp>
#include <boost/hash2/is_contiguous_range.hpp>
#include <boost/hash2/is_unordered_range.hpp>
#include <boost/hash2/is_tuple_like.hpp>
#include <boost/hash2/byte_type.hpp>
#include <boost/hash2/get_integral_result.hpp>
#include <boost/core/enable_if.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/cstdint.hpp>
#include <boost/config.hpp>
#include <boost/config/workaround.hpp>
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES) && !defined(BOOST_NO_CXX11_HDR_TUPLE)
# include <boost/mp11/integer_sequence.hpp>
#endif
#include <iterator>

namespace boost
{
namespace hash2
{

// hash_append_range

template<class H, class It>
    typename boost::enable_if_c< 
        !boost::is_same<It, byte_type const*>::value, void >::type
    hash_append_range( H & h, It first, It last )
{
    for( ; first != last; ++first )
    {
        hash_append( h, *first );
    }
}

template<class H, class T>
    typename boost::enable_if_c< 
        is_contiguously_hashable<T, H>::value &&
        !boost::is_same<T, byte_type const>::value, void >::type
    hash_append_range( H & h, T * first, T * last )
{
    hash_append_range( h, reinterpret_cast<byte_type const*>( first ), reinterpret_cast<byte_type const*>( last ) );
}

template<class H, class T>
    typename boost::enable_if_c< 
        boost::is_same<T, byte_type const>::value, void >::type
    hash_append_range( H & h, T * first, T * last )
{
    h.update( first, last - first );
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

// hash_append

// contiguously hashable (this includes byte_type const&)

template<class H, class T>
    typename boost::enable_if_c< 
        is_contiguously_hashable<T, H>::value, void >::type
    hash_append( H & h, T const & v )
{
    byte_type const * p = reinterpret_cast<byte_type const*>( &v );
    hash_append_range( h, p, p + sizeof(T) );
}

// floating point

template<class H, class T>
    typename boost::enable_if_c< is_floating_point<T>::value, void >::type
    hash_append( H & h, T const & v )
{
    T w = v == 0? 0: v;

    byte_type const * p = reinterpret_cast<byte_type const*>( &w );
    hash_append_range( h, p, p + sizeof(T) );
}

// C arrays

template<class H, class T, std::size_t N> void hash_append( H & h, T const (&v)[ N ] )
{
    hash_append_range( h, v + 0, v + N );
}

// contiguous containers and ranges, w/ size

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1400)

// std::vector has no data()

namespace detail
{

template<class T> struct is_vector: false_type
{
};

template<class T, class A> struct is_vector< std::vector<T, A> >: true_type
{
};

template<class T, class A> struct is_vector< std::vector<T, A> const >: true_type
{
};

} // namespace detail

template<class H, class T>
    typename boost::enable_if_c< is_contiguous_range<T>::value && !is_tuple_like<T>::value && !detail::is_vector<T>::value, void >::type
    hash_append( H & h, T const & v )
{
    hash_append_range( h, v.data(), v.data() + v.size() );
    hash_append_size( h, v.size() );
}

template<class H, class T>
    typename boost::enable_if_c< detail::is_vector<T>::value, void >::type
    hash_append( H & h, T const & v )
{
    typename T::value_type const * p = v.empty()? 0: &v[0];

    hash_append_range( h, p, p + v.size() );
    hash_append_size( h, v.size() );
}

#else

template<class H, class T>
    typename boost::enable_if_c< is_contiguous_range<T>::value && !is_tuple_like<T>::value, void >::type
    hash_append( H & h, T const & v )
{
    hash_append_range( h, v.data(), v.data() + v.size() );
    hash_append_size( h, v.size() );
}

#endif

// containers and ranges, w/ size

template<class H, class T>
    typename boost::enable_if_c< is_range<T>::value && !is_tuple_like<T>::value && !is_contiguous_range<T>::value && !is_unordered_range<T>::value, void >::type
    hash_append( H & h, T const & v )
{
    hash_append_sized_range( h, v.begin(), v.end() );
}

// std::array (both range and tuple-like)

template<class H, class T>
    typename boost::enable_if_c< is_range<T>::value && is_tuple_like<T>::value, void >::type
    hash_append( H & h, T const & v )
{
    hash_append_range( h, v.begin(), v.end() );
}

// unordered containers (is_unordered_range implies is_range)

namespace detail
{

template<class H, class It> void hash_append_unordered_range_( H & h, It first, It last )
{
    typename std::iterator_traits<It>::difference_type m = 0;

    boost::uint64_t w = 0;

    for( ; first != last; ++first, ++m )
    {
        H h2( h );

        hash_append( h2, *first );

        w += get_integral_result<boost::uint64_t>( h2.result() );
    }

    hash_append( h, w );
    hash_append_size( h, m );
}

} // namespace detail

template<class H, class T>
    typename boost::enable_if_c< is_unordered_range<T>::value, void >::type
    hash_append( H & h, T const & v )
{
    detail::hash_append_unordered_range_( h, v.begin(), v.end() );
}

// tuple-likes

#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES) && !defined(BOOST_NO_CXX11_HDR_TUPLE)

namespace detail
{

template<class H, class T, std::size_t... J> void hash_append_tuple( H & h, T const& v, boost::mp11::integer_sequence<std::size_t, J...> )
{
    using std::get;
    int a[] = { ((void)hash_append( h, get<J>(v) ), 0)... };
    (void)a;
}

template<class H, class T> void hash_append_tuple( H & /*h*/, T const& /*v*/, boost::mp11::integer_sequence<std::size_t> )
{
}

} // namespace detail

template<class H, class T>
    typename boost::enable_if_c< !is_range<T>::value && is_tuple_like<T>::value, void >::type
    hash_append( H & h, T const & v )
{
    typedef boost::mp11::make_index_sequence<std::tuple_size<T>::value> seq;
    detail::hash_append_tuple( h, v, seq() );
}

#else // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES) && !defined(BOOST_NO_CXX11_HDR_TUPLE)

template<class H, class T1, class T2> void hash_append( H & h, std::pair<T1, T2> const & v )
{
    hash_append( h, v.first );
    hash_append( h, v.second );
}

#if !defined(BOOST_NO_CXX11_HDR_TUPLE)

} // namespace hash2
} // namespace boost

#include <tuple>

template<class H> void hash_append( H & /*h*/, std::tuple<> const & /*v*/ )
{
}

template<class H, class T1> void hash_append( H & h, std::tuple<T1> const & v )
{
    hash_append( h, std::get<0>(v) );
}

template<class H, class T1, class T2> void hash_append( H & h, std::tuple<T1, T2> const & v )
{
    hash_append( h, std::get<0>(v) );
    hash_append( h, std::get<1>(v) );
}

template<class H, class T1, class T2, class T3> void hash_append( H & h, std::tuple<T1, T2, T3> const & v )
{
    hash_append( h, std::get<0>(v) );
    hash_append( h, std::get<1>(v) );
    hash_append( h, std::get<2>(v) );
}

template<class H, class T1, class T2, class T3, class T4> void hash_append( H & h, std::tuple<T1, T2, T3, T4> const & v )
{
    hash_append( h, std::get<0>(v) );
    hash_append( h, std::get<1>(v) );
    hash_append( h, std::get<2>(v) );
    hash_append( h, std::get<3>(v) );
}

template<class H, class T1, class T2, class T3, class T4, class T5> void hash_append( H & h, std::tuple<T1, T2, T3, T4, T5> const & v )
{
    hash_append( h, std::get<0>(v) );
    hash_append( h, std::get<1>(v) );
    hash_append( h, std::get<2>(v) );
    hash_append( h, std::get<3>(v) );
    hash_append( h, std::get<4>(v) );
}

template<class H, class T1, class T2, class T3, class T4, class T5, class T6> void hash_append( H & h, std::tuple<T1, T2, T3, T4, T5, T6> const & v )
{
    hash_append( h, std::get<0>(v) );
    hash_append( h, std::get<1>(v) );
    hash_append( h, std::get<2>(v) );
    hash_append( h, std::get<3>(v) );
    hash_append( h, std::get<4>(v) );
    hash_append( h, std::get<5>(v) );
}

namespace boost
{
namespace hash2
{

#endif // !defined(BOOST_NO_CXX11_HDR_TUPLE)

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES) && !defined(BOOST_NO_CXX11_HDR_TUPLE)

} // namespace hash2
} // namespace boost

#endif
