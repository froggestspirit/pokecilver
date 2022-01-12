#include "../constants.h"

int IsInArray(){
	SET_PC(0x3184U);
//  Find value a for every de bytes in array hl.
//  Return index in b and carry if found.

	LD_B(0);  // ld b, 0
	LD_C_A;  // ld c, a

loop:
	SET_PC(0x3187U);
	LD_A_hl;  // ld a, [hl]
	CP_A(-1);  // cp -1
	IF_Z goto NotInArray;  // jr z, .NotInArray
	CP_A_C;  // cp c
	IF_Z goto InArray;  // jr z, .InArray
	INC_B;  // inc b
	ADD_HL_DE;  // add hl, de
	goto loop;  // jr .loop


NotInArray:
	SET_PC(0x3193U);
	AND_A_A;  // and a
	RET;  // ret


InArray:
	SET_PC(0x3195U);
	SCF;  // scf
	RET;  // ret

}

int SkipNames(){
	SET_PC(0x3197U);
//  Skip a names.
	LD_BC(NAME_LENGTH);  // ld bc, NAME_LENGTH
	AND_A_A;  // and a
	RET_Z ;  // ret z

loop:
	SET_PC(0x319CU);
	ADD_HL_BC;  // add hl, bc
	DEC_A;  // dec a
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

int AddNTimes(){
	SET_PC(0x31A1U);
//  Add bc * a to hl.
	AND_A_A;  // and a
	RET_Z ;  // ret z

loop:
	SET_PC(0x31A3U);
	ADD_HL_BC;  // add hl, bc
	DEC_A;  // dec a
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

