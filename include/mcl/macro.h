#ifndef H663B9854_4FEC_49D3_80A3_445DAA79542B
#define H663B9854_4FEC_49D3_80A3_445DAA79542B

#define MCL_MARCO_ARGS_NUM(...) MCL_MAROC_ARGS_NUM_PRIVATE(0, ##__VA_ARGS__,\
	64, 63, 62, 61, 60, 					\
	59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
	49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
	39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
	29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
	19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
	 9,  8,  7,  6,  5,  4,  3,  2,  1,  0)

#define MCL_MAROC_ARGS_NUM_PRIVATE(					  \
	 _0,  _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, \
	_10, _11, _12, _13, _14, _15, _16, _17, _18, _19, \
	_20, _21, _22, _23, _24, _25, _26, _27, _28, _29, \
	_30, _31, _32, _33, _34, _35, _36, _37, _38, _39, \
	_40, _41, _42, _43, _44, _45, _46, _47, _48, _49, \
	_50, _51, _52, _53, _54, _55, _56, _57, _58, _59, \
	_60, _61, _62, _63, _64, N, ...) N

#define ARG_0(...) 
#define ARG_1(a, ...) a 
#define ARG_2(a, ...) a, ARG_1(__VA_ARGS__)
#define ARG_3(a, ...) a, ARG_2(__VA_ARGS__)
#define ARG_4(a, ...) a, ARG_3(__VA_ARGS__)
#define ARG_5(a, ...) a, ARG_4(__VA_ARGS__)

#define ARG_TAKE_N_0(n, ...) ARG_##n(__VA_ARGS__)
#define ARG_TAKE_N_1(n, a, ...) ARG_TAKE_N_0(n, __VA_ARGS__)
#define ARG_TAKE_N_2(n, a, ...) ARG_TAKE_N_1(n, __VA_ARGS__)
#define ARG_TAKE_N_3(n, a, ...) ARG_TAKE_N_2(n, __VA_ARGS__)
#define ARG_TAKE_N_4(n, a, ...) ARG_TAKE_N_3(n, __VA_ARGS__)
#define ARG_TAKE_N_5(n, a, ...) ARG_TAKE_N_4(n, __VA_ARGS__)

// pos start from 0
#define MCL_ARG_TAKE(pos, n, ...) ARG_TAKE_N_##pos(n, __VA_ARGS__)
#define MCL_ARGS(...) __VA_ARGS__


#endif
