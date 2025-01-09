// ctle Copyright (c) 2022 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_BASE_TYPES_H_
#define _CTLE_BASE_TYPES_H_

#ifdef CTLE_IMPLEMENTATION
#include "fwd.h"
#include "string_funcs.h"
namespace ctle
{
// Print base types to strings.
template<> std::string to_string( const i8 &val ) { return std::to_string(val); }
template<> std::string to_string( const u8 &val ) { return std::to_string(val); }
template<> std::string to_string( const i16 &val ) { return std::to_string(val); }
template<> std::string to_string( const u16 &val ) { return std::to_string(val); }
template<> std::string to_string( const i32 &val ) { return std::to_string(val); }
template<> std::string to_string( const u32 &val ) { return std::to_string(val); }
template<> std::string to_string( const i64 &val ) { return std::to_string(val); }
template<> std::string to_string( const u64 &val ) { return std::to_string(val); }
template<> std::string to_string( const f32 &val ) { return std::to_string(val); }
template<> std::string to_string( const f64 &val ) { return std::to_string(val); }

// Parse base types from strings.
template<> i8 from_string( const string_span<char> &str, bool &result ) noexcept
{
	i64 val = std::stoll( std::string( str.start, str.end ) );
	if( val < (i64)std::numeric_limits<i8>::min() || val > (i64)std::numeric_limits<i8>::max() ) { result = false; return {}; }
	return (i8)val;
}
template<> i8 from_string( const string_span<char> &str )
{
	bool result = true;
	auto val = from_string<i8>( str, result);
	if( !result ) { throw std::out_of_range("In ctle::from_string(): the value is out of range for the destination type (i8)."); }
	return val;
}
template<> u8 from_string( const string_span<char> &str, bool &result ) noexcept
{
	u64 val = std::stoull( std::string( str.start, str.end ) );
	if( val > (u64)std::numeric_limits<u8>::max() ) { result = false; return {}; }
	return (u8)val;
}
template<> u8 from_string( const string_span<char> &str )
{
	bool result = true;
	auto val = from_string<u8>( str, result);
	if( !result ) { throw std::out_of_range("In ctle::from_string(): the value is out of range for the destination type (u8)."); }
	return val;
}
template<> i16 from_string( const string_span<char> &str, bool &result ) noexcept
{
	i64 val = std::stoll( std::string( str.start, str.end ) );
	if( val < (i64)std::numeric_limits<i16>::min() || val > (i64)std::numeric_limits<i16>::max() ) { result = false; return {}; }
	return (i16)val;
}
template<> i16 from_string( const string_span<char> &str )
{
	bool result = true;
	auto val = from_string<i16>( str, result);
	if( !result ) { throw std::out_of_range("In ctle::from_string(): the value is out of range for the destination type (i16)."); }
	return val;
}
template<> u16 from_string( const string_span<char> &str, bool &result ) noexcept
{
	u64 val = std::stoull( std::string( str.start, str.end ) );
	if( val > (u64)std::numeric_limits<u16>::max() ) { result = false; return {}; }
	return (u16)val;
}
template<> u16 from_string( const string_span<char> &str )
{
	bool result = true;
	auto val = from_string<u16>( str, result);
	if( !result ) { throw std::out_of_range("In ctle::from_string(): the value is out of range for the destination type (u16)."); }
	return val;
}
template<> i32 from_string( const string_span<char> &str, bool &result ) noexcept
{
	i64 val = std::stoll( std::string( str.start, str.end ) );
	if( val < (i64)std::numeric_limits<i32>::min() || val > (i64)std::numeric_limits<i32>::max() ) { result = false; return {}; }
	return (i32)val;
}
template<> i32 from_string( const string_span<char> &str )
{
	bool result = true;
	auto val = from_string<i32>( str, result);
	if( !result ) { throw std::out_of_range("In ctle::from_string(): the value is out of range for the destination type (i32)."); }
	return val;
}
template<> u32 from_string( const string_span<char> &str, bool &result ) noexcept
{
	u64 val = std::stoull( std::string( str.start, str.end ) );
	if( val > (u64)std::numeric_limits<u32>::max() ) { result = false; return {}; }
	return (u32)val;
}
template<> u32 from_string( const string_span<char> &str )
{
	bool result = true;
	auto val = from_string<u32>( str, result);
	if( !result ) { throw std::out_of_range("In ctle::from_string(): the value is out of range for the destination type (u32)."); }
	return val;
}
template<> i64 from_string( const string_span<char> &str, bool & ) noexcept
{
	i64 val = std::stoll( std::string( str.start, str.end ) );
	return (i64)val;
}
template<> i64 from_string( const string_span<char> &str )
{
	bool result = true;
	auto val = from_string<i64>( str, result);
	return val;
}
template<> u64 from_string( const string_span<char> &str, bool & ) noexcept
{
	u64 val = std::stoull( std::string( str.start, str.end ) );
	return (u64)val;
}
template<> u64 from_string( const string_span<char> &str )
{
	bool result = true;
	auto val = from_string<u64>( str, result);
	return val;
}
template<> f32 from_string( const string_span<char> &str, bool & ) noexcept
{
	return std::stof( std::string( str.start, str.end ) );
}
template<> f32 from_string( const string_span<char> &str )
{
	return std::stof( std::string( str.start, str.end ) );
}
template<> f64 from_string( const string_span<char> &str, bool & ) noexcept
{
	return std::stod( std::string( str.start, str.end ) );
}
template<> f64 from_string( const string_span<char> &str )
{
	return std::stod( std::string( str.start, str.end ) );
}

}
//namespace ctle
#endif//CTLE_IMPLEMENTATION
#endif//_CTLE_BASE_TYPES_H_
