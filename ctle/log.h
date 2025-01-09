// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_LOG_H_
#define _CTLE_LOG_H_

/// @file log.h
/// @brief A simple logging system for ctle, with log levels and a global log function.
/// @details The log system is a simple logging system, which can be used to log messages with different log levels.
/// To use the log system with a set of macros, see the example below.
/// @code{.cpp}
/// // Example macros for logging with the ctle log system
/// 
/// // General function signature macro usable in both MSVC and GCC/Clang
/// #if defined(_MSC_VER)
/// #define _EX_FUNCTION_SIGNATURE __FUNCSIG__
/// #elif defined(__GNUC__)
/// #define _EX_FUNCTION_SIGNATURE __PRETTY_FUNCTION__ 
/// #endif		   
/// 
/// // _EX_STRINGIZE converts a number macro (like __LINE__) into a string.
/// // The _EX_STRINGIZE_DETAIL is needed because of how macros work in the preprocessor
/// // E.g. use _EX_STRINGIZE(__LINE__) to use __LINE__ as a string macro
/// // (i.e. convert any number or value to a string: 1 -> "1" )
/// #define _EX_STRINGIZE_DETAIL(x) #x
/// #define _EX_STRINGIZE(x) _EX_STRINGIZE_DETAIL(x)
/// 
/// // Logging macros for the log.h ctle type
/// #define EX_LOG_LEVEL( msg_level )\
/// 	if( ctle::log_level::msg_level <= ctle::get_global_log_level() ) {\
/// 		ctle::log_msg _EX_log_entry(ctle::log_level::msg_level,__FILE__,__LINE__,_EX_FUNCTION_SIGNATURE); _EX_log_entry.message()
/// #define EX_LOG_ERROR EX_LOG_LEVEL( error )
/// #define EX_LOG_WARNING EX_LOG_LEVEL( warning )
/// #define EX_LOG_INFO EX_LOG_LEVEL( info )
/// #define EX_LOG_DEBUG EX_LOG_LEVEL( debug )
/// #define EX_LOG_VERBOSE EX_LOG_LEVEL( verbose )
/// #define EX_LOG_END ""; }
///
/// @endcode
/// 
/// To use the macros, include the log.h file, and then use the macros in your code like this:
/// @code{.cpp}
/// EX_LOG_INFO << "This is an info message" << EX_LOG_END;
/// EX_LOG_DEBUG << "This is a debug message" << EX_LOG_END;
/// @endcode


#include <sstream>

namespace ctle
{

/// @brief The log levels, used to filter log messages.
enum class log_level : unsigned int
{
	error = 0,		///< log errors
	warning = 1,	///< log warnings
	info = 2,		///< log important info, (default level)
	debug = 3,		///< log debug messages 
	verbose = 4		///< log extra verbose debug info 
};

typedef void ( *log_function )( log_level level, const char *function_name, const char *message );

/// @brief set the global log level, used to filter logs
/// @param level the log level to set
void set_global_log_level(log_level level);

/// @brief get the global log level
/// @return the current global log level
log_level get_global_log_level();

/// @brief get the log_level value as a string
/// @param level the log_level value
/// @return the log_level value as a string
const char* get_log_level_as_string(log_level level);

// set/get the global log function, which receives the log messages

/// @brief Set the global log function, which receives the log messages. This will replace any current log function.
/// @param func the log function to set
void set_global_log_function(log_function func);

/// @brief Get the global log function, which receives the log messages.
/// @return the current global log function
log_function get_global_log_function();

/// @brief A log message object, which logs a message when it goes out of scope. Can be used with a stream to construct the message.
class log_msg
{
private:
	log_level level;
	std::stringstream msg;
	const char *file_name;
	int file_line;
	const char *func_sig;

	// enters the message into the log
	void enter_msg();

public:
	log_msg( log_level _level, const char *_file_name = nullptr, int _file_line = 0, const char *_func_sig = nullptr ) noexcept 
		: level( _level )
		, file_name( _file_name )
		, file_line( _file_line )
		, func_sig( _func_sig )
	{}

