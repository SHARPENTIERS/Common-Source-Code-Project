/*
	Author : Christophe.Avoinne
	Date   : 2019.10.01 -

	[ 512KB nor-flash memory ]
*/

#ifndef _SST39SF040_H_
#define _SST39SF040_H_

#include "../vm.h"
#include "../../emu.h"
#include "../device.h"

class SST39SF040 : public DEVICE
{
private:
	enum WriteCycle
	{
		WC1_XXXXYY,
		WC1_5555AA, // Special Prefix Write Cycle 1
		WC1_XXXXF0, // Software ID Exit Write Cycle 1
		WC2_2AAA55, // Special Prefix Write Cycle 2
		WC3_5555A0, // Byte Program Write Cycle 3
		WC3_555580, // Chip/Sector Erase Write Cycle 3
		WC3_555590, // Software ID Entry Write Cycle 3
		WC3_5555F0, // Software ID Exit Write Cycle 3
		WC4_5555AA, // Chip/Sector Erase Write Cycle 4
		WC5_2AAA55, // Chip/Sector Erase Write Cycle 5
		WC6_SXXX30, // Sector Erase Write Cycle 6
		WC6_555510  // Chip Erase Write Cycle 6
	};

	FILEIO* log;
	uint8_t *data_buffer;
	WriteCycle wc;
	uint32_t busy;
	bool modified;
	bool software_id_entry;
	FILEIO* logmem;
public:
	SST39SF040(VM_TEMPLATE* parent_vm, EMU* parent_emu) : DEVICE(parent_vm, parent_emu)
	{
		set_device_name(_T("SST39SF040"));
	}
	~SST39SF040() {}
	
	// common functions
	void initialize();
	void release();
	void reset();
	void write_data8(uint32_t addr, uint32_t data);
	uint32_t read_data8(uint32_t addr);
	bool process_state(FILEIO* state_fio, bool loading);
};

#endif

