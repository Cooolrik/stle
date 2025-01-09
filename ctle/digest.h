// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_DIGEST_H_
#define _CTLE_DIGEST_H_

/// @file digest.h
/// @brief Contains the digest structure, a templated hash/checksum digest structure with comparison operators and stream output.

#include <cstdint>
#include <functional>
#include <iosfwd>

#include "status.h"

namespace ctle
{

/// @brief A hash/checksum digest structure.
/// 
/// The digest structure encapsulates a digest of a hash or checksum calculation, and adds comparison operators.
/// Ctle also provides stream output for printing digests, as well as conversion to(ctle::to_string) and from(ctle::from_string) strings.
/// @tparam _Size the size of the digest in bits, must be 64, 128, 256 or 512.
/// @note The digest values are stored and compared big-endian (i.e. printed digests will be sorted alphabetically), with most significant 
/// byte at index 0, least significant byte at the last index (7, 15, 31 or 63, depending on digest size).
template<size_t _Size>
struct digest
{
	static_assert(_Size == 64 || _Size == 128 || _Size == 256 || _Size == 512, "Digest size must be one of 64, 128, 256 or 512");
	static constexpr const size_t digest_size = _Size;

	/// @brief The data storage for the digest value. 
	union
	{
		/// @brief The data as bytes, stored as most significant byte at index 0, least significant byte at the last index (7, 15, 31 or 63).
		uint8_t data[_Size / 8];
		uint64_t _data_q[_Size / 64] = {}; // a quadword memory overload, for faster comparisons
	};

	// compare operators
	bool operator<(const digest& other) const noexcept;
	bool operator==(const digest& other) const noexcept;
	bool operator!=(const digest& other) const noexcept;
};

template<size_t _Size>
inline bool digest<_Size>::operator<(const digest& right) const noexcept
{
	const uint8_t* u1 = this->data;
	const uint8_t* u2 = right.data;

	// digest values are stored big-endian, so MSB is first byte (index 0), LSB is last byte (index 7, 15, 31 or 63)
	size_t n = _Size / 8;
	do
	{
		if (*u1 != *u2)	// not equal, early exit, check if more or less than
		{
			if (*u1 < *u2)
				return true;	// less than
			else
				return false;	// more than
		}
		++u1;
		++u2;
		--n;
	} while (n > 0);

	return false; // equal, so not less
};

template<size_t _Size>
inline bool digest<_Size>::operator==(const digest& right) const noexcept
{
	const uint64_t* u1 = this->_data_q;
	const uint64_t* u2 = right._data_q;

	// compare the 64bit values
	size_t n = _Size / 64;
	do
	{
		if (*u1 != *u2)	// not equal, return false
		{
			return false;
		}
		++u1;
		++u2;
		--n;
	} while (n > 0);

	return true; // equal
};

template<size_t _Size>
inline bool digest<_Size>::operator!=(const digest& right) const noexcept
{
	return !this->operator==(right);
};

template<size_t _Size>
inline size_t calculate_size_hash(const digest<_Size>& value)
{
	static_assert(sizeof(std::size_t) == sizeof(std::uint64_t), "The hash code only works for 64bit size_t");
	size_t hval = value._data_q[0];
	for (size_t inx = 1; inx < (_Size / 64); ++inx)
	{
		hval ^= value._data_q[inx];
	}
	return hval;
}

}
//namespace ctle

////////////////////////////////////////

