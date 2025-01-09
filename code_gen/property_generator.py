# ctle Copyright (c) 2022 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

from ctlepy import formatted_output
from enum import Enum

class pass_type(Enum):
    cpy = 1,
    cref = 2
    cptr = 3

pass_type_string = { 
	pass_type.cpy: '_Ty', 
	pass_type.cref: 'const _Ty &',
 	pass_type.cptr: 'const _Ty *'
	}

class method_type(Enum):
    method = 1
    operator_cast = 2
    operator_ptr = 3
    operator_assign = 4
    
class storage_type(Enum):
    value = 1,
    ptr = 2,
    atomic = 3,
    custom = 4

class get_method:
	def __init__( self, methodType:method_type, passType:pass_type, statusUpdate:bool, noexcept:bool ):
		self.methodType = methodType
		self.passType = passType
		self.statusUpdate = statusUpdate
		self.noexcept = noexcept
  
	def generate_description( self, out ):
		match self.methodType:
			case method_type.method:
				out.comment_ln(f'property get method which returns {pass_type_string[self.passType]}')
			case method_type.operator_cast:
				out.comment_ln(f'property cast to {pass_type_string[self.passType]} operator')
			case method_type.operator_ptr:
				out.comment_ln(f'pointer operator which returns {pass_type_string[self.passType]}')
			case _:
				raise Exception("Invalid method type")
		if self.statusUpdate:
			out.comment_ln('- note: sets the result of the call in the result status parameter')
		if self.noexcept:
			out.comment_ln('- note: does not raise any exceptions')
		else:
			out.comment_ln('- note: raises a status_exception if the call fails')
  
	def get_declaration( self ) -> str:
		match self.methodType:
			case method_type.method:
				str = pass_type_string[self.passType] + ' get('
			case method_type.operator_cast:
				str = 'operator ' + pass_type_string[self.passType] + '('
			case method_type.operator_ptr:
				str = pass_type_string[self.passType] + ' operator -> ('
		if self.statusUpdate:
			str += ' status &result '
		str += ') const'
		if self.noexcept:
			str += ' noexcept'
		return str

	def generate_body( self, out ):
		match self.storageType:
			case storage_type.value:
				if self.statusUpdate:
					out.ln('result = status::ok;')
				match self.passType:
					case pass_type.cpy | pass_type.cref:
						out.ln('return this->value;')
					case _:
						raise Exception("Invalid combo of values")
	
			case storage_type.ptr:
				if self.statusUpdate:
					out.ln('if( !(this->value) )')
					with out.blk():
						out.ln('result = status::invalid;')
						out.ln(f'return nil_object::ref<{pass_type_string[self.passType]}>();')
					out.ln('result = status::ok;')
				elif not self.noexcept:
					out.ln('if( !(this->value) )')
					with out.blk():
						out.ln('throw ctle::status_error( status::invalid );')
				match self.passType:
					case pass_type.cpy | pass_type.cref:
						out.ln('return *(this->value.get());')
					case pass_type.cptr:
						out.ln('return this->value.get();')
      
			case storage_type.atomic:
				if self.statusUpdate:
					out.ln('result = status::ok;')
				match self.passType:
					case pass_type.cpy | pass_type.cref:
						out.ln('return this->value.load();')
					case _:
						raise Exception("Invalid combo of values")
	      
			case storage_type.custom:
				if self.statusUpdate:
					out.ln('result = status::ok; // default to ok, let method update')
				else:
					out.ln('status result = status::ok; // default to ok, let method update')
				out.ln( pass_type_string[self.passType] + ' ret_val = this->get_method(this, result);')
				if not self.noexcept:
					out.ln('if( result != status::ok )')
					with out.blk():
						out.ln('throw ctle::status_error( result );')
				out.ln('return ret_val;')

	def generate_method( self, out ):
		self.generate_description( out )
		out.ln( self.get_declaration() )
		with out.blk():
			self.generate_body( out )

