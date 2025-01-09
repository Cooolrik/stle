# ctle Copyright (c) 2022 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

from ctlepy import formatted_output

class error_value:
	def __init__(self,name,value,mapped_value,description):
		self.name = name.strip()
		self.value = value
		if mapped_value != None:
			self.mapped_value = mapped_value.strip()
		else:
			self.mapped_value = None
		self.description = description.strip()

class error_type:
	def __init__(self,type_name,values):
		self.type_name = type_name.strip()
		self.values = values

no_error = error_type('no error, success' , [ 
	error_value( 'ok' , 0 , '', 'ok, no error, successful' ) 
	] )

general = error_type('general errors' , [ 	
	error_value( 'undefined_error       ', -100, None, 'undefined error' ),
	error_value( 'invalid_param         ', -101, None, 'invalid parameter in method call' ),
	error_value( 'not_initialized       ', -102, None, 'the system is not initialized' ),
	error_value( 'already_initialized   ', -103, None, 'the system or class is already initialized or in a specific state' ),
	error_value( 'cant_allocate         ', -104, None, 'cant allocate memory' ),
	error_value( 'cant_open             ', -105, None, 'cant open file or handle' ),
	error_value( 'cant_read             ', -106, None, 'cant read from file or handle' ),
	error_value( 'corrupted             ', -107, None, 'a file, data or object is corrupted ' ),
	error_value( 'invalid               ', -108, None, 'invalid file, wrong file format, or failed validation' ),
	error_value( 'cant_write            ', -109, None, 'cant write to file or handle' ),
	error_value( 'not_found             ', -110, None, 'one or multiple objects are missing or not found' ),
	error_value( 'not_ready             ', -111, None, 'one or multiple objects are not ready, or out of sync' ),
	error_value( 'cant_access           ', -112, None, 'access is not allowed' ),
	error_value( 'already_exists        ', -113, None, 'a file or item already exists' ),
	] )

