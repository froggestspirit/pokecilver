#include "../constants.h"
#include "warp_connection.h"

void HandleNewMap(void){
    SET_PC(0x2084U);
    CCALL(aClearUnusedMapBuffer);  // call ClearUnusedMapBuffer
    CCALL(aResetMapBufferEventFlags);  // call ResetMapBufferEventFlags
    CCALL(aResetFlashIfOutOfCave);  // call ResetFlashIfOutOfCave
    CCALL(aGetCurrentMapSceneID);  // call GetCurrentMapSceneID
    CCALL(aResetBikeFlags);  // call ResetBikeFlags
    LD_A(MAPCALLBACK_NEWMAP);  // ld a, MAPCALLBACK_NEWMAP
    CALL(mRunMapCallback);  // call RunMapCallback
    return HandleContinueMap();
}

void HandleContinueMap(void){
    SET_PC(0x2098U);
    FARCALL(aClearCmdQueue);  // farcall ClearCmdQueue
    LD_A(MAPCALLBACK_CMDQUEUE);  // ld a, MAPCALLBACK_CMDQUEUE
    CALL(mRunMapCallback);  // call RunMapCallback
    CCALL(aGetMapTimeOfDay);  // call GetMapTimeOfDay
    LD_addr_A(wMapTimeOfDay);  // ld [wMapTimeOfDay], a
    RET;  // ret

}

void LoadMapTimeOfDay(void){
    SET_PC(0x20AAU);
    LD_A(0x1);  // ld a, $1
    LD_addr_A(wSpriteUpdatesEnabled);  // ld [wSpriteUpdatesEnabled], a
    FARCALL(aReplaceTimeOfDayPals);  // farcall ReplaceTimeOfDayPals
    FARCALL(aUpdateTimeOfDayPal);  // farcall UpdateTimeOfDayPal
    CALL(mOverworldTextModeSwitch);  // call OverworldTextModeSwitch
    CALL(mLoadMapTimeOfDay_ClearBGMap);  // call .ClearBGMap
    CALL(mLoadMapTimeOfDay_PushAttrmap);  // call .PushAttrmap
    RET;  // ret


ClearBGMap:
    SET_PC(0x20C5U);
    LD_A(HIGH(vBGMap0));  // ld a, HIGH(vBGMap0)
    LD_addr_A(wBGMapAnchor + 1);  // ld [wBGMapAnchor + 1], a
    XOR_A_A;  // xor a ; LOW(vBGMap0)
    LD_addr_A(wBGMapAnchor);  // ld [wBGMapAnchor], a
    LDH_addr_A(hSCY);  // ldh [hSCY], a
    LDH_addr_A(hSCX);  // ldh [hSCX], a
    FARCALL(aApplyBGMapAnchorToObjects);  // farcall ApplyBGMapAnchorToObjects

    LD_A(0x60);  // ld a, "■"
    LD_BC(vBGMap1 - vBGMap0);  // ld bc, vBGMap1 - vBGMap0
    hlbgcoord(0, 0, vBGMap0);  // hlbgcoord 0, 0
    CCALL(aByteFill);  // call ByteFill
    RET;  // ret


PushAttrmap:
    SET_PC(0x20E4U);
    decoord(0, 0, wTilemap);  // decoord 0, 0
    CALL(mLoadMapTimeOfDay_copy);  // call .copy
    LDH_A_addr(hCGB);  // ldh a, [hCGB]
    AND_A_A;  // and a
    RET_Z ;  // ret z

    decoord(0, 0, wAttrmap);  // decoord 0, 0, wAttrmap
    LD_A(0x1);  // ld a, $1
    LDH_addr_A(rVBK);  // ldh [rVBK], a

copy:
    SET_PC(0x20F5U);
    hlbgcoord(0, 0, vBGMap0);  // hlbgcoord 0, 0
    LD_C(SCREEN_WIDTH);  // ld c, SCREEN_WIDTH
    LD_B(SCREEN_HEIGHT);  // ld b, SCREEN_HEIGHT

row:
    SET_PC(0x20FCU);
    PUSH_BC;  // push bc

column:
    SET_PC(0x20FDU);
    LD_A_de;  // ld a, [de]
    INC_DE;  // inc de
    LD_hli_A;  // ld [hli], a
    DEC_C;  // dec c
    IF_NZ goto column;  // jr nz, .column
    LD_BC(BG_MAP_WIDTH - SCREEN_WIDTH);  // ld bc, BG_MAP_WIDTH - SCREEN_WIDTH
    ADD_HL_BC;  // add hl, bc
    POP_BC;  // pop bc
    DEC_B;  // dec b
    IF_NZ goto row;  // jr nz, .row
    LD_A(0x0);  // ld a, $0
    LDH_addr_A(rVBK);  // ldh [rVBK], a
    RET;  // ret

}

