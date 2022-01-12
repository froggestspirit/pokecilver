#include "../constants.h"

int Predef(){
	SET_PC(0x2E47U);
//  Call predefined function a.
//  Preserves bc, de, hl and f.

	LD_addr_A(wPredefID);  // ld [wPredefID], a
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af

	LD_A(BANK(aGetPredefPointer));  // ld a, BANK(GetPredefPointer)
	RST(mBankswitch);  // rst Bankswitch
	CALL(mGetPredefPointer);  // call GetPredefPointer ; stores hl in wPredefHL

//  Switch to the new function's bank
	RST(mBankswitch);  // rst Bankswitch

//  Instead of directly calling stuff,
//  push it to the stack in reverse.

	LD_HL(mPredef_Return);  // ld hl, .Return
	PUSH_HL;  // push hl

//  Call the Predef function
	LD_A_addr(wPredefAddress);  // ld a, [wPredefAddress]
	LD_H_A;  // ld h, a
	LD_A_addr(wPredefAddress + 1);  // ld a, [wPredefAddress + 1]
	LD_L_A;  // ld l, a
	PUSH_HL;  // push hl

//  Get hl back
	LD_A_addr(wPredefHL);  // ld a, [wPredefHL]
	LD_H_A;  // ld h, a
	LD_A_addr(wPredefHL + 1);  // ld a, [wPredefHL + 1]
	LD_L_A;  // ld l, a
	RET;  // ret


Return:
	SET_PC(0x2E6AU);
//  Clean up after the Predef call

	LD_A_H;  // ld a, h
	LD_addr_A(wPredefHL);  // ld [wPredefHL], a
	LD_A_L;  // ld a, l
	LD_addr_A(wPredefHL + 1);  // ld [wPredefHL + 1], a

	POP_HL;  // pop hl
	LD_A_H;  // ld a, h
	RST(mBankswitch);  // rst Bankswitch

	LD_A_addr(wPredefHL);  // ld a, [wPredefHL]
	LD_H_A;  // ld h, a
	LD_A_addr(wPredefHL + 1);  // ld a, [wPredefHL + 1]
	LD_L_A;  // ld l, a
	RET;  // ret

}



