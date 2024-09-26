// Copyright 2024 Christian Mazakas.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#define _CRT_SECURE_NO_WARNINGS

#include <boost/hash2/sha2.hpp>
#include <boost/core/lightweight_test.hpp>
#include <cstdio>
#include <string>
#include <vector>

template<std::size_t N> std::string to_string( std::array<unsigned char, N> const & v )
{
    std::string r;

    for( std::size_t i = 0; i < N; ++i )
    {
        char buffer[ 4 ];
        std::snprintf( buffer, sizeof( buffer ), "%02x", v[ i ] );
        r += buffer;
    }

    return r;
}

template<class H> std::string digest( std::string const & s )
{
    H h;

    h.update( s.data(), s.size() );

    return to_string( h.result() );
}

template<class H, std::size_t N> std::string digest( char (&buf)[N])
{
    H h;

    h.update( buf, N );

    return to_string( h.result() );
}

template<class H> std::string digest( std::vector<char> const & v )
{
    H h;

    h.update( v.data(), v.size() );

    return to_string( h.result() );
}

using boost::hash2::sha2_256;
using boost::hash2::sha2_224;

static
void sha_256()
{
    // https://en.wikipedia.org/wiki/SHA-2#Test_vectors

    BOOST_TEST_EQ( digest<sha2_256>( "" ), std::string( "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855" ) );

    // https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA256.pdf

    BOOST_TEST_EQ( digest<sha2_256>( "abc" ), std::string( "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad" ) );
    BOOST_TEST_EQ( digest<sha2_256>( "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq" ), std::string( "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1" ) );

    // use test vectors from here
    // https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA2_Additional.pdf

    BOOST_TEST_EQ( digest<sha2_256>( "\xbd" ), std::string( "68325720aabd7c82f30f554b313d0570c95accbb7dc4b5aae11204c08ffe732b" ) );
    BOOST_TEST_EQ( digest<sha2_256>( "\xc9\x8c\x8e\x55" ), std::string( "7abc22c0ae5af26ce93dbb94433a0e0b2e119d014f8e7f65bd56c61ccccd9504" ) );

    {
        char buf[ 55 ] = { 0 };
        BOOST_TEST_EQ( digest<sha2_256>( buf ), std::string( "02779466cdec163811d078815c633f21901413081449002f24aa3e80f0b88ef7" ) );
    }

    {
        char buf[ 56 ] = { 0 };
        BOOST_TEST_EQ( digest<sha2_256>( buf ), std::string( "d4817aa5497628e7c77e6b606107042bbba3130888c5f47a375e6179be789fbb" ) );
    }

    {
        char buf[ 64 ] = { 0 };
        BOOST_TEST_EQ( digest<sha2_256>( buf ), std::string( "f5a5fd42d16a20302798ef6ed309979b43003d2320d9f0e8ea9831a92759fb4b" ) );
    }

    {
        char buf[ 1000 ] = { 0 };
        BOOST_TEST_EQ( digest<sha2_256>( buf ), std::string( "541b3e9daa09b20bf85fa273e5cbd3e80185aa4ec298e765db87742b70138a53" ) );
    }

    {
        char buf[ 1000 ] = {};
        for( auto& c : buf ) c = 'A';
        BOOST_TEST_EQ( digest<sha2_256>( buf ), std::string( "c2e686823489ced2017f6059b8b239318b6364f6dcd835d0a519105a1eadd6e4" ) );
    }

    {
        char buf[ 1005 ] = {};
        for( auto& c : buf ) c = 'U';
        BOOST_TEST_EQ( digest<sha2_256>( buf ), std::string( "f4d62ddec0f3dd90ea1380fa16a5ff8dc4c54b21740650f24afc4120903552b0" ) );
    }

    {
        std::vector<char> buf(1000000, 0x00);
        BOOST_TEST_EQ( digest<sha2_256>( buf ), std::string( "d29751f2649b32ff572b5e0a9f541ea660a50f94ff0beedfb0b692b924cc8025" ) );
    }
}

static
void sha_224()
{
    // https://en.wikipedia.org/wiki/SHA-2#Test_vectors

    BOOST_TEST_EQ( digest<sha2_224>( "" ), std::string( "d14a028c2a3a2bc9476102bb288234c415a2b01f828ea62ac5b3e42f" ) );

    // https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA224.pdf
    BOOST_TEST_EQ( digest<sha2_224>( "abc" ), std::string( "23097d223405d8228642a477bda255b32aadbce4bda0b3f7e36c9da7" ) );
    BOOST_TEST_EQ( digest<sha2_224>( "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq" ), std::string( "75388b16512776cc5dba5da1fd890150b0c6455cb4f58b1952522525" ) );

    // https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA2_Additional.pdf
    BOOST_TEST_EQ( digest<sha2_224>( "\xff" ), std::string( "e33f9d75e6ae1369dbabf81b96b4591ae46bba30b591a6b6c62542b5" ) );
    BOOST_TEST_EQ( digest<sha2_224>( "\xe5\xe0\x99\x24" ), std::string( "fd19e74690d291467ce59f077df311638f1c3a46e510d0e49a67062d" ) );

    {
        char buf[ 56 ] = { 0 };
        BOOST_TEST_EQ( digest<sha2_224>( buf ), std::string( "5c3e25b69d0ea26f260cfae87e23759e1eca9d1ecc9fbf3c62266804" ) );
    }

    {
        char buf[ 1000 ] = {};
        for( auto& c : buf ) c = 'Q';
        BOOST_TEST_EQ( digest<sha2_224>( buf ), std::string( "3706197f66890a41779dc8791670522e136fafa24874685715bd0a8a" ) );
    }

    {
        char buf[ 1000 ] = {};
        for( auto& c : buf ) c = 'A';
        BOOST_TEST_EQ( digest<sha2_224>( buf ), std::string( "a8d0c66b5c6fdfd836eb3c6d04d32dfe66c3b1f168b488bf4c9c66ce" ) );
    }

    {
        char buf[ 1005 ] = {};
        for( auto& c : buf ) c = '\x99';
        BOOST_TEST_EQ( digest<sha2_224>( buf ), std::string( "cb00ecd03788bf6c0908401e0eb053ac61f35e7e20a2cfd7bd96d640" ) );
    }

    {
        std::vector<char> buf(1000000, 0x00);
        BOOST_TEST_EQ( digest<sha2_224>( buf ), std::string( "3a5d74b68f14f3a4b2be9289b8d370672d0b3d2f53bc303c59032df3" ) );
    }
}

int main()
{
    sha_256();
    sha_224();
    return boost::report_errors();
}
