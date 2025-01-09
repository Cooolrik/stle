# ctle Copyright (c) 2022 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

from ctlepy import formatted_output

main_comment = '''//
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
'''

def add_macro( name:str , text:str ):
	global undef_text
	global def_text

	undef_text.ln(f'#ifndef {name}')
	undef_text.ln(f'#error The expected macro {name} does not exist.')
	undef_text.ln(f'#endif//{name}')
	undef_text.ln(f'#undef {name}')
	undef_text.ln()

	def_text.ln( text.lstrip() )

def generate_macros( path:str, undef_path:str ):

	global undef_text
	global def_text

	header_text = formatted_output()
	header_text.generate_license_header()
	header_text.ln(main_comment)

	undef_text = formatted_output()
	def_text = formatted_output()

	def_text.comment_ln('Macro marker that the _macros.inl file has been included and the macros are defined. Also makes sure that the _macros.inl file is not included twice, without first including _undef_macros.inl before the second include.')
	def_text.ln( '''#ifdef _CTLE_MACROS_INCLUDED
#error The _macros.inl file is included more than once, without including _undef_macros.inl to undefine the macros before the next include of _macros.inl
#endif//_CTLE_MACROS_INCLUDED
#define _CTLE_MACROS_INCLUDED
''')
	
	undef_text.comment_ln('Remove the marker that _macros.inl has been included. Also make sure that _macros.inl actually was included before the inclusion of _undef_macros.inl')
	undef_text.ln('''#ifndef _CTLE_MACROS_INCLUDED
#error The _macros.inl has not been included before including the _undef_macros.inl file. 
#endif//_CTLE_MACROS_INCLUDED
#undef _CTLE_MACROS_INCLUDED
''')

	def_text.comment_ln('General function signature macro')
	add_macro( '_CTLE_FUNCTION_SIGNATURE', '''
#if defined(_MSC_VER)
#define _CTLE_FUNCTION_SIGNATURE __FUNCSIG__
#elif defined(__GNUC__)
#define _CTLE_FUNCTION_SIGNATURE __PRETTY_FUNCTION__ 
#endif		   
''' )
	
	def_text.comment_ln('_CTLE_STRINGIZE converts a number macro (like __LINE__) into a string.')
	def_text.comment_ln('The _CTLE_STRINGIZE_DETAIL is needed because of how macros work in the preprocessor')
	def_text.comment_ln('E.g. use _CTLE_STRINGIZE(__LINE__) to use __LINE__ as a string macro ')
	def_text.comment_ln('(i.e. convert any number or value to a string: 1 -> "1" )')
	add_macro( '_CTLE_STRINGIZE_DETAIL', '#define _CTLE_STRINGIZE_DETAIL(x) #x' )
	add_macro( '_CTLE_STRINGIZE', '#define _CTLE_STRINGIZE(x) _CTLE_STRINGIZE_DETAIL(x)' )
	def_text.ln()

	def_text.comment_ln('Logging macros for the log.h ctle type')
	add_macro( 'ctLogLevel', '''
#define ctLogLevel( msg_level )\\
	if( ctle::log_level::msg_level <= ctle::get_global_log_level() ) {\\
		ctle::log_msg _ctle_log_entry(ctle::log_level::msg_level,__FILE__,__LINE__,_CTLE_FUNCTION_SIGNATURE); _ctle_log_entry.message()''')
	add_macro( 'ctLogError','#define ctLogError ctLogLevel( error )')
	add_macro( 'ctLogWarning','#define ctLogWarning ctLogLevel( warning )')
	add_macro( 'ctLogInfo','#define ctLogInfo ctLogLevel( info )')
	add_macro( 'ctLogDebug','#define ctLogDebug ctLogLevel( debug )')
	add_macro( 'ctLogVerbose','#define ctLogVerbose ctLogLevel( verbose )')
	add_macro( 'ctLogEnd','#define ctLogEnd ""; }')
	def_text.ln()

	def_text.comment_ln('Logs the current this pointer to the log, for tracing')
	add_macro( 'ctLogThis', '#define ctLogThis ctLogInfo << this << ctLogEnd;' )
	def_text.ln()

	def_text.comment_ln('Checks an expression, and logs an error and returns if the statement is not true')
	add_macro( 'ctValidate', '#define ctValidate( statement , error_code_on_error ) if( !(statement) ) { const ctle::status _ctle_error_code = error_code_on_error; ctLogError' )
	add_macro( 'ctValidateEnd', '#define ctValidateEnd ctLogEnd; return _ctle_error_code; }' )
	def_text.ln()

	def_text.comment_ln('In debug mode, checks expressions which are assumed to be true. if not, throws a runtime error')
	add_macro( 'ctSanityCheck', '''
#ifndef NDEBUG
#define ctSanityCheck( statement )\\
	if( !(statement) ) {\\
		ctLogError << "SanityCheck failed: " << std::string(#statement) << ctLogEnd;\\
		const std::string err_val = std::string("SanityCheck " #statement " failed in " __FILE__ " line " _CTLE_STRINGIZE(__LINE__) " function ") + _CTLE_FUNCTION_SIGNATURE;\\
		throw std::runtime_error( err_val );\\
	}
#else
#define ctSanityCheck( statement )
#endif
''' )

	def_text.comment_ln('Calls a function which returns a ctle::status value, checks the value and reports/returns the value if it is an error, along with a log output')
	add_macro( 'ctStatusCall', '''
#define ctStatusCall( s ) \\
	{\\
		ctle::status _ctle_call_status = (s); \\
		if( !_ctle_call_status ) {\\
			ctLogError << "Call: " << #s << " failed, returned status_code: " << _ctle_call_status << ctLogEnd;\\
			return _ctle_call_status;\\
		}\\
	}
''' )

	def_text.comment_ln('Calls a function which returns a ctle::status_return value, checks the status and reports/returns the status part if it is an error, along with a log output.')
	def_text.comment_ln('If the call succeeds, the retval variable will receive the value part of the return value.')
	def_text.comment_ln('Note that the receiving variable must exist before the call. To create the value, use ctStatusAutoReturnCall, below.')
	add_macro( 'ctStatusReturnCall', '''
#define ctStatusReturnCall( retval , scall ) \\
    {\\
		auto _ctle_call_statuspair = (scall); \\
		if( !_ctle_call_statuspair.status() ) {\\
			ctLogError << "Call: " << #scall << " failed, returned status_code: " << _ctle_call_statuspair.status() << ctLogEnd;\\
			return _ctle_call_statuspair.status();\\
		}\\
		retval = std::move(_ctle_call_statuspair.value());\\
	}
''' )

	def_text.comment_ln('Same as ctStatusReturnCall, above, but first creates the return value before the call, made the same type as the value_type of the status_return return value.')
	def_text.comment_ln('Note that the receiving variable must NOT exist before the call. To use an existing value, use ctStatusReturnCall, above.')
	add_macro( 'ctStatusAutoReturnCall', '''
#define ctStatusAutoReturnCall( retval , scall )\\
	decltype( scall )::value_type retval; ctStatusReturnCall( retval, scall )
''' )

	def_text.comment_ln('Calls a function which returns a ctle::status value, checks the value and throws a status_error exception if the value if it is an error, along with a log output')
	add_macro( 'ctStatusCallThrow', '''
#define ctStatusCallThrow( s ) \\
	{\\
		ctle::status _ctle_call_status = (s); \\
		if( !_ctle_call_status ) {\\
			ctLogError << "Call: " << #s << " failed, returned status_code: " << _ctle_call_status << ", throwing a status_error exception" << ctLogEnd;\\
			throw ctle::status_error( _ctle_call_status );\\
		}\\
	}
''' )


	out = formatted_output()
	out.lines.extend( header_text.lines )
	out.lines.extend( def_text.lines )
	out.write_lines_to_file( path )

	out = formatted_output()
	out.lines.extend( header_text.lines )
	out.lines.extend( undef_text.lines )
	out.write_lines_to_file( undef_path )

