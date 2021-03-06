#include "../constants.h"
#include "map_objects.h"

//  Functions handling map objects.

void GetSpritePalette(void){
    SET_PC(0x169AU);
    PUSH_HL;  // push hl
    PUSH_DE;  // push de
    PUSH_BC;  // push bc
    LD_C_A;  // ld c, a

    FARCALL(av_GetSpritePalette);  // farcall _GetSpritePalette

    LD_A_C;  // ld a, c
    POP_BC;  // pop bc
    POP_DE;  // pop de
    POP_HL;  // pop hl
    RET;  // ret

}

void GetSpriteVTile(void){
    PUSH_HL;  // push hl
    PUSH_BC;  // push bc
    LD_HL(wUsedSprites + 2);  // ld hl, wUsedSprites + 2
    LD_C(SPRITE_GFX_LIST_CAPACITY - 1);  // ld c, SPRITE_GFX_LIST_CAPACITY - 1
    LD_B_A;  // ld b, a
    LDH_A_addr(hMapObjectIndex);  // ldh a, [hMapObjectIndex]
    CP_A(0);  // cp 0
    IF_Z goto nope;  // jr z, .nope
    LD_A_B;  // ld a, b

loop:
    CP_A_hl;  // cp [hl]
    IF_Z goto found;  // jr z, .found
    INC_HL;  // inc hl
    INC_HL;  // inc hl
    DEC_C;  // dec c
    IF_NZ goto loop;  // jr nz, .loop
    LD_A_addr(wUsedSprites + 1);  // ld a, [wUsedSprites + 1]
    SCF;  // scf
    goto done;  // jr .done


nope:
    LD_A_addr(wUsedSprites + 1);  // ld a, [wUsedSprites + 1]
    goto done;  // jr .done


found:
    INC_HL;  // inc hl
    XOR_A_A;  // xor a
    LD_A_hl;  // ld a, [hl]


done:
    POP_BC;  // pop bc
    POP_HL;  // pop hl
    RET;  // ret

}

void DoesSpriteHaveFacings(void){
    SET_PC(0x16D1U);
    PUSH_DE;  // push de
    PUSH_HL;  // push hl

    LD_B_A;  // ld b, a
    LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
    PUSH_AF;  // push af
    LD_A(BANK(av_DoesSpriteHaveFacings));  // ld a, BANK(_DoesSpriteHaveFacings)
    RST(mBankswitch);  // rst Bankswitch

    LD_A_B;  // ld a, b
    CALL(mv_DoesSpriteHaveFacings);  // call _DoesSpriteHaveFacings
    LD_C_A;  // ld c, a

    POP_DE;  // pop de
    LD_A_D;  // ld a, d
    RST(mBankswitch);  // rst Bankswitch

    POP_HL;  // pop hl
    POP_DE;  // pop de
    RET;  // ret

}

void GetPlayerStandingTile(void){
    LD_A_addr(wPlayerStandingTile);  // ld a, [wPlayerStandingTile]
    CCALL(aGetTileCollision);  // call GetTileCollision
    LD_B_A;  // ld b, a
    RET;  // ret

}

void CheckOnWater(void){
    LD_A_addr(wPlayerStandingTile);  // ld a, [wPlayerStandingTile]
    CCALL(aGetTileCollision);  // call GetTileCollision
    SUB_A(WATER_TILE);  // sub WATER_TILE
    RET_Z ;  // ret z
    AND_A_A;  // and a
    RET;  // ret

}

void GetTileCollision(void){
//  Get the collision type of tile a.

    PUSH_DE;  // push de
    PUSH_HL;  // push hl

    LD_HL(mTileCollisionTable);  // ld hl, TileCollisionTable
    LD_E_A;  // ld e, a
    LD_D(0);  // ld d, 0
    ADD_HL_DE;  // add hl, de

    LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
    PUSH_AF;  // push af
    LD_A(BANK(aTileCollisionTable));  // ld a, BANK(TileCollisionTable)
    Bankswitch();  // rst Bankswitch
    LD_E_hl;  // ld e, [hl]
    POP_AF;  // pop af
    Bankswitch();  // rst Bankswitch

    LD_A_E;  // ld a, e
    AND_A(0xf);  // and $f ; lo nybble only

    POP_HL;  // pop hl
    POP_DE;  // pop de
    RET;  // ret

}

