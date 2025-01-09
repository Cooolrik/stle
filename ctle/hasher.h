// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_HASHER_H_
#define _CTLE_HASHER_H_

/// @file hasher.h
/// @brief Hasher function wrappers for hashing data streams.
/// 
/// The hasher_[...] classes are wrappers around popular hash functions, and provide a simple interface for hashing data streams.
/// 
/// Each hasher class implements the following functions:
/// - ctor() to initialize the hasher
/// - update() to update the hash with a block of bytes
/// - finish(), to end the hashed stream, and return the final hash
/// @note The hashers are implemented using external libraries. All declarations exist, but to implement a specific library, include 
/// the library header before including hasher.h in the implementation source file. (see the example implementation in the 
/// documentation for ctle.h for more information).

#include "digest.h"
#include "status.h"
#include "status_return.h"

namespace ctle
{

/// @brief Implementation of a no-operation hasher class template, for use when hashing is not needed. Always returns a zero hash on finish.
/// @tparam _Size the size of the hash in bits, must be 64, 128, 256 or 512.
/// @note This hasher is used when hashing is not needed, but the interface requires a hasher to be passed, and can be used as a template default parameter.
template <size_t _Size>
class hasher_noop
{
	status _update(const uint8_t* /*data*/, size_t /*size*/) { return status::ok; }

public:
	hasher_noop() {};
	~hasher_noop() {};
	using hash_type = digest<_Size>;

	/// @brief Update the hash with a block of bytes.
	/// @param data the data to add to the hash
	/// @param size the size of the data in bytes
	/// @return status::ok if the update was successful	
	status update(const uint8_t* data, size_t size) { return _update(data,size); }

	/// @brief Finish the hash generation and return the final hash value.
	/// @return status::ok if the update was successful, and the final hash value	
	status_return<status, digest<_Size>> finish() { return digest<_Size>(); }
};

/// @brief Implementation of a SHA-256 hasher, using picosha2.
/// @note To use, include picosha2 in the build before hasher.h to implement (see the example implementation in the documentation for ctle.h).
class hasher_sha256
{
public:
	hasher_sha256();
	~hasher_sha256();
	using hash_type = digest<256>;

	/// @copydoc hasher_noop::update
	status update(const uint8_t* data, size_t size);

	/// @copydoc hasher_noop::finish
	status_return<status, digest<256>> finish();

private:
	void *context = nullptr;
};

/// @brief Implementation of XXH3 XXH64 hasher, using xxHash.
/// @note To use, include xxHash in the build before hasher.h to implement (see the example implementation in the documentation for ctle.h).
class hasher_xxh64
{
public:
	hasher_xxh64();
	~hasher_xxh64();
	using hash_type = digest<64>;

	/// @copydoc hasher_noop::update
	status update(const uint8_t* data, size_t size);

	/// @copydoc hasher_noop::finish
	status_return<status, digest<64>> finish();

private:
	void *context = nullptr;
};

/// @brief Implementation of XXH3 XXH128 hasher, using xxHash.
/// @note To use, include xxHash in the build before hasher.h to implement (see the example implementation in the documentation for ctle.h).
class hasher_xxh128
{
public:
	hasher_xxh128();
	~hasher_xxh128();
	using hash_type = digest<128>;

	/// @copydoc hasher_noop::update
	status update(const uint8_t* data, size_t size);

	/// @copydoc hasher_noop::finish
	status_return<status, digest<128>> finish();

private:
	void *context = nullptr;
};

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef CTLE_IMPLEMENTATION

namespace ctle
{

////////////////////////////////////////

#ifdef PICOSHA2_H

hasher_sha256::hasher_sha256()
{
	this->context = (void*) new picosha2::hash256_one_by_one();
}

hasher_sha256::~hasher_sha256()
{
	delete ((picosha2::hash256_one_by_one*)this->context);
}

status hasher_sha256::update(const uint8_t* data, size_t size)
{
	((picosha2::hash256_one_by_one*)this->context)->process( data, data + size );
	return status::ok;
}

status_return<status,digest<256>> hasher_sha256::finish()
{
	digest<256> ret;
	((picosha2::hash256_one_by_one*)this->context)->finish();
	((picosha2::hash256_one_by_one*)this->context)->get_hash_bytes( ret.data, ret.data+32 );
	return ret;
}

#endif//PICOSHA2_H

////////////////////////////////////////

#ifdef XXHASH_H_5627135585666179

hasher_xxh64::hasher_xxh64()
{
	this->context = (void*)XXH3_createState();
	XXH3_64bits_reset((XXH3_state_t*)this->context);
}

hasher_xxh64::~hasher_xxh64()
{
	XXH3_freeState((XXH3_state_t*)this->context);
}

status hasher_xxh64::update(const uint8_t* data, size_t size)
{
	XXH3_64bits_update((XXH3_state_t*)this->context, data, size);
	return status::ok;
}

status_return<status,digest<64>> hasher_xxh64::finish()
{
	XXH64_hash_t result = XXH3_64bits_digest((XXH3_state_t*)this->context);

	XXH64_canonical_t canonical;
	XXH64_canonicalFromHash( &canonical, result );

	digest<64> ret;
	memcpy( ret.data, canonical.digest, sizeof(ret.data) );
	return ret;
}

///////////////////

hasher_xxh128::hasher_xxh128()
{
	this->context = (void*)XXH3_createState();
	XXH3_128bits_reset((XXH3_state_t*)this->context);
}

hasher_xxh128::~hasher_xxh128()
{
	XXH3_freeState((XXH3_state_t*)this->context);
}

status hasher_xxh128::update(const uint8_t* data, size_t size)
{
	XXH3_128bits_update((XXH3_state_t*)this->context, data, size);
	return status::ok;
}

status_return<status,digest<128>> hasher_xxh128::finish()
{
	XXH128_hash_t result = XXH3_128bits_digest((XXH3_state_t*)this->context);

	XXH128_canonical_t canonical;
	XXH128_canonicalFromHash( &canonical, result );

	digest<128> ret;
	memcpy( ret.data, canonical.digest, sizeof(ret.data) );
	return ret;
}

#endif//XXHASH_H_5627135585666179

////////////////////////////////////////

}

#endif//CTLE_IMPLEMENTATION

#endif//_CTLE_HASHER_H_
