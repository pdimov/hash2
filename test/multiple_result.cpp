// Copyright 2017, 2018 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/fnv1a.hpp>
#include <boost/hash2/siphash.hpp>
#include <boost/hash2/xxhash.hpp>
#include <boost/hash2/md5.hpp>
#include <boost/hash2/sha1.hpp>
#include <boost/hash2/sha2.hpp>
#include <boost/hash2/ripemd.hpp>
#include <boost/core/lightweight_test.hpp>
#include <algorithm>
#include <cstddef>

template<class H> void test()
{
    H h;

    typename H::result_type r1 = h.result();

    typename H::result_type r2 = h.result();
    BOOST_TEST( r1 != r2 );

    typename H::result_type r3 = h.result();
    BOOST_TEST( r2 != r3 );

    typename H::result_type r4 = h.result();
    BOOST_TEST( r3 != r4 );
}

int main()
{
    test<boost::hash2::fnv1a_32>();
    test<boost::hash2::fnv1a_64>();
    test<boost::hash2::xxhash_32>();
    test<boost::hash2::xxhash_64>();
    test<boost::hash2::siphash_32>();
    test<boost::hash2::siphash_64>();
    test<boost::hash2::md5_128>();
    test<boost::hash2::sha1_160>();
    test<boost::hash2::sha2_256>();
    test<boost::hash2::sha2_224>();
    test<boost::hash2::sha2_512>();
    test<boost::hash2::sha2_384>();
    test<boost::hash2::sha2_512_224>();
    test<boost::hash2::sha2_512_256>();
    test<boost::hash2::hmac_md5_128>();
    test<boost::hash2::hmac_sha1_160>();
    test<boost::hash2::hmac_sha2_256>();
    test<boost::hash2::hmac_sha2_224>();
    test<boost::hash2::hmac_sha2_512>();
    test<boost::hash2::hmac_sha2_384>();
    test<boost::hash2::hmac_sha2_512_224>();
    test<boost::hash2::hmac_sha2_512_256>();
    test<boost::hash2::ripemd_160>();

    return boost::report_errors();
}
