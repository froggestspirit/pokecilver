#include "../constants.h"

int CopyName1(){
	SET_PC(0x3179U);
//  Copies the name from de to wStringBuffer2
	LD_HL(wStringBuffer2);  // ld hl, wStringBuffer2

	return mCopyName2;
}

int CopyName2(){
	SET_PC(0x317CU);
//  Copies the name from de to hl

loop:
	SET_PC(0x317CU);
	LD_A_de;  // ld a, [de]
	INC_DE;  // inc de
	LD_hli_A;  // ld [hli], a
	CP_A(0x50);  // cp "@"
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}