void CheckGrassTile(void){
    LD_D_A;  // ld d, a
    AND_A(0xf0);  // and $f0
    CP_A(HI_NYBBLE_TALL_GRASS);  // cp HI_NYBBLE_TALL_GRASS
    IF_Z goto grass;  // jr z, .grass
    CP_A(HI_NYBBLE_WATER);  // cp HI_NYBBLE_WATER
    IF_Z goto water;  // jr z, .water
    SCF;  // scf
    RET;  // ret


grass:
    LD_A_D;  // ld a, d
    AND_A(LO_NYBBLE_GRASS);  // and LO_NYBBLE_GRASS
    RET_Z ;  // ret z
    SCF;  // scf
    RET;  // ret
//  For some reason, the above code is duplicated down here.

water:
    LD_A_D;  // ld a, d
    AND_A(LO_NYBBLE_GRASS);  // and LO_NYBBLE_GRASS
    RET_Z ;  // ret z
    SCF;  // scf
    RET;  // ret

}

void CheckSuperTallGrassTile(void){
    CP_A(COLL_LONG_GRASS);  // cp COLL_LONG_GRASS
    RET_Z ;  // ret z
    CP_A(COLL_LONG_GRASS_1C);  // cp COLL_LONG_GRASS_1C
    RET;  // ret

}

void CheckCutTreeTile(void){
    CP_A(COLL_CUT_TREE);  // cp COLL_CUT_TREE
    RET_Z ;  // ret z
    CP_A(COLL_CUT_TREE_1A);  // cp COLL_CUT_TREE_1A
    RET;  // ret

}

void CheckHeadbuttTreeTile(void){
    CP_A(COLL_HEADBUTT_TREE);  // cp COLL_HEADBUTT_TREE
    RET_Z ;  // ret z
    CP_A(COLL_HEADBUTT_TREE_1D);  // cp COLL_HEADBUTT_TREE_1D
    RET;  // ret

}

void CheckCounterTile(void){
    CP_A(COLL_COUNTER);  // cp COLL_COUNTER
    RET_Z ;  // ret z
    CP_A(COLL_COUNTER_98);  // cp COLL_COUNTER_98
    RET;  // ret

}

void CheckPitTile(void){
    CP_A(COLL_PIT);  // cp COLL_PIT
    RET_Z ;  // ret z
    CP_A(COLL_PIT_68);  // cp COLL_PIT_68
    RET;  // ret

}

void CheckIceTile(void){
    CP_A(COLL_ICE);  // cp COLL_ICE
    RET_Z ;  // ret z
    CP_A(COLL_ICE_2B);  // cp COLL_ICE_2B
    RET_Z ;  // ret z
    SCF;  // scf
    RET;  // ret

}

void CheckWhirlpoolTile(void){
    NOP;  // nop
    CP_A(COLL_WHIRLPOOL);  // cp COLL_WHIRLPOOL
    RET_Z ;  // ret z
    CP_A(COLL_WHIRLPOOL_2C);  // cp COLL_WHIRLPOOL_2C
    RET_Z ;  // ret z
    SCF;  // scf
    RET;  // ret

}

void CheckWaterfallTile(void){
    CP_A(COLL_WATERFALL);  // cp COLL_WATERFALL
    RET_Z ;  // ret z
    CP_A(COLL_CURRENT_DOWN);  // cp COLL_CURRENT_DOWN
    RET;  // ret

}

void CheckStandingOnEntrance(void){
    LD_A_addr(wPlayerStandingTile);  // ld a, [wPlayerStandingTile]
    CP_A(COLL_DOOR);  // cp COLL_DOOR
    RET_Z ;  // ret z
    CP_A(COLL_DOOR_79);  // cp COLL_DOOR_79
    RET_Z ;  // ret z
    CP_A(COLL_STAIRCASE);  // cp COLL_STAIRCASE
    RET_Z ;  // ret z
    CP_A(COLL_CAVE);  // cp COLL_CAVE
    RET;  // ret

}

