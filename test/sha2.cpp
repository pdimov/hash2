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

int main()
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


    return boost::report_errors();
}