namespace std
{

// digest<64>
template <>
struct hash<ctle::digest<64>>
{
	size_t operator()(const ctle::digest<64>& val) const noexcept
	{
		return ctle::calculate_size_hash<64>(val);
	}
};
std::ostream& operator<<(std::ostream& os, const ctle::digest<64>& _digest);

// digest<128>
template <>
struct hash<ctle::digest<128>>
{
	size_t operator()(const ctle::digest<128>& val) const noexcept
	{
		return ctle::calculate_size_hash<128>(val);
	}
};
std::ostream& operator<<(std::ostream& os, const ctle::digest<128>& _digest);

// digest<256>
template <>
struct hash<ctle::digest<256>>
{
	size_t operator()(const ctle::digest<256>& val) const noexcept
	{
		return ctle::calculate_size_hash<256>(val);
	}
};
std::ostream& operator<<(std::ostream& os, const ctle::digest<256>& _digest);

// digest<512>
template <>
struct hash<ctle::digest<512>>
{
	size_t operator()(const ctle::digest<512>& val) const noexcept
	{
		return ctle::calculate_size_hash<512>(val);
	}
};
std::ostream& operator<<(std::ostream& os, const ctle::digest<512>& _digest);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef CTLE_IMPLEMENTATION

#include <random>
#include <algorithm>
#include <memory>
#include <array>

#include "string_funcs.h"

namespace ctle
{

template <> std::string to_string<digest<64>>(const digest<64>& value) { return bytes_to_hex_string(&value, 8); }
template <> std::string to_string<digest<128>>(const digest<128>& value) { return bytes_to_hex_string(&value, 16); }
template <> std::string to_string<digest<256>>(const digest<256>& value) { return bytes_to_hex_string(&value, 32); }
template <> std::string to_string<digest<512>>(const digest<512>& value) { return bytes_to_hex_string(&value, 64); }

// error return version
template <size_t _Size>
inline digest<_Size> digest_from_string(const string_span<char>& str, bool& success) noexcept
{
	constexpr const size_t byte_size = _Size / 8;
	static_assert(sizeof(digest<_Size>) == byte_size, "Error: digest<> invalid size.");

	// must be exactly 2 hex values per byte 
	if ((str.end - str.start) != (byte_size * 2))
	{
		success = false;
		return digest<_Size>();
	}

	digest<_Size> value;
	hex_string_to_bytes(&value, str.start, byte_size, success);
	return value;
}

// exception throwing version
template <size_t _Size>
inline digest<_Size> digest_from_string(const string_span<char>& str)
{
	bool success = true;
	auto value = ctle::from_string<digest<_Size>>(str, success);
	if (!success)
		throw std::runtime_error("Could not convert the string into a digest<> value, it is ill-formatted.");
	return value;
}

template <> digest<64> ctle::from_string<digest<64>>(const string_span<char>& str, bool& success) noexcept { return digest_from_string<64>(str, success); }
template <> digest<128> ctle::from_string<digest<128>>(const string_span<char>& str, bool& success) noexcept { return digest_from_string<128>(str, success); }
template <> digest<256> ctle::from_string<digest<256>>(const string_span<char>& str, bool& success) noexcept { return digest_from_string<256>(str, success); }
template <> digest<512> ctle::from_string<digest<512>>(const string_span<char>& str, bool& success) noexcept { return digest_from_string<512>(str, success); }

template <> digest<64> hex_string_to_value<digest<64>>(const char* hex_string, bool& success) noexcept { return ctle::from_string<digest<64>>(string_span<char>(hex_string, hex_string + 16), success); }
template <> digest<128> hex_string_to_value<digest<128>>(const char* hex_string, bool& success) noexcept { return ctle::from_string<digest<128>>(string_span<char>(hex_string, hex_string + 32), success); }
template <> digest<256> hex_string_to_value<digest<256>>(const char* hex_string, bool& success) noexcept { return ctle::from_string<digest<256>>(string_span<char>(hex_string, hex_string + 64), success); }
template <> digest<512> hex_string_to_value<digest<512>>(const char* hex_string, bool& success) noexcept { return ctle::from_string<digest<512>>(string_span<char>(hex_string, hex_string + 128), success); }

template <> digest<64> ctle::from_string<digest<64>>(const string_span<char>& str) noexcept { return digest_from_string<64>(str); }
template <> digest<128> ctle::from_string<digest<128>>(const string_span<char>& str) noexcept { return digest_from_string<128>(str); }
template <> digest<256> ctle::from_string<digest<256>>(const string_span<char>& str) noexcept { return digest_from_string<256>(str); }
template <> digest<512> ctle::from_string<digest<512>>(const string_span<char>& str) noexcept { return digest_from_string<512>(str); }

}
//namespace ctle

////////////////////////////////////////

namespace std
{

// implement std::operator<< for the digest class types 
std::ostream& operator<<(std::ostream& os, const ctle::digest<64>& _digest) { os << ctle::to_string(_digest); return os; }
std::ostream& operator<<(std::ostream& os, const ctle::digest<128>& _digest) { os << ctle::to_string(_digest); return os; }
std::ostream& operator<<(std::ostream& os, const ctle::digest<256>& _digest) { os << ctle::to_string(_digest); return os; }
std::ostream& operator<<(std::ostream& os, const ctle::digest<512>& _digest) { os << ctle::to_string(_digest); return os; }

}

#endif//CTLE_IMPLEMENTATION

#endif//_CTLE_DIGEST_H_
