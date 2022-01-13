#include "../constants.h"

void CopyName1(void){
//  Copies the name from de to wStringBuffer2
	LD_HL(wStringBuffer2);  // ld hl, wStringBuffer2

	return CopyName2();
}

void CopyName2(void){
//  Copies the name from de to hl

loop:
	LD_A_de;  // ld a, [de]
	INC_DE;  // inc de
	LD_hli_A;  // ld [hli], a
	CP_A(0x50);  // cp "@"
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}