class set_method:
	def __init__( self, methodType:method_type, passType:pass_type, statusUpdate:bool, noexcept:bool ):
		self.methodType = methodType
		self.passType = passType
		self.statusUpdate = statusUpdate
		self.noexcept = noexcept
  
	def generate_description( self, out ):
		match self.methodType:
			case method_type.method:
				out.comment_ln(f'property set method which sets the value using a {pass_type_string[self.passType]}')
			case method_type.operator_assign:
				out.comment_ln(f'pointer operator which assigns the property to a {pass_type_string[self.passType]}')
			case _:
				raise Exception("Invalid method type")    
		if self.statusUpdate:
			out.comment_ln('- note: sets the result of the call in the result status parameter')
		if self.noexcept:
			out.comment_ln('- note: does not raise any exceptions')
		else:
			out.comment_ln('- note: raises a status_exception if the call fails')
  
	def generate_declaration( self, out, className:str ):
		match self.methodType:
			case method_type.method:
				if self.statusUpdate:
					str = 'status'       
				else:
					str = 'void'
				str += ' set('
			case method_type.operator_assign:
				str = 'const ' + className + ' & operator = ('
			case _:
				raise Exception("Invalid method type")
		str += ' ' + pass_type_string[self.passType] + ' _value )'
		if self.noexcept:
			str += ' noexcept'
		out.ln( str )

	def generate_body( self, out ):
		match self.storageType:
			case storage_type.value:
				match self.passType:
					case pass_type.cpy | pass_type.cref:
						out.ln('this->value = _value;')
					case _:
						raise Exception("Invalid combo of values")
	
			case storage_type.ptr:
				match self.passType:
					case pass_type.cpy | pass_type.cref:
						out.ln('if( this->value )')
						with out.blk():
							out.ln('*(this->value.get()) = _value; // in-place replace value')
						out.ln('else')
						with out.blk():
							out.ln('this->value = std::unique_ptr<_Ty>( new _Ty(_value) ); // create new object, copy ctor')
					case pass_type.cptr:
						out.ln('if( _value )')
						with out.blk():					
							out.ln('if( this->value )')
							with out.blk():
								out.ln('*(this->value.get()) = *_value; // in-place replace value')
							out.ln('else')
							with out.blk():
								out.ln('this->value = std::unique_ptr<_Ty>( new _Ty(*_value) ); // create new object, copy ctor')      
						out.ln('else')
						with out.blk():
							out.ln('this->value.reset(); // remove value')
      
			case storage_type.atomic:
				match self.passType:
					case pass_type.cpy | pass_type.cref:
						out.ln('this->value.store( _value );')
					case _:
						raise Exception("Invalid combo of values")
      
			case storage_type.custom:
				if self.statusUpdate or not self.noexcept:
					out.ln('status result = this->set_method(this, _value);')
				else:
					out.ln('this->set_method(this, _value);')
				if not self.noexcept:
					out.ln('if( result != status::ok )')
					with out.blk():
						out.ln('throw ctle::status_error( result );')
	
		if self.statusUpdate:
			if self.storageType == storage_type.custom:
				out.ln('return result;')
			else:
				out.ln('return status::ok;')
		elif self.methodType == method_type.operator_assign:
			out.ln('return *this;')          

	def generate_method( self, out, className:str ):
		self.generate_description( out )
		self.generate_declaration( out, className )
		with out.blk():
			self.generate_body( out )

