#include "../constants.h"

int CountSetBits(){
//  Count the number of set bits in b bytes starting from hl.
//  Return in a, c and [wNumSetBits].
	LD_C(0);  // ld c, 0

_next:
	SET_PC(0x35A0U);
	LD_A_hli;  // ld a, [hli]
	LD_E_A;  // ld e, a
	LD_D(8);  // ld d, 8


_count:
	SET_PC(0x35A4U);
	SRL_E;  // srl e
	LD_A(0);  // ld a, 0
	ADC_A_C;  // adc c
	LD_C_A;  // ld c, a
	DEC_D;  // dec d
	IF_NZ goto _count;  // jr nz, .count

	DEC_B;  // dec b
	IF_NZ goto _next;  // jr nz, .next

	LD_A_C;  // ld a, c
	LD_addr_A(wNumSetBits);  // ld [wNumSetBits], a
	RET;  // ret

}

int GetWeekday(){
	LD_A_addr(wCurDay);  // ld a, [wCurDay]

_mod:
	SET_PC(0x35B8U);
	SUB_A(7);  // sub 7
	IF_NC goto _mod;  // jr nc, .mod
	ADD_A(7);  // add 7
	RET;  // ret

}

int SetSeenAndCaughtMon(){
	PUSH_AF;  // push af
	LD_C_A;  // ld c, a
	LD_HL(wPokedexCaught);  // ld hl, wPokedexCaught
	LD_B(SET_FLAG);  // ld b, SET_FLAG
	CALL(mPokedexFlagAction);  // call PokedexFlagAction
	POP_AF;  // pop af
// ; fallthrough

	return mSetSeenMon;
}

int SetSeenMon(){
	LD_C_A;  // ld c, a
	LD_HL(wPokedexSeen);  // ld hl, wPokedexSeen
	LD_B(SET_FLAG);  // ld b, SET_FLAG
	JR(mPokedexFlagAction);  // jr PokedexFlagAction

}

int CheckCaughtMon(){
	LD_C_A;  // ld c, a
	LD_HL(wPokedexCaught);  // ld hl, wPokedexCaught
	LD_B(CHECK_FLAG);  // ld b, CHECK_FLAG
	JR(mPokedexFlagAction);  // jr PokedexFlagAction

}

int CheckSeenMon(){
	LD_C_A;  // ld c, a
	LD_HL(wPokedexSeen);  // ld hl, wPokedexSeen
	LD_B(CHECK_FLAG);  // ld b, CHECK_FLAG
// ; fallthrough

	return mPokedexFlagAction;
}

int PokedexFlagAction(){
	LD_D(0);  // ld d, 0
	PREDEF(pSmallFarFlagAction);  // predef SmallFarFlagAction
	LD_A_C;  // ld a, c
	AND_A_A;  // and a
	RET;  // ret

}