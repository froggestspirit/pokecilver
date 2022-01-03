#include "../constants.h"

int HandleStoneQueue(){
	SET_PC(0x37A8U);
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af

	CALL(mSwitchToMapScriptsBank);  // call SwitchToMapScriptsBank
	CALL(mHandleStoneQueue_WarpAction);  // call .WarpAction

	POP_BC;  // pop bc
	LD_A_B;  // ld a, b
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret


WarpAction:
	SET_PC(0x37B5U);
	LD_HL(OBJECT_MAP_OBJECT_INDEX);  // ld hl, OBJECT_MAP_OBJECT_INDEX
	ADD_HL_DE;  // add hl, de
	LD_A_hl;  // ld a, [hl]
	CP_A(0xff);  // cp $ff
	IF_Z goto nope;  // jr z, .nope

	LD_L_A;  // ld l, a
	PUSH_HL;  // push hl
	CALL(mHandleStoneQueue_IsObjectOnWarp);  // call .IsObjectOnWarp
	POP_HL;  // pop hl
	IF_NC goto nope;  // jr nc, .nope
	LD_D_A;  // ld d, a
	LD_E_L;  // ld e, l
	CALL(mHandleStoneQueue_IsObjectInStoneTable);  // call .IsObjectInStoneTable
	IF_NC goto nope;  // jr nc, .nope
	CALL(mCallMapScript);  // call CallMapScript
	FARCALL(aEnableScriptMode);  // farcall EnableScriptMode
	SCF;  // scf
	RET;  // ret


nope:
	SET_PC(0x37D8U);
	AND_A_A;  // and a
	RET;  // ret


IsObjectOnWarp:
	SET_PC(0x37DAU);
	PUSH_DE;  // push de

	LD_HL(OBJECT_NEXT_MAP_X);  // ld hl, OBJECT_NEXT_MAP_X
	ADD_HL_DE;  // add hl, de
	LD_A_hl;  // ld a, [hl]
	LD_HL(OBJECT_NEXT_MAP_Y);  // ld hl, OBJECT_NEXT_MAP_Y
	ADD_HL_DE;  // add hl, de
	LD_E_hl;  // ld e, [hl]

	SUB_A(4);  // sub 4
	LD_D_A;  // ld d, a
	LD_A_E;  // ld a, e
	SUB_A(4);  // sub 4
	LD_E_A;  // ld e, a
	CALL(mHandleStoneQueue_check_on_warp);  // call .check_on_warp

	POP_DE;  // pop de
	RET;  // ret


check_on_warp:
	SET_PC(0x37F1U);
	LD_HL(wCurMapWarpsPointer);  // ld hl, wCurMapWarpsPointer
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	LD_A_addr(wCurMapWarpCount);  // ld a, [wCurMapWarpCount]
	AND_A_A;  // and a
	IF_Z goto nope2;  // jr z, .nope2


loop:
	SET_PC(0x37FDU);
	PUSH_AF;  // push af
	LD_A_hl;  // ld a, [hl]
	CP_A_E;  // cp e
	IF_NZ goto not_on_warp;  // jr nz, .not_on_warp
	INC_HL;  // inc hl
	LD_A_hld;  // ld a, [hld]
	CP_A_D;  // cp d
	IF_NZ goto not_on_warp;  // jr nz, .not_on_warp
	goto found_warp;  // jr .found_warp


not_on_warp:
	SET_PC(0x3809U);
	LD_A(WARP_EVENT_SIZE);  // ld a, WARP_EVENT_SIZE
	ADD_A_L;  // add l
	LD_L_A;  // ld l, a
	IF_NC goto no_carry;  // jr nc, .no_carry
	INC_H;  // inc h

no_carry:
	SET_PC(0x3810U);

	POP_AF;  // pop af
	DEC_A;  // dec a
	IF_NZ goto loop;  // jr nz, .loop


nope2:
	SET_PC(0x3814U);
	AND_A_A;  // and a
	RET;  // ret


found_warp:
	SET_PC(0x3816U);
	POP_AF;  // pop af
	LD_D_A;  // ld d, a
	LD_A_addr(wCurMapWarpCount);  // ld a, [wCurMapWarpCount]
	SUB_A_D;  // sub d
	INC_A;  // inc a
	SCF;  // scf
	RET;  // ret


IsObjectInStoneTable:
	SET_PC(0x381FU);
	LD_HL(CMDQUEUE_ADDR);  // ld hl, CMDQUEUE_ADDR
	ADD_HL_BC;  // add hl, bc
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a

loop2:
	SET_PC(0x3826U);
	LD_A_hli;  // ld a, [hli]
	CP_A(0xff);  // cp $ff
	IF_Z goto nope3;  // jr z, .nope3
	CP_A_D;  // cp d
	IF_NZ goto next_inc3;  // jr nz, .next_inc3
	LD_A_hli;  // ld a, [hli]
	CP_A_E;  // cp e
	IF_NZ goto next_inc2;  // jr nz, .next_inc2
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	goto yes;  // jr .yes


next_inc3:
	SET_PC(0x3837U);
	INC_HL;  // inc hl


next_inc2:
	SET_PC(0x3838U);
	INC_HL;  // inc hl
	INC_HL;  // inc hl
	goto loop2;  // jr .loop2


nope3:
	SET_PC(0x383CU);
	AND_A_A;  // and a
	RET;  // ret


yes:
	SET_PC(0x383EU);
	SCF;  // scf
	RET;  // ret

}
