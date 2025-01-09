// ctle Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_BIMAP_H_
#define _CTLE_BIMAP_H_

/// @file bimap.h
/// @brief class bimap, a bi-directional unordered_map, providing O(1) average lookup for both key->value and value->key.

#include <unordered_map>

namespace ctle
{

/// @brief Bi-directional unordered_map, providing O(1) average lookup for both key->value and value->key.
/// 
/// This class stores both key and value directly and requires std::hash functors to be defined for both.
/// Currently implemented using two maps, which duplicates the memory used.This should be changed to a shared memory layout.
/// 
/// @tparam _Kty Type of the key.
/// @tparam _Vty Type of the value.
template<class _Kty, class _Vty> class bimap
{
private:
    using map_type = std::unordered_map<_Kty, _Vty>;
    using rmap_type = std::unordered_map<_Vty, _Kty>;
    using key_type = _Kty;
    using value_type = _Vty;
    using iterator = typename map_type::iterator;
    using riterator = typename rmap_type::iterator;

    map_type Fwd; ///< Forward map from key to value.
    rmap_type Rev; ///< Reverse map from value to key.

public:
    using const_iterator = typename map_type::const_iterator;

	// ctors/dtor and assignment operators.
	bimap() = default;
    bimap( const bimap &other ) = default;
    bimap &operator=( const bimap &other ) = default;
    bimap( bimap &&other ) = default;
    bimap &operator=( bimap &&other ) = default;
    ~bimap() = default;

    /// @brief Compares this bimap with another for equality. Only the forward map is compared, as the reverse map is just a reverse lookup.
	bool operator==(const bimap& other) const noexcept
	{
        return ( this->Fwd == other.Fwd );
    }

	/// @brief Compares this bimap with another for inequality.
	bool operator!=(const bimap& other) const noexcept
    {
        return !operator==( other );
    }

    /// @brief Returns a constant iterator to the beginning of the forward mapping key->value.
    const_iterator begin() const noexcept
    {
        return this->Fwd.begin();
    }

    /// @brief Returns a constant iterator to beyond the end of the forward mapping key->value.
    const_iterator end() const noexcept
    {
        return this->Fwd.end();
    }

	/// @brief Checks if a key exists in the map.
	/// @returns True if the key exists in the map.
	bool contains_key(const _Kty& key) const noexcept
    {
        return this->Fwd.find( key ) != this->Fwd.end();
    }

	/// @brief Checks if a value exists in the map.
	/// @returns True if the value exists in the map.
	bool contains_value(const _Vty& value) const noexcept
    {
        return this->Rev.find( value ) != this->Rev.end();
    }

	/// @brief Returns the value mapped to a key.
	/// @returns The value that the key maps to.
	/// @throws std::out_of_range if the key does not exist.
    const _Vty &at_key( const _Kty &key ) const
    {
        return this->Fwd.at( key );
    }

	/// @brief Returns the key that maps to a value.
	/// @returns The key that maps to the value.
	/// @throws std::out_of_range if the value does not exist.
    const _Kty &at_value( const _Vty &value ) const
    {
        return this->Rev.at( value );
    }

    /// @brief Gets the value mapped to a key, if the key exists.
    /// @return A pair containing the value and a boolean indicating if the key exists.
    std::pair<_Vty, bool> get_value( const _Kty &key ) const noexcept
    {
        auto it = this->Fwd.find( key );
        if( it != this->Fwd.end() )
        {
            return std::make_pair( it->second, true );
        }
        return std::make_pair( _Vty(), false );
    }

    /// @brief Gets the key that maps to a value, if the value exists.
    /// @return A pair containing the key and a boolean indicating if the value exists.
    std::pair<_Kty, bool> get_key( const _Vty &value ) const noexcept
    {
        auto it = this->Rev.find( value );
        if( it != this->Rev.end() )
        {
            return std::make_pair( it->second, true );
        }
        return std::make_pair( _Kty(), false );
    }

    /// @brief Clears the map.
    void clear() noexcept
    {
        this->Fwd.clear();
        this->Rev.clear();
    }

    /// @brief Inserts or replaces a key/value pair. If the key or value already exist in the bimap, they are removed along with the values they are mapped to.
    void insert( const _Kty &key, const _Vty &value ) noexcept
    {
        // Make sure neither value is already in the map.
        erase_by_key( key );
        erase_by_value( value );

        // Insert into maps.
        this->Fwd.emplace( key, value );
        this->Rev.emplace( value, key );
    }

    /// @brief Removes a key->value pair from the map. Looks up the pair by key.
	/// @return Number of pairs removed (0 or 1).
    size_t erase_by_key( const _Kty &key ) noexcept
    {
        iterator it = this->Fwd.find( key );
        if( it != this->Fwd.end() )
        {
            riterator rit = this->Rev.find( it->second );
            this->Fwd.erase( it );
            if( rit != this->Rev.end() )
            {
                this->Rev.erase( rit );
            }
            return 1;
        }
        return 0;
    }

	/// @brief Removes a key->value pair from the map. Looks up the pair by value.
	/// @return Number of pairs removed (0 or 1).
	size_t erase_by_value(const _Vty& value) noexcept
    {
        riterator rit = this->Rev.find( value );
        if( rit != this->Rev.end() )
        {
            iterator it = this->Fwd.find( rit->second );
            this->Rev.erase( rit );
            if( it != this->Fwd.end() )
            {
                this->Fwd.erase( it );
            }
            return 1;
        }
        return 0;
    }

    /// @brief Returns the number of mappings (key->value pairs).
    /// @return The number of mappings.
    size_t size() const noexcept
    {
        return this->Fwd.size();
    }
};

}
//namespace ctle

#endif//_CTLE_BIMAP_H_