class class_def:
	def __init__( self, storageType:storage_type, mainPassType:pass_type, getMethods:list, setMethods:list ):
		self.storageType = storageType
		self.mainPassType = mainPassType
		self.getMethods = getMethods
		if self.getMethods:
			for gm in self.getMethods:
				gm.storageType = storageType
		self.setMethods = setMethods
		if self.setMethods:
			for gm in self.setMethods:
				gm.storageType = storageType

	def get_name( self, includeParameters:bool = False ):
		str = 'prop'
		if self.getMethods:
			str += '_get'
		if self.setMethods:
			str += '_set'
		if includeParameters:
			str += '<_Ty,prop::'
			if self.getMethods:
				str += self.mainPassType.name + '_'
			str += self.storageType.name + '>'
		return str

	def generate_declaration( self, out ):
		name = self.get_name( includeParameters = True )
		out.ln('template<class _Ty>')
		out.ln(f'class {name}')

	def generate_class( self, out ):
		name = self.get_name()
		self.generate_declaration( out )
		with out.blk( add_semicolon=True ):
			with out.tab('public:'):
				# standard definitions
				out.ln('using value_type = _Ty;')
				if self.storageType == storage_type.custom:
					if self.getMethods:
						out.ln(f'using _get_type = const std::function<{pass_type_string[self.mainPassType]} (const {name} *, status & )>;')
					if self.setMethods:
						out.ln(f'using _set_type = const std::function<status ({name} *, const _Ty &)>;')
				out.ln()
    
				# generate the ctor
				out.comment_ln('property standard ctor')
				out.ln( name + '(' )
				match self.storageType:
					case storage_type.value:
						out.ln( ' _Ty &_value ) : value(_value)', append = True )
						with out.blk():
							out.comment_ln('set a default value IF the value type does not automatically set a default value')
							out.ln('identity_assign_if_trivially_default_constructible<_Ty>( _value );')
						out.ln()

						# add a second ctor with default value assignment
						out.comment_ln('property standard ctor with specific assignment of default value')
						out.ln( name + '( _Ty &_value, const _Ty &_initial_value ) : value(_value)' )
						with out.blk():
							out.comment_ln('set an initial value to the object')
							out.ln('this->value = _initial_value;')

					case storage_type.ptr:
						out.ln( ' std::unique_ptr<_Ty> &_value ) : value(_value) {}', append = True )
					case storage_type.atomic:
						out.ln( ' std::atomic<_Ty> &_value ) : value(_value) {}', append = True )
					case storage_type.custom:
						if self.getMethods:
							out.ln( ' _get_type _get', append = True )
						if self.setMethods:
							if self.getMethods:
								out.ln(',', append = True )
							out.ln( ' _set_type _set', append = True )
						out.ln( ' ) :', append = True )
						if self.getMethods:
							out.ln( ' get_method(_get)', append = True )
						if self.setMethods:
							if self.getMethods:
								out.ln(',', append = True )
							out.ln( ' set_method(_set)', append = True )
						out.ln( ' {}', append = True )
				out.ln()
    
				# define get and set methods
				if self.getMethods:
					for m in self.getMethods:
						m.generate_method( out )
						out.ln()
				if self.setMethods:
					for m in self.setMethods:
						m.generate_method( out, name )
						out.ln()
      
			with out.tab('private:'):
				out.ln(f'{name}() = delete;')
				out.ln(f'{name}( const {name} & ) = delete;')
				out.ln(f'const {name} & operator= ( const {name} & ) = delete;')
				out.ln(f'{name}( {name} && ) = delete;')
				out.ln(f'const {name} & operator= ( {name} && ) = delete;')
				out.ln()
				match self.storageType:
					case storage_type.value:
						out.ln( '_Ty &value;' )
					case storage_type.ptr:
						out.ln( 'std::unique_ptr<_Ty> &value;' )
					case storage_type.atomic:
						out.ln( 'std::atomic<_Ty> &value;' )
					case storage_type.custom:
						if self.getMethods:
							out.ln( '_get_type get_method;' )
						if self.setMethods:
							out.ln( '_set_type set_method;' )



def get_value_and_atomic_property_types():
	types = []
	for storagetype in [storage_type.value, storage_type.atomic]:
		for gettype in [pass_type.cpy, pass_type.cref, None]:
			for settype in [False, True]:
				if gettype != None:
					gm = [
						get_method( method_type.method, gettype, True, True ),
						get_method( method_type.method, gettype, False, True ),
						get_method( method_type.operator_cast, gettype, False, True ),
					]
				else:
					gm = None
		
				if settype:
					sm = [
						set_method( method_type.method, pass_type.cref, True, True ),
						set_method( method_type.operator_assign, pass_type.cref, False, True ),
					]
				else:
					sm = None
		
				if sm or gm:
					types.append( class_def( storagetype, gettype, gm, sm ) )
	return types

def get_ptr_property_types():
	types = []
	for gettype in [pass_type.cpy, pass_type.cref, pass_type.cptr, None]:
		for settype in [False, True]:
			if gettype != None:
				if gettype == pass_type.cptr:
					gm = [
						get_method( method_type.method, gettype, True, True ),
						get_method( method_type.method, gettype, False, True ),
						get_method( method_type.operator_cast, gettype, False, True ),
						get_method( method_type.operator_ptr, gettype, False, True ),					
					]
				else:
					gm = [
						get_method( method_type.method, gettype, True, True ),
						get_method( method_type.method, gettype, False, False ),
						get_method( method_type.operator_cast, gettype, False, False ),
					]
			else:
				gm = None
    
			if settype:
				sm = [
					set_method( method_type.method, pass_type.cref, True, True ),
					set_method( method_type.method, pass_type.cptr, True, True ),
					set_method( method_type.operator_assign, pass_type.cref, False, True ),
					set_method( method_type.operator_assign, pass_type.cptr, False, True ),     
				]
			else:
				sm = None
    
			if sm or gm:
				types.append( class_def( storage_type.ptr, gettype, gm, sm ) )
	return types