void LoadMapGraphics(void){
    SET_PC(0x2110U);
    CCALL(aLoadMapTileset);  // call LoadMapTileset
    CALL(mLoadTilesetGFX);  // call LoadTilesetGFX
    XOR_A_A;  // xor a
    LDH_addr_A(hMapAnims);  // ldh [hMapAnims], a
    XOR_A_A;  // xor a
    LDH_addr_A(hTileAnimFrame);  // ldh [hTileAnimFrame], a
    FARCALL(aRefreshSprites);  // farcall RefreshSprites
    CALL(mLoadFontsExtra);  // call LoadFontsExtra
    RET;  // ret

}

void LoadMapPalettes(void){
    SET_PC(0x2126U);
    LD_B(0x9);  // ld b, $9
    JP(mGetSGBLayout);  // jp GetSGBLayout

}

void RefreshMapSprites(void){
    SET_PC(0x212BU);
    CCALL(aClearSprites);  // call ClearSprites
    CCALL(aResetBGWindow);  // call ResetBGWindow
    CALL(mGetMovementPermissions);  // call GetMovementPermissions
    FARCALL(aRefreshPlayerSprite);  // farcall RefreshPlayerSprite
    FARCALL(aCheckUpdatePlayerSprite);  // farcall CheckUpdatePlayerSprite
    LD_HL(wPlayerSpriteSetupFlags);  // ld hl, wPlayerSpriteSetupFlags
    BIT_hl(PLAYERSPRITESETUP_SKIP_RELOAD_GFX_F);  // bit PLAYERSPRITESETUP_SKIP_RELOAD_GFX_F, [hl]
    IF_NZ goto skip;  // jr nz, .skip
    LD_HL(wVramState);  // ld hl, wVramState
    SET_hl(0);  // set 0, [hl]
    CALL(mSafeUpdateSprites);  // call SafeUpdateSprites

skip:
    SET_PC(0x214FU);
    XOR_A_A;  // xor a
    LD_addr_A(wPlayerSpriteSetupFlags);  // ld [wPlayerSpriteSetupFlags], a
    RET;  // ret

}

void CheckMovingOffEdgeOfMap(void){
    LD_A_addr(wPlayerStepDirection);  // ld a, [wPlayerStepDirection]
    CP_A(STANDING);  // cp STANDING
    RET_Z ;  // ret z
    AND_A_A;  // and a ; DOWN
    IF_Z goto down;  // jr z, .down
    CP_A(UP);  // cp UP
    IF_Z goto up;  // jr z, .up
    CP_A(LEFT);  // cp LEFT
    IF_Z goto left;  // jr z, .left
    CP_A(RIGHT);  // cp RIGHT
    IF_Z goto right;  // jr z, .right
    AND_A_A;  // and a
    RET;  // ret


down:
    LD_A_addr(wPlayerStandingMapY);  // ld a, [wPlayerStandingMapY]
    SUB_A(4);  // sub 4
    LD_B_A;  // ld b, a
    LD_A_addr(wMapHeight);  // ld a, [wMapHeight]
    ADD_A_A;  // add a
    CP_A_B;  // cp b
    IF_Z goto ok;  // jr z, .ok
    AND_A_A;  // and a
    RET;  // ret


up:
    LD_A_addr(wPlayerStandingMapY);  // ld a, [wPlayerStandingMapY]
    SUB_A(4);  // sub 4
    CP_A(-1);  // cp -1
    IF_Z goto ok;  // jr z, .ok
    AND_A_A;  // and a
    RET;  // ret


left:
    LD_A_addr(wPlayerStandingMapX);  // ld a, [wPlayerStandingMapX]
    SUB_A(4);  // sub 4
    CP_A(-1);  // cp -1
    IF_Z goto ok;  // jr z, .ok
    AND_A_A;  // and a
    RET;  // ret


right:
    LD_A_addr(wPlayerStandingMapX);  // ld a, [wPlayerStandingMapX]
    SUB_A(4);  // sub 4
    LD_B_A;  // ld b, a
    LD_A_addr(wMapWidth);  // ld a, [wMapWidth]
    ADD_A_A;  // add a
    CP_A_B;  // cp b
    IF_Z goto ok;  // jr z, .ok
    AND_A_A;  // and a
    RET;  // ret


ok:
    SCF;  // scf
    RET;  // ret

}

