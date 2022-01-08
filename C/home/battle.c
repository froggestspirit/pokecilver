#include "../constants.h"

int GetPartyParamLocation(){
	SET_PC(0x3B36U);
//  Get the location of parameter a from wCurPartyMon in hl
	PUSH_BC;  // push bc
	LD_HL(wPartyMons);  // ld hl, wPartyMons
	LD_C_A;  // ld c, a
	LD_B(0);  // ld b, 0
	ADD_HL_BC;  // add hl, bc
	LD_A_addr(wCurPartyMon);  // ld a, [wCurPartyMon]
	CALL(mGetPartyLocation);  // call GetPartyLocation
	POP_BC;  // pop bc
	RET;  // ret

}

int GetPartyLocation(){
	SET_PC(0x3B46U);
//  Add the length of a PartyMon struct to hl a times.
	LD_BC(PARTYMON_STRUCT_LENGTH);  // ld bc, PARTYMON_STRUCT_LENGTH
	JP(mAddNTimes);  // jp AddNTimes

}

int GetDexNumber(){
	SET_PC(0x3B4CU);
//  //  unreferenced
//  Probably used in gen 1 to convert index number to dex number
//  Not required in gen 2 because index number == dex number
	PUSH_HL;  // push hl
	LD_A_B;  // ld a, b
	DEC_A;  // dec a
	LD_B(0);  // ld b, 0
	ADD_HL_BC;  // add hl, bc
	LD_HL(mBaseData + BASE_DEX_NO);  // ld hl, BaseData + BASE_DEX_NO
	LD_BC(BASE_DATA_SIZE);  // ld bc, BASE_DATA_SIZE
	CALL(mAddNTimes);  // call AddNTimes
	POP_BC;  // pop bc
	LD_A(BANK(aBaseData));  // ld a, BANK(BaseData)
	CALL(mGetFarWord);  // call GetFarWord
	LD_B_L;  // ld b, l
	LD_C_H;  // ld c, h
	POP_HL;  // pop hl
	RET;  // ret

}

int UserPartyAttr(){
	SET_PC(0x3B65U);
	PUSH_AF;  // push af
	LDH_A_addr(hBattleTurn);  // ldh a, [hBattleTurn]
	AND_A_A;  // and a
	IF_NZ goto ot;  // jr nz, .ot
	POP_AF;  // pop af
	JR(mBattlePartyAttr);  // jr BattlePartyAttr

ot:
	SET_PC(0x3B6EU);
	POP_AF;  // pop af
	JR(mOTPartyAttr);  // jr OTPartyAttr

}

int OpponentPartyAttr(){
	SET_PC(0x3B71U);
	PUSH_AF;  // push af
	LDH_A_addr(hBattleTurn);  // ldh a, [hBattleTurn]
	AND_A_A;  // and a
	IF_Z goto ot;  // jr z, .ot
	POP_AF;  // pop af
	JR(mBattlePartyAttr);  // jr BattlePartyAttr

ot:
	SET_PC(0x3B7AU);
	POP_AF;  // pop af
	JR(mOTPartyAttr);  // jr OTPartyAttr

}

int BattlePartyAttr(){
	SET_PC(0x3B7DU);
//  Get attribute a from the party struct of the active battle mon.
	PUSH_BC;  // push bc
	LD_C_A;  // ld c, a
	LD_B(0);  // ld b, 0
	LD_HL(wPartyMons);  // ld hl, wPartyMons
	ADD_HL_BC;  // add hl, bc
	LD_A_addr(wCurBattleMon);  // ld a, [wCurBattleMon]
	CALL(mGetPartyLocation);  // call GetPartyLocation
	POP_BC;  // pop bc
	RET;  // ret

}

int OTPartyAttr(){
	SET_PC(0x3B8DU);
//  Get attribute a from the party struct of the active enemy mon.
	PUSH_BC;  // push bc
	LD_C_A;  // ld c, a
	LD_B(0);  // ld b, 0
	LD_HL(wOTPartyMon1Species);  // ld hl, wOTPartyMon1Species
	ADD_HL_BC;  // add hl, bc
	LD_A_addr(wCurOTMon);  // ld a, [wCurOTMon]
	CALL(mGetPartyLocation);  // call GetPartyLocation
	POP_BC;  // pop bc
	RET;  // ret

}