	~log_msg()
	{
		this->enter_msg();
	}

	/// @brief access the message member to construct the message
	/// @return a reference to the message stream, which is used to construct the message
	std::stringstream& message()
	{
		return msg;
	}
};

}
// namespace ctle

#ifdef CTLE_IMPLEMENTATION

#include <unordered_map>
#include <iostream>

#include "string_funcs.h"

namespace ctle
{

static log_level global_log_level = log_level::info;

void set_global_log_level( log_level level )
{
	global_log_level = level;
}
log_level get_global_log_level()
{
	return global_log_level;
}

static void default_global_log_function( log_level level, const char *function_name, const char *message )
{
	std::cout << get_log_level_as_string( level ) << " log: " << function_name << ": " << message << std::endl;
}

static log_function global_log_function = &default_global_log_function;

void set_global_log_function( log_function func )
{
	global_log_function = func;
}
log_function get_global_log_function()
{
	return global_log_function;
}

static const std::unordered_map<log_level, const char *> log_level_to_string
{
	{ log_level::error , "Error" },
	{ log_level::warning , "Warning" },
	{ log_level::info , "Info" },
	{ log_level::debug , "Debug" },
	{ log_level::verbose , "Verbose" }
};

const char *get_log_level_as_string( log_level level )
{
	std::unordered_map<log_level, const char *>::const_iterator it = log_level_to_string.find( level );
	if( it == log_level_to_string.end() )
		return nullptr;
	return it->second;
}

inline bool trim_function_signature_span( string_span<char> &sig )
{
	// try finding a '(', start of function
	size_t rightTrim = strchr_t( sig, '(' );
	if( rightTrim < sig.length() )
	{
		sig.end = sig.start + rightTrim; // up to, but dont inclue the '('

		// now, search back for the first whitespace
		size_t leftTrim = strcrspn_t( sig, " \t\n\r" );
		if( leftTrim < sig.length() )
		{
			sig.start = sig.start + leftTrim + 1; // up to, but dont inclue the whitespace
		}

		return true;
	}

	return false;
}


void log_msg::enter_msg()
{
	std::string func_log;
	if( this->func_sig )
	{
		// get the span of the function signature
		string_span<char> sig = { this->func_sig, this->func_sig + strlen( this->func_sig ) };

		// try trimming the signature, just keep the function name (including namespaces & class)
		if( trim_function_signature_span( sig ) )
		{
			func_log = std::string( sig ) + "()";
		}
		else
		{
			func_log = std::string( this->func_sig );
		}
	}

	// call the log function
	( *global_log_function )( this->level, func_log.c_str(), this->msg.str().c_str() );
}

}
// namespace ctle

#endif//CTLE_IMPLEMENTATION

#ifdef CTLE_IMPLEMENT_EXAMPLE_LOG_MACROS

#if defined(_MSC_VER)
#define LOG_FUNCTION_NAME __FUNCSIG__
#elif defined(__GNUC__)
#define LOG_FUNCTION_NAME __PRETTY_FUNCTION__
#endif

#define ctle_log( msg_level )\
	if( ctle::log_level::msg_level <= ctle::get_global_log_level() ) {\
		ctle::log_msg _ctle_log_entry(ctle::log_level::msg_level,__FILE__,__LINE__,LOG_FUNCTION_NAME); _ctle_log_entry.message()

#define ctle_log_error ctle_log( error )
#define ctle_log_warning ctle_log( warning )
#define ctle_log_info ctle_log( info )
#define ctle_log_debug ctle_log( debug )
#define ctle_log_verbose ctle_log( verbose )

#define ctle_log_end ""; }

#endif//CTLE_IMPLEMENT_EXAMPLE_LOG_MACROS

#endif//_CTLE_LOG_H_