void EnterMapConnection(void){
    SET_PC(0x21A1U);
//  Return carry if a connection has been entered.
    LD_A_addr(wPlayerStepDirection);  // ld a, [wPlayerStepDirection]
    AND_A_A;  // and a ; DOWN
    JP_Z (mEnterMapConnection_south);  // jp z, .south
    CP_A(UP);  // cp UP
    JP_Z (mEnterMapConnection_north);  // jp z, .north
    CP_A(LEFT);  // cp LEFT
    JP_Z (mEnterMapConnection_west);  // jp z, .west
    CP_A(RIGHT);  // cp RIGHT
    JP_Z (mEnterMapConnection_east);  // jp z, .east
    RET;  // ret


west:
    SET_PC(0x21B8U);
    LD_A_addr(wWestConnectedMapGroup);  // ld a, [wWestConnectedMapGroup]
    LD_addr_A(wMapGroup);  // ld [wMapGroup], a
    LD_A_addr(wWestConnectedMapNumber);  // ld a, [wWestConnectedMapNumber]
    LD_addr_A(wMapNumber);  // ld [wMapNumber], a
    LD_A_addr(wWestConnectionStripXOffset);  // ld a, [wWestConnectionStripXOffset]
    LD_addr_A(wXCoord);  // ld [wXCoord], a
    LD_A_addr(wWestConnectionStripYOffset);  // ld a, [wWestConnectionStripYOffset]
    LD_HL(wYCoord);  // ld hl, wYCoord
    ADD_A_hl;  // add [hl]
    LD_hl_A;  // ld [hl], a
    LD_C_A;  // ld c, a
    LD_HL(wWestConnectionWindow);  // ld hl, wWestConnectionWindow
    LD_A_hli;  // ld a, [hli]
    LD_H_hl;  // ld h, [hl]
    LD_L_A;  // ld l, a
    SRL_C;  // srl c
    IF_Z goto skip_to_load;  // jr z, .skip_to_load
    LD_A_addr(wWestConnectedMapWidth);  // ld a, [wWestConnectedMapWidth]
    ADD_A(6);  // add 6
    LD_E_A;  // ld e, a
    LD_D(0);  // ld d, 0


loop:
    SET_PC(0x21E5U);
    ADD_HL_DE;  // add hl, de
    DEC_C;  // dec c
    IF_NZ goto loop;  // jr nz, .loop


skip_to_load:
    SET_PC(0x21E9U);
    LD_A_L;  // ld a, l
    LD_addr_A(wOverworldMapAnchor);  // ld [wOverworldMapAnchor], a
    LD_A_H;  // ld a, h
    LD_addr_A(wOverworldMapAnchor + 1);  // ld [wOverworldMapAnchor + 1], a
    JP(mEnterMapConnection_done);  // jp .done


east:
    SET_PC(0x21F4U);
    LD_A_addr(wEastConnectedMapGroup);  // ld a, [wEastConnectedMapGroup]
    LD_addr_A(wMapGroup);  // ld [wMapGroup], a
    LD_A_addr(wEastConnectedMapNumber);  // ld a, [wEastConnectedMapNumber]
    LD_addr_A(wMapNumber);  // ld [wMapNumber], a
    LD_A_addr(wEastConnectionStripXOffset);  // ld a, [wEastConnectionStripXOffset]
    LD_addr_A(wXCoord);  // ld [wXCoord], a
    LD_A_addr(wEastConnectionStripYOffset);  // ld a, [wEastConnectionStripYOffset]
    LD_HL(wYCoord);  // ld hl, wYCoord
    ADD_A_hl;  // add [hl]
    LD_hl_A;  // ld [hl], a
    LD_C_A;  // ld c, a
    LD_HL(wEastConnectionWindow);  // ld hl, wEastConnectionWindow
    LD_A_hli;  // ld a, [hli]
    LD_H_hl;  // ld h, [hl]
    LD_L_A;  // ld l, a
    SRL_C;  // srl c
    IF_Z goto skip_to_load2;  // jr z, .skip_to_load2
    LD_A_addr(wEastConnectedMapWidth);  // ld a, [wEastConnectedMapWidth]
    ADD_A(6);  // add 6
    LD_E_A;  // ld e, a
    LD_D(0);  // ld d, 0


loop2:
    SET_PC(0x2221U);
    ADD_HL_DE;  // add hl, de
    DEC_C;  // dec c
    IF_NZ goto loop2;  // jr nz, .loop2


skip_to_load2:
    SET_PC(0x2225U);
    LD_A_L;  // ld a, l
    LD_addr_A(wOverworldMapAnchor);  // ld [wOverworldMapAnchor], a
    LD_A_H;  // ld a, h
    LD_addr_A(wOverworldMapAnchor + 1);  // ld [wOverworldMapAnchor + 1], a
    JP(mEnterMapConnection_done);  // jp .done


north:
    SET_PC(0x2230U);
    LD_A_addr(wNorthConnectedMapGroup);  // ld a, [wNorthConnectedMapGroup]
    LD_addr_A(wMapGroup);  // ld [wMapGroup], a
    LD_A_addr(wNorthConnectedMapNumber);  // ld a, [wNorthConnectedMapNumber]
    LD_addr_A(wMapNumber);  // ld [wMapNumber], a
    LD_A_addr(wNorthConnectionStripYOffset);  // ld a, [wNorthConnectionStripYOffset]
    LD_addr_A(wYCoord);  // ld [wYCoord], a
    LD_A_addr(wNorthConnectionStripXOffset);  // ld a, [wNorthConnectionStripXOffset]
    LD_HL(wXCoord);  // ld hl, wXCoord
    ADD_A_hl;  // add [hl]
    LD_hl_A;  // ld [hl], a
    LD_C_A;  // ld c, a
    LD_HL(wNorthConnectionWindow);  // ld hl, wNorthConnectionWindow
    LD_A_hli;  // ld a, [hli]
    LD_H_hl;  // ld h, [hl]
    LD_L_A;  // ld l, a
    LD_B(0);  // ld b, 0
    SRL_C;  // srl c
    ADD_HL_BC;  // add hl, bc
    LD_A_L;  // ld a, l
    LD_addr_A(wOverworldMapAnchor);  // ld [wOverworldMapAnchor], a
    LD_A_H;  // ld a, h
    LD_addr_A(wOverworldMapAnchor + 1);  // ld [wOverworldMapAnchor + 1], a
    JP(mEnterMapConnection_done);  // jp .done


south:
    SET_PC(0x2261U);
    LD_A_addr(wSouthConnectedMapGroup);  // ld a, [wSouthConnectedMapGroup]
    LD_addr_A(wMapGroup);  // ld [wMapGroup], a
    LD_A_addr(wSouthConnectedMapNumber);  // ld a, [wSouthConnectedMapNumber]
    LD_addr_A(wMapNumber);  // ld [wMapNumber], a
    LD_A_addr(wSouthConnectionStripYOffset);  // ld a, [wSouthConnectionStripYOffset]
    LD_addr_A(wYCoord);  // ld [wYCoord], a
    LD_A_addr(wSouthConnectionStripXOffset);  // ld a, [wSouthConnectionStripXOffset]
    LD_HL(wXCoord);  // ld hl, wXCoord
    ADD_A_hl;  // add [hl]
    LD_hl_A;  // ld [hl], a
    LD_C_A;  // ld c, a
    LD_HL(wSouthConnectionWindow);  // ld hl, wSouthConnectionWindow
    LD_A_hli;  // ld a, [hli]
    LD_H_hl;  // ld h, [hl]
    LD_L_A;  // ld l, a
    LD_B(0);  // ld b, 0
    SRL_C;  // srl c
    ADD_HL_BC;  // add hl, bc
    LD_A_L;  // ld a, l
    LD_addr_A(wOverworldMapAnchor);  // ld [wOverworldMapAnchor], a
    LD_A_H;  // ld a, h
    LD_addr_A(wOverworldMapAnchor + 1);  // ld [wOverworldMapAnchor + 1], a

done:
    SET_PC(0x228FU);
    SCF;  // scf
    RET;  // ret

}

