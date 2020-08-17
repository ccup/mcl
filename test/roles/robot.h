#ifndef H52046B97_3269_47D9_86EA_4AED86AFEB6B
#define H52046B97_3269_47D9_86EA_4AED86AFEB6B

#include "mcl/stdc.h"
#include <stdint.h>
#include <stdbool.h>

MCL_STDC_BEGIN

typedef struct Robot {
    uint8_t charge_percent;
}Robot;

void robot_init(Robot*, uint8_t);
void robot_charge(Robot*, uint8_t);
void robot_consume(Robot*, uint8_t);
bool robot_exhaused(const Robot*);

MCL_STDC_END

#endif
