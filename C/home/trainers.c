#include "../constants.h"

int CheckTrainerBattle(){
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af

	CALL(mSwitchToMapScriptsBank);  // call SwitchToMapScriptsBank
	CALL(m_CheckTrainerBattle);  // call _CheckTrainerBattle

	POP_BC;  // pop bc
	LD_A_B;  // ld a, b
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret

}

int _CheckTrainerBattle(){
//  Check if any trainer on the map sees the player and wants to battle.

//  Skip the player object.
	LD_A(2);  // ld a, 2
	LD_DE(wMap2Object);  // ld de, wMap2Object


_loop:
	SET_PC(0x3852U);

//  Start a battle if the object:
	PUSH_AF;  // push af
	PUSH_DE;  // push de

//  Has a sprite
	LD_HL(MAPOBJECT_SPRITE);  // ld hl, MAPOBJECT_SPRITE
	ADD_HL_DE;  // add hl, de
	LD_A_hl;  // ld a, [hl]
	AND_A_A;  // and a
	IF_Z goto _next;  // jr z, .next

//  Is a trainer
	LD_HL(MAPOBJECT_COLOR);  // ld hl, MAPOBJECT_COLOR
	ADD_HL_DE;  // add hl, de
	LD_A_hl;  // ld a, [hl]
	AND_A(0xf);  // and $f
	CP_A(OBJECTTYPE_TRAINER);  // cp OBJECTTYPE_TRAINER
	IF_NZ goto _next;  // jr nz, .next

//  Is visible on the map
	LD_HL(MAPOBJECT_OBJECT_STRUCT_ID);  // ld hl, MAPOBJECT_OBJECT_STRUCT_ID
	ADD_HL_DE;  // add hl, de
	LD_A_hl;  // ld a, [hl]
	CP_A(-1);  // cp -1
	IF_Z goto _next;  // jr z, .next

//  Is facing the player...
	CALL(mGetObjectStruct);  // call GetObjectStruct
	CALL(mFacingPlayerDistance_bc);  // call FacingPlayerDistance_bc
	IF_NC goto _next;  // jr nc, .next

//  ...within their sight range
	LD_HL(MAPOBJECT_RANGE);  // ld hl, MAPOBJECT_RANGE
	ADD_HL_DE;  // add hl, de
	LD_A_hl;  // ld a, [hl]
	CP_A_B;  // cp b
	IF_C goto _next;  // jr c, .next

//  And hasn't already been beaten
	PUSH_BC;  // push bc
	PUSH_DE;  // push de
	LD_HL(MAPOBJECT_SCRIPT_POINTER);  // ld hl, MAPOBJECT_SCRIPT_POINTER
	ADD_HL_DE;  // add hl, de
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]
	LD_B(CHECK_FLAG);  // ld b, CHECK_FLAG
	CALL(mEventFlagAction);  // call EventFlagAction
	LD_A_C;  // ld a, c
	POP_DE;  // pop de
	POP_BC;  // pop bc
	AND_A_A;  // and a
	IF_Z goto _startbattle;  // jr z, .startbattle


_next:
	SET_PC(0x3897U);
	POP_DE;  // pop de
	LD_HL(MAPOBJECT_LENGTH);  // ld hl, MAPOBJECT_LENGTH
	ADD_HL_DE;  // add hl, de
	LD_D_H;  // ld d, h
	LD_E_L;  // ld e, l

	POP_AF;  // pop af
	INC_A;  // inc a
	CP_A(NUM_OBJECTS);  // cp NUM_OBJECTS
	IF_NZ goto _loop;  // jr nz, .loop
	XOR_A_A;  // xor a
	RET;  // ret


_startbattle:
	SET_PC(0x38A6U);
	POP_DE;  // pop de
	POP_AF;  // pop af
	LDH_addr_A(hLastTalked);  // ldh [hLastTalked], a
	LD_A_B;  // ld a, b
	LD_addr_A(wSeenTrainerDistance);  // ld [wSeenTrainerDistance], a
	LD_A_C;  // ld a, c
	LD_addr_A(wSeenTrainerDirection);  // ld [wSeenTrainerDirection], a
	JR(mLoadTrainer_continue);  // jr LoadTrainer_continue

}

int TalkToTrainer(){
	LD_A(1);  // ld a, 1
	LD_addr_A(wSeenTrainerDistance);  // ld [wSeenTrainerDistance], a
	LD_A(-1);  // ld a, -1
	LD_addr_A(wSeenTrainerDirection);  // ld [wSeenTrainerDirection], a

	return mLoadTrainer_continue;
}

int LoadTrainer_continue(){
	CALL(mGetMapScriptsBank);  // call GetMapScriptsBank
	LD_addr_A(wSeenTrainerBank);  // ld [wSeenTrainerBank], a

	LDH_A_addr(hLastTalked);  // ldh a, [hLastTalked]
	CALL(mGetMapObject);  // call GetMapObject

	LD_HL(MAPOBJECT_SCRIPT_POINTER);  // ld hl, MAPOBJECT_SCRIPT_POINTER
	ADD_HL_BC;  // add hl, bc
	LD_A_addr(wSeenTrainerBank);  // ld a, [wSeenTrainerBank]
	CALL(mGetFarWord);  // call GetFarWord
	LD_DE(wTempTrainer);  // ld de, wTempTrainer
	LD_BC(wTempTrainerEnd - wTempTrainer);  // ld bc, wTempTrainerEnd - wTempTrainer
	LD_A_addr(wSeenTrainerBank);  // ld a, [wSeenTrainerBank]
	CALL(mFarCopyBytes);  // call FarCopyBytes
	XOR_A_A;  // xor a
	LD_addr_A(wRunningTrainerBattleScript);  // ld [wRunningTrainerBattleScript], a
	SCF;  // scf
	RET;  // ret

}

