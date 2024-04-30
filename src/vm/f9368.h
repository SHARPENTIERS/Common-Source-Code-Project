/*
	Skelton for retropc emulator

	Author : Takeda.Toshiya
	Date   : 2015.12.15-

	[ F9368 ]
*/

#ifndef _F9368_H_
#define _F9368_H_

#include "../common.h"

/*
	 AAAA 
	F    B
	F    B
	 GGGG 
	E    C
	E    C
	 DDDD 
*/

static const uint8_t f9368[32][8] = {
	{1, 1, 1, 1, 1, 1, 0, 0},	// 0
	{0, 1, 1, 0, 0, 0, 0, 0},	// 1
	{1, 1, 0, 1, 1, 0, 1, 0},	// 2
	{1, 1, 1, 1, 0, 0, 1, 0},	// 3
	{0, 1, 1, 0, 0, 1, 1, 0},	// 4
	{1, 0, 1, 1, 0, 1, 1, 0},	// 5
	{1, 0, 1, 1, 1, 1, 1, 0},	// 6
	{1, 1, 1, 0, 0, 1, 0, 0},	// 7
	{1, 1, 1, 1, 1, 1, 1, 0},	// 8
	{1, 1, 1, 1, 0, 1, 1, 0},	// 9
	{1, 1, 1, 0, 1, 1, 1, 0},	// A
	{0, 0, 1, 1, 1, 1, 1, 0},	// b
	{1, 0, 0, 1, 1, 1, 0, 0},	// C
	{0, 1, 1, 1, 1, 0, 1, 0},	// d
	{1, 0, 0, 1, 1, 1, 1, 0},	// E
	{1, 0, 0, 0, 1, 1, 1, 0},	// F
};

#endif

