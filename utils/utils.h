#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#include "instruction.h"

#define getMask64(start, end)                                    \
    ((int64_t)(((int64_t)1) << end) - (((uint64_t)1) << start) + \
     (int64_t)(((int64_t)1) << end))

#define chooseBitsNoMove(instruc_bin, start, end) \
    (instruc_bin & getMask64(start, end))

extern size_t file_size(FILE*);
extern uint32_t chooseBits(BinaryInstruction, unsigned int, unsigned int);
extern int32_t chooseBitsSigned(BinaryInstruction, unsigned int, unsigned int);

#endif
