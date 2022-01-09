#include "../constants.h"

int Joypad(){
	SET_PC(0x08DFU);
//  Replaced by UpdateJoypad, called from VBlank instead of the useless
//  joypad interrupt.

//  This is a placeholder in case the interrupt is somehow enabled.
	RET;  // ret

}

int ClearJoypad(){
	SET_PC(0x08E0U);
	XOR_A_A;  // xor a
//  Pressed this frame (delta)
	LDH_addr_A(hJoyPressed);  // ldh [hJoyPressed], a
//  Currently pressed
	LDH_addr_A(hJoyDown);  // ldh [hJoyDown], a
	RET;  // ret

}

int UpdateJoypad(){
	SET_PC(0x08E6U);
//  This is called automatically every frame in VBlank.
//  Read the joypad register and translate it to something more
//  workable for use in-game. There are 8 buttons, so we can use
//  one byte to contain all player input.

//  Updates:

//  hJoypadReleased: released this frame (delta)
//  hJoypadPressed: pressed this frame (delta)
//  hJoypadDown: currently pressed
//  hJoypadSum: pressed so far

//  Any of these three bits can be used to disable input.
	LD_A_addr(wJoypadDisable);  // ld a, [wJoypadDisable]
	AND_A((1 << JOYPAD_DISABLE_MON_FAINT_F) | (1 << JOYPAD_DISABLE_SGB_TRANSFER_F) | (1 << 4));  // and (1 << JOYPAD_DISABLE_MON_FAINT_F) | (1 << JOYPAD_DISABLE_SGB_TRANSFER_F) | (1 << 4)
	RET_NZ ;  // ret nz

//  If we're saving, input is disabled.
	LD_A_addr(wGameLogicPaused);  // ld a, [wGameLogicPaused]
	AND_A_A;  // and a
	RET_NZ ;  // ret nz

//  We can only get four inputs at a time.
//  We take d-pad first for no particular reason.
	LD_A(R_DPAD);  // ld a, R_DPAD
	LDH_addr_A(rJOYP);  // ldh [rJOYP], a
//  Read twice to give the request time to take.
	LDH_A_addr(rJOYP);  // ldh a, [rJOYP]
	LDH_A_addr(rJOYP);  // ldh a, [rJOYP]

//  The Joypad register output is in the lo nybble (inversed).
//  We make the hi nybble of our new container d-pad input.
	CPL;  // cpl
	AND_A(0xf);  // and $f
	SWAP_A;  // swap a

//  We'll keep this in b for now.
	LD_B_A;  // ld b, a

//  Buttons make 8 total inputs (A, B, Select, Start).
//  We can fit this into one byte.
	LD_A(R_BUTTONS);  // ld a, R_BUTTONS
	LDH_addr_A(rJOYP);  // ldh [rJOYP], a
//  Wait for input to stabilize.
	for(int rept = 0; rept < 6; rept++){
	LDH_A_addr(rJOYP);  // ldh a, [rJOYP]
	}
//  Buttons take the lo nybble.
	CPL;  // cpl
	AND_A(0xf);  // and $f
	OR_A_B;  // or b
	LD_B_A;  // ld b, a

//  Reset the joypad register since we're done with it.
	LD_A(0x30);  // ld a, $30
	LDH_addr_A(rJOYP);  // ldh [rJOYP], a

//  To get the delta we xor the last frame's input with the new one.
	LDH_A_addr(hJoypadDown);  // ldh a, [hJoypadDown] ; last frame
	LD_E_A;  // ld e, a
	XOR_A_B;  // xor b
	LD_D_A;  // ld d, a
//  Released this frame:
	AND_A_E;  // and e
	LDH_addr_A(hJoypadReleased);  // ldh [hJoypadReleased], a
//  Pressed this frame:
	LD_A_D;  // ld a, d
	AND_A_B;  // and b
	LDH_addr_A(hJoypadPressed);  // ldh [hJoypadPressed], a

//  Add any new presses to the list of collective presses:
	LD_C_A;  // ld c, a
	LDH_A_addr(hJoypadSum);  // ldh a, [hJoypadSum]
	OR_A_C;  // or c
	LDH_addr_A(hJoypadSum);  // ldh [hJoypadSum], a

//  Currently pressed:
	LD_A_B;  // ld a, b
	LDH_addr_A(hJoypadDown);  // ldh [hJoypadDown], a

//  Now that we have the input, we can do stuff with it.

//  For example, soft reset:
	AND_A(A_BUTTON | B_BUTTON | SELECT | START);  // and A_BUTTON | B_BUTTON | SELECT | START
	CP_A(A_BUTTON | B_BUTTON | SELECT | START);  // cp  A_BUTTON | B_BUTTON | SELECT | START
	JP_Z (mReset);  // jp z, Reset

	RET;  // ret

}

