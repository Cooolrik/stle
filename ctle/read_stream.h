// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_READ_STREAM_H_
#define _CTLE_READ_STREAM_H_

/// @file read_stream.h
/// @brief A read-only input stream for streaming data sequentially, using a memory buffer, while also calculating a hash on the input stream.

#include <vector>

#include "fwd.h"
#include "status.h"
#include "status_return.h"
#include "hasher.h"
#include "file_funcs.h"

namespace ctle
{
/// @brief A read-only input stream with optional hashing
/// @details A read-only input stream which is designed for streaming data sequentially, using a 
/// memory buffer, while also calculating a hash on the input stream.
template<class _DataSourceTy, class _HashTy /* = hasher_noop<64> */>
class read_stream
{
	const size_t buffer_size = 2 * 1024 * 1024;

public:
	read_stream( _DataSourceTy &_data_source );
	~read_stream();

	using data_source_type = _DataSourceTy;
	using hasher_type = _HashTy;
	using hash_type = typename _HashTy::hash_type;

	/// @brief Get the current position/number of bytes read from the stream (not including any pre-read data in the read buffer)
	u64 get_position() const { return this->current_position; };
	
	/// @brief Read a single value from the stream. 
	/// @details Read a single value from the stream. If at end of the stream, an empty value is returned. (Note that for reading
	/// multiple values, the read method which takes a count is much more efficient.)
	/// @note The type must be trivially copyable, and memcpy is used since the source values may not be correctly aligned
	/// @note Make sure the type is correctly packed, since any alignment byte will also be read from the stream
	template<class _DataTy> _DataTy read();
	
	/// @brief Read a number of values from the stream. 
	/// @details Read a number of values from the stream. If at the end of the stream, an error is returned.
	/// @note The type must be trivially copyable, and memcpy is used since the source values may not be correctly aligned
	/// @note Make sure the type is correctly packed, since any alignment byte will also be read from the stream
	template<class _DataTy> status read(_DataTy* dest, size_t count = 1);

	/// @brief Read a raw byte stream into a memory area
	/// @param dest the destination memory area
	/// @param count the number of bytes to read
	/// @note dest must be a valid memory area of at least count bytes
	status read_bytes(u8* dest, size_t count);

	/// @brief Returns true if the stream has ended (eos/eof)
	bool has_ended() const;

	/// @brief Get the hash digest from the stream. 
	/// @note The hash value will be calculated when the stream has ended, any call before then will return an empty hash digest
	status_return<status,hash_type> get_digest() const { return hash_digest; };

private:
	u64 current_position = 0;
	size_t buffer_position = 0;
	size_t buffer_end = 0;
	std::vector<u8> buffer;

	data_source_type &data_source;
	hasher_type hasher;
	hash_type hash_digest;

	void read_from_buffer( u8* const dest, const size_t count );
	status fill_buffer();
};

}

#include "log.h"
#include "_macros.inl"

namespace ctle
{

template<class _DataSourceTy, class _HashTy>
inline read_stream<_DataSourceTy,_HashTy>::read_stream( _DataSourceTy &_data_source ) 
	: data_source(_data_source)
{
	this->buffer.resize(buffer_size);
	ctStatusCallThrow( this->fill_buffer() );
}

template<class _DataSourceTy, class _HashTy>
inline read_stream<_DataSourceTy,_HashTy>::~read_stream()
{
}

template<class _DataSourceTy, class _HashTy>
template<class _DataTy>
inline _DataTy read_stream<_DataSourceTy,_HashTy>::read()
{
	_DataTy ret = {};
	this->read<_DataTy>( &ret );
	return ret;
}

template<class _DataSourceTy, class _HashTy>
template<class _DataTy>
inline status read_stream<_DataSourceTy,_HashTy>::read(_DataTy* dest, size_t count)
{
	static_assert( std::is_trivially_copyable<_DataTy>() );

	ctStatusCall( this->read_bytes( (u8*)dest, sizeof(_DataTy)*count) );
	return status::ok;
}

template<class _DataSourceTy, class _HashTy>
inline status read_stream<_DataSourceTy,_HashTy>::read_bytes(u8* const dest, const size_t count)
{
	// if there are enough bytes left in the buffer, use these directly
	if( this->buffer_end - this->buffer_position > count )
	{
		this->read_from_buffer( dest, count );
	}
	else
	{
		// not enough bytes, split up the read in as many buffer reads are required
		size_t read_bytes = 0;
		while( read_bytes < count )
		{
			ctValidate(!this->has_ended(), status::cant_read) << "The stream ended before reading the desired data count" << ctValidateEnd;

			const size_t in_buffer = this->buffer_end - this->buffer_position;
			const size_t to_read = std::min(count - read_bytes , in_buffer);
		
			// copy from the buffer, and step up number of bytes read
			this->read_from_buffer( &dest[read_bytes], to_read );
			read_bytes += to_read;

			// if we are at the end of the buffer, fill it with new data
			if (this->buffer_position >= this->buffer_end)
				ctStatusCall(this->fill_buffer());
		}
	}

	this->current_position += count;
	return status::ok;
}

template<class _DataSourceTy, class _HashTy>
inline bool read_stream<_DataSourceTy,_HashTy>::has_ended() const
{
	// the definition of the end of the stream is that the buffer is not totally filled, since 
	// no more bytes can be read, and that the buffer_position has come to the end of the partially filled buffer
	if(this->buffer_end == this->buffer_size)
		return false;
	return this->buffer_position >= this->buffer_end;
}

template<class _DataSourceTy, class _HashTy>
inline void read_stream<_DataSourceTy,_HashTy>::read_from_buffer( u8* const dest, const size_t count )
{
	u8* const src = this->buffer.data();
	memcpy(dest, &src[this->buffer_position], count);
	this->buffer_position += count;
}

template<class _DataSourceTy, class _HashTy>
inline status read_stream<_DataSourceTy,_HashTy>::fill_buffer()
{
	u8* const buffer_data = this->buffer.data();
	const size_t buffer_count = buffer_end - buffer_position;

	// move whatever is left in the buffer to the beginning
	if (buffer_count > 0)
		memcpy( (void*)buffer_data, (void*)&buffer_data[buffer_position], buffer_count);
	buffer_position = 0;
	buffer_end = buffer_count;

	// fill up with new data. 
	const size_t fill_start = buffer_end;
	const size_t fill_count = this->buffer.size() - buffer_end;
	size_t read_count = 0;
	ctStatusReturnCall(read_count, this->data_source.read(&buffer_data[fill_start], fill_count));
	buffer_end += read_count;

	// update hash digest
	this->hasher.update(&buffer_data[fill_start], read_count);

	// if at the end of the stream, get the final hash value
	if (read_count < fill_count)
		ctStatusReturnCall( this->hash_digest , this->hasher.finish() );
	
	return status::ok;
}

}
// namespace ctle

#include "_undef_macros.inl"
#endif//_CTLE_READ_STREAM_H_
