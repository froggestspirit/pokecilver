#include "../constants.h"

void UpdateBGMapBuffer(void){
//  Copy [hBGMapTileCount] 16x8 tiles from wBGMapBuffer
//  to bg map addresses in wBGMapBufferPointers.

//  [hBGMapTileCount] must be even since this is done in pairs.

//  Return carry on success.

    LDH_A_addr(hBGMapUpdate);  // ldh a, [hBGMapUpdate]
    AND_A_A;  // and a
    RET_Z ;  // ret z

//  Relocate the stack pointer to wBGMapBufferPointers
    LD_addr_SP(hSPBuffer);  // ld [hSPBuffer], sp
    LD_HL(wBGMapBufferPointers);  // ld hl, wBGMapBufferPointers
    LD_SP_HL;  // ld sp, hl

//  We can now pop the addresses of affected spots on the BG Map

    LD_HL(wBGMapPalBuffer);  // ld hl, wBGMapPalBuffer
    LD_DE(wBGMapBuffer);  // ld de, wBGMapBuffer


next:
//  Copy a pair of 16x8 blocks (one 16x16 block)

    for(int rept = 0; rept < 2; rept++){
//  Get our BG Map address
    POP_BC;  // pop bc

//  Palettes
    LD_A(1);  // ld a, 1
    LDH_addr_A(rVBK);  // ldh [rVBK], a

    LD_A_hli;  // ld a, [hli]
    LD_bc_A;  // ld [bc], a
    INC_C;  // inc c
    LD_A_hli;  // ld a, [hli]
    LD_bc_A;  // ld [bc], a
    DEC_C;  // dec c

//  Tiles
    LD_A(0);  // ld a, 0
    LDH_addr_A(rVBK);  // ldh [rVBK], a

    LD_A_de;  // ld a, [de]
    INC_DE;  // inc de
    LD_bc_A;  // ld [bc], a
    INC_C;  // inc c
    LD_A_de;  // ld a, [de]
    INC_DE;  // inc de
    LD_bc_A;  // ld [bc], a
    }

//  We've done 2 16x8 blocks
    LDH_A_addr(hBGMapTileCount);  // ldh a, [hBGMapTileCount]
    DEC_A;  // dec a
    DEC_A;  // dec a
    LDH_addr_A(hBGMapTileCount);  // ldh [hBGMapTileCount], a

    IF_NZ goto next;  // jr nz, .next

//  Restore the stack pointer
    LDH_A_addr(hSPBuffer);  // ldh a, [hSPBuffer]
    LD_L_A;  // ld l, a
    LDH_A_addr(hSPBuffer + 1);  // ldh a, [hSPBuffer + 1]
    LD_H_A;  // ld h, a
    LD_SP_HL;  // ld sp, hl

    XOR_A_A;  // xor a
    LDH_addr_A(hBGMapUpdate);  // ldh [hBGMapUpdate], a
    SCF;  // scf
    RET;  // ret

}

void WaitTop(void){
    SET_PC(0x14A7U);
//  Wait until the top third of the BG Map is being updated.

    LDH_A_addr(hBGMapMode);  // ldh a, [hBGMapMode]
    AND_A_A;  // and a
    RET_Z ;  // ret z

    LDH_A_addr(hBGMapThird);  // ldh a, [hBGMapThird]
    AND_A_A;  // and a
    IF_Z goto done;  // jr z, .done

    CALL(mDelayFrame);  // call DelayFrame
    JR(mWaitTop);  // jr WaitTop


done:
    SET_PC(0x14B5U);
    XOR_A_A;  // xor a
    LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a
    RET;  // ret

}

