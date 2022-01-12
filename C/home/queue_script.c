#include "../constants.h"

int QueueScript(){
	SET_PC(0x341FU);
//  Push pointer hl in the current bank to wQueuedScriptBank.
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]

	return FarQueueScript();
}

int FarQueueScript(){
	SET_PC(0x3421U);
//  Push pointer a:hl to wQueuedScriptBank.
	LD_addr_A(wQueuedScriptBank);  // ld [wQueuedScriptBank], a
	LD_A_L;  // ld a, l
	LD_addr_A(wQueuedScriptAddr);  // ld [wQueuedScriptAddr], a
	LD_A_H;  // ld a, h
	LD_addr_A(wQueuedScriptAddr + 1);  // ld [wQueuedScriptAddr + 1], a
	RET;  // ret

}

