// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_ENDIANNESS_H_
#define _CTLE_ENDIANNESS_H_

/// @file endianness.h
/// @brief Functions for converting endianness of values.

#include <stdint.h>
#include <stddef.h>

#include <utility>

namespace ctle
{

/// @brief Creates values from big-endian raw 2, 4, or 8 byte data.
/// 
/// Template specialization is implemented for uint16_t, uint32_t, and uint64_t.
/// 
/// @tparam T The type of the value to create.
/// @param src Pointer to the source big-endian data.
/// @return The value created from the big-endian data.
template <class T> T value_from_bigendian( const uint8_t *src );

/// @brief Creates values from big-endian raw data. Specialization for uint16_t.
/// 
/// @param src Pointer to the source big-endian data.
/// @return The uint16_t value created from the big-endian data.
template <> inline uint16_t value_from_bigendian<uint16_t>( const uint8_t *src )
{
    return (uint16_t)( uint16_t( uint16_t( src[0] ) << 8 ) | uint16_t( src[1] ) );
}

/// @brief Creates values from big-endian raw data. Specialization for uint32_t.
/// 
/// @param src Pointer to the source big-endian data.
/// @return The uint32_t value created from the big-endian data.
template <> inline uint32_t value_from_bigendian<uint32_t>( const uint8_t *src )
{
    return ( uint32_t( value_from_bigendian<uint16_t>( &src[0] ) ) << 16 ) | uint32_t( value_from_bigendian<uint16_t>( &src[2] ) );
}

/// @brief Creates values from big-endian raw data. Specialization for uint64_t.
/// 
/// @param src Pointer to the source big-endian data.
/// @return The uint64_t value created from the big-endian data.
template <> inline uint64_t value_from_bigendian<uint64_t>( const uint8_t *src )
{
    return ( uint64_t( value_from_bigendian<uint32_t>( &src[0] ) ) << 32 ) | uint64_t( value_from_bigendian<uint32_t>( &src[4] ) );
}

/// @brief Creates big-endian raw 2, 4, or 8 byte data from values. Template specialization is implemented for uint16_t, uint32_t, and uint64_t.
/// 
/// @tparam T The type of the value to convert.
/// @param dst Pointer to the destination buffer for the big-endian data.
/// @param value The value to convert to big-endian data.
template <class T> void bigendian_from_value( uint8_t *dst, T value );

/// @brief Creates big-endian raw data from values. Specialization for uint16_t.
/// 
/// @param dst Pointer to the destination buffer for the big-endian data.
/// @param value The uint16_t value to convert to big-endian data.
template <> inline void bigendian_from_value<uint16_t>( uint8_t *dst, uint16_t value )
{
    dst[0] = uint8_t( ( value >> 8 ) & 0xff );
    dst[1] = uint8_t( value & 0xff );
}

/// @brief Creates big-endian raw data from values. Specialization for uint32_t.
/// 
/// @param dst Pointer to the destination buffer for the big-endian data.
/// @param value The uint32_t value to convert to big-endian data.
template <> inline void bigendian_from_value<uint32_t>( uint8_t *dst, uint32_t value )
{
    bigendian_from_value<uint16_t>( &dst[0], uint16_t( ( value >> 16 ) & 0xffff ) );
    bigendian_from_value<uint16_t>( &dst[2], uint16_t( value & 0xffff ) );
}

/// @brief Creates big-endian raw data from values. Specialization for uint64_t.
/// 
/// @param dst Pointer to the destination buffer for the big-endian data.
/// @param value The uint64_t value to convert to big-endian data.
template <> inline void bigendian_from_value<uint64_t>( uint8_t *dst, uint64_t value )
{
    bigendian_from_value<uint32_t>( &dst[0], uint32_t( ( value >> 32 ) & 0xffffffff ) );
    bigendian_from_value<uint32_t>( &dst[4], uint32_t( value & 0xffffffff ) );
}

/// @brief Swap two bytes.
/// 
/// @param pA Pointer to the first byte.
/// @param pB Pointer to the second byte.
inline void swap_bytes( uint8_t *pA, uint8_t *pB )
{
    uint8_t tmp = *pA;
    *pA = *pB;
    *pB = tmp;
}

/// @brief Swap byte order of a single value. Template specialization is implemented for uint16_t, uint32_t, and uint64_t.
/// 
/// @tparam T The type of the value to swap byte order.
/// @param dest Pointer to the value to swap byte order.
template <class T> void swap_byte_order( T *dest );


/// @brief Swap byte order of a single value. Specialization for uint16_t.
/// 
/// @param dest Pointer to the uint16_t value to swap byte order.
template<> inline void swap_byte_order<uint16_t>( uint16_t *dest )
{
    swap_bytes( &( (uint8_t *)dest )[0], &( (uint8_t *)dest )[1] );
}

/// @brief Swap byte order of a single value. Specialization for uint32_t.
/// 
/// @param dest Pointer to the uint32_t value to swap byte order.
template<> inline void swap_byte_order<uint32_t>( uint32_t *dest )
{
    swap_bytes( &( (uint8_t *)dest )[0], &( (uint8_t *)dest )[3] );
    swap_bytes( &( (uint8_t *)dest )[1], &( (uint8_t *)dest )[2] );
}

/// @brief Swap byte order of a single value. Specialization for uint64_t.
/// 
/// @param dest Pointer to the uint64_t value to swap byte order.
template<> inline void swap_byte_order<uint64_t>( uint64_t *dest )
{
    swap_bytes( &( (uint8_t *)dest )[0], &( (uint8_t *)dest )[7] );
    swap_bytes( &( (uint8_t *)dest )[1], &( (uint8_t *)dest )[6] );
    swap_bytes( &( (uint8_t *)dest )[2], &( (uint8_t *)dest )[5] );
    swap_bytes( &( (uint8_t *)dest )[3], &( (uint8_t *)dest )[4] );
}

/// @brief Swap byte order of multiple values. Template specialization is implemented for uint16_t, uint32_t, and uint64_t.
/// 
/// @tparam T The type of the values to swap byte order.
/// @param dest Pointer to the array of values to swap byte order.
/// @param count The number of values in the array.
template <class T> void swap_byte_order( T *dest, size_t count );


/// @brief Swap byte order of multiple values. Specialization for uint16_t.
/// 
/// @param dest Pointer to the array of uint16_t values to swap byte order.
/// @param count The number of values in the array.
template<> inline void swap_byte_order<uint16_t>( uint16_t *dest, size_t count )
{
    for( size_t i = 0; i < count; ++i )
    {
        swap_bytes( &( (uint8_t *)dest )[0], &( (uint8_t *)dest )[1] );
        ++dest;
    }
}

/// @brief Swap byte order of multiple values. Specialization for uint32_t.
/// 
/// @param dest Pointer to the array of uint32_t values to swap byte order.
/// @param count The number of values in the array.
template<> inline void swap_byte_order<uint32_t>( uint32_t *dest, size_t count )
{
    for( size_t i = 0; i < count; ++i )
    {
        swap_bytes( &( (uint8_t *)dest )[0], &( (uint8_t *)dest )[3] );
        swap_bytes( &( (uint8_t *)dest )[1], &( (uint8_t *)dest )[2] );
        ++dest;
    }
}

/// @brief Swap byte order of multiple values. Specialization for uint64_t.
/// 
/// @param dest Pointer to the array of uint64_t values to swap byte order.
/// @param count The number of values in the array.
template<> inline void swap_byte_order<uint64_t>( uint64_t *dest, size_t count )
{
    for( size_t i = 0; i < count; ++i )
    {
        swap_bytes( &( (uint8_t *)dest )[0], &( (uint8_t *)dest )[7] );
        swap_bytes( &( (uint8_t *)dest )[1], &( (uint8_t *)dest )[6] );
        swap_bytes( &( (uint8_t *)dest )[2], &( (uint8_t *)dest )[5] );
        swap_bytes( &( (uint8_t *)dest )[3], &( (uint8_t *)dest )[4] );
        ++dest;
    }
}

}
//namespace ctle

#endif//_CTLE_ENDIANNESS_H_