void UpdateBGMap(void){
//  Update the BG Map, in thirds, from wTilemap and wAttrmap.

    LDH_A_addr(hBGMapMode);  // ldh a, [hBGMapMode]
    AND_A_A;  // and a ; 0
    RET_Z ;  // ret z

//  BG Map 0
    DEC_A;  // dec a ; 1
    IF_Z return UpdateBGMap_Tiles();  // jr z, .Tiles
    DEC_A;  // dec a ; 2
    IF_Z return UpdateBGMap_Attr();  // jr z, .Attr

//  BG Map 1
    DEC_A;  // dec a ; useless

    LDH_A_addr(hBGMapAddress);  // ldh a, [hBGMapAddress]
    LD_L_A;  // ld l, a
    LDH_A_addr(hBGMapAddress + 1);  // ldh a, [hBGMapAddress + 1]
    LD_H_A;  // ld h, a
    PUSH_HL;  // push hl

    XOR_A_A;  // xor a ; LOW(vBGMap1)
    LDH_addr_A(hBGMapAddress);  // ldh [hBGMapAddress], a
    LD_A(HIGH(vBGMap1));  // ld a, HIGH(vBGMap1)
    LDH_addr_A(hBGMapAddress + 1);  // ldh [hBGMapAddress + 1], a

    LDH_A_addr(hBGMapMode);  // ldh a, [hBGMapMode]
    PUSH_AF;  // push af
    CP_A(3);  // cp 3
    IF_Z CCALL(aUpdateBGMap_Tiles);  // call z, .Tiles
    POP_AF;  // pop af
    CP_A(4);  // cp 4
    IF_Z CCALL(aUpdateBGMap_Attr);  // call z, .Attr

    POP_HL;  // pop hl
    LD_A_L;  // ld a, l
    LDH_addr_A(hBGMapAddress);  // ldh [hBGMapAddress], a
    LD_A_H;  // ld a, h
    LDH_addr_A(hBGMapAddress + 1);  // ldh [hBGMapAddress + 1], a
    RET;  // ret
}

void UpdateBGMap_Attr(void){
    LD_A(1);  // ld a, 1
    LDH_addr_A(rVBK);  // ldh [rVBK], a

    hlcoord(0, 0, wAttrmap);  // hlcoord 0, 0, wAttrmap
    CCALL(aUpdateBGMap_update);  // call .update

    LD_A(0);  // ld a, 0
    LDH_addr_A(rVBK);  // ldh [rVBK], a
    RET;  // ret
}

void UpdateBGMap_Tiles(void){
    hlcoord(0, 0, wTilemap);  // hlcoord 0, 0
    return UpdateBGMap_update();
}

void UpdateBGMap_update(void){
    LD_addr_SP(hSPBuffer);  // ld [hSPBuffer], sp

//  Which third?
    LDH_A_addr(hBGMapThird);  // ldh a, [hBGMapThird]
    AND_A_A;  // and a ; 0
    IF_Z return UpdateBGMap_top();  // jr z, .top
    DEC_A;  // dec a ; 1
    IF_Z return UpdateBGMap_middle();  // jr z, .middle
// ; 2

#define THIRD_HEIGHT (SCREEN_HEIGHT / 3)

//  bottom
    LD_DE(2 * THIRD_HEIGHT * SCREEN_WIDTH);  // ld de, 2 * THIRD_HEIGHT * SCREEN_WIDTH
    ADD_HL_DE;  // add hl, de
    LD_SP_HL;  // ld sp, hl

    LDH_A_addr(hBGMapAddress + 1);  // ldh a, [hBGMapAddress + 1]
    LD_H_A;  // ld h, a
    LDH_A_addr(hBGMapAddress);  // ldh a, [hBGMapAddress]
    LD_L_A;  // ld l, a

    LD_DE(2 * THIRD_HEIGHT * BG_MAP_WIDTH);  // ld de, 2 * THIRD_HEIGHT * BG_MAP_WIDTH
    ADD_HL_DE;  // add hl, de

//  Next time: top third
    XOR_A_A;  // xor a
    return UpdateBGMap_start();
}