void CheckWarpTile(void){
    SET_PC(0x2291U);
    CALL(mGetDestinationWarpNumber);  // call GetDestinationWarpNumber
    RET_NC ;  // ret nc

    PUSH_BC;  // push bc
    FARCALL(aCheckDirectionalWarp);  // farcall CheckDirectionalWarp
    POP_BC;  // pop bc
    RET_NC ;  // ret nc

    CALL(mCopyWarpData);  // call CopyWarpData
    SCF;  // scf
    RET;  // ret

}

void WarpCheck(void){
    SET_PC(0x22A3U);
    CALL(mGetDestinationWarpNumber);  // call GetDestinationWarpNumber
    RET_NC ;  // ret nc
    CALL(mCopyWarpData);  // call CopyWarpData
    RET;  // ret

}

void GetDestinationWarpNumber(void){
    SET_PC(0x22ABU);
    FARCALL(aCheckWarpCollision);  // farcall CheckWarpCollision
    RET_NC ;  // ret nc

    LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
    PUSH_AF;  // push af

    CCALL(aSwitchToMapScriptsBank);  // call SwitchToMapScriptsBank
    CALL(mGetDestinationWarpNumber_GetDestinationWarpNumber);  // call .GetDestinationWarpNumber

    POP_DE;  // pop de
    LD_A_D;  // ld a, d
    RST(mBankswitch);  // rst Bankswitch
    RET;  // ret


GetDestinationWarpNumber:
    SET_PC(0x22BFU);
    LD_A_addr(wPlayerStandingMapY);  // ld a, [wPlayerStandingMapY]
    SUB_A(4);  // sub 4
    LD_E_A;  // ld e, a
    LD_A_addr(wPlayerStandingMapX);  // ld a, [wPlayerStandingMapX]
    SUB_A(4);  // sub 4
    LD_D_A;  // ld d, a
    LD_A_addr(wCurMapWarpCount);  // ld a, [wCurMapWarpCount]
    AND_A_A;  // and a
    RET_Z ;  // ret z

    LD_C_A;  // ld c, a
    LD_HL(wCurMapWarpsPointer);  // ld hl, wCurMapWarpsPointer
    LD_A_hli;  // ld a, [hli]
    LD_H_hl;  // ld h, [hl]
    LD_L_A;  // ld l, a

loop:
    SET_PC(0x22D7U);
    PUSH_HL;  // push hl
    LD_A_hli;  // ld a, [hli]
    CP_A_E;  // cp e
    IF_NZ goto next;  // jr nz, .next
    LD_A_hli;  // ld a, [hli]
    CP_A_D;  // cp d
    IF_NZ goto next;  // jr nz, .next
    goto found_warp;  // jr .found_warp


next:
    SET_PC(0x22E2U);
    POP_HL;  // pop hl
    LD_A(WARP_EVENT_SIZE);  // ld a, WARP_EVENT_SIZE
    ADD_A_L;  // add l
    LD_L_A;  // ld l, a
    IF_NC goto okay;  // jr nc, .okay
    INC_H;  // inc h


okay:
    SET_PC(0x22EAU);
    DEC_C;  // dec c
    IF_NZ goto loop;  // jr nz, .loop
    XOR_A_A;  // xor a
    RET;  // ret


found_warp:
    SET_PC(0x22EFU);
    POP_HL;  // pop hl
    CALL(mGetDestinationWarpNumber_IncreaseHLTwice);  // call .IncreaseHLTwice
    RET_NC ;  // ret nc ; never encountered

    LD_A_addr(wCurMapWarpCount);  // ld a, [wCurMapWarpCount]
    INC_A;  // inc a
    SUB_A_C;  // sub c
    LD_C_A;  // ld c, a
    SCF;  // scf
    RET;  // ret


IncreaseHLTwice:
    SET_PC(0x22FCU);
    INC_HL;  // inc hl
    INC_HL;  // inc hl
    SCF;  // scf
    RET;  // ret

}

