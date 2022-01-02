#include "../constants.h"

int InitString(){
//  Init a string of length c.
	PUSH_HL;  // push hl
	JR(mv_InitString);  // jr _InitString

}

int InitName(){
//  Intended for names, so this function is limited to ten characters.
	PUSH_HL;  // push hl
	LD_C(NAME_LENGTH - 1);  // ld c, NAME_LENGTH - 1
	return mv_InitString;
}

int v_InitString(){
//  if the string pointed to by hl is empty (defined as "zero or more spaces
//  followed by a null"), then initialize it to the string pointed to by de.
	PUSH_BC;  // push bc

loop:
	SET_PC(0x2FBBU);
	LD_A_hli;  // ld a, [hli]
	CP_A("@");  // cp "@"
	IF_Z goto blank;  // jr z, .blank
	CP_A(" ");  // cp " "
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
	CALL(mCopyBytes);  // call CopyBytes
	RET;  // ret


notblank:
	SET_PC(0x2FD2U);
	POP_BC;  // pop bc
	POP_HL;  // pop hl
	RET;  // ret

}
