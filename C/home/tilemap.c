#include "../constants.h"
#include "tilemap.h"

void ClearBGPalettes(void){
    SET_PC(0x3445U);
    CCALL(aClearPalettes);  // call ClearPalettes
    return WaitBGMap();
}

void WaitBGMap(void){
    SET_PC(0x3448U);
//  Tell VBlank to update BG Map
    LD_A(1);  // ld a, 1 ; BG Map 0 tiles
    LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a
//  Wait for it to do its magic
    LD_C(4);  // ld c, 4
    CALL(mDelayFrames);  // call DelayFrames
    RET;  // ret

}

void WaitBGMap2(void){
    SET_PC(0x3452U);
    LDH_A_addr(hCGB);  // ldh a, [hCGB]
    AND_A_A;  // and a
    IF_Z goto bg0;  // jr z, .bg0

    LD_A(2);  // ld a, 2
    LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a
    LD_C(4);  // ld c, 4
    CALL(mDelayFrames);  // call DelayFrames


bg0:
    SET_PC(0x3460U);
    LD_A(1);  // ld a, 1
    LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a
    LD_C(4);  // ld c, 4
    CALL(mDelayFrames);  // call DelayFrames
    RET;  // ret

}

void IsCGB(void){
    LDH_A_addr(hCGB);  // ldh a, [hCGB]
    AND_A_A;  // and a
    RET;  // ret

}

void ApplyTilemap(void){
    SET_PC(0x346EU);
    LDH_A_addr(hCGB);  // ldh a, [hCGB]
    AND_A_A;  // and a
    IF_Z goto dmg;  // jr z, .dmg

    LD_A_addr(wSpriteUpdatesEnabled);  // ld a, [wSpriteUpdatesEnabled]
    CP_A(0);  // cp 0
    IF_Z goto dmg;  // jr z, .dmg

    LD_A(1);  // ld a, 1
    LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a
    JR(mCopyTilemapAtOnce);  // jr CopyTilemapAtOnce


dmg:
    SET_PC(0x3480U);
//  WaitBGMap
    LD_A(1);  // ld a, 1
    LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a
    LD_C(4);  // ld c, 4
    CALL(mDelayFrames);  // call DelayFrames
    RET;  // ret

}

void CGBOnly_CopyTilemapAtOnce(void){
    SET_PC(0x348AU);
    LDH_A_addr(hCGB);  // ldh a, [hCGB]
    AND_A_A;  // and a
    JR_Z (mWaitBGMap);  // jr z, WaitBGMap

}

