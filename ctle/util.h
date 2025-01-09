// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_UTIL_H_
#define _CTLE_UTIL_H_

#include <type_traits>

namespace ctle
{


// identity_assign_if_trivially_default_constructible is a conditional template function which:
//  - Initializes trivially constructable types by using the = {} assignment. 
//  - For all other types, the template is a noop and does nothing. 
// This is to avoid applying the = {} where there is a complex default constructor, with e.g. default parameters.
template<typename _Ty, typename std::enable_if<std::is_trivially_default_constructible<_Ty>{},bool>::type = true> void identity_assign_if_trivially_default_constructible( _Ty &val ) { val = {}; }
template<typename _Ty, typename std::enable_if<!std::is_trivially_default_constructible<_Ty>{},bool>::type = true> void identity_assign_if_trivially_default_constructible( _Ty & ) { /*noop*/ }

// nil_object is a static class with an allocated object, which can be 
// used to point to or reference an invalid object, when nullptr is not applicable or apt
class nil_object
{
private:
	static void *allocation;

public:
	// get a nil ptr and compare ptr to nil ptr
	static void *ptr() noexcept { return allocation; }
	template<class _Ty> static bool is_nil( _Ty *ptr ) noexcept { return allocation == ptr; }
	template<class _Ty> static bool is_nil( const _Ty *ptr ) noexcept { return allocation == ptr; }
			 
	// get a nil ref and compare ref to nil ref
	template<class _Ty> static _Ty &ref() noexcept { return *((_Ty*)allocation); }
	template<class _Ty> static bool is_nil( _Ty &ref ) noexcept { return allocation == (void*)&ref; }
	template<class _Ty> static bool is_nil( const _Ty &ref ) noexcept { return allocation == (void*)&ref; }
};

#ifdef CTLE_IMPLEMENTATION
static int64_t nil_object_mem;
void *nil_object::allocation = &nil_object_mem;
#endif//CTLE_IMPLEMENTATION


}
//namespace ctle
#endif//_CTLE_UTIL_H_
