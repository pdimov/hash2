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
#include <vector>
#include <tuple>

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

static void hmac_sha_224()
{
    using boost::hash2::hmac_sha2_224;

    // use selected samples from the download available here: https://csrc.nist.gov/Projects/cryptographic-algorithm-validation-program/Message-Authentication
    std::vector<std::tuple<char const*, char const*, char const*>> inputs =
    {     /* expected mac */                                          /* key */                                                                                               /* msg */
        { "5507c4c7c127046c04d161fdc75ec2f5122ed44e86679c39c69c5ae8", "82a289b0911f55c532e2571866fa354d973948ec7b899d573b833f00d53eb8d8bf65d50bcd8daaea54c5d628906e084a434d", "d06d296cad7d428b56b25c53fdfcf558b5bc4211ed31cfe63732678a4f23338b582225286881986247793056741c08c9698c0c05206ba3e4d692922a0f061d17da276e3d562b3b9075cba4bc003c5abbdc6106a68b3fe9cbf5b1bf01695dea38dfe6cc548064753c68117e2daa44345594a4fa9d3595e8c61df7b8b76410e315" },
        { "3613be648e6da86b363598f8d3c622239c70374b154e9758be6ac9da", "5ed770cab9aa8cf1107dcb4fed42a48832e6a30d5b83c8f003d62af7a9f3dfefb5372042b67fa9ccee975feb10f635d548a8", "5c42ded57c112cfd1365227f99e993790bb7fef88e6e57b056840af1c6de3ed6d89e56c0c03dbe02ccd9e4de8fd084b13e8d296a5aa2f1408f1790a5dd029775f558f774e603c0f7f8352fe1d8fbe8d1f615809a2c053ef4a0662371eb7f27b27b18c6da358cbb435f200c70be934643fbf70a704cbfc8325018cb3d3a7d76f3" },
        { "9d0e55b33e5b72be9f073e5f4ef4bd8a46d8c18a128ec33a09a536f1", "e8ed22a9cea119cb930ddca9c2675ac8dafd3b7fea7616585e5be5b819d6b6c3c4b9a37a65c10326331de2faa92aaff4d981", "3d2edc42edc46ae84ef03cb26b60d56bfbc0c4af36391e96e7372b93c370609bf9228d890a9b58c1dbffea130946e8e37c84a5873f6d96f18e18068e31030a6d0958b8a4e96e94f0a7934b9b7209a4a3654eb3185da050c608c1c7143ff8d12c9658c35c314406bcd933c7124eb1eff2dfa7562a39db335d7dbad8ed95569c1e" },
        { "14e6f0b573b44c2899b9fbb916b3f0c3b44fced9088fede4add62cbf", "faa2e1ec2658570a8d1b60376f7f5241ed72781cbe1c61e1230a7e171d8ced32dd5b8f597c7f606cec7008462f7cad3fc668", "db0b6f188d114902a58112a506c653f52099b0955cd56d49ba8731aeb8450a2977c2b21d199f11616d8302f1ce6d5ae34849279dfdc23546c758d77a1fb2e3423e35ff4381d51b35122b3f97c5c887a140ca6c7b5fad44acd7630cd519a6669f798ddef28f5d7168dda25001b37fe118504307954a3ca36530d05b276dc99a28" },
        { "1f7e48a7c46fd9fc87160d68914a52cb4032e671fba07dcc8f684d84", "167b50360501077b3b91f9f612716bad0d661c2d0f191d17cabfe3db79f99bc4d2f9bd3536e7e4322c83dd210d0bfdd2015f", "07c15fd37f8f92a60640f55a8819868d81bc37f41eb137959f467236ff995b15d7f3e9d98057dfc9775b513c06376450211c55df8fc7bdaf4a5513e9275972cdb92c9ca3738440f50e4878fb2aa396573858200397551987fd8a826f19fb21fc7714811aa33d3a9291af72813c86a4098e17a1468083d483828dc9ae1da8f10a" },
        { "1c4944843c3ee7f98ab52987e0d2d4494aa72548bfdc3602db45b5f4", "b34a6eba59e632e3c334df582fd0b03aca7f641c6914920a79943dca3998a86127f36bdab795c7424baf37f76018472305f0a983928386", "d03dd94e43d96ca459928aa96d2b81c35ed54566e33e66351fca406b7620727a9ee991f2f9d41da322deb306c6d085d9b509080a387decd6a6fe513232bb386a07063708975a72f72f9cc6e8cfa147f53af1ed8449ca8a6f8468ff62f384b084321b3559c47053e7a9542a1733a5af5f15155d9ebd2c2847e491b3c26385c20f" },
        { "eabba9f35ba39cfb9283390d5425687cdd70d4cb1fea433925647c79", "2fd5863ab5ac0109cd1cc8a6598e75d85811a84d0df14cd55e8b1cce7a5f65dfbe670deadaa8d43b2f06da067c5c6210baccd5ac44540a", "85c02d7cfab29f8adf0fa55ef36722a04757c8865053d2af3ba2f64e80aa958aba6e3625b655325cca2db00f686fd422f2c53423d0c98c2dc110b20c6e67cca1455cc0888401ecf994ec18ec9982a8814776169ef78ca0dafaa33e9a2df2d779cd92b4ee8d3c3529e655c33daf270584ed72573fec23787e8f638240e4d320da" },
        { "9d2df0d7aebb6b08e816bef8df1bcf7d907622c617449346113dd317", "b67787d13434effcc68528109442b9418df025de59e5788e8fad1fda29adfceb3506ba95cdbc5e9548181f17b24a787c7a537b596b2268", "59282658eba93c8d2997b78c201d29700adac276c58668bc83d90a49c2a8324e7f52daaba65a81b5ce557e37230793b7a3c9c5fa8e2d6bca17e586f660e1509ba5b818a2b8494c8aada2d867d621ae2bf2d8a625a13689c71be468ac22ee33da3523874407bec6932f466d2521b2a2565dd10dffe6e9e93df69f5b74f0160293" },
        { "7d3cf647380947283bbc71d6a6f0f2aaeb1bd74a2cb0ec7b9a02c962", "112bb3c6f8d2e4f85235a7325d2cbb457336f9b9d3263619a4196b9237ab14bd8d6bd5b8198c637bc6dda2693cfb0b265fc451a9191044", "db2a58b070862fa950c80788b5cf8d5d443e31e492e5cda2e67c87314197f8d99bd68e60705c2e227b831e64c2ce23e5d220e6b64c2b9c4ca02ae6d419d3f6c61a87f4bda7cda08626fe387667429c01e3bab08941ff02741fccad810ef77d5cdea653701dbfcbfcdaa01d69ae1141dbfe7657067a4a4543e9c8a2e229ebd3c2" },
        { "b942bae8786a0addc6c61e27a776429b2a110fa913711b835aca58e4", "9dae3b463c86d64ce598ba9a567ca83783265e402d20c642b5176ee7339938d5c31d53d31f44692d735e1a5a6c30aadd92f5f669d9a2ad", "a7052e9a8f361a959f3f988e98e722a95c9e38f60a11a5a36bfb03708d3e8162ad32e4bc5a3dcc2bd8a43941c3a04d24f4b55488a93f01dadf4efabe03b398bcca5be7365edcf59eba4e49e6575cb137089a41e5221a98a3e13a65f307bf2ae1c62840f1e03c1c560de6d0649d42d4485ef807f7c0c9f16437674bf6e18ac139" },
        { "695c328dc858a46be7ac8b8debf58dd9a9bd72d7a408a43e6bbc69a3", "f3aa51ee90aa06e98e2388df7a3af2cd697f2a528f2a14140da405600b4a7b10076d0bb26c9d9ae66727d008f6dcca0f42140fb52ef3c6d930eb262160822de3", "2c070e5eb0a7efbfc40b234314c055fc436ce1e2300539b3784287fd3c4f947824c5e89aa3d933667dd4eb8587c33797ae6f0ccb3b8f95ad563dd940c8a7984d050775fa69d55b9ec23a19d40fc94fcf876a8eeddd96bb8ec3cab426ad353691d7b4ffa789012f119a28f45b333a6649d544b06f9d8271fcc7d162d7726338ed", },
        { "ee63e86aac85f36961d697451288cf780041eafe462ca6557035d92a", "a27f5ba4a0d5a80cb4ef9bb18d4ffc4ad487680cb5a8f6c69cfe1b293cfacb67d670e101b803c1d904dd3c8fb2a3b40ad8a78055296091adbc1856d810e5deb8", "802ed28873c2160562fb9162246b60d75e2ffb8cecc5bb08831e9aec94f43be1735e2acfb8a26450e64a1a644a7ab26626800f3e0fad12d9946360d620706682229a9b076a7b05e0d690ff902f8c39944ef2009683e2c0e33733c71fcfc34b38d40baf9851f7029f7df4c9509a4afe4a1dac58fa0f0dcd10b3b6562f6996f2e9", },
        { "5b25f50fd3ba3f628058d49d28cd6ba02fdda45d842d935ff9a8ead6", "489bd8b1eaa33c50e65f35208b752111d3896607e6b010e9e027bcf4d445b45611ae5bf8a4cacc9b1e75be2697f8ab3ef2383118632eb88b2f97556ab3edee10", "4b2b843db0ae2327dea304821a4d31bd65b55b52a34222f9fc89911d8366e88c154c9f7284d9a788f5aef389877d37e63663f0bafe79a40043a66b0470b7ae17dfb12f87f96549ce9e467a0ac7781ead69297d769d2408a0ffa3e059536598756f013c64557a92619f139fed20656d7cc8ae0c6ec86cd740f72bf804749b2f0e", },
        { "d6c24c2c68c3c879b141677702852fac78c88a39701de6bb8a466fee", "21556df80e1bba77a079a3f3c86ebcffdbcc209ee53c62604182c7ce6602b20804835fad5034d4fb9bec15382ab69850428df8bf8264edb1766a7c68688b6bdf", "908049137194295e174f2a0565dd5737dc8a5e3fb283416224e14f060de3531ab67b0bb1f00ddbf06073c32b1b448f4b73564d73108104e342a6a31c95f03844a65a62cd367209527d5c4cc1c019bbbf260ac748c8af769607b55c452230c6b4082538ae6a4b1a4a1512ae0f7fe5455c9facb307029600451c1560cadc2a6531", },
        { "929a3b33729126cea761fb2d23baf40757e77e0b3c5536dcceb68433", "f86fe94d5c6ac0accbb652d7eda9aa7559cf380f098ddbcf0a043f87783869f4aa729045aae32fd7e88958432f574b9ad79047ef3d74a016701e12ac2dad5557", "8cc53f92c6522670b1408bf15ef09e97131b7e12920ba1119e0b7558bc1c471237e9656966fd7f55313453b1d26f8a94d98cdcde73967b11eb38af637178bd111ba20ef529818d5b8dd55111ab027cbd5d0663733aadef87180843a0cf1b9e0e40d6835e9647182bcc0dcd3770103dbaa66f59f96fbac31b2b6278385d6db2b4", },
        { "0ae2eceff877842b3d77b4210d391f42151b696006c6974722f0bf46", "d83efe16354e973367648ce9c3127b6f7957b7da1914ed3456bf08cf095b51d879a8db2fd9971e761200e269c3975f6d861da26b76d332deee34f42b93e34c3294", "0df219910ce5b0bb03e05b21dd747a7413866d661ff4a66d347c02c42cd959b4c22e541101a87e07fc5f9e17c07e455c87a4a3132e8a9e5ae9e52638368ec2de0fe06f2c6461ab77c8a5cd8a6b7097ea481b0e5f9f3989afa67a09ad7ccfc8725ece15eb1d595e94cf7a8f82c5929ba7beb357525a4a8ff5d201542be5e393f9" },
        { "69269527fe9a121b9098c2472e1250577c66e1ef48a432059488f3ea", "b74637baa33d5a7443304b85aa7260f48c788b01008d397184c67926423961f2926aadadbf06aa850ed45396fce8b9bbc81aa8a331b68727817580dacaace1e270", "0c671581ca7648fc5717d4559b9e83b568a757af5d64872b0b35c4a25779c0911fa5f12721939341181cec9927e8d33a7b8573382db74d9d650f7ca6c5bf3d3005adae6ddac6d6db1d95e6772914be2e85c2b394c3f590d8a772daf88c3b593104a8d3babfb91bb1aeab4b5667c94d77cc85a02be7c758c3198567fe74941c7c" },
        { "818bdeb99de0067263ad93afcc03b7bb91e076d16d1cb2f2762636d9", "411356543a5c2819773ff2a93fb94894f575bf6eea4b5d7675084d74466dd7e8f77a9060e4ecce2a3d045f2b598a7b0f4d12f6a3be7eba7ef3c475e72ef18ff33d", "d1287e9a5a8f7f32ff9bb5c9d0a461acb7815474b8b358da78e1cbccfc858dc0b4e41d48b843fee651586022bbfffcdb292e40db0a2bacae8bfc7cb35d208773b87c6732a098dac4670b68a27334463d412b7b92aaf741ff6502ae55ceca932ee5d8a5819af6da73489d16707fad077b06604dc22081df760fd9aad9d7eb7b99" },
        { "55b681a0747e9f2db56762ff44129ae29c94f5ffb933156197dbc655", "f105508d6a45d37a80a7f963f6f927a9f5cf7594f3d2b248d95ba8f56441a04fcbef293ea3b916d60070c689de33a89bbf89fdd795ade081e129848b1da6be0b58", "eeabda87856e3ba98a57d619be748ff5508a2cbf50b66818665042cac4149b0c598400024788c584985853202c9bc262f6d1ab5269ee373f2c477cbb15b20ae755e9c0f1f030e5da7138747ddb42b6cde287165e5bd83227f2822e861b744d844ba4c25b67aec3357dbee037b2b193be7b7718e810bc7e571ec7ff31bca28d6b" },
        { "ef9157092c101ffe29e2770231cdb0721d3b90d2d0c08344eef32d70", "67f86b8af7c8bc9bfb568522f9dcadbf23990354e5197566bab882c64362f2709a9cc4b3e4c8689225a42e6b5915ffd6c0234bd07944421952dd47dadf06a18bc9", "9f552fdb9eb3464a75fe354c3c7e748dc257eb63d1b958a5b358f0c0f41c49076a19774b73c176ac7e58c8aecda95e9079f8bcd9b6375933587d447c96b30d1b0e91e3900ac9fa4f028130cf6207efde8cc05537216292567d63928b80b2dd4e94a4bbd2eb319bdd0960b5d68300ef200699fda69aa2cdb728f422153a7f7f48" },
        { "39eeb4ce5ac153717476531a8b0c1659bb77fb6bfea052c9b90e86e9", "f67ab51d4988b3e9d55f05b379acb03c84d498a42993982d2de3d243e31f44b95a689d56d5484e907b95e39750989495646f17344bb0b962cc4fad51ddc2a4c1d0", "69d557b16e874038598b25f616afeb4f4a900be7dd0d38b5b6fb4259c51a3aaf4748d7a445f518485ed72b25c7df8ed0906b74bd29bd6a5724ac3a503c990f3697a5db484821f68718470810862728a80ce34599a41fc5bd8bb46dd845a4812ae1532c457ef4211d0e41835e5a6f030247614822571c930c727ba397e723d6b3" },
        { "e51184d7cae852dd4464bf04cf3dc35b201d286a995d8c045ffb025c", "a629f7b9614bd8800a9a53196555fbd55174389105dea58b9adcb674cace9ec6f71d70576e70842fee231c9664f54772f90006d04eefaec1066b8d6786d6d04ff8", "8ed85b5f96565939cca9c21728f9ef816229019b59af6d9fb5a3b98acb158016c2a4bce9f44c450c9780949e9d79cdcad8cdfdf662439f3d3ce66620355836780e6973afdbe87f30acba7e1c6d285d3f0b7d1242d9d1252231206fe15e9f227bb94f091b13d2118f537e09f960b0a6c84262cae784000534dc785d340e914a05" },
        { "e9169a8bed8cac987699ee1e93910c43f6285d5ededf37b4ea99fbda", "fb396dfb0055d6c7cba8ff2275b9e569fa6189e38ed2eaed35d730548e5e2a03ef33fadeb70e0fede0d054bdd40aa70382bf8d83b06b2430934359959dbf5a7396", "73db1e155da94a056e175a070f7b994a888e2c6fc220f4e1510ab565b16377f2d0f72cc2cdf494a900e75ea63ae790b84e1028f2d03403d21cccffcf9a355d31411969255a2b5c118717812503b897332b8c77c03e66f65b61630d2de61f37d2cd291b678bb46be40b8f0cacf357776328eb5f009420942327b627a81b8bcf7c" },
    };

    for( auto const& input : inputs )
    {
        BOOST_TEST_EQ( digest<hmac_sha2_224>( from_hex( std::get<1>( input ) ), from_hex( std::get<2>( input) ) ), std::string( std::get<0>( input ) ) );
    }
}

int main()
{
    hmac_sha_256();
    hmac_sha_224();

    return boost::report_errors();
}
