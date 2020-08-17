#ifndef H1AAF9B3B_8991_491A_A081_DEC6E6907DAF
#define H1AAF9B3B_8991_491A_A081_DEC6E6907DAF

#include "mcl/role.h"
#include <stdint.h>

MCL_STDC_BEGIN

MCL_FWD_DECL(Energy);

typedef enum WorkerType {
	HUMAN, ROBOT,
}WorkerType;

DEFINE_ROLE(Worker) {
	uint32_t produce_num;
	USE_ROLE(Energy);
};

Worker* worker_create(WorkerType, uint8_t);
void    worker_release(Worker*);
void    worker_produce(Worker*);

MCL_STDC_END

#endif