stl_errors = error_type('stl portable errors (from errc)' , [ 
	error_value( 'stl_unrecognized_error_code            ', -1000, None, 'unknown/unrecognized STL error, which could not be mapped to a specific error value' ),
	error_value( 'stl_address_family_not_supported       ', -1001, 'std::errc::address_family_not_supported        ', 'stl error code: errc::address_family_not_supported' ),
	error_value( 'stl_address_in_use                     ', -1002, 'std::errc::address_in_use                      ', 'stl error code: errc::address_in_use' ),
	error_value( 'stl_address_not_available              ', -1003, 'std::errc::address_not_available               ', 'stl error code: errc::address_not_available' ),
	error_value( 'stl_already_connected                  ', -1004, 'std::errc::already_connected                   ', 'stl error code: errc::already_connected' ),
	error_value( 'stl_argument_list_too_long             ', -1005, 'std::errc::argument_list_too_long              ', 'stl error code: errc::argument_list_too_long' ),
	error_value( 'stl_argument_out_of_domain             ', -1006, 'std::errc::argument_out_of_domain              ', 'stl error code: errc::argument_out_of_domain' ),
	error_value( 'stl_bad_address                        ', -1007, 'std::errc::bad_address                         ', 'stl error code: errc::bad_address' ),
	error_value( 'stl_bad_file_descriptor                ', -1008, 'std::errc::bad_file_descriptor                 ', 'stl error code: errc::bad_file_descriptor' ),
	error_value( 'stl_bad_message                        ', -1009, 'std::errc::bad_message                         ', 'stl error code: errc::bad_message' ),
	error_value( 'stl_broken_pipe                        ', -1010, 'std::errc::broken_pipe                         ', 'stl error code: errc::broken_pipe' ),
	error_value( 'stl_connection_aborted                 ', -1011, 'std::errc::connection_aborted                  ', 'stl error code: errc::connection_aborted' ),
	error_value( 'stl_connection_already_in_progress     ', -1012, 'std::errc::connection_already_in_progress      ', 'stl error code: errc::connection_already_in_progress' ),
	error_value( 'stl_connection_refused                 ', -1013, 'std::errc::connection_refused                  ', 'stl error code: errc::connection_refused' ),
	error_value( 'stl_connection_reset                   ', -1014, 'std::errc::connection_reset                    ', 'stl error code: errc::connection_reset' ),
	error_value( 'stl_cross_device_link                  ', -1015, 'std::errc::cross_device_link                   ', 'stl error code: errc::cross_device_link' ),
	error_value( 'stl_destination_address_required       ', -1016, 'std::errc::destination_address_required        ', 'stl error code: errc::destination_address_required' ),
	error_value( 'stl_device_or_resource_busy            ', -1017, 'std::errc::device_or_resource_busy             ', 'stl error code: errc::device_or_resource_busy' ),
	error_value( 'stl_directory_not_empty                ', -1018, 'std::errc::directory_not_empty                 ', 'stl error code: errc::directory_not_empty' ),
	error_value( 'stl_executable_format_error            ', -1019, 'std::errc::executable_format_error             ', 'stl error code: errc::executable_format_error' ),
	error_value( 'stl_file_exists                        ', -1020, 'std::errc::file_exists                         ', 'stl error code: errc::file_exists' ),
	error_value( 'stl_file_too_large                     ', -1021, 'std::errc::file_too_large                      ', 'stl error code: errc::file_too_large' ),
	error_value( 'stl_filename_too_long                  ', -1022, 'std::errc::filename_too_long                   ', 'stl error code: errc::filename_too_long' ),
	error_value( 'stl_function_not_supported             ', -1023, 'std::errc::function_not_supported              ', 'stl error code: errc::function_not_supported' ),
	error_value( 'stl_host_unreachable                   ', -1024, 'std::errc::host_unreachable                    ', 'stl error code: errc::host_unreachable' ),
	error_value( 'stl_identifier_removed                 ', -1025, 'std::errc::identifier_removed                  ', 'stl error code: errc::identifier_removed' ),
	error_value( 'stl_illegal_byte_sequence              ', -1026, 'std::errc::illegal_byte_sequence               ', 'stl error code: errc::illegal_byte_sequence' ),
	error_value( 'stl_inappropriate_io_control_operation ', -1027, 'std::errc::inappropriate_io_control_operation  ', 'stl error code: errc::inappropriate_io_control_operation' ),
	error_value( 'stl_interrupted                        ', -1028, 'std::errc::interrupted                         ', 'stl error code: errc::interrupted' ),
	error_value( 'stl_invalid_argument                   ', -1029, 'std::errc::invalid_argument                    ', 'stl error code: errc::invalid_argument' ),
	error_value( 'stl_invalid_seek                       ', -1030, 'std::errc::invalid_seek                        ', 'stl error code: errc::invalid_seek' ),
	error_value( 'stl_io_error                           ', -1031, 'std::errc::io_error                            ', 'stl error code: errc::io_error' ),
	error_value( 'stl_is_a_directory                     ', -1032, 'std::errc::is_a_directory                      ', 'stl error code: errc::is_a_directory' ),
	error_value( 'stl_message_size                       ', -1033, 'std::errc::message_size                        ', 'stl error code: errc::message_size' ),
	error_value( 'stl_network_down                       ', -1034, 'std::errc::network_down                        ', 'stl error code: errc::network_down' ),
	error_value( 'stl_network_reset                      ', -1035, 'std::errc::network_reset                       ', 'stl error code: errc::network_reset' ),
	error_value( 'stl_network_unreachable                ', -1036, 'std::errc::network_unreachable                 ', 'stl error code: errc::network_unreachable' ),
	error_value( 'stl_no_buffer_space                    ', -1037, 'std::errc::no_buffer_space                     ', 'stl error code: errc::no_buffer_space' ),
	error_value( 'stl_no_child_process                   ', -1038, 'std::errc::no_child_process                    ', 'stl error code: errc::no_child_process' ),
	error_value( 'stl_no_link                            ', -1039, 'std::errc::no_link                             ', 'stl error code: errc::no_link' ),
	error_value( 'stl_no_lock_available                  ', -1040, 'std::errc::no_lock_available                   ', 'stl error code: errc::no_lock_available' ),
	error_value( 'stl_no_message_available               ', -1041, 'std::errc::no_message_available                ', 'stl error code: errc::no_message_available' ),
	error_value( 'stl_no_message                         ', -1042, 'std::errc::no_message                          ', 'stl error code: errc::no_message' ),
	error_value( 'stl_no_protocol_option                 ', -1043, 'std::errc::no_protocol_option                  ', 'stl error code: errc::no_protocol_option' ),
	error_value( 'stl_no_space_on_device                 ', -1044, 'std::errc::no_space_on_device                  ', 'stl error code: errc::no_space_on_device' ),
	error_value( 'stl_no_stream_resources                ', -1045, 'std::errc::no_stream_resources                 ', 'stl error code: errc::no_stream_resources' ),
	error_value( 'stl_no_such_device_or_address          ', -1046, 'std::errc::no_such_device_or_address           ', 'stl error code: errc::no_such_device_or_address' ),
	error_value( 'stl_no_such_device                     ', -1047, 'std::errc::no_such_device                      ', 'stl error code: errc::no_such_device' ),
	error_value( 'stl_no_such_file_or_directory          ', -1048, 'std::errc::no_such_file_or_directory           ', 'stl error code: errc::no_such_file_or_directory' ),
	error_value( 'stl_no_such_process                    ', -1049, 'std::errc::no_such_process                     ', 'stl error code: errc::no_such_process' ),
	error_value( 'stl_not_a_directory                    ', -1050, 'std::errc::not_a_directory                     ', 'stl error code: errc::not_a_directory' ),
	error_value( 'stl_not_a_socket                       ', -1051, 'std::errc::not_a_socket                        ', 'stl error code: errc::not_a_socket' ),
	error_value( 'stl_not_a_stream                       ', -1052, 'std::errc::not_a_stream                        ', 'stl error code: errc::not_a_stream' ),
	error_value( 'stl_not_connected                      ', -1053, 'std::errc::not_connected                       ', 'stl error code: errc::not_connected' ),
	error_value( 'stl_not_enough_memory                  ', -1054, 'std::errc::not_enough_memory                   ', 'stl error code: errc::not_enough_memory' ),
	error_value( 'stl_not_supported                      ', -1055, 'std::errc::not_supported                       ', 'stl error code: errc::not_supported' ),
	error_value( 'stl_operation_canceled                 ', -1056, 'std::errc::operation_canceled                  ', 'stl error code: errc::operation_canceled' ),
	error_value( 'stl_operation_in_progress              ', -1057, 'std::errc::operation_in_progress               ', 'stl error code: errc::operation_in_progress' ),
	error_value( 'stl_operation_not_permitted            ', -1058, 'std::errc::operation_not_permitted             ', 'stl error code: errc::operation_not_permitted' ),
	error_value( 'stl_operation_not_supported            ', -1059, 'std::errc::operation_not_supported             ', 'stl error code: errc::operation_not_supported' ),
	error_value( 'stl_operation_would_block              ', -1060, 'std::errc::operation_would_block               ', 'stl error code: errc::operation_would_block' ),
	error_value( 'stl_owner_dead                         ', -1061, 'std::errc::owner_dead                          ', 'stl error code: errc::owner_dead' ),
	error_value( 'stl_permission_denied                  ', -1062, 'std::errc::permission_denied                   ', 'stl error code: errc::permission_denied' ),
	error_value( 'stl_protocol_error                     ', -1063, 'std::errc::protocol_error                      ', 'stl error code: errc::protocol_error' ),
	error_value( 'stl_protocol_not_supported             ', -1064, 'std::errc::protocol_not_supported              ', 'stl error code: errc::protocol_not_supported' ),
	error_value( 'stl_read_only_file_system              ', -1065, 'std::errc::read_only_file_system               ', 'stl error code: errc::read_only_file_system' ),
	error_value( 'stl_resource_deadlock_would_occur      ', -1066, 'std::errc::resource_deadlock_would_occur       ', 'stl error code: errc::resource_deadlock_would_occur' ),
	error_value( 'stl_resource_unavailable_try_again     ', -1067, 'std::errc::resource_unavailable_try_again      ', 'stl error code: errc::resource_unavailable_try_again' ),
	error_value( 'stl_result_out_of_range                ', -1068, 'std::errc::result_out_of_range                 ', 'stl error code: errc::result_out_of_range' ),
	error_value( 'stl_state_not_recoverable              ', -1069, 'std::errc::state_not_recoverable               ', 'stl error code: errc::state_not_recoverable' ),
	error_value( 'stl_stream_timeout                     ', -1070, 'std::errc::stream_timeout                      ', 'stl error code: errc::stream_timeout' ),
	error_value( 'stl_text_file_busy                     ', -1071, 'std::errc::text_file_busy                      ', 'stl error code: errc::text_file_busy' ),
	error_value( 'stl_timed_out                          ', -1072, 'std::errc::timed_out                           ', 'stl error code: errc::timed_out' ),
	error_value( 'stl_too_many_files_open_in_system      ', -1073, 'std::errc::too_many_files_open_in_system       ', 'stl error code: errc::too_many_files_open_in_system' ),
	error_value( 'stl_too_many_files_open                ', -1074, 'std::errc::too_many_files_open                 ', 'stl error code: errc::too_many_files_open' ),
	error_value( 'stl_too_many_links                     ', -1075, 'std::errc::too_many_links                      ', 'stl error code: errc::too_many_links' ),
	error_value( 'stl_too_many_symbolic_link_levels      ', -1076, 'std::errc::too_many_symbolic_link_levels       ', 'stl error code: errc::too_many_symbolic_link_levels' ),
	error_value( 'stl_value_too_large                    ', -1077, 'std::errc::value_too_large                     ', 'stl error code: errc::value_too_large' ),
	error_value( 'stl_wrong_protocol_type                ', -1078, 'std::errc::wrong_protocol_type                 ', 'stl error code: errc::wrong_protocol_type' ),
	] )

