// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "../ctle/sockets.h"

#include "unit_tests.h"

#include <future>
#include <thread>

#include "../ctle/read_stream.h"
#include "../ctle/data_source.h"

#include "../ctle/write_stream.h"
#include "../ctle/data_destination.h"

#include "../ctle/ntup.h"

using namespace ctle;

TEST( data_stream, basic_test )
{
	// set up a list of random values
	const size_t value_list_size = 200;
	std::vector<std::unique_ptr<variant>> value_list(value_list_size);
	std::vector<size_t> positions(value_list_size);
	for( size_t inx=0; inx<value_list_size; ++inx )
	{
		value_list[inx] = random_variant();
		value_list[inx]->random();
	}

	// write the data to a file
	digest<128> digest1;
	if( true )
	{
		file_data_destination dd("./data_stream_basic_test.dat");
		write_stream<file_data_destination,hasher_xxh128> ws(dd);
		for( size_t inx=0; inx<value_list_size; ++inx )
		{
			auto res = value_list[inx]->write_to_stream( ws );
			ASSERT_EQ( res, status::ok );
			positions[inx] = ws.get_position();
		}
		ASSERT_EQ( ws.end(), status::ok );
		digest1 = ws.get_digest().value();
	}

	// read the data back to a second list
	digest<128> digest2;
	std::vector<std::unique_ptr<variant>> value_list2(value_list_size);
	std::vector<size_t> positions2(value_list_size);
	if( true )
	{
		file_data_source ds("./data_stream_basic_test.dat");
		read_stream<file_data_source,hasher_xxh128> rs(ds);

		for( size_t inx=0; inx<value_list_size; ++inx )
		{
			value_list2[inx] = new_variant( value_list[inx]->type() );
			auto res = value_list2[inx]->read_from_stream( rs );
			ASSERT_EQ( res, status::ok );
			positions2[inx] = rs.get_position();
		}
		EXPECT_TRUE( rs.has_ended() );
		digest2 = rs.get_digest().value();
	}

	// make sure the lists agree
	for( size_t inx=0; inx<value_list_size; ++inx )
	{
		EXPECT_TRUE( value_list[inx]->is_equal( *(value_list2[inx].get()) ) );
		EXPECT_EQ( positions[inx], positions2[inx] );
	}

}
