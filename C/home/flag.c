#include "../constants.h"

int ResetMapBufferEventFlags(){
	SET_PC(0x2F0EU);
	XOR_A_A;  // xor a
	LD_HL(wEventFlags);  // ld hl, wEventFlags
	LD_hli_A;  // ld [hli], a
	RET;  // ret

}

int ResetBikeFlags(){
	SET_PC(0x2F14U);
	XOR_A_A;  // xor a
	LD_HL(wBikeFlags);  // ld hl, wBikeFlags
	LD_hli_A;  // ld [hli], a
	LD_hl_A;  // ld [hl], a
	RET;  // ret

}

int ResetFlashIfOutOfCave(){
	SET_PC(0x2F1BU);
	LD_A_addr(wEnvironment);  // ld a, [wEnvironment]
	CP_A(ROUTE);  // cp ROUTE
	IF_Z goto outdoors;  // jr z, .outdoors
	CP_A(TOWN);  // cp TOWN
	IF_Z goto outdoors;  // jr z, .outdoors
	RET;  // ret


outdoors:
	SET_PC(0x2F27U);
	LD_HL(wStatusFlags);  // ld hl, wStatusFlags
	RES_hl(STATUSFLAGS_FLASH_F);  // res STATUSFLAGS_FLASH_F, [hl]
	RET;  // ret

}

int EventFlagAction(){
	SET_PC(0x2F2DU);
	LD_HL(wEventFlags);  // ld hl, wEventFlags
	CCALL(aFlagAction);  // call FlagAction
	RET;  // ret

}

int FlagAction(){
	SET_PC(0x2F34U);
//  Perform action b on bit de in flag array hl.

//  inputs:
//  b: function
//     0  RESET_FLAG  clear bit
//     1  SET_FLAG    set bit
//     2  CHECK_FLAG  check bit
//  de: bit number
//  hl: pointer to the flag array

// ; get index within the byte
	LD_A_E;  // ld a, e
	AND_A(7);  // and 7

// ; shift de right by three bits (get the index within memory)
	for(int rept = 0; rept < 3; rept++){
	SRL_D;  // srl d
	RR_E;  // rr e
	}
	ADD_HL_DE;  // add hl, de

// ; implement a decoder
	LD_C(1);  // ld c, 1
	RRCA;  // rrca
	IF_NC goto one;  // jr nc, .one
	RLC_C;  // rlc c

one:
	SET_PC(0x2F4BU);
	RRCA;  // rrca
	IF_NC goto two;  // jr nc, .two
	RLC_C;  // rlc c
	RLC_C;  // rlc c

two:
	SET_PC(0x2F52U);
	RRCA;  // rrca
	IF_NC goto three;  // jr nc, .three
	SWAP_C;  // swap c

three:
	SET_PC(0x2F57U);

// ; check b's value: 0, 1, 2
	LD_A_B;  // ld a, b
	CP_A(SET_FLAG);  // cp SET_FLAG
	IF_C goto clearbit;  // jr c, .clearbit ; RESET_FLAG
	IF_Z goto setbit;  // jr z, .setbit ; SET_FLAG

// ; check bit
	LD_A_hl;  // ld a, [hl]
	AND_A_C;  // and c
	LD_C_A;  // ld c, a
	RET;  // ret


setbit:
	SET_PC(0x2F62U);
// ; set bit
	LD_A_hl;  // ld a, [hl]
	OR_A_C;  // or c
	LD_hl_A;  // ld [hl], a
	RET;  // ret


clearbit:
	SET_PC(0x2F66U);
// ; clear bit
	LD_A_C;  // ld a, c
	CPL;  // cpl
	AND_A_hl;  // and [hl]
	LD_hl_A;  // ld [hl], a
	RET;  // ret

}

int CheckReceivedDex(){
	SET_PC(0x2F6BU);
	LD_DE(ENGINE_POKEDEX);  // ld de, ENGINE_POKEDEX
	LD_B(CHECK_FLAG);  // ld b, CHECK_FLAG
	FARCALL(aEngineFlagAction);  // farcall EngineFlagAction
	LD_A_C;  // ld a, c
	AND_A_A;  // and a
	RET;  // ret

}

int CheckBPressedDebug(){
	SET_PC(0x2F79U);
//  //  unreferenced
//  Used in debug ROMs to walk through walls and avoid encounters.

	LD_A_addr(wDebugFlags);  // ld a, [wDebugFlags]
	BIT_A(DEBUG_FIELD_F);  // bit DEBUG_FIELD_F, a
	RET_Z ;  // ret z

	LDH_A_addr(hJoyDown);  // ldh a, [hJoyDown]
	BIT_A(B_BUTTON_F);  // bit B_BUTTON_F, a
	RET;  // ret

}

int xor_a(){
	SET_PC(0x2F84U);
	XOR_A_A;  // xor a
	RET;  // ret

}

int xor_a_dec_a(){
	SET_PC(0x2F86U);
	XOR_A_A;  // xor a
	DEC_A;  // dec a
	RET;  // ret

}

int CheckFieldDebug(){
	SET_PC(0x2F89U);
//  //  unreferenced
	PUSH_HL;  // push hl
	LD_HL(wDebugFlags);  // ld hl, wDebugFlags
	BIT_hl(DEBUG_FIELD_F);  // bit DEBUG_FIELD_F, [hl]
	POP_HL;  // pop hl
	RET;  // ret

}