void CopyWarpData(void){
    SET_PC(0x2300U);
    LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
    PUSH_AF;  // push af

    CCALL(aSwitchToMapScriptsBank);  // call SwitchToMapScriptsBank
    CALL(mCopyWarpData_CopyWarpData);  // call .CopyWarpData

    POP_AF;  // pop af
    RST(mBankswitch);  // rst Bankswitch
    SCF;  // scf
    RET;  // ret


CopyWarpData:
    SET_PC(0x230DU);
    PUSH_BC;  // push bc
    LD_HL(wCurMapWarpsPointer);  // ld hl, wCurMapWarpsPointer
    LD_A_hli;  // ld a, [hli]
    LD_H_hl;  // ld h, [hl]
    LD_L_A;  // ld l, a
    LD_A_C;  // ld a, c
    DEC_A;  // dec a
    LD_BC(WARP_EVENT_SIZE);  // ld bc, WARP_EVENT_SIZE
    CCALL(aAddNTimes);  // call AddNTimes
    LD_BC(2);  // ld bc, 2 ; warp number
    ADD_HL_BC;  // add hl, bc
    LD_A_hli;  // ld a, [hli]
    CP_A(-1);  // cp -1
    IF_NZ goto skip;  // jr nz, .skip
    LD_HL(wBackupWarpNumber);  // ld hl, wBackupWarpNumber
    LD_A_hli;  // ld a, [hli]


skip:
    SET_PC(0x2329U);
    POP_BC;  // pop bc
    LD_addr_A(wNextWarp);  // ld [wNextWarp], a
    LD_A_hli;  // ld a, [hli]
    LD_addr_A(wNextMapGroup);  // ld [wNextMapGroup], a
    LD_A_hli;  // ld a, [hli]
    LD_addr_A(wNextMapNumber);  // ld [wNextMapNumber], a

    LD_A_C;  // ld a, c
    LD_addr_A(wPrevWarp);  // ld [wPrevWarp], a
    LD_A_addr(wMapGroup);  // ld a, [wMapGroup]
    LD_addr_A(wPrevMapGroup);  // ld [wPrevMapGroup], a
    LD_A_addr(wMapNumber);  // ld a, [wMapNumber]
    LD_addr_A(wPrevMapNumber);  // ld [wPrevMapNumber], a
    SCF;  // scf
    RET;  // ret

}

