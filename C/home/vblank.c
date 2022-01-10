#include "../constants.h"
#include "vblank.h"
//  VBlank is the interrupt responsible for updating VRAM.

//  In Pokemon Gold and Silver, VBlank has been hijacked to act as the
//  main loop. After time-sensitive graphics operations have been
//  performed, joypad input and sound functions are executed.

//  This prevents the display and audio output from lagging.


int VBlank(){
	SET_PC(0x0150U);
	static int (*VBlanks[8])() = {VBlank0,
								VBlank1,
								VBlank2,
								VBlank3,
								VBlank4,
								VBlank5,
								VBlank0,
								VBlank0};
	PUSH_AF;  // push af
	PUSH_BC;  // push bc
	PUSH_DE;  // push de
	PUSH_HL;  // push hl

	LD_DE(mVBlank_return);  // ld de, .return
	PUSH_DE;  // push de
	return VBlanks[gb_read(hVBlank)]();


l_return:
	SET_PC(0x0168U);
	CCALL(aGameTimer);  // call GameTimer

	POP_HL;  // pop hl
	POP_DE;  // pop de
	POP_BC;  // pop bc
	POP_AF;  // pop af
	RET;  // ret
}

int VBlank0(){
	SET_PC(0x0180U);
//  normal operation

//  rng
//  scx, scy, wy, wx
//  bg map buffer
//  palettes
//  dma transfer
//  bg map
//  tiles
//  oam
//  joypad
//  sound

// ; inc frame counter
	LD_HL(hVBlankCounter);  // ld hl, hVBlankCounter
	INC_hl;  // inc [hl]

// ; advance random variables
	LDH_A_addr(rDIV);  // ldh a, [rDIV]
	LD_B_A;  // ld b, a
	LDH_A_addr(hRandomAdd);  // ldh a, [hRandomAdd]
	ADC_A_B;  // adc b
	LDH_addr_A(hRandomAdd);  // ldh [hRandomAdd], a

	LDH_A_addr(rDIV);  // ldh a, [rDIV]
	LD_B_A;  // ld b, a
	LDH_A_addr(hRandomSub);  // ldh a, [hRandomSub]
	SBC_A_B;  // sbc b
	LDH_addr_A(hRandomSub);  // ldh [hRandomSub], a

	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	LD_addr_A(wROMBankBackup);  // ld [wROMBankBackup], a

	LDH_A_addr(hSCX);  // ldh a, [hSCX]
	LDH_addr_A(rSCX);  // ldh [rSCX], a
	LDH_A_addr(hSCY);  // ldh a, [hSCY]
	LDH_addr_A(rSCY);  // ldh [rSCY], a
	LDH_A_addr(hWY);  // ldh a, [hWY]
	LDH_addr_A(rWY);  // ldh [rWY], a
	LDH_A_addr(hWX);  // ldh a, [hWX]
	LDH_addr_A(rWX);  // ldh [rWX], a

// ; There's only time to call one of these in one vblank.
// ; Calls are in order of priority.

	CCALL(aUpdateBGMapBuffer);  // call UpdateBGMapBuffer
	IF_C goto done;  // jr c, .done
	CCALL(aUpdatePalsIfCGB);  // call UpdatePalsIfCGB
	IF_C goto done;  // jr c, .done
	CALL(mUpdateBGMap);  // call UpdateBGMap

// ; These have their own timing checks.

	CCALL(aServe2bppRequest);  // call Serve2bppRequest
	CCALL(aServe1bppRequest);  // call Serve1bppRequest
	CALL(mAnimateTileset);  // call AnimateTileset
	CCALL(aFillBGMap0WithBlack);  // call FillBGMap0WithBlack


done:
	SET_PC(0x01C2U);

	LDH_A_addr(hOAMUpdate);  // ldh a, [hOAMUpdate]
	AND_A_A;  // and a
	IF_NZ goto done_oam;  // jr nz, .done_oam
	CALL(hTransferVirtualOAM);  // call hTransferVirtualOAM

done_oam:
	SET_PC(0x01CAU);

// ; vblank-sensitive operations are done

	XOR_A_A;  // xor a
	LD_addr_A(wVBlankOccurred);  // ld [wVBlankOccurred], a

	LD_A_addr(wOverworldDelay);  // ld a, [wOverworldDelay]
	AND_A_A;  // and a
	IF_Z goto ok;  // jr z, .ok
	DEC_A;  // dec a
	LD_addr_A(wOverworldDelay);  // ld [wOverworldDelay], a

ok:
	SET_PC(0x01D8U);

	LD_A_addr(wTextDelayFrames);  // ld a, [wTextDelayFrames]
	AND_A_A;  // and a
	IF_Z goto ok2;  // jr z, .ok2
	DEC_A;  // dec a
	LD_addr_A(wTextDelayFrames);  // ld [wTextDelayFrames], a

ok2:
	SET_PC(0x01E2U);

	CALL(mUpdateJoypad);  // call UpdateJoypad

	LD_A(BANK(av_UpdateSound));  // ld a, BANK(_UpdateSound)
	RST(mBankswitch);  // rst Bankswitch
	CCALL(av_UpdateSound);  // call _UpdateSound
	LD_A_addr(wROMBankBackup);  // ld a, [wROMBankBackup]
	RST(mBankswitch);  // rst Bankswitch

	LDH_A_addr(hSeconds);  // ldh a, [hSeconds]
	LDH_addr_A(hUnusedBackup);  // ldh [hUnusedBackup], a

	RET;  // ret

}

