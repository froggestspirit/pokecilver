#include "../constants.h"

//  LCD handling

int LCD(){
	PUSH_AF;  // push af
	LDH_A_addr(hLCDCPointer);  // ldh a, [hLCDCPointer]
	AND_A_A;  // and a
	IF_Z goto done;  // jr z, .done

	PUSH_HL;  // push hl
	LDH_A_addr(rLY);  // ldh a, [rLY]
	LD_L_A;  // ld l, a
	LD_H(HIGH(wLYOverrides));  // ld h, HIGH(wLYOverrides)
	LD_H_hl;  // ld h, [hl]
	LDH_A_addr(hLCDCPointer);  // ldh a, [hLCDCPointer]
	LD_L_A;  // ld l, a
	LD_A_H;  // ld a, h
	LD_H(HIGH(rSCY));  // ld h, HIGH(rSCY)
	LD_hl_A;  // ld [hl], a
	POP_HL;  // pop hl


done:
	POP_AF;  // pop af
	RET;  // ret

}

int DisableLCD(){
//  Turn the LCD off

//  Don't need to do anything if the LCD is already off
	LDH_A_addr(rLCDC);  // ldh a, [rLCDC]
	BIT_A(rLCDC_ENABLE);  // bit rLCDC_ENABLE, a
	RET_Z ;  // ret z

	XOR_A_A;  // xor a
	LDH_addr_A(rIF);  // ldh [rIF], a
	LDH_A_addr(rIE);  // ldh a, [rIE]
	LD_B_A;  // ld b, a

//  Disable VBlank
	RES_A(VBLANK);  // res VBLANK, a
	LDH_addr_A(rIE);  // ldh [rIE], a


//  Wait until VBlank would normally happen
	LDH_A_addr(rLY);  // ldh a, [rLY]
	CP_A(LY_VBLANK + 1);  // cp LY_VBLANK + 1
	IF_NZ goto wait;  // jr nz, .wait
wait:  // hack to avoid hanging

	LDH_A_addr(rLCDC);  // ldh a, [rLCDC]
	AND_A(0xff ^ (1 << rLCDC_ENABLE));  // and $ff ^ (1 << rLCDC_ENABLE)
	LDH_addr_A(rLCDC);  // ldh [rLCDC], a

	XOR_A_A;  // xor a
	LDH_addr_A(rIF);  // ldh [rIF], a
	LD_A_B;  // ld a, b
	LDH_addr_A(rIE);  // ldh [rIE], a
	RET;  // ret

}

int EnableLCD(){
	LDH_A_addr(rLCDC);  // ldh a, [rLCDC]
	SET_A(rLCDC_ENABLE);  // set rLCDC_ENABLE, a
	LDH_addr_A(rLCDC);  // ldh [rLCDC], a
	RET;  // ret

}



