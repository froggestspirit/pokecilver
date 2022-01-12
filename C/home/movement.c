#include "../constants.h"

int InitMovementBuffer(){
	LD_addr_A(wMovementBufferObject);  // ld [wMovementBufferObject], a
	XOR_A_A;  // xor a
	LD_addr_A(wMovementBufferCount);  // ld [wMovementBufferCount], a
	LD_A(BANK(wMovementBuffer));  // ld a, BANK(wMovementBuffer)
	LD_addr_A(wUnusedMovementBufferBank);  // ld [wUnusedMovementBufferBank], a
	LD_A(LOW(wMovementBuffer));  // ld a, LOW(wMovementBuffer)
	LD_addr_A(wUnusedMovementBufferPointer);  // ld [wUnusedMovementBufferPointer], a
	LD_A(HIGH(wMovementBuffer));  // ld a, HIGH(wMovementBuffer)
	LD_addr_A(wUnusedMovementBufferPointer + 1);  // ld [wUnusedMovementBufferPointer + 1], a
	RET;  // ret

}

int DecrementMovementBufferCount(){
	LD_A_addr(wMovementBufferCount);  // ld a, [wMovementBufferCount]
	AND_A_A;  // and a
	RET_Z ;  // ret z
	DEC_A;  // dec a
	LD_addr_A(wMovementBufferCount);  // ld [wMovementBufferCount], a
	RET;  // ret

}

int AppendToMovementBuffer(){
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	LD_HL(wMovementBufferCount);  // ld hl, wMovementBufferCount
	LD_E_hl;  // ld e, [hl]
	INC_hl;  // inc [hl]
	LD_D(0);  // ld d, 0
	LD_HL(wMovementBuffer);  // ld hl, wMovementBuffer
	ADD_HL_DE;  // add hl, de
	LD_hl_A;  // ld [hl], a
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int AppendToMovementBufferNTimes(){
	PUSH_AF;  // push af
	LD_A_C;  // ld a, c
	AND_A_A;  // and a
	IF_NZ goto okay;  // jr nz, .okay
	POP_AF;  // pop af
	RET;  // ret


okay:
	POP_AF;  // pop af

loop:
	CCALL(aAppendToMovementBuffer);  // call AppendToMovementBuffer
	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

int ComputePathToWalkToPlayer(){
	SET_PC(0x19FAU);
	PUSH_AF;  // push af
//  compare x coords, load left/right into h, and x distance into d
	LD_A_B;  // ld a, b
	SUB_A_D;  // sub d
	LD_H(LEFT);  // ld h, LEFT
	IF_NC goto got_x_distance;  // jr nc, .got_x_distance
	DEC_A;  // dec a
	CPL;  // cpl
	LD_H(RIGHT);  // ld h, RIGHT


got_x_distance:
	SET_PC(0x1A05U);
	LD_D_A;  // ld d, a
//  compare y coords, load up/down into l, and y distance into e
	LD_A_C;  // ld a, c
	SUB_A_E;  // sub e
	LD_L(UP);  // ld l, UP
	IF_NC goto got_y_distance;  // jr nc, .got_y_distance
	DEC_A;  // dec a
	CPL;  // cpl
	LD_L(DOWN);  // ld l, DOWN


got_y_distance:
	SET_PC(0x1A10U);
	LD_E_A;  // ld e, a
//  if the x distance is less than the y distance, swap h and l, and swap d and e
	CP_A_D;  // cp d
	IF_NC goto done;  // jr nc, .done
	LD_A_H;  // ld a, h
	LD_H_L;  // ld h, l
	LD_L_A;  // ld l, a
	LD_A_D;  // ld a, d
	LD_D_E;  // ld d, e
	LD_E_A;  // ld e, a


done:
	SET_PC(0x1A1AU);
	POP_AF;  // pop af
	LD_B_A;  // ld b, a
//  Add movement in the longer direction first...
	LD_A_H;  // ld a, h
	CALL(mComputePathToWalkToPlayer_GetMovementData);  // call .GetMovementData
	LD_C_D;  // ld c, d
	CCALL(aAppendToMovementBufferNTimes);  // call AppendToMovementBufferNTimes
//  ... then add the shorter direction.
	LD_A_L;  // ld a, l
	CALL(mComputePathToWalkToPlayer_GetMovementData);  // call .GetMovementData
	LD_C_E;  // ld c, e
	CCALL(aAppendToMovementBufferNTimes);  // call AppendToMovementBufferNTimes
	RET;  // ret


GetMovementData:
	SET_PC(0x1A2DU);
	PUSH_DE;  // push de
	PUSH_HL;  // push hl
	LD_L_B;  // ld l, b
	LD_H(0);  // ld h, 0
	ADD_HL_HL;  // add hl, hl
	ADD_HL_HL;  // add hl, hl
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
	ADD_HL_DE;  // add hl, de
	LD_DE(mComputePathToWalkToPlayer_MovementData);  // ld de, .MovementData
	ADD_HL_DE;  // add hl, de
	LD_A_hl;  // ld a, [hl]
	POP_HL;  // pop hl
	POP_DE;  // pop de
	RET;  // ret


MovementData:
	SET_PC(0x1A40U);
	//slow_step ['DOWN']  // slow_step DOWN
	//slow_step ['UP']  // slow_step UP
	//slow_step ['LEFT']  // slow_step LEFT
	//slow_step ['RIGHT']  // slow_step RIGHT
	//step ['DOWN']  // step DOWN
	//step ['UP']  // step UP
	//step ['LEFT']  // step LEFT
	//step ['RIGHT']  // step RIGHT
	//big_step ['DOWN']  // big_step DOWN
	//big_step ['UP']  // big_step UP
	//big_step ['LEFT']  // big_step LEFT
	//big_step ['RIGHT']  // big_step RIGHT

}
