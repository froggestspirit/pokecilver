#include "../constants.h"

int RefreshScreen(){
	CALL(mClearWindowData);  // call ClearWindowData
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A(BANK(aReanchorBGMap_NoOAMUpdate));  // ld a, BANK(ReanchorBGMap_NoOAMUpdate) ; aka BANK(LoadFonts_NoOAMUpdate)
	RST(mBankswitch);  // rst Bankswitch

	CALL(mReanchorBGMap_NoOAMUpdate);  // call ReanchorBGMap_NoOAMUpdate
	CALL(mv_OpenAndCloseMenu_HDMATransferTilemapAndAttrmap);  // call _OpenAndCloseMenu_HDMATransferTilemapAndAttrmap
	CALL(mLoadFonts_NoOAMUpdate);  // call LoadFonts_NoOAMUpdate

	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret

}

int CloseText(){
	LDH_A_addr(hOAMUpdate);  // ldh a, [hOAMUpdate]
	PUSH_AF;  // push af
	LD_A(0x1);  // ld a, $1
	LDH_addr_A(hOAMUpdate);  // ldh [hOAMUpdate], a

	CALL(mCloseText_CloseText);  // call .CloseText

	POP_AF;  // pop af
	LDH_addr_A(hOAMUpdate);  // ldh [hOAMUpdate], a
	RET;  // ret


CloseText:
	SET_PC(0x2EA1U);
	CALL(mClearWindowData);  // call ClearWindowData
	XOR_A_A;  // xor a
	LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a
	CALL(mOverworldTextModeSwitch);  // call OverworldTextModeSwitch
	CALL(mv_OpenAndCloseMenu_HDMATransferTilemapAndAttrmap);  // call _OpenAndCloseMenu_HDMATransferTilemapAndAttrmap
	XOR_A_A;  // xor a
	LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a
	CALL(mSafeUpdateSprites);  // call SafeUpdateSprites
	LD_A(0x90);  // ld a, $90
	LDH_addr_A(hWY);  // ldh [hWY], a
	FARCALL(av_ClearSprites);  // farcall _ClearSprites
	CALL(mUpdatePlayerSprite);  // call UpdatePlayerSprite
	LD_HL(wEnteredMapFromContinue);  // ld hl, wEnteredMapFromContinue
	RES_hl(7);  // res 7, [hl]
	CALL(mResetBGWindow);  // call ResetBGWindow
	RET;  // ret

}

int OpenText(){
	CALL(mClearWindowData);  // call ClearWindowData
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A(BANK(aReanchorBGMap_NoOAMUpdate));  // ld a, BANK(ReanchorBGMap_NoOAMUpdate) ; aka BANK(LoadFonts_NoOAMUpdate)
	RST(mBankswitch);  // rst Bankswitch

	CALL(mReanchorBGMap_NoOAMUpdate);  // call ReanchorBGMap_NoOAMUpdate ; clear bgmap
	CALL(mSpeechTextbox);  // call SpeechTextbox
	CALL(mv_OpenAndCloseMenu_HDMATransferTilemapAndAttrmap);  // call _OpenAndCloseMenu_HDMATransferTilemapAndAttrmap ; anchor bgmap
	CALL(mLoadFonts_NoOAMUpdate);  // call LoadFonts_NoOAMUpdate ; load font
	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch

	RET;  // ret

}

int v_OpenAndCloseMenu_HDMATransferTilemapAndAttrmap(){
	LDH_A_addr(hOAMUpdate);  // ldh a, [hOAMUpdate]
	PUSH_AF;  // push af
	LD_A(0x1);  // ld a, $1
	LDH_addr_A(hOAMUpdate);  // ldh [hOAMUpdate], a

	CALL(mCGBOnly_CopyTilemapAtOnce);  // call CGBOnly_CopyTilemapAtOnce

	POP_AF;  // pop af
	LDH_addr_A(hOAMUpdate);  // ldh [hOAMUpdate], a
	RET;  // ret

}

int SafeUpdateSprites(){
	LDH_A_addr(hOAMUpdate);  // ldh a, [hOAMUpdate]
	PUSH_AF;  // push af
	LDH_A_addr(hBGMapMode);  // ldh a, [hBGMapMode]
	PUSH_AF;  // push af
	XOR_A_A;  // xor a
	LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a
	LD_A(0x1);  // ld a, $1
	LDH_addr_A(hOAMUpdate);  // ldh [hOAMUpdate], a

	CALL(mUpdateSprites);  // call UpdateSprites

	XOR_A_A;  // xor a
	LDH_addr_A(hOAMUpdate);  // ldh [hOAMUpdate], a
	CALL(mDelayFrame);  // call DelayFrame
	POP_AF;  // pop af
	LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a
	POP_AF;  // pop af
	LDH_addr_A(hOAMUpdate);  // ldh [hOAMUpdate], a
	RET;  // ret

}

int SetCarryFlag(){  //  unreferenced
	SCF;  // scf
	RET;  // ret

}
