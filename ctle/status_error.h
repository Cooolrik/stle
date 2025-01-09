// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_STATUS_ERROR_H_
#define _CTLE_STATUS_ERROR_H_

/// @file status_error.h
/// @brief Contains the status_error class, a runtime_error exception class for status values.

#include "status.h"
#include <stdexcept>

namespace ctle
{
	/// @brief status_error exception class, derived from std::runtime_error.
	/// @details This exception class implements a runtime_error exception, with a status value attached.
	class status_error : public std::runtime_error	
	{
		public:
			status value;
			explicit status_error( status _value, char const* const _Message = "" ) noexcept : std::runtime_error(_Message), value(_value) {}
	};				  
}
// namespace ctle
#endif//_CTLE_STATUS_ERROR_H_
