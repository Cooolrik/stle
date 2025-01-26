// ctle Copyright (c) 2022 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
//
// _macros.inl & _undef_macros.inl are used to define convenience macros 
// for implementation files. 
//
// usage: include _macros.inl in implementation, *after* any other header file, so 
// as to not pollute any of those files with the macros defined. 
//
// If you *really* need to include in a header file (not recommended), include the
// _undef_macros.inl after the macros are used in the same file. This undefines/removes the
// macros from the rest of files used, and does not pollute the other parts of compilation.
//
// The _macros.inl and _undef_macros.inl can be safely included multiple times, 
// but pair each inclusion of _macros.inl with an inclusion of _undef_macros.inl, 
// before the next inclusion of _macros.inl
//

// Macro marker that the _macros.inl file has been included and the macros are defined. Also makes sure that the
// _macros.inl file is not included twice, without first including _undef_macros.inl before the second include.
#ifdef _CTLE_MACROS_INCLUDED
#error The _macros.inl file is included more than once, without including _undef_macros.inl to undefine the macros before the next include of _macros.inl
#endif//_CTLE_MACROS_INCLUDED
#define _CTLE_MACROS_INCLUDED

// General function signature macro
#if defined(_MSC_VER)
#define _CTLE_FUNCTION_SIGNATURE __FUNCSIG__
#elif defined(__GNUC__)
#define _CTLE_FUNCTION_SIGNATURE __PRETTY_FUNCTION__ 
#endif		   

// _CTLE_STRINGIZE converts a number macro (like __LINE__) into a string.
// The _CTLE_STRINGIZE_DETAIL is needed because of how macros work in the preprocessor
// E.g. use _CTLE_STRINGIZE(__LINE__) to use __LINE__ as a string macro
// (i.e. convert any number or value to a string: 1 -> "1" )
#define _CTLE_STRINGIZE_DETAIL(x) #x
#define _CTLE_STRINGIZE(x) _CTLE_STRINGIZE_DETAIL(x)

// Logging macros for the log.h ctle type
#define ctLogLevel( msg_level )\
	if( ctle::log_level::msg_level <= ctle::get_global_log_level() ) {\
		ctle::log_msg _ctle_log_entry(ctle::log_level::msg_level,__FILE__,__LINE__,_CTLE_FUNCTION_SIGNATURE); _ctle_log_entry.message()
#define ctLogError ctLogLevel( error )
#define ctLogWarning ctLogLevel( warning )
#define ctLogInfo ctLogLevel( info )
#define ctLogDebug ctLogLevel( debug )
#define ctLogVerbose ctLogLevel( verbose )
#define ctLogEnd ""; }

// Logs the current this pointer to the log, for tracing
#define ctLogThis ctLogInfo << this << ctLogEnd;

// Checks an expression, and logs an error and returns if the statement is not true
#define ctValidate( statement , error_code_on_error ) if( !(statement) ) { const ctle::status _ctle_error_code = error_code_on_error; ctLogError
#define ctValidateEnd ctLogEnd; return _ctle_error_code; }

// In debug mode, checks expressions which are assumed to be true. if not, throws a runtime error
#ifndef NDEBUG
#define ctSanityCheck( statement )\
	if( !(statement) ) {\
		ctLogError << "SanityCheck failed: " << std::string(#statement) << ctLogEnd;\
		const std::string err_val = std::string("SanityCheck " #statement " failed in " __FILE__ " line " _CTLE_STRINGIZE(__LINE__) " function ") + _CTLE_FUNCTION_SIGNATURE;\
		throw std::runtime_error( err_val );\
	}
#else
#define ctSanityCheck( statement )
#endif

// Calls a function which returns a ctle::status value, checks the value and reports/returns the value if it is an error,
// along with a log output
#define ctStatusCall( s ) \
	{\
		ctle::status _ctle_call_status = (s); \
		if( !_ctle_call_status ) {\
			ctLogError << "Call: " << #s << " failed, returned status_code: " << _ctle_call_status << ctLogEnd;\
			return _ctle_call_status;\
		}\
	}

// Calls a function which returns a ctle::status_return value, checks the status and reports/returns the status part if it
// is an error, along with a log output.
// If the call succeeds, the retval variable will receive the value part of the return value.
// Note that the receiving variable must exist before the call. To create the value, use ctStatusAutoReturnCall, below.
#define ctStatusReturnCall( retval , scall ) \
    {\
		auto _ctle_call_statuspair = (scall); \
		if( !_ctle_call_statuspair.status() ) {\
			ctLogError << "Call: " << #scall << " failed, returned status_code: " << _ctle_call_statuspair.status() << ctLogEnd;\
			return _ctle_call_statuspair.status();\
		}\
		retval = std::move(_ctle_call_statuspair.value());\
	}

// Same as ctStatusReturnCall, above, but first creates the return value before the call, made the same type as the
// value_type of the status_return return value.
// Note that the receiving variable must NOT exist before the call. To use an existing value, use ctStatusReturnCall,
// above.
#define ctStatusAutoReturnCall( retval , scall )\
	decltype( scall )::value_type retval; ctStatusReturnCall( retval, scall )

// Calls a function which returns a ctle::status value, checks the value and throws a status_error exception if the value
// if it is an error, along with a log output
#define ctStatusCallThrow( s ) \
	{\
		ctle::status _ctle_call_status = (s); \
		if( !_ctle_call_status ) {\
			ctLogError << "Call: " << #s << " failed, returned status_code: " << _ctle_call_status << ", throwing a status_error exception" << ctLogEnd;\
			throw ctle::status_error( _ctle_call_status );\
		}\
	}