int GetJoypad(){
	SET_PC(0x0935U);
//  Update mirror joypad input from hJoypadDown (real input)

//  hJoyReleased: released this frame (delta)
//  hJoyPressed: pressed this frame (delta)
//  hJoyDown: currently pressed

//  bit 0 A
//      1 B
//      2 SELECT
//      3 START
//      4 RIGHT
//      5 LEFT
//      6 UP
//      7 DOWN

	PUSH_AF;  // push af
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc

//  The player input can be automated using an input stream.
//  See more below.
	LD_A_addr(wInputType);  // ld a, [wInputType]
	CP_A(AUTO_INPUT);  // cp AUTO_INPUT
	IF_Z goto l_auto;  // jr z, .auto

//  To get deltas, take this and last frame's input.
	LDH_A_addr(hJoypadDown);  // ldh a, [hJoypadDown] ; real input
	LD_B_A;  // ld b, a
	LDH_A_addr(hJoyDown);  // ldh a, [hJoyDown] ; last frame mirror
	LD_E_A;  // ld e, a

//  Released this frame:
	XOR_A_B;  // xor b
	LD_D_A;  // ld d, a
	AND_A_E;  // and e
	LDH_addr_A(hJoyReleased);  // ldh [hJoyReleased], a

//  Pressed this frame:
	LD_A_D;  // ld a, d
	AND_A_B;  // and b
	LDH_addr_A(hJoyPressed);  // ldh [hJoyPressed], a

//  It looks like the collective presses got commented out here.
	LD_C_A;  // ld c, a

//  Currently pressed:
	LD_A_B;  // ld a, b
	LDH_addr_A(hJoyDown);  // ldh [hJoyDown], a ; frame input


quit:
	SET_PC(0x0953U);
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	POP_AF;  // pop af
	RET;  // ret


l_auto:
	SET_PC(0x0958U);
//  Use a predetermined input stream (used in the catching tutorial).

//  Stream format: [input][duration]
//  A value of $ff will immediately end the stream.

//  Read from the input stream.
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A_addr(wAutoInputBank);  // ld a, [wAutoInputBank]
	RST(mBankswitch);  // rst Bankswitch

	LD_HL(wAutoInputAddress);  // ld hl, wAutoInputAddress
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a

//  We only update when the input duration has expired.
	LD_A_addr(wAutoInputLength);  // ld a, [wAutoInputLength]
	AND_A_A;  // and a
	IF_Z goto updateauto;  // jr z, .updateauto

//  Until then, don't change anything.
	DEC_A;  // dec a
	LD_addr_A(wAutoInputLength);  // ld [wAutoInputLength], a
	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch
	goto quit;  // jr .quit


updateauto:
	SET_PC(0x0973U);
//  An input of $ff will end the stream.
	LD_A_hli;  // ld a, [hli]
	CP_A(-1);  // cp -1
	IF_Z goto stopauto;  // jr z, .stopauto
	LD_B_A;  // ld b, a

//  A duration of $ff will end the stream indefinitely.
	LD_A_hli;  // ld a, [hli]
	LD_addr_A(wAutoInputLength);  // ld [wAutoInputLength], a
	CP_A(-1);  // cp -1
	IF_NZ goto next;  // jr nz, .next

//  The current input is overwritten.
	DEC_HL;  // dec hl
	DEC_HL;  // dec hl
	LD_B(NO_INPUT);  // ld b, NO_INPUT
	goto finishauto;  // jr .finishauto


next:
	SET_PC(0x0987U);
//  On to the next input...
	LD_A_L;  // ld a, l
	LD_addr_A(wAutoInputAddress);  // ld [wAutoInputAddress], a
	LD_A_H;  // ld a, h
	LD_addr_A(wAutoInputAddress + 1);  // ld [wAutoInputAddress + 1], a
	goto finishauto;  // jr .finishauto


stopauto:
	SET_PC(0x0991U);
	CCALL(aStopAutoInput);  // call StopAutoInput
	LD_B(NO_INPUT);  // ld b, NO_INPUT


finishauto:
	SET_PC(0x0996U);
	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch
	LD_A_B;  // ld a, b
	LDH_addr_A(hJoyPressed);  // ldh [hJoyPressed], a ; pressed
	LDH_addr_A(hJoyDown);  // ldh [hJoyDown], a ; input
	goto quit;  // jr .quit

	return StartAutoInput();
}

