// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "unit_tests.h"

#include "../ctle/data_source.h"
#include "../ctle/file_funcs.h"

using namespace ctle;

TEST( data_source, file_test )
{
	const char *data_source_file = "data_source_file_test.dat";
	constexpr const size_t file_size = 100000;
	auto file_data = random_vector<u8>(file_size);

	// write a file using the write_file function
	ASSERT_EQ( write_file(data_source_file, file_data, true ), status::ok );

	// read file using a random number of block sizes and calls, and compare results
	std::vector<u8> read_buffer(file_size);
	if( true )
	{
		file_data_source ds(data_source_file);
		size_t read_bytes = 0;
		while( read_bytes < file_size )
		{
			u64 read_chunk_size = random_value<u64>() % 1000;
			auto result = ds.read( &read_buffer.data()[read_bytes], read_chunk_size );
			ASSERT_EQ( result.status(), status::ok );
			EXPECT_TRUE( result.value() <= read_chunk_size );
			read_bytes += result.value();
		}
		EXPECT_TRUE( ds.read( nullptr, 0 ).value() == 0 ); // making sure that 0 is fine with nullptr
		ASSERT_EQ( read_bytes, file_size );
	}

	EXPECT_TRUE( read_buffer == file_data );
}
