// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_STRING_FUNCS_H_
#define _CTLE_STRING_FUNCS_H_

#include "endianness.h"

#include <string>
#include <vector>
#include <stdexcept>

namespace ctle
{
// a span of characters, with start and end
template<class _Ty> struct string_span
{
	string_span() = default;
	string_span( const _Ty *_start, const _Ty *_end ) : start( _start ), end( _end ) {}

	const _Ty *start = {}; // first char of span
	const _Ty *end = {}; // the character after the last char of the span

	// get the length of the span, returns 0 if the span is invalid
	size_t length() const noexcept
	{
		return ( end > start ) ? end - start : 0;
	}

	// make a copy to a string, returns empty string if the span is invalid (end<=start)
	operator std::basic_string<_Ty>() noexcept
	{
		return ( end > start ) ? ( std::basic_string<_Ty>( this->start, this->end ) ) : ( std::basic_string<_Ty>() );
	}
};

// Parse types from strings. 
// The versions which includes a bool &success parameter do not throw any exception if the conversion fails, but resets the success parameter to false. 
// Note! The success parameter must be set to true before calling the functions!
template<class _Ty> _Ty from_string( const string_span<char> &str, bool &success ) noexcept;
template<class _Ty> _Ty from_string( const string_span<char> &str );
template<class _Ty> inline _Ty from_string( const char *start, const char *end, bool &success ) noexcept { return from_string<_Ty>( string_span<char>(start,end), success ); }
template<class _Ty> inline _Ty from_string( const char *start, const char *end ) { return from_string<_Ty>( string_span<char>(start,end) ); }
template<class _Ty> inline _Ty from_string( const std::basic_string<char> &str, bool &success ) noexcept { return from_string<_Ty>( string_span<char>(str.data(), str.data() + str.size()), success ); }
template<class _Ty> inline _Ty from_string( const std::basic_string<char> &str ) { return from_string<_Ty>( string_span<char>(str.data(), str.data() + str.size()) ); }

// Print types to strings.
template<class _Ty> std::string to_string( const _Ty &val );

// Get span of character set in string
// get length of span of characters between start and end, which are in the control list of chars
// Caveat: The chars in control are required to be in the lower set (not extended characters)
template<class _Ty> inline size_t strspn_t( const _Ty *start, const _Ty *end, const _Ty *control ) noexcept
{
	const _Ty *ptr = start;

	// scan until end at maximum
	while( ptr < end )
	{
		// look through delimiters for a match
		bool found = false;
		for( const _Ty *d = control; *d != _Ty( 0 ); ++d )
		{
			if( *d == *ptr )
			{
				found = true;
				break;
			}
		}

		// if not found, we are at the end of the span, early out
		if( !found )
		{
			// return the span to before this pointer
			return ptr - start;
		}

		// step
		++ptr;
	}

	// at the end
	return end - start;
}
template<class _Ty> inline size_t strspn_t( const string_span<_Ty> &span, const _Ty *control ) noexcept
{
	return strspn_t( span.start, span.end, control );
}

// Get span until character in string
// get length of span of characters between start and end, until first occurance of a character from the control list
// Caveat: The chars in control are required to be in the lower set (not extended characters)
template<class _Ty> inline size_t strcspn_t( const _Ty *start, const _Ty *end, const _Ty *control ) noexcept
{
	const _Ty *ptr = start;

	// scan until end at maximum
	while( ptr < end )
	{
		// look through delimiters for a match
		for( const _Ty *d = control; *d != _Ty( 0 ); ++d )
		{
			if( *d == *ptr )
			{
				// we have a match, early out
				// return the span to before this pointer
				return ptr - start;
			}
		}

		// no match, step one
		++ptr;
	}

	// not found, return the full span from start to end
	return end - start;
}
template<class _Ty> inline size_t strcspn_t( const string_span<_Ty> &span, const _Ty *control ) noexcept
{
	return strcspn_t( span.start, span.end, control );
}

// Get position of last occuring character in control list
// get length of span of characters between start and end, until last occurance of a character from the control list
// Caveat: The chars in control are required to be in the lower set (not extended characters)
template<class _Ty> inline size_t strcrspn_t( const _Ty *start, const _Ty *end, const _Ty *control ) noexcept
{
	const _Ty *ptr = end - 1;

	// scan until end at maximum
	while( ptr >= start )
	{
		// look through delimiters for a match
		for( const _Ty *d = control; *d != _Ty( 0 ); ++d )
		{
			if( *d == *ptr )
			{
				// we have a match, early out
				// return the span to before this pointer
				return ptr - start;
			}
		}

		// no match, step one
		--ptr;
	}

	// not found, return the full span from start to end
	return end - start;
}
template<class _Ty> inline size_t strcrspn_t( const string_span<_Ty> &span, const _Ty *control ) noexcept
{
	return strcrspn_t( span.start, span.end, control );
}

// given a start and end pointer to a string, and a list of deliminators, return the first token.
// the implementation does not modify the input string, and does not hold any internal state.
// Caveat: The chars in delims are required to be in the lower set (not extended characters)
template<class _Ty> inline string_span<_Ty> strtok_t( const _Ty *start, const _Ty *end, const _Ty *delims ) noexcept
{
	// skip over initial delimiters
	size_t span = strspn_t<_Ty>( start, end, delims );
	const _Ty *tokStart = start + span;

	// get span until next occurance of delimiter
	span = strcspn_t<_Ty>( tokStart, end, delims );
	const _Ty *tokEnd = tokStart + span;

	return { tokStart, tokEnd };
}
template<class _Ty> inline string_span<_Ty> strtok_t( const string_span<_Ty> &span, const _Ty *delims ) noexcept
{
	return strtok_t( span.start, span.end, delims );
}

// Locate first occurrence of character in string
// get length of span of characters between start and end, until first occurance of a character from the control list
template<class _Ty> inline size_t strchr_t( const _Ty *start, const _Ty *end, _Ty character ) noexcept
{
	const _Ty *ptr = start;

	// scan until end at maximum
	while( ptr < end )
	{
		if( character == *ptr )
		{
			// we have a match, early out
			// return the span to before this pointer
			return ptr - start;
		}

		// no match, step one
		++ptr;
	}

	// not found, return the full span from start to end
	return end - start;
}
template<class _Ty> inline size_t strchr_t( const string_span<_Ty> &span, _Ty character ) noexcept
{
	return strchr_t( span.start, span.end, character );
}

// Locate last occurrence of character in string
// get length of span of characters between start and end, until first occurance of a character from the control list
template<class _Ty> inline size_t strrchr_t( const _Ty *start, const _Ty *end, _Ty character ) noexcept
{
	const _Ty *ptr = end - 1;

	// scan until start at maximum
	while( ptr >= start )
	{
		if( character == *ptr )
		{
			// we have a match, early out
			// return the span to before this pointer
			return ptr - start;
		}

		// no match, step one
		--ptr;
	}

	// not found, return the full span from start to end
	return end - start;
}
template<class _Ty> inline size_t strrchr_t( const string_span<_Ty> &span, _Ty character ) noexcept
{
	return strrchr_t( span.start, span.end, character );
}

// given a start and end pointer to a string, parse an unsigned decimal number.
// if values other than numbers are found, the parsing stops and the value is returned, so trim any preceeding white space
// the implementation does not modify the input string, and does not hold any internal state.
template<class _Ty> inline uint64_t stou64_t( const _Ty *start, const _Ty *end ) noexcept
{
	const _Ty *ptr = start;
	uint64_t value = 0;

	if( ptr >= end )
		return 0;

	while( ptr < end )
	{
		size_t c = *ptr - _Ty( '0' );
		if( c >= 10 )
			break; // not a digit

		value = value * 10 + c;
		++ptr;
	}

	return value;
}
template<class _Ty> inline uint64_t stou64_t( const string_span<_Ty> &span ) noexcept
{
	return stou64_t( span.start, span.end );
}

// given a start and end pointer to a string, parse a signed decimal number.
// if values other than numbers are found, the parsing stops and the value is returned, so trim any preceeding white space
// the implementation does not modify the input string, and does not hold any internal state.
template<class _Ty> inline int64_t stoi64_t( const _Ty *start, const _Ty *end ) noexcept
{
	const _Ty *ptr = start;
	bool sign = false;

	if( ptr >= end )
		return 0;

	// check for preceeding sign
	if( *ptr == _Ty( '-' ) )
	{
		sign = true;
		++ptr;
	}

	int64_t value = int64_t( stou64_t( ptr, end ) );
	return sign ? -value : value;
}
template<class _Ty> inline int64_t stoi64_t( const string_span<_Ty> &span ) noexcept
{
	return stoi64_t( span.start, span.end );
}

// given a start and end pointer to a string, lex into a vector of tokens. the lexer handles
// whitespace and quoted strings, as well as separator characters. 
// (pre-reserve in the vector the expected nunber of tokens)
// Caveat: The lexer will not handle multi-char separators, but will instead return each
// char in the separators set as an individual token
template<class _Ty> inline bool lex_t( std::vector<string_span<_Ty>> *dest, const _Ty *start, const _Ty *end, const _Ty *separators = ",/*()=[]{}", const _Ty *quotes = "'\"", const _Ty *whitespaces = " \t\r\n" )
{
	const _Ty *ptr = start;

	// lambda to skip over any whitespace 
	auto ws_skip = [&]()
		{
			for( ; ptr < end; ++ptr )
			{
				bool found = false;
				for( const _Ty *d = whitespaces; *d != _Ty( 0 ); ++d )
				{
					if( *d == *ptr )
					{
						found = true;
						break;
					}
				}
				if( !found )
					return true;
			}
			return false;
		};

	// check if in list
	auto is_a = [&]( const _Ty *char_list )
		{
			for( const _Ty *d = char_list; *d != _Ty( 0 ); ++d )
			{
				if( *d == *ptr )
					return *ptr;
			}
			return _Ty( 0 );
		};

	// main parse loop
	while( ptr < end )
	{
		// skip whitespace, break if nothing left
		if( !ws_skip() )
			break;

		// if this is a separator, output as a specific token, and continue
		if( is_a( separators ) )
		{
			dest->emplace_back( string_span<_Ty>( { ptr,ptr + 1 } ) );
			++ptr;
			continue;
		}

		// if this is the start of a string in quotes, parse the whole string as a token
		if( _Ty c = is_a( quotes ) )
		{
			string_span<_Ty> str_span;

			// scan until we find another of the same quotation mark, (skip the marks in the token span)
			++ptr;
			str_span.start = ptr;
			for( ;;)
			{
				if( ptr >= end )
					return false; // error: reached end of string without an end quote
				if( *ptr == c )
				{
					str_span.end = ptr;
					break;
				}
				++ptr;
			}

			dest->emplace_back( str_span );
			++ptr;
			continue;
		}

		// not a string or separator, parse as token until we reach any other character
		string_span<_Ty> token_span;
		token_span.start = ptr;
		++ptr;
		for( ;;)
		{
			if( ptr >= end
				|| is_a( separators )
				|| is_a( whitespaces )
				|| is_a( quotes ) )
			{
				token_span.end = ptr;
				dest->emplace_back( token_span );
				break;
			}
			++ptr;
		}
	}

	return true;
}
template<class _Ty> inline bool lex_t( std::vector<string_span<_Ty>> *dest, const string_span<_Ty> &span, const _Ty *separators = ",/*()=[]{}", const _Ty *quotes = "'\"", const _Ty *whitespaces = " \t\r\n" )
{
	return lex_t( dest, span.start, span.end, separators, quotes, whitespaces );
}

// writes array of bytes to string of hex values. the hex values will be
// in the same order as the bytes, so if you need to convert a litte-endian
// word into hex, be sure to flip the byte order before.
std::string inline bytes_to_hex_string( const void *bytes, size_t count )
{
	static const char hexchars[16] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };

