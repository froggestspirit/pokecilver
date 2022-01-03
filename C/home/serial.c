#include "../constants.h"

int Serial(){
	SET_PC(0x06AAU);
//  The serial interrupt.

	PUSH_AF;  // push af
	PUSH_BC;  // push bc
	PUSH_DE;  // push de
	PUSH_HL;  // push hl

	LD_A_addr(wPrinterConnectionOpen);  // ld a, [wPrinterConnectionOpen]
	BIT_A(0);  // bit 0, a
	IF_NZ goto printer;  // jr nz, .printer

	LDH_A_addr(hSerialConnectionStatus);  // ldh a, [hSerialConnectionStatus]
	INC_A;  // inc a ; is it equal to CONNECTION_NOT_ESTABLISHED?
	IF_Z goto establish_connection;  // jr z, .establish_connection

	LDH_A_addr(rSB);  // ldh a, [rSB]
	LDH_addr_A(hSerialReceive);  // ldh [hSerialReceive], a

	LDH_A_addr(hSerialSend);  // ldh a, [hSerialSend]
	LDH_addr_A(rSB);  // ldh [rSB], a

	LDH_A_addr(hSerialConnectionStatus);  // ldh a, [hSerialConnectionStatus]
	CP_A(USING_INTERNAL_CLOCK);  // cp USING_INTERNAL_CLOCK
	IF_Z goto player2;  // jr z, .player2

	LD_A((0 << rSC_ON) | (0 << rSC_CLOCK));  // ld a, (0 << rSC_ON) | (0 << rSC_CLOCK)
	LDH_addr_A(rSC);  // ldh [rSC], a
	LD_A((1 << rSC_ON) | (0 << rSC_CLOCK));  // ld a, (1 << rSC_ON) | (0 << rSC_CLOCK)
	LDH_addr_A(rSC);  // ldh [rSC], a
	goto player2;  // jr .player2


printer:
	SET_PC(0x06D2U);
	CALL(mPrinterReceive);  // call PrinterReceive
	goto end;  // jr .end


establish_connection:
	SET_PC(0x06D7U);
	LDH_A_addr(rSB);  // ldh a, [rSB]
	CP_A(USING_EXTERNAL_CLOCK);  // cp USING_EXTERNAL_CLOCK
	IF_Z goto player1;  // jr z, .player1
	CP_A(USING_INTERNAL_CLOCK);  // cp USING_INTERNAL_CLOCK
	IF_NZ goto player2;  // jr nz, .player2


player1:
	SET_PC(0x06E1U);
	LDH_addr_A(hSerialReceive);  // ldh [hSerialReceive], a
	LDH_addr_A(hSerialConnectionStatus);  // ldh [hSerialConnectionStatus], a
	CP_A(USING_INTERNAL_CLOCK);  // cp USING_INTERNAL_CLOCK
	IF_Z goto _player2;  // jr z, ._player2

	XOR_A_A;  // xor a
	LDH_addr_A(rSB);  // ldh [rSB], a

	LD_A(3);  // ld a, 3
	LDH_addr_A(rDIV);  // ldh [rDIV], a

delay_loop:
	SET_PC(0x06F0U);
	LDH_A_addr(rDIV);  // ldh a, [rDIV]
	BIT_A(7);  // bit 7, a
	IF_NZ goto delay_loop;  // jr nz, .delay_loop

	LD_A((0 << rSC_ON) | (0 << rSC_CLOCK));  // ld a, (0 << rSC_ON) | (0 << rSC_CLOCK)
	LDH_addr_A(rSC);  // ldh [rSC], a
	LD_A((1 << rSC_ON) | (0 << rSC_CLOCK));  // ld a, (1 << rSC_ON) | (0 << rSC_CLOCK)
	LDH_addr_A(rSC);  // ldh [rSC], a
	goto player2;  // jr .player2


_player2:
	SET_PC(0x0700U);
	XOR_A_A;  // xor a
	LDH_addr_A(rSB);  // ldh [rSB], a


player2:
	SET_PC(0x0703U);
	LD_A(TRUE);  // ld a, TRUE
	LDH_addr_A(hSerialReceivedNewData);  // ldh [hSerialReceivedNewData], a
	LD_A(SERIAL_NO_DATA_BYTE);  // ld a, SERIAL_NO_DATA_BYTE
	LDH_addr_A(hSerialSend);  // ldh [hSerialSend], a


end:
	SET_PC(0x070BU);
	POP_HL;  // pop hl
	POP_DE;  // pop de
	POP_BC;  // pop bc
	POP_AF;  // pop af
	RET;  // ret

}