vulkan_errors = error_type('Vulkan errors' , [ 	
	error_value( 'vulkan_unrecognized_error_code                     ', -2000, None, 'unknown/unrecognized Vulkan error, which could not be mapped to a specific error value' ),
	error_value( 'vulkan_out_of_host_memory                          ', -2001, 'VkResult::VK_ERROR_OUT_OF_HOST_MEMORY                          ', 'Vulkan error code VkResult::VK_ERROR_OUT_OF_HOST_MEMORY' ),
	error_value( 'vulkan_out_of_device_memory                        ', -2002, 'VkResult::VK_ERROR_OUT_OF_DEVICE_MEMORY                        ', 'Vulkan error code VkResult::VK_ERROR_OUT_OF_DEVICE_MEMORY' ),
	error_value( 'vulkan_initialization_failed                       ', -2003, 'VkResult::VK_ERROR_INITIALIZATION_FAILED                       ', 'Vulkan error code VkResult::VK_ERROR_INITIALIZATION_FAILED' ),
	error_value( 'vulkan_device_lost                                 ', -2004, 'VkResult::VK_ERROR_DEVICE_LOST                                 ', 'Vulkan error code VkResult::VK_ERROR_DEVICE_LOST' ),
	error_value( 'vulkan_memory_map_failed                           ', -2005, 'VkResult::VK_ERROR_MEMORY_MAP_FAILED                           ', 'Vulkan error code VkResult::VK_ERROR_MEMORY_MAP_FAILED' ),
	error_value( 'vulkan_layer_not_present                           ', -2006, 'VkResult::VK_ERROR_LAYER_NOT_PRESENT                           ', 'Vulkan error code VkResult::VK_ERROR_LAYER_NOT_PRESENT' ),
	error_value( 'vulkan_extension_not_present                       ', -2007, 'VkResult::VK_ERROR_EXTENSION_NOT_PRESENT                       ', 'Vulkan error code VkResult::VK_ERROR_EXTENSION_NOT_PRESENT' ),
	error_value( 'vulkan_feature_not_present                         ', -2008, 'VkResult::VK_ERROR_FEATURE_NOT_PRESENT                         ', 'Vulkan error code VkResult::VK_ERROR_FEATURE_NOT_PRESENT' ),
	error_value( 'vulkan_incompatible_driver                         ', -2009, 'VkResult::VK_ERROR_INCOMPATIBLE_DRIVER                         ', 'Vulkan error code VkResult::VK_ERROR_INCOMPATIBLE_DRIVER' ),
	error_value( 'vulkan_too_many_objects                            ', -2010, 'VkResult::VK_ERROR_TOO_MANY_OBJECTS                            ', 'Vulkan error code VkResult::VK_ERROR_TOO_MANY_OBJECTS' ),
	error_value( 'vulkan_format_not_supported                        ', -2011, 'VkResult::VK_ERROR_FORMAT_NOT_SUPPORTED                        ', 'Vulkan error code VkResult::VK_ERROR_FORMAT_NOT_SUPPORTED' ),
	error_value( 'vulkan_fragmented_pool                             ', -2012, 'VkResult::VK_ERROR_FRAGMENTED_POOL                             ', 'Vulkan error code VkResult::VK_ERROR_FRAGMENTED_POOL' ),
	error_value( 'vulkan_unknown                                     ', -2013, 'VkResult::VK_ERROR_UNKNOWN                                     ', 'Vulkan error code VkResult::VK_ERROR_UNKNOWN' ),
	error_value( 'vulkan_out_of_pool_memory                          ', -2014, 'VkResult::VK_ERROR_OUT_OF_POOL_MEMORY                          ', 'Vulkan error code VkResult::VK_ERROR_OUT_OF_POOL_MEMORY' ),
	error_value( 'vulkan_invalid_external_handle                     ', -2015, 'VkResult::VK_ERROR_INVALID_EXTERNAL_HANDLE                     ', 'Vulkan error code VkResult::VK_ERROR_INVALID_EXTERNAL_HANDLE' ),
	error_value( 'vulkan_fragmentation                               ', -2016, 'VkResult::VK_ERROR_FRAGMENTATION                               ', 'Vulkan error code VkResult::VK_ERROR_FRAGMENTATION' ),
	error_value( 'vulkan_invalid_opaque_capture_address              ', -2017, 'VkResult::VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS              ', 'Vulkan error code VkResult::VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS' ),
	error_value( 'vulkan_surface_lost_khr                            ', -2018, 'VkResult::VK_ERROR_SURFACE_LOST_KHR                            ', 'Vulkan error code VkResult::VK_ERROR_SURFACE_LOST_KHR' ),
	error_value( 'vulkan_native_window_in_use_khr                    ', -2019, 'VkResult::VK_ERROR_NATIVE_WINDOW_IN_USE_KHR                    ', 'Vulkan error code VkResult::VK_ERROR_NATIVE_WINDOW_IN_USE_KHR' ),
	error_value( 'vulkan_out_of_date_khr                             ', -2020, 'VkResult::VK_ERROR_OUT_OF_DATE_KHR                             ', 'Vulkan error code VkResult::VK_ERROR_OUT_OF_DATE_KHR' ),
	error_value( 'vulkan_incompatible_display_khr                    ', -2021, 'VkResult::VK_ERROR_INCOMPATIBLE_DISPLAY_KHR                    ', 'Vulkan error code VkResult::VK_ERROR_INCOMPATIBLE_DISPLAY_KHR' ),
	error_value( 'vulkan_validation_failed_ext                       ', -2022, 'VkResult::VK_ERROR_VALIDATION_FAILED_EXT                       ', 'Vulkan error code VkResult::VK_ERROR_VALIDATION_FAILED_EXT' ),
	error_value( 'vulkan_invalid_shader_nv                           ', -2023, 'VkResult::VK_ERROR_INVALID_SHADER_NV                           ', 'Vulkan error code VkResult::VK_ERROR_INVALID_SHADER_NV' ),
	error_value( 'vulkan_invalid_drm_format_modifier_plane_layout_ext', -2024, 'VkResult::VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT', 'Vulkan error code VkResult::VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT' ),
	error_value( 'vulkan_not_permitted_khr                           ', -2025, 'VkResult::VK_ERROR_NOT_PERMITTED_KHR                           ', 'Vulkan error code VkResult::VK_ERROR_NOT_PERMITTED_KHR' ),
	error_value( 'vulkan_full_screen_exclusive_mode_lost_ext         ', -2026, 'VkResult::VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT         ', 'Vulkan error code VkResult::VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT' ),
	error_value( 'vulkan_compression_exhausted_ext                   ', -2027, 'VkResult::VK_ERROR_COMPRESSION_EXHAUSTED_EXT                   ', 'Vulkan error code VkResult::VK_ERROR_COMPRESSION_EXHAUSTED_EXT' ),
	] )

