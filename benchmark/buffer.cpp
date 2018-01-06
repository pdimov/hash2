#include <boost/hash2/fnv1a.hpp>
#include <boost/hash2/siphash.hpp>
#include <boost/hash2/xxhash.hpp>
#include <boost/hash2/spooky2.hpp>
#include <boost/hash2/murmur3.hpp>
#include <boost/hash2/md5.hpp>
#include <boost/hash2/sha1.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/get_integral_result.hpp>
#include <boost/chrono.hpp>
#include <boost/core/demangle.hpp>
#include <vector>
#include <cstdio>
#include <typeinfo>

template<class H> void test_( unsigned char const * p, int N, int M )
{
    H h;

    typedef boost::chrono::steady_clock clock_type;

    clock_type::time_point t1 = clock_type::now();

    for( int i = 0; i < M; ++i )
    {
        hash_append_range( h, p, p + N );
    }

    clock_type::time_point t2 = clock_type::now();

    long long ms = boost::chrono::duration_cast<boost::chrono::milliseconds>( t2 - t1 ).count();

    using boost::hash2::get_integral_result;
    unsigned r = get_integral_result<unsigned>( h.result() );

    std::printf( "%s (N=%d): %u: %lld ms, %.2f MB/s\n", boost::core::demangle( typeid( H ).name() ).c_str(), N, r, ms, 1000.0 * N * M / ms / 1048576 );
}

extern unsigned char data[];

void test( int N, int M )
{
    test_<boost::hash2::fnv1a_32>( data, N, M );
    test_<boost::hash2::fnv1a_64>( data, N, M );
    test_<boost::hash2::murmur3_32>( data, N, M );
    test_<boost::hash2::murmur3_128>( data, N, M );
    test_<boost::hash2::xxhash_32>( data, N, M );
    test_<boost::hash2::xxhash_64>( data, N, M );
    test_<boost::hash2::spooky2_128>( data, N, M );
    test_<boost::hash2::siphash_32>( data, N, M );
    test_<boost::hash2::siphash_64>( data, N, M );
    test_<boost::hash2::md5_128>( data, N, M );
    test_<boost::hash2::sha1_160>( data, N, M );
    test_<boost::hash2::hmac_md5_128>( data, N, M );
    test_<boost::hash2::hmac_sha1_160>( data, N, M );

    puts( "--" );
}

unsigned const N1 = 65536;
unsigned const M1 = 65535;

unsigned const N2 = 15;
unsigned const M2 = M1 * N1 / N2;

unsigned const N3 = 4;
unsigned const M3 = M1 * N1 / N3;

int main()
{
    test( N1, M1 );
    test( N2, M2 );
    test( N3, M3 );
}

unsigned char data[ N1 ];
