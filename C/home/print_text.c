#include "../constants.h"
#include "print_text.h"

void PrintLetterDelay(void){
    SET_PC(0x31E0U);
//  Wait before printing the next letter.

//  The text speed setting in wOptions is actually a frame count:
//      fast: 1 frame
//      mid:  3 frames
//      slow: 5 frames

//  wTextboxFlags[!0] and A or B override text speed with a one-frame delay.
//  wOptions[4] and wTextboxFlags[!1] disable the delay.

    LD_A_addr(wOptions);  // ld a, [wOptions]
    BIT_A(NO_TEXT_SCROLL);  // bit NO_TEXT_SCROLL, a
    RET_NZ ;  // ret nz

//  non-scrolling text?
    LD_A_addr(wTextboxFlags);  // ld a, [wTextboxFlags]
    BIT_A(NO_TEXT_DELAY_F);  // bit NO_TEXT_DELAY_F, a
    RET_Z ;  // ret z

    PUSH_HL;  // push hl
    PUSH_DE;  // push de
    PUSH_BC;  // push bc

    LD_HL(hOAMUpdate);  // ld hl, hOAMUpdate
    LD_A_hl;  // ld a, [hl]
    PUSH_AF;  // push af

//  orginally turned oam update off...
//     ld a, 1
    LD_hl_A;  // ld [hl], a

//  force fast scroll?
    LD_A_addr(wTextboxFlags);  // ld a, [wTextboxFlags]
    BIT_A(FAST_TEXT_DELAY_F);  // bit FAST_TEXT_DELAY_F, a
    IF_Z goto fast;  // jr z, .fast

//  text speed
    LD_A_addr(wOptions);  // ld a, [wOptions]
    AND_A(0b111);  // and %111
    goto updatedelay;  // jr .updatedelay


fast:
    SET_PC(0x3203U);
    LD_A(TEXT_DELAY_FAST);  // ld a, TEXT_DELAY_FAST


updatedelay:
    SET_PC(0x3205U);
    LD_addr_A(wTextDelayFrames);  // ld [wTextDelayFrames], a


checkjoypad:
    SET_PC(0x3208U);
    CCALL(aGetJoypad);  // call GetJoypad

//  input override
    LD_A_addr(wDisableTextAcceleration);  // ld a, [wDisableTextAcceleration]
    AND_A_A;  // and a
    IF_NZ goto wait;  // jr nz, .wait

//  Wait one frame if holding A or B.
    LDH_A_addr(hJoyDown);  // ldh a, [hJoyDown]
    BIT_A(A_BUTTON_F);  // bit A_BUTTON_F, a
    IF_Z goto checkb;  // jr z, .checkb
    goto delay;  // jr .delay

checkb:
    SET_PC(0x3219U);
    BIT_A(B_BUTTON_F);  // bit B_BUTTON_F, a
    IF_Z goto wait;  // jr z, .wait


delay:
    SET_PC(0x321DU);
    CALL(mDelayFrame);  // call DelayFrame
    goto end;  // jr .end


wait:
    SET_PC(0x3222U);
    LD_A_addr(wTextDelayFrames);  // ld a, [wTextDelayFrames]
    AND_A_A;  // and a
// ;(port fix)jr nz, .checkjoypad


end:
    SET_PC(0x3226U);
    POP_AF;  // pop af
    LDH_addr_A(hOAMUpdate);  // ldh [hOAMUpdate], a
    POP_BC;  // pop bc
    POP_DE;  // pop de
    POP_HL;  // pop hl
    RET;  // ret

}

void CopyDataUntil(void){
    SET_PC(0x322DU);
//  Copy [hl .. bc) to de.

//  In other words, the source data is
//  from hl up to but not including bc,
//  and the destination is de.

    LD_A_hli;  // ld a, [hli]
    LD_de_A;  // ld [de], a
    INC_DE;  // inc de
    LD_A_H;  // ld a, h
    CP_A_B;  // cp b
    JR_NZ (mCopyDataUntil);  // jr nz, CopyDataUntil
    LD_A_L;  // ld a, l
    CP_A_C;  // cp c
    JR_NZ (mCopyDataUntil);  // jr nz, CopyDataUntil
    RET;  // ret

// INCLUDE "home/print_num.asm"

    return FarPrintText();
}

void FarPrintText(void){
    SET_PC(0x3400U);
    LD_addr_A(wTempBank);  // ld [wTempBank], a
    LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
    PUSH_AF;  // push af
    LD_A_addr(wTempBank);  // ld a, [wTempBank]
    RST(mBankswitch);  // rst Bankswitch

    CALL(mPrintText);  // call PrintText

    POP_AF;  // pop af
    RST(mBankswitch);  // rst Bankswitch
    RET;  // ret

}

void CallPointerAt(void){
    SET_PC(0x3410U);
    LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
    PUSH_AF;  // push af
    LD_A_hli;  // ld a, [hli]
    RST(mBankswitch);  // rst Bankswitch

    LD_A_hli;  // ld a, [hli]
    LD_H_hl;  // ld h, [hl]
    LD_L_A;  // ld l, a

    CALL(mv_hl_);  // call _hl_

    POP_HL;  // pop hl
    LD_A_H;  // ld a, h
    RST(mBankswitch);  // rst Bankswitch
    RET;  // ret

}