void GetMapObject(void){
//  Return the location of map object a in bc.
    LD_HL(wMapObjects);  // ld hl, wMapObjects
    LD_BC(MAPOBJECT_LENGTH);  // ld bc, MAPOBJECT_LENGTH
    CCALL(aAddNTimes);  // call AddNTimes
    LD_B_H;  // ld b, h
    LD_C_L;  // ld c, l
    RET;  // ret

}

void CheckObjectVisibility(void){
//  Sets carry if the object is not visible on the screen.
    LDH_addr_A(hMapObjectIndex);  // ldh [hMapObjectIndex], a
    CCALL(aGetMapObject);  // call GetMapObject
    LD_HL(MAPOBJECT_OBJECT_STRUCT_ID);  // ld hl, MAPOBJECT_OBJECT_STRUCT_ID
    ADD_HL_BC;  // add hl, bc
    LD_A_hl;  // ld a, [hl]
    CP_A(-1);  // cp -1
    IF_Z goto not_visible;  // jr z, .not_visible
    LDH_addr_A(hObjectStructIndex);  // ldh [hObjectStructIndex], a
    CCALL(aGetObjectStruct);  // call GetObjectStruct
    AND_A_A;  // and a
    RET;  // ret


not_visible:
    SCF;  // scf
    RET;  // ret

}

void CheckObjectTime(void){
    LD_HL(MAPOBJECT_HOUR);  // ld hl, MAPOBJECT_HOUR
    ADD_HL_BC;  // add hl, bc
    LD_A_hl;  // ld a, [hl]
    CP_A(-1);  // cp -1
    IF_NZ goto check_hour;  // jr nz, .check_hour
    LD_HL(MAPOBJECT_TIMEOFDAY);  // ld hl, MAPOBJECT_TIMEOFDAY
    ADD_HL_BC;  // add hl, bc
    LD_A_hl;  // ld a, [hl]
    CP_A(-1);  // cp -1
    IF_Z goto timeofday_always;  // jr z, .timeofday_always
    LD_HL(mCheckObjectTime_TimesOfDay);  // ld hl, .TimesOfDay
    LD_A_addr(wTimeOfDay);  // ld a, [wTimeOfDay]
    ADD_A_L;  // add l
    LD_L_A;  // ld l, a
    IF_NC goto ok;  // jr nc, .ok
    INC_H;  // inc h


ok:
    LD_A_hl;  // ld a, [hl]
    LD_HL(MAPOBJECT_TIMEOFDAY);  // ld hl, MAPOBJECT_TIMEOFDAY
    ADD_HL_BC;  // add hl, bc
    AND_A_hl;  // and [hl]
    IF_NZ goto timeofday_always;  // jr nz, .timeofday_always
    SCF;  // scf
    RET;  // ret


timeofday_always:
    AND_A_A;  // and a
    RET;  // ret


TimesOfDay:
//  entries correspond to TimeOfDay values
    //db ['MORN'];  // db MORN
    //db ['DAY'];  // db DAY
    //db ['NITE'];  // db NITE


check_hour:
    LD_HL(MAPOBJECT_HOUR);  // ld hl, MAPOBJECT_HOUR
    ADD_HL_BC;  // add hl, bc
    LD_D_hl;  // ld d, [hl]
    LD_HL(MAPOBJECT_TIMEOFDAY);  // ld hl, MAPOBJECT_TIMEOFDAY
    ADD_HL_BC;  // add hl, bc
    LD_E_hl;  // ld e, [hl]
    LD_HL(hHours);  // ld hl, hHours
    LD_A_D;  // ld a, d
    CP_A_E;  // cp e
    IF_Z goto yes;  // jr z, .yes
    IF_C goto check_timeofday;  // jr c, .check_timeofday
    LD_A_hl;  // ld a, [hl]
    CP_A_D;  // cp d
    IF_NC goto yes;  // jr nc, .yes
    CP_A_E;  // cp e
    IF_C goto yes;  // jr c, .yes
    IF_Z goto yes;  // jr z, .yes
    goto no;  // jr .no


check_timeofday:
    LD_A_E;  // ld a, e
    CP_A_hl;  // cp [hl]
    IF_C goto no;  // jr c, .no
    LD_A_hl;  // ld a, [hl]
    CP_A_D;  // cp d
    IF_NC goto yes;  // jr nc, .yes
    goto no;  // jr .no


yes:
    AND_A_A;  // and a
    RET;  // ret


no:
    SCF;  // scf
    RET;  // ret

}

