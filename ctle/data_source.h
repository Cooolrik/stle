// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_DATA_SOURCE_H_
#define _CTLE_DATA_SOURCE_H_

/// @file data_source.h
/// @brief Data source objects implement a read method, and can be used for streaming data classes, e.g. read_stream
/// @attention Blocking call which reads from the source to dest_buffer, until the read_count bytes have been read, the source is at an end, or an error occurs. On succes, the method must return status::ok, and the actual number of bytes read from the source. The method should continue to return status::ok and 0 if called after reaching the end of the data source (eos/eof).

#include "status.h"
#include "status_return.h"
#include "status_error.h"
#include "file_funcs.h"

namespace ctle
{

/// @brief Data source objects for reading data from a file. Used as a source for streaming data classes, e.g., read_stream to read in data from a file.
class file_data_source
{
public:
	file_data_source( const std::string &filepath );
	~file_data_source();

	/// @brief read from source into dest_buffer, return number of bytes actually read
	/// 
	/// @param dest_buffer the buffer to read into
	/// @param read_count the number of bytes to read
	/// @return status::ok, along with the number of bytes read, or an error status if the read failed.
	status_return<status, u64> read(u8* dest_buffer, u64 read_count);

private:
	u64 file_position = 0;
	_file_object file;
};

}
// namespace ctle

#ifdef CTLE_IMPLEMENTATION

#include "log.h"
#include "_macros.inl"

namespace ctle
{

file_data_source::file_data_source( const std::string &filepath )
{
	ctStatusCallThrow(this->file.open_read(filepath));
}

file_data_source::~file_data_source()
{
	this->file.close();
}

status_return<status, u64> file_data_source::read(u8* dest_buffer, u64 read_count)
{
	// cap the read size to the size of the file
	const u64 data_left = (u64)this->file.size() - this->file_position;
	const u64 read_size = std::min( read_count, data_left );

	if( read_size > 0 )
	{
		ctStatusCall(this->file.read(dest_buffer, read_size));
		this->file_position += read_size;
	}

	return read_size;
}

}
// namespace ctle

#include "_undef_macros.inl"

#endif//CTLE_IMPLEMENTATION

#endif//_CTLE_DATA_SOURCE_H_
