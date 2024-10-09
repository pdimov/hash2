// Copyright 2024 Christian Mazakas.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#define _CRT_SECURE_NO_WARNINGS

#ifdef _MSC_VER
# pragma warning(disable: 4309) // truncation of constant value
#endif

#include <boost/hash2/sha2.hpp>
#include <boost/core/lightweight_test.hpp>
#include <cstdint>
#include <string>
#include <cstddef>
#include <cstdio>

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

template<std::size_t N> std::string to_string( std::array<unsigned char, N> const & v )
{
    std::string r;

    for( std::size_t i = 0; i < N; ++i )
    {
        char buffer[ 8 ];

        std::snprintf( buffer, sizeof( buffer ), "%02x", static_cast<int>( v[ i ] ) );

        r += buffer;
    }

    return r;
}

template<class H> std::string digest( std::string const & k, std::string const & s )
{
    H h( reinterpret_cast<unsigned char const*>( k.data() ), k.size() );

    h.update( s.data(), s.size() );

    return to_string( h.result() );
}

static void hmac_sha_256()
{
    using boost::hash2::hmac_sha2_256;

    // Test vectors from https://en.wikipedia.org/wiki/Hash-based_message_authentication_code
    BOOST_TEST_EQ( digest<hmac_sha2_256>( "key", "The quick brown fox jumps over the lazy dog" ), std::string( "f7bc83f430538424b13298e6aa6fb143ef4d59a14946175997479dbc2d1a3cd8" ) );

    // Test vectors from https://www.rfc-editor.org/rfc/rfc4868#section-2.7
    BOOST_TEST_EQ( digest<hmac_sha2_256>( from_hex( "0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b"), "Hi There" ), std::string( "b0344c61d8db38535ca8afceaf0bf12b881dc200c9833da726e9376c2e32cff7" ) );
    BOOST_TEST_EQ( digest<hmac_sha2_256>( "Jefe", "what do ya want for nothing?" ), std::string( "5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843" ) );
    BOOST_TEST_EQ( digest<hmac_sha2_256>( from_hex( "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"), from_hex( "dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd" ) ), std::string( "773ea91e36800e46854db8ebd09181a72959098b3ef8c122d9635514ced565fe" ) );
    BOOST_TEST_EQ( digest<hmac_sha2_256>( from_hex( "0102030405060708090a0b0c0d0e0f10111213141516171819"), from_hex( "cdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcd" ) ), std::string( "82558a389a443c0ea4cc819899f2083a85f0faa3e578f8077a2e3ff46729665b" ) );
    BOOST_TEST_EQ( digest<hmac_sha2_256>( from_hex( "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"), std::string( "Test Using Larger Than Block-Size Key - Hash Key First" ) ), std::string( "60e431591ee0b67f0d8a26aacbf5b77f8e0bc6213728c5140546040f0ee37f54" ) );
    BOOST_TEST_EQ( digest<hmac_sha2_256>( from_hex( "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"), std::string( "This is a test using a larger than block-size key and a larger than block-size data. The key needs to be hashed before being used by the HMAC algorithm." ) ), std::string( "9b09ffa71b942fcb27635fbcd5b0e944bfdc63644f0713938a7f51535c3a35e2" ) );

    BOOST_TEST_EQ( digest<hmac_sha2_256>( from_hex( "0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b"), "Hi There" ), std::string( "198a607eb44bfbc69903a0f1cf2bbdc5ba0aa3f3d9ae3c1c7a3b1696a0b68cf7" ) );
    BOOST_TEST_EQ( digest<hmac_sha2_256>( "JefeJefeJefeJefeJefeJefeJefeJefe", "what do ya want for nothing?" ), std::string( "167f928588c5cc2eef8e3093caa0e87c9ff566a14794aa61648d81621a2a40c6" ) );
    BOOST_TEST_EQ( digest<hmac_sha2_256>( from_hex( "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" ), from_hex( "dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd" ) ), std::string( "cdcb1220d1ecccea91e53aba3092f962e549fe6ce9ed7fdc43191fbde45c30b0" ) );
    BOOST_TEST_EQ( digest<hmac_sha2_256>( from_hex( "0102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f20" ), from_hex( "cdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcd" ) ), std::string( "372efcf9b40b35c2115b1346903d2ef42fced46f0846e7257bb156d3d7b30d3f" ) );

    // use selected samples from the download available here: https://csrc.nist.gov/Projects/cryptographic-algorithm-validation-program/Message-Authentication
    BOOST_TEST_EQ( digest<hmac_sha2_256>( from_hex( "9779d9120642797f1747025d5b22b7ac607cab08e1758f2f3a46c8be1e25c53b8c6a8f58ffefa176" ), from_hex( "b1689c2591eaf3c9e66070f8a77954ffb81749f1b00346f9dfe0b2ee905dcc288baf4a92de3f4001dd9f44c468c3d07d6c6ee82faceafc97c2fc0fc0601719d2dcd0aa2aec92d1b0ae933c65eb06a03c9c935c2bad0459810241347ab87e9f11adb30415424c6c7f5f22a003b8ab8de54f6ded0e3ab9245fa79568451dfa258e" ) ), std::string( "769f00d3e6a6cc1fb426a14a4f76c6462e6149726e0dee0ec0cf97a16605ac8b" ) );
    BOOST_TEST_EQ( digest<hmac_sha2_256>( from_hex( "09675f2dcc4783b599f18fb765583668a0fd8ae4096f6fcdc60d4f35b4130fbefcd542ffe7459d2a" ), from_hex( "0cf2198c31376f5c8915660137725f2bbc180a986e5a7bda27fa81593a4a339bab92cbc39fb2b8581108ee48c794812d845a72ce8008c9e915d9e330bbb90e9136aa53ba0e6693dd4046d6b03362dfb9edfa04c887153cc5de677aab8c7839d517035879679c29727e96c5426324a2575fbe678d6cc7fef5eb6cebd595cfddef" ) ), std::string( "6b142d4dfe217f1881aa0e6483b271dd5d43f70b85605953a0fef272ddde46ca" ) );
    BOOST_TEST_EQ( digest<hmac_sha2_256>( from_hex( "b763263dc4fc62b227cd3f6b4e9e358c21ca036ce396ab9259c1bedd2f5cd90297dc703c336eca3e358a4d6dc5" ), from_hex( "53cb09d0a788e4466d01588df6945d8728d9363f76cd012a10308dad562b6be093364892e8397a8d86f1d81a2096cfc8a1bbb26a1a75525ffebfcf16911dadd09e802aa8686acfd1e4524620254a6bca18dfa56e71417756e5a452fa9ae5aec5dc71591c11630e9defec49a4ecf85a14f60eb854657899972ea5bf6159cb9547" ) ), std::string( "737301dea93db6bcbadd7bf796693961317ca680b380416f12f466f06526b36b" ) );
    BOOST_TEST_EQ( digest<hmac_sha2_256>( from_hex( "9fe42dfac92a4a136fa7c9f6e331b5d3a61aa73035b53a8d2517be43721b31b215a96b9bd43798cb5e8febfa97" ), from_hex( "f9660fb784c14b5fbec280526a69c2294fba12aea163789bbe9f52a51b5aebb97d964f866c0d5e3be41820924fcf580db0725c7f210823cf7f45a0f964b14e5555070d1c3ddb2c281a80c7fbf72953031a4e771d7e521d578462cafae5a02ac8eb81f082e173ddadc8c41d964bbfda94f5180c8da28a8ebb33be77b0866fa798" ) ), std::string( "7786c155d10c741b63ec650b7b1aa3bfd71ac71881ad06ae98fb082f17e0caa0" ) );
    BOOST_TEST_EQ( digest<hmac_sha2_256>( from_hex( "2914da23e86a603cda1eede153be2431c2947cdaeed6a1ea801d18e2c218220ca682e40f0a51c4c13a31163cb730f83437bb7a88ecc903160956f0d483137d1d145ce948866ad57f2eca" ), from_hex( "6b8db9acdfd24150808a92368596557181d445e5a04e91112db2812b58035d72378d8bc00a1ef75ec373b81dc6f1f0a2ed96f302cf2eac8f42ca3df11e6ee678440a28b0dfab2a36eaf35bcbf3c759a71e47120f6c03292a3d6b9b111488a2259bead9a5e7e2a180fcf1c467947f59271cd0e8360035ce8b287fe2b3c3b95822" ) ), std::string( "4de7bab7fe9a0a9bf7b51a7cdf7d929f2b1c6ff4575fd527baba1efdf4254890" ) );
    BOOST_TEST_EQ( digest<hmac_sha2_256>( from_hex( "4b7ab133efe99e02fc89a28409ee187d579e774f4cba6fc223e13504e3511bef8d4f638b9aca55d4a43b8fbd64cf9d74dcc8c9e8d52034898c70264ea911a3fd70813fa73b083371289b" ), from_hex( "138efc832c64513d11b9873c6fd4d8a65dbf367092a826ddd587d141b401580b798c69025ad510cff05fcfbceb6cf0bb03201aaa32e423d5200925bddfadd418d8e30e18050eb4f0618eb9959d9f78c1157d4b3e02cd5961f138afd57459939917d9144c95d8e6a94c8f6d4eef3418c17b1ef0b46c2a7188305d9811dccb3d99" ) ), std::string( "4f1ee7cb36c58803a8721d4ac8c4cf8cae5d8832392eed2a96dc59694252801b" ) );
}

int main()
{
    hmac_sha_256();

    return boost::report_errors();
}
