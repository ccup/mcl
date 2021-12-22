#ifndef H4D78FA8B_D9EE_497D_8F79_1F053DF8DD52
#define H4D78FA8B_D9EE_497D_8F79_1F053DF8DD52

#include "mcl/typedef.h"
#include "mcl/status.h"
#include "mcl/task/key.h"

MCL_STDC_BEGIN

MCL_TYPE_DEF(MclTask) {
	MclTaskKey key;
	MclStatus (*execute)(MclTask*);
	void (*destroy)(MclTask*);
};

MclStatus MclTask_Execute(MclTask*);
void MclTask_Destroy(MclTask*);

#define MCL_TASK(KEY, EXECUTE, DESTROY) {.key = KEY, .execute = EXEVUTE, .destroy = DESTROY}

MCL_STDC_END

#endif