void EnterMapWarp(void){
    SET_PC(0x2347U);
    CALL(mEnterMapWarp_SaveDigWarp);  // call .SaveDigWarp
    CALL(mEnterMapWarp_SetSpawn);  // call .SetSpawn
    LD_A_addr(wNextWarp);  // ld a, [wNextWarp]
    LD_addr_A(wWarpNumber);  // ld [wWarpNumber], a
    LD_A_addr(wNextMapGroup);  // ld a, [wNextMapGroup]
    LD_addr_A(wMapGroup);  // ld [wMapGroup], a
    LD_A_addr(wNextMapNumber);  // ld a, [wNextMapNumber]
    LD_addr_A(wMapNumber);  // ld [wMapNumber], a
    RET;  // ret


SaveDigWarp:
    SET_PC(0x2360U);
    CCALL(aGetMapEnvironment);  // call GetMapEnvironment
    CCALL(aCheckOutdoorMap);  // call CheckOutdoorMap
    RET_NZ ;  // ret nz
    LD_A_addr(wNextMapGroup);  // ld a, [wNextMapGroup]
    LD_B_A;  // ld b, a
    LD_A_addr(wNextMapNumber);  // ld a, [wNextMapNumber]
    LD_C_A;  // ld c, a
    CCALL(aGetAnyMapEnvironment);  // call GetAnyMapEnvironment
    CCALL(aCheckIndoorMap);  // call CheckIndoorMap
    RET_NZ ;  // ret nz

//  MOUNT_MOON_SQUARE and TIN_TOWER_ROOF are outdoor maps within indoor maps.
//  Dig and Escape Rope should not take you to them.
    LD_A_addr(wPrevMapGroup);  // ld a, [wPrevMapGroup]
    CP_A(GROUP_MOUNT_MOON_SQUARE);  // cp GROUP_MOUNT_MOON_SQUARE
    IF_NZ goto not_mt_moon_square_or_tin_tower_roof;  // jr nz, .not_mt_moon_square_or_tin_tower_roof
    //assert ['GROUP_MOUNT_MOON_SQUARE == GROUP_TIN_TOWER_ROOF'];  // assert GROUP_MOUNT_MOON_SQUARE == GROUP_TIN_TOWER_ROOF
    LD_A_addr(wPrevMapNumber);  // ld a, [wPrevMapNumber]
    CP_A(MAP_MOUNT_MOON_SQUARE);  // cp MAP_MOUNT_MOON_SQUARE
    RET_Z ;  // ret z
    CP_A(MAP_TIN_TOWER_ROOF);  // cp MAP_TIN_TOWER_ROOF
    RET_Z ;  // ret z

not_mt_moon_square_or_tin_tower_roof:
    SET_PC(0x2386U);

    LD_A_addr(wPrevWarp);  // ld a, [wPrevWarp]
    LD_addr_A(wDigWarpNumber);  // ld [wDigWarpNumber], a
    LD_A_addr(wPrevMapGroup);  // ld a, [wPrevMapGroup]
    LD_addr_A(wDigMapGroup);  // ld [wDigMapGroup], a
    LD_A_addr(wPrevMapNumber);  // ld a, [wPrevMapNumber]
    LD_addr_A(wDigMapNumber);  // ld [wDigMapNumber], a
    RET;  // ret


SetSpawn:
    SET_PC(0x2399U);
    CCALL(aGetMapEnvironment);  // call GetMapEnvironment
    CCALL(aCheckOutdoorMap);  // call CheckOutdoorMap
    RET_NZ ;  // ret nz
    LD_A_addr(wNextMapGroup);  // ld a, [wNextMapGroup]
    LD_B_A;  // ld b, a
    LD_A_addr(wNextMapNumber);  // ld a, [wNextMapNumber]
    LD_C_A;  // ld c, a
    CCALL(aGetAnyMapEnvironment);  // call GetAnyMapEnvironment
    CCALL(aCheckIndoorMap);  // call CheckIndoorMap
    RET_NZ ;  // ret nz
    LD_A_addr(wNextMapGroup);  // ld a, [wNextMapGroup]
    LD_B_A;  // ld b, a
    LD_A_addr(wNextMapNumber);  // ld a, [wNextMapNumber]
    LD_C_A;  // ld c, a

//  Respawn in Pokémon Centers.
    CCALL(aGetAnyMapTileset);  // call GetAnyMapTileset
    LD_A_C;  // ld a, c
    CP_A(TILESET_POKECENTER);  // cp TILESET_POKECENTER
    RET_NZ ;  // ret nz
    LD_A_addr(wPrevMapGroup);  // ld a, [wPrevMapGroup]
    LD_addr_A(wLastSpawnMapGroup);  // ld [wLastSpawnMapGroup], a
    LD_A_addr(wPrevMapNumber);  // ld a, [wPrevMapNumber]
    LD_addr_A(wLastSpawnMapNumber);  // ld [wLastSpawnMapNumber], a
    RET;  // ret

}
