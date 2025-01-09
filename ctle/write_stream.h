// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_WRITE_STREAM_H_
#define _CTLE_WRITE_STREAM_H_

#include <vector>

#include "fwd.h"
#include "status.h"
#include "status_return.h"
#include "hasher.h"
#include "file_funcs.h"

namespace ctle
{
// base class for a write_stream, a read-only input stream which is designed for 
// streaming data sequentially, using a memory buffer, while also calculating a hash on the input stream.
template<class _DataDestTy, class _HashTy /* = hasher_noop<64> */>
class write_stream
{
	const size_t buffer_size = 2 * 1024 * 1024;

public:
	write_stream( _DataDestTy &_data_dest );
	~write_stream();

	using data_destination_type = _DataDestTy;
	using hasher_type = _HashTy;
	using hash_type = typename _HashTy::hash_type;

	// Get the current position/number of bytes written to the stream (not the actual bytes written to the destination, which may be less because of cacheing)
	u64 get_position() const { return this->current_position; };
	
	// Writes a single value to the stream. (Note that for writing multiple values, the write method which takes a pointer and a count is much more efficient.)
	// Caveat! The type must be trivially copyable, memcpy is used since the dest cache data values may not be correctly aligned
	// Caveat! Make sure the type is correctly packed, since any alignment byte will also be written to the stream
	template<class _DataTy> status write(const _DataTy& src);

	// Write a number of values to the stream. Returns status::ok on success, or an error value on erro
	// Caveat! The type must be trivially copyable, memcpy is used since the dest cache data values may not be correctly aligned
	// Caveat! Make sure the type is correctly packed, since any alignment byte will also be written to the stream
	template<class _DataTy> status write(const _DataTy* src, size_t count = 1);

	// Write raw bytes to the stream 
	status write_bytes( const u8* src, size_t count);

	// Ends the stream, flushes the destination, and calculates the final hash. 
	status end();

	// Get the hash digest from the stream. Note that the hash value will be 
	// calculated when the stream has ended. any call before then will return an empty hash digest
	status_return<status,hash_type> get_digest() const { return hash_digest; };

private:
	u64 current_position = 0;
	size_t buffer_position = 0;
	std::vector<u8> buffer;

	data_destination_type &data_dest;
	hasher_type hasher;
	hash_type hash_digest;

	void write_to_buffer( const u8 *src, size_t count );
	status write_to_destination( const u8 *src, size_t count );
	status flush_buffer();
};

}

#include "log.h"
#include "_macros.inl"

namespace ctle
{

template<class _DataDestTy, class _HashTy>
inline write_stream<_DataDestTy,_HashTy>::write_stream( _DataDestTy &_data_dest ) 
	: data_dest(_data_dest)
{
	this->buffer.resize(buffer_size);
}

template<class _DataDestTy, class _HashTy>
inline write_stream<_DataDestTy,_HashTy>::~write_stream()
{
	this->end();
}

template<class _DataDestTy, class _HashTy>
template<class _DataTy>
inline status write_stream<_DataDestTy,_HashTy>::write( const _DataTy& src )
{
	return this->write( &src, 1);
}

template<class _DataDestTy, class _HashTy>
template<class _DataTy>
inline status write_stream<_DataDestTy,_HashTy>::write( const _DataTy* src, size_t count)
{
	static_assert( std::is_trivially_copyable<_DataTy>() );

	ctStatusCall( this->write_bytes( (const u8*)src, sizeof(_DataTy)*count) );
	return status::ok;
}

template<class _DataDestTy, class _HashTy>
inline status write_stream<_DataDestTy,_HashTy>::write_bytes(const u8* src, size_t count)
{
	// if the write fits in the buffer, use the buffer
	if( (buffer_size - this->buffer_position) > count )
	{
		this->write_to_buffer( src, count );
	}
	else
	{
		// the data does not fit in the buffer. flush the current buffer to the destination
		ctStatusCall(this->flush_buffer());

		// write to buffer if it now fits, else skip the buffer and write to the destination directly
		if( (buffer_size - this->buffer_position) > count )
		{
			this->write_to_buffer( src, count );
		}
		else
		{
			ctStatusCall(this->write_to_destination(src,count));
		}
	}

	this->current_position += count;
	return status::ok;
}

template<class _DataDestTy, class _HashTy>
inline status write_stream<_DataDestTy,_HashTy>::end()
{
	ctStatusCall(this->flush_buffer());
	ctStatusReturnCall( this->hash_digest , this->hasher.finish() );
	return status::ok;
}

template<class _DataDestTy, class _HashTy>
inline void write_stream<_DataDestTy,_HashTy>::write_to_buffer( const u8 *src, size_t count )
{
	u8* const dest = this->buffer.data();
	memcpy(&dest[this->buffer_position], src, count);
	this->buffer_position += count;
}

template<class _DataDestTy, class _HashTy>
inline status write_stream<_DataDestTy,_HashTy>::write_to_destination( const u8 *src, size_t count )
{
	ctStatusCall(this->hasher.update(src, count));
	size_t written_count = 0;
	ctStatusReturnCall(written_count, this->data_dest.write(src, count));
	ctValidate( written_count == count, status::cant_write ) << "The write operation failed. " << written_count << " of " << count << " bytes were written." << ctValidateEnd;
	return status::ok;
}

template<class _DataDestTy, class _HashTy>
inline status write_stream<_DataDestTy,_HashTy>::flush_buffer()
{
	if( this->buffer_position > 0 )
		ctStatusCall( this->write_to_destination( this->buffer.data(), this->buffer_position ) );
	this->buffer_position = 0;

	return status::ok;
}

}
// namespace ctle

#include "_undef_macros.inl"
#endif//_CTLE_WRITE_STREAM_H_
