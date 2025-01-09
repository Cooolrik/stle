// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

// we assume column major order for ctle MxN tuples, which matches glm
#define CTLE_MN_TUP_COLUMNMAJOR

#include <glm/glm.hpp>
#include <type_traits>

#include "../ctle/types.h"
#include "../ctle/ntup.h"

#include "unit_tests.h"

using namespace ctle;

//static_assert(i8tup2x4::dimensions == 2);
//static_assert(i8tup2x4::value_type::dimensions == 4);
//static_assert(i8tup2x4::num_rows == 2);
//static_assert(i8tup2x4::num_columns == 4);

template<class _Ty, size_t _Size> void CompareExact(const n_tup<_Ty, _Size>& tuple, const n_tup<_Ty, _Size>& tuple2)
{
	// make sure they match exactly
	for( size_t i = 0; i < _Size; ++i )
	{
		EXPECT_EQ( tuple[i], tuple2[i] );
	}

	EXPECT_EQ( tuple, tuple2 );
	EXPECT_FALSE( tuple < tuple2 );
	EXPECT_FALSE( tuple > tuple2 );
}

template<class _Ty, size_t _Size> void CompareNear( const n_tup<_Ty, _Size> &tuple, const n_tup<_Ty, _Size> &tuple2 )
{
	// make sure they are near
	for( size_t i = 0; i < _Size; ++i )
	{
		EXPECT_NEAR( (double)tuple[i], (double)tuple2[i], 0.00001 );
	}
}

template<class _Ty, size_t _Size, bool _Exact> void TestTuple()
{
	n_tup<_Ty, _Size> tuple = {};

	static_assert( sizeof(tuple) == sizeof(_Ty)*_Size );

	// init with random values
	for( size_t i = 0; i < _Size; ++i )
	{
		tuple[i] = random_value<_Ty>();
	}

	// convert to string
	const std::string str = to_string( tuple );

	// read back from string
	auto tuple2 = from_string<n_tup<_Ty, _Size>>( str );

	if( _Exact )
	{
		CompareExact<_Ty,_Size>( tuple, tuple2 );
	}
	else
	{
		CompareNear<_Ty,_Size>( tuple, tuple2 );
	}
}

template<class _Ty, bool _Exact> void TestNTuple()
{
	TestTuple<_Ty, 1, _Exact>();
	TestTuple<_Ty, 2, _Exact>();
	TestTuple<_Ty, 3, _Exact>();
	TestTuple<_Ty, 4, _Exact>();
}

TEST( types, basic_test )
{
	TestNTuple<i8, true>();
	TestNTuple<u8, true>();
	TestNTuple<i16, true>();
	TestNTuple<u16, true>();
	TestNTuple<i32, true>();
	TestNTuple<u32, true>();
	TestNTuple<i64, true>();
	TestNTuple<u64, true>();	
	TestNTuple<float, false>();
	TestNTuple<double, false>();
}

template<class _Ty, size_t _InnerSize, size_t _OuterSize, bool _Exact> void TestTupleOfTuples()
{
	mn_tup<_Ty, _InnerSize, _OuterSize> tupletuple = {};

	static_assert( sizeof(tupletuple) == sizeof(_Ty) * _InnerSize * _OuterSize );

	// init with random values
	for( size_t i = 0; i < _OuterSize; ++i )
	{
		tupletuple[i] = random_value<n_tup<_Ty,_InnerSize>>();
	}

	// convert to string
	const std::string str = to_string( tupletuple );

	// read back from string
	auto tupletuple2 = from_string<mn_tup<_Ty, _InnerSize, _OuterSize>>( str );

	// make sure they match
	for( size_t i = 0; i < _OuterSize; ++i )
	{
		if( _Exact )
		{
			CompareExact<_Ty,_InnerSize>( tupletuple[i], tupletuple2[i] );
		}
		else
		{
			CompareNear<_Ty,_InnerSize>( tupletuple[i], tupletuple2[i] );
		}
	}

}

