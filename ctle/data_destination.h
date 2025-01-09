// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_DATA_DESTINATION_H_
#define _CTLE_DATA_DESTINATION_H_

/// @file data_destination.h
/// @brief Data destination objects implement a write method and can be used for streaming data classes, e.g., write_stream.
/// @attention Blocking call which writes to the destination from a src_buffer, until the write_count bytes have been written, or an error occurs. On success, the method must return status::ok, and the actual number of bytes written to the destination.

#include "status.h"
#include "status_return.h"
#include "status_error.h"
#include "file_funcs.h"

namespace ctle
{

/// @brief Data destination objects for writing data to a file. Used as a destination for streaming data classes, e.g., write_stream to write out data to a file.
class file_data_destination
{
public:
	file_data_destination( const std::string &filepath, bool overwrite_existing = true );
	~file_data_destination();

	/// @brief Write from source buffer into file.
	/// 
	/// @param src_buffer the buffer to write from
	/// @param write_count the number of bytes to write
	/// @return status::ok, along with the number of bytes written, or an error status if the write failed.
	status_return<status, u64> write(const u8* src_buffer, u64 write_count);

private:
	_file_object file;
};

}
// namespace ctle

#ifdef CTLE_IMPLEMENTATION

#include "log.h"
#include "_macros.inl"

namespace ctle
{

file_data_destination::file_data_destination( const std::string &filepath, bool overwrite_existing )
{
	ctStatusCallThrow(this->file.open_write(filepath,overwrite_existing));
}

file_data_destination::~file_data_destination()
{
	this->file.close();
}

status_return<status, u64> file_data_destination::write(const u8* src_buffer, u64 write_count)
{
	if( write_count > 0 )
	{
		ctStatusCall(this->file.write(src_buffer, write_count));
	}

	return write_count;
}

}
// namespace ctle

#include "_undef_macros.inl"

#endif//CTLE_IMPLEMENTATION

#endif//_CTLE_DATA_DESTINATION_H_
