#include "mcl/task/task.h"
#include "mcl/assert.h"
#include "mcl/log/log.h"

MclStatus MclTask_Execute(MclTask *self) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(self->execute);

	MCL_LOG_DBG("Execute task %u", self->key);
	return self->execute(self);
}

void MclTask_Destroy(MclTask *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MCL_LOG_DBG("Destroy task %u", self->key);
	if (self->destroy) self->destroy(self);
}
