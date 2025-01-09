// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_THREAD_SAFE_MAP_H_
#define _CTLE_THREAD_SAFE_MAP_H_

#include <unordered_map>
#include <mutex>
#include "readers_writer_lock.h"

namespace ctle
{
// thread safe map, forces single access to map. performace as if single threaded access.
// (this code should be upgraded to a lock free map for multi-core performance)
template<class _Kty, class _Ty> class thread_safe_map
{
private:
	using map_type = std::unordered_map<_Kty, _Ty>;
	using key_type = _Kty;
	using mapped_type = _Ty;
	using iterator = typename map_type::iterator;
	using const_iterator = typename map_type::const_iterator;
	using value_type = std::pair<const _Kty, _Ty>;

	map_type Data;
	readers_writer_lock AccessLock;

public:
	bool has( const _Kty &key )
	{
		readers_writer_lock::read_guard guard( this->AccessLock );

		const_iterator it = this->Data.find( key );
		return it != this->Data.end();
	}

	std::pair<_Ty, bool> get( const _Kty &key )
	{
		readers_writer_lock::read_guard guard( this->AccessLock );

		const_iterator it = this->Data.find( key );
		if( it != this->Data.end() )
		{
			return std::make_pair( it->second, true );
		}
		return std::make_pair( _Ty(), false );
	}

	void clear()
	{
		readers_writer_lock::write_guard guard( this->AccessLock );

		this->Data.clear();
	}

	bool insert( const value_type &value )
	{
		readers_writer_lock::write_guard guard( this->AccessLock );

		return this->Data.insert( value ).second;
	}

	bool insert( value_type &&value )
	{
		readers_writer_lock::write_guard guard( this->AccessLock );

		return this->Data.insert( value ).second;
	}

	size_t erase( const _Kty &key )
	{
		readers_writer_lock::write_guard guard( this->AccessLock );

		return this->Data.erase( key );
	}

	size_t size()
	{
		readers_writer_lock::read_guard guard( this->AccessLock );

		return this->Data.size();
	}
};

}
//namespace ctle
#endif//_CTLE_THREAD_SAFE_MAP_H_