void UpdateBGMap_middle(void){
    LD_DE(THIRD_HEIGHT * SCREEN_WIDTH);  // ld de, THIRD_HEIGHT * SCREEN_WIDTH
    ADD_HL_DE;  // add hl, de
    LD_SP_HL;  // ld sp, hl

    LDH_A_addr(hBGMapAddress + 1);  // ldh a, [hBGMapAddress + 1]
    LD_H_A;  // ld h, a
    LDH_A_addr(hBGMapAddress);  // ldh a, [hBGMapAddress]
    LD_L_A;  // ld l, a

    LD_DE(THIRD_HEIGHT * BG_MAP_WIDTH);  // ld de, THIRD_HEIGHT * BG_MAP_WIDTH
    ADD_HL_DE;  // add hl, de

//  Next time: bottom third
    LD_A(2);  // ld a, 2
    return UpdateBGMap_start();
}

void UpdateBGMap_top(void){
    LD_SP_HL;  // ld sp, hl

    LDH_A_addr(hBGMapAddress + 1);  // ldh a, [hBGMapAddress + 1]
    LD_H_A;  // ld h, a
    LDH_A_addr(hBGMapAddress);  // ldh a, [hBGMapAddress]
    LD_L_A;  // ld l, a

//  Next time: middle third
    LD_A(1);  // ld a, 1
    return UpdateBGMap_start();
}

void UpdateBGMap_start(void){
//  Which third to update next time
    LDH_addr_A(hBGMapThird);  // ldh [hBGMapThird], a

//  Rows of tiles in a third
    LD_A(THIRD_HEIGHT);  // ld a, THIRD_HEIGHT

//  Discrepancy between wTilemap and BGMap
    LD_BC(BG_MAP_WIDTH - (SCREEN_WIDTH - 1));  // ld bc, BG_MAP_WIDTH - (SCREEN_WIDTH - 1)
    return UpdateBGMap_row();
}

void UpdateBGMap_row(void){
//  Copy a row of 20 tiles
    for(int rept = 0; rept < SCREEN_WIDTH / 2 - 1; rept++){
    POP_DE;  // pop de
    LD_hl_E;  // ld [hl], e
    INC_L;  // inc l
    LD_hl_D;  // ld [hl], d
    INC_L;  // inc l
    }
    POP_DE;  // pop de
    LD_hl_E;  // ld [hl], e
    INC_L;  // inc l
    LD_hl_D;  // ld [hl], d

    ADD_HL_BC;  // add hl, bc
    DEC_A;  // dec a
    IF_NZ return UpdateBGMap_row();  // jr nz, .row

    LDH_A_addr(hSPBuffer);  // ldh a, [hSPBuffer]
    LD_L_A;  // ld l, a
    LDH_A_addr(hSPBuffer + 1);  // ldh a, [hSPBuffer + 1]
    LD_H_A;  // ld h, a
    LD_SP_HL;  // ld sp, hl
    RET;  // ret
}

