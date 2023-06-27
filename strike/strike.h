#ifndef STRIKE_H
#define STRIKE_H

#include "time.h"
#include "types.h"

extern void checkoff_strike(const char* strike_id);
extern void process_strike(StationRaw* station1, StationRaw* station2,
                           StationRaw* station3, time_t time_received);

#endif
