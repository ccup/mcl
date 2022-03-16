#ifndef H68503BB8_2A01_4C21_B8B1_41124775F438
#define H68503BB8_2A01_4C21_B8B1_41124775F438

#include "mcl/keyword.h"

MCL_STDC_BEGIN

typedef volatile MclSize MclAtomic;

MCL_INLINE void MclAtomic_Clear(MclAtomic *self) {
	__sync_lock_release(self);
}

MCL_INLINE MclAtomic MclAtomic_Set(MclAtomic *self, MclSize value) {
    return __sync_lock_test_and_set(self, value);
}

MCL_INLINE MclAtomic MclAtomic_Get(MclAtomic *self) {
    return __sync_add_and_fetch(self, 0);
}

MCL_INLINE bool MclAtomic_IsTrue(MclAtomic *self) {
	return __sync_bool_compare_and_swap(self, 1, 1);
}

MCL_INLINE bool MclAtomic_IsFalse(MclAtomic *self) {
	return __sync_bool_compare_and_swap(self, 0, 0);
}

MCL_INLINE MclAtomic MclAtomic_FetchAdd(MclAtomic *self, MclSize value) {
    return __sync_fetch_and_add(self, value);
}

MCL_INLINE MclAtomic MclAtomic_FetchSub(MclAtomic *self, MclSize value) {
    return __sync_fetch_and_sub(self, value);
}

MCL_INLINE MclAtomic MclAtomic_FetchOr(MclAtomic *self, MclSize value) {
    return __sync_fetch_and_or(self, value);
}

MCL_INLINE MclAtomic MclAtomic_FetchAnd(MclAtomic *self, MclSize value) {
    return __sync_fetch_and_and(self, value);
}

MCL_INLINE MclAtomic MclAtomic_FetchXor(MclAtomic *self, MclSize value) {
    return __sync_fetch_and_xor(self, value);
}

MCL_INLINE MclAtomic MclAtomic_AddFetch(MclAtomic *self, MclSize value) {
    return __sync_add_and_fetch(self, value);
}

MCL_INLINE MclAtomic MclAtomic_SubFetch(MclAtomic *self, MclSize value) {
    return __sync_sub_and_fetch(self, value);
}

MCL_INLINE MclAtomic MclAtomic_OrFetch(MclAtomic *self, MclSize value) {
    return __sync_or_and_fetch(self, value);
}

MCL_INLINE MclAtomic MclAtomic_AndFetch(MclAtomic *self, MclSize value) {
    return __sync_and_and_fetch(self, value);
}

MCL_INLINE MclAtomic MclAtomic_XorFetch(MclAtomic *self, MclSize value) {
    return __sync_xor_and_fetch(self, value);
}

MCL_INLINE bool MclAtomic_CompareSwap(MclAtomic *self, MclSize oldValue, MclSize newValue) {
    return __sync_bool_compare_and_swap(self, oldValue, newValue);
}

MCL_INLINE MclAtomic MclAtomic_CompareSwapVal(MclAtomic *self, MclSize oldValue, MclSize newValue) {
    return __sync_val_compare_and_swap(self, oldValue, newValue);
}

#define MCL_ATOMIC_SYNC(...)  __sync_synchronize(__VA_ARGS__)

MCL_STDC_END

#endif
