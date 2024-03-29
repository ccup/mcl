#ifndef H4D78FA8B_D9EE_497D_8F79_1F053DF8DD52
#define H4D78FA8B_D9EE_497D_8F79_1F053DF8DD52

#include "mcl/interface.h"
#include "mcl/status.h"
#include "mcl/task/task_key.h"

MCL_STDC_BEGIN

MCL_INTERFACE(MclTask) {
	MclTaskKey key;
	MclStatus (*execute)(MclTask*);
	void (*destroy)(MclTask*);
};

MclStatus MclTask_Execute(MclTask*);
void MclTask_Destroy(MclTask*);

///////////////////////////////////////////////////////////
#define MCL_TASK(KEY, EXECUTE, DESTROY)					\
{.key = (KEY), .execute = (EXECUTE), .destroy = (DESTROY)}

MCL_STDC_END

#endif
