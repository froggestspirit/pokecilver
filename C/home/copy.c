#include "../constants.h"

int CopyBytes(){
	SET_PC(0x3118U);
//  copy bc bytes from hl to de
	INC_B;  // inc b ; we bail the moment b hits 0, so include the last run
	INC_C;  // inc c ; same thing; include last byte
	goto HandleLoop;  // jr .HandleLoop

CopyByte:
	SET_PC(0x311CU);
	LD_A_hli;  // ld a, [hli]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de

HandleLoop:
	SET_PC(0x311FU);
	DEC_C;  // dec c
	IF_NZ goto CopyByte;  // jr nz, .CopyByte
	DEC_B;  // dec b
	IF_NZ goto CopyByte;  // jr nz, .CopyByte
	RET;  // ret

}

int GetFarByte(){
	SET_PC(0x3126U);
//  retrieve a single byte from a:hl, and return it in a.
// ; bankswitch to new bank
	LD_addr_A(wTempBank);  // ld [wTempBank], a
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A_addr(wTempBank);  // ld a, [wTempBank]
	RST(mBankswitch);  // rst Bankswitch

// ; get byte from new bank
	LD_A_hl;  // ld a, [hl]
	LD_addr_A(wFarByte);  // ld [wFarByte], a

// ; bankswitch to previous bank
	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch

// ; return retrieved value in a
	LD_A_addr(wFarByte);  // ld a, [wFarByte]
	RET;  // ret

}

int GetFarWord(){
	SET_PC(0x313AU);
//  retrieve a halfword from a:hl, and return it in hl.
// ; bankswitch to new bank
	LD_addr_A(wTempBank);  // ld [wTempBank], a
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A_addr(wTempBank);  // ld a, [wTempBank]
	RST(mBankswitch);  // rst Bankswitch

// ; get halfword from new bank, put it in hl
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a

// ; bankswitch to previous bank and return
	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret

}

int ByteFill(){
	SET_PC(0x314AU);
//  fill bc bytes with the value of a, starting at hl
	INC_B;  // inc b ; we bail the moment b hits 0, so include the last run
	INC_C;  // inc c ; same thing; include last byte
	goto HandleLoop;  // jr .HandleLoop

PutByte:
	SET_PC(0x314EU);
	LD_hli_A;  // ld [hli], a

HandleLoop:
	SET_PC(0x314FU);
	DEC_C;  // dec c
	IF_NZ goto PutByte;  // jr nz, .PutByte
	DEC_B;  // dec b
	IF_NZ goto PutByte;  // jr nz, .PutByte
	RET;  // ret

}
