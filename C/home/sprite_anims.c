#include "../constants.h"

int InitSpriteAnimStruct(){
	LD_addr_A(wSpriteAnimID);  // ld [wSpriteAnimID], a
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af

	LD_A(BANK(a_InitSpriteAnimStruct));  // ld a, BANK(_InitSpriteAnimStruct)
	RST(mBankswitch);  // rst Bankswitch
	LD_A_addr(wSpriteAnimID);  // ld a, [wSpriteAnimID]

	CALL(m_InitSpriteAnimStruct);  // call _InitSpriteAnimStruct

	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch

	RET;  // ret

}

int ReinitSpriteAnimFrame(){  //  unreferenced
	LD_addr_A(wSpriteAnimID);  // ld [wSpriteAnimID], a
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af

	LD_A(BANK(a_ReinitSpriteAnimFrame));  // ld a, BANK(_ReinitSpriteAnimFrame)
	RST(mBankswitch);  // rst Bankswitch
	LD_A_addr(wSpriteAnimID);  // ld a, [wSpriteAnimID]

	CALL(m_ReinitSpriteAnimFrame);  // call _ReinitSpriteAnimFrame

	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch

	RET;  // ret

}
