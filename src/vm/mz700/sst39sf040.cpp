/*
	Author : Christophe.Avoinne
	Date   : 2019.10.01 -

	[ 512KB nor-flash memory ]
*/

#include "sst39sf040.h"

#define DATA_SIZE	128*4096
#define ADDR_MASK	(DATA_SIZE - 1)

void SST39SF040::initialize()
{
	// init memory
	data_buffer = (uint8_t *)malloc(DATA_SIZE);
	memset(data_buffer, 255, DATA_SIZE);
	modified = false;
	wc = WC1_XXXXYY;
	software_id_entry = false;
	busy = 0;

	// load SST39SF040 image
	FILEIO* fio = new FILEIO();
	if(fio->Fopen(create_local_path(_T("SST39SF040.BIN")), FILEIO_READ_BINARY)) {
		fio->Fread(data_buffer, DATA_SIZE, 1);
		fio->Fclose();
	}
	delete fio;

	if (false) {
		log = new FILEIO();
		log->Fopen(create_local_path(_T("SST39SF040.TXT")), FILEIO_READ_WRITE_NEW_ASCII);
		log->Fprintf("%s\n", __func__);
		log->Fflush();
	}
	else {
		log = 0;
	}
}

void SST39SF040::release()
{
	if(modified) {
		FILEIO* fio = new FILEIO();
		if(fio->Fopen(create_local_path(_T("SST39SF040.BIN")), FILEIO_WRITE_BINARY)) {
			fio->Fwrite(data_buffer, DATA_SIZE, 1);
			fio->Fclose();
		}
		delete fio;
	}
	
	// release memory
	free(data_buffer);

	if (log) {
		log->Fprintf("%s\n", __func__);
		log->Fclose();
		delete log;
		log = 0;
	}
}

void SST39SF040::reset()
{
	wc = WC1_XXXXYY;
	software_id_entry = false;
	busy = 0;
}

void SST39SF040::write_data8(uint32_t addr, uint32_t data)
{
	if (log) log->Fprintf("WR 0x%08x <- 0x%02x (MS:%d)\n", addr, data, wc);
#if 0 // ALLOW DIRECT WRITING SO EMULATOR MAY PREPARE THE CONTENTS
	if (busy) {
		return;
	}
	uint32_t code = (addr << 8) | (data & 255);
	switch (wc) {
	case WC1_XXXXYY:
		if (code == 0x5555AA) {
			wc = WC1_5555AA;
		}
		break;

	case WC1_5555AA:
		if (code == 0x2AAA55) {
			wc = WC2_2AAA55;
			break;
		} 
		wc = WC1_XXXXYY;
		break;

	case WC2_2AAA55:
		switch (code) {
		case 0x555580:
			wc = WC3_555580;
			break;
		case 0x555590:
			software_id_entry = true;
			wc = WC1_XXXXYY;
			if (log) log->Fprintf("EX SOFTWARE ID ENTRY\n");
			if (log) log->Fflush();
			break;
		case 0x5555A0:
			wc = WC3_5555A0;
			break;
		case 0x5555F0:
			software_id_entry = false;
			wc = WC1_XXXXYY;
			if (log) log->Fprintf("EX SOFTWARE ID EXIT\n");
			if (log) log->Fflush();
			break;
		default:
			wc = WC1_XXXXYY;
			break;
		}
		break;

	case WC3_555580:
		if (code == 0x5555AA) {
			wc = WC4_5555AA;
			break;
		}		
		wc = WC1_XXXXYY;
		break;

	case WC3_555590:
		if (code == 0x5555AA) {
			wc = WC4_5555AA;
			break;
		}
		else if (data == 0xF0) {
			software_id_entry = false;
			wc = WC1_XXXXYY;
			if (log) log->Fprintf("EX SOFTWARE ID EXIT\n");
			if (log) log->Fflush();
			break;
		}
		break;

	case WC3_5555A0:
		modified = true;
		data_buffer[addr & ADDR_MASK] = uint8_t(data);
		busy = 4;
		wc = WC1_XXXXYY;
		if (log) log->Fprintf("EX BYTE-PROGRAM: [0x%08x] = 0x%02x\n", addr, data);
		if (log) log->Fflush();
		break;

	case WC4_5555AA:
		if (code == 0x2AAA55) {
			wc = WC5_2AAA55;
			break;
		}
		wc = WC1_XXXXYY;
		break;

	case WC5_2AAA55:
		if (code == 0x555510) {
			busy = 20000;
			memset(data_buffer, 0xFF, DATA_SIZE);
			modified = true;
			wc = WC1_XXXXYY;
			if (log) log->Fprintf("EX CHIP-ERASE\n");
			if (log) log->Fflush();
			break;
		}
		else if (data == 0x30) {
			busy = 5000;
			memset(data_buffer+(addr & (ADDR_MASK ^ 0x0FFF)), 0xFF, 0x1000);
			modified = true;
			wc = WC1_XXXXYY;
			if (log) log->Fprintf("EX SECTOR-ERASE: 0x%08x\n", addr);
			if (log) log->Fflush();
			break;
		}
		wc = WC1_XXXXYY;
		break;
	}
#else
	modified = true;
	data_buffer[addr & ADDR_MASK] = uint8_t(data);
#endif
}

uint32_t SST39SF040::read_data8(uint32_t addr)
{
	uint32_t byte = uint32_t(data_buffer[addr & ADDR_MASK]);
	if (busy) {
		uint32_t result = (byte ^ 0x80) ^ ((--busy & 1) << 6);
		if (log) log->Fprintf("RD 0x%08x -> 0x%02x (0x%02x), BUSY: %d\n", addr, result, byte, busy);
		return result;
	}
	if (software_id_entry) {
		switch (addr) {
		case 0:
			if (log) log->Fprintf("RD 0x%08x -> 0xBF (SIE mode)\n", addr);
			if (log) log->Fflush();
			return 0xBF;
		case 1:
			if (log) log->Fprintf("RD 0x%08x -> 0xB7 (SIE mode)\n", addr);
			if (log) log->Fflush();
			return 0xB7;
		default:
			if (log) log->Fprintf("RD 0x%08x -> 0xFF (SIE mode)\n", addr);
			if (log) log->Fflush();
			return 0xff;
		}
	}
	else {
		if (log) log->Fprintf("RD 0x%08x -> 0x%02x\n", addr, byte);
	}
	return byte;
}

#define STATE_VERSION	1

bool SST39SF040::process_state(FILEIO* state_fio, bool loading)
{
	if(!state_fio->StateCheckUint32(STATE_VERSION)) {
		return false;
	}
	if(!state_fio->StateCheckInt32(this_device_id)) {
		return false;
	}
	state_fio->StateArray(data_buffer, DATA_SIZE, 1);
	state_fio->StateValue(modified);
	state_fio->StateValue(software_id_entry);
	state_fio->StateValue(busy);
	return true;
}

