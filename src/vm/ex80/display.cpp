/*
	TOSHIBA EX-80 Emulator 'eEX-80'

	Author : Takeda.Toshiya
	Date   : 2015.12.10-

	[ display ]
*/

#include "display.h"
#include "../f9368.h"

#define EVENT_HBLANK 0

/*
@ABCDEFGHIJKLMNO
PQRSTUVWXYZ[\]^_
 !"#$%&'()*+,-./
0123456789:;<=>?
����������������
����������������
 ���������������
����������������
*/

uint8_t font_pattern[8 * 128] = {
	0x0e, 0x11, 0x1d, 0x15, 0x1d, 0x01, 0x0e, 0x00,
	0x04, 0x0a, 0x0a, 0x11, 0x1f, 0x11, 0x11, 0x00,
	0x0f, 0x11, 0x11, 0x0f, 0x11, 0x11, 0x0f, 0x00,
	0x0e, 0x11, 0x01, 0x01, 0x01, 0x11, 0x0e, 0x00,
	0x07, 0x09, 0x11, 0x11, 0x11, 0x09, 0x07, 0x00,
	0x1f, 0x01, 0x01, 0x1f, 0x01, 0x01, 0x1f, 0x00,
	0x1f, 0x01, 0x01, 0x1f, 0x01, 0x01, 0x01, 0x00,
	0x0e, 0x11, 0x01, 0x1d, 0x11, 0x11, 0x0e, 0x00,
	0x11, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x11, 0x00,
	0x0e, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0e, 0x00,
	0x1c, 0x08, 0x08, 0x08, 0x09, 0x09, 0x06, 0x00,
	0x11, 0x09, 0x05, 0x03, 0x05, 0x09, 0x11, 0x00,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x1f, 0x00,
	0x11, 0x1b, 0x15, 0x15, 0x11, 0x11, 0x11, 0x00,
	0x11, 0x11, 0x13, 0x15, 0x19, 0x11, 0x11, 0x00,
	0x0e, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e, 0x00,
	0x0f, 0x11, 0x11, 0x0f, 0x01, 0x01, 0x01, 0x00,
	0x0e, 0x11, 0x11, 0x15, 0x19, 0x0e, 0x10, 0x00,
	0x0f, 0x11, 0x11, 0x0f, 0x05, 0x09, 0x11, 0x00,
	0x0e, 0x11, 0x01, 0x0e, 0x10, 0x11, 0x0e, 0x00,
	0x1f, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00,
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e, 0x00,
	0x11, 0x11, 0x11, 0x0a, 0x0a, 0x0a, 0x04, 0x00,
	0x11, 0x11, 0x15, 0x15, 0x15, 0x0a, 0x0a, 0x00,
	0x11, 0x11, 0x0a, 0x04, 0x0a, 0x11, 0x11, 0x00,
	0x11, 0x11, 0x0a, 0x0a, 0x04, 0x04, 0x04, 0x00,
	0x1f, 0x10, 0x08, 0x04, 0x02, 0x01, 0x1f, 0x00,
	0x0e, 0x02, 0x02, 0x02, 0x02, 0x02, 0x0e, 0x00,
	0x11, 0x0a, 0x1f, 0x04, 0x1f, 0x04, 0x04, 0x00,
	0x0e, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0e, 0x00,
	0x04, 0x0a, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x04, 0x0e, 0x0e, 0x04, 0x04, 0x00, 0x04, 0x00,
	0x12, 0x12, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x0a, 0x0a, 0x1f, 0x0a, 0x1f, 0x0a, 0x0a, 0x00,
	0x04, 0x1e, 0x05, 0x0e, 0x14, 0x0f, 0x04, 0x00,
	0x13, 0x0b, 0x08, 0x04, 0x02, 0x1a, 0x19, 0x00,
	0x0c, 0x12, 0x0a, 0x04, 0x0a, 0x11, 0x16, 0x00,
	0x04, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x04, 0x02, 0x02, 0x02, 0x04, 0x08, 0x00,
	0x02, 0x04, 0x08, 0x08, 0x08, 0x04, 0x02, 0x00,
	0x04, 0x15, 0x0e, 0x04, 0x0e, 0x15, 0x04, 0x00,
	0x04, 0x04, 0x04, 0x1f, 0x04, 0x04, 0x04, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x02, 0x00,
	0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x00,
	0x10, 0x08, 0x08, 0x04, 0x02, 0x02, 0x01, 0x00,
	0x0e, 0x11, 0x19, 0x15, 0x13, 0x11, 0x0e, 0x00,
	0x04, 0x06, 0x04, 0x04, 0x04, 0x04, 0x0e, 0x00,
	0x0e, 0x11, 0x10, 0x08, 0x04, 0x02, 0x1f, 0x00,
	0x0e, 0x11, 0x10, 0x0e, 0x10, 0x11, 0x0e, 0x00,
	0x08, 0x0c, 0x0a, 0x09, 0x1f, 0x08, 0x08, 0x00,
	0x1f, 0x01, 0x01, 0x0f, 0x10, 0x11, 0x0e, 0x00,
	0x0c, 0x02, 0x01, 0x0f, 0x11, 0x11, 0x0e, 0x00,
	0x1f, 0x11, 0x11, 0x08, 0x08, 0x04, 0x04, 0x00,
	0x0e, 0x11, 0x11, 0x0e, 0x11, 0x11, 0x0e, 0x00,
	0x0e, 0x11, 0x11, 0x1e, 0x10, 0x11, 0x0e, 0x00,
	0x00, 0x04, 0x04, 0x00, 0x04, 0x04, 0x00, 0x00,
	0x00, 0x04, 0x04, 0x00, 0x04, 0x04, 0x02, 0x00,
	0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 0x00,
	0x00, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x00, 0x00,
	0x01, 0x02, 0x04, 0x08, 0x04, 0x02, 0x01, 0x00,
	0x0e, 0x11, 0x10, 0x08, 0x04, 0x00, 0x04, 0x00,
	0x1e, 0x12, 0x11, 0x14, 0x08, 0x04, 0x02, 0x00,
	0x18, 0x0f, 0x08, 0x1f, 0x08, 0x04, 0x02, 0x00,
	0x15, 0x15, 0x1a, 0x10, 0x08, 0x04, 0x02, 0x00,
	0x0e, 0x00, 0x1f, 0x08, 0x08, 0x04, 0x02, 0x00,
	0x02, 0x02, 0x06, 0x0a, 0x12, 0x02, 0x02, 0x00,
	0x08, 0x08, 0x1f, 0x08, 0x08, 0x04, 0x02, 0x00,
	0x00, 0x0e, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00,
	0x00, 0x1f, 0x10, 0x0a, 0x04, 0x0a, 0x11, 0x00,
	0x04, 0x1f, 0x08, 0x04, 0x0e, 0x15, 0x04, 0x00,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x04, 0x02, 0x00,
	0x04, 0x08, 0x0a, 0x12, 0x11, 0x11, 0x11, 0x00,
	0x01, 0x11, 0x0f, 0x01, 0x01, 0x01, 0x1e, 0x00,
	0x00, 0x1f, 0x10, 0x10, 0x08, 0x04, 0x02, 0x00,
	0x00, 0x06, 0x05, 0x09, 0x10, 0x10, 0x00, 0x00,
	0x04, 0x1f, 0x04, 0x15, 0x15, 0x15, 0x06, 0x00,
	0x00, 0x1f, 0x10, 0x10, 0x0a, 0x04, 0x08, 0x00,
	0x07, 0x18, 0x03, 0x1c, 0x00, 0x07, 0x18, 0x00,
	0x04, 0x02, 0x02, 0x02, 0x09, 0x1f, 0x10, 0x00,
	0x08, 0x08, 0x0b, 0x0c, 0x18, 0x04, 0x03, 0x00,
	0x00, 0x1f, 0x02, 0x1f, 0x02, 0x02, 0x1c, 0x00,
	0x02, 0x1f, 0x12, 0x12, 0x02, 0x04, 0x04, 0x00,
	0x00, 0x0e, 0x08, 0x08, 0x08, 0x08, 0x1f, 0x00,
	0x00, 0x1f, 0x10, 0x1e, 0x10, 0x10, 0x1f, 0x00,
	0x0e, 0x00, 0x1f, 0x10, 0x10, 0x08, 0x04, 0x00,
	0x09, 0x09, 0x09, 0x08, 0x08, 0x04, 0x02, 0x00,
	0x04, 0x04, 0x05, 0x05, 0x05, 0x15, 0x0d, 0x00,
	0x01, 0x01, 0x01, 0x11, 0x09, 0x05, 0x03, 0x00,
	0x00, 0x1f, 0x11, 0x11, 0x11, 0x11, 0x1f, 0x00,
	0x00, 0x1f, 0x11, 0x11, 0x10, 0x08, 0x04, 0x00,
	0x00, 0x01, 0x12, 0x14, 0x10, 0x08, 0x07, 0x00,
	0x05, 0x0a, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x05, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x04, 0x0a, 0x04, 0x00,
	0x0e, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x0f, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x02, 0x04, 0x08, 0x00,
	0x00, 0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x00,
	0x00, 0x1f, 0x10, 0x1f, 0x10, 0x08, 0x04, 0x00,
	0x00, 0x00, 0x1f, 0x10, 0x1c, 0x04, 0x02, 0x00,
	0x00, 0x00, 0x08, 0x04, 0x07, 0x04, 0x04, 0x00,
	0x00, 0x00, 0x04, 0x1f, 0x11, 0x08, 0x04, 0x00,
	0x00, 0x00, 0x00, 0x0e, 0x04, 0x04, 0x1f, 0x00,
	0x00, 0x00, 0x08, 0x1f, 0x0c, 0x0a, 0x0d, 0x00,
	0x00, 0x00, 0x02, 0x1f, 0x12, 0x02, 0x04, 0x00,
	0x00, 0x00, 0x00, 0x0e, 0x08, 0x08, 0x1f, 0x00,
	0x00, 0x00, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x00,
	0x00, 0x00, 0x15, 0x15, 0x10, 0x08, 0x04, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x1e, 0x00, 0x00, 0x00,
	0x1f, 0x10, 0x14, 0x0c, 0x04, 0x04, 0x02, 0x00,
	0x10, 0x08, 0x04, 0x06, 0x05, 0x04, 0x04, 0x00,
	0x04, 0x1f, 0x11, 0x11, 0x10, 0x08, 0x04, 0x00,
	0x00, 0x1f, 0x04, 0x04, 0x04, 0x04, 0x1f, 0x00,
	0x08, 0x08, 0x1f, 0x0c, 0x0a, 0x09, 0x0c, 0x00,
	0x04, 0x04, 0x1f, 0x14, 0x12, 0x11, 0x0c, 0x00,
	0x04, 0x04, 0x1f, 0x04, 0x1f, 0x04, 0x04, 0x00,
	0x1e, 0x12, 0x11, 0x10, 0x08, 0x04, 0x02, 0x00,
	0x02, 0x02, 0x1e, 0x09, 0x08, 0x08, 0x04, 0x00,
	0x00, 0x1f, 0x10, 0x10, 0x10, 0x10, 0x1f, 0x00,
	0x0a, 0x1f, 0x0a, 0x08, 0x08, 0x04, 0x02, 0x00,
	0x01, 0x06, 0x11, 0x16, 0x10, 0x08, 0x07, 0x00,
	0x00, 0x1f, 0x10, 0x08, 0x04, 0x0a, 0x11, 0x00,
	0x02, 0x1f, 0x12, 0x0a, 0x02, 0x02, 0x1c, 0x00,
	0x11, 0x11, 0x12, 0x10, 0x08, 0x04, 0x02, 0x00,
};