int ResetDamage(){
	SET_PC(0x3B9DU);
	XOR_A_A;  // xor a
	LD_addr_A(wCurDamage);  // ld [wCurDamage], a
	LD_addr_A(wCurDamage + 1);  // ld [wCurDamage + 1], a
	RET;  // ret

}

int SetPlayerTurn(){
	SET_PC(0x3BA5U);
	XOR_A_A;  // xor a
	LDH_addr_A(hBattleTurn);  // ldh [hBattleTurn], a
	RET;  // ret

}

int SetEnemyTurn(){
	SET_PC(0x3BA9U);
	LD_A(1);  // ld a, 1
	LDH_addr_A(hBattleTurn);  // ldh [hBattleTurn], a
	RET;  // ret

}

int UpdateOpponentInParty(){
	SET_PC(0x3BAEU);
	LDH_A_addr(hBattleTurn);  // ldh a, [hBattleTurn]
	AND_A_A;  // and a
	JR_Z (mUpdateEnemyMonInParty);  // jr z, UpdateEnemyMonInParty
	JR(mUpdateBattleMonInParty);  // jr UpdateBattleMonInParty

}

int UpdateUserInParty(){
	SET_PC(0x3BB5U);
	LDH_A_addr(hBattleTurn);  // ldh a, [hBattleTurn]
	AND_A_A;  // and a
	JR_Z (mUpdateBattleMonInParty);  // jr z, UpdateBattleMonInParty
	JR(mUpdateEnemyMonInParty);  // jr UpdateEnemyMonInParty

}

int UpdateBattleMonInParty(){
	SET_PC(0x3BBCU);
//  Update level, status, current HP

	LD_A_addr(wCurBattleMon);  // ld a, [wCurBattleMon]

	return UpdateBattleMon();
}

int UpdateBattleMon(){
	SET_PC(0x3BBFU);
	LD_HL(wPartyMon1Level);  // ld hl, wPartyMon1Level
	CALL(mGetPartyLocation);  // call GetPartyLocation

	LD_D_H;  // ld d, h
	LD_E_L;  // ld e, l
	LD_HL(wBattleMonLevel);  // ld hl, wBattleMonLevel
	LD_BC(wBattleMonMaxHP - wBattleMonLevel);  // ld bc, wBattleMonMaxHP - wBattleMonLevel
	JP(mCopyBytes);  // jp CopyBytes

}

int UpdateEnemyMonInParty(){
	SET_PC(0x3BD0U);
//  Update level, status, current HP

//  No wildmons.
	LD_A_addr(wBattleMode);  // ld a, [wBattleMode]
	DEC_A;  // dec a
	RET_Z ;  // ret z

	LD_A_addr(wCurOTMon);  // ld a, [wCurOTMon]
	LD_HL(wOTPartyMon1Level);  // ld hl, wOTPartyMon1Level
	CALL(mGetPartyLocation);  // call GetPartyLocation

	LD_D_H;  // ld d, h
	LD_E_L;  // ld e, l
	LD_HL(wEnemyMonLevel);  // ld hl, wEnemyMonLevel
	LD_BC(wEnemyMonMaxHP - wEnemyMonLevel);  // ld bc, wEnemyMonMaxHP - wEnemyMonLevel
	JP(mCopyBytes);  // jp CopyBytes

}

int RefreshBattleHuds(){
	SET_PC(0x3BE9U);
	CALL(mUpdateBattleHuds);  // call UpdateBattleHuds
	LD_C(3);  // ld c, 3
	CALL(mDelayFrames);  // call DelayFrames
	JP(mWaitBGMap);  // jp WaitBGMap

}

int UpdateBattleHuds(){
	SET_PC(0x3BF4U);
	FARCALL(aUpdatePlayerHUD);  // farcall UpdatePlayerHUD
	FARCALL(aUpdateEnemyHUD);  // farcall UpdateEnemyHUD
	RET;  // ret

// INCLUDE "home/battle_vars.asm"

	return FarCopyRadioText();
}