void CopyMapObjectStruct(void){
    SET_PC(0x17E8U);
//  //  unreferenced
    LDH_addr_A(hMapObjectIndex);  // ldh [hMapObjectIndex], a
    CCALL(aGetMapObject);  // call GetMapObject
    CALL(mCopyObjectStruct);  // call CopyObjectStruct
    RET;  // ret

}

void UnmaskCopyMapObjectStruct(void){
    SET_PC(0x17F1U);
    LDH_addr_A(hMapObjectIndex);  // ldh [hMapObjectIndex], a
    CCALL(aUnmaskObject);  // call UnmaskObject
    LDH_A_addr(hMapObjectIndex);  // ldh a, [hMapObjectIndex]
    CCALL(aGetMapObject);  // call GetMapObject
    FARCALL(aCopyObjectStruct);  // farcall CopyObjectStruct
    RET;  // ret

}

void ApplyDeletionToMapObject(void){
    SET_PC(0x1802U);
    LDH_addr_A(hMapObjectIndex);  // ldh [hMapObjectIndex], a
    CCALL(aGetMapObject);  // call GetMapObject
    LD_HL(MAPOBJECT_OBJECT_STRUCT_ID);  // ld hl, MAPOBJECT_OBJECT_STRUCT_ID
    ADD_HL_BC;  // add hl, bc
    LD_A_hl;  // ld a, [hl]
    CP_A(-1);  // cp -1
    RET_Z ;  // ret z ; already hidden
    LD_hl(-1);  // ld [hl], -1
    PUSH_AF;  // push af
    CALL(mApplyDeletionToMapObject_CheckStopFollow);  // call .CheckStopFollow
    POP_AF;  // pop af
    CCALL(aGetObjectStruct);  // call GetObjectStruct
    FARCALL(aDeleteMapObject);  // farcall DeleteMapObject
    RET;  // ret


CheckStopFollow:
    SET_PC(0x1820U);
    LD_HL(wObjectFollow_Leader);  // ld hl, wObjectFollow_Leader
    CP_A_hl;  // cp [hl]
    IF_Z goto ok;  // jr z, .ok
    LD_HL(wObjectFollow_Follower);  // ld hl, wObjectFollow_Follower
    CP_A_hl;  // cp [hl]
    RET_NZ ;  // ret nz

ok:
    SET_PC(0x182BU);
    FARCALL(aStopFollow);  // farcall StopFollow
    LD_A(-1);  // ld a, -1
    LD_addr_A(wObjectFollow_Leader);  // ld [wObjectFollow_Leader], a
    LD_addr_A(wObjectFollow_Follower);  // ld [wObjectFollow_Follower], a
    RET;  // ret

}

void DeleteObjectStruct(void){
    SET_PC(0x183AU);
    CALL(mApplyDeletionToMapObject);  // call ApplyDeletionToMapObject
    CCALL(aMaskObject);  // call MaskObject
    RET;  // ret

}

void CopyPlayerObjectTemplate(void){
    PUSH_HL;  // push hl
    CCALL(aGetMapObject);  // call GetMapObject
    LD_D_B;  // ld d, b
    LD_E_C;  // ld e, c
    LD_A(-1);  // ld a, -1
    LD_de_A;  // ld [de], a
    INC_DE;  // inc de
    POP_HL;  // pop hl
    LD_BC(MAPOBJECT_LENGTH - 1);  // ld bc, MAPOBJECT_LENGTH - 1
    CCALL(aCopyBytes);  // call CopyBytes
    RET;  // ret

}