error_types = [ 
	no_error , 
	general ,
	stl_errors , 
	vulkan_errors ,
	]

def generate_status( path:str ):

	out = formatted_output()

	out.generate_license_header()
	guard_def = out.begin_header_guard( 'status.h', 'ctle' )

	out.lines.append( '''
#include <iosfwd>

namespace ctle
{
	enum class status_code : int
	{''')

	out.indentation = 2

	for type in error_types:
		out.comment_ln( type.type_name )
		for value in type.values:
			out.ln( f'{value.name.ljust(52)}= {value.value}, // {value.description}' )
		out.ln()

	out.lines.append( '''	};

	// status maps a number of error values into one error enum
	// and has ctors which converts these error values into a status_code value
	class status
	{
	public:
''')

	out.indentation = 2

	for type in error_types:
		out.comment_ln( type.type_name )
		for value in type.values:
			out.ln( f'static const status {(value.name + ";").ljust(53)} // {value.description}' )
		out.ln()

	out.lines.append( '''	private:
		status_code svalue = status_code::ok;

	public:
		status() = default;
		status( const status &other ) = default;

		status( const status_code &_value ) noexcept : svalue( _value ) {}
		const status &operator = ( const status_code &_value ) noexcept { this->svalue = _value; return *this; }

#ifdef _SYSTEM_ERROR_
		// convert from STL std::errc
		static status_code to_status_code( std::errc _value ) noexcept;
		status( const std::errc &_value ) noexcept : svalue( to_status_code(_value) ) {}
		const status &operator = ( const std::errc &_value ) noexcept { this->svalue = to_status_code(_value); return *this; }
#endif//_SYSTEM_ERROR_

#ifdef VULKAN_CORE_H_
		// convert from Vulkan error: VkResult
		static status_code to_status_code( VkResult _value ) noexcept;
		status( const VkResult &_value ) noexcept : svalue( to_status_code(_value) ) {}
		const status &operator = ( const VkResult &_value ) noexcept { this->svalue = to_status_code(_value); return *this; }
#endif//VULKAN_CORE_H_

		// use as a bool
		operator bool() const { return svalue >= status_code::ok; }
		bool operator !() const { return svalue < status_code::ok; }

		// compare to status_code
		bool operator == ( const status_code &_value ) const noexcept { return this->svalue == _value; }
		bool operator != ( const status_code &_value ) const noexcept { return this->svalue != _value; }

		// get the status_code value 
		status_code value() const { return svalue; }

		// get the name of the status code as a string 
		const char* name() const;

		// get a description of the status code value
		const char* description() const;
	};		  
}
// namespace ctle

std::ostream &operator<<( std::ostream &os, const ctle::status &_status );

#ifdef CTLE_IMPLEMENTATION

#include <string>
#include <unordered_map>

namespace ctle				  
{
''')

	out.indentation = 1		

	for type in error_types:
		out.comment_ln( type.type_name )
		for value in type.values:
			out.ln( f'const status status::{value.name} = status_code::{value.name};')
		out.ln()	

	out.lines.append('''	struct status_code_string_description
	{
		const char *name;
		const char *description;
	};

	static const std::unordered_map<status_code, status_code_string_description> status_code_string_descriptions = 
	{''')
			
	out.indentation = 2

	for type in error_types:
		out.comment_ln( type.type_name )
		for value in type.values:
			out.ln( f'{{ status_code::{value.name} , {{ "{value.name}", "{value.description}" }} }} , ')
		out.ln('')		
			
	out.lines.append( '''	};

	// get the name of the status code as a string 
	const char* status::name() const
	{
		auto it = status_code_string_descriptions.find( this->svalue );
		if( it == status_code_string_descriptions.end() )
		{
			return "";
		}
		return it->second.name;
	}
	
	// get a description of the status code value
	const char* status::description() const
	{
		auto it = status_code_string_descriptions.find( this->svalue );
		if( it == status_code_string_descriptions.end() )
		{
			return "";
		}
		return it->second.description;
	}
		
#ifdef _SYSTEM_ERROR_
	static const std::unordered_map<std::errc, status_code> errc_to_status_code_mapping =
	{''' )
			
	for value in stl_errors.values:
		if( value.mapped_value != None ):
			out.ln( f'{{ {value.mapped_value} , status_code::{value.name} }} , ')
			
	out.lines.append( '''	};

	status_code status::to_status_code( std::errc value ) noexcept
	{
		auto it = errc_to_status_code_mapping.find( value );
		if( it == errc_to_status_code_mapping.end() )
			return status_code::stl_unrecognized_error_code;
		return it->second;
	}

#endif//_SYSTEM_ERROR_

#ifdef VULKAN_CORE_H_
	static const std::unordered_map<VkResult, status_code> vkresult_to_status_code_mapping =
	{''' )
			
	for value in vulkan_errors.values:
		if( value.mapped_value != None ):
			out.ln( f'{{ {value.mapped_value} , status_code::{value.name} }} , ')
					
	out.lines.append('''	};
	
	status_code status::to_status_code( VkResult value ) noexcept
	{
		if( value >= VK_SUCCESS )	
			return status_code::ok;
		auto it = vkresult_to_status_code_mapping.find( value );
		if( it == vkresult_to_status_code_mapping.end() )
			return status_code::vulkan_unrecognized_error_code;
		return it->second;
	}

#endif//VULKAN_CORE_H_

}
// namespace ctle

// stream operator for writing a status to a stream
std::ostream &operator<<( std::ostream &os, const ctle::status &_status )
{
	os << _status.name() << std::string(" (\\"") << _status.description() << std::string("\\")");
	return os;
}
				  
#endif//CTLE_IMPLEMENTATION  

''')	
	out.indentation = 0
	
	out.end_header_guard( guard_def )

	out.write_lines_to_file( path )