void CopyTilemapAtOnce(void){
    SET_PC(0x348FU);
    LDH_A_addr(hBGMapMode);  // ldh a, [hBGMapMode]
    PUSH_AF;  // push af
    XOR_A_A;  // xor a
    LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a

    LDH_A_addr(hMapAnims);  // ldh a, [hMapAnims]
    PUSH_AF;  // push af
    XOR_A_A;  // xor a
    LDH_addr_A(hMapAnims);  // ldh [hMapAnims], a


wait:
    SET_PC(0x349BU);
    LDH_A_addr(rLY);  // ldh a, [rLY]
    CP_A(0x80 - 1);  // cp $80 - 1
    IF_C goto wait;  // jr c, .wait

    NOP;  // di
    LD_A(BANK(vBGMap2));  // ld a, BANK(vBGMap2)
    LDH_addr_A(rVBK);  // ldh [rVBK], a
    hlcoord(0, 0, wAttrmap);  // hlcoord 0, 0, wAttrmap
    CALL(mCopyTilemapAtOnce_CopyBGMapViaStack);  // call .CopyBGMapViaStack
    LD_A(BANK(vBGMap0));  // ld a, BANK(vBGMap0)
    LDH_addr_A(rVBK);  // ldh [rVBK], a
    hlcoord(0, 0, wTilemap);  // hlcoord 0, 0
    CALL(mCopyTilemapAtOnce_CopyBGMapViaStack);  // call .CopyBGMapViaStack


wait2:
    SET_PC(0x34B6U);
    LDH_A_addr(rLY);  // ldh a, [rLY]
    CP_A(0x80 - 1);  // cp $80 - 1
    IF_C goto wait2;  // jr c, .wait2
    NOP;  // ei

    POP_AF;  // pop af
    LDH_addr_A(hMapAnims);  // ldh [hMapAnims], a
    POP_AF;  // pop af
    LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a
    RET;  // ret


CopyBGMapViaStack:
    SET_PC(0x34C4U);
//  Copy all tiles to vBGMap
    LD_addr_SP(hSPBuffer);  // ld [hSPBuffer], sp
    LD_SP_HL;  // ld sp, hl
    LDH_A_addr(hBGMapAddress + 1);  // ldh a, [hBGMapAddress + 1]
    LD_H_A;  // ld h, a
    LD_L(0);  // ld l, 0
    LD_A(SCREEN_HEIGHT);  // ld a, SCREEN_HEIGHT
    LDH_addr_A(hTilesPerCycle);  // ldh [hTilesPerCycle], a
    LD_B(1 << 1);  // ld b, 1 << 1 ; not in v/hblank
    LD_C(LOW(rSTAT));  // ld c, LOW(rSTAT)


loop:
    SET_PC(0x34D5U);
    POP_DE;  // pop de
//  if in v/hblank, wait until not in v/hblank

loop_u8221:
    SET_PC(0x34D6U);
    LDH_A_c;  // ldh a, [c]
    AND_A_B;  // and b
    IF_NZ goto loop_u8221;  // jr nz, .loop_u8221
//  load vBGMap
    LD_hl_E;  // ld [hl], e
    INC_L;  // inc l
    LD_hl_D;  // ld [hl], d
    INC_L;  // inc l
    POP_DE;  // pop de
//  if in v/hblank, wait until not in v/hblank

loop_u8222:
    SET_PC(0x34DFU);
    LDH_A_c;  // ldh a, [c]
    AND_A_B;  // and b
    IF_NZ goto loop_u8222;  // jr nz, .loop_u8222
//  load vBGMap
    LD_hl_E;  // ld [hl], e
    INC_L;  // inc l
    LD_hl_D;  // ld [hl], d
    INC_L;  // inc l
    POP_DE;  // pop de
//  if in v/hblank, wait until not in v/hblank

loop_u8223:
    SET_PC(0x34E8U);
    LDH_A_c;  // ldh a, [c]
    AND_A_B;  // and b
    IF_NZ goto loop_u8223;  // jr nz, .loop_u8223
//  load vBGMap
    LD_hl_E;  // ld [hl], e
    INC_L;  // inc l
    LD_hl_D;  // ld [hl], d
    INC_L;  // inc l
    POP_DE;  // pop de
//  if in v/hblank, wait until not in v/hblank

loop_u8224:
    SET_PC(0x34F1U);
    LDH_A_c;  // ldh a, [c]
    AND_A_B;  // and b
    IF_NZ goto loop_u8224;  // jr nz, .loop_u8224
//  load vBGMap
    LD_hl_E;  // ld [hl], e
    INC_L;  // inc l
    LD_hl_D;  // ld [hl], d
    INC_L;  // inc l
    POP_DE;  // pop de
//  if in v/hblank, wait until not in v/hblank

loop_u8225:
    SET_PC(0x34FAU);
    LDH_A_c;  // ldh a, [c]
    AND_A_B;  // and b
    IF_NZ goto loop_u8225;  // jr nz, .loop_u8225
//  load vBGMap
    LD_hl_E;  // ld [hl], e
    INC_L;  // inc l
    LD_hl_D;  // ld [hl], d
    INC_L;  // inc l
    POP_DE;  // pop de
//  if in v/hblank, wait until not in v/hblank

loop_u8226:
    SET_PC(0x3503U);
    LDH_A_c;  // ldh a, [c]
    AND_A_B;  // and b
    IF_NZ goto loop_u8226;  // jr nz, .loop_u8226
//  load vBGMap
    LD_hl_E;  // ld [hl], e
    INC_L;  // inc l
    LD_hl_D;  // ld [hl], d
    INC_L;  // inc l
    POP_DE;  // pop de
//  if in v/hblank, wait until not in v/hblank

loop_u8227:
    SET_PC(0x350CU);
    LDH_A_c;  // ldh a, [c]
    AND_A_B;  // and b
    IF_NZ goto loop_u8227;  // jr nz, .loop_u8227
//  load vBGMap
    LD_hl_E;  // ld [hl], e
    INC_L;  // inc l
    LD_hl_D;  // ld [hl], d
    INC_L;  // inc l
    POP_DE;  // pop de
//  if in v/hblank, wait until not in v/hblank

loop_u8228:
    SET_PC(0x3515U);
    LDH_A_c;  // ldh a, [c]
    AND_A_B;  // and b
    IF_NZ goto loop_u8228;  // jr nz, .loop_u8228
//  load vBGMap
    LD_hl_E;  // ld [hl], e
    INC_L;  // inc l
    LD_hl_D;  // ld [hl], d
    INC_L;  // inc l
    POP_DE;  // pop de
//  if in v/hblank, wait until not in v/hblank

loop_u8229:
    SET_PC(0x351EU);
    LDH_A_c;  // ldh a, [c]
    AND_A_B;  // and b
    IF_NZ goto loop_u8229;  // jr nz, .loop_u8229
//  load vBGMap
    LD_hl_E;  // ld [hl], e
    INC_L;  // inc l
    LD_hl_D;  // ld [hl], d
    INC_L;  // inc l
    POP_DE;  // pop de
//  if in v/hblank, wait until not in v/hblank

loop_u8230:
    SET_PC(0x3527U);
    LDH_A_c;  // ldh a, [c]
    AND_A_B;  // and b
    IF_NZ goto loop_u8230;  // jr nz, .loop_u8230
//  load vBGMap
    LD_hl_E;  // ld [hl], e
    INC_L;  // inc l
    LD_hl_D;  // ld [hl], d
    INC_L;  // inc l

    LD_DE(BG_MAP_WIDTH - SCREEN_WIDTH);  // ld de, BG_MAP_WIDTH - SCREEN_WIDTH
    ADD_HL_DE;  // add hl, de
    LDH_A_addr(hTilesPerCycle);  // ldh a, [hTilesPerCycle]
    DEC_A;  // dec a
    LDH_addr_A(hTilesPerCycle);  // ldh [hTilesPerCycle], a
    IF_NZ goto loop;  // jr nz, .loop

    LDH_A_addr(hSPBuffer);  // ldh a, [hSPBuffer]
    LD_L_A;  // ld l, a
    LDH_A_addr(hSPBuffer + 1);  // ldh a, [hSPBuffer + 1]
    LD_H_A;  // ld h, a
    LD_SP_HL;  // ld sp, hl
    RET;  // ret

}