void DeleteFollowerMapObject(void){
    SET_PC(0x1853U);
//  //  unreferenced
    CCALL(aGetMapObject);  // call GetMapObject
    LD_HL(MAPOBJECT_OBJECT_STRUCT_ID);  // ld hl, MAPOBJECT_OBJECT_STRUCT_ID
    ADD_HL_BC;  // add hl, bc
    LD_A_hl;  // ld a, [hl]
    PUSH_AF;  // push af
    LD_hl(-1);  // ld [hl], -1
    INC_HL;  // inc hl
    LD_BC(MAPOBJECT_LENGTH - 1);  // ld bc, MAPOBJECT_LENGTH - 1
    XOR_A_A;  // xor a
    CCALL(aByteFill);  // call ByteFill
    POP_AF;  // pop af
    CP_A(-1);  // cp -1
    RET_Z ;  // ret z
    CP_A(NUM_OBJECT_STRUCTS);  // cp NUM_OBJECT_STRUCTS
    RET_NC ;  // ret nc
    LD_B_A;  // ld b, a
    LD_A_addr(wObjectFollow_Leader);  // ld a, [wObjectFollow_Leader]
    CP_A_B;  // cp b
    IF_NZ goto ok;  // jr nz, .ok
    LD_A(-1);  // ld a, -1
    LD_addr_A(wObjectFollow_Leader);  // ld [wObjectFollow_Leader], a


ok:
    SET_PC(0x1879U);
    LD_A_B;  // ld a, b
    CCALL(aGetObjectStruct);  // call GetObjectStruct
    FARCALL(aDeleteMapObject);  // farcall DeleteMapObject
    RET;  // ret

}

void LoadMovementDataPointer(void){
//  Load the movement data pointer for object a.
    LD_addr_A(wMovementObject);  // ld [wMovementObject], a
    LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
    LD_addr_A(wMovementDataBank);  // ld [wMovementDataBank], a
    LD_A_L;  // ld a, l
    LD_addr_A(wMovementDataAddress);  // ld [wMovementDataAddress], a
    LD_A_H;  // ld a, h
    LD_addr_A(wMovementDataAddress + 1);  // ld [wMovementDataAddress + 1], a
    LD_A_addr(wMovementObject);  // ld a, [wMovementObject]
    CCALL(aCheckObjectVisibility);  // call CheckObjectVisibility
    RET_C ;  // ret c

    LD_HL(OBJECT_MOVEMENTTYPE);  // ld hl, OBJECT_MOVEMENTTYPE
    ADD_HL_BC;  // add hl, bc
    LD_hl(SPRITEMOVEDATA_SCRIPTED);  // ld [hl], SPRITEMOVEDATA_SCRIPTED

    LD_HL(OBJECT_STEP_TYPE);  // ld hl, OBJECT_STEP_TYPE
    ADD_HL_BC;  // add hl, bc
    LD_hl(STEP_TYPE_RESET);  // ld [hl], STEP_TYPE_RESET

    LD_HL(wVramState);  // ld hl, wVramState
    SET_hl(7);  // set 7, [hl]
    AND_A_A;  // and a
    RET;  // ret

}

void FindFirstEmptyObjectStruct(void){
//  Returns the index of the first empty object struct in A and its address in HL, then sets carry.
//  If all object structs are occupied, A = 0 and Z is set.
//  Preserves BC and DE.
    PUSH_BC;  // push bc
    PUSH_DE;  // push de
    LD_HL(wObjectStructs);  // ld hl, wObjectStructs
    LD_DE(OBJECT_LENGTH);  // ld de, OBJECT_LENGTH
    LD_C(NUM_OBJECT_STRUCTS);  // ld c, NUM_OBJECT_STRUCTS

loop:
    LD_A_hl;  // ld a, [hl]
    AND_A_A;  // and a
    IF_Z goto l_break;  // jr z, .break
    ADD_HL_DE;  // add hl, de
    DEC_C;  // dec c
    IF_NZ goto loop;  // jr nz, .loop
    XOR_A_A;  // xor a
    goto done;  // jr .done


l_break:
    LD_A(NUM_OBJECT_STRUCTS);  // ld a, NUM_OBJECT_STRUCTS
    SUB_A_C;  // sub c
    SCF;  // scf


done:
    POP_DE;  // pop de
    POP_BC;  // pop bc
    RET;  // ret

}