int VBlank1(){
	SET_PC(0x01F4U);
//  scx, scy
//  palettes
//  bg map
//  tiles
//  oam
//  sound / lcd stat

	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	LD_addr_A(wROMBankBackup);  // ld [wROMBankBackup], a
	LDH_A_addr(hSCX);  // ldh a, [hSCX]
	LDH_addr_A(rSCX);  // ldh [rSCX], a
	LDH_A_addr(hSCY);  // ldh a, [hSCY]
	LDH_addr_A(rSCY);  // ldh [rSCY], a
	CALL(mUpdatePals);  // call UpdatePals
	IF_C goto done;  // jr c, .done

	CALL(mUpdateBGMap);  // call UpdateBGMap
	CCALL(aServe2bppRequest);  // call Serve2bppRequest

	CALL(hTransferVirtualOAM);  // call hTransferVirtualOAM


done:
	SET_PC(0x020FU);
	LDH_A_addr(hLCDCPointer);  // ldh a, [hLCDCPointer]
	OR_A_A;  // or a
	IF_Z goto skip_lcd;  // jr z, .skip_lcd
	LD_C_A;  // ld c, a
	LD_A_addr(wLYOverrides);  // ld a, [wLYOverrides]
	LDH_c_A;  // ldh [c], a


skip_lcd:
	SET_PC(0x0219U);
	XOR_A_A;  // xor a
	LD_addr_A(wVBlankOccurred);  // ld [wVBlankOccurred], a

// ; get requested ints
	LDH_A_addr(rIF);  // ldh a, [rIF]
	LD_B_A;  // ld b, a
// ; discard requested ints
	XOR_A_A;  // xor a
	LDH_addr_A(rIF);  // ldh [rIF], a
// ; enable lcd stat
	LD_A(1 << LCD_STAT);  // ld a, 1 << LCD_STAT
	LDH_addr_A(rIE);  // ldh [rIE], a
// ; rerequest serial int if applicable (still disabled)
// ; request lcd stat
	LD_A_B;  // ld a, b
	AND_A(1 << SERIAL);  // and 1 << SERIAL
	OR_A(1 << LCD_STAT);  // or 1 << LCD_STAT
	LDH_addr_A(rIF);  // ldh [rIF], a

	NOP;  // ei
	LD_A(BANK(av_UpdateSound));  // ld a, BANK(_UpdateSound)
	RST(mBankswitch);  // rst Bankswitch
	CCALL(av_UpdateSound);  // call _UpdateSound
	LD_A_addr(wROMBankBackup);  // ld a, [wROMBankBackup]
	RST(mBankswitch);  // rst Bankswitch
// ; enable ints
	LD_A(IE_DEFAULT);  // ld a, IE_DEFAULT
	LDH_addr_A(rIE);  // ldh [rIE], a
	RET;  // ret

}

