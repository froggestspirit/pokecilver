#include "../constants.h"

int CopyBytes(){
//  copy bc bytes from hl to de
	INC_B;  // inc b ; we bail the moment b hits 0, so include the last run
	INC_C;  // inc c ; same thing; include last byte
	goto HandleLoop;  // jr .HandleLoop

CopyByte:
	LD_A_hli;  // ld a, [hli]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de

HandleLoop:
	DEC_C;  // dec c
	IF_NZ goto CopyByte;  // jr nz, .CopyByte
	DEC_B;  // dec b
	IF_NZ goto CopyByte;  // jr nz, .CopyByte
	RET;  // ret

}

int GetFarByte(){
//  retrieve a single byte from a:hl, and return it in a.
// ; bankswitch to new bank
	LD_addr_A(wTempBank);  // ld [wTempBank], a
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A_addr(wTempBank);  // ld a, [wTempBank]
	Bankswitch();  // rst Bankswitch

// ; get byte from new bank
	LD_A_hl;  // ld a, [hl]
	LD_addr_A(wFarByte);  // ld [wFarByte], a

// ; bankswitch to previous bank
	POP_AF;  // pop af
	Bankswitch();  // rst Bankswitch

// ; return retrieved value in a
	LD_A_addr(wFarByte);  // ld a, [wFarByte]
	RET;  // ret

}

int GetFarWord(){
//  retrieve a halfword from a:hl, and return it in hl.
// ; bankswitch to new bank
	LD_addr_A(wTempBank);  // ld [wTempBank], a
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A_addr(wTempBank);  // ld a, [wTempBank]
	Bankswitch();  // rst Bankswitch

// ; get halfword from new bank, put it in hl
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a

// ; bankswitch to previous bank and return
	POP_AF;  // pop af
	Bankswitch();  // rst Bankswitch
	RET;  // ret

}

int ByteFill(){
//  fill bc bytes with the value of a, starting at hl
	INC_B;  // inc b ; we bail the moment b hits 0, so include the last run
	INC_C;  // inc c ; same thing; include last byte
	goto HandleLoop;  // jr .HandleLoop

PutByte:
	LD_hli_A;  // ld [hli], a

HandleLoop:
	DEC_C;  // dec c
	IF_NZ goto PutByte;  // jr nz, .PutByte
	DEC_B;  // dec b
	IF_NZ goto PutByte;  // jr nz, .PutByte
	RET;  // ret

}
