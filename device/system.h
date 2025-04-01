#ifndef _GW_SYSTEM_H_
#define _GW_SYSTEM_H_
#include "def.h"

int init(int argc, char **argv);
int system_run(int clock_cycles);
u32 get_buttons();

void writeR(u8 data);
u8 readK(u8 S);
u8 readBA();
u8 readB();

#endif
