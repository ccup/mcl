#ifndef HF06F9B22_D70F_4E73_A0A5_6425F53DF6D0
#define HF06F9B22_D70F_4E73_A0A5_6425F53DF6D0

#include "mcl/interface.h"
#include "mcl/status.h"

MCL_STDC_BEGIN

MCL_INTERFACE(MclOwnerShip) {
	MclStatus (*borrow)(MclOwnerShip*, void*);
	void (*release)(MclOwnerShip*, void*);
};

MCL_STDC_END

#endif
