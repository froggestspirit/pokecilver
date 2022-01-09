#include "../constants.h"

int InitString(){
	SET_PC(0x2FB4U);
//  Init a string of length c.
	PUSH_HL;  // push hl
	JR(mv_InitString);  // jr _InitString

}

int InitName(){
	SET_PC(0x2FB7U);
//  Intended for names, so this function is limited to ten characters.
	PUSH_HL;  // push hl
	LD_C(NAME_LENGTH - 1);  // ld c, NAME_LENGTH - 1
	return v_InitString();
}

int v_InitString(){
	SET_PC(0x2FBAU);
//  if the string pointed to by hl is empty (defined as "zero or more spaces
//  followed by a null"), then initialize it to the string pointed to by de.
	PUSH_BC;  // push bc

loop:
	SET_PC(0x2FBBU);
	LD_A_hli;  // ld a, [hli]
	CP_A(0x50);  // cp "@"
	IF_Z goto blank;  // jr z, .blank
	CP_A(0x7f);  // cp " "
	IF_NZ goto notblank;  // jr nz, .notblank
	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop

blank:
	SET_PC(0x2FC7U);
	POP_BC;  // pop bc
	LD_L_E;  // ld l, e
	LD_H_D;  // ld h, d
	POP_DE;  // pop de
	LD_B(0);  // ld b, 0
	INC_C;  // inc c
	CCALL(aCopyBytes);  // call CopyBytes
	RET;  // ret


notblank:
	SET_PC(0x2FD2U);
	POP_BC;  // pop bc
	POP_HL;  // pop hl
	RET;  // ret

}
