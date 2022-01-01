#include "../constants.h"

int GetBattleVar(){
	PUSH_HL;  // push hl
	CALL(mGetBattleVarAddr);  // call GetBattleVarAddr
	POP_HL;  // pop hl
	RET;  // ret

}

int GetBattleVarAddr(){
//  Get variable from pair a, depending on whose turn it is.
//  There are 21 variable pairs.
	PUSH_BC;  // push bc

	LD_HL(mBattleVarPairs);  // ld hl, BattleVarPairs
	LD_C_A;  // ld c, a
	LD_B(0);  // ld b, 0
	ADD_HL_BC;  // add hl, bc
	ADD_HL_BC;  // add hl, bc

	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a

//  Enemy turn uses the second byte instead.
//  This lets battle variable calls be side-neutral.
	LDH_A_addr(hBattleTurn);  // ldh a, [hBattleTurn]
	AND_A_A;  // and a
	IF_Z goto _getvar;  // jr z, .getvar
	INC_HL;  // inc hl


_getvar:
	SET_PC(0x3C19U);
//  var id
	LD_A_hl;  // ld a, [hl]
	LD_C_A;  // ld c, a
	LD_B(0);  // ld b, 0

	LD_HL(mBattleVarLocations);  // ld hl, BattleVarLocations
	ADD_HL_BC;  // add hl, bc
	ADD_HL_BC;  // add hl, bc

	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a

	LD_A_hl;  // ld a, [hl]

	POP_BC;  // pop bc
	RET;  // ret

}

int BattleVarPairs(){
//  entries correspond to BATTLE_VARS_* constants
	//table_width ['2', 'BattleVarPairs']  // table_width 2, BattleVarPairs
	//dw ['.Substatus1'];  // dw .Substatus1
	//dw ['.Substatus2'];  // dw .Substatus2
	//dw ['.Substatus3'];  // dw .Substatus3
	//dw ['.Substatus4'];  // dw .Substatus4
	//dw ['.Substatus5'];  // dw .Substatus5
	//dw ['.Substatus1Opp'];  // dw .Substatus1Opp
	//dw ['.Substatus2Opp'];  // dw .Substatus2Opp
	//dw ['.Substatus3Opp'];  // dw .Substatus3Opp
	//dw ['.Substatus4Opp'];  // dw .Substatus4Opp
	//dw ['.Substatus5Opp'];  // dw .Substatus5Opp
	//dw ['.Status'];  // dw .Status
	//dw ['.StatusOpp'];  // dw .StatusOpp
	//dw ['.MoveAnim'];  // dw .MoveAnim
	//dw ['.MoveEffect'];  // dw .MoveEffect
	//dw ['.MovePower'];  // dw .MovePower
	//dw ['.MoveType'];  // dw .MoveType
	//dw ['.CurMove'];  // dw .CurMove
	//dw ['.LastCounter'];  // dw .LastCounter
	//dw ['.LastCounterOpp'];  // dw .LastCounterOpp
	//dw ['.LastMove'];  // dw .LastMove
	//dw ['.LastMoveOpp'];  // dw .LastMoveOpp
	//assert_table_length ['NUM_BATTLE_VARS']  // assert_table_length NUM_BATTLE_VARS

//                    player                 enemy

_Substatus1:
//     db PLAYER_SUBSTATUS_1,    ENEMY_SUBSTATUS_1
	SET_PC(0x3C52U);

_Substatus1Opp:
//  db ENEMY_SUBSTATUS_1,     PLAYER_SUBSTATUS_1
	SET_PC(0x3C54U);

_Substatus2:
//     db PLAYER_SUBSTATUS_2,    ENEMY_SUBSTATUS_2
	SET_PC(0x3C56U);

_Substatus2Opp:
//  db ENEMY_SUBSTATUS_2,     PLAYER_SUBSTATUS_2
	SET_PC(0x3C58U);

_Substatus3:
//     db PLAYER_SUBSTATUS_3,    ENEMY_SUBSTATUS_3
	SET_PC(0x3C5AU);

_Substatus3Opp:
//  db ENEMY_SUBSTATUS_3,     PLAYER_SUBSTATUS_3
	SET_PC(0x3C5CU);

_Substatus4:
//     db PLAYER_SUBSTATUS_4,    ENEMY_SUBSTATUS_4
	SET_PC(0x3C5EU);

_Substatus4Opp:
//  db ENEMY_SUBSTATUS_4,     PLAYER_SUBSTATUS_4
	SET_PC(0x3C60U);

_Substatus5:
//     db PLAYER_SUBSTATUS_5,    ENEMY_SUBSTATUS_5
	SET_PC(0x3C62U);

_Substatus5Opp:
//  db ENEMY_SUBSTATUS_5,     PLAYER_SUBSTATUS_5
	SET_PC(0x3C64U);

_Status:
//         db PLAYER_STATUS,         ENEMY_STATUS
	SET_PC(0x3C66U);

_StatusOpp:
//      db ENEMY_STATUS,          PLAYER_STATUS
	SET_PC(0x3C68U);

_MoveAnim:
//       db PLAYER_MOVE_ANIMATION, ENEMY_MOVE_ANIMATION
	SET_PC(0x3C6AU);

_MoveEffect:
//     db PLAYER_MOVE_EFFECT,    ENEMY_MOVE_EFFECT
	SET_PC(0x3C6CU);

_MovePower:
//      db PLAYER_MOVE_POWER,     ENEMY_MOVE_POWER
	SET_PC(0x3C6EU);

_MoveType:
//       db PLAYER_MOVE_TYPE,      ENEMY_MOVE_TYPE
	SET_PC(0x3C70U);

_CurMove:
//        db PLAYER_CUR_MOVE,       ENEMY_CUR_MOVE
	SET_PC(0x3C72U);

_LastCounter:
//    db PLAYER_COUNTER_MOVE,   ENEMY_COUNTER_MOVE
	SET_PC(0x3C74U);

_LastCounterOpp:
// db ENEMY_COUNTER_MOVE,    PLAYER_COUNTER_MOVE
	SET_PC(0x3C76U);

_LastMove:
//       db PLAYER_LAST_MOVE,      ENEMY_LAST_MOVE
	SET_PC(0x3C78U);

_LastMoveOpp:
//    db ENEMY_LAST_MOVE,       PLAYER_LAST_MOVE
	SET_PC(0x3C7AU);

	return mBattleVarLocations;
}

