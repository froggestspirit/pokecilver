#include "../constants.h"
#include "vblank.h"
//  VBlank is the interrupt responsible for updating VRAM.

//  In Pokemon Gold and Silver, VBlank has been hijacked to act as the
//  main loop. After time-sensitive graphics operations have been
//  performed, joypad input and sound functions are executed.

//  This prevents the display and audio output from lagging.

void TransferVirtualOAM(void){
    // initiate DMA
    LD_A(HIGH(wVirtualOAM));
    LDH_addr_A(rDMA);
    // wait for DMA to finish
    LD_A(NUM_SPRITE_OAM_STRUCTS);
wait:
    DEC_A;
    IF_NZ goto wait;
    return;
}

void VBlank(void){
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

void VBlank0(void){
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
    CCALL(aUpdateBGMap);  // call UpdateBGMap

// ; These have their own timing checks.

    CCALL(aServe2bppRequest);  // call Serve2bppRequest
    CCALL(aServe1bppRequest);  // call Serve1bppRequest
    //CALL(mAnimateTileset);  // call AnimateTileset //Temporarily disable
    CCALL(aFillBGMap0WithBlack);  // call FillBGMap0WithBlack


done:

    LDH_A_addr(hOAMUpdate);  // ldh a, [hOAMUpdate]
    AND_A_A;  // and a
    IF_NZ goto done_oam;  // jr nz, .done_oam
    TransferVirtualOAM();  // call hTransferVirtualOAM

done_oam:

// ; vblank-sensitive operations are done

    XOR_A_A;  // xor a
    LD_addr_A(wVBlankOccurred);  // ld [wVBlankOccurred], a

    LD_A_addr(wOverworldDelay);  // ld a, [wOverworldDelay]
    AND_A_A;  // and a
    IF_Z goto ok;  // jr z, .ok
    DEC_A;  // dec a
    LD_addr_A(wOverworldDelay);  // ld [wOverworldDelay], a

ok:

    LD_A_addr(wTextDelayFrames);  // ld a, [wTextDelayFrames]
    AND_A_A;  // and a
    IF_Z goto ok2;  // jr z, .ok2
    DEC_A;  // dec a
    LD_addr_A(wTextDelayFrames);  // ld [wTextDelayFrames], a

ok2:

    CCALL(aUpdateJoypad);  // call UpdateJoypad

    LD_A(BANK(av_UpdateSound));  // ld a, BANK(_UpdateSound)
    Bankswitch();
    v_UpdateSound();  // call _UpdateSound
    LD_A_addr(wROMBankBackup);  // ld a, [wROMBankBackup]
    Bankswitch();

    LDH_A_addr(hSeconds);  // ldh a, [hSeconds]
    LDH_addr_A(hUnusedBackup);  // ldh [hUnusedBackup], a

    RET;  // ret

}

void VBlank1(void){
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
    CCALL(aUpdatePals);  // call UpdatePals
    IF_C goto done;  // jr c, .done

    CCALL(aUpdateBGMap);  // call UpdateBGMap
    CCALL(aServe2bppRequest);  // call Serve2bppRequest

    TransferVirtualOAM();  // call hTransferVirtualOAM


done:
    LDH_A_addr(hLCDCPointer);  // ldh a, [hLCDCPointer]
    OR_A_A;  // or a
    IF_Z goto skip_lcd;  // jr z, .skip_lcd
    LD_C_A;  // ld c, a
    LD_A_addr(wLYOverrides);  // ld a, [wLYOverrides]
    LDH_c_A;  // ldh [c], a


skip_lcd:
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
    Bankswitch();
    v_UpdateSound();  // call _UpdateSound
    LD_A_addr(wROMBankBackup);  // ld a, [wROMBankBackup]
    Bankswitch();
// ; enable ints
    LD_A(IE_DEFAULT);  // ld a, IE_DEFAULT
    LDH_addr_A(rIE);  // ldh [rIE], a
    RET;  // ret

}

void UpdatePals(void){
//  update pals for either dmg or cgb

    LDH_A_addr(hCGB);  // ldh a, [hCGB]
    AND_A_A;  // and a
    IF_NZ return UpdateCGBPals();  // jp nz, UpdateCGBPals

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

void VBlank4(void){
//  bg map
//  tiles
//  oam
//  joypad
//  serial
//  sound

    LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
    LD_addr_A(wROMBankBackup);  // ld [wROMBankBackup], a

    CCALL(aUpdateBGMap);  // call UpdateBGMap
    CCALL(aServe2bppRequest);  // call Serve2bppRequest

    TransferVirtualOAM();  // call hTransferVirtualOAM

    CCALL(aUpdateJoypad);  // call UpdateJoypad

    XOR_A_A;  // xor a
    LD_addr_A(wVBlankOccurred);  // ld [wVBlankOccurred], a

    CCALL(aAskSerial);  // call AskSerial

    LD_A(BANK(av_UpdateSound));  // ld a, BANK(_UpdateSound)
    Bankswitch();
    v_UpdateSound();  // call _UpdateSound

    LD_A_addr(wROMBankBackup);  // ld a, [wROMBankBackup]
    Bankswitch();
    RET;  // ret

}

void VBlank5(void){
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

    CCALL(aUpdateBGMap);  // call UpdateBGMap
    CCALL(aServe2bppRequest);  // call Serve2bppRequest

done:

    XOR_A_A;  // xor a
    LD_addr_A(wVBlankOccurred);  // ld [wVBlankOccurred], a

    CCALL(aUpdateJoypad);  // call UpdateJoypad

    XOR_A_A;  // xor a
    LDH_addr_A(rIF);  // ldh [rIF], a
    LD_A(1 << LCD_STAT);  // ld a, 1 << LCD_STAT
    LDH_addr_A(rIE);  // ldh [rIE], a
// ; request lcd stat
    LDH_addr_A(rIF);  // ldh [rIF], a

    NOP;  // ei
    LD_A(BANK(av_UpdateSound));  // ld a, BANK(_UpdateSound)
    Bankswitch();
    v_UpdateSound();  // call _UpdateSound
    LD_A_addr(wROMBankBackup);  // ld a, [wROMBankBackup]
    Bankswitch();
    NOP;  // di

    XOR_A_A;  // xor a
    LDH_addr_A(rIF);  // ldh [rIF], a
// ; enable ints
    LD_A(IE_DEFAULT);  // ld a, IE_DEFAULT
    LDH_addr_A(rIE);  // ldh [rIE], a
    RET;  // ret

}

void VBlank2(void){
//  sound only

    LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
    LD_addr_A(wROMBankBackup);  // ld [wROMBankBackup], a

    LD_A(BANK(av_UpdateSound));  // ld a, BANK(_UpdateSound)
    Bankswitch();
    v_UpdateSound();  // call _UpdateSound

    LD_A_addr(wROMBankBackup);  // ld a, [wROMBankBackup]
    Bankswitch();

    XOR_A_A;  // xor a
    LD_addr_A(wVBlankOccurred);  // ld [wVBlankOccurred], a
    RET;  // ret

}

void VBlank3(void){
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

    CCALL(aUpdateJoypad);  // call UpdateJoypad

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

    CCALL(aUpdateBGMap);  // call UpdateBGMap
    CCALL(aUpdateBGMapBuffer);  // call UpdateBGMapBuffer

    CCALL(aServe2bppRequest);  // call Serve2bppRequest
    CCALL(aServe1bppRequest);  // call Serve1bppRequest
    //CALL(mAnimateTileset);  // call AnimateTileset  //Temporarily disable
    TransferVirtualOAM();  // call hTransferVirtualOAM

    XOR_A_A;  // xor a
    LD_addr_A(wVBlankOccurred);  // ld [wVBlankOccurred], a

    LD_A_addr(wTextDelayFrames);  // ld a, [wTextDelayFrames]
    AND_A_A;  // and a
    IF_Z goto okay;  // jr z, .okay
    DEC_A;  // dec a
    LD_addr_A(wTextDelayFrames);  // ld [wTextDelayFrames], a


okay:
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
    Bankswitch();
    v_UpdateSound();  // call _UpdateSound
    LD_A_addr(wROMBankBackup);  // ld a, [wROMBankBackup]
    Bankswitch();
    NOP;  // di

// ; discard requested ints
    XOR_A_A;  // xor a
    LDH_addr_A(rIF);  // ldh [rIF], a
// ; enable ints
    LD_A(IE_DEFAULT);  // ld a, IE_DEFAULT
    LDH_addr_A(rIE);  // ldh [rIE], a
    RET;  // ret

}
