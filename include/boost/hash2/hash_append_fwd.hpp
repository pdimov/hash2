#ifndef BOOST_HASH2_HASH_APPEND_FWD_HPP_INCLUDED
#define BOOST_HASH2_HASH_APPEND_FWD_HPP_INCLUDED

// Copyright 2024 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

namespace boost
{
namespace hash2
{

template<class Hash, class T> void hash_append( Hash& h, T const& v );
template<class Hash, class It> void hash_append_range( Hash& h, It first, It last );
template<class Hash, class T> void hash_append_size( Hash& h, T const& v );
template<class Hash, class It> void hash_append_sized_range( Hash& h, It first, It last );
template<class Hash, class It> void hash_append_unordered_range( Hash& h, It first, It last );

} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_HASH_APPEND_FWD_HPP_INCLUDED