void GetSpriteMovementFunction(void){
    LD_HL(OBJECT_MOVEMENTTYPE);  // ld hl, OBJECT_MOVEMENTTYPE
    ADD_HL_BC;  // add hl, bc
    LD_A_hl;  // ld a, [hl]
    CP_A(NUM_SPRITEMOVEDATA);  // cp NUM_SPRITEMOVEDATA
    IF_C goto ok;  // jr c, .ok
    XOR_A_A;  // xor a


ok:
    LD_HL(mSpriteMovementData + SPRITEMOVEATTR_MOVEMENT);  // ld hl, SpriteMovementData + SPRITEMOVEATTR_MOVEMENT
    LD_E_A;  // ld e, a
    LD_D(0);  // ld d, 0
    for(int rept = 0; rept < NUM_SPRITEMOVEDATA_FIELDS; rept++){
    ADD_HL_DE;  // add hl, de
    }
    LD_A_hl;  // ld a, [hl]
    RET;  // ret

}

void GetInitialFacing(void){
    PUSH_BC;  // push bc
    PUSH_DE;  // push de
    LD_E_A;  // ld e, a
    LD_D(0);  // ld d, 0
    LD_HL(mSpriteMovementData + SPRITEMOVEATTR_FACING);  // ld hl, SpriteMovementData + SPRITEMOVEATTR_FACING
    for(int rept = 0; rept < NUM_SPRITEMOVEDATA_FIELDS; rept++){
    ADD_HL_DE;  // add hl, de
    }
    LD_A(BANK(aSpriteMovementData));  // ld a, BANK(SpriteMovementData)
    CCALL(aGetFarByte);  // call GetFarByte
    ADD_A_A;  // add a
    ADD_A_A;  // add a
    maskbits(NUM_DIRECTIONS, 2);  // maskbits NUM_DIRECTIONS, 2
    POP_DE;  // pop de
    POP_BC;  // pop bc
    RET;  // ret

}

void CopySpriteMovementData(void){
    SET_PC(0x18FCU);
    LD_L_A;  // ld l, a
    LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
    PUSH_AF;  // push af
    LD_A(BANK(aSpriteMovementData));  // ld a, BANK(SpriteMovementData)
    RST(mBankswitch);  // rst Bankswitch
    LD_A_L;  // ld a, l
    PUSH_BC;  // push bc

    CALL(mCopySpriteMovementData_CopyData);  // call .CopyData

    POP_BC;  // pop bc
    POP_AF;  // pop af
    RST(mBankswitch);  // rst Bankswitch

    RET;  // ret


CopyData:
    SET_PC(0x190CU);
    LD_HL(OBJECT_MOVEMENTTYPE);  // ld hl, OBJECT_MOVEMENTTYPE
    ADD_HL_DE;  // add hl, de
    LD_hl_A;  // ld [hl], a

    PUSH_DE;  // push de
    LD_E_A;  // ld e, a
    LD_D(0);  // ld d, 0
    LD_HL(mSpriteMovementData + SPRITEMOVEATTR_FACING);  // ld hl, SpriteMovementData + SPRITEMOVEATTR_FACING
    for(int rept = 0; rept < NUM_SPRITEMOVEDATA_FIELDS; rept++){
    ADD_HL_DE;  // add hl, de
    }
    LD_B_H;  // ld b, h
    LD_C_L;  // ld c, l
    POP_DE;  // pop de

    LD_A_bc;  // ld a, [bc]
    INC_BC;  // inc bc
    RLCA;  // rlca
    RLCA;  // rlca
    maskbits(NUM_DIRECTIONS, 2);  // maskbits NUM_DIRECTIONS, 2
    LD_HL(OBJECT_FACING);  // ld hl, OBJECT_FACING
    ADD_HL_DE;  // add hl, de
    LD_hl_A;  // ld [hl], a

    LD_A_bc;  // ld a, [bc]
    INC_BC;  // inc bc
    LD_HL(OBJECT_ACTION);  // ld hl, OBJECT_ACTION
    ADD_HL_DE;  // add hl, de
    LD_hl_A;  // ld [hl], a

    LD_A_bc;  // ld a, [bc]
    INC_BC;  // inc bc
    LD_HL(OBJECT_FLAGS1);  // ld hl, OBJECT_FLAGS1
    ADD_HL_DE;  // add hl, de
    LD_hl_A;  // ld [hl], a

    LD_A_bc;  // ld a, [bc]
    INC_BC;  // inc bc
    LD_HL(OBJECT_FLAGS2);  // ld hl, OBJECT_FLAGS2
    ADD_HL_DE;  // add hl, de
    LD_hl_A;  // ld [hl], a

    LD_A_bc;  // ld a, [bc]
    INC_BC;  // inc bc
    LD_HL(OBJECT_PALETTE);  // ld hl, OBJECT_PALETTE
    ADD_HL_DE;  // add hl, de
    LD_hl_A;  // ld [hl], a
    RET;  // ret

}

