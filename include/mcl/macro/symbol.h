#ifndef H5B60114C_61E3_4600_971C_356DB8AD102C
#define H5B60114C_61E3_4600_971C_356DB8AD102C

#define MCL_SYMBOL_RINGIZE_IMPL(s) #s
#define MCL_SYMBOL_STRINGIZE(s) MCL_SYMBOL_RINGIZE_IMPL(s)

#define MCL_SYMBOL_JOIN_AGAIN(s1, s2) s1##s2
#define MCL_SYMBOL_JOIN_IMPL(s1, s2) MCL_SYMBOL_JOIN_AGAIN(s1, s2)
#define MCL_SYMBOL_JOIN(s1, s2) MCL_SYMBOL_JOIN_IMPL(s1, s2)

#ifdef __COUNTER__
#  define MCL_SYMBOL_UNIQUE_ID __COUNTER__
#else
#  define MCL_SYMBOL_UNIQUE_ID __LINE__
#endif

///////////////////////////////////////////////////////////
#define MCL_SYMBOL_UNIQUE(prefix) MCL_SYMBOL_JOIN(prefix, MCL_SYMBOL_UNIQUE_ID)

#endif