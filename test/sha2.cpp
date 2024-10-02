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

static
void sha_256()
{
    using boost::hash2::sha2_256;

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
    using boost::hash2::sha2_224;

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

static
void sha_512()
{
    using boost::hash2::sha2_512;

    // https://en.wikipedia.org/wiki/SHA-2#Test_vectors
    BOOST_TEST_EQ( digest<sha2_512>( "" ), std::string( "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e" ) );

    // https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA512.pdf
    BOOST_TEST_EQ( digest<sha2_512>( "abc" ), std::string( "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f" ) );
    BOOST_TEST_EQ( digest<sha2_512>( "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu" ), std::string( "8e959b75dae313da8cf4f72814fc143f8f7779c6eb9f7fa17299aeadb6889018501d289e4900f7e4331b99dec4b5433ac7d329eeb6dd26545e96e55b874be909" ) );

    // https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA2_Additional.pdf
    {
        char buf[ 111 ] = {};
        for( auto& c : buf ) c = 0;
        BOOST_TEST_EQ( digest<sha2_512>( buf ), std::string( "77ddd3a542e530fd047b8977c657ba6ce72f1492e360b2b2212cd264e75ec03882e4ff0525517ab4207d14c70c2259ba88d4d335ee0e7e20543d22102ab1788c" ) );
    }

    {
        char buf[ 112 ] = {};
        for( auto& c : buf ) c = 0;
        BOOST_TEST_EQ( digest<sha2_512>( buf ), std::string( "2be2e788c8a8adeaa9c89a7f78904cacea6e39297d75e0573a73c756234534d6627ab4156b48a6657b29ab8beb73334040ad39ead81446bb09c70704ec707952" ) );
    }

    {
        char buf[ 113 ] = {};
        for( auto& c : buf ) c = 0;
        BOOST_TEST_EQ( digest<sha2_512>( buf ), std::string( "0e67910bcf0f9ccde5464c63b9c850a12a759227d16b040d98986d54253f9f34322318e56b8feb86c5fb2270ed87f31252f7f68493ee759743909bd75e4bb544" ) );
    }

    {
        char buf[ 122 ] = {};
        for( auto& c : buf ) c = 0;
        BOOST_TEST_EQ( digest<sha2_512>( buf ), std::string( "4f3f095d015be4a7a7cc0b8c04da4aa09e74351e3a97651f744c23716ebd9b3e822e5077a01baa5cc0ed45b9249e88ab343d4333539df21ed229da6f4a514e0f" ) );
    }

    {
        char buf[ 1000 ] = {};
        for( auto& c : buf ) c = 0;
        BOOST_TEST_EQ( digest<sha2_512>( buf ), std::string( "ca3dff61bb23477aa6087b27508264a6f9126ee3a004f53cb8db942ed345f2f2d229b4b59c859220a1cf1913f34248e3803bab650e849a3d9a709edc09ae4a76" ) );
    }

    {
        char buf[ 1000 ] = {};
        for( auto& c : buf ) c = 'A';
        BOOST_TEST_EQ( digest<sha2_512>( buf ), std::string( "329c52ac62d1fe731151f2b895a00475445ef74f50b979c6f7bb7cae349328c1d4cb4f7261a0ab43f936a24b000651d4a824fcdd577f211aef8f806b16afe8af" ) );
    }

    {
        char buf[ 1005 ] = {};
        for( auto& c : buf ) c = 'U';
        BOOST_TEST_EQ( digest<sha2_512>( buf ), std::string( "59f5e54fe299c6a8764c6b199e44924a37f59e2b56c3ebad939b7289210dc8e4c21b9720165b0f4d4374c90f1bf4fb4a5ace17a1161798015052893a48c3d161" ) );
    }

    {
        std::vector<char> buf(1000000, 0x00);
        BOOST_TEST_EQ( digest<sha2_512>( buf ), std::string( "ce044bc9fd43269d5bbc946cbebc3bb711341115cc4abdf2edbc3ff2c57ad4b15deb699bda257fea5aef9c6e55fcf4cf9dc25a8c3ce25f2efe90908379bff7ed" ) );
    }

}

static void sha_384()
{
    using boost::hash2::sha2_384;

    // https://en.wikipedia.org/wiki/SHA-2#Test_vectors
    BOOST_TEST_EQ( digest<sha2_384>( "" ), std::string("38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b"));

    // https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA384.pdf
    BOOST_TEST_EQ( digest<sha2_384>( "abc" ), std::string( "cb00753f45a35e8bb5a03d699ac65007272c32ab0eded1631a8b605a43ff5bed8086072ba1e7cc2358baeca134c825a7" ) );
    BOOST_TEST_EQ( digest<sha2_384>( "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu" ), std::string( "09330c33f71147e83d192fc782cd1b4753111b173b3b05d22fa08086e3b0f712fcc7c71a557e2db966c3e9fa91746039" ) );

    // https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA2_Additional.pdf
    {
        char buf[ 111 ] = {};
        BOOST_TEST_EQ( digest<sha2_384>( buf ), std::string( "435770712c611be7293a66dd0dc8d1450dc7ff7337bfe115bf058ef2eb9bed09cee85c26963a5bcc0905dc2df7cc6a76" ) );
    }

    {
        char buf[ 112 ] = {};
        BOOST_TEST_EQ( digest<sha2_384>( buf ), std::string( "3e0cbf3aee0e3aa70415beae1bd12dd7db821efa446440f12132edffce76f635e53526a111491e75ee8e27b9700eec20" ) );
    }

    {
        char buf[ 113 ] = {};
        BOOST_TEST_EQ( digest<sha2_384>( buf ), std::string( "6be9af2cf3cd5dd12c8d9399ec2b34e66034fbd699d4e0221d39074172a380656089caafe8f39963f94cc7c0a07e3d21" ) );
    }

    {
        char buf[ 122 ] = {};
        BOOST_TEST_EQ( digest<sha2_384>( buf ), std::string( "12a72ae4972776b0db7d73d160a15ef0d19645ec96c7f816411ab780c794aa496a22909d941fe671ed3f3caee900bdd5" ) );
    }

    {
        char buf[ 1000 ] = {};
        BOOST_TEST_EQ( digest<sha2_384>( buf ), std::string( "aae017d4ae5b6346dd60a19d52130fb55194b6327dd40b89c11efc8222292de81e1a23c9b59f9f58b7f6ad463fa108ca" ) );
    }

    {
        char buf[ 1000 ];
        for( auto& c : buf ) c = 'A';
        BOOST_TEST_EQ( digest<sha2_384>( buf ), std::string( "7df01148677b7f18617eee3a23104f0eed6bb8c90a6046f715c9445ff43c30d69e9e7082de39c3452fd1d3afd9ba0689" ) );
    }

    {
        char buf[ 1005 ];
        for( auto& c : buf ) c = 'U';
        BOOST_TEST_EQ( digest<sha2_384>( buf ), std::string( "1bb8e256da4a0d1e87453528254f223b4cb7e49c4420dbfa766bba4adba44eeca392ff6a9f565bc347158cc970ce44ec" ) );
    }

    {
        std::vector<char> buf(1000000, 0);
        BOOST_TEST_EQ( digest<sha2_384>( buf ), std::string( "8a1979f9049b3fff15ea3a43a4cf84c634fd14acad1c333fecb72c588b68868b66a994386dc0cd1687b9ee2e34983b81" ) );
    }
}

int main()
{
    sha_256();
    sha_224();
    sha_512();
    sha_384();
    return boost::report_errors();
}
