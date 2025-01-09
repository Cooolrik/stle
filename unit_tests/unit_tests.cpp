// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#define CTLE_IMPLEMENTATION

#include "unit_tests.h"

#include "../ctle/ctle.h"

// just some value
const i64 global_random_seed = 12876123876;

inline void setup_random_seed()
{
	i64 seed = (global_random_seed == -1) ? i64( time( nullptr ) ) : global_random_seed;
	srand(u32( seed & 0xffffffff ));
}

// add headers that you want to pre-compile here
inline u8 u8_rand() { return (u8)(rand() & 0xff); } 
inline u16 u16_rand() { return u16(rand() << 4) ^ u16(rand()); } 
inline u32 u32_rand() { return u32(u16_rand()) << 16 | u32(u16_rand()); } 
inline u64 u64_rand() { return u64(u32_rand()) << 32 | u64(u32_rand()); } 

template<> bool random_value<bool>() { return (bool)(rand() & 0x1); } 

template<> i8 random_value<i8>() { return u8_rand(); }
template<> i16 random_value<i16>() { return u16_rand(); }
template<> i32 random_value<i32>() { return u32_rand(); }
template<> i64 random_value<i64>() { return u64_rand(); }

template<> u8 random_value<u8>() { return u8_rand(); }
template<> u16 random_value<u16>() { return u16_rand(); }
template<> u32 random_value<u32>() { return u32_rand(); }
template<> u64 random_value<u64>() { return u64_rand(); }

template<> double random_value<double>() { return (double)((i64)u64_rand()) / ((double)u32_rand() + (double)0xffffffff); }
template<> float random_value<float>() { return (float)u64_rand() ; }

template<class _Ty, size_t _Size> using n_tup = ctle::n_tup<_Ty,_Size>;
template<class _Ty, size_t _InnerSize, size_t _OuterSize> using mn_tup = ctle::mn_tup<_Ty,_InnerSize,_OuterSize>;

