// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/fnv1a.hpp>
#include <boost/hash2/siphash.hpp>
#include <boost/hash2/xxhash.hpp>
#include <boost/hash2/spooky2.hpp>
#include <boost/hash2/murmur3.hpp>
#include <boost/hash2/md5.hpp>
#include <boost/hash2/sha1.hpp>
#include <boost/hash2/get_integral_result.hpp>
#include <boost/core/lightweight_test.hpp>
#include <cstdint>
#include <algorithm>
#include <cstddef>

using boost::hash2::get_integral_result;

template<class H> void test()
{
    H h;

    get_integral_result<int>( h.result() );
    get_integral_result<std::uint32_t>( h.result() );
    get_integral_result<std::uint64_t>( h.result() );
}

int main()
{
    test<boost::hash2::fnv1a_32>();
    test<boost::hash2::fnv1a_64>();
    test<boost::hash2::murmur3_32>();
    test<boost::hash2::murmur3_128>();
    test<boost::hash2::xxhash_32>();
    test<boost::hash2::xxhash_64>();
    test<boost::hash2::spooky2_128>();
    test<boost::hash2::siphash_32>();
    test<boost::hash2::siphash_64>();
    test<boost::hash2::md5_128>();
    test<boost::hash2::sha1_160>();
    test<boost::hash2::hmac_md5_128>();
    test<boost::hash2::hmac_sha1_160>();

    return boost::report_errors();
}