int UpdatePals(){
	SET_PC(0x023EU);
//  update pals for either dmg or cgb

	LDH_A_addr(hCGB);  // ldh a, [hCGB]
	AND_A_A;  // and a
	JP_NZ (mUpdateCGBPals);  // jp nz, UpdateCGBPals

// ; update gb pals
	LD_A_addr(wBGP);  // ld a, [wBGP]
	LDH_addr_A(rBGP);  // ldh [rBGP], a
	LD_A_addr(wOBP0);  // ld a, [wOBP0]
	LDH_addr_A(rOBP0);  // ldh [rOBP0], a
	LD_A_addr(wOBP1);  // ld a, [wOBP1]
	LDH_addr_A(rOBP1);  // ldh [rOBP1], a

	AND_A_A;  // and a
	RET;  // ret

}

int VBlank4(){
	SET_PC(0x0255U);
//  bg map
//  tiles
//  oam
//  joypad
//  serial
//  sound

	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	LD_addr_A(wROMBankBackup);  // ld [wROMBankBackup], a

	CALL(mUpdateBGMap);  // call UpdateBGMap
	CCALL(aServe2bppRequest);  // call Serve2bppRequest

	CALL(hTransferVirtualOAM);  // call hTransferVirtualOAM

	CALL(mUpdateJoypad);  // call UpdateJoypad

	XOR_A_A;  // xor a
	LD_addr_A(wVBlankOccurred);  // ld [wVBlankOccurred], a

	CCALL(aAskSerial);  // call AskSerial

	LD_A(BANK(av_UpdateSound));  // ld a, BANK(_UpdateSound)
	RST(mBankswitch);  // rst Bankswitch
	CCALL(av_UpdateSound);  // call _UpdateSound

	LD_A_addr(wROMBankBackup);  // ld a, [wROMBankBackup]
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret

}

int VBlank5(){
	SET_PC(0x0278U);
//  scx
//  palettes
//  bg map
//  tiles
//  joypad
//

	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	LD_addr_A(wROMBankBackup);  // ld [wROMBankBackup], a

	LDH_A_addr(hSCX);  // ldh a, [hSCX]
	LDH_addr_A(rSCX);  // ldh [rSCX], a

	CCALL(aUpdatePalsIfCGB);  // call UpdatePalsIfCGB
	IF_C goto done;  // jr c, .done

	CALL(mUpdateBGMap);  // call UpdateBGMap
	CCALL(aServe2bppRequest);  // call Serve2bppRequest

done:
	SET_PC(0x028CU);

	XOR_A_A;  // xor a
	LD_addr_A(wVBlankOccurred);  // ld [wVBlankOccurred], a

	CALL(mUpdateJoypad);  // call UpdateJoypad

	XOR_A_A;  // xor a
	LDH_addr_A(rIF);  // ldh [rIF], a
	LD_A(1 << LCD_STAT);  // ld a, 1 << LCD_STAT
	LDH_addr_A(rIE);  // ldh [rIE], a
// ; request lcd stat
	LDH_addr_A(rIF);  // ldh [rIF], a

	NOP;  // ei
	LD_A(BANK(av_UpdateSound));  // ld a, BANK(_UpdateSound)
	RST(mBankswitch);  // rst Bankswitch
	CCALL(av_UpdateSound);  // call _UpdateSound
	LD_A_addr(wROMBankBackup);  // ld a, [wROMBankBackup]
	RST(mBankswitch);  // rst Bankswitch
	NOP;  // di

	XOR_A_A;  // xor a
	LDH_addr_A(rIF);  // ldh [rIF], a
// ; enable ints
	LD_A(IE_DEFAULT);  // ld a, IE_DEFAULT
	LDH_addr_A(rIE);  // ldh [rIE], a
	RET;  // ret

}

