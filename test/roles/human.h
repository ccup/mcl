#ifndef HC40D096A_08C2_4D40_BDA7_56BA9C80DBD3
#define HC40D096A_08C2_4D40_BDA7_56BA9C80DBD3

#include "mcl/stdc.h"
#include <stdint.h>
#include <stdbool.h>

MCL_STDC_BEGIN

typedef struct Human {
    bool is_hungry;
    uint8_t consumed_energy;
    uint8_t total_energy;
}Human;

void human_init(Human*, uint8_t);
void human_eat_food(Human*, uint8_t);
void human_consume(Human*, uint8_t);
bool human_exhaused(const Human*);

MCL_STDC_END

#endif
