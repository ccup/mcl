#ifndef H5BE712A4_D1D9_4E2F_8A0A_93C283D0704A
#define H5BE712A4_D1D9_4E2F_8A0A_93C283D0704A

#if defined(__GNUC__)
# define likely(expr)   __builtin_expect(!!(expr), 1)
# define unlikely(expr) __builtin_expect(!!(expr), 0)
#else
# define likely(expr)   !!(expr)
# define unlikely(expr) !!(expr)
#endif

#endif