def get_custom_property_types():
	types = []
	for gettype in [pass_type.cpy, pass_type.cref, pass_type.cptr, None]:
		for settype in [False, True]:
			if gettype != None:
				gm = [
					get_method( method_type.method, gettype, True, True ),
					get_method( method_type.method, gettype, False, False ),
					get_method( method_type.operator_cast, gettype, False, False ),
				]
			else:
				gm = None
	
			if settype:
				sm = [
					set_method( method_type.method, pass_type.cref, True, True ),
					set_method( method_type.operator_assign, pass_type.cref, False, False ),
				]
			else:
				sm = None
	
			if sm or gm:
				types.append( class_def( storage_type.custom, gettype, gm, sm ) )
	return types

def generate_property( path:str ):

	out = formatted_output()

	out.generate_license_header()
	guard_def = out.begin_header_guard( 'prop.h', 'ctle' )
	out.ln()
	out.ln('#include <memory>')
	out.ln('#include <atomic>')
	out.ln('#include <functional>')
	out.ln()
	out.ln('#include "status.h"')
	out.ln('#include "util.h"')
	out.ln()

	props = []
	props.extend( get_value_and_atomic_property_types() )
	props.extend( get_ptr_property_types() )
	props.extend( get_custom_property_types() )

	out.ln('namespace ctle')
	with out.blk():
		out.ln( '''    // The prop_[...] template classes are a convenient way to implement properties in classes, where each property can be accessed as a 
    // normal variable, but can also be made read-only, write-only, read/write, and let the owner class override if a value is returned 
    // from a variable, or evaluated on-the-fly, etc.     
         
	class prop
	{
	public:
		// types of property specializations
		enum class type
		{
			cpy_value,		// return a copy, direct stored value
			cref_value,		// return a const ref, direct stored value
			cpy_atomic,		// return a copy, atomic-access value
			cref_atomic,	// return a const ref, atomic-access value
			cpy_ptr,		// return a copy, value stored as a unique ptr
			cref_ptr,		// return a const ref, value stored as a unique ptr
			cptr_ptr,		// return a const pointer, value stored as a unique ptr
			cpy_custom,		// return a copy, custom value handled by the owner 
			cref_custom,	// return a const ref, custom value handled by the owner 
			cptr_custom,	// return a const pointer, custom value handled by the owner 
		};
		
		// const aliases, to directly use enum value
		static constexpr const type cpy_value = type::cpy_value;		// return a copy, direct stored value
		static constexpr const type cref_value = type::cref_value;		// return a const ref, direct stored value
		static constexpr const type cpy_atomic = type::cpy_atomic;		// return a copy, atomic-access value
		static constexpr const type cref_atomic = type::cref_atomic;	// return a const ref, atomic-access value
		static constexpr const type cpy_ptr = type::cpy_ptr;			// return a copy, value stored as a unique ptr
		static constexpr const type cref_ptr = type::cref_ptr;			// return a const ref, value stored as a unique ptr
		static constexpr const type cptr_ptr = type::cptr_ptr;			// return a const pointer, value stored as a unique ptr
		static constexpr const type cpy_custom = type::cpy_custom;		// return a copy, custom value handled by the owner 
		static constexpr const type cref_custom = type::cref_custom;	// return a const ref, custom value handled by the owner 
		static constexpr const type cptr_custom = type::cptr_custom;	// return a const pointer, custom value handled by the owner 

		// default types for when return type is not defined. defaults to const ref'd, and should be used as parameter by all set-only templates. 
		static constexpr const type value = type::cref_value;
		static constexpr const type atomic = type::cref_atomic;
		static constexpr const type ptr = type::cref_ptr;
		static constexpr const type custom = type::cref_custom;
	};

	// prop_[...] base templates, get, get/set or set types. defaults to const ref'd direct values if no _type parameter is specified
	template<class _Ty, prop::type _type=prop::value> class prop_get;
	template<class _Ty, prop::type _type=prop::value> class prop_get_set;
	template<class _Ty, prop::type _type=prop::value> class prop_set;
''')         

		out.comment_ln('prop_[...] template specializations follow below')
		out.ln()
		for v in props:
			v.generate_class( out )
			out.ln()
			
	out.ln()
	out.end_header_guard( guard_def )
	out.write_lines_to_file( path )
