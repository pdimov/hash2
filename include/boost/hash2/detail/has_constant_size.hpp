#ifndef BOOST_HASH2_DETAIL_HAS_CONSTANT_SIZE_HPP_INCLUDED
#define BOOST_HASH2_DETAIL_HAS_CONSTANT_SIZE_HPP_INCLUDED

// Copyright 2023 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/mp11/integral.hpp>
#include <boost/mp11/utility.hpp>

namespace boost
{
namespace hash2
{
namespace detail
{

template<class T> using has_constant_size_impl = mp11::mp_size_t< T().size() >;
template<class T> using has_constant_size = mp11::mp_valid<has_constant_size_impl, T>;

} // namespace detail
} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_DETAIL_HAS_CONSTANT_SIZE_HPP_INCLUDED
