// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "../ctle/sockets.h"

#include "unit_tests.h"

#include <future>
#include <thread>

#include "../ctle/data_destination.h"

using namespace ctle;

TEST( data_destination, file_test )
{
	const char *data_destination_file = "data_destination_file_test.dat";
	constexpr const size_t file_size = 100000;
	auto file_data = random_vector<u8>(file_size);

	// write file using a random number of block sizes and calls
	if( true )
	{
		file_data_destination ds(data_destination_file);
		size_t written_bytes = 0;
		while( written_bytes < file_size )
		{
			u64 write_chunk_size = random_value<u64>() % 1000;
			write_chunk_size = std::min( write_chunk_size, file_size - written_bytes ); // cap at size of file
			auto result = ds.write( &file_data.data()[written_bytes], write_chunk_size );
			ASSERT_EQ( result.status(), status::ok );
			EXPECT_TRUE( result.value() <= write_chunk_size );
			written_bytes += result.value();
		}
		EXPECT_TRUE( ds.write( nullptr, 0 ).value() == 0 ); // making sure that 0 is fine with nullptr
		ASSERT_EQ( written_bytes, file_size );
	}

	// write a file using the write_file function
	std::vector<u8> read_buffer(file_size);
	ASSERT_EQ( read_file(data_destination_file, read_buffer ), status::ok );

	EXPECT_TRUE( read_buffer == file_data );
}