template<> n_tup<i8,1> random_value<n_tup<i8,1>>() { return n_tup<i8,1>(random_value<i8>()); }
template<> n_tup<i8,2> random_value<n_tup<i8,2>>() { return n_tup<i8,2>(random_value<i8>(), random_value<i8>()); }
template<> n_tup<i8,3> random_value<n_tup<i8,3>>() { return n_tup<i8,3>(random_value<i8>(), random_value<i8>(), random_value<i8>()); }
template<> n_tup<i8,4> random_value<n_tup<i8,4>>() { return n_tup<i8,4>(random_value<i8>(), random_value<i8>(), random_value<i8>(), random_value<i8>()); }
template<> mn_tup<i8,1,1> random_value<mn_tup<i8,1,1>>() { return mn_tup<i8,1,1>(random_value<n_tup<i8,1>>()); }
template<> mn_tup<i8,1,2> random_value<mn_tup<i8,1,2>>() { return mn_tup<i8,1,2>(random_value<n_tup<i8,1>>(), random_value<n_tup<i8,1>>()); }
template<> mn_tup<i8,1,3> random_value<mn_tup<i8,1,3>>() { return mn_tup<i8,1,3>(random_value<n_tup<i8,1>>(), random_value<n_tup<i8,1>>(), random_value<n_tup<i8,1>>()); }
template<> mn_tup<i8,1,4> random_value<mn_tup<i8,1,4>>() { return mn_tup<i8,1,4>(random_value<n_tup<i8,1>>(), random_value<n_tup<i8,1>>(), random_value<n_tup<i8,1>>(), random_value<n_tup<i8,1>>()); }
template<> mn_tup<i8,2,1> random_value<mn_tup<i8,2,1>>() { return mn_tup<i8,2,1>(random_value<n_tup<i8,2>>()); }
template<> mn_tup<i8,2,2> random_value<mn_tup<i8,2,2>>() { return mn_tup<i8,2,2>(random_value<n_tup<i8,2>>(), random_value<n_tup<i8,2>>()); }
template<> mn_tup<i8,2,3> random_value<mn_tup<i8,2,3>>() { return mn_tup<i8,2,3>(random_value<n_tup<i8,2>>(), random_value<n_tup<i8,2>>(), random_value<n_tup<i8,2>>()); }
template<> mn_tup<i8,2,4> random_value<mn_tup<i8,2,4>>() { return mn_tup<i8,2,4>(random_value<n_tup<i8,2>>(), random_value<n_tup<i8,2>>(), random_value<n_tup<i8,2>>(), random_value<n_tup<i8,2>>()); }
template<> mn_tup<i8,3,1> random_value<mn_tup<i8,3,1>>() { return mn_tup<i8,3,1>(random_value<n_tup<i8,3>>()); }
template<> mn_tup<i8,3,2> random_value<mn_tup<i8,3,2>>() { return mn_tup<i8,3,2>(random_value<n_tup<i8,3>>(), random_value<n_tup<i8,3>>()); }
template<> mn_tup<i8,3,3> random_value<mn_tup<i8,3,3>>() { return mn_tup<i8,3,3>(random_value<n_tup<i8,3>>(), random_value<n_tup<i8,3>>(), random_value<n_tup<i8,3>>()); }
template<> mn_tup<i8,3,4> random_value<mn_tup<i8,3,4>>() { return mn_tup<i8,3,4>(random_value<n_tup<i8,3>>(), random_value<n_tup<i8,3>>(), random_value<n_tup<i8,3>>(), random_value<n_tup<i8,3>>()); }
template<> mn_tup<i8,4,1> random_value<mn_tup<i8,4,1>>() { return mn_tup<i8,4,1>(random_value<n_tup<i8,4>>()); }
template<> mn_tup<i8,4,2> random_value<mn_tup<i8,4,2>>() { return mn_tup<i8,4,2>(random_value<n_tup<i8,4>>(), random_value<n_tup<i8,4>>()); }
template<> mn_tup<i8,4,3> random_value<mn_tup<i8,4,3>>() { return mn_tup<i8,4,3>(random_value<n_tup<i8,4>>(), random_value<n_tup<i8,4>>(), random_value<n_tup<i8,4>>()); }
template<> mn_tup<i8,4,4> random_value<mn_tup<i8,4,4>>() { return mn_tup<i8,4,4>(random_value<n_tup<i8,4>>(), random_value<n_tup<i8,4>>(), random_value<n_tup<i8,4>>(), random_value<n_tup<i8,4>>()); }
template<> n_tup<u8,1> random_value<n_tup<u8,1>>() { return n_tup<u8,1>(random_value<u8>()); }
template<> n_tup<u8,2> random_value<n_tup<u8,2>>() { return n_tup<u8,2>(random_value<u8>(), random_value<u8>()); }
template<> n_tup<u8,3> random_value<n_tup<u8,3>>() { return n_tup<u8,3>(random_value<u8>(), random_value<u8>(), random_value<u8>()); }
template<> n_tup<u8,4> random_value<n_tup<u8,4>>() { return n_tup<u8,4>(random_value<u8>(), random_value<u8>(), random_value<u8>(), random_value<u8>()); }
template<> mn_tup<u8,1,1> random_value<mn_tup<u8,1,1>>() { return mn_tup<u8,1,1>(random_value<n_tup<u8,1>>()); }
template<> mn_tup<u8,1,2> random_value<mn_tup<u8,1,2>>() { return mn_tup<u8,1,2>(random_value<n_tup<u8,1>>(), random_value<n_tup<u8,1>>()); }
template<> mn_tup<u8,1,3> random_value<mn_tup<u8,1,3>>() { return mn_tup<u8,1,3>(random_value<n_tup<u8,1>>(), random_value<n_tup<u8,1>>(), random_value<n_tup<u8,1>>()); }
template<> mn_tup<u8,1,4> random_value<mn_tup<u8,1,4>>() { return mn_tup<u8,1,4>(random_value<n_tup<u8,1>>(), random_value<n_tup<u8,1>>(), random_value<n_tup<u8,1>>(), random_value<n_tup<u8,1>>()); }
template<> mn_tup<u8,2,1> random_value<mn_tup<u8,2,1>>() { return mn_tup<u8,2,1>(random_value<n_tup<u8,2>>()); }
template<> mn_tup<u8,2,2> random_value<mn_tup<u8,2,2>>() { return mn_tup<u8,2,2>(random_value<n_tup<u8,2>>(), random_value<n_tup<u8,2>>()); }
template<> mn_tup<u8,2,3> random_value<mn_tup<u8,2,3>>() { return mn_tup<u8,2,3>(random_value<n_tup<u8,2>>(), random_value<n_tup<u8,2>>(), random_value<n_tup<u8,2>>()); }
template<> mn_tup<u8,2,4> random_value<mn_tup<u8,2,4>>() { return mn_tup<u8,2,4>(random_value<n_tup<u8,2>>(), random_value<n_tup<u8,2>>(), random_value<n_tup<u8,2>>(), random_value<n_tup<u8,2>>()); }
template<> mn_tup<u8,3,1> random_value<mn_tup<u8,3,1>>() { return mn_tup<u8,3,1>(random_value<n_tup<u8,3>>()); }
template<> mn_tup<u8,3,2> random_value<mn_tup<u8,3,2>>() { return mn_tup<u8,3,2>(random_value<n_tup<u8,3>>(), random_value<n_tup<u8,3>>()); }
template<> mn_tup<u8,3,3> random_value<mn_tup<u8,3,3>>() { return mn_tup<u8,3,3>(random_value<n_tup<u8,3>>(), random_value<n_tup<u8,3>>(), random_value<n_tup<u8,3>>()); }
template<> mn_tup<u8,3,4> random_value<mn_tup<u8,3,4>>() { return mn_tup<u8,3,4>(random_value<n_tup<u8,3>>(), random_value<n_tup<u8,3>>(), random_value<n_tup<u8,3>>(), random_value<n_tup<u8,3>>()); }
template<> mn_tup<u8,4,1> random_value<mn_tup<u8,4,1>>() { return mn_tup<u8,4,1>(random_value<n_tup<u8,4>>()); }
template<> mn_tup<u8,4,2> random_value<mn_tup<u8,4,2>>() { return mn_tup<u8,4,2>(random_value<n_tup<u8,4>>(), random_value<n_tup<u8,4>>()); }
template<> mn_tup<u8,4,3> random_value<mn_tup<u8,4,3>>() { return mn_tup<u8,4,3>(random_value<n_tup<u8,4>>(), random_value<n_tup<u8,4>>(), random_value<n_tup<u8,4>>()); }
template<> mn_tup<u8,4,4> random_value<mn_tup<u8,4,4>>() { return mn_tup<u8,4,4>(random_value<n_tup<u8,4>>(), random_value<n_tup<u8,4>>(), random_value<n_tup<u8,4>>(), random_value<n_tup<u8,4>>()); }
template<> n_tup<i16,1> random_value<n_tup<i16,1>>() { return n_tup<i16,1>(random_value<i16>()); }
template<> n_tup<i16,2> random_value<n_tup<i16,2>>() { return n_tup<i16,2>(random_value<i16>(), random_value<i16>()); }
template<> n_tup<i16,3> random_value<n_tup<i16,3>>() { return n_tup<i16,3>(random_value<i16>(), random_value<i16>(), random_value<i16>()); }
template<> n_tup<i16,4> random_value<n_tup<i16,4>>() { return n_tup<i16,4>(random_value<i16>(), random_value<i16>(), random_value<i16>(), random_value<i16>()); }
template<> mn_tup<i16,1,1> random_value<mn_tup<i16,1,1>>() { return mn_tup<i16,1,1>(random_value<n_tup<i16,1>>()); }
template<> mn_tup<i16,1,2> random_value<mn_tup<i16,1,2>>() { return mn_tup<i16,1,2>(random_value<n_tup<i16,1>>(), random_value<n_tup<i16,1>>()); }
template<> mn_tup<i16,1,3> random_value<mn_tup<i16,1,3>>() { return mn_tup<i16,1,3>(random_value<n_tup<i16,1>>(), random_value<n_tup<i16,1>>(), random_value<n_tup<i16,1>>()); }
template<> mn_tup<i16,1,4> random_value<mn_tup<i16,1,4>>() { return mn_tup<i16,1,4>(random_value<n_tup<i16,1>>(), random_value<n_tup<i16,1>>(), random_value<n_tup<i16,1>>(), random_value<n_tup<i16,1>>()); }
template<> mn_tup<i16,2,1> random_value<mn_tup<i16,2,1>>() { return mn_tup<i16,2,1>(random_value<n_tup<i16,2>>()); }
template<> mn_tup<i16,2,2> random_value<mn_tup<i16,2,2>>() { return mn_tup<i16,2,2>(random_value<n_tup<i16,2>>(), random_value<n_tup<i16,2>>()); }
template<> mn_tup<i16,2,3> random_value<mn_tup<i16,2,3>>() { return mn_tup<i16,2,3>(random_value<n_tup<i16,2>>(), random_value<n_tup<i16,2>>(), random_value<n_tup<i16,2>>()); }
template<> mn_tup<i16,2,4> random_value<mn_tup<i16,2,4>>() { return mn_tup<i16,2,4>(random_value<n_tup<i16,2>>(), random_value<n_tup<i16,2>>(), random_value<n_tup<i16,2>>(), random_value<n_tup<i16,2>>()); }
template<> mn_tup<i16,3,1> random_value<mn_tup<i16,3,1>>() { return mn_tup<i16,3,1>(random_value<n_tup<i16,3>>()); }
template<> mn_tup<i16,3,2> random_value<mn_tup<i16,3,2>>() { return mn_tup<i16,3,2>(random_value<n_tup<i16,3>>(), random_value<n_tup<i16,3>>()); }
template<> mn_tup<i16,3,3> random_value<mn_tup<i16,3,3>>() { return mn_tup<i16,3,3>(random_value<n_tup<i16,3>>(), random_value<n_tup<i16,3>>(), random_value<n_tup<i16,3>>()); }
template<> mn_tup<i16,3,4> random_value<mn_tup<i16,3,4>>() { return mn_tup<i16,3,4>(random_value<n_tup<i16,3>>(), random_value<n_tup<i16,3>>(), random_value<n_tup<i16,3>>(), random_value<n_tup<i16,3>>()); }
template<> mn_tup<i16,4,1> random_value<mn_tup<i16,4,1>>() { return mn_tup<i16,4,1>(random_value<n_tup<i16,4>>()); }
template<> mn_tup<i16,4,2> random_value<mn_tup<i16,4,2>>() { return mn_tup<i16,4,2>(random_value<n_tup<i16,4>>(), random_value<n_tup<i16,4>>()); }
template<> mn_tup<i16,4,3> random_value<mn_tup<i16,4,3>>() { return mn_tup<i16,4,3>(random_value<n_tup<i16,4>>(), random_value<n_tup<i16,4>>(), random_value<n_tup<i16,4>>()); }
template<> mn_tup<i16,4,4> random_value<mn_tup<i16,4,4>>() { return mn_tup<i16,4,4>(random_value<n_tup<i16,4>>(), random_value<n_tup<i16,4>>(), random_value<n_tup<i16,4>>(), random_value<n_tup<i16,4>>()); }
template<> n_tup<u16,1> random_value<n_tup<u16,1>>() { return n_tup<u16,1>(random_value<u16>()); }
template<> n_tup<u16,2> random_value<n_tup<u16,2>>() { return n_tup<u16,2>(random_value<u16>(), random_value<u16>()); }
template<> n_tup<u16,3> random_value<n_tup<u16,3>>() { return n_tup<u16,3>(random_value<u16>(), random_value<u16>(), random_value<u16>()); }
template<> n_tup<u16,4> random_value<n_tup<u16,4>>() { return n_tup<u16,4>(random_value<u16>(), random_value<u16>(), random_value<u16>(), random_value<u16>()); }
template<> mn_tup<u16,1,1> random_value<mn_tup<u16,1,1>>() { return mn_tup<u16,1,1>(random_value<n_tup<u16,1>>()); }
template<> mn_tup<u16,1,2> random_value<mn_tup<u16,1,2>>() { return mn_tup<u16,1,2>(random_value<n_tup<u16,1>>(), random_value<n_tup<u16,1>>()); }
template<> mn_tup<u16,1,3> random_value<mn_tup<u16,1,3>>() { return mn_tup<u16,1,3>(random_value<n_tup<u16,1>>(), random_value<n_tup<u16,1>>(), random_value<n_tup<u16,1>>()); }
template<> mn_tup<u16,1,4> random_value<mn_tup<u16,1,4>>() { return mn_tup<u16,1,4>(random_value<n_tup<u16,1>>(), random_value<n_tup<u16,1>>(), random_value<n_tup<u16,1>>(), random_value<n_tup<u16,1>>()); }
template<> mn_tup<u16,2,1> random_value<mn_tup<u16,2,1>>() { return mn_tup<u16,2,1>(random_value<n_tup<u16,2>>()); }
template<> mn_tup<u16,2,2> random_value<mn_tup<u16,2,2>>() { return mn_tup<u16,2,2>(random_value<n_tup<u16,2>>(), random_value<n_tup<u16,2>>()); }
template<> mn_tup<u16,2,3> random_value<mn_tup<u16,2,3>>() { return mn_tup<u16,2,3>(random_value<n_tup<u16,2>>(), random_value<n_tup<u16,2>>(), random_value<n_tup<u16,2>>()); }
template<> mn_tup<u16,2,4> random_value<mn_tup<u16,2,4>>() { return mn_tup<u16,2,4>(random_value<n_tup<u16,2>>(), random_value<n_tup<u16,2>>(), random_value<n_tup<u16,2>>(), random_value<n_tup<u16,2>>()); }
template<> mn_tup<u16,3,1> random_value<mn_tup<u16,3,1>>() { return mn_tup<u16,3,1>(random_value<n_tup<u16,3>>()); }
template<> mn_tup<u16,3,2> random_value<mn_tup<u16,3,2>>() { return mn_tup<u16,3,2>(random_value<n_tup<u16,3>>(), random_value<n_tup<u16,3>>()); }
template<> mn_tup<u16,3,3> random_value<mn_tup<u16,3,3>>() { return mn_tup<u16,3,3>(random_value<n_tup<u16,3>>(), random_value<n_tup<u16,3>>(), random_value<n_tup<u16,3>>()); }
template<> mn_tup<u16,3,4> random_value<mn_tup<u16,3,4>>() { return mn_tup<u16,3,4>(random_value<n_tup<u16,3>>(), random_value<n_tup<u16,3>>(), random_value<n_tup<u16,3>>(), random_value<n_tup<u16,3>>()); }
template<> mn_tup<u16,4,1> random_value<mn_tup<u16,4,1>>() { return mn_tup<u16,4,1>(random_value<n_tup<u16,4>>()); }
template<> mn_tup<u16,4,2> random_value<mn_tup<u16,4,2>>() { return mn_tup<u16,4,2>(random_value<n_tup<u16,4>>(), random_value<n_tup<u16,4>>()); }
template<> mn_tup<u16,4,3> random_value<mn_tup<u16,4,3>>() { return mn_tup<u16,4,3>(random_value<n_tup<u16,4>>(), random_value<n_tup<u16,4>>(), random_value<n_tup<u16,4>>()); }
template<> mn_tup<u16,4,4> random_value<mn_tup<u16,4,4>>() { return mn_tup<u16,4,4>(random_value<n_tup<u16,4>>(), random_value<n_tup<u16,4>>(), random_value<n_tup<u16,4>>(), random_value<n_tup<u16,4>>()); }
template<> n_tup<i32,1> random_value<n_tup<i32,1>>() { return n_tup<i32,1>(random_value<i32>()); }
template<> n_tup<i32,2> random_value<n_tup<i32,2>>() { return n_tup<i32,2>(random_value<i32>(), random_value<i32>()); }
template<> n_tup<i32,3> random_value<n_tup<i32,3>>() { return n_tup<i32,3>(random_value<i32>(), random_value<i32>(), random_value<i32>()); }
template<> n_tup<i32,4> random_value<n_tup<i32,4>>() { return n_tup<i32,4>(random_value<i32>(), random_value<i32>(), random_value<i32>(), random_value<i32>()); }
template<> mn_tup<i32,1,1> random_value<mn_tup<i32,1,1>>() { return mn_tup<i32,1,1>(random_value<n_tup<i32,1>>()); }
template<> mn_tup<i32,1,2> random_value<mn_tup<i32,1,2>>() { return mn_tup<i32,1,2>(random_value<n_tup<i32,1>>(), random_value<n_tup<i32,1>>()); }
template<> mn_tup<i32,1,3> random_value<mn_tup<i32,1,3>>() { return mn_tup<i32,1,3>(random_value<n_tup<i32,1>>(), random_value<n_tup<i32,1>>(), random_value<n_tup<i32,1>>()); }
template<> mn_tup<i32,1,4> random_value<mn_tup<i32,1,4>>() { return mn_tup<i32,1,4>(random_value<n_tup<i32,1>>(), random_value<n_tup<i32,1>>(), random_value<n_tup<i32,1>>(), random_value<n_tup<i32,1>>()); }
template<> mn_tup<i32,2,1> random_value<mn_tup<i32,2,1>>() { return mn_tup<i32,2,1>(random_value<n_tup<i32,2>>()); }
template<> mn_tup<i32,2,2> random_value<mn_tup<i32,2,2>>() { return mn_tup<i32,2,2>(random_value<n_tup<i32,2>>(), random_value<n_tup<i32,2>>()); }
template<> mn_tup<i32,2,3> random_value<mn_tup<i32,2,3>>() { return mn_tup<i32,2,3>(random_value<n_tup<i32,2>>(), random_value<n_tup<i32,2>>(), random_value<n_tup<i32,2>>()); }
template<> mn_tup<i32,2,4> random_value<mn_tup<i32,2,4>>() { return mn_tup<i32,2,4>(random_value<n_tup<i32,2>>(), random_value<n_tup<i32,2>>(), random_value<n_tup<i32,2>>(), random_value<n_tup<i32,2>>()); }
template<> mn_tup<i32,3,1> random_value<mn_tup<i32,3,1>>() { return mn_tup<i32,3,1>(random_value<n_tup<i32,3>>()); }
template<> mn_tup<i32,3,2> random_value<mn_tup<i32,3,2>>() { return mn_tup<i32,3,2>(random_value<n_tup<i32,3>>(), random_value<n_tup<i32,3>>()); }
template<> mn_tup<i32,3,3> random_value<mn_tup<i32,3,3>>() { return mn_tup<i32,3,3>(random_value<n_tup<i32,3>>(), random_value<n_tup<i32,3>>(), random_value<n_tup<i32,3>>()); }
template<> mn_tup<i32,3,4> random_value<mn_tup<i32,3,4>>() { return mn_tup<i32,3,4>(random_value<n_tup<i32,3>>(), random_value<n_tup<i32,3>>(), random_value<n_tup<i32,3>>(), random_value<n_tup<i32,3>>()); }
template<> mn_tup<i32,4,1> random_value<mn_tup<i32,4,1>>() { return mn_tup<i32,4,1>(random_value<n_tup<i32,4>>()); }
template<> mn_tup<i32,4,2> random_value<mn_tup<i32,4,2>>() { return mn_tup<i32,4,2>(random_value<n_tup<i32,4>>(), random_value<n_tup<i32,4>>()); }
template<> mn_tup<i32,4,3> random_value<mn_tup<i32,4,3>>() { return mn_tup<i32,4,3>(random_value<n_tup<i32,4>>(), random_value<n_tup<i32,4>>(), random_value<n_tup<i32,4>>()); }
template<> mn_tup<i32,4,4> random_value<mn_tup<i32,4,4>>() { return mn_tup<i32,4,4>(random_value<n_tup<i32,4>>(), random_value<n_tup<i32,4>>(), random_value<n_tup<i32,4>>(), random_value<n_tup<i32,4>>()); }
template<> n_tup<u32,1> random_value<n_tup<u32,1>>() { return n_tup<u32,1>(random_value<u32>()); }
template<> n_tup<u32,2> random_value<n_tup<u32,2>>() { return n_tup<u32,2>(random_value<u32>(), random_value<u32>()); }
template<> n_tup<u32,3> random_value<n_tup<u32,3>>() { return n_tup<u32,3>(random_value<u32>(), random_value<u32>(), random_value<u32>()); }
template<> n_tup<u32,4> random_value<n_tup<u32,4>>() { return n_tup<u32,4>(random_value<u32>(), random_value<u32>(), random_value<u32>(), random_value<u32>()); }
template<> mn_tup<u32,1,1> random_value<mn_tup<u32,1,1>>() { return mn_tup<u32,1,1>(random_value<n_tup<u32,1>>()); }
template<> mn_tup<u32,1,2> random_value<mn_tup<u32,1,2>>() { return mn_tup<u32,1,2>(random_value<n_tup<u32,1>>(), random_value<n_tup<u32,1>>()); }
template<> mn_tup<u32,1,3> random_value<mn_tup<u32,1,3>>() { return mn_tup<u32,1,3>(random_value<n_tup<u32,1>>(), random_value<n_tup<u32,1>>(), random_value<n_tup<u32,1>>()); }
template<> mn_tup<u32,1,4> random_value<mn_tup<u32,1,4>>() { return mn_tup<u32,1,4>(random_value<n_tup<u32,1>>(), random_value<n_tup<u32,1>>(), random_value<n_tup<u32,1>>(), random_value<n_tup<u32,1>>()); }
template<> mn_tup<u32,2,1> random_value<mn_tup<u32,2,1>>() { return mn_tup<u32,2,1>(random_value<n_tup<u32,2>>()); }
template<> mn_tup<u32,2,2> random_value<mn_tup<u32,2,2>>() { return mn_tup<u32,2,2>(random_value<n_tup<u32,2>>(), random_value<n_tup<u32,2>>()); }
template<> mn_tup<u32,2,3> random_value<mn_tup<u32,2,3>>() { return mn_tup<u32,2,3>(random_value<n_tup<u32,2>>(), random_value<n_tup<u32,2>>(), random_value<n_tup<u32,2>>()); }
template<> mn_tup<u32,2,4> random_value<mn_tup<u32,2,4>>() { return mn_tup<u32,2,4>(random_value<n_tup<u32,2>>(), random_value<n_tup<u32,2>>(), random_value<n_tup<u32,2>>(), random_value<n_tup<u32,2>>()); }
template<> mn_tup<u32,3,1> random_value<mn_tup<u32,3,1>>() { return mn_tup<u32,3,1>(random_value<n_tup<u32,3>>()); }
template<> mn_tup<u32,3,2> random_value<mn_tup<u32,3,2>>() { return mn_tup<u32,3,2>(random_value<n_tup<u32,3>>(), random_value<n_tup<u32,3>>()); }
template<> mn_tup<u32,3,3> random_value<mn_tup<u32,3,3>>() { return mn_tup<u32,3,3>(random_value<n_tup<u32,3>>(), random_value<n_tup<u32,3>>(), random_value<n_tup<u32,3>>()); }
template<> mn_tup<u32,3,4> random_value<mn_tup<u32,3,4>>() { return mn_tup<u32,3,4>(random_value<n_tup<u32,3>>(), random_value<n_tup<u32,3>>(), random_value<n_tup<u32,3>>(), random_value<n_tup<u32,3>>()); }
template<> mn_tup<u32,4,1> random_value<mn_tup<u32,4,1>>() { return mn_tup<u32,4,1>(random_value<n_tup<u32,4>>()); }
template<> mn_tup<u32,4,2> random_value<mn_tup<u32,4,2>>() { return mn_tup<u32,4,2>(random_value<n_tup<u32,4>>(), random_value<n_tup<u32,4>>()); }
template<> mn_tup<u32,4,3> random_value<mn_tup<u32,4,3>>() { return mn_tup<u32,4,3>(random_value<n_tup<u32,4>>(), random_value<n_tup<u32,4>>(), random_value<n_tup<u32,4>>()); }
template<> mn_tup<u32,4,4> random_value<mn_tup<u32,4,4>>() { return mn_tup<u32,4,4>(random_value<n_tup<u32,4>>(), random_value<n_tup<u32,4>>(), random_value<n_tup<u32,4>>(), random_value<n_tup<u32,4>>()); }
template<> n_tup<i64,1> random_value<n_tup<i64,1>>() { return n_tup<i64,1>(random_value<i64>()); }
template<> n_tup<i64,2> random_value<n_tup<i64,2>>() { return n_tup<i64,2>(random_value<i64>(), random_value<i64>()); }
template<> n_tup<i64,3> random_value<n_tup<i64,3>>() { return n_tup<i64,3>(random_value<i64>(), random_value<i64>(), random_value<i64>()); }
template<> n_tup<i64,4> random_value<n_tup<i64,4>>() { return n_tup<i64,4>(random_value<i64>(), random_value<i64>(), random_value<i64>(), random_value<i64>()); }
template<> mn_tup<i64,1,1> random_value<mn_tup<i64,1,1>>() { return mn_tup<i64,1,1>(random_value<n_tup<i64,1>>()); }
template<> mn_tup<i64,1,2> random_value<mn_tup<i64,1,2>>() { return mn_tup<i64,1,2>(random_value<n_tup<i64,1>>(), random_value<n_tup<i64,1>>()); }
template<> mn_tup<i64,1,3> random_value<mn_tup<i64,1,3>>() { return mn_tup<i64,1,3>(random_value<n_tup<i64,1>>(), random_value<n_tup<i64,1>>(), random_value<n_tup<i64,1>>()); }
template<> mn_tup<i64,1,4> random_value<mn_tup<i64,1,4>>() { return mn_tup<i64,1,4>(random_value<n_tup<i64,1>>(), random_value<n_tup<i64,1>>(), random_value<n_tup<i64,1>>(), random_value<n_tup<i64,1>>()); }
template<> mn_tup<i64,2,1> random_value<mn_tup<i64,2,1>>() { return mn_tup<i64,2,1>(random_value<n_tup<i64,2>>()); }
template<> mn_tup<i64,2,2> random_value<mn_tup<i64,2,2>>() { return mn_tup<i64,2,2>(random_value<n_tup<i64,2>>(), random_value<n_tup<i64,2>>()); }
template<> mn_tup<i64,2,3> random_value<mn_tup<i64,2,3>>() { return mn_tup<i64,2,3>(random_value<n_tup<i64,2>>(), random_value<n_tup<i64,2>>(), random_value<n_tup<i64,2>>()); }
template<> mn_tup<i64,2,4> random_value<mn_tup<i64,2,4>>() { return mn_tup<i64,2,4>(random_value<n_tup<i64,2>>(), random_value<n_tup<i64,2>>(), random_value<n_tup<i64,2>>(), random_value<n_tup<i64,2>>()); }
template<> mn_tup<i64,3,1> random_value<mn_tup<i64,3,1>>() { return mn_tup<i64,3,1>(random_value<n_tup<i64,3>>()); }
template<> mn_tup<i64,3,2> random_value<mn_tup<i64,3,2>>() { return mn_tup<i64,3,2>(random_value<n_tup<i64,3>>(), random_value<n_tup<i64,3>>()); }
template<> mn_tup<i64,3,3> random_value<mn_tup<i64,3,3>>() { return mn_tup<i64,3,3>(random_value<n_tup<i64,3>>(), random_value<n_tup<i64,3>>(), random_value<n_tup<i64,3>>()); }
template<> mn_tup<i64,3,4> random_value<mn_tup<i64,3,4>>() { return mn_tup<i64,3,4>(random_value<n_tup<i64,3>>(), random_value<n_tup<i64,3>>(), random_value<n_tup<i64,3>>(), random_value<n_tup<i64,3>>()); }
template<> mn_tup<i64,4,1> random_value<mn_tup<i64,4,1>>() { return mn_tup<i64,4,1>(random_value<n_tup<i64,4>>()); }
template<> mn_tup<i64,4,2> random_value<mn_tup<i64,4,2>>() { return mn_tup<i64,4,2>(random_value<n_tup<i64,4>>(), random_value<n_tup<i64,4>>()); }
template<> mn_tup<i64,4,3> random_value<mn_tup<i64,4,3>>() { return mn_tup<i64,4,3>(random_value<n_tup<i64,4>>(), random_value<n_tup<i64,4>>(), random_value<n_tup<i64,4>>()); }
template<> mn_tup<i64,4,4> random_value<mn_tup<i64,4,4>>() { return mn_tup<i64,4,4>(random_value<n_tup<i64,4>>(), random_value<n_tup<i64,4>>(), random_value<n_tup<i64,4>>(), random_value<n_tup<i64,4>>()); }
template<> n_tup<u64,1> random_value<n_tup<u64,1>>() { return n_tup<u64,1>(random_value<u64>()); }
template<> n_tup<u64,2> random_value<n_tup<u64,2>>() { return n_tup<u64,2>(random_value<u64>(), random_value<u64>()); }
template<> n_tup<u64,3> random_value<n_tup<u64,3>>() { return n_tup<u64,3>(random_value<u64>(), random_value<u64>(), random_value<u64>()); }
template<> n_tup<u64,4> random_value<n_tup<u64,4>>() { return n_tup<u64,4>(random_value<u64>(), random_value<u64>(), random_value<u64>(), random_value<u64>()); }
template<> mn_tup<u64,1,1> random_value<mn_tup<u64,1,1>>() { return mn_tup<u64,1,1>(random_value<n_tup<u64,1>>()); }
template<> mn_tup<u64,1,2> random_value<mn_tup<u64,1,2>>() { return mn_tup<u64,1,2>(random_value<n_tup<u64,1>>(), random_value<n_tup<u64,1>>()); }
template<> mn_tup<u64,1,3> random_value<mn_tup<u64,1,3>>() { return mn_tup<u64,1,3>(random_value<n_tup<u64,1>>(), random_value<n_tup<u64,1>>(), random_value<n_tup<u64,1>>()); }
template<> mn_tup<u64,1,4> random_value<mn_tup<u64,1,4>>() { return mn_tup<u64,1,4>(random_value<n_tup<u64,1>>(), random_value<n_tup<u64,1>>(), random_value<n_tup<u64,1>>(), random_value<n_tup<u64,1>>()); }
template<> mn_tup<u64,2,1> random_value<mn_tup<u64,2,1>>() { return mn_tup<u64,2,1>(random_value<n_tup<u64,2>>()); }
template<> mn_tup<u64,2,2> random_value<mn_tup<u64,2,2>>() { return mn_tup<u64,2,2>(random_value<n_tup<u64,2>>(), random_value<n_tup<u64,2>>()); }
template<> mn_tup<u64,2,3> random_value<mn_tup<u64,2,3>>() { return mn_tup<u64,2,3>(random_value<n_tup<u64,2>>(), random_value<n_tup<u64,2>>(), random_value<n_tup<u64,2>>()); }
template<> mn_tup<u64,2,4> random_value<mn_tup<u64,2,4>>() { return mn_tup<u64,2,4>(random_value<n_tup<u64,2>>(), random_value<n_tup<u64,2>>(), random_value<n_tup<u64,2>>(), random_value<n_tup<u64,2>>()); }
template<> mn_tup<u64,3,1> random_value<mn_tup<u64,3,1>>() { return mn_tup<u64,3,1>(random_value<n_tup<u64,3>>()); }
template<> mn_tup<u64,3,2> random_value<mn_tup<u64,3,2>>() { return mn_tup<u64,3,2>(random_value<n_tup<u64,3>>(), random_value<n_tup<u64,3>>()); }
template<> mn_tup<u64,3,3> random_value<mn_tup<u64,3,3>>() { return mn_tup<u64,3,3>(random_value<n_tup<u64,3>>(), random_value<n_tup<u64,3>>(), random_value<n_tup<u64,3>>()); }
template<> mn_tup<u64,3,4> random_value<mn_tup<u64,3,4>>() { return mn_tup<u64,3,4>(random_value<n_tup<u64,3>>(), random_value<n_tup<u64,3>>(), random_value<n_tup<u64,3>>(), random_value<n_tup<u64,3>>()); }
template<> mn_tup<u64,4,1> random_value<mn_tup<u64,4,1>>() { return mn_tup<u64,4,1>(random_value<n_tup<u64,4>>()); }
template<> mn_tup<u64,4,2> random_value<mn_tup<u64,4,2>>() { return mn_tup<u64,4,2>(random_value<n_tup<u64,4>>(), random_value<n_tup<u64,4>>()); }
template<> mn_tup<u64,4,3> random_value<mn_tup<u64,4,3>>() { return mn_tup<u64,4,3>(random_value<n_tup<u64,4>>(), random_value<n_tup<u64,4>>(), random_value<n_tup<u64,4>>()); }
template<> mn_tup<u64,4,4> random_value<mn_tup<u64,4,4>>() { return mn_tup<u64,4,4>(random_value<n_tup<u64,4>>(), random_value<n_tup<u64,4>>(), random_value<n_tup<u64,4>>(), random_value<n_tup<u64,4>>()); }
template<> n_tup<f32,1> random_value<n_tup<f32,1>>() { return n_tup<f32,1>(random_value<f32>()); }
template<> n_tup<f32,2> random_value<n_tup<f32,2>>() { return n_tup<f32,2>(random_value<f32>(), random_value<f32>()); }
template<> n_tup<f32,3> random_value<n_tup<f32,3>>() { return n_tup<f32,3>(random_value<f32>(), random_value<f32>(), random_value<f32>()); }
template<> n_tup<f32,4> random_value<n_tup<f32,4>>() { return n_tup<f32,4>(random_value<f32>(), random_value<f32>(), random_value<f32>(), random_value<f32>()); }
template<> mn_tup<f32,1,1> random_value<mn_tup<f32,1,1>>() { return mn_tup<f32,1,1>(random_value<n_tup<f32,1>>()); }
template<> mn_tup<f32,1,2> random_value<mn_tup<f32,1,2>>() { return mn_tup<f32,1,2>(random_value<n_tup<f32,1>>(), random_value<n_tup<f32,1>>()); }
template<> mn_tup<f32,1,3> random_value<mn_tup<f32,1,3>>() { return mn_tup<f32,1,3>(random_value<n_tup<f32,1>>(), random_value<n_tup<f32,1>>(), random_value<n_tup<f32,1>>()); }
template<> mn_tup<f32,1,4> random_value<mn_tup<f32,1,4>>() { return mn_tup<f32,1,4>(random_value<n_tup<f32,1>>(), random_value<n_tup<f32,1>>(), random_value<n_tup<f32,1>>(), random_value<n_tup<f32,1>>()); }
template<> mn_tup<f32,2,1> random_value<mn_tup<f32,2,1>>() { return mn_tup<f32,2,1>(random_value<n_tup<f32,2>>()); }
template<> mn_tup<f32,2,2> random_value<mn_tup<f32,2,2>>() { return mn_tup<f32,2,2>(random_value<n_tup<f32,2>>(), random_value<n_tup<f32,2>>()); }
template<> mn_tup<f32,2,3> random_value<mn_tup<f32,2,3>>() { return mn_tup<f32,2,3>(random_value<n_tup<f32,2>>(), random_value<n_tup<f32,2>>(), random_value<n_tup<f32,2>>()); }
template<> mn_tup<f32,2,4> random_value<mn_tup<f32,2,4>>() { return mn_tup<f32,2,4>(random_value<n_tup<f32,2>>(), random_value<n_tup<f32,2>>(), random_value<n_tup<f32,2>>(), random_value<n_tup<f32,2>>()); }
template<> mn_tup<f32,3,1> random_value<mn_tup<f32,3,1>>() { return mn_tup<f32,3,1>(random_value<n_tup<f32,3>>()); }
template<> mn_tup<f32,3,2> random_value<mn_tup<f32,3,2>>() { return mn_tup<f32,3,2>(random_value<n_tup<f32,3>>(), random_value<n_tup<f32,3>>()); }
template<> mn_tup<f32,3,3> random_value<mn_tup<f32,3,3>>() { return mn_tup<f32,3,3>(random_value<n_tup<f32,3>>(), random_value<n_tup<f32,3>>(), random_value<n_tup<f32,3>>()); }
template<> mn_tup<f32,3,4> random_value<mn_tup<f32,3,4>>() { return mn_tup<f32,3,4>(random_value<n_tup<f32,3>>(), random_value<n_tup<f32,3>>(), random_value<n_tup<f32,3>>(), random_value<n_tup<f32,3>>()); }
template<> mn_tup<f32,4,1> random_value<mn_tup<f32,4,1>>() { return mn_tup<f32,4,1>(random_value<n_tup<f32,4>>()); }
template<> mn_tup<f32,4,2> random_value<mn_tup<f32,4,2>>() { return mn_tup<f32,4,2>(random_value<n_tup<f32,4>>(), random_value<n_tup<f32,4>>()); }
template<> mn_tup<f32,4,3> random_value<mn_tup<f32,4,3>>() { return mn_tup<f32,4,3>(random_value<n_tup<f32,4>>(), random_value<n_tup<f32,4>>(), random_value<n_tup<f32,4>>()); }
template<> mn_tup<f32,4,4> random_value<mn_tup<f32,4,4>>() { return mn_tup<f32,4,4>(random_value<n_tup<f32,4>>(), random_value<n_tup<f32,4>>(), random_value<n_tup<f32,4>>(), random_value<n_tup<f32,4>>()); }
template<> n_tup<f64,1> random_value<n_tup<f64,1>>() { return n_tup<f64,1>(random_value<f64>()); }
template<> n_tup<f64,2> random_value<n_tup<f64,2>>() { return n_tup<f64,2>(random_value<f64>(), random_value<f64>()); }
template<> n_tup<f64,3> random_value<n_tup<f64,3>>() { return n_tup<f64,3>(random_value<f64>(), random_value<f64>(), random_value<f64>()); }
template<> n_tup<f64,4> random_value<n_tup<f64,4>>() { return n_tup<f64,4>(random_value<f64>(), random_value<f64>(), random_value<f64>(), random_value<f64>()); }
template<> mn_tup<f64,1,1> random_value<mn_tup<f64,1,1>>() { return mn_tup<f64,1,1>(random_value<n_tup<f64,1>>()); }
template<> mn_tup<f64,1,2> random_value<mn_tup<f64,1,2>>() { return mn_tup<f64,1,2>(random_value<n_tup<f64,1>>(), random_value<n_tup<f64,1>>()); }
template<> mn_tup<f64,1,3> random_value<mn_tup<f64,1,3>>() { return mn_tup<f64,1,3>(random_value<n_tup<f64,1>>(), random_value<n_tup<f64,1>>(), random_value<n_tup<f64,1>>()); }
template<> mn_tup<f64,1,4> random_value<mn_tup<f64,1,4>>() { return mn_tup<f64,1,4>(random_value<n_tup<f64,1>>(), random_value<n_tup<f64,1>>(), random_value<n_tup<f64,1>>(), random_value<n_tup<f64,1>>()); }
template<> mn_tup<f64,2,1> random_value<mn_tup<f64,2,1>>() { return mn_tup<f64,2,1>(random_value<n_tup<f64,2>>()); }
template<> mn_tup<f64,2,2> random_value<mn_tup<f64,2,2>>() { return mn_tup<f64,2,2>(random_value<n_tup<f64,2>>(), random_value<n_tup<f64,2>>()); }
template<> mn_tup<f64,2,3> random_value<mn_tup<f64,2,3>>() { return mn_tup<f64,2,3>(random_value<n_tup<f64,2>>(), random_value<n_tup<f64,2>>(), random_value<n_tup<f64,2>>()); }
template<> mn_tup<f64,2,4> random_value<mn_tup<f64,2,4>>() { return mn_tup<f64,2,4>(random_value<n_tup<f64,2>>(), random_value<n_tup<f64,2>>(), random_value<n_tup<f64,2>>(), random_value<n_tup<f64,2>>()); }
template<> mn_tup<f64,3,1> random_value<mn_tup<f64,3,1>>() { return mn_tup<f64,3,1>(random_value<n_tup<f64,3>>()); }
template<> mn_tup<f64,3,2> random_value<mn_tup<f64,3,2>>() { return mn_tup<f64,3,2>(random_value<n_tup<f64,3>>(), random_value<n_tup<f64,3>>()); }
template<> mn_tup<f64,3,3> random_value<mn_tup<f64,3,3>>() { return mn_tup<f64,3,3>(random_value<n_tup<f64,3>>(), random_value<n_tup<f64,3>>(), random_value<n_tup<f64,3>>()); }
template<> mn_tup<f64,3,4> random_value<mn_tup<f64,3,4>>() { return mn_tup<f64,3,4>(random_value<n_tup<f64,3>>(), random_value<n_tup<f64,3>>(), random_value<n_tup<f64,3>>(), random_value<n_tup<f64,3>>()); }
template<> mn_tup<f64,4,1> random_value<mn_tup<f64,4,1>>() { return mn_tup<f64,4,1>(random_value<n_tup<f64,4>>()); }
template<> mn_tup<f64,4,2> random_value<mn_tup<f64,4,2>>() { return mn_tup<f64,4,2>(random_value<n_tup<f64,4>>(), random_value<n_tup<f64,4>>()); }
template<> mn_tup<f64,4,3> random_value<mn_tup<f64,4,3>>() { return mn_tup<f64,4,3>(random_value<n_tup<f64,4>>(), random_value<n_tup<f64,4>>(), random_value<n_tup<f64,4>>()); }
template<> mn_tup<f64,4,4> random_value<mn_tup<f64,4,4>>() { return mn_tup<f64,4,4>(random_value<n_tup<f64,4>>(), random_value<n_tup<f64,4>>(), random_value<n_tup<f64,4>>(), random_value<n_tup<f64,4>>()); }

