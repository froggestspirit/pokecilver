#include "../constants.h"

int ResetGameTime(){
	SET_PC(0x1EE4U);
	XOR_A_A;  // xor a
	LD_addr_A(wGameTimeCap);  // ld [wGameTimeCap], a
	LD_addr_A(wGameTimeHours);  // ld [wGameTimeHours], a
	LD_addr_A(wGameTimeHours + 1);  // ld [wGameTimeHours + 1], a
	LD_addr_A(wGameTimeMinutes);  // ld [wGameTimeMinutes], a
	LD_addr_A(wGameTimeSeconds);  // ld [wGameTimeSeconds], a
	LD_addr_A(wGameTimeFrames);  // ld [wGameTimeFrames], a
	RET;  // ret

}

int GameTimer(){
	SET_PC(0x1EF8U);
	NOP;  // nop

//  Increment the game timer by one frame.
//  The game timer is capped at 999:59:59.00.

//  Don't update if game logic is paused.
	LD_A_addr(wGameLogicPaused);  // ld a, [wGameLogicPaused]
	AND_A_A;  // and a
	RET_NZ ;  // ret nz

//  Is the timer paused?
	LD_HL(wGameTimerPaused);  // ld hl, wGameTimerPaused
	BIT_hl(GAME_TIMER_PAUSED_F);  // bit GAME_TIMER_PAUSED_F, [hl]
	RET_Z ;  // ret z

//  Is the timer already capped?
	LD_HL(wGameTimeCap);  // ld hl, wGameTimeCap
	BIT_hl(0);  // bit 0, [hl]
	RET_NZ ;  // ret nz

//  +1 frame
	LD_HL(wGameTimeFrames);  // ld hl, wGameTimeFrames
	LD_A_hl;  // ld a, [hl]
	INC_A;  // inc a

	CP_A(60);  // cp 60 ; frames/second
	IF_NC goto second;  // jr nc, .second

	LD_hl_A;  // ld [hl], a
	RET;  // ret


second:
	SET_PC(0x1F15U);
	XOR_A_A;  // xor a
	LD_hl_A;  // ld [hl], a

//  +1 second
	LD_HL(wGameTimeSeconds);  // ld hl, wGameTimeSeconds
	LD_A_hl;  // ld a, [hl]
	INC_A;  // inc a

	CP_A(60);  // cp 60 ; seconds/minute
	IF_NC goto minute;  // jr nc, .minute

	LD_hl_A;  // ld [hl], a
	RET;  // ret


minute:
	SET_PC(0x1F22U);
	XOR_A_A;  // xor a
	LD_hl_A;  // ld [hl], a

//  +1 minute
	LD_HL(wGameTimeMinutes);  // ld hl, wGameTimeMinutes
	LD_A_hl;  // ld a, [hl]
	INC_A;  // inc a

	CP_A(60);  // cp 60 ; minutes/hour
	IF_NC goto hour;  // jr nc, .hour

	LD_hl_A;  // ld [hl], a
	RET;  // ret


hour:
	SET_PC(0x1F2FU);
	XOR_A_A;  // xor a
	LD_hl_A;  // ld [hl], a

//  +1 hour
	LD_A_addr(wGameTimeHours);  // ld a, [wGameTimeHours]
	LD_H_A;  // ld h, a
	LD_A_addr(wGameTimeHours + 1);  // ld a, [wGameTimeHours + 1]
	LD_L_A;  // ld l, a
	INC_HL;  // inc hl

//  Cap the timer after 1000 hours.
	LD_A_H;  // ld a, h
	CP_A(HIGH(1000));  // cp HIGH(1000)
	IF_C goto ok;  // jr c, .ok

	LD_A_L;  // ld a, l
	CP_A(LOW(1000));  // cp LOW(1000)
	IF_C goto ok;  // jr c, .ok

	LD_HL(wGameTimeCap);  // ld hl, wGameTimeCap
	SET_hl(0);  // set 0, [hl]

	LD_A(59);  // ld a, 59 ; 999:59:59.00
	LD_addr_A(wGameTimeMinutes);  // ld [wGameTimeMinutes], a
	LD_addr_A(wGameTimeSeconds);  // ld [wGameTimeSeconds], a
	RET;  // ret


ok:
	SET_PC(0x1F52U);
	LD_A_H;  // ld a, h
	LD_addr_A(wGameTimeHours);  // ld [wGameTimeHours], a
	LD_A_L;  // ld a, l
	LD_addr_A(wGameTimeHours + 1);  // ld [wGameTimeHours + 1], a
	RET;  // ret

}