	std::string ret;
	const uint8_t *p = (const uint8_t *)bytes;
	for( size_t i = 0; i < count; ++i )
	{
		ret += hexchars[( ( *p ) >> 4 ) & 0xf]; // high nibble
		ret += hexchars[( *p ) & 0xf]; // low nibble
		++p;
	}
	return ret;
}

template <class T> std::string value_to_hex_string( const T &value );

template <> inline std::string value_to_hex_string( const uint8_t &value )
{
	return bytes_to_hex_string( &value, sizeof( value ) );
}

template <> inline std::string value_to_hex_string( const uint16_t &value )
{
	bigendian_from_value( (uint8_t *)&value, value ); // in-place make sure big endian
	return bytes_to_hex_string( &value, sizeof( value ) );
}

template <> inline std::string value_to_hex_string( const uint32_t &value )
{
	bigendian_from_value( (uint8_t *)&value, value ); // in-place make sure big endian
	return bytes_to_hex_string( &value, sizeof( value ) );
}

template <> inline std::string value_to_hex_string( const uint64_t &value )
{
	bigendian_from_value( (uint8_t *)&value, value ); // in-place make sure big endian
	return bytes_to_hex_string( &value, sizeof( value ) );
}

// retrieves bytes from a hex string of known length.
// versions which does not throw instead clears the success flag if the conversion fails. 
// note: the count is equal to the number of bytes, and the hex string is assumed to be twice the count (since two hex values is combined into one byte)
// Note: The flag must be set before calling the function
template <class T> T hex_string_to_value( const char *hex_string, bool &success ) noexcept;
template <class T> T hex_string_to_value( const char *hex_string )
{
	bool success = true;
	auto value = hex_string_to_value<T>( hex_string, success );
	if( !success )
		throw std::runtime_error("Could not convert hex string to bytes, the string contains invalid (non-hex) characters.");
	return value;
}

