#include "../constants.h"

int OpenSRAM(){
//  switch to sram bank a
	PUSH_AF;  // push af
//  latch clock data
	LD_A(1);  // ld a, 1
	LD_addr_A(MBC3LatchClock);  // ld [MBC3LatchClock], a
//  enable sram/clock write
	LD_A(SRAM_ENABLE);  // ld a, SRAM_ENABLE
	LD_addr_A(MBC3SRamEnable);  // ld [MBC3SRamEnable], a
//  select sram bank
	POP_AF;  // pop af
	LD_addr_A(MBC3SRamBank);  // ld [MBC3SRamBank], a
	RET;  // ret

}

int CloseSRAM(){
	PUSH_AF;  // push af
	LD_A(SRAM_DISABLE);  // ld a, SRAM_DISABLE
//  reset clock latch for next time
	LD_addr_A(MBC3LatchClock);  // ld [MBC3LatchClock], a
//  disable sram/clock write
	LD_addr_A(MBC3SRamEnable);  // ld [MBC3SRamEnable], a
	POP_AF;  // pop af
	RET;  // ret

}



