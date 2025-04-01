#ifndef __GW_TYPE_DEFS_H__
#define __GW_TYPE_DEFS_H__
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GW_LCD_WIDTH 320
#define GW_LCD_HEIGHT 240
#define GW_SCREEN_WIDTH 320
#define GW_SCREEN_HEIGHT 240

#define SYS_FREQ 32768U

#define ROM_CPU_SM5A "SM5A_"

typedef unsigned char byte;
typedef unsigned char un8, u8, uint8;
typedef unsigned short un16, u16, uint16;
typedef unsigned int un32, u32, uint32;
typedef uint64_t un64, u64, uint64;

typedef signed char n8;
typedef signed short n16;
typedef signed int n32;
typedef un16 word;
typedef word addr;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#endif