template<class _Ty, bool _Exact> void TestMNTuple()
{
	TestTupleOfTuples<_Ty,1,1,_Exact>();
	TestTupleOfTuples<_Ty,1,2,_Exact>();
	TestTupleOfTuples<_Ty,1,3,_Exact>();
	TestTupleOfTuples<_Ty,1,4,_Exact>();
	TestTupleOfTuples<_Ty,2,1,_Exact>();
	TestTupleOfTuples<_Ty,2,2,_Exact>();
	TestTupleOfTuples<_Ty,2,3,_Exact>();
	TestTupleOfTuples<_Ty,2,4,_Exact>();
	TestTupleOfTuples<_Ty,3,1,_Exact>();
	TestTupleOfTuples<_Ty,3,2,_Exact>();
	TestTupleOfTuples<_Ty,3,3,_Exact>();
	TestTupleOfTuples<_Ty,3,4,_Exact>();
	TestTupleOfTuples<_Ty,4,1,_Exact>();
	TestTupleOfTuples<_Ty,4,2,_Exact>();
	TestTupleOfTuples<_Ty,4,3,_Exact>();
	TestTupleOfTuples<_Ty,4,4,_Exact>();	
}

TEST( types, tuple_of_tuple_test )
{
	TestMNTuple<i8, true>();
	TestMNTuple<u8, true>();
	TestMNTuple<i16, true>();
	TestMNTuple<u16, true>();
	TestMNTuple<i32, true>();
	TestMNTuple<u32, true>();
	TestMNTuple<i64, true>();
	TestMNTuple<u64, true>();	
	TestMNTuple<float, false>();
	TestMNTuple<double, false>();
}

// test converting to M x N glm matrix and back
// (_M is the number of rows, _N is the number of columns)
template<class _Ty, size_t _M, size_t _N> void TestMNTupleConvert()
{
	using glm_mtx = typename glm::mat<_N, _M, _Ty>;
	using ctle_tup = typename mn_tup<_Ty, _M, _N>;

	// make sure the glm matrix is the right size
	// (note that the size of a column is the number of rows, and the size of a row is the number of columns)
	static_assert( sizeof(glm_mtx::col_type) / sizeof(_Ty) == _M );
	static_assert( sizeof(glm_mtx::row_type) / sizeof(_Ty) == _N );

	ctle_tup tuptup = {};
	static_assert( sizeof(tuptup) == sizeof(_Ty) * _M * _N );
	static_assert( ctle_tup::num_rows == _M );
	static_assert( ctle_tup::num_columns == _N );

	// init with values 1->M*N, in data order
	_Ty *dest = tuptup.data();
	for (size_t inx = 0; inx < _M * _N; ++inx)
	{
		dest[inx] = (_Ty)(inx + 1);
	}

	// since we assume column major order, read back in column order and make sure it matches
	for( size_t col_inx = 0; col_inx < _N; ++col_inx )
	{
		// get the reference to the column
		auto& coltup = tuptup[col_inx];
		for( size_t row_inx = 0; row_inx < _M; ++row_inx )
		{
			// make sure the value matches for the row index within the column vector
			EXPECT_EQ(coltup[row_inx], (_Ty)(col_inx * _M + row_inx + 1));
		}
	}

	// convert to glm matrix
	glm_mtx glmmtx = tuptup;

	// read back from glm matrix, make sure it matches
	for( size_t col_inx = 0; col_inx < _N; ++col_inx )
	{
		// get the reference to the column vector in the glm matrix
		auto &col = glmmtx[(glm::length_t)col_inx];
		for( size_t row_inx = 0; row_inx < _M; ++row_inx )
		{
			// make sure the value matches for the row index within the column vector
			EXPECT_EQ(col[(glm::length_t)row_inx], (_Ty)(col_inx * _M + row_inx + 1));
		}
	}

	// convert back to ctle tuple
	ctle_tup tuptup2 = glmmtx;

	// make sure the values match
	EXPECT_TRUE( tuptup == tuptup2 );
	EXPECT_TRUE( memcmp( tuptup.data(), tuptup2.data(), sizeof(_Ty) * _M * _N ) == 0 );
}

template<class _Ty> void TestTupleConvert()
{
	TestMNTupleConvert<i8, 2, 2>();
	TestMNTupleConvert<i8, 2, 3>();
	TestMNTupleConvert<i8, 2, 4>();
	TestMNTupleConvert<i8, 3, 2>();
	TestMNTupleConvert<i8, 3, 3>();
	TestMNTupleConvert<i8, 3, 4>();
	TestMNTupleConvert<i8, 4, 2>();
	TestMNTupleConvert<i8, 4, 3>();
	TestMNTupleConvert<i8, 4, 4>();		
}

TEST(types, tuple_glm_convert_test)
{
	TestTupleConvert<i8>();
	TestTupleConvert<u8>();
	TestTupleConvert<i16>();
	TestTupleConvert<u16>();
	TestTupleConvert<i32>();
	TestTupleConvert<u32>();
	TestTupleConvert<i64>();
	TestTupleConvert<u64>();
	TestTupleConvert<f32>();
	TestTupleConvert<f64>();
}
	