int StartAutoInput(){
	SET_PC(0x099FU);
//  Start reading automated input stream at a:hl.

	LD_addr_A(wAutoInputBank);  // ld [wAutoInputBank], a
	LD_A_L;  // ld a, l
	LD_addr_A(wAutoInputAddress);  // ld [wAutoInputAddress], a
	LD_A_H;  // ld a, h
	LD_addr_A(wAutoInputAddress + 1);  // ld [wAutoInputAddress + 1], a
//  Start reading the stream immediately.
	XOR_A_A;  // xor a
	LD_addr_A(wAutoInputLength);  // ld [wAutoInputLength], a
//  Reset input mirrors.
	XOR_A_A;  // xor a
	LDH_addr_A(hJoyPressed);  // ldh [hJoyPressed], a ; pressed this frame
	LDH_addr_A(hJoyReleased);  // ldh [hJoyReleased], a ; released this frame
	LDH_addr_A(hJoyDown);  // ldh [hJoyDown], a ; currently pressed

	LD_A(AUTO_INPUT);  // ld a, AUTO_INPUT
	LD_addr_A(wInputType);  // ld [wInputType], a
	RET;  // ret

}

int StopAutoInput(){
	SET_PC(0x09BBU);
//  Clear variables related to automated input.
	XOR_A_A;  // xor a
	LD_addr_A(wAutoInputBank);  // ld [wAutoInputBank], a
	LD_addr_A(wAutoInputAddress);  // ld [wAutoInputAddress], a
	LD_addr_A(wAutoInputAddress + 1);  // ld [wAutoInputAddress + 1], a
	LD_addr_A(wAutoInputLength);  // ld [wAutoInputLength], a
//  Back to normal input.
	LD_addr_A(wInputType);  // ld [wInputType], a
	RET;  // ret

}

int JoyTitleScreenInput(){
	SET_PC(0x09CCU);
//  //  unreferenced

loop:
	SET_PC(0x09CCU);
	CALL(mDelayFrame);  // call DelayFrame

	PUSH_BC;  // push bc
	CALL(mJoyTextDelay);  // call JoyTextDelay
	POP_BC;  // pop bc

//  Save data can be deleted by pressing Up + B + Select.
	LDH_A_addr(hJoyDown);  // ldh a, [hJoyDown]
	CP_A(D_UP | SELECT | B_BUTTON);  // cp D_UP | SELECT | B_BUTTON
	IF_Z goto keycombo;  // jr z, .keycombo

//  Press Start or A to start the game.
	LDH_A_addr(hJoyLast);  // ldh a, [hJoyLast]
	AND_A(START | A_BUTTON);  // and START | A_BUTTON
	IF_NZ goto keycombo;  // jr nz, .keycombo

	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop

	AND_A_A;  // and a
	RET;  // ret


keycombo:
	SET_PC(0x09E5U);
	SCF;  // scf
	RET;  // ret

}

int JoyWaitAorB(){
	SET_PC(0x09E7U);

loop:
	SET_PC(0x09E7U);
	CALL(mDelayFrame);  // call DelayFrame
	CALL(mGetJoypad);  // call GetJoypad
	LDH_A_addr(hJoyPressed);  // ldh a, [hJoyPressed]
	AND_A(A_BUTTON | B_BUTTON);  // and A_BUTTON | B_BUTTON
	RET_NZ ;  // ret nz
	CALL(mUpdateTimeAndPals);  // call UpdateTimeAndPals
	goto loop;  // jr .loop

}

int WaitButton(){
	SET_PC(0x09F7U);
	LDH_A_addr(hOAMUpdate);  // ldh a, [hOAMUpdate]
	PUSH_AF;  // push af
	LD_A(1);  // ld a, 1
	LDH_addr_A(hOAMUpdate);  // ldh [hOAMUpdate], a
	CALL(mWaitBGMap);  // call WaitBGMap
	CALL(mJoyWaitAorB);  // call JoyWaitAorB
	POP_AF;  // pop af
	LDH_addr_A(hOAMUpdate);  // ldh [hOAMUpdate], a
	RET;  // ret

}