void SetPalettes(void){
    SET_PC(0x3542U);
//  Inits the Palettes
//  depending on the system the monochromes palettes or color palettes
    LDH_A_addr(hCGB);  // ldh a, [hCGB]
    AND_A_A;  // and a
    IF_NZ goto SetPalettesForGameBoyColor;  // jr nz, .SetPalettesForGameBoyColor
    LD_A(0b11100100);  // ld a, %11100100
    LDH_addr_A(rBGP);  // ldh [rBGP], a
    LD_A(0b11010000);  // ld a, %11010000
    LDH_addr_A(rOBP0);  // ldh [rOBP0], a
    LDH_addr_A(rOBP1);  // ldh [rOBP1], a
    RET;  // ret


SetPalettesForGameBoyColor:
    SET_PC(0x3552U);
    PUSH_DE;  // push de
    LD_A(0b11100100);  // ld a, %11100100
    CALL(mDmgToCgbBGPals);  // call DmgToCgbBGPals
    LD_DE((0b11100100 << 8) | 0b11100100);  // lb de, %11100100, %11100100
    CALL(mDmgToCgbObjPals);  // call DmgToCgbObjPals
    POP_DE;  // pop de
    RET;  // ret

}

void ClearPalettes(void){
//  Make all palettes white

//  CGB: make all the palette colors white
    LDH_A_addr(hCGB);  // ldh a, [hCGB]
    AND_A_A;  // and a
    IF_NZ goto cgb;  // jr nz, .cgb

//  DMG: just change palettes to 0 (white)
    XOR_A_A;  // xor a
    LDH_addr_A(rBGP);  // ldh [rBGP], a
    LDH_addr_A(rOBP0);  // ldh [rOBP0], a
    LDH_addr_A(rOBP1);  // ldh [rOBP1], a
    RET;  // ret


cgb:
//  Fill wBGPals2 and wOBPals2 with $ffff (white)
    LD_HL(wBGPals2);  // ld hl, wBGPals2
    LD_BC(16 * PALETTE_SIZE);  // ld bc, 16 palettes
    LD_A(0xff);  // ld a, $ff
    CCALL(aByteFill);  // call ByteFill
//  Request palette update
    LD_A(TRUE);  // ld a, TRUE
    LDH_addr_A(hCGBPalUpdate);  // ldh [hCGBPalUpdate], a
    RET;  // ret

}

void GetMemSGBLayout(void){
    SET_PC(0x357DU);
    LD_B(SCGB_DEFAULT);  // ld b, SCGB_DEFAULT
    return GetSGBLayout();
}

void GetSGBLayout(void){
    SET_PC(0x357FU);
//  load sgb packets unless dmg

    LDH_A_addr(hCGB);  // ldh a, [hCGB]
    AND_A_A;  // and a
    IF_NZ goto sgb;  // jr nz, .sgb

    LDH_A_addr(hSGB);  // ldh a, [hSGB]
    AND_A_A;  // and a
    RET_Z ;  // ret z


sgb:
    SET_PC(0x3588U);
    PREDEF_JUMP(pLoadSGBLayout);  // predef_jump LoadSGBLayout

    return SetHPPal();
}

void SetHPPal(void){
//  Set palette for hp bar pixel length e at hl.
    CCALL(aGetHPPal);  // call GetHPPal
    LD_hl_D;  // ld [hl], d
    RET;  // ret

}

void GetHPPal(void){
//  Get palette for hp bar pixel length e in d.
    LD_D(HP_GREEN);  // ld d, HP_GREEN
    LD_A_E;  // ld a, e
    CP_A((HP_BAR_LENGTH_PX * 50 / 100));  // cp (HP_BAR_LENGTH_PX * 50 / 100) ; 24
    RET_NC ;  // ret nc
    INC_D;  // inc d ; HP_YELLOW
    CP_A((HP_BAR_LENGTH_PX * 21 / 100));  // cp (HP_BAR_LENGTH_PX * 21 / 100) ; 10
    RET_NC ;  // ret nc
    INC_D;  // inc d ; HP_RED
    RET;  // ret

}
