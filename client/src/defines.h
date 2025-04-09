#pragma once

//TODO: i think a lot of these types are wrong do a lil sizeof() print thang

#define u64 unsigned long
#define i64 long
#define u32 unsigned int
#define i32 int
#define u16 unsigned short 
#define i16 short
#define u8 unsigned char
#define i8 signed char
#define P1 0
#define P2 1

#define true 1
#define false 0

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 500

//command stuff so that any events that need command data can send them when fired
enum {
    ACTION_W1,
    ACTION_W2,
    ACTION_W3,
    ACTION_W4,
    ACTION_W5,
    ACTION_W6,
    ACTION_S,
    ACTION_G,
    ACTION_CH,
    ACTION_T0,
    ACTION_T1,
    ACTION_T2,
    ACTION_T3,
    ACTION_T4,
    ACTION_T5,
    ACTION_T6,
    ACTION_T7,
    ACTION_T8,
    ACTION_T9,
    ACTION_DA,
    ACTION_DD,
    ACTION_C,
    ACTION_R1,
    ACTION_R2,
    ACTION_R3,
    ACTION_L1,
    ACTION_L2,
    ACTION_L3,
};