int BattleVarLocations(){
//  entries correspond to PLAYER_* and ENEMY_* constants
	//table_width ['2 + 2', 'BattleVarLocations']  // table_width 2 + 2, BattleVarLocations
	//dw ['wPlayerSubStatus1', 'wEnemySubStatus1'];  // dw wPlayerSubStatus1,          wEnemySubStatus1
	//dw ['wPlayerSubStatus2', 'wEnemySubStatus2'];  // dw wPlayerSubStatus2,          wEnemySubStatus2
	//dw ['wPlayerSubStatus3', 'wEnemySubStatus3'];  // dw wPlayerSubStatus3,          wEnemySubStatus3
	//dw ['wPlayerSubStatus4', 'wEnemySubStatus4'];  // dw wPlayerSubStatus4,          wEnemySubStatus4
	//dw ['wPlayerSubStatus5', 'wEnemySubStatus5'];  // dw wPlayerSubStatus5,          wEnemySubStatus5
	//dw ['wBattleMonStatus', 'wEnemyMonStatus'];  // dw wBattleMonStatus,           wEnemyMonStatus
	//dw ['wPlayerMoveStructAnimation', 'wEnemyMoveStructAnimation'];  // dw wPlayerMoveStructAnimation, wEnemyMoveStructAnimation
	//dw ['wPlayerMoveStructEffect', 'wEnemyMoveStructEffect'];  // dw wPlayerMoveStructEffect,    wEnemyMoveStructEffect
	//dw ['wPlayerMoveStructPower', 'wEnemyMoveStructPower'];  // dw wPlayerMoveStructPower,     wEnemyMoveStructPower
	//dw ['wPlayerMoveStructType', 'wEnemyMoveStructType'];  // dw wPlayerMoveStructType,      wEnemyMoveStructType
	//dw ['wCurPlayerMove', 'wCurEnemyMove'];  // dw wCurPlayerMove,             wCurEnemyMove
	//dw ['wLastPlayerCounterMove', 'wLastEnemyCounterMove'];  // dw wLastPlayerCounterMove,     wLastEnemyCounterMove
	//dw ['wLastPlayerMove', 'wLastEnemyMove'];  // dw wLastPlayerMove,            wLastEnemyMove
	//assert_table_length ['NUM_BATTLE_VAR_LOCATION_PAIRS']  // assert_table_length NUM_BATTLE_VAR_LOCATION_PAIRS

}