// Note: The flag must be set before calling the function
static inline uint8_t decode_hex_char( char c, bool &success ) noexcept
{
	if( c >= '0' && c <= '9' )
		return uint8_t( c - '0' );
	else if( c >= 'a' && c <= 'f' )
		return uint8_t( ( c - 'a' ) + 10 );
	else if( c >= 'A' && c <= 'F' )
		return uint8_t( ( c - 'A' ) + 10 );

	success = false;
	return 0;
}

// retrieves bytes from a hex string of known length.
// note: the count is equal to the number of bytes, and the hex string is assumed to be twice the count (since two hex values is combined into one byte)
inline void hex_string_to_bytes( void *bytes, const char *hex_string, size_t count, bool &success ) noexcept
{
	uint8_t *p = (uint8_t *)bytes;
	for( size_t i = 0; i < count; ++i )
	{
		uint8_t hn = uint8_t( decode_hex_char( hex_string[i * 2 + 0], success ) << 4 );
		uint8_t ln = decode_hex_char( hex_string[i * 2 + 1], success );
		p[i] = uint8_t( hn | ln );
	}
}

inline void hex_string_to_bytes( void *bytes, const char *hex_string, size_t count )
{
	bool success = true;
	hex_string_to_bytes( bytes, hex_string, count, success );
	if( !success )
		throw std::runtime_error("Could not convert hex string to bytes, the string contains invalid characters.");
}