static const int led_pattern[LED_HEIGHT][LED_WIDTH] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,2,0,0,0},
	{0,0,0,0,6,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,2,2,0,0,0},
	{0,0,0,0,6,6,0,1,1,1,1,1,1,1,1,1,1,1,1,0,2,2,2,0,0,0},
	{0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0},
	{0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0},
	{0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0},
	{0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0},
	{0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0},
	{0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0},
	{0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0},
	{0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0},
	{0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0},
	{0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0},
	{0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0},
	{0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0},
	{0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0},
	{0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0},
	{0,0,0,0,0,6,0,7,7,7,7,7,7,7,7,7,7,7,7,7,0,2,0,0,0,0},
	{0,0,0,0,0,0,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0,0,0,0,0},
	{0,0,0,0,5,0,7,7,7,7,7,7,7,7,7,7,7,7,7,0,3,0,0,0,0,0},
	{0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0},
	{0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0},
	{0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0},
	{0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0},
	{0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0},
	{0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0},
	{0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0},
	{0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0},
	{0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0},
	{0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0},
	{0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0},
	{0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0},
	{0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0},
	{0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0},
	{0,0,0,5,5,0,4,4,4,4,4,4,4,4,4,4,4,4,0,3,3,3,0,0,0,0},
	{0,0,0,5,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,3,3,0,0,0,0},
	{0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,3,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

void DISPLAY::initialize()
{
	// load rom image
	FILEIO* fio = new FILEIO();
	if(fio->Fopen(create_local_path(_T("FONT.ROM")), FILEIO_READ_BINARY)) {
		fio->Fread(font, sizeof(font), 1);
		fio->Fclose();
	} else {
		memcpy(font, font_pattern, sizeof(font));
	}
	delete fio;
	
	memset(screen, 0, sizeof(screen));
	odd_even = 0;
	dma = true;
	
	register_frame_event(this);
	register_vline_event(this);
}

void DISPLAY::write_signal(int id, uint32_t data, uint32_t mask)
{
	if(id == SIG_DISPLAY_DMA) {
		dma = ((data & mask) != 0);
	}
}

void DISPLAY::event_frame()
{
	odd_even = (odd_even + 1) & 1;
}

void DISPLAY::event_vline(int v, int clock)
{
	if((v & 1) == odd_even && v < 8 * 29 * 2) {
		uint8_t* dest = screen[v];
		if(dma) {
/*
			SW2	ON = CHAR / OFF = BIT
			SW3-1/2	ON ,ON  = 8000H-81FFH
				OFF,ON  = 8200H-83FFH
				ON ,OFF = 8400H-85FFH
				OFF,OFF = 8600H-87FFH
*/
			int line = v >> 4;
			int raster = (v >> 1) & 7;
			uint8_t* base = ram + (~(config.dipswitch >> 2) & 3) * 0x200 + (line + 1) * 16;
			
			for(int x = 0; x < 12; x++) {
				uint8_t pat = base[x];
				if(config.dipswitch & 2) {
					pat = (font[(pat & 0x7f) * 8 + raster] & 0x1f) << 2;
				}
				dest[0] = (pat & 0x01) >> 0;
				dest[1] = (pat & 0x02) >> 1;
				dest[2] = (pat & 0x04) >> 2;
				dest[3] = (pat & 0x08) >> 3;
				dest[4] = (pat & 0x10) >> 4;
				dest[5] = (pat & 0x20) >> 5;
				dest[6] = (pat & 0x40) >> 6;
				dest[7] = (pat & 0x80) >> 7;
				dest += 8;
			}
			d_cpu->write_signal(SIG_CPU_BUSREQ, 1, 1);
			// wait 8clk/byte
			register_event_by_clock(this, EVENT_HBLANK, 8 * 12, false, NULL);
		} else {
			memset(dest, 0, 8 * 12);
		}
	}
}

void DISPLAY::event_callback(int event_id, int err)
{
	if(event_id == EVENT_HBLANK) {
		d_cpu->write_signal(SIG_CPU_BUSREQ, 0, 0);
	}
}

void DISPLAY::draw_screen()
{
	// draw screen
	scrntype_t color_crt[2] = {RGB_COLOR(255, 255, 255), 0};
	for(int y = 0; y < 8 * 29 * 2; y++) {
		scrntype_t* dest = emu->get_screen_buffer(y + vm_ranges[8].y) + vm_ranges[8].x;
		uint8_t* src = screen[y];
		for(int x = 0; x < 8 * 12; x++) {
			dest[0] = dest[1] = dest[2] = dest[3] = dest[4] = dest[5] = color_crt[src[x] & 1];
			dest += 6;
		}
	}
	
	// draw 7-seg LEDs
	scrntype_t color_on = RGB_COLOR(255, 8, 72);
	scrntype_t color_off = RGB_COLOR(56, 0, 0);
	scrntype_t color_led[9];
	uint8_t* base = ram + (~(config.dipswitch >> 2) & 3) * 0x200;
	color_led[0] = RGB_COLOR(38, 8, 0);
	
	for(int i = 0; i < 8; i++) {
		uint8_t val = (base[i >> 1] >> ((i & 1) ? 0 : 4)) & 0x0f;
		
		for(int b = 0; b < 8; b++) {
			color_led[b + 1] = (dma && f9368[val][b]) ? color_on : color_off;
		}
		for(int y = 0; y < LED_HEIGHT; y++) {
			scrntype_t* dest = emu->get_screen_buffer(vm_ranges[i].y + y) + vm_ranges[i].x;
			for(int x = 0; x < LED_WIDTH; x++) {
				dest[x] = color_led[led_pattern[y][x]];
			}
		}
	}
}

#define STATE_VERSION	1

bool DISPLAY::process_state(FILEIO* state_fio, bool loading)
{
	if(!state_fio->StateCheckUint32(STATE_VERSION)) {
		return false;
	}
	if(!state_fio->StateCheckInt32(this_device_id)) {
		return false;
	}
	state_fio->StateValue(odd_even);
	state_fio->StateValue(dma);
	return true;
}