int FarCopyRadioText(){
	SET_PC(0x3CB0U);
	INC_HL;  // inc hl
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A_hli;  // ld a, [hli]
	LD_E_A;  // ld e, a
	LD_A_hli;  // ld a, [hli]
	LD_D_A;  // ld d, a
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a
	LD_A_E;  // ld a, e
	LD_L_A;  // ld l, a
	LD_A_D;  // ld a, d
	LD_H_A;  // ld h, a
	LD_DE(wRadioText);  // ld de, wRadioText
	LD_BC(2 * SCREEN_WIDTH);  // ld bc, 2 * SCREEN_WIDTH
	CALL(mCopyBytes);  // call CopyBytes
	POP_AF;  // pop af
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a
	RET;  // ret


}

int StdBattleTextbox(){
	SET_PC(0x3CD2U);
//  Open a textbox and print battle text at 20:hl.

	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af

	LD_A(BANK(aBattleText));  // ld a, BANK(BattleText)
	RST(mBankswitch);  // rst Bankswitch

	CALL(mPrintText);  // call PrintText

	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret

}

int GetBattleAnimPointer(){
	SET_PC(0x3CDEU);
	LD_A(BANK(aBattleAnimations));  // ld a, BANK(BattleAnimations)
	RST(mBankswitch);  // rst Bankswitch

	LD_A_hli;  // ld a, [hli]
	LD_addr_A(wBattleAnimAddress);  // ld [wBattleAnimAddress], a
	LD_A_hl;  // ld a, [hl]
	LD_addr_A(wBattleAnimAddress + 1);  // ld [wBattleAnimAddress + 1], a

// ; ClearBattleAnims is the only function that calls this...
	LD_A(BANK(aClearBattleAnims));  // ld a, BANK(ClearBattleAnims)
	RST(mBankswitch);  // rst Bankswitch

	RET;  // ret

}

int GetBattleAnimByte(){
	SET_PC(0x3CEDU);
	PUSH_HL;  // push hl
	PUSH_DE;  // push de

	LD_HL(wBattleAnimAddress);  // ld hl, wBattleAnimAddress
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]

	LD_A(BANK(aBattleAnimations));  // ld a, BANK(BattleAnimations)
	RST(mBankswitch);  // rst Bankswitch

	LD_A_de;  // ld a, [de]
	LD_addr_A(wBattleAnimByte);  // ld [wBattleAnimByte], a
	INC_DE;  // inc de

	LD_A(BANK(aBattleAnimCommands));  // ld a, BANK(BattleAnimCommands)
	RST(mBankswitch);  // rst Bankswitch

	LD_hl_D;  // ld [hl], d
	DEC_HL;  // dec hl
	LD_hl_E;  // ld [hl], e

	POP_DE;  // pop de
	POP_HL;  // pop hl

	LD_A_addr(wBattleAnimByte);  // ld a, [wBattleAnimByte]
	RET;  // ret

}

int PushLYOverrides(){
	SET_PC(0x3D09U);
	LDH_A_addr(hLCDCPointer);  // ldh a, [hLCDCPointer]
	AND_A_A;  // and a
	RET_Z ;  // ret z

	LD_A(LOW(wLYOverridesBackup));  // ld a, LOW(wLYOverridesBackup)
	LD_addr_A(wRequested2bppSource);  // ld [wRequested2bppSource], a
	LD_A(HIGH(wLYOverridesBackup));  // ld a, HIGH(wLYOverridesBackup)
	LD_addr_A(wRequested2bppSource + 1);  // ld [wRequested2bppSource + 1], a

	LD_A(LOW(wLYOverrides));  // ld a, LOW(wLYOverrides)
	LD_addr_A(wRequested2bppDest);  // ld [wRequested2bppDest], a
	LD_A(HIGH(wLYOverrides));  // ld a, HIGH(wLYOverrides)
	LD_addr_A(wRequested2bppDest + 1);  // ld [wRequested2bppDest + 1], a

	LD_A((wLYOverridesEnd - wLYOverrides) / LEN_2BPP_TILE);  // ld a, (wLYOverridesEnd - wLYOverrides) / LEN_2BPP_TILE
	LD_addr_A(wRequested2bppSize);  // ld [wRequested2bppSize], a
	RET;  // ret

}
