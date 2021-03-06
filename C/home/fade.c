#include "../constants.h"
#include "fade.h"

//  Functions to fade the screen in and out.

void TimeOfDayFade(void){
//  //  unreferenced
    LD_A_addr(wTimeOfDayPal);  // ld a, [wTimeOfDayPal]
    LD_B_A;  // ld b, a
    LD_HL(mIncGradGBPalTable_11);  // ld hl, IncGradGBPalTable_11
    LD_A_L;  // ld a, l
    SUB_A_B;  // sub b
    LD_L_A;  // ld l, a
    IF_NC goto okay;  // jr nc, .okay
    DEC_H;  // dec h


okay:
    LD_A_hli;  // ld a, [hli]
    LDH_addr_A(rBGP);  // ldh [rBGP], a
    LD_A_hli;  // ld a, [hli]
    LDH_addr_A(rOBP0);  // ldh [rOBP0], a
    LD_A_hli;  // ld a, [hli]
    LDH_addr_A(rOBP1);  // ldh [rOBP1], a
    RET;  // ret

}

void RotateFourPalettesRight(void){
    SET_PC(0x0377U);
    LDH_A_addr(hCGB);  // ldh a, [hCGB]
    AND_A_A;  // and a
    IF_Z goto dmg;  // jr z, .dmg
    LD_HL(mIncGradGBPalTable_00);  // ld hl, IncGradGBPalTable_00
    LD_B(4);  // ld b, 4
    JR(mRotatePalettesRight);  // jr RotatePalettesRight


dmg:
    SET_PC(0x0383U);
    LD_HL(mIncGradGBPalTable_08);  // ld hl, IncGradGBPalTable_08
    LD_B(4);  // ld b, 4
    JR(mRotatePalettesRight);  // jr RotatePalettesRight

}

void RotateThreePalettesRight(void){
    SET_PC(0x038AU);
    LDH_A_addr(hCGB);  // ldh a, [hCGB]
    AND_A_A;  // and a
    IF_Z goto dmg;  // jr z, .dmg
    LD_HL(mIncGradGBPalTable_05);  // ld hl, IncGradGBPalTable_05
    LD_B(3);  // ld b, 3
    JR(mRotatePalettesRight);  // jr RotatePalettesRight


dmg:
    SET_PC(0x0396U);
    LD_HL(mIncGradGBPalTable_13);  // ld hl, IncGradGBPalTable_13
    LD_B(3);  // ld b, 3
    return RotatePalettesRight();
}

void RotatePalettesRight(void){
    SET_PC(0x039BU);
//  Rotate palettes to the right and fill with loaded colors from the left
//  If we're already at the leftmost color, fill with the leftmost color
    PUSH_DE;  // push de
    LD_A_hli;  // ld a, [hli]
    CALL(mDmgToCgbBGPals);  // call DmgToCgbBGPals
    LD_A_hli;  // ld a, [hli]
    LD_E_A;  // ld e, a
    LD_A_hli;  // ld a, [hli]
    LD_D_A;  // ld d, a
    CALL(mDmgToCgbObjPals);  // call DmgToCgbObjPals
    LD_C(8);  // ld c, 8
    CALL(mDelayFrames);  // call DelayFrames
    POP_DE;  // pop de
    DEC_B;  // dec b
    JR_NZ (mRotatePalettesRight);  // jr nz, RotatePalettesRight
    RET;  // ret

}

void RotateFourPalettesLeft(void){
    SET_PC(0x03B1U);
    LDH_A_addr(hCGB);  // ldh a, [hCGB]
    AND_A_A;  // and a
    IF_Z goto dmg;  // jr z, .dmg
    LD_HL(mIncGradGBPalTable_04 - 1);  // ld hl, IncGradGBPalTable_04 - 1
    LD_B(4);  // ld b, 4
    JR(mRotatePalettesLeft);  // jr RotatePalettesLeft


dmg:
    SET_PC(0x03BDU);
    LD_HL(mIncGradGBPalTable_12 - 1);  // ld hl, IncGradGBPalTable_12 - 1
    LD_B(4);  // ld b, 4
    JR(mRotatePalettesLeft);  // jr RotatePalettesLeft

}