template <> inline uint8_t hex_string_to_value<uint8_t>( const char *hex_string, bool &success ) noexcept
{
	uint8_t ret;
	hex_string_to_bytes( &ret, hex_string, sizeof( uint8_t ), success );
	return ret;
}

template <> inline uint16_t hex_string_to_value<uint16_t>( const char *hex_string, bool &success ) noexcept
{
	uint8_t bytes[sizeof( uint16_t )];
	hex_string_to_bytes( bytes, hex_string, sizeof( uint16_t ), success );
	return value_from_bigendian<uint16_t>( bytes );
}

template <> inline uint32_t hex_string_to_value<uint32_t>( const char *hex_string, bool &success ) noexcept
{
	uint8_t bytes[sizeof( uint32_t )];
	hex_string_to_bytes( bytes, hex_string, sizeof( uint32_t ), success );
	return value_from_bigendian<uint32_t>( bytes );
}

template <> inline uint64_t hex_string_to_value<uint64_t>( const char *hex_string, bool &success ) noexcept
{
	uint8_t bytes[sizeof( uint64_t )];
	hex_string_to_bytes( bytes, hex_string, sizeof( uint64_t ), success );
	return value_from_bigendian<uint64_t>( bytes );
}

}
// namespace ctle

#endif//_CTLE_STRING_FUNCS_H_
