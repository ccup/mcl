#ifndef H68503BB8_2A01_4C21_B8B1_41124775F438
#define H68503BB8_2A01_4C21_B8B1_41124775F438

#include "mcl/keyword.h"

MCL_STDC_BEGIN

typedef volatile int MclAtom;

#define MCL_ATOM_INIT(ATOM, VALUE) ATOM = VALUE

MCL_INLINE void MclAtom_Set(MclAtom *atom, int value) {
	*atom = value;
}

MCL_INLINE int MclAtom_Get(MclAtom *atom) {
	return *atom;
}

MCL_INLINE bool MclAtom_IsTrue(MclAtom *atom) {
	return MclAtom_Get(atom) != 0;
}

MCL_STDC_END

#endif
