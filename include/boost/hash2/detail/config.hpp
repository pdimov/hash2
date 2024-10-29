#ifndef BOOST_HASH2_DETAIL_CONFIG_HPP_INCLUDED
#define BOOST_HASH2_DETAIL_CONFIG_HPP_INCLUDED

// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/config.hpp>

#if defined(BOOST_MSVC) && BOOST_MSVC >= 1925
# define BOOST_HASH2_HAS_BUILTIN_IS_CONSTANT_EVALUATED
#endif

#if defined(__has_builtin)
# if __has_builtin(__builtin_is_constant_evaluated)
#  define BOOST_HASH2_HAS_BUILTIN_IS_CONSTANT_EVALUATED
# endif
#endif

#if !defined(BOOST_NO_CXX14_CONSTEXPR) && defined(BOOST_HASH2_HAS_BUILTIN_IS_CONSTANT_EVALUATED)

# define BOOST_HASH2_HAS_CXX14_CONSTEXPR
# define BOOST_HASH2_CXX14_CONSTEXPR constexpr

#else

# define BOOST_HASH2_CXX14_CONSTEXPR

#endif

#endif // #ifndef BOOST_HASH2_DETAIL_CONFIG_HPP_INCLUDED