void Serve1bppRequest(void){
    LD_A_addr(wRequested1bppSize);  // ld a, [wRequested1bppSize]
    AND_A_A;  // and a
    RET_Z ;  // ret z

//  Copy [wRequested1bppSize] 1bpp tiles from [wRequested1bppSource] to [wRequested1bppDest]

    LD_addr_SP(hSPBuffer);  // ld [hSPBuffer], sp

//  Source
    LD_HL(wRequested1bppSource);  // ld hl, wRequested1bppSource
    LD_A_hli;  // ld a, [hli]
    LD_H_hl;  // ld h, [hl]
    LD_L_A;  // ld l, a
    LD_SP_HL;  // ld sp, hl

//  Destination
    LD_HL(wRequested1bppDest);  // ld hl, wRequested1bppDest
    LD_A_hli;  // ld a, [hli]
    LD_H_hl;  // ld h, [hl]
    LD_L_A;  // ld l, a

//  # tiles to copy
    LD_A_addr(wRequested1bppSize);  // ld a, [wRequested1bppSize]
    LD_B_A;  // ld b, a

    XOR_A_A;  // xor a
    LD_addr_A(wRequested1bppSize);  // ld [wRequested1bppSize], a


next:

    for(int rept = 0; rept < 3; rept++){
    POP_DE;  // pop de
    LD_hl_E;  // ld [hl], e
    INC_L;  // inc l
    LD_hl_E;  // ld [hl], e
    INC_L;  // inc l
    LD_hl_D;  // ld [hl], d
    INC_L;  // inc l
    LD_hl_D;  // ld [hl], d
    INC_L;  // inc l
    }
    POP_DE;  // pop de
    LD_hl_E;  // ld [hl], e
    INC_L;  // inc l
    LD_hl_E;  // ld [hl], e
    INC_L;  // inc l
    LD_hl_D;  // ld [hl], d
    INC_L;  // inc l
    LD_hl_D;  // ld [hl], d

    INC_HL;  // inc hl
    DEC_B;  // dec b
    IF_NZ goto next;  // jr nz, .next

    LD_A_L;  // ld a, l
    LD_addr_A(wRequested1bppDest);  // ld [wRequested1bppDest], a
    LD_A_H;  // ld a, h
    LD_addr_A(wRequested1bppDest + 1);  // ld [wRequested1bppDest + 1], a

    LD_addr_SP(wRequested1bppSource);  // ld [wRequested1bppSource], sp

    LDH_A_addr(hSPBuffer);  // ldh a, [hSPBuffer]
    LD_L_A;  // ld l, a
    LDH_A_addr(hSPBuffer + 1);  // ldh a, [hSPBuffer + 1]
    LD_H_A;  // ld h, a
    LD_SP_HL;  // ld sp, hl
    RET;  // ret

}

void Serve2bppRequest(void){
    LD_A_addr(wRequested2bppSize);  // ld a, [wRequested2bppSize]
    AND_A_A;  // and a
    RET_Z ;  // ret z

//  Copy [wRequested2bppSize] 2bpp tiles from [wRequested2bppSource] to [wRequested2bppDest]

    LD_addr_SP(hSPBuffer);  // ld [hSPBuffer], sp

//  Source
    LD_HL(wRequested2bppSource);  // ld hl, wRequested2bppSource
    LD_A_hli;  // ld a, [hli]
    LD_H_hl;  // ld h, [hl]
    LD_L_A;  // ld l, a
    LD_SP_HL;  // ld sp, hl

//  Destination
    LD_HL(wRequested2bppDest);  // ld hl, wRequested2bppDest
    LD_A_hli;  // ld a, [hli]
    LD_H_hl;  // ld h, [hl]
    LD_L_A;  // ld l, a

//  # tiles to copy
    LD_A_addr(wRequested2bppSize);  // ld a, [wRequested2bppSize]
    LD_B_A;  // ld b, a

    XOR_A_A;  // xor a
    LD_addr_A(wRequested2bppSize);  // ld [wRequested2bppSize], a


next:

    for(int rept = 0; rept < 7; rept++){
    POP_DE;  // pop de
    LD_hl_E;  // ld [hl], e
    INC_L;  // inc l
    LD_hl_D;  // ld [hl], d
    INC_L;  // inc l
    }
    POP_DE;  // pop de
    LD_hl_E;  // ld [hl], e
    INC_L;  // inc l
    LD_hl_D;  // ld [hl], d

    INC_HL;  // inc hl
    DEC_B;  // dec b
    IF_NZ goto next;  // jr nz, .next

    LD_A_L;  // ld a, l
    LD_addr_A(wRequested2bppDest);  // ld [wRequested2bppDest], a
    LD_A_H;  // ld a, h
    LD_addr_A(wRequested2bppDest + 1);  // ld [wRequested2bppDest + 1], a

    LD_addr_SP(wRequested2bppSource);  // ld [wRequested2bppSource], sp

    LDH_A_addr(hSPBuffer);  // ldh a, [hSPBuffer]
    LD_L_A;  // ld l, a
    LDH_A_addr(hSPBuffer + 1);  // ldh a, [hSPBuffer + 1]
    LD_H_A;  // ld h, a
    LD_SP_HL;  // ld sp, hl
    RET;  // ret

}

