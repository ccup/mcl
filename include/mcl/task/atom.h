#ifndef H68503BB8_2A01_4C21_B8B1_41124775F438
#define H68503BB8_2A01_4C21_B8B1_41124775F438

#include "mcl/keyword.h"

MCL_STDC_BEGIN

typedef volatile uint32_t MclAtom;

MCL_INLINE MclAtom MclAtom_Add(MclAtom *atom, uint32_t value) {
    return __sync_add_and_fetch(atom, value);
}

MCL_INLINE MclAtom MclAtom_Sub(MclAtom *atom, uint32_t value) {
    return __sync_sub_and_fetch(atom, value);
}

MCL_INLINE MclAtom MclAtom_Set(MclAtom *atom, uint32_t value) {
    return __sync_lock_test_and_set(atom, value);
}

MCL_INLINE void MclAtom_Clear(MclAtom *atom) {
	__sync_lock_release(atom);
}

MCL_INLINE bool MclAtom_CompareSwap(MclAtom *atom, uint32_t oldValue, uint32_t newValue) {
    return __sync_bool_compare_and_swap(atom, oldValue, newValue);
}

MCL_INLINE bool MclAtom_IsTrue(const MclAtom *atom) {
	return (*atom) != 0;
}

MCL_STDC_END

#endif