void RotateThreePalettesLeft(void){
    SET_PC(0x03C4U);
    LDH_A_addr(hCGB);  // ldh a, [hCGB]
    AND_A_A;  // and a
    IF_Z goto dmg;  // jr z, .dmg
    LD_HL(mIncGradGBPalTable_07 - 1);  // ld hl, IncGradGBPalTable_07 - 1
    LD_B(3);  // ld b, 3
    JR(mRotatePalettesLeft);  // jr RotatePalettesLeft


dmg:
    SET_PC(0x03D0U);
    LD_HL(mIncGradGBPalTable_15 - 1);  // ld hl, IncGradGBPalTable_15 - 1
    LD_B(3);  // ld b, 3
    return RotatePalettesLeft();
}

void RotatePalettesLeft(void){
    SET_PC(0x03D5U);
//  Rotate palettes to the left and fill with loaded colors from the right
//  If we're already at the rightmost color, fill with the rightmost color
    PUSH_DE;  // push de
    LD_A_hld;  // ld a, [hld]
    LD_D_A;  // ld d, a
    LD_A_hld;  // ld a, [hld]
    LD_E_A;  // ld e, a
    CALL(mDmgToCgbObjPals);  // call DmgToCgbObjPals
    LD_A_hld;  // ld a, [hld]
    CALL(mDmgToCgbBGPals);  // call DmgToCgbBGPals
    LD_C(8);  // ld c, 8
    CALL(mDelayFrames);  // call DelayFrames
    POP_DE;  // pop de
    DEC_B;  // dec b
    JR_NZ (mRotatePalettesLeft);  // jr nz, RotatePalettesLeft
    RET;  // ret

}

void IncGradGBPalTable_00(void){
//dc 3,3,3,3, 3,3,3,3, 3,3,3,3
    return IncGradGBPalTable_01();
}

void IncGradGBPalTable_01(void){
//dc 3,3,3,2, 3,3,3,2, 3,3,3,2
    return IncGradGBPalTable_02();
}

void IncGradGBPalTable_02(void){
//dc 3,3,2,1, 3,3,2,1, 3,3,2,1
    return IncGradGBPalTable_03();
}

void IncGradGBPalTable_03(void){
//dc 3,2,1,0, 3,2,1,0, 3,2,1,0

    return IncGradGBPalTable_04();
}

void IncGradGBPalTable_04(void){
//dc 3,2,1,0, 3,2,1,0, 3,2,1,0
    return IncGradGBPalTable_05();
}

void IncGradGBPalTable_05(void){
//dc 2,1,0,0, 2,1,0,0, 2,1,0,0
    return IncGradGBPalTable_06();
}

void IncGradGBPalTable_06(void){
//dc 1,0,0,0, 1,0,0,0, 1,0,0,0

    return IncGradGBPalTable_07();
}

void IncGradGBPalTable_07(void){
//dc 0,0,0,0, 0,0,0,0, 0,0,0,0
//                            bgp      obp1     obp2
    return IncGradGBPalTable_08();
}

void IncGradGBPalTable_08(void){
//dc 3,3,3,3, 3,3,3,3, 3,3,3,3
    return IncGradGBPalTable_09();
}

void IncGradGBPalTable_09(void){
//dc 3,3,3,2, 3,3,3,2, 3,3,2,0
    return IncGradGBPalTable_10();
}

void IncGradGBPalTable_10(void){
//dc 3,3,2,1, 3,2,1,0, 3,2,1,0
    return IncGradGBPalTable_11();
}

void IncGradGBPalTable_11(void){
//dc 3,2,1,0, 3,1,0,0, 3,2,0,0

    return IncGradGBPalTable_12();
}

void IncGradGBPalTable_12(void){
//dc 3,2,1,0, 3,1,0,0, 3,2,0,0
    return IncGradGBPalTable_13();
}

void IncGradGBPalTable_13(void){
//dc 2,1,0,0, 2,0,0,0, 2,1,0,0
    return IncGradGBPalTable_14();
}

void IncGradGBPalTable_14(void){
//dc 1,0,0,0, 1,0,0,0, 1,0,0,0

    return IncGradGBPalTable_15();
}

void IncGradGBPalTable_15(void){
//dc 0,0,0,0, 0,0,0,0, 0,0,0,0

}
