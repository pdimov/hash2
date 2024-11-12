// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/md5.hpp>
#include <iostream>

// xxd -i resource
constexpr unsigned char resource[] = {
  0x1f, 0x8b, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x03, 0x6d, 0x90,
  0xcf, 0x6e, 0x83, 0x30, 0x0c, 0xc6, 0xcf, 0x45, 0xea, 0x3b, 0x78, 0x9c,
  0x4b, 0x02, 0x3d, 0x6e, 0xd0, 0x43, 0xff, 0x1c, 0x26, 0x55, 0x3b, 0x14,
  0x6d, 0xd7, 0x2a, 0x04, 0x43, 0x22, 0x95, 0x84, 0x25, 0x66, 0x8c, 0x47,
  0xda, 0x5b, 0x2e, 0x91, 0xd6, 0xcb, 0xd4, 0x93, 0x2d, 0xdb, 0xdf, 0xef,
  0xb3, 0x5d, 0x2a, 0x1a, 0x6e, 0xbb, 0x75, 0x52, 0x2a, 0x14, 0x6d, 0x8c,
  0x03, 0x92, 0x00, 0x45, 0x34, 0x66, 0xf8, 0x39, 0xe9, 0xaf, 0x2a, 0x75,
  0xd8, 0x39, 0xf4, 0x2a, 0x05, 0x69, 0x0d, 0xa1, 0xa1, 0x2a, 0xcd, 0x5f,
  0xe0, 0xfd, 0x72, 0xae, 0x5a, 0x2b, 0x79, 0x54, 0x73, 0x25, 0xbc, 0xda,
  0xb2, 0x98, 0xa6, 0x91, 0xc0, 0xef, 0xa8, 0xc6, 0xb6, 0x4b, 0x88, 0x17,
  0x6c, 0xb5, 0x43, 0x49, 0xda, 0xf4, 0x40, 0x16, 0xca, 0x80, 0x0f, 0xcc,
  0x2a, 0x7d, 0xa8, 0x7f, 0x50, 0x2c, 0xb9, 0xd8, 0x31, 0xc6, 0x22, 0xf9,
  0x8f, 0x58, 0xf2, 0xfb, 0xd6, 0x4f, 0x59, 0xb6, 0x4e, 0x56, 0x3f, 0x70,
  0xb0, 0xe3, 0xe2, 0x74, 0xaf, 0x08, 0xf6, 0x38, 0x08, 0x03, 0x47, 0x31,
  0xa3, 0xdf, 0xc0, 0x36, 0xcf, 0x8b, 0xd0, 0x3f, 0x6a, 0x4f, 0x4e, 0x37,
  0x13, 0x61, 0x0b, 0x93, 0x69, 0xd1, 0x01, 0x29, 0x84, 0xbd, 0xb5, 0x9e,
  0xa0, 0xb6, 0x1d, 0xcd, 0xc2, 0x21, 0x9c, 0xb5, 0x44, 0xe3, 0x71, 0x03,
  0x1f, 0xe8, 0xbc, 0xb6, 0x06, 0x0a, 0x96, 0x07, 0xd3, 0x55, 0x8d, 0x08,
  0x42, 0x4a, 0x3b, 0x8c, 0xc2, 0x2c, 0xf1, 0x86, 0x4e, 0xdf, 0xc2, 0xf4,
  0xeb, 0xe1, 0xf4, 0x56, 0x9f, 0xae, 0xc5, 0x35, 0x67, 0xf4, 0x4d, 0x60,
  0x5d, 0xf8, 0xcf, 0xb8, 0x80, 0xa0, 0x20, 0x89, 0xef, 0x7b, 0xe6, 0x7c,
  0x9e, 0x67, 0xd6, 0x44, 0x13, 0x66, 0x5d, 0xcf, 0xff, 0x29, 0xd6, 0x49,
  0x96, 0x85, 0x0b, 0x7e, 0x01, 0x36, 0x66, 0x95, 0x6b, 0x80, 0x01, 0x00,
  0x00
};

template<std::size_t N> constexpr auto md5( unsigned char const(&a)[ N ] )
{
    boost::hash2::md5_128 hash;
    hash.update( a, N );
    return hash.result();
}

constexpr auto resource_digest = md5( resource );

int main()
{
    std::cout << "Resource digest: " << resource_digest << std::endl;
}