int FacingPlayerDistance_bc(){
	PUSH_DE;  // push de
	CALL(mFacingPlayerDistance);  // call FacingPlayerDistance
	LD_B_D;  // ld b, d
	LD_C_E;  // ld c, e
	POP_DE;  // pop de
	RET;  // ret

}

int FacingPlayerDistance(){
//  Return carry if the sprite at bc is facing the player,
//  its distance in d, and its direction in e.

	LD_HL(OBJECT_NEXT_MAP_X);  // ld hl, OBJECT_NEXT_MAP_X ; x
	ADD_HL_BC;  // add hl, bc
	LD_D_hl;  // ld d, [hl]

	LD_HL(OBJECT_NEXT_MAP_Y);  // ld hl, OBJECT_NEXT_MAP_Y ; y
	ADD_HL_BC;  // add hl, bc
	LD_E_hl;  // ld e, [hl]

	LD_A_addr(wPlayerStandingMapX);  // ld a, [wPlayerStandingMapX]
	CP_A_D;  // cp d
	IF_Z goto _CheckY;  // jr z, .CheckY

	LD_A_addr(wPlayerStandingMapY);  // ld a, [wPlayerStandingMapY]
	CP_A_E;  // cp e
	IF_Z goto _CheckX;  // jr z, .CheckX

	AND_A_A;  // and a
	RET;  // ret


_CheckY:
	SET_PC(0x3905U);
	LD_A_addr(wPlayerStandingMapY);  // ld a, [wPlayerStandingMapY]
	SUB_A_E;  // sub e
	IF_Z goto _NotFacing;  // jr z, .NotFacing
	IF_NC goto _Above;  // jr nc, .Above

//  Below
	CPL;  // cpl
	INC_A;  // inc a
	LD_D_A;  // ld d, a
	LD_E(OW_UP);  // ld e, OW_UP
	goto _CheckFacing;  // jr .CheckFacing


_Above:
	SET_PC(0x3914U);
	LD_D_A;  // ld d, a
	LD_E(OW_DOWN);  // ld e, OW_DOWN
	goto _CheckFacing;  // jr .CheckFacing


_CheckX:
	SET_PC(0x3919U);
	LD_A_addr(wPlayerStandingMapX);  // ld a, [wPlayerStandingMapX]
	SUB_A_D;  // sub d
	IF_Z goto _NotFacing;  // jr z, .NotFacing
	IF_NC goto _Left;  // jr nc, .Left

//  Right
	CPL;  // cpl
	INC_A;  // inc a
	LD_D_A;  // ld d, a
	LD_E(OW_LEFT);  // ld e, OW_LEFT
	goto _CheckFacing;  // jr .CheckFacing


_Left:
	SET_PC(0x3928U);
	LD_D_A;  // ld d, a
	LD_E(OW_RIGHT);  // ld e, OW_RIGHT


_CheckFacing:
	SET_PC(0x392BU);
	CALL(mGetSpriteDirection);  // call GetSpriteDirection
	CP_A_E;  // cp e
	IF_NZ goto _NotFacing;  // jr nz, .NotFacing
	SCF;  // scf
	RET;  // ret


_NotFacing:
	SET_PC(0x3933U);
	AND_A_A;  // and a
	RET;  // ret

}

int CheckTrainerFlag(){  //  unreferenced
	PUSH_BC;  // push bc
	LD_HL(OBJECT_MAP_OBJECT_INDEX);  // ld hl, OBJECT_MAP_OBJECT_INDEX
	ADD_HL_BC;  // add hl, bc
	LD_A_hl;  // ld a, [hl]
	CALL(mGetMapObject);  // call GetMapObject
	LD_HL(MAPOBJECT_SCRIPT_POINTER);  // ld hl, MAPOBJECT_SCRIPT_POINTER
	ADD_HL_BC;  // add hl, bc
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	CALL(mGetMapScriptsBank);  // call GetMapScriptsBank
	CALL(mGetFarWord);  // call GetFarWord
	LD_D_H;  // ld d, h
	LD_E_L;  // ld e, l
	PUSH_DE;  // push de
	LD_B(CHECK_FLAG);  // ld b, CHECK_FLAG
	CALL(mEventFlagAction);  // call EventFlagAction
	POP_DE;  // pop de
	LD_A_C;  // ld a, c
	AND_A_A;  // and a
	POP_BC;  // pop bc
	RET;  // ret

}

int PrintWinLossText(){
	LD_A_addr(wBattleType);  // ld a, [wBattleType]
	CP_A(BATTLETYPE_CANLOSE);  // cp BATTLETYPE_CANLOSE
// ; code was probably dummied out here
	goto _canlose;  // jr .canlose

//  unused
	LD_HL(wWinTextPointer);  // ld hl, wWinTextPointer
	goto _ok;  // jr .ok


_canlose:
	SET_PC(0x3964U);
	LD_A_addr(wBattleResult);  // ld a, [wBattleResult]
	LD_HL(wWinTextPointer);  // ld hl, wWinTextPointer
	AND_A_A;  // and a ; WIN?
	IF_Z goto _ok;  // jr z, .ok
	LD_HL(wLossTextPointer);  // ld hl, wLossTextPointer


_ok:
	SET_PC(0x3970U);
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	CALL(mGetMapScriptsBank);  // call GetMapScriptsBank
	CALL(mFarPrintText);  // call FarPrintText
	CALL(mWaitBGMap);  // call WaitBGMap
	CALL(mWaitPressAorB_BlinkCursor);  // call WaitPressAorB_BlinkCursor
	RET;  // ret

}
