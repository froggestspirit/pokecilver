#include "../constants.h"

void IsInJohto(void){
//  Return 0 if the player is in Johto, and 1 in Kanto.

    LD_A_addr(wMapGroup);  // ld a, [wMapGroup]
    LD_B_A;  // ld b, a
    LD_A_addr(wMapNumber);  // ld a, [wMapNumber]
    LD_C_A;  // ld c, a
    CCALL(aGetWorldMapLocation);  // call GetWorldMapLocation

    CP_A(LANDMARK_FAST_SHIP);  // cp LANDMARK_FAST_SHIP
    IF_Z goto Johto;  // jr z, .Johto

    CP_A(LANDMARK_SPECIAL);  // cp LANDMARK_SPECIAL
    IF_NZ goto CheckRegion;  // jr nz, .CheckRegion

    LD_A_addr(wBackupMapGroup);  // ld a, [wBackupMapGroup]
    LD_B_A;  // ld b, a
    LD_A_addr(wBackupMapNumber);  // ld a, [wBackupMapNumber]
    LD_C_A;  // ld c, a
    CCALL(aGetWorldMapLocation);  // call GetWorldMapLocation


CheckRegion:
    CP_A(KANTO_LANDMARK);  // cp KANTO_LANDMARK
    IF_NC goto Kanto;  // jr nc, .Kanto


Johto:
    XOR_A_A;  // xor a ; JOHTO_REGION
    RET;  // ret


Kanto:
    LD_A(KANTO_REGION);  // ld a, KANTO_REGION
    RET;  // ret

}

void SetXYCompareFlags(void){
    SET_PC(0x2FFCU);
    PUSH_HL;  // push hl
    XOR_A_A;  // xor a
    LD_HL(wXYCompareFlags);  // ld hl, wXYCompareFlags
    LD_hli_A;  // ld [hli], a
    LD_hli_A;  // ld [hli], a
    LD_hli_A;  // ld [hli], a
    LD_hl_A;  // ld [hl], a
    LD_A_addr(wXYComparePointer);  // ld a, [wXYComparePointer]
    LD_L_A;  // ld l, a
    LD_A_addr(wXYComparePointer + 1);  // ld a, [wXYComparePointer + 1]
    LD_H_A;  // ld h, a
    OR_A_L;  // or l
    IF_Z goto quit;  // jr z, .quit
    LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
    PUSH_AF;  // push af
    CCALL(aSwitchToMapScriptsBank);  // call SwitchToMapScriptsBank
    LD_A_addr(wPlayerStandingMapX);  // ld a, [wPlayerStandingMapX]
    ADD_A(0x4);  // add $4
    LD_D_A;  // ld d, a
    LD_A_addr(wPlayerStandingMapY);  // ld a, [wPlayerStandingMapY]
    LD_A(0x4);  // ld a, $4 ; should be "add $4"
    LD_E_A;  // ld e, a
    PUSH_BC;  // push bc
    LD_C(0);  // ld c, 0

loop:
    SET_PC(0x3025U);
    LD_A_hl;  // ld a, [hl]
    CP_A(-1);  // cp -1 ; end?
    IF_Z goto done;  // jr z, .done
    PUSH_HL;  // push hl
    LD_A_D;  // ld a, d
    CP_A_hl;  // cp [hl]
    IF_NZ goto next;  // jr nz, .next
    INC_HL;  // inc hl
    LD_A_E;  // ld a, e
    CP_A_hl;  // cp [hl]
    IF_NZ goto next;  // jr nz, .next
    LD_HL(wXYCompareFlags);  // ld hl, wXYCompareFlags
    LD_B(SET_FLAG);  // ld b, SET_FLAG
    PUSH_DE;  // push de
    PUSH_BC;  // push bc
    LD_D(0);  // ld d, 0
    PREDEF(pSmallFarFlagAction);  // predef SmallFarFlagAction
    POP_BC;  // pop bc
    POP_DE;  // pop de

next:
    SET_PC(0x3044U);
    POP_HL;  // pop hl
    INC_HL;  // inc hl
    INC_HL;  // inc hl
    INC_HL;  // inc hl
    INC_C;  // inc c
    LD_A_C;  // ld a, c
    CP_A(MAX_XYCOMPARE_LENGTH);  // cp MAX_XYCOMPARE_LENGTH
    IF_C goto loop;  // jr c, .loop

done:
    SET_PC(0x304EU);
    POP_BC;  // pop bc
    POP_AF;  // pop af
    RST(mBankswitch);  // rst Bankswitch

quit:
    SET_PC(0x3051U);
    POP_HL;  // pop hl
    RET;  // ret

}
