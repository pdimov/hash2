#ifndef BOOST_HASH2_IS_CONTIGUOUSLY_HASHABLE_HPP_INCLUDED
#define BOOST_HASH2_IS_CONTIGUOUSLY_HASHABLE_HPP_INCLUDED

// Copyright 2017 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hash2/is_uniquely_represented.hpp>

namespace boost
{
namespace hash2
{

template<class T, class H> struct is_contiguously_hashable:
    is_uniquely_represented<T>
{
};

} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_IS_CONTIGUOUSLY_HASHABLE_HPP_INCLUDED