int Serial_ExchangeBytes(){
	SET_PC(0x0710U);
//  send bc bytes from hl, receive bc bytes to de
	LD_A(TRUE);  // ld a, TRUE
	LDH_addr_A(hSerialIgnoringInitialData);  // ldh [hSerialIgnoringInitialData], a


loop:
	SET_PC(0x0714U);
	LD_A_hl;  // ld a, [hl]
	LDH_addr_A(hSerialSend);  // ldh [hSerialSend], a
	CALL(mSerial_ExchangeByte);  // call Serial_ExchangeByte
	PUSH_BC;  // push bc
	LD_B_A;  // ld b, a
	INC_HL;  // inc hl

	LD_A(48);  // ld a, 48

wait:
	SET_PC(0x071FU);
	DEC_A;  // dec a
	IF_NZ goto wait;  // jr nz, .wait

	LDH_A_addr(hSerialIgnoringInitialData);  // ldh a, [hSerialIgnoringInitialData]
	AND_A_A;  // and a
	LD_A_B;  // ld a, b
	POP_BC;  // pop bc
	IF_Z goto load;  // jr z, .load
	DEC_HL;  // dec hl
	CP_A(SERIAL_PREAMBLE_BYTE);  // cp SERIAL_PREAMBLE_BYTE
	IF_NZ goto loop;  // jr nz, .loop
	XOR_A_A;  // xor a
	LDH_addr_A(hSerialIgnoringInitialData);  // ldh [hSerialIgnoringInitialData], a
	goto loop;  // jr .loop


load:
	SET_PC(0x0733U);
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
	DEC_BC;  // dec bc
	LD_A_B;  // ld a, b
	OR_A_C;  // or c
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

int Serial_ExchangeByte(){
	SET_PC(0x073BU);

timeout_loop:
	SET_PC(0x073BU);
	XOR_A_A;  // xor a
	LDH_addr_A(hSerialReceivedNewData);  // ldh [hSerialReceivedNewData], a
	LDH_A_addr(hSerialConnectionStatus);  // ldh a, [hSerialConnectionStatus]
	CP_A(USING_INTERNAL_CLOCK);  // cp USING_INTERNAL_CLOCK
	IF_NZ goto not_player_2;  // jr nz, .not_player_2
	LD_A((0 << rSC_ON) | (1 << rSC_CLOCK));  // ld a, (0 << rSC_ON) | (1 << rSC_CLOCK)
	LDH_addr_A(rSC);  // ldh [rSC], a
	LD_A((1 << rSC_ON) | (1 << rSC_CLOCK));  // ld a, (1 << rSC_ON) | (1 << rSC_CLOCK)
	LDH_addr_A(rSC);  // ldh [rSC], a

not_player_2:
	SET_PC(0x074CU);

loop:
	SET_PC(0x074CU);
	LDH_A_addr(hSerialReceivedNewData);  // ldh a, [hSerialReceivedNewData]
	AND_A_A;  // and a
	IF_NZ goto await_new_data;  // jr nz, .await_new_data
	LDH_A_addr(hSerialConnectionStatus);  // ldh a, [hSerialConnectionStatus]
	CP_A(USING_EXTERNAL_CLOCK);  // cp USING_EXTERNAL_CLOCK
	IF_NZ goto not_player_1_or_timed_out;  // jr nz, .not_player_1_or_timed_out
	CALL(mCheckLinkTimeoutFramesNonzero);  // call CheckLinkTimeoutFramesNonzero
	IF_Z goto not_player_1_or_timed_out;  // jr z, .not_player_1_or_timed_out
	CALL(mSerial_ExchangeByte_ShortDelay);  // call .ShortDelay
	PUSH_HL;  // push hl
	LD_HL(wLinkTimeoutFrames + 1);  // ld hl, wLinkTimeoutFrames + 1
	INC_hl;  // inc [hl]
	IF_NZ goto no_rollover_up;  // jr nz, .no_rollover_up
	DEC_HL;  // dec hl
	INC_hl;  // inc [hl]


no_rollover_up:
	SET_PC(0x0768U);
	POP_HL;  // pop hl
	CALL(mCheckLinkTimeoutFramesNonzero);  // call CheckLinkTimeoutFramesNonzero
	IF_NZ goto loop;  // jr nz, .loop
	JP(mSerialDisconnected);  // jp SerialDisconnected


not_player_1_or_timed_out:
	SET_PC(0x0771U);
	LDH_A_addr(rIE);  // ldh a, [rIE]
	AND_A((1 << SERIAL) | (1 << TIMER) | (1 << LCD_STAT) | (1 << VBLANK));  // and (1 << SERIAL) | (1 << TIMER) | (1 << LCD_STAT) | (1 << VBLANK)
	CP_A(1 << SERIAL);  // cp 1 << SERIAL
	IF_NZ goto loop;  // jr nz, .loop
	LD_A_addr(wLinkByteTimeout);  // ld a, [wLinkByteTimeout]
	DEC_A;  // dec a
	LD_addr_A(wLinkByteTimeout);  // ld [wLinkByteTimeout], a
	IF_NZ goto loop;  // jr nz, .loop
	LD_A_addr(wLinkByteTimeout + 1);  // ld a, [wLinkByteTimeout + 1]
	DEC_A;  // dec a
	LD_addr_A(wLinkByteTimeout + 1);  // ld [wLinkByteTimeout + 1], a
	IF_NZ goto loop;  // jr nz, .loop
	LDH_A_addr(hSerialConnectionStatus);  // ldh a, [hSerialConnectionStatus]
	CP_A(USING_EXTERNAL_CLOCK);  // cp USING_EXTERNAL_CLOCK
	IF_Z goto await_new_data;  // jr z, .await_new_data

	LD_A(255);  // ld a, 255

long_delay_loop:
	SET_PC(0x0793U);
	DEC_A;  // dec a
	IF_NZ goto long_delay_loop;  // jr nz, .long_delay_loop


await_new_data:
	SET_PC(0x0796U);
	XOR_A_A;  // xor a
	LDH_addr_A(hSerialReceivedNewData);  // ldh [hSerialReceivedNewData], a
	LDH_A_addr(rIE);  // ldh a, [rIE]
	AND_A((1 << SERIAL) | (1 << TIMER) | (1 << LCD_STAT) | (1 << VBLANK));  // and (1 << SERIAL) | (1 << TIMER) | (1 << LCD_STAT) | (1 << VBLANK)
	SUB_A(1 << SERIAL);  // sub 1 << SERIAL
	IF_NZ goto non_serial_interrupts_enabled;  // jr nz, .non_serial_interrupts_enabled

// ; a == 0
	//assert ['LOW(SERIAL_LINK_BYTE_TIMEOUT) == 0'];  // assert LOW(SERIAL_LINK_BYTE_TIMEOUT) == 0
	LD_addr_A(wLinkByteTimeout);  // ld [wLinkByteTimeout], a
	LD_A(HIGH(SERIAL_LINK_BYTE_TIMEOUT));  // ld a, HIGH(SERIAL_LINK_BYTE_TIMEOUT)
	LD_addr_A(wLinkByteTimeout + 1);  // ld [wLinkByteTimeout + 1], a


non_serial_interrupts_enabled:
	SET_PC(0x07A9U);
	LDH_A_addr(hSerialReceive);  // ldh a, [hSerialReceive]
	CP_A(SERIAL_NO_DATA_BYTE);  // cp SERIAL_NO_DATA_BYTE
	RET_NZ ;  // ret nz
	CALL(mCheckLinkTimeoutFramesNonzero);  // call CheckLinkTimeoutFramesNonzero
	IF_Z goto timed_out;  // jr z, .timed_out
	PUSH_HL;  // push hl
	LD_HL(wLinkTimeoutFrames + 1);  // ld hl, wLinkTimeoutFrames + 1
	LD_A_hl;  // ld a, [hl]
	DEC_A;  // dec a
	LD_hld_A;  // ld [hld], a
	INC_A;  // inc a
	IF_NZ goto no_rollover;  // jr nz, .no_rollover
	DEC_hl;  // dec [hl]


no_rollover:
	SET_PC(0x07BEU);
	POP_HL;  // pop hl
	CALL(mCheckLinkTimeoutFramesNonzero);  // call CheckLinkTimeoutFramesNonzero
	JR_Z (mSerialDisconnected);  // jr z, SerialDisconnected


timed_out:
	SET_PC(0x07C4U);
	LDH_A_addr(rIE);  // ldh a, [rIE]
	AND_A((1 << SERIAL) | (1 << TIMER) | (1 << LCD_STAT) | (1 << VBLANK));  // and (1 << SERIAL) | (1 << TIMER) | (1 << LCD_STAT) | (1 << VBLANK)
	CP_A(1 << SERIAL);  // cp 1 << SERIAL
	LD_A(SERIAL_NO_DATA_BYTE);  // ld a, SERIAL_NO_DATA_BYTE
	RET_Z ;  // ret z
	LD_A_hl;  // ld a, [hl]
	LDH_addr_A(hSerialSend);  // ldh [hSerialSend], a
	CALL(mDelayFrame);  // call DelayFrame
	JP(mSerial_ExchangeByte_timeout_loop);  // jp .timeout_loop


ShortDelay:
	SET_PC(0x07D6U);
	LD_A(15);  // ld a, 15

short_delay_loop:
	SET_PC(0x07D8U);
	DEC_A;  // dec a
	IF_NZ goto short_delay_loop;  // jr nz, .short_delay_loop
	RET;  // ret

}

int CheckLinkTimeoutFramesNonzero(){
	SET_PC(0x07DCU);
	PUSH_HL;  // push hl
	LD_HL(wLinkTimeoutFrames);  // ld hl, wLinkTimeoutFrames
	LD_A_hli;  // ld a, [hli]
	OR_A_hl;  // or [hl]
	POP_HL;  // pop hl
	RET;  // ret

//  This sets wLinkTimeoutFrames to $ffff, since
//  a is always 0 when it is called.
	return mSerialDisconnected;
}

int SerialDisconnected(){
	SET_PC(0x07E4U);
	DEC_A;  // dec a
	LD_addr_A(wLinkTimeoutFrames);  // ld [wLinkTimeoutFrames], a
	LD_addr_A(wLinkTimeoutFrames + 1);  // ld [wLinkTimeoutFrames + 1], a
	RET;  // ret

//  This is used to check that both players entered the same Cable Club room.
	return mSerial_ExchangeSyncBytes;
}

int Serial_ExchangeSyncBytes(){
	SET_PC(0x07ECU);
	LD_HL(wLinkPlayerSyncBuffer);  // ld hl, wLinkPlayerSyncBuffer
	LD_DE(wLinkReceivedSyncBuffer);  // ld de, wLinkReceivedSyncBuffer
	LD_C(2);  // ld c, 2
	LD_A(TRUE);  // ld a, TRUE
	LDH_addr_A(hSerialIgnoringInitialData);  // ldh [hSerialIgnoringInitialData], a

exchange:
	SET_PC(0x07F8U);
	CALL(mDelayFrame);  // call DelayFrame
	LD_A_hl;  // ld a, [hl]
	LDH_addr_A(hSerialSend);  // ldh [hSerialSend], a
	CALL(mSerial_ExchangeByte);  // call Serial_ExchangeByte
	LD_B_A;  // ld b, a
	INC_HL;  // inc hl
	LDH_A_addr(hSerialIgnoringInitialData);  // ldh a, [hSerialIgnoringInitialData]
	AND_A_A;  // and a
	LD_A(FALSE);  // ld a, FALSE
	LDH_addr_A(hSerialIgnoringInitialData);  // ldh [hSerialIgnoringInitialData], a
	IF_NZ goto exchange;  // jr nz, .exchange
	LD_A_B;  // ld a, b
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
	DEC_C;  // dec c
	IF_NZ goto exchange;  // jr nz, .exchange
	RET;  // ret

}

int Serial_PrintWaitingTextAndSyncAndExchangeNybble(){
	SET_PC(0x0813U);
	CALL(mLoadTilemapToTempTilemap);  // call LoadTilemapToTempTilemap
	CALLFAR(aPlaceWaitingText);  // callfar PlaceWaitingText
	CALL(mWaitLinkTransfer);  // call WaitLinkTransfer
	JP(mSafeLoadTempTilemapToTilemap);  // jp SafeLoadTempTilemapToTilemap

}

int Serial_SyncAndExchangeNybble(){
	SET_PC(0x0822U);
//  //  unreferenced
	CALL(mLoadTilemapToTempTilemap);  // call LoadTilemapToTempTilemap
	CALLFAR(aPlaceWaitingText);  // callfar PlaceWaitingText
	JP(mWaitLinkTransfer);  // jp WaitLinkTransfer ; pointless

}

int WaitLinkTransfer(){
	SET_PC(0x082EU);
	LD_A(0xff);  // ld a, $ff
	LD_addr_A(wOtherPlayerLinkAction);  // ld [wOtherPlayerLinkAction], a

loop:
	SET_PC(0x0833U);
	CALL(mLinkTransfer);  // call LinkTransfer
	CALL(mDelayFrame);  // call DelayFrame
	CALL(mCheckLinkTimeoutFramesNonzero);  // call CheckLinkTimeoutFramesNonzero
	IF_Z goto check;  // jr z, .check
	PUSH_HL;  // push hl
	LD_HL(wLinkTimeoutFrames + 1);  // ld hl, wLinkTimeoutFrames + 1
	DEC_hl;  // dec [hl]
	IF_NZ goto skip;  // jr nz, .skip
	DEC_HL;  // dec hl
	DEC_hl;  // dec [hl]
	IF_NZ goto skip;  // jr nz, .skip
// ; We might be disconnected
	POP_HL;  // pop hl
	XOR_A_A;  // xor a
	JP(mSerialDisconnected);  // jp SerialDisconnected


skip:
	SET_PC(0x084EU);
	POP_HL;  // pop hl


check:
	SET_PC(0x084FU);
	LD_A_addr(wOtherPlayerLinkAction);  // ld a, [wOtherPlayerLinkAction]
	INC_A;  // inc a
	IF_Z goto loop;  // jr z, .loop

	LD_B(10);  // ld b, 10

receive:
	SET_PC(0x0857U);
	CALL(mDelayFrame);  // call DelayFrame
	CALL(mLinkTransfer);  // call LinkTransfer
	DEC_B;  // dec b
	IF_NZ goto receive;  // jr nz, .receive

	LD_B(10);  // ld b, 10

acknowledge:
	SET_PC(0x0862U);
	CALL(mDelayFrame);  // call DelayFrame
	CALL(mLinkDataReceived);  // call LinkDataReceived
	DEC_B;  // dec b
	IF_NZ goto acknowledge;  // jr nz, .acknowledge

	LD_A_addr(wOtherPlayerLinkAction);  // ld a, [wOtherPlayerLinkAction]
	LD_addr_A(wOtherPlayerLinkMode);  // ld [wOtherPlayerLinkMode], a
	RET;  // ret

}

int LinkTransfer(){
	SET_PC(0x0872U);
	PUSH_BC;  // push bc
	LD_B(SERIAL_TIMECAPSULE);  // ld b, SERIAL_TIMECAPSULE
	LD_A_addr(wLinkMode);  // ld a, [wLinkMode]
	CP_A(LINK_TIMECAPSULE);  // cp LINK_TIMECAPSULE
	IF_Z goto got_high_nybble;  // jr z, .got_high_nybble
	LD_B(SERIAL_TIMECAPSULE);  // ld b, SERIAL_TIMECAPSULE
	IF_C goto got_high_nybble;  // jr c, .got_high_nybble
	CP_A(LINK_TRADECENTER);  // cp LINK_TRADECENTER
	LD_B(SERIAL_TRADECENTER);  // ld b, SERIAL_TRADECENTER
	IF_Z goto got_high_nybble;  // jr z, .got_high_nybble
	LD_B(SERIAL_BATTLE);  // ld b, SERIAL_BATTLE


got_high_nybble:
	SET_PC(0x0888U);
	CALL(mLinkTransfer_Receive);  // call .Receive
	LD_A_addr(wPlayerLinkAction);  // ld a, [wPlayerLinkAction]
	ADD_A_B;  // add b
	LDH_addr_A(hSerialSend);  // ldh [hSerialSend], a
	LDH_A_addr(hSerialConnectionStatus);  // ldh a, [hSerialConnectionStatus]
	CP_A(USING_INTERNAL_CLOCK);  // cp USING_INTERNAL_CLOCK
	IF_NZ goto player_1;  // jr nz, .player_1
	LD_A((0 << rSC_ON) | (1 << rSC_CLOCK));  // ld a, (0 << rSC_ON) | (1 << rSC_CLOCK)
	LDH_addr_A(rSC);  // ldh [rSC], a
	LD_A((1 << rSC_ON) | (1 << rSC_CLOCK));  // ld a, (1 << rSC_ON) | (1 << rSC_CLOCK)
	LDH_addr_A(rSC);  // ldh [rSC], a


player_1:
	SET_PC(0x089FU);
	CALL(mLinkTransfer_Receive);  // call .Receive
	POP_BC;  // pop bc
	RET;  // ret


Receive:
	SET_PC(0x08A4U);
	LDH_A_addr(hSerialReceive);  // ldh a, [hSerialReceive]
	LD_addr_A(wOtherPlayerLinkMode);  // ld [wOtherPlayerLinkMode], a
	AND_A(0xf0);  // and $f0
	CP_A_B;  // cp b
	RET_NZ ;  // ret nz
	XOR_A_A;  // xor a
	LDH_addr_A(hSerialReceive);  // ldh [hSerialReceive], a
	LD_A_addr(wOtherPlayerLinkMode);  // ld a, [wOtherPlayerLinkMode]
	AND_A(0xf);  // and $f
	LD_addr_A(wOtherPlayerLinkAction);  // ld [wOtherPlayerLinkAction], a
	RET;  // ret

}

int LinkDataReceived(){
	SET_PC(0x08B9U);
//  Let the other system know that the data has been received.
	XOR_A_A;  // xor a
	LDH_addr_A(hSerialSend);  // ldh [hSerialSend], a
	LDH_A_addr(hSerialConnectionStatus);  // ldh a, [hSerialConnectionStatus]
	CP_A(USING_INTERNAL_CLOCK);  // cp USING_INTERNAL_CLOCK
	RET_NZ ;  // ret nz
	LD_A((0 << rSC_ON) | (1 << rSC_CLOCK));  // ld a, (0 << rSC_ON) | (1 << rSC_CLOCK)
	LDH_addr_A(rSC);  // ldh [rSC], a
	LD_A((1 << rSC_ON) | (1 << rSC_CLOCK));  // ld a, (1 << rSC_ON) | (1 << rSC_CLOCK)
	LDH_addr_A(rSC);  // ldh [rSC], a
	RET;  // ret

}

int SetBitsForTimeCapsuleRequestIfNotLinked(){
	SET_PC(0x08CAU);
//  //  unreferenced
//  Similar to SetBitsForTimeCapsuleRequest (see engine/link/link.asm).
	LD_A_addr(wLinkMode);  // ld a, [wLinkMode]
	AND_A_A;  // and a
	RET_NZ ;  // ret nz
	LD_A(USING_INTERNAL_CLOCK);  // ld a, USING_INTERNAL_CLOCK
	LDH_addr_A(rSB);  // ldh [rSB], a
	XOR_A_A;  // xor a
	LDH_addr_A(hSerialReceive);  // ldh [hSerialReceive], a
	LD_A((0 << rSC_ON) | (0 << rSC_CLOCK));  // ld a, (0 << rSC_ON) | (0 << rSC_CLOCK)
	LDH_addr_A(rSC);  // ldh [rSC], a
	LD_A((1 << rSC_ON) | (0 << rSC_CLOCK));  // ld a, (1 << rSC_ON) | (0 << rSC_CLOCK)
	LDH_addr_A(rSC);  // ldh [rSC], a
	RET;  // ret

}
