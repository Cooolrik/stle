// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_UNIT_TESTS_H_
#define _CTLE_UNIT_TESTS_H_

#include <cstdint>

#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>
#include <vector>

// include external headers
// silence warning we can't control
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4456 )
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#endif

#include <gtest/gtest.h>
#include <picosha2.h>
#include <xxhash.h>
#include <functional>

// re-enable warnings again
#ifdef _MSC_VER
#pragma warning( pop )
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include "../ctle/fwd.h"

// import basic types and tuples
using ctle::i8; 
using ctle::u8; 
using ctle::i16;
using ctle::u16;
using ctle::i32;
using ctle::u32;
using ctle::i64;
using ctle::u64;
using ctle::f32;
using ctle::f64;

template<class T> T random_value();

// if size == 0, returns a random lenght vector, up to 100 values in length
template<class T> inline std::vector<T> random_vector( size_t size = 0 )
{
    if( size == 0 )
        size = (rand() % 100) + 1;
	std::vector<T> data(size);
	for( size_t inx=0; inx<size; ++inx )
		data[inx] = random_value<T>();
    return data;
}

// variant implements a number of any of the regular types, and can be set to a random value, converted to/from string, and compared to another variant 
class variant
{
public:
    virtual ~variant() {};
    virtual size_t type() const = 0;
    virtual const char *type_name() const = 0;
    virtual size_t type_size() const = 0;
    virtual size_t type_count() const = 0;
    virtual bool is_equal( const variant &other ) const = 0;
    virtual void clear() = 0;
    virtual void random() = 0;
    virtual const void *cdata() const = 0;
    virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) = 0;
    virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const = 0;
};

// creates a new variant of a specific type (index)
std::unique_ptr<variant> new_variant( size_t index );
std::unique_ptr<variant> random_variant();

#define STANDARD_TEST_INIT() \
	TEST_METHOD_INITIALIZE( InitMethod )\
		{\
		setup_random_seed();\
		}

// runs a function, measures how long the function took to run, and makes sure it is within the specified max time
// waits for the function to return true within the timeout in milliseconds, else returns false
template <typename Func>
bool run_function_with_timeout(Func func, long long timeout_ms) 
{
    const auto start = std::chrono::high_resolution_clock::now();
    bool result = false;

    while( true )
    {
        result = func();
        if( result )
            return true;

        const auto now = std::chrono::high_resolution_clock::now();
        const auto current_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
        if(current_ms >= timeout_ms) 
            return false;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

#endif//_CTLE_UNIT_TESTS_H_
