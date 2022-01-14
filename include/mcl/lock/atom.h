#ifndef H68503BB8_2A01_4C21_B8B1_41124775F438
#define H68503BB8_2A01_4C21_B8B1_41124775F438

#include "mcl/keyword.h"

MCL_STDC_BEGIN

typedef volatile MclSize MclAtom;

MCL_INLINE void MclAtom_Clear(MclAtom *self) {
	__sync_lock_release(self);
}

MCL_INLINE MclAtom MclAtom_Set(MclAtom *self, MclSize value) {
    return __sync_lock_test_and_set(self, value);
}

MCL_INLINE MclAtom MclAtom_Get(MclAtom *self) {
    return __sync_add_and_fetch(self, 0);
}

MCL_INLINE bool MclAtom_IsTrue(MclAtom *self) {
	return __sync_bool_compare_and_swap(self, 1, 1);
}

MCL_INLINE bool MclAtom_IsFalse(MclAtom *self) {
	return __sync_bool_compare_and_swap(self, 0, 0);
}

MCL_INLINE MclAtom MclAtom_FetchAdd(MclAtom *self, MclSize value) {
    return __sync_fetch_and_add(self, value);
}

MCL_INLINE MclAtom MclAtom_FetchSub(MclAtom *self, MclSize value) {
    return __sync_fetch_and_sub(self, value);
}

MCL_INLINE MclAtom MclAtom_FetchOr(MclAtom *self, MclSize value) {
    return __sync_fetch_and_or(self, value);
}

MCL_INLINE MclAtom MclAtom_FetchAnd(MclAtom *self, MclSize value) {
    return __sync_fetch_and_and(self, value);
}

MCL_INLINE MclAtom MclAtom_FetchXor(MclAtom *self, MclSize value) {
    return __sync_fetch_and_xor(self, value);
}

MCL_INLINE MclAtom MclAtom_FetchNand(MclAtom *self, MclSize value) {
    return __sync_fetch_and_nand(self, value);
}

MCL_INLINE MclAtom MclAtom_AddFetch(MclAtom *self, MclSize value) {
    return __sync_add_and_fetch(self, value);
}

MCL_INLINE MclAtom MclAtom_SubFetch(MclAtom *self, MclSize value) {
    return __sync_sub_and_fetch(self, value);
}

MCL_INLINE MclAtom MclAtom_OrFetch(MclAtom *self, MclSize value) {
    return __sync_or_and_fetch(self, value);
}

MCL_INLINE MclAtom MclAtom_AndFetch(MclAtom *self, MclSize value) {
    return __sync_and_and_fetch(self, value);
}

MCL_INLINE MclAtom MclAtom_XorFetch(MclAtom *self, MclSize value) {
    return __sync_xor_and_fetch(self, value);
}

MCL_INLINE MclAtom MclAtom_NandFetch(MclAtom *self, MclSize value) {
    return __sync_nand_and_fetch(self, value);
}

MCL_INLINE bool MclAtom_CompareSwap(MclAtom *self, MclSize oldValue, MclSize newValue) {
    return __sync_bool_compare_and_swap(self, oldValue, newValue);
}

MCL_INLINE MclAtom MclAtom_CompareSwapVal(MclAtom *self, MclSize oldValue, MclSize newValue) {
    return __sync_val_compare_and_swap(self, oldValue, newValue);
}

#define MCL_ATOM_SYNC(...)  __sync_synchronize(__VA_ARGS__)

MCL_STDC_END

#endif
