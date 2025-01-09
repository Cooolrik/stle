// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "../ctle/digest.h"
#include "../ctle/string_funcs.h"

#include "unit_tests.h"

#include <map>
#include <unordered_map>

using namespace ctle;

template<size_t _Size>
static digest<_Size> random_hash()
{
	digest<_Size> val;
	for( size_t inx=0; inx<_Size/64; ++inx )
	{
		val._data_q[inx] = random_value<uint64_t>();
	}
	return val;
}

TEST( hash, basic_test )
{
	using hash = ctle::digest<256>;

	const hash hsh0 = from_string<hash>( "0000000000000000000000000000000000000000000000000000000000000000" ); // lowest
	const hash hsh1 = from_string<hash>( "0000000000000000ffffffffffffffffffffffffffffffffffffffffffffffff" );
	const hash hsh2 = from_string<hash>( "00000200ffffffffffffffffffffffffffffffffffffffffffffffffffffffff" );
	const hash hsh3 = from_string<hash>( "ffffffffffffffffffffffffffffffffffff0000000000000000000000000000" ); // highest

	std::string hshstr = to_string( hsh1 );
	auto hshval2 = from_string<hash>( hshstr.c_str() );
	EXPECT_TRUE( hsh1 == hshval2 );

	std::stringstream str;
	str << hsh2;
	auto hshval3 = from_string<hash>( str.str().c_str() );
	EXPECT_TRUE( hsh2 == hshval3 );

	auto h2 = hsh3;
	EXPECT_TRUE( h2 == hsh3 );

	std::vector<hash> a = { random_hash<hash::digest_size>() , random_hash<hash::digest_size>() };
	std::vector<hash> b;
	EXPECT_FALSE( a == b );
	EXPECT_TRUE( a != b );
	EXPECT_TRUE( b.empty() );

	b = std::move( a );
	EXPECT_FALSE( a == b );
	EXPECT_TRUE( a != b );
	EXPECT_TRUE( a.empty() );

	a = b;
	EXPECT_TRUE( a == b );
	EXPECT_FALSE( a != b );

	// test ordering
	if( true )
	{
		EXPECT_TRUE( hsh0._data_q[0] == 0 && hsh0._data_q[1] == 0 );
		EXPECT_TRUE( hsh1._data_q[0] == 0 && hsh1._data_q[1] != 0 );
		EXPECT_TRUE( hsh2._data_q[0] != 0 && hsh2._data_q[1] != 0 );
		EXPECT_TRUE( hsh3._data_q[0] == 0xffffffffffffffff && hsh3._data_q[3] == 0 );

		EXPECT_TRUE( hsh0 != hsh1 );
		EXPECT_TRUE( hsh1 != hsh0 );
		EXPECT_TRUE( hsh0 < hsh1 );
		EXPECT_FALSE( hsh1 < hsh0 );
		EXPECT_FALSE( hsh0 == hsh1 );
		EXPECT_FALSE( hsh1 == hsh0 );

		EXPECT_TRUE( std::hash<hash>{}( hsh0 ) != std::hash<hash>{}( hsh1 ) );
		EXPECT_TRUE( std::hash<hash>{}( hsh1 ) != std::hash<hash>{}( hsh0 ) );
	}

	// test lookup in map
	if( true )
	{
		std::map<hash, std::string> idstrmap;
		idstrmap.insert( std::pair<hash, std::string>( hsh0, "hsh0" ) );
		idstrmap.insert( std::pair<hash, std::string>( hsh1, "hsh1" ) );
		idstrmap.insert( std::pair<hash, std::string>( hsh2, "hsh2" ) );
		idstrmap.insert( std::pair<hash, std::string>( hsh3, "hsh3" ) );
		EXPECT_EQ( idstrmap.size(), 4 );

		EXPECT_EQ( idstrmap.find( hsh0 )->second, "hsh0" );
		EXPECT_EQ( idstrmap.find( hsh1 )->second, "hsh1" );
		EXPECT_EQ( idstrmap.find( hsh2 )->second, "hsh2" );
		EXPECT_EQ( idstrmap.find( hsh3 )->second, "hsh3" );
	}

	// test lookup in unordered_map
	if( true )
	{
		std::unordered_map<hash, std::string> idstrmap;
		idstrmap.insert( std::pair<hash, std::string>( hsh0, "hsh0" ) );
		idstrmap.insert( std::pair<hash, std::string>( hsh1, "hsh1" ) );
		idstrmap.insert( std::pair<hash, std::string>( hsh2, "hsh2" ) );
		idstrmap.insert( std::pair<hash, std::string>( hsh3, "hsh3" ) );
		EXPECT_EQ( idstrmap.size(), 4 );

		EXPECT_EQ( idstrmap.find( hsh0 )->second, "hsh0" );
		EXPECT_EQ( idstrmap.find( hsh1 )->second, "hsh1" );
		EXPECT_EQ( idstrmap.find( hsh2 )->second, "hsh2" );
		EXPECT_EQ( idstrmap.find( hsh3 )->second, "hsh3" );
	}
	
	// insert a number of generated values.
	if( true )
	{
		std::map<hash, hash> idmap;
		for( size_t inx = 0; inx < 1000; ++inx )
		{
			hash myid = random_hash<hash::digest_size>();
			idmap[myid] = myid;
		}
		EXPECT_EQ( idmap.size(), 1000 );
	}
}

template<size_t _Size>
static void test_hash_of_size()
{
	using hash = ctle::digest<_Size>;

	std::vector<hash> a = { random_hash<hash::digest_size>() , random_hash<hash::digest_size>() };
	std::vector<hash> b;
	EXPECT_FALSE( a == b );
	EXPECT_TRUE( a != b );
	EXPECT_TRUE( b.empty() );

	b = std::move( a );
	EXPECT_FALSE( a == b );
	EXPECT_TRUE( a != b );
	EXPECT_TRUE( a.empty() );

	a = b;
	EXPECT_TRUE( a == b );
	EXPECT_FALSE( a != b );

	hash q1 = random_hash<hash::digest_size>();
	auto q1string = ctle::to_string(q1);
	hash q2 = ctle::from_string<hash>(q1string);

	bool success = true;
	hash q3 = ctle::from_string<hash>(q1string,success);
	EXPECT_TRUE( success );

	hash q4 = ctle::from_string<hash>("invalid",success);
	EXPECT_FALSE( success );

	success = true;
	hash q5 = ctle::hex_string_to_value<hash>(q1string.c_str(),success);
	EXPECT_TRUE( success );

	hash qempty;
	for( size_t inx=0; inx<_Size/8; ++inx )
	{
		EXPECT_EQ( qempty.data[inx] , 0 );
	}

	EXPECT_TRUE( q1 == q2 );
	EXPECT_FALSE( q1 == qempty );
	EXPECT_TRUE( q1 == q3 );
	EXPECT_FALSE( q1 == q4 );
	EXPECT_TRUE( q4 == qempty );
	EXPECT_TRUE( q1 == q5 );
	EXPECT_FALSE( q5 == qempty );
}

TEST( hash, all_sizes )
{
	test_hash_of_size<64>();
	test_hash_of_size<128>();
	test_hash_of_size<256>();
	test_hash_of_size<512>();
}
