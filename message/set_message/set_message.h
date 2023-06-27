#ifndef SET_MESSAGE_H
#define SET_MESSAGE_H

#include <stdio.h>

#include "types.h"

extern void convert_strikes_csv_to_json(void);
extern void write_strike_data_to_file(FILE *file_name, StrikeData *strike_data);
extern void set_single_strike(StrikeData *strike_data);

#endif