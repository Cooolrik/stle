// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_MULTITHREAD_POOL_H_
#define _CTLE_MULTITHREAD_POOL_H_

/// @file multithread_pool.h
/// @brief A pool of objects which can be shared by tasks in multiple threads.

#include <vector>
#include <unordered_set>
#include "readers_writer_lock.h"

namespace ctle
{

/// @brief A multithread pool of objects, which can be shared by tasks in multiple threads.
/// @details The multithread_pool template is used to create a pool of objects which
/// can be shared by multiple threads. The objects are assumed to be expensive to allocate or
/// have allocated, so it makes more sense to share them among multiple threads/tasks, instead
/// of having one pool per thread. On init, a vector of preallocated objects are inserted to the pool
/// and when the pool is deinitialized, the list of objects is returned, so that the caller 
/// can deallocate the objects in a correct fashion. If the objects automatically clean up, 
template<class _Ty>
class multithread_pool
{
private:
	// the pool of objects
	std::vector<std::unique_ptr<_Ty>> pool;

	// the access mutex
	readers_writer_lock accessLock;

	// the available objects
	std::vector<_Ty *> available;

	// the used objects
	std::unordered_set<_Ty *> borrowed;

public:
	using value_type = _Ty;

	/// @brief Initialize the pool, and hand over a list of preallocated objects.
	/// @note Not thread safe, this method is assumed to only be called on setup, so is not guarded from multiple threads.
	void initialize( std::vector<std::unique_ptr<_Ty>> &objectList )
	{
		this->pool = std::move( objectList );
		this->available.resize( this->pool.size() );
		for( size_t inx = 0; inx < this->pool.size(); ++inx )
		{
			this->available[inx] = this->pool[inx].get();
		}
	}

	/// @brief Clears the pool, and returns all objects back to the caller.
	/// @note All items are moved back to the caller, even borrowed items which have not yet been returned. 
	/// @return false if the pool has outstanding borrowed items, true if all items are returned since before.
	bool deinitialize(std::vector<std::unique_ptr<_Ty>>& objectList)
	{
		readers_writer_lock::write_guard guard( this->accessLock );

		// move all pool objects to the return object list
		objectList = std::move(this->pool);

		// check if there are any borrowed items left
		return this->borrowed.empty();
	}

	/// @brief returns true if there is an item available in the pool
	bool item_available()
	{
		readers_writer_lock::read_guard guard( this->accessLock );
		return !available.empty();
	}

	/// @brief returns the number of items available in the pool
	size_t available_count()
	{
		readers_writer_lock::read_guard guard( this->accessLock );
		return available.size();
	}

	/// @brief returns true if any item is borrowed from the pool
	bool item_borrowed()
	{
		readers_writer_lock::read_guard guard( this->accessLock );
		return !borrowed.empty();
	}

	/// @brief borrow an item from the pool
	/// @return a pointer to the item, or nullptr if no item is available
	_Ty* borrow_item()
	{
		readers_writer_lock::write_guard guard( this->accessLock );

		// item available?
		if( available.empty() )
			return nullptr;

		// move the last available item into the locked set
		_Ty *ret = this->available.back();
		this->available.pop_back();
		this->borrowed.emplace( ret );

		return ret;
	}

	/// @brief return an item to the pool
	/// @param item the item to return, must be a valid item from the pool
	/// @return true if the item was returned, false if the item was not borrowed from the pool, and cannot be returned
	bool return_item(_Ty* item)
	{
		readers_writer_lock::write_guard guard( this->accessLock );

		// check that we have the item in the pool
		auto it = this->borrowed.find( item );
		if( it == this->borrowed.end() )
			return false;

		// found, return to available list
		this->available.emplace_back( *it );
		this->borrowed.erase( it );

		return true;
	}

};

}
//namespace ctle

#endif//_CTLE_MULTITHREAD_POOL_H_