void v_GetMovementByte(void){
//  Switch to the movement data bank
    LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
    PUSH_AF;  // push af
    LD_A_hli;  // ld a, [hli]
    Bankswitch();  // rst Bankswitch
//  Load the current script byte as given by OBJECT_MOVEMENT_BYTE_INDEX, and increment OBJECT_MOVEMENT_BYTE_INDEX
    LD_A_hli;  // ld a, [hli]
    LD_D_hl;  // ld d, [hl]
    LD_HL(OBJECT_MOVEMENT_BYTE_INDEX);  // ld hl, OBJECT_MOVEMENT_BYTE_INDEX
    ADD_HL_BC;  // add hl, bc
    ADD_A_hl;  // add [hl]
    LD_E_A;  // ld e, a
    LD_A_D;  // ld a, d
    ADC_A(0);  // adc 0
    LD_D_A;  // ld d, a
    INC_hl;  // inc [hl]
    LD_A_de;  // ld a, [de]
    LD_H_A;  // ld h, a
    POP_AF;  // pop af
    Bankswitch();  // rst Bankswitch

    LD_A_H;  // ld a, h
    RET;  // ret

}

void SetVramState_Bit0(void){
//  //  unreferenced
    LD_HL(wVramState);  // ld hl, wVramState
    SET_hl(0);  // set 0, [hl]
    RET;  // ret

}

void ResetVramState_Bit0(void){
//  //  unreferenced
    LD_HL(wVramState);  // ld hl, wVramState
    RES_hl(0);  // res 0, [hl]
    RET;  // ret

}

void UpdateSprites(void){
    SET_PC(0x196DU);
    LD_A_addr(wVramState);  // ld a, [wVramState]
    BIT_A(0);  // bit 0, a
    RET_Z ;  // ret z

    FARCALL(aUpdateAllObjectsFrozen);  // farcall UpdateAllObjectsFrozen
    FARCALL(av_UpdateSprites);  // farcall _UpdateSprites
    RET;  // ret

}

void GetObjectStruct(void){
    LD_BC(OBJECT_LENGTH);  // ld bc, OBJECT_LENGTH
    LD_HL(wObjectStructs);  // ld hl, wObjectStructs
    CCALL(aAddNTimes);  // call AddNTimes
    LD_B_H;  // ld b, h
    LD_C_L;  // ld c, l
    RET;  // ret

}

void DoesObjectHaveASprite(void){
    LD_HL(OBJECT_SPRITE);  // ld hl, OBJECT_SPRITE
    ADD_HL_BC;  // add hl, bc
    LD_A_hl;  // ld a, [hl]
    AND_A_A;  // and a
    RET;  // ret

}

void SetSpriteDirection(void){
// ; preserves other flags
    PUSH_AF;  // push af
    LD_HL(OBJECT_FACING);  // ld hl, OBJECT_FACING
    ADD_HL_BC;  // add hl, bc
    LD_A_hl;  // ld a, [hl]
    AND_A(0b11110011);  // and %11110011
    LD_E_A;  // ld e, a
    POP_AF;  // pop af
    maskbits(NUM_DIRECTIONS, 2);  // maskbits NUM_DIRECTIONS, 2
    OR_A_E;  // or e
    LD_hl_A;  // ld [hl], a
    RET;  // ret

}

void GetSpriteDirection(void){
    LD_HL(OBJECT_FACING);  // ld hl, OBJECT_FACING
    ADD_HL_BC;  // add hl, bc
    LD_A_hl;  // ld a, [hl]
    maskbits(NUM_DIRECTIONS, 2);  // maskbits NUM_DIRECTIONS, 2
    RET;  // ret

}
