# Hash2, a hash function library

This library contains an extensible framework for implementing
hashing algorithms that can support user-defined types, based on the paper
["Types don't know #"](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3980.html)
by Howard Hinnant, Vinnie Falco and John Bytheway.

It also contains implementations of several popular hashing algorithms:

* [FNV-1a](https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function)
* [MurmurHash 3](https://github.com/aappleby/smhasher/wiki/MurmurHash3)
* [xxHash](https://cyan4973.github.io/xxHash/)
* [SpookyHash v2](http://burtleburtle.net/bob/hash/spooky.html)
* [SipHash](https://131002.net/siphash/)
* [MD5](https://tools.ietf.org/html/rfc1321)
* [SHA-1](https://tools.ietf.org/html/rfc3174)
* [HMAC-MD5 and HMAC-SHA1](https://tools.ietf.org/html/rfc2104)

The hashing algorithms conform to the following concept:

```
struct HashAlgorithm
{
    typedef /*integral or array<byte_type, N>*/ result_type; // result type
    typedef /*unsigned integral*/ size_type; // how is container.size() hashed

    HashAlgorithm(); // default-constructible
    explicit HashAlgorithm( uint64_t seed ); // seed-constructible
    HashAlgorithm( byte_type const* seed, ptrdiff_t n ); // seed-constructible

    HashAlgorithm( HashAlgorithm const& r ); // copy-constructible
    HashAlgorithm& operator=( HashAlgorithm const& r ); // assignable

    void update( byte_type const* data, ptrdiff_t n ); // feed bytes

    result_type result(); // obtain result; also advances state,
                          // and can be called multiple times
};
```

## Supported compilers

The library supports C++03 and does not require C++11. The following compilers:

* g++ 4.4 or later
* clang++ 3.3 or later
* Visual Studio 2008 and above

are being tested on [Travis](https://travis-ci.org/pdimov/hash2/) and [Appveyor](https://ci.appveyor.com/project/pdimov/hash2/).

In addition, Visual Studio 2005 is also supported.

## Documentation

None yet.

## Planned Additions

* Documentation (obv.)
* SHA2-256, 384, 512
* Support for endian-independent hashing
* An `std::hash`-compatible adaptor
* A type-erased `hash_function`
* A `hash2sum` example

## License

Distributed under the [Boost Software License, Version 1.0](http://boost.org/LICENSE_1_0.txt).
