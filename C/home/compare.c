#include "../constants.h"

int CompareBytes(){
//  Compare c bytes at de and hl.
//  Return z if they all match.

loop:
	SET_PC(0x342DU);
	LD_A_de;  // ld a, [de]
	CP_A_hl;  // cp [hl]
	RET_NZ ;  // ret nz
	INC_DE;  // inc de
	INC_HL;  // inc hl
	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

int CompareBytesLong(){
//  Compare bc bytes at de and hl.
//  Return carry if they all match.

loop:
	SET_PC(0x3436U);
	LD_A_de;  // ld a, [de]
	CP_A_hl;  // cp [hl]
	IF_NZ goto diff;  // jr nz, .diff

	INC_DE;  // inc de
	INC_HL;  // inc hl
	DEC_BC;  // dec bc

	LD_A_B;  // ld a, b
	OR_A_C;  // or c
	IF_NZ goto loop;  // jr nz, .loop

	SCF;  // scf
	RET;  // ret


diff:
	SET_PC(0x3443U);
	AND_A_A;  // and a
	RET;  // ret

}
