#include "../constants.h"

int FarCall(){
	JP(mFarCall_hl);  // jp FarCall_hl
}

int Bankswitch(){
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a
	RET;  // ret
}

int JumpTable(){
	PUSH_DE;  // push de
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
	ADD_HL_DE;  // add hl, de
	ADD_HL_DE;  // add hl, de
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	POP_DE;  // pop de
	JP_hl;  // jp hl
}

int IHandler(){
	PUSH_AF;  // push af
	PUSH_HL;  // push hl
	LD_HL(0xFF44);  // ld hl, $FF44
	LD_A(144);  // ld a, 144

	SET_PC(0x006AU);
	CP_A_hl;  // cp [hl]
	IF_NC goto wait;  // jr nc, .wait
wait:  // hack to avoid hanging
	POP_HL;  // pop hl
	POP_AF;  // pop af
	CALL(mVBlank);  // call VBlank
	CCALL(aLCD);  // call LCD
	CALL(mSerial);  // call Serial
	CCALL(aJoypad);  // call Joypad
	RET;  // ret
}

int Start(){
	NOP;  // nop
	JP(mv_Start);  // jp _Start
}