int JoyTextDelay(){
	SET_PC(0x0A08U);
	CALL(mGetJoypad);  // call GetJoypad
	LDH_A_addr(hInMenu);  // ldh a, [hInMenu]
	AND_A_A;  // and a
	LDH_A_addr(hJoyPressed);  // ldh a, [hJoyPressed]
	IF_Z goto ok;  // jr z, .ok
	LDH_A_addr(hJoyDown);  // ldh a, [hJoyDown]

ok:
	SET_PC(0x0A14U);
	LDH_addr_A(hJoyLast);  // ldh [hJoyLast], a
	LDH_A_addr(hJoyPressed);  // ldh a, [hJoyPressed]
	AND_A_A;  // and a
	IF_Z goto checkframedelay;  // jr z, .checkframedelay
	LD_A(15);  // ld a, 15
	LD_addr_A(wTextDelayFrames);  // ld [wTextDelayFrames], a
	RET;  // ret


checkframedelay:
	SET_PC(0x0A21U);
	LD_A_addr(wTextDelayFrames);  // ld a, [wTextDelayFrames]
	AND_A_A;  // and a
	IF_Z goto restartframedelay;  // jr z, .restartframedelay
	XOR_A_A;  // xor a
	LDH_addr_A(hJoyLast);  // ldh [hJoyLast], a
	RET;  // ret


restartframedelay:
	SET_PC(0x0A2BU);
	LD_A(5);  // ld a, 5
	LD_addr_A(wTextDelayFrames);  // ld [wTextDelayFrames], a
	RET;  // ret

}

int WaitPressAorB_BlinkCursor(){
	SET_PC(0x0A31U);
//  Show a blinking cursor in the lower right-hand
//  corner of a textbox and wait until A or B is
//  pressed.
//
//  NOTE: The cursor has to be shown before calling
//  this function or no cursor will be shown at all.
	LDH_A_addr(hMapObjectIndex);  // ldh a, [hMapObjectIndex]
	PUSH_AF;  // push af
	LDH_A_addr(hObjectStructIndex);  // ldh a, [hObjectStructIndex]
	PUSH_AF;  // push af
	XOR_A_A;  // xor a
	LDH_addr_A(hMapObjectIndex);  // ldh [hMapObjectIndex], a
	LD_A(6);  // ld a, 6
	LDH_addr_A(hObjectStructIndex);  // ldh [hObjectStructIndex], a


loop:
	SET_PC(0x0A3EU);
	PUSH_HL;  // push hl
	hlcoord(18, 17, wTilemap);  // hlcoord 18, 17
	CCALL(aBlinkCursor);  // call BlinkCursor
	POP_HL;  // pop hl

	CALL(mJoyTextDelay);  // call JoyTextDelay
	LDH_A_addr(hJoyLast);  // ldh a, [hJoyLast]
	AND_A(A_BUTTON | B_BUTTON);  // and A_BUTTON | B_BUTTON
// ;(port fix)jr z, .loop

	POP_AF;  // pop af
	LDH_addr_A(hObjectStructIndex);  // ldh [hObjectStructIndex], a
	POP_AF;  // pop af
	LDH_addr_A(hMapObjectIndex);  // ldh [hMapObjectIndex], a
	RET;  // ret

}

int SimpleWaitPressAorB(){
	SET_PC(0x0A54U);

loop:
	SET_PC(0x0A54U);
	CALL(mJoyTextDelay);  // call JoyTextDelay
	LDH_A_addr(hJoyLast);  // ldh a, [hJoyLast]
	AND_A(A_BUTTON | B_BUTTON);  // and A_BUTTON | B_BUTTON
	IF_Z goto loop;  // jr z, .loop
	RET;  // ret

}