void AnimateTileset(void){
    SET_PC(0x1629U);
    LDH_A_addr(hMapAnims);  // ldh a, [hMapAnims]
    AND_A_A;  // and a
    RET_Z ;  // ret z

    LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
    PUSH_AF;  // push af
    LD_A(BANK(av_AnimateTileset));  // ld a, BANK(_AnimateTileset)
    RST(mBankswitch);  // rst Bankswitch

    CALL(mv_AnimateTileset);  // call _AnimateTileset

    POP_AF;  // pop af
    Bankswitch();  // rst Bankswitch
    RET;  // ret

}

void Video_DummyFunction(void){
//  //  unreferenced
    RET;  // ret

}

void EnableSpriteDisplay(void){
//  //  unreferenced
    LD_HL(rLCDC);  // ld hl, rLCDC
    SET_hl(1);  // set 1, [hl]
    RET;  // ret

}

void FillBGMap0WithBlack(void){
    NOP;  // nop
    LDH_A_addr(hBlackOutBGMapThird);  // ldh a, [hBlackOutBGMapThird]
    AND_A_A;  // and a ; 0
    RET_Z ;  // ret z

    DEC_A;  // dec a ; 1
    IF_Z goto one;  // jr z, .one
    DEC_A;  // dec a ; 2
    IF_Z goto two;  // jr z, .two
// ; 3

#define BG_THIRD_HEIGHT (BG_MAP_HEIGHT - SCREEN_HEIGHT) / 2

//  Black out the 18 BG Map rows right of the screen area
    LD_A(2);  // ld a, 2
    LDH_addr_A(hBlackOutBGMapThird);  // ldh [hBlackOutBGMapThird], a
    LD_HL(hBGMapAddress);  // ld hl, hBGMapAddress
    LD_A_hli;  // ld a, [hli]
    LD_H_hl;  // ld h, [hl]
    LD_L_A;  // ld l, a
    LD_DE(SCREEN_WIDTH);  // ld de, SCREEN_WIDTH
    ADD_HL_DE;  // add hl, de
    LD_B(SCREEN_HEIGHT);  // ld b, SCREEN_HEIGHT
    LD_A(0x60);  // ld a, "■"

loop1:
    for(int rept = 0; rept < BG_MAP_WIDTH - SCREEN_WIDTH; rept++){
    LD_hli_A;  // ld [hli], a
    }
    ADD_HL_DE;  // add hl, de
    DEC_B;  // dec b
    IF_NZ goto loop1;  // jr nz, .loop1
    RET;  // ret


two:
//  Black out the top 7 BG Map rows below the screen area
    LD_A(1);  // ld a, 1
    LD_DE(BG_MAP_WIDTH * SCREEN_HEIGHT);  // ld de, BG_MAP_WIDTH * SCREEN_HEIGHT
    goto go;  // jr .go


one:
//  Black out the bottom 7 BG Map rows below the screen area
    XOR_A_A;  // xor a
    LD_DE(BG_MAP_WIDTH * (SCREEN_HEIGHT + BG_THIRD_HEIGHT));  // ld de, BG_MAP_WIDTH * (SCREEN_HEIGHT + BG_THIRD_HEIGHT)


go:
    LDH_addr_A(hBlackOutBGMapThird);  // ldh [hBlackOutBGMapThird], a
    LD_HL(hBGMapAddress);  // ld hl, hBGMapAddress
    LD_A_hli;  // ld a, [hli]
    LD_H_hl;  // ld h, [hl]
    LD_L_A;  // ld l, a
    ADD_HL_DE;  // add hl, de
    LD_B(BG_THIRD_HEIGHT * 2);  // ld b, BG_THIRD_HEIGHT * 2
    LD_A(0x60);  // ld a, "■"

loop2:
    for(int rept = 0; rept < BG_MAP_WIDTH / 2; rept++){
    LD_hli_A;  // ld [hli], a
    }
    DEC_B;  // dec b
    IF_NZ goto loop2;  // jr nz, .loop2
    RET;  // ret

}
