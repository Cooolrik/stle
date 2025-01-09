// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_IDX_VECTOR_H_
#define _CTLE_IDX_VECTOR_H_

/// @file idx_vector.h
/// @brief Contains the idx_vector class template, a vector of values with an index vector into the values.

#include <vector>

#include "fwd.h"

namespace ctle
{

/// @brief idx_vector: std::vector of values, with an std::vector as index into the values
/// @details The idx_vector class template is a vector of values with an index vector into 
/// the values.The index vector is used to index into the values vector, and all indices must 
/// be within the bounds of the values vector.
/// @tparam _Ty The value type.
/// @tparam _IdxTy The index type, defaults to std::vector<i32>.
/// @tparam _VecTy The values vector type, defaults to std::vector<_Ty>.
template <
	class _Ty,
	class _IdxTy /* = std::vector<i32>*/,
	class _VecTy /* = std::vector<_Ty>*/
> class idx_vector
{
public:
	using value_type = _Ty;				///< The value type. The value vector must contain values of this type.
	using values_vector_type = _VecTy; 	///< The values vector type.
	using index_vector_type = _IdxTy; 	///< The index vector type.

	using allocator_type = typename values_vector_type::allocator_type; 	///< The allocator type. Adopted from the values vector type.
	using pointer = typename values_vector_type::pointer; 					///< The pointer type. Adopted from the values vector type.
	using const_pointer = typename values_vector_type::const_pointer; 		///< The const pointer type. Adopted from the values vector type.
	using reference = typename values_vector_type::reference; 				///< The reference type. Adopted from the values vector type.
	using const_reference = typename values_vector_type::const_reference; 	///< The const reference type. Adopted from the values vector type.
	using size_type = typename values_vector_type::size_type; 				///< The size type. Adopted from the values vector type.

private:
	values_vector_type values_m;
	index_vector_type index_m;

public:
	idx_vector() = default;
	idx_vector( const idx_vector &_other ) noexcept : values_m( _other.values_m ), index_m( _other.index_m ) {}
	idx_vector &operator = ( const idx_vector &_other )	{ this->values_m = _other.values_m; this->index_m = _other.index_m; return *this; }
	idx_vector( idx_vector &&_other ) noexcept : values_m( std::move( _other.values_m ) ), index_m( std::move( _other.index_m ) ) {}
	idx_vector &operator = ( idx_vector &&_other ) { this->values_m = std::move( _other.values_m ); this->index_m = std::move( _other.index_m ); return *this; }

	// comparison operators
	bool operator==(const idx_vector& _other) const;
	bool operator!=( const idx_vector &_other ) const;

	/// @brief Get a reference to the value at the given index
	/// @details The index must be within the bounds of the index vector.
	/// @param _idx The index of the value to get
	/// @return A reference to the value which the current index points to. 
	/// @note A reference to the values table is returned, which may be shared by other items in the index list. This function does not check if the index is within the bounds of the index vector.
	/// @throws std::out_of_range if the index is out of bounds
	reference operator[](size_type _idx)
	{
		return this->values_m[this->index_m[_idx]];
	}
	/// @copydoc operator[](size_type)
	const_reference operator[](size_type _idx) const
	{
		return this->values_m[this->index_m[_idx]];
	}
	
	/// @brief Get the size of the index vector
	size_type size() const
	{
		return this->index_m.size();
	}

	/// @brief Clear the vector
	void clear()
	{
		this->values_m.clear(); this->index_m.clear();
	}

	/// @brief Get a reference to the index vector
	index_vector_type& index()
	{
		return this->index_m;
	}
	/// @brief Get a const reference to the index vector
	const index_vector_type &index() const
	{
		return this->index_m;
	}

	/// @brief Get a reference to the values vector
	values_vector_type &values()
	{
		return this->values_m;
	}
	/// @brief Get a const reference to the values vector
	const values_vector_type &values() const
	{
		return this->values_m;
	}

	/// @brief Validate the index vector, checking that all indices are within the bounds of the values vector
	/// @return true if the index vector is valid, false otherwise
	bool is_valid() const
	{
		const size_type values_size = this->values_m.size();
		for (const size_type idx : this->index_m)
		{
			if( idx >= values_size )
			{
				return false;
			}
		}
		return true;
	} 

};

template <class _Ty, class _IdxTy, class _VecTy>
bool idx_vector<_Ty, _IdxTy, _VecTy>::operator==( const idx_vector &_other ) const
{
	return ( this->values_m == _other.values_m )
		&& ( this->index_m == _other.index_m );
}

template <class _Ty, class _IdxTy, class _VecTy>
bool idx_vector<_Ty, _IdxTy, _VecTy>::operator!=( const idx_vector &_other ) const
{
	return ( this->values_m != _other.values_m )
		|| ( this->index_m != _other.index_m );
}

}
//namespace ctle

#endif//_CTLE_IDX_VECTOR_H_
