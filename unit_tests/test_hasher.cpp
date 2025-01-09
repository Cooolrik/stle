// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "../ctle/hasher.h"
#include "../ctle/string_funcs.h"

#include "unit_tests.h"

#include <map>
#include <unordered_map>

using namespace ctle;

// testing hex string: 4E6F626F6479206578706563747320746865205370616E69736820496E717569736974696F6E21
static const u8 hashing_testdata[] = {  0x4E,0x6F,0x62,0x6F,0x64,0x79,0x20,0x65,0x78,0x70,0x65,0x63,0x74,0x73,0x20,0x74,
										0x68,0x65,0x20,0x53,0x70,0x61,0x6E,0x69,0x73,0x68,0x20,0x49,0x6E,0x71,0x75,0x69,
										0x73,0x69,0x74,0x69,0x6F,0x6E,0x21 };

template<class _Ty>
void test_expected_hash( const u8 *srcdata, size_t size, const char *expected_hash_string )
{
	using hash = _Ty::hash_type;

	status result;
	_Ty hasher;

	result = hasher.update( srcdata, size );
	EXPECT_EQ( result, status::ok );
	auto return_value = hasher.finish();
	EXPECT_EQ( return_value.status(), status::ok );
	auto hash_digest = return_value.value();

	hash expected_hash = from_string<hash>(expected_hash_string);
	EXPECT_EQ( memcmp( hash_digest.data, expected_hash.data, sizeof(hash) ), 0 );

	std::stringstream sts;
	sts << hash_digest;
	hash hashfromstring = from_string<hash>( sts.str() );
	EXPECT_TRUE( hash_digest == hashfromstring );
}

TEST( hasher, test_expected_hash_values )
{
	test_expected_hash<hasher_sha256>(hashing_testdata,sizeof(hashing_testdata),"0A2591AAF3340AD92FAECBC5908E74D04B51EE5D2DEEE78F089F1607570E2E91");
	test_expected_hash<hasher_xxh64>(hashing_testdata,sizeof(hashing_testdata),"625A8B25C833FD36");
	test_expected_hash<hasher_xxh128>(hashing_testdata,sizeof(hashing_testdata),"828D13C68D1BAC3AA5AA63C0925F9C1E");

	// test no-op hashers as well, expect zero strings
	test_expected_hash<hasher_noop<64>>(hashing_testdata,sizeof(hashing_testdata) ,"0000000000000000");
	test_expected_hash<hasher_noop<128>>(hashing_testdata,sizeof(hashing_testdata),"00000000000000000000000000000000");
	test_expected_hash<hasher_noop<256>>(hashing_testdata,sizeof(hashing_testdata),"0000000000000000000000000000000000000000000000000000000000000000");
	test_expected_hash<hasher_noop<512>>(hashing_testdata,sizeof(hashing_testdata),"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
}

template<class _Ty>
typename _Ty::hash_type calc_hash_with_blocksize( const u8 *srcdata, size_t size, size_t block_size )
{
	_Ty hasher;
	size_t total_hashed = 0;
	while( total_hashed < size )
	{
		size_t to_hash = std::min( size - total_hashed , block_size );
		hasher.update( &srcdata[total_hashed], to_hash );
		total_hashed += to_hash;
	}
	return hasher.finish().value();
}

template<class _Ty>
void test_hash_determenism( const u8 *srcdata, size_t size, size_t block_size1, size_t block_size2 )
{
	auto hash1 = calc_hash_with_blocksize<_Ty>( srcdata, size, block_size1 );
	auto hash2 = calc_hash_with_blocksize<_Ty>( srcdata, size, block_size2 );
	EXPECT_EQ( hash1 , hash2 );
}

TEST( hasher, test_determenism )
{
	const size_t random_data_size = ((size_t)(random_value<u32>() % 4000000)) + 1000000;
	std::vector<u8> random_data(random_data_size);
	for( size_t inx=0; inx<random_data_size; ++inx )
	{
		random_data[inx] = random_value<u8>();
	}

	const size_t block_size1 = (size_t)random_value<u16>() + 100;
	const size_t block_size2 = (size_t)random_value<u16>() + 100;

	test_hash_determenism<hasher_sha256>( random_data.data(), random_data.size(), block_size1, block_size2 );
	test_hash_determenism<hasher_xxh64>( random_data.data(), random_data.size(), block_size1, block_size2 );
	test_hash_determenism<hasher_xxh128>( random_data.data(), random_data.size(), block_size1, block_size2 );
}