int VBlank2(){
	SET_PC(0x02B0U);
//  sound only

	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	LD_addr_A(wROMBankBackup);  // ld [wROMBankBackup], a

	LD_A(BANK(av_UpdateSound));  // ld a, BANK(_UpdateSound)
	RST(mBankswitch);  // rst Bankswitch
	CCALL(av_UpdateSound);  // call _UpdateSound

	LD_A_addr(wROMBankBackup);  // ld a, [wROMBankBackup]
	RST(mBankswitch);  // rst Bankswitch

	XOR_A_A;  // xor a
	LD_addr_A(wVBlankOccurred);  // ld [wVBlankOccurred], a
	RET;  // ret

}

int VBlank3(){
	SET_PC(0x02C4U);
//  scx, scy
//  palettes
//  bg map
//  tiles
//  oam
//  sound / lcd stat

	LDH_A_addr(hVBlankCounter);  // ldh a, [hVBlankCounter]
	INC_A;  // inc a
	LDH_addr_A(hVBlankCounter);  // ldh [hVBlankCounter], a

	LDH_A_addr(rDIV);  // ldh a, [rDIV]
	LD_B_A;  // ld b, a
	LDH_A_addr(hRandomAdd);  // ldh a, [hRandomAdd]
	ADC_A_B;  // adc b
	LDH_addr_A(hRandomAdd);  // ldh [hRandomAdd], a

	LDH_A_addr(rDIV);  // ldh a, [rDIV]
	LD_B_A;  // ld b, a
	LDH_A_addr(hRandomSub);  // ldh a, [hRandomSub]
	SBC_A_B;  // sbc b
	LDH_addr_A(hRandomSub);  // ldh [hRandomSub], a

	CALL(mUpdateJoypad);  // call UpdateJoypad

	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	LD_addr_A(wROMBankBackup);  // ld [wROMBankBackup], a

	LDH_A_addr(hSCX);  // ldh a, [hSCX]
	LDH_addr_A(rSCX);  // ldh [rSCX], a
	LDH_A_addr(hSCY);  // ldh a, [hSCY]
	LDH_addr_A(rSCY);  // ldh [rSCY], a

	LDH_A_addr(hWY);  // ldh a, [hWY]
	LDH_addr_A(rWY);  // ldh [rWY], a
	LDH_A_addr(hWX);  // ldh a, [hWX]
	LDH_addr_A(rWX);  // ldh [rWX], a

	CALL(mUpdateBGMap);  // call UpdateBGMap
	CCALL(aUpdateBGMapBuffer);  // call UpdateBGMapBuffer

	CCALL(aServe2bppRequest);  // call Serve2bppRequest
	CCALL(aServe1bppRequest);  // call Serve1bppRequest
	CALL(mAnimateTileset);  // call AnimateTileset
	CALL(hTransferVirtualOAM);  // call hTransferVirtualOAM

	XOR_A_A;  // xor a
	LD_addr_A(wVBlankOccurred);  // ld [wVBlankOccurred], a

	LD_A_addr(wTextDelayFrames);  // ld a, [wTextDelayFrames]
	AND_A_A;  // and a
	IF_Z goto okay;  // jr z, .okay
	DEC_A;  // dec a
	LD_addr_A(wTextDelayFrames);  // ld [wTextDelayFrames], a


okay:
	SET_PC(0x0311U);
// ; discard requested ints
	XOR_A_A;  // xor a
	LDH_addr_A(rIF);  // ldh [rIF], a
// ; enable lcd stat
	LD_A(1 << LCD_STAT);  // ld a, 1 << LCD_STAT
	LDH_addr_A(rIE);  // ldh [rIE], a
// ; request lcd stat
	LDH_addr_A(rIF);  // ldh [rIF], a

	NOP;  // ei
	LD_A(BANK(av_UpdateSound));  // ld a, BANK(_UpdateSound)
	RST(mBankswitch);  // rst Bankswitch
	CCALL(av_UpdateSound);  // call _UpdateSound
	LD_A_addr(wROMBankBackup);  // ld a, [wROMBankBackup]
	RST(mBankswitch);  // rst Bankswitch
	NOP;  // di

// ; discard requested ints
	XOR_A_A;  // xor a
	LDH_addr_A(rIF);  // ldh [rIF], a
// ; enable ints
	LD_A(IE_DEFAULT);  // ld a, IE_DEFAULT
	LDH_addr_A(rIE);  // ldh [rIE], a
	RET;  // ret

}