int PromptButton(){
	SET_PC(0x0A5EU);
//  Show a blinking cursor in the lower right-hand
//  corner of a textbox and wait until A or B is
//  pressed, afterwards, play a sound.
	LD_A_addr(wLinkMode);  // ld a, [wLinkMode]
	AND_A_A;  // and a
	IF_NZ goto link;  // jr nz, .link
	CALL(mPromptButton_wait_input);  // call .wait_input
	PUSH_DE;  // push de
	LD_DE(SFX_READ_TEXT_2);  // ld de, SFX_READ_TEXT_2
	CCALL(aPlaySFX);  // call PlaySFX
	POP_DE;  // pop de
	RET;  // ret


link:
	SET_PC(0x0A70U);
	LD_C(65);  // ld c, 65
	JP(mDelayFrames);  // jp DelayFrames


wait_input:
	SET_PC(0x0A75U);
	LDH_A_addr(hOAMUpdate);  // ldh a, [hOAMUpdate]
	PUSH_AF;  // push af
	LD_A(0x1);  // ld a, $1
	LDH_addr_A(hOAMUpdate);  // ldh [hOAMUpdate], a
	LD_A_addr(wInputType);  // ld a, [wInputType]
	OR_A_A;  // or a
	IF_Z goto input_wait_loop;  // jr z, .input_wait_loop
	FARCALL(av_DudeAutoInput_A);  // farcall _DudeAutoInput_A


input_wait_loop:
	SET_PC(0x0A88U);
	CALL(mPromptButton_blink_cursor);  // call .blink_cursor
	CALL(mJoyTextDelay);  // call JoyTextDelay
	LDH_A_addr(hJoyPressed);  // ldh a, [hJoyPressed]
	AND_A(A_BUTTON | B_BUTTON);  // and A_BUTTON | B_BUTTON
	IF_NZ goto received_input;  // jr nz, .received_input
	CALL(mUpdateTimeAndPals);  // call UpdateTimeAndPals
	LD_A(0x1);  // ld a, $1
	LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a
	CALL(mDelayFrame);  // call DelayFrame
	goto input_wait_loop;  // jr .input_wait_loop


received_input:
	SET_PC(0x0AA0U);
	POP_AF;  // pop af
	LDH_addr_A(hOAMUpdate);  // ldh [hOAMUpdate], a
	RET;  // ret


blink_cursor:
	SET_PC(0x0AA4U);
	LDH_A_addr(hVBlankCounter);  // ldh a, [hVBlankCounter]
	AND_A(0b00010000);  // and %00010000 ; bit 4, a
	IF_Z goto cursor_off;  // jr z, .cursor_off
	LD_A(0xee);  // ld a, "▼"
	goto load_cursor_state;  // jr .load_cursor_state


cursor_off:
	SET_PC(0x0AAEU);
	LD_A(0x7a);  // ld a, "─"


load_cursor_state:
	SET_PC(0x0AB0U);
	ldcoord_a(18, 17, wTilemap);  // ldcoord_a 18, 17
	RET;  // ret

}

int BlinkCursor(){
	SET_PC(0x0AB4U);
	PUSH_BC;  // push bc
	LD_A_hl;  // ld a, [hl]
	LD_B_A;  // ld b, a
	LD_A(0xee);  // ld a, "▼"
	CP_A_B;  // cp b
	POP_BC;  // pop bc
	IF_NZ goto place_arrow;  // jr nz, .place_arrow
	LDH_A_addr(hMapObjectIndex);  // ldh a, [hMapObjectIndex]
	DEC_A;  // dec a
	LDH_addr_A(hMapObjectIndex);  // ldh [hMapObjectIndex], a
	RET_NZ ;  // ret nz
	LDH_A_addr(hObjectStructIndex);  // ldh a, [hObjectStructIndex]
	DEC_A;  // dec a
	LDH_addr_A(hObjectStructIndex);  // ldh [hObjectStructIndex], a
	RET_NZ ;  // ret nz
	LD_A(0x7a);  // ld a, "─"
	LD_hl_A;  // ld [hl], a
	LD_A(-1);  // ld a, -1
	LDH_addr_A(hMapObjectIndex);  // ldh [hMapObjectIndex], a
	LD_A(6);  // ld a, 6
	LDH_addr_A(hObjectStructIndex);  // ldh [hObjectStructIndex], a
	RET;  // ret


place_arrow:
	SET_PC(0x0AD5U);
	LDH_A_addr(hMapObjectIndex);  // ldh a, [hMapObjectIndex]
	AND_A_A;  // and a
	RET_Z ;  // ret z
	DEC_A;  // dec a
	LDH_addr_A(hMapObjectIndex);  // ldh [hMapObjectIndex], a
	RET_NZ ;  // ret nz
	DEC_A;  // dec a
	LDH_addr_A(hMapObjectIndex);  // ldh [hMapObjectIndex], a
	LDH_A_addr(hObjectStructIndex);  // ldh a, [hObjectStructIndex]
	DEC_A;  // dec a
	LDH_addr_A(hObjectStructIndex);  // ldh [hObjectStructIndex], a
	RET_NZ ;  // ret nz
	LD_A(6);  // ld a, 6
	LDH_addr_A(hObjectStructIndex);  // ldh [hObjectStructIndex], a
	LD_A(0xee);  // ld a, "▼"
	LD_hl_A;  // ld [hl], a
	RET;  // ret

}
