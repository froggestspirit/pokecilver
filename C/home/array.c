#include "../constants.h"

int IsInArray(){
//  Find value a for every de bytes in array hl.
//  Return index in b and carry if found.

	LD_B(0);  // ld b, 0
	LD_C_A;  // ld c, a

loop:
	LD_A_hl;  // ld a, [hl]
	CP_A(-1);  // cp -1
	IF_Z goto NotInArray;  // jr z, .NotInArray
	CP_A_C;  // cp c
	IF_Z goto InArray;  // jr z, .InArray
	INC_B;  // inc b
	ADD_HL_DE;  // add hl, de
	goto loop;  // jr .loop


NotInArray:
	AND_A_A;  // and a
	RET;  // ret


InArray:
	SCF;  // scf
	RET;  // ret

}

int SkipNames(){
//  Skip a names.
	LD_BC(NAME_LENGTH);  // ld bc, NAME_LENGTH
	AND_A_A;  // and a
	RET_Z ;  // ret z

loop:
	ADD_HL_BC;  // add hl, bc
	DEC_A;  // dec a
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

int AddNTimes(){
//  Add bc * a to hl.
	AND_A_A;  // and a
	RET_Z ;  // ret z

loop:
	ADD_HL_BC;  // add hl, bc
	DEC_A;  // dec a
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}



