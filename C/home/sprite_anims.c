#include "../constants.h"

void InitSpriteAnimStruct(void){
	SET_PC(0x3D27U);
	LD_addr_A(wSpriteAnimID);  // ld [wSpriteAnimID], a
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af

	LD_A(BANK(av_InitSpriteAnimStruct));  // ld a, BANK(_InitSpriteAnimStruct)
	RST(mBankswitch);  // rst Bankswitch
	LD_A_addr(wSpriteAnimID);  // ld a, [wSpriteAnimID]

	CALL(mv_InitSpriteAnimStruct);  // call _InitSpriteAnimStruct

	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch

	RET;  // ret

}

void ReinitSpriteAnimFrame(void){
	SET_PC(0x3D39U);
//  //  unreferenced
	LD_addr_A(wSpriteAnimID);  // ld [wSpriteAnimID], a
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af

	LD_A(BANK(av_ReinitSpriteAnimFrame));  // ld a, BANK(_ReinitSpriteAnimFrame)
	RST(mBankswitch);  // rst Bankswitch
	LD_A_addr(wSpriteAnimID);  // ld a, [wSpriteAnimID]

	CALL(mv_ReinitSpriteAnimFrame);  // call _ReinitSpriteAnimFrame

	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch

	RET;  // ret

}
