#ifndef MODES_H
# define MODES_H

#include <avr/io.h>

#define MODE0	0
#define MODE1	1
#define MODE2	2
#define MODE3	3
#define MODE4	4
#define MODE5	5
#define MODE6	6
#define MODE7	7
#define MODE8	8
#define MODE9	9
#define MODE10	10

extern	uint8_t g_current_mode;

void mode0(void);
void mode1(void);
void mode2(void);
void mode3(void);
void mode4(void);
void mode6(void);
void mode7(void);
void mode8(void);

#endif
