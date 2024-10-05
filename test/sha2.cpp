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

std::string from_hex( char const* str )
{
    auto f = []( char c ) { return ( c >= 'a' ? c - 'a' + 10 : c - '0' ); };

    std::string s;
    while( *str != '\0' )
    {
        s.push_back( static_cast<char>( ( f( str[ 0 ] ) << 4 ) + f( str[ 1 ] ) ) );
        str += 2;
    }
    return s;
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

static void sha_256()
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

static void sha_224()
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

static void sha_512()
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
    BOOST_TEST_EQ( digest<sha2_384>( "" ), std::string( "38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b" ));

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

static void sha_512_224()
{
    using boost::hash2::sha2_512_224;

    // https://en.wikipedia.org/wiki/SHA-2#Test_vectors
    BOOST_TEST_EQ( digest<sha2_512_224>( "" ), std::string( "6ed0dd02806fa89e25de060c19d3ac86cabb87d6a0ddd05c333b84f4" ));

    // https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA512_224.pdf
    BOOST_TEST_EQ( digest<sha2_512_224>( "abc" ), std::string( "4634270f707b6a54daae7530460842e20e37ed265ceee9a43e8924aa" ) );
    BOOST_TEST_EQ( digest<sha2_512_224>( "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu" ), std::string( "23fec5bb94d60b23308192640b0c453335d664734fe40e7268674af9" ) );

    // selected samples from the download available here:
    // https://csrc.nist.gov/Projects/Cryptographic-Algorithm-Validation-Program/Secure-Hashing
    std::pair<char const*, char const*> inputs[] =
    {
          /* expected hash */                                         /* input message */
        { "4199239e87d47b6feda016802bf367fb6e8b5655eff6225cb2668f4a", "cf" },
        { "392b99b593b85e147f031986c2a9edfdb4ffd9f24c77c452d339c9fc", "ca2d" },
        { "a9c345d58a959af20a42c84e28523ba47e3bf8fad8e8c3f32b7a72ae", "497604" },
        { "a74af68819afe81bcdaceba64201c0d41f843e4b08e4002a375be761", "3c7e038401fa74c6c06e41" },
        { "087ed68f1db90ffb2fb4ff7dc4b17fe08100b64383850378ef543339", "40bd7d47b636c2a749a247fdda75807c238b" },
        { "72b43417b071f4811833027731b0ca28549c0357530fe258ca00533e", "cd5fee5fde5e9aa2884b4f4882cfa7d5571f8fd572c5f9bf77a3d21fda35" },
        { "fa425bc732d6033566c073560b2c5fe322aa4fa22aaa3ec51154ffd8", "f15284a11c61e129ea0606bd6531f2f1213776e01e253d1def530bed1c3c42b3c68caa" },
        { "75a0fa978c45d268124d8cd9ef0a08ecabbbed53412cfc7cb1c00398", "e3089c05ce1549c47a97785d82474c73cb096a27c5205de8ed9e3a8c971f7fa0eab741fd2c29879c40" },
        { "1e7590e408c038b794e9820b25d011c262062b96d111dccc46dc6783", "0431a7bfbbec1bb8116a62e1db7e1346862d31ad5110ff1bb9fa169a35dbb43a24e4575604ec8b18e41300" },
        { "33a5a8d6119bb6dd7b2e72ece8e4d5d02aa99048c0459169ee9e6d04", "4dbe1290524bd73d9db5f21f9d035e183dc285b85ba755057c769777be227c470e3679ea9a7355d889bb8191ea2ea7e2" },
        { "09900c5ae3074fe73e6c4eef51f785e57947bafbe1d8dea38868e3d1", "13e6b1b4f021d610c81c97f0f952daba2766034d815b5dda4603bcf788ba60ee31541d5b4353b9f6645d96ad99ee90f6524b2963a7b7e476e1e8eeb83cbc0305eb29902a5d72" },
        { "9a9176e97aec99ab07f468f6a226876710d6d877021d27061d4d0132", "4b9895235cb4956aefffe815415252e7d6b21921bd7f675315eff071d0bbd429b718c774aee96f6c3a330d5d40d1601e1069c7a2a19ea5ca1e87097da2608ffb4180816e478b42c3c4e9edb748773935eb7ca0df90dec0eb6b960130c1617880efb80b39ae03d617950ace4ce0aca4d36fd3ed0112a77f5d03021eb1b42458" },
        { "72640a79fbb1cfb26e09b4b35385389ed633a55e092906d01a7186e1", "9625ae618ea633fd7ae5b20ceafd6b1f3ab1a6aa20aded66810e78f38925e9c2fa783a32c40af3f9d7dda0c635b482254b1d85a281af7231109166cd133c8360e281e5e39bcdd7c601ac47928a8c78cdb3c4f71e97d4d0b1c0ee01dd3db62f04f44798bb3a76492ba15a91b7110cb5e01babe56589a36fae3a2f336a2d1d5778dbd23c03ca8db0f25ff0657ff4bca1252adc38c080a5b8f0255ce3be0bf862823d2ab704729b74e1e275aa305824a566895ed677a460113e2a7bf91f00d0b8ebc358f3035b27fcc1d3f14a1367cd2769df39a9d21c5ee361f1965cd6342cc17a1463d6" },
        { "13f224102f57b2a5774d979be2ff6691a9ae7125a1443e805598abe9", "1e13f580fa2de14f1294e1eba9ce789c75072c9f54fcdc253c17549d70db1f36fa839ca055e655136a9e8b93be691672e9cf6c164a06fffdf912ccfdfd3030d1bf75fed1bfb3d001869c9a4ddc2a85133a3efae28287fc82eea5bc34468e673a5731439aa05afa204ed636a26bd76d87529aff8a66467ebc03184cc8b5bd6c7ba8ff928460a47c78aa938519d33978d7172ba2975c0d2bb421b2a643b184e69c9c2713166759fe11831db23a7c184c0a733b0c90cea2ab712ebcef2da1ad7ea31af0f0d81e4127f4bfbae38dce3c91284d1064fd23cea7fb137e520ceffedb9a09a44e52eb23a02848b3419b326cf03a8cf3d367c359c75bb940f56a0240a6885389580d37c910b50450f3eb0e7210471a2c8155160bb298074a00ce423a8676dfe733906bf920a4ddd82105cb149b57de03954f84ac11bae4e39cc117b6246b95080a63ed7c78fcba95f572d21b3673c0c037dd75038bca3a55f1cae97a276f5d33030f271abfc582cd95b98a4e8ba1a8aa918851d9c9cef0e626712050388a1faf461b7f9a9e071fc929625a7742eb7e0ac8d780f672d1eedf633e24feffc5c3c5fc0f5fc6bccb78d1daf6ac5c03592a2807536a222fd81c88d2ba5e4c232731bd4d742e64c218752ccffce7c775f2954c74827725a8ed6228986c34044db952df60d0543d57f7fe2432fc727e40bea5be37ee10c68417a808e0dc0fec24820c725eae3919246ffbff287eb7490dc543c5791f9bce6c5fd671d09358ce518a48c06e9345c0d0885406db0b1df2841058ab629c820607cbd4a9901875707bd9ef0fb909e0f9044af281732a0c3f4ca6cab619ed2b33fe668af849f3d09a4b78e7a86797728d68bebbb81764ecfa0e8f832fc79a0020f835e0a823adcf16686e1ad39fc66345a3eb98f2f04026e971d4695e932b67949e42bf0045cddcab7cc36ed8891c1be75eda7179264620d51e7c668aa629ca5698940d47d8db5e202c7fcdfe4e23023b40912f93e0fe4385bc8f61fc271c902f8d33b4c60360bd3e22ab1e8e1663800bf21ab88946f1f7c3f41641fd3f8a21e725289ac5efe360cd064f49d00863fe1d1b7df6a382a6a5556e17f0d316fde546097aa98bc3ecb1957f71350c4dabe23a64c0d028b9a5b304d19d55c6d3fc44" },
        { "8f7b464921f5344b3ea2f07ea95028bf0ef23573d1025160caa5b000", "cd4c8dbfb9cb8b99f4fe7c65f027ed8f3f1da8c79a41d89c27b46f4cfcc7c5dadfb381624f8bbdaac96e65f94401e02cfe1cf3067ac4e2c8c95ad8010a1188a82da61568fc4b2f3b5be4803ace7546b43ddb417df4bbcf88eeb956b853eae2f43f8b0f7ac5f32a7534c693d1986c654e84c159d30f0ee8061d39b019e03819e69ecb383fb46bf47fdb7d6765aacc87235c3605b1d4a67567de42b0cbb8e576a8d1cf5860d09bd582d872c49f0a7ff25a4ca20bbf4969bed6b93c1c77e3d7415f60fe3784216b17a6b40c7127c26bee1cdd6e34a478400a79378fbdc46af8c236d4bf9c54b0c40112f6b238a4da7ac591950048096913a378dfa35b5b542e4153b37c5177848cdaef26cedfbf5893bde0ffc10f9523cbcffc3ae0bee3a96305de889c1c96a5adeb64ccc72f1f469d2032a01951f1ae4ed72dceafad05717e5700d887c6591c0d7c9a7105f06cab8234ae0271d5c4128603322676f169b5677fe02da41767f096d4b99cf0ea509029427dac2f64b7575a9a34c79e52b67d06c5a80e5dd669292e3de8d6f0e471fc7a91c626bdc67286b1e65a3b909895cf43707d0419cd06d0bffd4fa66da877f75d6bb2f981c39ca7752dcd09b38badb506ca899641c2e7ec305d1794cffc0b8aba1669ace522ddc08db8b4b96ef3a7363719f44c3b7eb48569a568287ea9d5bb79db12c74faddfdbe1cf38d32b972a5abea6e6ae70c914a4521d69ebe2777c948202d5d2292a47ff59b981e05140df2a83b98645486aaf4fd30d4364632da91dbdd3db56da62c035ddc0b66848b66dad6e9afc982c2b2e91d4bc1d1c6a95035d13964cdbca1df084552afff177c17eec6890712f82d2396f3d7320995d75335c6a9250e761237ff4a94085054829b7ed57e40d93b32877f3f67a7465a696e022ac100d04073b168dc14a702b22b5c2ba25dff74b28f59124e7194008abc0b3b8bf58aeb9242f5d588590e8ec6d2f475bb8c658df48012e0af998ac08d6ba53258db2598e20c5fb5cde5914ce925dedd6fae457d87a13f7ae123ee2ef8e4e6a71fc66370c63c699a1b2ef1c5bf7075d35d1801dbd28f594171b5407443a429da5f6bb6fba55f9de30eb98291b05f73f8027e1d116b96dac4dfd224690445f96c762fa29215f2873106f9d639524f43abde3509c6c9544e7599ef0c55a136dfde2276c8fc7e719ff492846f151bdc5f6f6ed15a6452442ef42e806ac2a0f3479fb2f56c63657952be4fcdafbd736331c322d78162ccd2e6910c2ab2488a07bb31c6103f9f615649fe8d5a3abb0f906547a8bc114a6fbd100cc132955fd2e0c534ca5ba4e8d8b4e025d9ae727636c0645d5ef37ca3d13f45a7dc5c6661021ad0094e3c2ed851f1bfc4c33ef9778f9fa984b41235e787e5d1f2bbfcb7f4ae12762fa0364ff663b9237bc8703247707acf4e469cdf8dfa4d5f8dee980ca32d6422289eee8acb6467d58b5806af6e9fde202a94f7dc1fbed238d9f2b2c6c5069c5e8468a961bffa8a20a3f056e345645656aa1db7be053c21185756d18902c007a5b3bc0b575c8c8c9f363ff55064446fdd3d4f60e7e7089658869978d5c4f0527246f798fe19a10bcbb285468953d6871e54b8680c3d8ace8408291d1f840de95ebea9b6a88fce7420b97edef09c8138b490d7417d615916252c8432c8d87f58c57b50f8f9276b5228bb6b2328a6eaf11a722a91ce0afa29b694396e843adb0d410d7be462690e325e28f8b783eb5a4cca6930cd6082c455a2bfc704c41397934bf588fd76cd3d713b82a610b50285b84ef67848f7716e46940fabb9fb0b3b7750c4169898dc6fab4b78d1a31e64c3a012aa6fb8d54f4e68e51" },
    };

    for( auto const& input : inputs )
    {
        BOOST_TEST_EQ( digest<sha2_512_224>( from_hex( input.second ) ), std::string( input.first ));
    }
}

int main()
{
    sha_256();
    sha_224();
    sha_512();
    sha_384();
    sha_512_224();
    return boost::report_errors();
}
