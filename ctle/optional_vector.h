// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_OPTIONAL_VECTOR_H_
#define _CTLE_OPTIONAL_VECTOR_H_

/// @file optional_idx_vector.h
/// @brief Contains the optional_vector class template, a version of std::vector which adds a flag to indicate if it the vector is allocated or not. 

#include <vector>
#include "optional_value.h"

namespace ctle
{

// optional_vector: optional std::vector 
template <class _Ty, class _Alloc = std::allocator<_Ty>>
class optional_vector 
{
public:
	using value_type = _Ty;
	using allocator_type = _Alloc;
	using pointer = typename std::vector<_Ty,_Alloc>::pointer;
	using const_pointer = typename std::vector<_Ty,_Alloc>::const_pointer;
	using reference = typename std::vector<_Ty,_Alloc>::reference;
	using const_reference = typename std::vector<_Ty,_Alloc>::const_reference;
	using size_type = typename std::vector<_Ty,_Alloc>::size_type;

private:
	std::vector<_Ty,_Alloc> vector_m;
	bool has_value_m = false;

public:
	optional_vector() = default;
	optional_vector( const optional_vector &_other ) : vector_m( _other.vector_m ), has_value_m(_other.has_value_m) {}
	optional_vector &operator = ( const optional_vector &_other ) { this->has_value_m = _other.has_value_m; this->vector_m = _other.vector_m; return *this; }
	optional_vector( optional_vector &&_other ) : vector_m( std::move(_other.vector_m) ), has_value_m(_other.has_value_m) { _other.has_value_m = false; }
	optional_vector &operator = ( optional_vector &&_other ) { this->has_value_m = _other.has_value_m; this->vector_m = std::move( _other.vector_m ); _other.has_value_m = false; return *this; }

	bool operator==( const optional_vector &_other ) const;
	bool operator!=( const optional_vector &_other ) const;

	/// @brief Reset the optional_vector. has_value() will return false.
	void reset() { this->has_value_m = false; this->vector_m.clear(); }

	/// @brief Set the value of the optional_vector to an empty vector. has_value() will return true.
	void set() { this->has_value_m = true; this->vector_m.clear(); }

	/// @brief Set the value of the optional_vector. has_value() will return true.
	void set(const std::vector<_Ty, _Alloc>& _values) { this->has_value_m = true; this->vector_m = _values; }

	/// @brief Check if the optional_vector has a value.
	bool has_value() const { return this->has_value_m; }

	/// @brief Get the vector of the optional_vector.
	/// @throw ctle::bad_optional_value_access if the optional_vector has no value.
	std::vector<_Ty, _Alloc>& vector() { if (!this->has_value_m) { throw ctle::bad_optional_value_access("optional_vector has no value"); } return this->vector_m; }

	/// @brief Get the vector of the optional_vector.
	/// @throw ctle::bad_optional_value_access if the optional_vector has no value.
	const std::vector<_Ty, _Alloc>& vector() const { if (!this->has_value_m) { throw ctle::bad_optional_value_access("optional_vector has no value"); } return this->vector_m; }

	/// @brief Get a reference to the vector.
	/// @throw ctle::bad_optional_value_access if the optional_vector has no value.
	std::vector<_Ty, _Alloc>& values() { return vector(); }

	/// @brief Get a const reference to the vector.
	/// @throw ctle::bad_optional_value_access if the optional_vector has no value.
	const std::vector<_Ty, _Alloc>& values() const { return vector(); }

	/// @brief Get a reference to the vector.
	/// @throw ctle::bad_optional_value_access if the optional_vector has no value.
	operator std::vector<_Ty, _Alloc> &() { return vector(); }

	/// @brief Get a const reference to the vector.
	/// @throw ctle::bad_optional_value_access if the optional_vector has no value.
	operator const std::vector<_Ty, _Alloc>& () const { return vector(); }
};

template <class _Ty, class _Alloc>
bool optional_vector<_Ty, _Alloc>::operator==( const optional_vector &_other ) const
{
	if( this->has_value_m )
	{
		if( _other.has_value_m )
			return this->vector_m == _other.vector_m;
		return false;
	}
	else
	{
		if( _other.has_value_m )
			return false;
		return true;
	}
}

template <class _Ty, class _Alloc>
bool optional_vector<_Ty, _Alloc>::operator!=( const optional_vector &_other ) const
{
	if( this->has_value_m )
	{
		if( _other.has_value_m )
			return this->vector_m != _other.vector_m;
		return true;
	}
	else
	{
		if( _other.has_value_m )
			return true;
		return false;
	}
}

}
//namespace ctle

#endif//_CTLE_OPTIONAL_VECTOR_H_
