#include "../constants.h"

//  Functions dealing with rendering and interacting with maps.

int ClearUnusedMapBuffer(){
	SET_PC(0x1F5BU);
	LD_HL(wUnusedMapBuffer);  // ld hl, wUnusedMapBuffer
	LD_BC(wUnusedMapBufferEnd - wUnusedMapBuffer);  // ld bc, wUnusedMapBufferEnd - wUnusedMapBuffer
	LD_A(0);  // ld a, 0
	CALL(mByteFill);  // call ByteFill
	RET;  // ret

}

int CheckScenes(){
	SET_PC(0x1F67U);
//  Checks wCurMapSceneScriptPointer.  If it's empty, returns -1 in a.  Otherwise, returns the active scene ID in a.
	PUSH_HL;  // push hl
	LD_HL(wCurMapSceneScriptPointer);  // ld hl, wCurMapSceneScriptPointer
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	OR_A_H;  // or h
	LD_A_hl;  // ld a, [hl]
	IF_NZ goto scene_exists;  // jr nz, .scene_exists
	LD_A(-1);  // ld a, -1


scene_exists:
	SET_PC(0x1F74U);
	POP_HL;  // pop hl
	RET;  // ret

}

int GetCurrentMapSceneID(){
	SET_PC(0x1F76U);
//  Grabs the wram map scene script pointer for the current map and loads it into wCurMapSceneScriptPointer.
//  If there is no scene, both bytes of wCurMapSceneScriptPointer are wiped clean.
//  Copy the current map group and number into bc.  This is needed for GetMapSceneID.
	LD_A_addr(wMapGroup);  // ld a, [wMapGroup]
	LD_B_A;  // ld b, a
	LD_A_addr(wMapNumber);  // ld a, [wMapNumber]
	LD_C_A;  // ld c, a
//  Blank out wCurMapSceneScriptPointer
	XOR_A_A;  // xor a
	LD_addr_A(wCurMapSceneScriptPointer);  // ld [wCurMapSceneScriptPointer], a
	LD_addr_A(wCurMapSceneScriptPointer + 1);  // ld [wCurMapSceneScriptPointer + 1], a
	CALL(mGetMapSceneID);  // call GetMapSceneID
	RET_C ;  // ret c ; The map is not in the scene script table
//  Load the scene script pointer from de into wCurMapSceneScriptPointer
	LD_A_E;  // ld a, e
	LD_addr_A(wCurMapSceneScriptPointer);  // ld [wCurMapSceneScriptPointer], a
	LD_A_D;  // ld a, d
	LD_addr_A(wCurMapSceneScriptPointer + 1);  // ld [wCurMapSceneScriptPointer + 1], a
	XOR_A_A;  // xor a
	RET;  // ret

}

int GetMapSceneID(){
	SET_PC(0x1F93U);
//  Searches the scene_var table for the map group and number loaded in bc, and returns the wram pointer in de.
//  If the map is not in the scene_var table, returns carry.
	PUSH_BC;  // push bc
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A(BANK(aMapScenes));  // ld a, BANK(MapScenes)
	RST(mBankswitch);  // rst Bankswitch

	LD_HL(mMapScenes);  // ld hl, MapScenes

loop:
	SET_PC(0x1F9DU);
	PUSH_HL;  // push hl
	LD_A_hli;  // ld a, [hli] ; map group, or terminator
	CP_A(-1);  // cp -1
	IF_Z goto end;  // jr z, .end ; the current map is not in the scene_var table
	CP_A_B;  // cp b
	IF_NZ goto next;  // jr nz, .next ; map group did not match
	LD_A_hli;  // ld a, [hli] ; map number
	CP_A_C;  // cp c
	IF_NZ goto next;  // jr nz, .next ; map number did not match
	goto found;  // jr .found ; we found our map


next:
	SET_PC(0x1FACU);
	POP_HL;  // pop hl
	LD_DE(4);  // ld de, 4 ; scene_var size
	ADD_HL_DE;  // add hl, de
	goto loop;  // jr .loop


end:
	SET_PC(0x1FB3U);
	SCF;  // scf
	goto done;  // jr .done


found:
	SET_PC(0x1FB6U);
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]


done:
	SET_PC(0x1FB9U);
	POP_HL;  // pop hl
	POP_BC;  // pop bc
	LD_A_B;  // ld a, b
	RST(mBankswitch);  // rst Bankswitch

	POP_BC;  // pop bc
	RET;  // ret

}

int OverworldTextModeSwitch(){
	SET_PC(0x1FBFU);
	CALL(mLoadMapPart);  // call LoadMapPart
	CALL(mSwapTextboxPalettes);  // call SwapTextboxPalettes
	RET;  // ret

}

int LoadMapPart(){
	SET_PC(0x1FC6U);
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af

	LD_A_addr(wTilesetBlocksBank);  // ld a, [wTilesetBlocksBank]
	RST(mBankswitch);  // rst Bankswitch
	CALL(mLoadMetatiles);  // call LoadMetatiles

	LD_A(0x60);  // ld a, "■"
	hlcoord(0, 0, wTilemap);  // hlcoord 0, 0
	LD_BC(SCREEN_WIDTH * SCREEN_HEIGHT);  // ld bc, SCREEN_WIDTH * SCREEN_HEIGHT
	CALL(mByteFill);  // call ByteFill

	LD_A(BANK(av_LoadMapPart));  // ld a, BANK(_LoadMapPart)
	RST(mBankswitch);  // rst Bankswitch
	CALL(mv_LoadMapPart);  // call _LoadMapPart

	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret

}

int LoadMetatiles(){
	SET_PC(0x1FE4U);
// ; de <- wOverworldMapAnchor
	LD_A_addr(wOverworldMapAnchor);  // ld a, [wOverworldMapAnchor]
	LD_E_A;  // ld e, a
	LD_A_addr(wOverworldMapAnchor + 1);  // ld a, [wOverworldMapAnchor + 1]
	LD_D_A;  // ld d, a
	LD_HL(wSurroundingTiles);  // ld hl, wSurroundingTiles
	LD_B(SCREEN_META_HEIGHT);  // ld b, SCREEN_META_HEIGHT


row:
	SET_PC(0x1FF1U);
	PUSH_DE;  // push de
	PUSH_HL;  // push hl
	LD_C(SCREEN_META_WIDTH);  // ld c, SCREEN_META_WIDTH


col:
	SET_PC(0x1FF5U);
	PUSH_DE;  // push de
	PUSH_HL;  // push hl
// ; Load the current map block.
// ; If the current map block is a border block, load the border block.
	LD_A_de;  // ld a, [de]
	AND_A_A;  // and a
	IF_NZ goto ok;  // jr nz, .ok
	LD_A_addr(wMapBorderBlock);  // ld a, [wMapBorderBlock]


ok:
	SET_PC(0x1FFEU);
// ; Load the current wSurroundingTiles address into de.
	LD_E_L;  // ld e, l
	LD_D_H;  // ld d, h
// ; Set hl to the address of the current metatile data ([wTilesetBlocksAddress] + (a) tiles).
// ; This is buggy; it wraps around past 128 blocks.
// ; To fix, uncomment the line below.
	ADD_A_A;  // add a ; Comment or delete this line to fix the above bug.
	LD_L_A;  // ld l, a
	LD_H(0);  // ld h, 0
// ; add hl, hl
	ADD_HL_HL;  // add hl, hl
	ADD_HL_HL;  // add hl, hl
	ADD_HL_HL;  // add hl, hl
	LD_A_addr(wTilesetBlocksAddress);  // ld a, [wTilesetBlocksAddress]
	ADD_A_L;  // add l
	LD_L_A;  // ld l, a
	LD_A_addr(wTilesetBlocksAddress + 1);  // ld a, [wTilesetBlocksAddress + 1]
	ADC_A_H;  // adc h
	LD_H_A;  // ld h, a

// ; copy the 4x4 metatile
for(int rept = 0; rept < METATILE_WIDTH; rept++){
	LD_A_hli;  // ld a, [hli]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
}
	LD_A_E;  // ld a, e
	ADD_A(SURROUNDING_WIDTH - METATILE_WIDTH);  // add SURROUNDING_WIDTH - METATILE_WIDTH
	LD_E_A;  // ld e, a
	IF_NC goto next_u8129;  // jr nc, .next_u8129
	INC_D;  // inc d

next_u8129:
	SET_PC(0x2024U);
for(int rept = 0; rept < METATILE_WIDTH; rept++){
	LD_A_hli;  // ld a, [hli]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
}
	LD_A_E;  // ld a, e
	ADD_A(SURROUNDING_WIDTH - METATILE_WIDTH);  // add SURROUNDING_WIDTH - METATILE_WIDTH
	LD_E_A;  // ld e, a
	IF_NC goto next_u8134;  // jr nc, .next_u8134
	INC_D;  // inc d

next_u8134:
	SET_PC(0x2037U);
for(int rept = 0; rept < METATILE_WIDTH; rept++){
	LD_A_hli;  // ld a, [hli]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
}
	LD_A_E;  // ld a, e
	ADD_A(SURROUNDING_WIDTH - METATILE_WIDTH);  // add SURROUNDING_WIDTH - METATILE_WIDTH
	LD_E_A;  // ld e, a
	IF_NC goto next_u8139;  // jr nc, .next_u8139
	INC_D;  // inc d

next_u8139:
	SET_PC(0x204AU);
for(int rept = 0; rept < METATILE_WIDTH; rept++){
	LD_A_hli;  // ld a, [hli]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
}
// ; Next metatile
	POP_HL;  // pop hl
	LD_DE(METATILE_WIDTH);  // ld de, METATILE_WIDTH
	ADD_HL_DE;  // add hl, de
	POP_DE;  // pop de
	INC_DE;  // inc de
	DEC_C;  // dec c
	JP_NZ (mLoadMetatiles_col);  // jp nz, .col
// ; Next metarow
	POP_HL;  // pop hl
	LD_DE(SURROUNDING_WIDTH * METATILE_WIDTH);  // ld de, SURROUNDING_WIDTH * METATILE_WIDTH
	ADD_HL_DE;  // add hl, de
	POP_DE;  // pop de
	LD_A_addr(wMapWidth);  // ld a, [wMapWidth]
	ADD_A(MAP_CONNECTION_PADDING_WIDTH * 2);  // add MAP_CONNECTION_PADDING_WIDTH * 2
	ADD_A_E;  // add e
	LD_E_A;  // ld e, a
	IF_NC goto ok2;  // jr nc, .ok2
	INC_D;  // inc d

ok2:
	SET_PC(0x2071U);
	DEC_B;  // dec b
	JP_NZ (mLoadMetatiles_row);  // jp nz, .row
	RET;  // ret

}

int ReturnToMapFromSubmenu(){
	SET_PC(0x2076U);
	LD_A(MAPSETUP_SUBMENU);  // ld a, MAPSETUP_SUBMENU
	LDH_addr_A(hMapEntryMethod);  // ldh [hMapEntryMethod], a
	FARCALL(aRunMapSetupScript);  // farcall RunMapSetupScript
	XOR_A_A;  // xor a
	LDH_addr_A(hMapEntryMethod);  // ldh [hMapEntryMethod], a
	RET;  // ret

// INCLUDE "home/warp_connection.asm"

	return CheckOutdoorMap();
}

int CheckOutdoorMap(){
	SET_PC(0x23CBU);
	CP_A(ROUTE);  // cp ROUTE
	RET_Z ;  // ret z
	CP_A(TOWN);  // cp TOWN
	RET;  // ret

}

int CheckIndoorMap(){
	SET_PC(0x23D1U);
	CP_A(INDOOR);  // cp INDOOR
	RET_Z ;  // ret z
	CP_A(CAVE);  // cp CAVE
	RET_Z ;  // ret z
	CP_A(DUNGEON);  // cp DUNGEON
	RET_Z ;  // ret z
	CP_A(GATE);  // cp GATE
	RET;  // ret

}

int CheckUnknownMap(){
	SET_PC(0x23DDU);
//  //  unreferenced
	CP_A(INDOOR);  // cp INDOOR
	RET_Z ;  // ret z
	CP_A(GATE);  // cp GATE
	RET_Z ;  // ret z
	CP_A(ENVIRONMENT_5);  // cp ENVIRONMENT_5
	RET;  // ret

}

int LoadMapAttributes(){
	SET_PC(0x23E6U);
	CALL(mCopyMapPartialAndAttributes);  // call CopyMapPartialAndAttributes
	CALL(mSwitchToMapScriptsBank);  // call SwitchToMapScriptsBank
	CALL(mReadMapScripts);  // call ReadMapScripts
	XOR_A_A;  // xor a ; do not skip object events
	CALL(mReadMapEvents);  // call ReadMapEvents
	RET;  // ret

}

int LoadMapAttributes_SkipObjects(){
	SET_PC(0x23F4U);
	CALL(mCopyMapPartialAndAttributes);  // call CopyMapPartialAndAttributes
	CALL(mSwitchToMapScriptsBank);  // call SwitchToMapScriptsBank
	CALL(mReadMapScripts);  // call ReadMapScripts
	LD_A(TRUE);  // ld a, TRUE ; skip object events
	CALL(mReadMapEvents);  // call ReadMapEvents
	RET;  // ret

}

int CopyMapPartialAndAttributes(){
	SET_PC(0x2403U);
	CALL(mCopyMapPartial);  // call CopyMapPartial
	CALL(mSwitchToMapAttributesBank);  // call SwitchToMapAttributesBank
	CALL(mGetMapAttributesPointer);  // call GetMapAttributesPointer
	CALL(mCopyMapAttributes);  // call CopyMapAttributes
	CALL(mGetMapConnections);  // call GetMapConnections
	RET;  // ret

}

int ReadMapEvents(){
	SET_PC(0x2413U);
	PUSH_AF;  // push af
	LD_HL(wMapEventsPointer);  // ld hl, wMapEventsPointer
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	INC_HL;  // inc hl
	INC_HL;  // inc hl
	CALL(mReadWarps);  // call ReadWarps
	CALL(mReadCoordEvents);  // call ReadCoordEvents
	CALL(mReadBGEvents);  // call ReadBGEvents

	POP_AF;  // pop af
	AND_A_A;  // and a ; skip object events?
	RET_NZ ;  // ret nz

	CALL(mReadObjectEvents);  // call ReadObjectEvents
	RET;  // ret

}

int ReadMapScripts(){
	SET_PC(0x242CU);
	LD_HL(wMapScriptsPointer);  // ld hl, wMapScriptsPointer
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	CALL(mReadMapSceneScripts);  // call ReadMapSceneScripts
	CALL(mReadMapCallbacks);  // call ReadMapCallbacks
	RET;  // ret

}

int CopyMapAttributes(){
	SET_PC(0x2439U);
	LD_DE(wMapAttributes);  // ld de, wMapAttributes
	LD_C(wMapAttributesEnd - wMapAttributes);  // ld c, wMapAttributesEnd - wMapAttributes

loop:
	SET_PC(0x243EU);
	LD_A_hli;  // ld a, [hli]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

int GetMapConnections(){
	SET_PC(0x2445U);
	LD_A(0xff);  // ld a, $ff
	LD_addr_A(wNorthConnectedMapGroup);  // ld [wNorthConnectedMapGroup], a
	LD_addr_A(wSouthConnectedMapGroup);  // ld [wSouthConnectedMapGroup], a
	LD_addr_A(wWestConnectedMapGroup);  // ld [wWestConnectedMapGroup], a
	LD_addr_A(wEastConnectedMapGroup);  // ld [wEastConnectedMapGroup], a

	LD_A_addr(wMapConnections);  // ld a, [wMapConnections]
	LD_B_A;  // ld b, a

	BIT_B(NORTH_F);  // bit NORTH_F, b
	IF_Z goto no_north;  // jr z, .no_north
	LD_DE(wNorthMapConnection);  // ld de, wNorthMapConnection
	CALL(mGetMapConnection);  // call GetMapConnection

no_north:
	SET_PC(0x2461U);

	BIT_B(SOUTH_F);  // bit SOUTH_F, b
	IF_Z goto no_south;  // jr z, .no_south
	LD_DE(wSouthMapConnection);  // ld de, wSouthMapConnection
	CALL(mGetMapConnection);  // call GetMapConnection

no_south:
	SET_PC(0x246BU);

	BIT_B(WEST_F);  // bit WEST_F, b
	IF_Z goto no_west;  // jr z, .no_west
	LD_DE(wWestMapConnection);  // ld de, wWestMapConnection
	CALL(mGetMapConnection);  // call GetMapConnection

no_west:
	SET_PC(0x2475U);

	BIT_B(EAST_F);  // bit EAST_F, b
	IF_Z goto no_east;  // jr z, .no_east
	LD_DE(wEastMapConnection);  // ld de, wEastMapConnection
	CALL(mGetMapConnection);  // call GetMapConnection

no_east:
	SET_PC(0x247FU);

	RET;  // ret

}

int GetMapConnection(){
	SET_PC(0x2480U);
//  Load map connection struct at hl into de.
	LD_C(wSouthMapConnection - wNorthMapConnection);  // ld c, wSouthMapConnection - wNorthMapConnection

loop:
	SET_PC(0x2482U);
	LD_A_hli;  // ld a, [hli]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

int ReadMapSceneScripts(){
	SET_PC(0x2489U);
	LD_A_hli;  // ld a, [hli] ; scene_script count
	LD_C_A;  // ld c, a
	LD_addr_A(wCurMapSceneScriptCount);  // ld [wCurMapSceneScriptCount], a
	LD_A_L;  // ld a, l
	LD_addr_A(wCurMapSceneScriptsPointer);  // ld [wCurMapSceneScriptsPointer], a
	LD_A_H;  // ld a, h
	LD_addr_A(wCurMapSceneScriptsPointer + 1);  // ld [wCurMapSceneScriptsPointer + 1], a
	LD_A_C;  // ld a, c
	AND_A_A;  // and a
	RET_Z ;  // ret z

	LD_BC(SCENE_SCRIPT_SIZE);  // ld bc, SCENE_SCRIPT_SIZE
	CALL(mAddNTimes);  // call AddNTimes
	RET;  // ret

}

int ReadMapCallbacks(){
	SET_PC(0x24A0U);
	LD_A_hli;  // ld a, [hli]
	LD_C_A;  // ld c, a
	LD_addr_A(wCurMapCallbackCount);  // ld [wCurMapCallbackCount], a
	LD_A_L;  // ld a, l
	LD_addr_A(wCurMapCallbacksPointer);  // ld [wCurMapCallbacksPointer], a
	LD_A_H;  // ld a, h
	LD_addr_A(wCurMapCallbacksPointer + 1);  // ld [wCurMapCallbacksPointer + 1], a
	LD_A_C;  // ld a, c
	AND_A_A;  // and a
	RET_Z ;  // ret z

	LD_BC(CALLBACK_SIZE);  // ld bc, CALLBACK_SIZE
	CALL(mAddNTimes);  // call AddNTimes
	RET;  // ret

}

int ReadWarps(){
	SET_PC(0x24B7U);
	LD_A_hli;  // ld a, [hli]
	LD_C_A;  // ld c, a
	LD_addr_A(wCurMapWarpCount);  // ld [wCurMapWarpCount], a
	LD_A_L;  // ld a, l
	LD_addr_A(wCurMapWarpsPointer);  // ld [wCurMapWarpsPointer], a
	LD_A_H;  // ld a, h
	LD_addr_A(wCurMapWarpsPointer + 1);  // ld [wCurMapWarpsPointer + 1], a
	LD_A_C;  // ld a, c
	AND_A_A;  // and a
	RET_Z ;  // ret z
	LD_BC(WARP_EVENT_SIZE);  // ld bc, WARP_EVENT_SIZE
	CALL(mAddNTimes);  // call AddNTimes
	RET;  // ret

}

int ReadCoordEvents(){
	SET_PC(0x24CEU);
	LD_A_hli;  // ld a, [hli]
	LD_C_A;  // ld c, a
	LD_addr_A(wCurMapCoordEventCount);  // ld [wCurMapCoordEventCount], a
	LD_A_L;  // ld a, l
	LD_addr_A(wCurMapCoordEventsPointer);  // ld [wCurMapCoordEventsPointer], a
	LD_A_H;  // ld a, h
	LD_addr_A(wCurMapCoordEventsPointer + 1);  // ld [wCurMapCoordEventsPointer + 1], a

	LD_A_C;  // ld a, c
	AND_A_A;  // and a
	RET_Z ;  // ret z

	LD_BC(COORD_EVENT_SIZE);  // ld bc, COORD_EVENT_SIZE
	CALL(mAddNTimes);  // call AddNTimes
	RET;  // ret

}

int ReadBGEvents(){
	SET_PC(0x24E5U);
	LD_A_hli;  // ld a, [hli]
	LD_C_A;  // ld c, a
	LD_addr_A(wCurMapBGEventCount);  // ld [wCurMapBGEventCount], a
	LD_A_L;  // ld a, l
	LD_addr_A(wCurMapBGEventsPointer);  // ld [wCurMapBGEventsPointer], a
	LD_A_H;  // ld a, h
	LD_addr_A(wCurMapBGEventsPointer + 1);  // ld [wCurMapBGEventsPointer + 1], a

	LD_A_C;  // ld a, c
	AND_A_A;  // and a
	RET_Z ;  // ret z

	LD_BC(BG_EVENT_SIZE);  // ld bc, BG_EVENT_SIZE
	CALL(mAddNTimes);  // call AddNTimes
	RET;  // ret

}

int ReadObjectEvents(){
	SET_PC(0x24FCU);
	PUSH_HL;  // push hl
	CALL(mClearObjectStructs);  // call ClearObjectStructs
	POP_DE;  // pop de
	LD_HL(wMap2Object);  // ld hl, wMap2Object
	LD_A_de;  // ld a, [de]
	INC_DE;  // inc de
	LD_addr_A(wCurMapObjectEventCount);  // ld [wCurMapObjectEventCount], a
	LD_A_E;  // ld a, e
	LD_addr_A(wCurMapObjectEventsPointer);  // ld [wCurMapObjectEventsPointer], a
	LD_A_D;  // ld a, d
	LD_addr_A(wCurMapObjectEventsPointer + 1);  // ld [wCurMapObjectEventsPointer + 1], a

	LD_A_addr(wCurMapObjectEventCount);  // ld a, [wCurMapObjectEventCount]
	CALL(mCopyMapObjectEvents);  // call CopyMapObjectEvents

//  get NUM_OBJECTS - [wCurMapObjectEventCount]
	LD_A_addr(wCurMapObjectEventCount);  // ld a, [wCurMapObjectEventCount]
	LD_C_A;  // ld c, a
	LD_A(NUM_OBJECTS);  // ld a, NUM_OBJECTS ; - 1
	SUB_A_C;  // sub c
	IF_Z goto skip;  // jr z, .skip
// ; jr c, .skip

// ; could have done "inc hl" instead
	LD_BC(1);  // ld bc, 1
	ADD_HL_BC;  // add hl, bc
//  Fill the remaining sprite IDs and y coords with 0 and -1, respectively.
//  Bleeds into wObjectMasks due to a bug.  Uncomment the above code to fix.
	LD_BC(MAPOBJECT_LENGTH);  // ld bc, MAPOBJECT_LENGTH

loop:
	SET_PC(0x2527U);
	LD_hl(0);  // ld [hl],  0
	INC_HL;  // inc hl
	LD_hl(-1);  // ld [hl], -1
	DEC_HL;  // dec hl
	ADD_HL_BC;  // add hl, bc
	DEC_A;  // dec a
	IF_NZ goto loop;  // jr nz, .loop


skip:
	SET_PC(0x2531U);
	LD_H_D;  // ld h, d
	LD_L_E;  // ld l, e
	RET;  // ret

}

int CopyMapObjectEvents(){
	SET_PC(0x2534U);
	AND_A_A;  // and a
	RET_Z ;  // ret z

	LD_C_A;  // ld c, a

loop:
	SET_PC(0x2537U);
	PUSH_BC;  // push bc
	PUSH_HL;  // push hl
	LD_A(0xff);  // ld a, $ff
	LD_hli_A;  // ld [hli], a
	LD_B(OBJECT_EVENT_SIZE);  // ld b, OBJECT_EVENT_SIZE

loop2:
	SET_PC(0x253EU);
	LD_A_de;  // ld a, [de]
	INC_DE;  // inc de
	LD_hli_A;  // ld [hli], a
	DEC_B;  // dec b
	IF_NZ goto loop2;  // jr nz, .loop2

	POP_HL;  // pop hl
	LD_BC(MAPOBJECT_LENGTH);  // ld bc, MAPOBJECT_LENGTH
	ADD_HL_BC;  // add hl, bc
	POP_BC;  // pop bc
	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

int ClearObjectStructs(){
	SET_PC(0x254EU);
	LD_HL(wObject1Struct);  // ld hl, wObject1Struct
	LD_BC(OBJECT_LENGTH * (NUM_OBJECT_STRUCTS - 1));  // ld bc, OBJECT_LENGTH * (NUM_OBJECT_STRUCTS - 1)
	XOR_A_A;  // xor a
	CALL(mByteFill);  // call ByteFill

//  Just to make sure (this is rather pointless)
	LD_HL(wObject1Struct);  // ld hl, wObject1Struct
	LD_DE(OBJECT_LENGTH);  // ld de, OBJECT_LENGTH
	LD_C(NUM_OBJECT_STRUCTS - 1);  // ld c, NUM_OBJECT_STRUCTS - 1
	XOR_A_A;  // xor a

loop:
	SET_PC(0x2561U);
	LD_hl_A;  // ld [hl], a
	ADD_HL_DE;  // add hl, de
	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

int GetWarpDestCoords(){
	SET_PC(0x2567U);
	CALL(mGetMapScriptsBank);  // call GetMapScriptsBank
	RST(mBankswitch);  // rst Bankswitch

	LD_HL(wMapEventsPointer);  // ld hl, wMapEventsPointer
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
for(int rept = 0; rept < 3; rept++){  //  get to the warp coords
	INC_HL;  // inc hl
}
	LD_A_addr(wWarpNumber);  // ld a, [wWarpNumber]
	DEC_A;  // dec a
	LD_C_A;  // ld c, a
	LD_B(0);  // ld b, 0
	LD_A(WARP_EVENT_SIZE);  // ld a, WARP_EVENT_SIZE
	CALL(mAddNTimes);  // call AddNTimes
	LD_A_hli;  // ld a, [hli]
	LD_addr_A(wYCoord);  // ld [wYCoord], a
	LD_A_hli;  // ld a, [hli]
	LD_addr_A(wXCoord);  // ld [wXCoord], a
// ; destination warp number
	LD_A_hli;  // ld a, [hli]
	CP_A(-1);  // cp -1
	IF_NZ goto skip;  // jr nz, .skip
	CALL(mGetWarpDestCoords_backup);  // call .backup


skip:
	SET_PC(0x2590U);
	CALL(mGetMapScreenCoords);  // call GetMapScreenCoords
	RET;  // ret


backup:
	SET_PC(0x2594U);
	LD_A_addr(wPrevWarp);  // ld a, [wPrevWarp]
	LD_addr_A(wBackupWarpNumber);  // ld [wBackupWarpNumber], a
	LD_A_addr(wPrevMapGroup);  // ld a, [wPrevMapGroup]
	LD_addr_A(wBackupMapGroup);  // ld [wBackupMapGroup], a
	LD_A_addr(wPrevMapNumber);  // ld a, [wPrevMapNumber]
	LD_addr_A(wBackupMapNumber);  // ld [wBackupMapNumber], a
	RET;  // ret

}

int GetMapScreenCoords(){
	SET_PC(0x25A7U);
	LD_HL(wOverworldMapBlocks);  // ld hl, wOverworldMapBlocks
	LD_A_addr(wXCoord);  // ld a, [wXCoord]
	BIT_A(0);  // bit 0, a
	IF_NZ goto odd_x;  // jr nz, .odd_x
//  even x
	SRL_A;  // srl a
	ADD_A(1);  // add 1
	goto got_block_x;  // jr .got_block_x

odd_x:
	SET_PC(0x25B7U);
	ADD_A(1);  // add 1
	SRL_A;  // srl a

got_block_x:
	SET_PC(0x25BBU);
	LD_C_A;  // ld c, a
	LD_B(0);  // ld b, 0
	ADD_HL_BC;  // add hl, bc
	LD_A_addr(wMapWidth);  // ld a, [wMapWidth]
	ADD_A(MAP_CONNECTION_PADDING_WIDTH * 2);  // add MAP_CONNECTION_PADDING_WIDTH * 2
	LD_C_A;  // ld c, a
	LD_B(0);  // ld b, 0
	LD_A_addr(wYCoord);  // ld a, [wYCoord]
	BIT_A(0);  // bit 0, a
	IF_NZ goto odd_y;  // jr nz, .odd_y
//  even y
	SRL_A;  // srl a
	ADD_A(1);  // add 1
	goto got_block_y;  // jr .got_block_y

odd_y:
	SET_PC(0x25D4U);
	ADD_A(1);  // add 1
	SRL_A;  // srl a

got_block_y:
	SET_PC(0x25D8U);
	CALL(mAddNTimes);  // call AddNTimes
	LD_A_L;  // ld a, l
	LD_addr_A(wOverworldMapAnchor);  // ld [wOverworldMapAnchor], a
	LD_A_H;  // ld a, h
	LD_addr_A(wOverworldMapAnchor + 1);  // ld [wOverworldMapAnchor + 1], a
	LD_A_addr(wYCoord);  // ld a, [wYCoord]
	AND_A(1);  // and 1
	LD_addr_A(wMetatileStandingY);  // ld [wMetatileStandingY], a
	LD_A_addr(wXCoord);  // ld a, [wXCoord]
	AND_A(1);  // and 1
	LD_addr_A(wMetatileStandingX);  // ld [wMetatileStandingX], a
	RET;  // ret

}

int LoadBlockData(){
	SET_PC(0x25F4U);
	LD_HL(wOverworldMapBlocks);  // ld hl, wOverworldMapBlocks
	LD_BC(wOverworldMapBlocksEnd - wOverworldMapBlocks);  // ld bc, wOverworldMapBlocksEnd - wOverworldMapBlocks
	LD_A(0);  // ld a, 0
	CALL(mByteFill);  // call ByteFill
	CALL(mChangeMap);  // call ChangeMap
	CALL(mFillMapConnections);  // call FillMapConnections
	LD_A(MAPCALLBACK_TILES);  // ld a, MAPCALLBACK_TILES
	CALL(mRunMapCallback);  // call RunMapCallback
	RET;  // ret

}

int ChangeMap(){
	SET_PC(0x260BU);
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af

	LD_HL(wOverworldMapBlocks);  // ld hl, wOverworldMapBlocks
	LD_A_addr(wMapWidth);  // ld a, [wMapWidth]
	LDH_addr_A(hConnectedMapWidth);  // ldh [hConnectedMapWidth], a
	ADD_A(0x6);  // add $6
	LDH_addr_A(hConnectionStripLength);  // ldh [hConnectionStripLength], a
	LD_C_A;  // ld c, a
	LD_B(0);  // ld b, 0
	ADD_HL_BC;  // add hl, bc
	ADD_HL_BC;  // add hl, bc
	ADD_HL_BC;  // add hl, bc
	LD_C(3);  // ld c, 3
	ADD_HL_BC;  // add hl, bc
	LD_A_addr(wMapBlocksBank);  // ld a, [wMapBlocksBank]
	RST(mBankswitch);  // rst Bankswitch

	LD_A_addr(wMapBlocksPointer);  // ld a, [wMapBlocksPointer]
	LD_E_A;  // ld e, a
	LD_A_addr(wMapBlocksPointer + 1);  // ld a, [wMapBlocksPointer + 1]
	LD_D_A;  // ld d, a
	LD_A_addr(wMapHeight);  // ld a, [wMapHeight]
	LD_B_A;  // ld b, a

row:
	SET_PC(0x2633U);
	PUSH_HL;  // push hl
	LDH_A_addr(hConnectedMapWidth);  // ldh a, [hConnectedMapWidth]
	LD_C_A;  // ld c, a

col:
	SET_PC(0x2637U);
	LD_A_de;  // ld a, [de]
	INC_DE;  // inc de
	LD_hli_A;  // ld [hli], a
	DEC_C;  // dec c
	IF_NZ goto col;  // jr nz, .col
	POP_HL;  // pop hl
	LDH_A_addr(hConnectionStripLength);  // ldh a, [hConnectionStripLength]
	ADD_A_L;  // add l
	LD_L_A;  // ld l, a
	IF_NC goto okay;  // jr nc, .okay
	INC_H;  // inc h

okay:
	SET_PC(0x2645U);
	DEC_B;  // dec b
	IF_NZ goto row;  // jr nz, .row

	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret

}

int FillMapConnections(){
	SET_PC(0x264BU);
//  North
	LD_A_addr(wNorthConnectedMapGroup);  // ld a, [wNorthConnectedMapGroup]
	CP_A(0xff);  // cp $ff
	IF_Z goto South;  // jr z, .South
	LD_B_A;  // ld b, a
	LD_A_addr(wNorthConnectedMapNumber);  // ld a, [wNorthConnectedMapNumber]
	LD_C_A;  // ld c, a
	CALL(mGetAnyMapBlocksBank);  // call GetAnyMapBlocksBank

	LD_A_addr(wNorthConnectionStripPointer);  // ld a, [wNorthConnectionStripPointer]
	LD_L_A;  // ld l, a
	LD_A_addr(wNorthConnectionStripPointer + 1);  // ld a, [wNorthConnectionStripPointer + 1]
	LD_H_A;  // ld h, a
	LD_A_addr(wNorthConnectionStripLocation);  // ld a, [wNorthConnectionStripLocation]
	LD_E_A;  // ld e, a
	LD_A_addr(wNorthConnectionStripLocation + 1);  // ld a, [wNorthConnectionStripLocation + 1]
	LD_D_A;  // ld d, a
	LD_A_addr(wNorthConnectionStripLength);  // ld a, [wNorthConnectionStripLength]
	LDH_addr_A(hConnectionStripLength);  // ldh [hConnectionStripLength], a
	LD_A_addr(wNorthConnectedMapWidth);  // ld a, [wNorthConnectedMapWidth]
	LDH_addr_A(hConnectedMapWidth);  // ldh [hConnectedMapWidth], a
	CALL(mFillNorthConnectionStrip);  // call FillNorthConnectionStrip


South:
	SET_PC(0x2677U);
	LD_A_addr(wSouthConnectedMapGroup);  // ld a, [wSouthConnectedMapGroup]
	CP_A(0xff);  // cp $ff
	IF_Z goto West;  // jr z, .West
	LD_B_A;  // ld b, a
	LD_A_addr(wSouthConnectedMapNumber);  // ld a, [wSouthConnectedMapNumber]
	LD_C_A;  // ld c, a
	CALL(mGetAnyMapBlocksBank);  // call GetAnyMapBlocksBank

	LD_A_addr(wSouthConnectionStripPointer);  // ld a, [wSouthConnectionStripPointer]
	LD_L_A;  // ld l, a
	LD_A_addr(wSouthConnectionStripPointer + 1);  // ld a, [wSouthConnectionStripPointer + 1]
	LD_H_A;  // ld h, a
	LD_A_addr(wSouthConnectionStripLocation);  // ld a, [wSouthConnectionStripLocation]
	LD_E_A;  // ld e, a
	LD_A_addr(wSouthConnectionStripLocation + 1);  // ld a, [wSouthConnectionStripLocation + 1]
	LD_D_A;  // ld d, a
	LD_A_addr(wSouthConnectionStripLength);  // ld a, [wSouthConnectionStripLength]
	LDH_addr_A(hConnectionStripLength);  // ldh [hConnectionStripLength], a
	LD_A_addr(wSouthConnectedMapWidth);  // ld a, [wSouthConnectedMapWidth]
	LDH_addr_A(hConnectedMapWidth);  // ldh [hConnectedMapWidth], a
	CALL(mFillSouthConnectionStrip);  // call FillSouthConnectionStrip


West:
	SET_PC(0x26A3U);
	LD_A_addr(wWestConnectedMapGroup);  // ld a, [wWestConnectedMapGroup]
	CP_A(0xff);  // cp $ff
	IF_Z goto East;  // jr z, .East
	LD_B_A;  // ld b, a
	LD_A_addr(wWestConnectedMapNumber);  // ld a, [wWestConnectedMapNumber]
	LD_C_A;  // ld c, a
	CALL(mGetAnyMapBlocksBank);  // call GetAnyMapBlocksBank

	LD_A_addr(wWestConnectionStripPointer);  // ld a, [wWestConnectionStripPointer]
	LD_L_A;  // ld l, a
	LD_A_addr(wWestConnectionStripPointer + 1);  // ld a, [wWestConnectionStripPointer + 1]
	LD_H_A;  // ld h, a
	LD_A_addr(wWestConnectionStripLocation);  // ld a, [wWestConnectionStripLocation]
	LD_E_A;  // ld e, a
	LD_A_addr(wWestConnectionStripLocation + 1);  // ld a, [wWestConnectionStripLocation + 1]
	LD_D_A;  // ld d, a
	LD_A_addr(wWestConnectionStripLength);  // ld a, [wWestConnectionStripLength]
	LD_B_A;  // ld b, a
	LD_A_addr(wWestConnectedMapWidth);  // ld a, [wWestConnectedMapWidth]
	LDH_addr_A(hConnectionStripLength);  // ldh [hConnectionStripLength], a
	CALL(mFillWestConnectionStrip);  // call FillWestConnectionStrip


East:
	SET_PC(0x26CEU);
	LD_A_addr(wEastConnectedMapGroup);  // ld a, [wEastConnectedMapGroup]
	CP_A(0xff);  // cp $ff
	IF_Z goto Done;  // jr z, .Done
	LD_B_A;  // ld b, a
	LD_A_addr(wEastConnectedMapNumber);  // ld a, [wEastConnectedMapNumber]
	LD_C_A;  // ld c, a
	CALL(mGetAnyMapBlocksBank);  // call GetAnyMapBlocksBank

	LD_A_addr(wEastConnectionStripPointer);  // ld a, [wEastConnectionStripPointer]
	LD_L_A;  // ld l, a
	LD_A_addr(wEastConnectionStripPointer + 1);  // ld a, [wEastConnectionStripPointer + 1]
	LD_H_A;  // ld h, a
	LD_A_addr(wEastConnectionStripLocation);  // ld a, [wEastConnectionStripLocation]
	LD_E_A;  // ld e, a
	LD_A_addr(wEastConnectionStripLocation + 1);  // ld a, [wEastConnectionStripLocation + 1]
	LD_D_A;  // ld d, a
	LD_A_addr(wEastConnectionStripLength);  // ld a, [wEastConnectionStripLength]
	LD_B_A;  // ld b, a
	LD_A_addr(wEastConnectedMapWidth);  // ld a, [wEastConnectedMapWidth]
	LDH_addr_A(hConnectionStripLength);  // ldh [hConnectionStripLength], a
	CALL(mFillEastConnectionStrip);  // call FillEastConnectionStrip


Done:
	SET_PC(0x26F9U);
	RET;  // ret

}

int FillNorthConnectionStrip(){
	SET_PC(0x26FAU);
	return FillSouthConnectionStrip();
}

int FillSouthConnectionStrip(){
	SET_PC(0x26FAU);
	LD_C(3);  // ld c, 3

y:
	SET_PC(0x26FCU);
	PUSH_DE;  // push de

	PUSH_HL;  // push hl
	LDH_A_addr(hConnectionStripLength);  // ldh a, [hConnectionStripLength]
	LD_B_A;  // ld b, a

x:
	SET_PC(0x2701U);
	LD_A_hli;  // ld a, [hli]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
	DEC_B;  // dec b
	IF_NZ goto x;  // jr nz, .x
	POP_HL;  // pop hl

	LDH_A_addr(hConnectedMapWidth);  // ldh a, [hConnectedMapWidth]
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
	ADD_HL_DE;  // add hl, de
	POP_DE;  // pop de

	LD_A_addr(wMapWidth);  // ld a, [wMapWidth]
	ADD_A(6);  // add 6
	ADD_A_E;  // add e
	LD_E_A;  // ld e, a
	IF_NC goto okay;  // jr nc, .okay
	INC_D;  // inc d

okay:
	SET_PC(0x2719U);
	DEC_C;  // dec c
	IF_NZ goto y;  // jr nz, .y
	RET;  // ret

}

int FillWestConnectionStrip(){
	SET_PC(0x271DU);
	return FillEastConnectionStrip();
}

int FillEastConnectionStrip(){
	SET_PC(0x271DU);

loop:
	SET_PC(0x271DU);
	LD_A_addr(wMapWidth);  // ld a, [wMapWidth]
	ADD_A(6);  // add 6
	LDH_addr_A(hConnectedMapWidth);  // ldh [hConnectedMapWidth], a

	PUSH_DE;  // push de

	PUSH_HL;  // push hl
	LD_A_hli;  // ld a, [hli]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
	LD_A_hli;  // ld a, [hli]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
	LD_A_hli;  // ld a, [hli]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
	POP_HL;  // pop hl

	LDH_A_addr(hConnectionStripLength);  // ldh a, [hConnectionStripLength]
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
	ADD_HL_DE;  // add hl, de
	POP_DE;  // pop de

	LDH_A_addr(hConnectedMapWidth);  // ldh a, [hConnectedMapWidth]
	ADD_A_E;  // add e
	LD_E_A;  // ld e, a
	IF_NC goto okay;  // jr nc, .okay
	INC_D;  // inc d

okay:
	SET_PC(0x273EU);
	DEC_B;  // dec b
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

int LoadMapStatus(){
	SET_PC(0x2742U);
	LD_addr_A(wMapStatus);  // ld [wMapStatus], a
	RET;  // ret

}

int CallScript(){
	SET_PC(0x2746U);
//  Call a script at a:hl.

	LD_addr_A(wScriptBank);  // ld [wScriptBank], a
	LD_A_L;  // ld a, l
	LD_addr_A(wScriptPos);  // ld [wScriptPos], a
	LD_A_H;  // ld a, h
	LD_addr_A(wScriptPos + 1);  // ld [wScriptPos + 1], a

	LD_A(PLAYEREVENT_MAPSCRIPT);  // ld a, PLAYEREVENT_MAPSCRIPT
	LD_addr_A(wScriptRunning);  // ld [wScriptRunning], a

	SCF;  // scf
	RET;  // ret

}

int CallMapScript(){
	SET_PC(0x2758U);
//  Call a script at hl in the current bank if there isn't already a script running
	LD_A_addr(wScriptRunning);  // ld a, [wScriptRunning]
	AND_A_A;  // and a
	RET_NZ ;  // ret nz
	CALL(mGetMapScriptsBank);  // call GetMapScriptsBank
	JR(mCallScript);  // jr CallScript

}

int RunMapCallback(){
	SET_PC(0x2762U);
//  Will run the first callback found with execution index equal to a.
	LD_B_A;  // ld b, a
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	CALL(mSwitchToMapScriptsBank);  // call SwitchToMapScriptsBank
	CALL(mRunMapCallback_FindCallback);  // call .FindCallback
	IF_NC goto done;  // jr nc, .done

	CALL(mGetMapScriptsBank);  // call GetMapScriptsBank
	LD_B_A;  // ld b, a
	LD_D_H;  // ld d, h
	LD_E_L;  // ld e, l
	CALL(mExecuteCallbackScript);  // call ExecuteCallbackScript


done:
	SET_PC(0x2777U);
	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret


FindCallback:
	SET_PC(0x277AU);
	LD_A_addr(wCurMapCallbackCount);  // ld a, [wCurMapCallbackCount]
	LD_C_A;  // ld c, a
	AND_A_A;  // and a
	RET_Z ;  // ret z
	LD_HL(wCurMapCallbacksPointer);  // ld hl, wCurMapCallbacksPointer
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	OR_A_H;  // or h
	RET_Z ;  // ret z
	LD_DE(CALLBACK_SIZE);  // ld de, CALLBACK_SIZE

loop:
	SET_PC(0x278BU);
	LD_A_hl;  // ld a, [hl]
	CP_A_B;  // cp b
	IF_Z goto found;  // jr z, .found
	ADD_HL_DE;  // add hl, de
	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop
	XOR_A_A;  // xor a
	RET;  // ret


found:
	SET_PC(0x2795U);
	INC_HL;  // inc hl
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	SCF;  // scf
	RET;  // ret

}

int ExecuteCallbackScript(){
	SET_PC(0x279BU);
//  Do map callback de and return to script bank b.
	FARCALL(aCallCallback);  // farcall CallCallback
	LD_A_addr(wScriptMode);  // ld a, [wScriptMode]
	PUSH_AF;  // push af
	LD_HL(wScriptFlags);  // ld hl, wScriptFlags
	LD_A_hl;  // ld a, [hl]
	PUSH_AF;  // push af
	SET_hl(1);  // set 1, [hl]
	FARCALL(aEnableScriptMode);  // farcall EnableScriptMode
	FARCALL(aScriptEvents);  // farcall ScriptEvents
	POP_AF;  // pop af
	LD_addr_A(wScriptFlags);  // ld [wScriptFlags], a
	POP_AF;  // pop af
	LD_addr_A(wScriptMode);  // ld [wScriptMode], a
	RET;  // ret

}

int MapTextbox(){
	SET_PC(0x27C1U);
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af

	LD_A_B;  // ld a, b
	RST(mBankswitch);  // rst Bankswitch

	CALL(mSetUpTextbox);  // call SetUpTextbox
	LD_A(1);  // ld a, 1
	LDH_addr_A(hOAMUpdate);  // ldh [hOAMUpdate], a
	CALL(mPrintTextboxText);  // call PrintTextboxText
	XOR_A_A;  // xor a
	LDH_addr_A(hOAMUpdate);  // ldh [hOAMUpdate], a

	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret

}

int Call_a_de(){
	SET_PC(0x27D6U);
//  Call a:de.

	LD_addr_A(wTempBank);  // ld [wTempBank], a
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A_addr(wTempBank);  // ld a, [wTempBank]
	RST(mBankswitch);  // rst Bankswitch

	CALL(mCall_a_de_de);  // call .de

	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret


de:
	SET_PC(0x27E6U);
	PUSH_DE;  // push de
	RET;  // ret

}

int GetMovementData(){
	SET_PC(0x27E8U);
//  Initialize the movement data for object c at b:hl
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A_B;  // ld a, b
	RST(mBankswitch);  // rst Bankswitch

	LD_A_C;  // ld a, c
	CALL(mLoadMovementDataPointer);  // call LoadMovementDataPointer

	POP_HL;  // pop hl
	LD_A_H;  // ld a, h
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret

}

int GetScriptByte(){
	SET_PC(0x27F5U);
//  Return byte at wScriptBank:wScriptPos in a.

	PUSH_HL;  // push hl
	PUSH_BC;  // push bc
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A_addr(wScriptBank);  // ld a, [wScriptBank]
	RST(mBankswitch);  // rst Bankswitch

	LD_HL(wScriptPos);  // ld hl, wScriptPos
	LD_C_hl;  // ld c, [hl]
	INC_HL;  // inc hl
	LD_B_hl;  // ld b, [hl]

	LD_A_bc;  // ld a, [bc]

	INC_BC;  // inc bc
	LD_hl_B;  // ld [hl], b
	DEC_HL;  // dec hl
	LD_hl_C;  // ld [hl], c

	LD_B_A;  // ld b, a
	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch
	LD_A_B;  // ld a, b
	POP_BC;  // pop bc
	POP_HL;  // pop hl
	RET;  // ret

}

int ObjectEvent(){
	SET_PC(0x2810U);
	//jumptextfaceplayer ['ObjectEventText']  // jumptextfaceplayer ObjectEventText

	return ObjectEventText();
}

int ObjectEventText(){
	SET_PC(0x2813U);
	//text_far ['_ObjectEventText']  // text_far _ObjectEventText
	//text_end ['?']  // text_end

	return BGEvent();
}

int BGEvent(){
	SET_PC(0x2818U);
//  //  unreferenced
	//jumptext ['BGEventText']  // jumptext BGEventText

	return BGEventText();
}

int BGEventText(){
	SET_PC(0x281BU);
	//text_far ['_BGEventText']  // text_far _BGEventText
	//text_end ['?']  // text_end

	return CoordinatesEvent();
}

int CoordinatesEvent(){
	SET_PC(0x2820U);
//  //  unreferenced
	//jumptext ['CoordinatesEventText']  // jumptext CoordinatesEventText

	return CoordinatesEventText();
}

int CoordinatesEventText(){
	SET_PC(0x2823U);
	//text_far ['_CoordinatesEventText']  // text_far _CoordinatesEventText
	//text_end ['?']  // text_end

	return CheckObjectMask();
}

int CheckObjectMask(){
	SET_PC(0x2828U);
	LDH_A_addr(hMapObjectIndex);  // ldh a, [hMapObjectIndex]
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
	LD_HL(wObjectMasks);  // ld hl, wObjectMasks
	ADD_HL_DE;  // add hl, de
	LD_A_hl;  // ld a, [hl]
	RET;  // ret

}

int MaskObject(){
	SET_PC(0x2833U);
	LDH_A_addr(hMapObjectIndex);  // ldh a, [hMapObjectIndex]
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
	LD_HL(wObjectMasks);  // ld hl, wObjectMasks
	ADD_HL_DE;  // add hl, de
	LD_hl(-1);  // ld [hl], -1 ; masked
	RET;  // ret

}

int UnmaskObject(){
	SET_PC(0x283FU);
	LDH_A_addr(hMapObjectIndex);  // ldh a, [hMapObjectIndex]
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
	LD_HL(wObjectMasks);  // ld hl, wObjectMasks
	ADD_HL_DE;  // add hl, de
	LD_hl(0);  // ld [hl], 0 ; unmasked
	RET;  // ret

}

int ScrollMapUp(){
	SET_PC(0x284BU);
	hlcoord(0, 0, wTilemap);  // hlcoord 0, 0
	LD_DE(wBGMapBuffer);  // ld de, wBGMapBuffer
	CALL(mBackupBGMapRow);  // call BackupBGMapRow
	LD_C(2 * SCREEN_WIDTH);  // ld c, 2 * SCREEN_WIDTH
	CALL(mScrollBGMapPalettes);  // call ScrollBGMapPalettes
	LD_A_addr(wBGMapAnchor);  // ld a, [wBGMapAnchor]
	LD_E_A;  // ld e, a
	LD_A_addr(wBGMapAnchor + 1);  // ld a, [wBGMapAnchor + 1]
	LD_D_A;  // ld d, a
	CALL(mUpdateBGMapRow);  // call UpdateBGMapRow
	LD_A(0x1);  // ld a, $1
	LDH_addr_A(hBGMapUpdate);  // ldh [hBGMapUpdate], a
	RET;  // ret

}

int ScrollMapDown(){
	SET_PC(0x2869U);
	hlcoord(0, SCREEN_HEIGHT - 2, wTilemap);  // hlcoord 0, SCREEN_HEIGHT - 2
	LD_DE(wBGMapBuffer);  // ld de, wBGMapBuffer
	CALL(mBackupBGMapRow);  // call BackupBGMapRow
	LD_C(2 * SCREEN_WIDTH);  // ld c, 2 * SCREEN_WIDTH
	CALL(mScrollBGMapPalettes);  // call ScrollBGMapPalettes
	LD_A_addr(wBGMapAnchor);  // ld a, [wBGMapAnchor]
	LD_L_A;  // ld l, a
	LD_A_addr(wBGMapAnchor + 1);  // ld a, [wBGMapAnchor + 1]
	LD_H_A;  // ld h, a
	LD_BC(BG_MAP_WIDTH * LEN_2BPP_TILE);  // ld bc, BG_MAP_WIDTH tiles
	ADD_HL_BC;  // add hl, bc
//  cap d at HIGH(vBGMap0)
	LD_A_H;  // ld a, h
	AND_A(0b00000011);  // and %00000011
	OR_A(HIGH(vBGMap0));  // or HIGH(vBGMap0)
	LD_E_L;  // ld e, l
	LD_D_A;  // ld d, a
	CALL(mUpdateBGMapRow);  // call UpdateBGMapRow
	LD_A(0x1);  // ld a, $1
	LDH_addr_A(hBGMapUpdate);  // ldh [hBGMapUpdate], a
	RET;  // ret

}

int ScrollMapLeft(){
	SET_PC(0x2892U);
	hlcoord(0, 0, wTilemap);  // hlcoord 0, 0
	LD_DE(wBGMapBuffer);  // ld de, wBGMapBuffer
	CALL(mBackupBGMapColumn);  // call BackupBGMapColumn
	LD_C(2 * SCREEN_HEIGHT);  // ld c, 2 * SCREEN_HEIGHT
	CALL(mScrollBGMapPalettes);  // call ScrollBGMapPalettes
	LD_A_addr(wBGMapAnchor);  // ld a, [wBGMapAnchor]
	LD_E_A;  // ld e, a
	LD_A_addr(wBGMapAnchor + 1);  // ld a, [wBGMapAnchor + 1]
	LD_D_A;  // ld d, a
	CALL(mUpdateBGMapColumn);  // call UpdateBGMapColumn
	LD_A(0x1);  // ld a, $1
	LDH_addr_A(hBGMapUpdate);  // ldh [hBGMapUpdate], a
	RET;  // ret

}

int ScrollMapRight(){
	SET_PC(0x28B0U);
	hlcoord(SCREEN_WIDTH - 2, 0, wTilemap);  // hlcoord SCREEN_WIDTH - 2, 0
	LD_DE(wBGMapBuffer);  // ld de, wBGMapBuffer
	CALL(mBackupBGMapColumn);  // call BackupBGMapColumn
	LD_C(2 * SCREEN_HEIGHT);  // ld c, 2 * SCREEN_HEIGHT
	CALL(mScrollBGMapPalettes);  // call ScrollBGMapPalettes
	LD_A_addr(wBGMapAnchor);  // ld a, [wBGMapAnchor]
	LD_E_A;  // ld e, a
	AND_A(0b11100000);  // and %11100000
	LD_B_A;  // ld b, a
	LD_A_E;  // ld a, e
	ADD_A(SCREEN_HEIGHT);  // add SCREEN_HEIGHT
	AND_A(0b00011111);  // and %00011111
	OR_A_B;  // or b
	LD_E_A;  // ld e, a
	LD_A_addr(wBGMapAnchor + 1);  // ld a, [wBGMapAnchor + 1]
	LD_D_A;  // ld d, a
	CALL(mUpdateBGMapColumn);  // call UpdateBGMapColumn
	LD_A(0x1);  // ld a, $1
	LDH_addr_A(hBGMapUpdate);  // ldh [hBGMapUpdate], a
	RET;  // ret

}

int BackupBGMapRow(){
	SET_PC(0x28D8U);
	LD_C(2 * SCREEN_WIDTH);  // ld c, 2 * SCREEN_WIDTH

loop:
	SET_PC(0x28DAU);
	LD_A_hli;  // ld a, [hli]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

int BackupBGMapColumn(){
	SET_PC(0x28E1U);
	LD_C(SCREEN_HEIGHT);  // ld c, SCREEN_HEIGHT

loop:
	SET_PC(0x28E3U);
	LD_A_hli;  // ld a, [hli]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
	LD_A_hl;  // ld a, [hl]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
	LD_A(SCREEN_WIDTH - 1);  // ld a, SCREEN_WIDTH - 1
	ADD_A_L;  // add l
	LD_L_A;  // ld l, a
	IF_NC goto skip;  // jr nc, .skip
	INC_H;  // inc h


skip:
	SET_PC(0x28F0U);
	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

int UpdateBGMapRow(){
	SET_PC(0x28F4U);
	LD_HL(wBGMapBufferPointers);  // ld hl, wBGMapBufferPointers
	PUSH_DE;  // push de
	CALL(mUpdateBGMapRow_iteration);  // call .iteration
	POP_DE;  // pop de
	LD_A(BG_MAP_WIDTH);  // ld a, BG_MAP_WIDTH
	ADD_A_E;  // add e
	LD_E_A;  // ld e, a


iteration:
	SET_PC(0x2900U);
	LD_C(10);  // ld c, 10

loop:
	SET_PC(0x2902U);
	LD_A_E;  // ld a, e
	LD_hli_A;  // ld [hli], a
	LD_A_D;  // ld a, d
	LD_hli_A;  // ld [hli], a
	LD_A_E;  // ld a, e
	INC_A;  // inc a
	INC_A;  // inc a
	AND_A(0x1f);  // and $1f
	LD_B_A;  // ld b, a
	LD_A_E;  // ld a, e
	AND_A(0xe0);  // and $e0
	OR_A_B;  // or b
	LD_E_A;  // ld e, a
	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop
	LD_A(SCREEN_WIDTH);  // ld a, SCREEN_WIDTH
	LDH_addr_A(hBGMapTileCount);  // ldh [hBGMapTileCount], a
	RET;  // ret

}

int UpdateBGMapColumn(){
	SET_PC(0x2919U);
	LD_HL(wBGMapBufferPointers);  // ld hl, wBGMapBufferPointers
	LD_C(SCREEN_HEIGHT);  // ld c, SCREEN_HEIGHT

loop:
	SET_PC(0x291EU);
	LD_A_E;  // ld a, e
	LD_hli_A;  // ld [hli], a
	LD_A_D;  // ld a, d
	LD_hli_A;  // ld [hli], a
	LD_A(BG_MAP_WIDTH);  // ld a, BG_MAP_WIDTH
	ADD_A_E;  // add e
	LD_E_A;  // ld e, a
	IF_NC goto skip;  // jr nc, .skip
	INC_D;  // inc d
//  cap d at HIGH(vBGMap0)
	LD_A_D;  // ld a, d
	AND_A(0b11);  // and %11
	OR_A(HIGH(vBGMap0));  // or HIGH(vBGMap0)
	LD_D_A;  // ld d, a


skip:
	SET_PC(0x292FU);
	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop
	LD_A(SCREEN_HEIGHT);  // ld a, SCREEN_HEIGHT
	LDH_addr_A(hBGMapTileCount);  // ldh [hBGMapTileCount], a
	RET;  // ret

}

int ClearBGMapBuffer(){
	SET_PC(0x2937U);
//  //  unreferenced
	LD_HL(wBGMapBuffer);  // ld hl, wBGMapBuffer
	LD_BC(wBGMapBufferEnd - wBGMapBuffer);  // ld bc, wBGMapBufferEnd - wBGMapBuffer
	XOR_A_A;  // xor a
	CALL(mByteFill);  // call ByteFill
	RET;  // ret

}

int LoadTilesetGFX(){
	SET_PC(0x2942U);
	LD_HL(wTilesetAddress);  // ld hl, wTilesetAddress
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	LD_DE(vTiles2);  // ld de, vTiles2
	LD_A_addr(wTilesetBank);  // ld a, [wTilesetBank]
	CALL(mFarDecompress);  // call FarDecompress

//  These tilesets support dynamic per-mapgroup roof tiles.
	LD_A_addr(wMapTileset);  // ld a, [wMapTileset]
	CP_A(TILESET_JOHTO);  // cp TILESET_JOHTO
	IF_Z goto load_roof;  // jr z, .load_roof
	CP_A(TILESET_JOHTO_MODERN);  // cp TILESET_JOHTO_MODERN
	IF_Z goto load_roof;  // jr z, .load_roof
	goto skip_roof;  // jr .skip_roof


load_roof:
	SET_PC(0x295EU);
	FARCALL(aLoadMapGroupRoof);  // farcall LoadMapGroupRoof


skip_roof:
	SET_PC(0x2964U);
	XOR_A_A;  // xor a
	LDH_addr_A(hTileAnimFrame);  // ldh [hTileAnimFrame], a
	RET;  // ret

}

int BufferScreen(){
	SET_PC(0x2968U);
	LD_HL(wOverworldMapAnchor);  // ld hl, wOverworldMapAnchor
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	LD_DE(wScreenSave);  // ld de, wScreenSave
	LD_C(SCREEN_META_HEIGHT);  // ld c, SCREEN_META_HEIGHT
	LD_B(SCREEN_META_WIDTH);  // ld b, SCREEN_META_WIDTH

row:
	SET_PC(0x2975U);
	PUSH_BC;  // push bc
	PUSH_HL;  // push hl

col:
	SET_PC(0x2977U);
	LD_A_hli;  // ld a, [hli]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
	DEC_B;  // dec b
	IF_NZ goto col;  // jr nz, .col
	POP_HL;  // pop hl
	LD_A_addr(wMapWidth);  // ld a, [wMapWidth]
	ADD_A(6);  // add 6
	LD_C_A;  // ld c, a
	LD_B(0);  // ld b, 0
	ADD_HL_BC;  // add hl, bc
	POP_BC;  // pop bc
	DEC_C;  // dec c
	IF_NZ goto row;  // jr nz, .row
	RET;  // ret

}

int SaveScreen(){
	SET_PC(0x298CU);
	LD_HL(wOverworldMapAnchor);  // ld hl, wOverworldMapAnchor
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	LD_DE(wScreenSave);  // ld de, wScreenSave
	LD_A_addr(wMapWidth);  // ld a, [wMapWidth]
	ADD_A(6);  // add 6
	LDH_addr_A(hMapObjectIndex);  // ldh [hMapObjectIndex], a
	LD_A_addr(wPlayerStepDirection);  // ld a, [wPlayerStepDirection]
	AND_A_A;  // and a
	IF_Z goto down;  // jr z, .down
	CP_A(UP);  // cp UP
	IF_Z goto up;  // jr z, .up
	CP_A(LEFT);  // cp LEFT
	IF_Z goto left;  // jr z, .left
	CP_A(RIGHT);  // cp RIGHT
	IF_Z goto right;  // jr z, .right
	RET;  // ret


up:
	SET_PC(0x29AFU);
	LD_DE(wScreenSave + SCREEN_META_WIDTH);  // ld de, wScreenSave + SCREEN_META_WIDTH
	LDH_A_addr(hMapObjectIndex);  // ldh a, [hMapObjectIndex]
	LD_C_A;  // ld c, a
	LD_B(0);  // ld b, 0
	ADD_HL_BC;  // add hl, bc
	goto vertical;  // jr .vertical


down:
	SET_PC(0x29BAU);
	LD_DE(wScreenSave);  // ld de, wScreenSave

vertical:
	SET_PC(0x29BDU);
	LD_B(SCREEN_META_WIDTH);  // ld b, SCREEN_META_WIDTH
	LD_C(SCREEN_META_HEIGHT - 1);  // ld c, SCREEN_META_HEIGHT - 1
	JR(mSaveScreen_LoadConnection);  // jr SaveScreen_LoadConnection


left:
	SET_PC(0x29C3U);
	LD_DE(wScreenSave + 1);  // ld de, wScreenSave + 1
	INC_HL;  // inc hl
	goto horizontal;  // jr .horizontal


right:
	SET_PC(0x29C9U);
	LD_DE(wScreenSave);  // ld de, wScreenSave

horizontal:
	SET_PC(0x29CCU);
	LD_B(SCREEN_META_WIDTH - 1);  // ld b, SCREEN_META_WIDTH - 1
	LD_C(SCREEN_META_HEIGHT);  // ld c, SCREEN_META_HEIGHT
	JR(mSaveScreen_LoadConnection);  // jr SaveScreen_LoadConnection

}

int LoadConnectionBlockData(){
	SET_PC(0x29D2U);
	LD_HL(wOverworldMapAnchor);  // ld hl, wOverworldMapAnchor
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	LD_A_addr(wMapWidth);  // ld a, [wMapWidth]
	ADD_A(6);  // add 6
	LDH_addr_A(hConnectionStripLength);  // ldh [hConnectionStripLength], a
	LD_DE(wScreenSave);  // ld de, wScreenSave
	LD_B(SCREEN_META_WIDTH);  // ld b, SCREEN_META_WIDTH
	LD_C(SCREEN_META_HEIGHT);  // ld c, SCREEN_META_HEIGHT

	return SaveScreen_LoadConnection();
}

int SaveScreen_LoadConnection(){
	SET_PC(0x29E6U);

row:
	SET_PC(0x29E6U);
	PUSH_BC;  // push bc
	PUSH_HL;  // push hl
	PUSH_DE;  // push de

col:
	SET_PC(0x29E9U);
	LD_A_de;  // ld a, [de]
	INC_DE;  // inc de
	LD_hli_A;  // ld [hli], a
	DEC_B;  // dec b
	IF_NZ goto col;  // jr nz, .col
	POP_DE;  // pop de
	LD_A_E;  // ld a, e
	ADD_A(6);  // add 6
	LD_E_A;  // ld e, a
	IF_NC goto okay;  // jr nc, .okay
	INC_D;  // inc d

okay:
	SET_PC(0x29F7U);
	POP_HL;  // pop hl
	LDH_A_addr(hConnectionStripLength);  // ldh a, [hConnectionStripLength]
	LD_C_A;  // ld c, a
	LD_B(0);  // ld b, 0
	ADD_HL_BC;  // add hl, bc
	POP_BC;  // pop bc
	DEC_C;  // dec c
	IF_NZ goto row;  // jr nz, .row
	RET;  // ret

}

int GetMovementPermissions(){
	SET_PC(0x2A03U);
	XOR_A_A;  // xor a
	LD_addr_A(wTilePermissions);  // ld [wTilePermissions], a
	CALL(mGetMovementPermissions_LeftRight);  // call .LeftRight
	CALL(mGetMovementPermissions_UpDown);  // call .UpDown
//  get coords of current tile
	LD_A_addr(wPlayerStandingMapX);  // ld a, [wPlayerStandingMapX]
	LD_D_A;  // ld d, a
	LD_A_addr(wPlayerStandingMapY);  // ld a, [wPlayerStandingMapY]
	LD_E_A;  // ld e, a
	CALL(mGetCoordTile);  // call GetCoordTile
	LD_addr_A(wPlayerStandingTile);  // ld [wPlayerStandingTile], a
	CALL(mGetMovementPermissions_CheckHiNybble);  // call .CheckHiNybble
	RET_NZ ;  // ret nz

	LD_A_addr(wPlayerStandingTile);  // ld a, [wPlayerStandingTile]
	AND_A(7);  // and 7
	LD_HL(mGetMovementPermissions_MovementPermissionsData);  // ld hl, .MovementPermissionsData
	ADD_A_L;  // add l
	LD_L_A;  // ld l, a
	LD_A(0);  // ld a, 0
	ADC_A_H;  // adc h
	LD_H_A;  // ld h, a
	LD_A_hl;  // ld a, [hl]
	LD_HL(wTilePermissions);  // ld hl, wTilePermissions
	OR_A_hl;  // or [hl]
	LD_hl_A;  // ld [hl], a
	RET;  // ret


MovementPermissionsData:
	SET_PC(0x2A34U);
	//db ['DOWN_MASK'];  // db DOWN_MASK
	//db ['UP_MASK'];  // db UP_MASK
	//db ['LEFT_MASK'];  // db LEFT_MASK
	//db ['RIGHT_MASK'];  // db RIGHT_MASK
	//db ['DOWN_MASK | RIGHT_MASK'];  // db DOWN_MASK | RIGHT_MASK
	//db ['UP_MASK | RIGHT_MASK'];  // db UP_MASK | RIGHT_MASK
	//db ['DOWN_MASK | LEFT_MASK'];  // db DOWN_MASK | LEFT_MASK
	//db ['UP_MASK | LEFT_MASK'];  // db UP_MASK | LEFT_MASK


UpDown:
	SET_PC(0x2A3CU);
	LD_A_addr(wPlayerStandingMapX);  // ld a, [wPlayerStandingMapX]
	LD_D_A;  // ld d, a
	LD_A_addr(wPlayerStandingMapY);  // ld a, [wPlayerStandingMapY]
	LD_E_A;  // ld e, a

	PUSH_DE;  // push de
	INC_E;  // inc e
	CALL(mGetCoordTile);  // call GetCoordTile
	LD_addr_A(wTileDown);  // ld [wTileDown], a
	CALL(mGetMovementPermissions_Down);  // call .Down

	POP_DE;  // pop de
	DEC_E;  // dec e
	CALL(mGetCoordTile);  // call GetCoordTile
	LD_addr_A(wTileUp);  // ld [wTileUp], a
	CALL(mGetMovementPermissions_Up);  // call .Up
	RET;  // ret


LeftRight:
	SET_PC(0x2A5BU);
	LD_A_addr(wPlayerStandingMapX);  // ld a, [wPlayerStandingMapX]
	LD_D_A;  // ld d, a
	LD_A_addr(wPlayerStandingMapY);  // ld a, [wPlayerStandingMapY]
	LD_E_A;  // ld e, a

	PUSH_DE;  // push de
	DEC_D;  // dec d
	CALL(mGetCoordTile);  // call GetCoordTile
	LD_addr_A(wTileLeft);  // ld [wTileLeft], a
	CALL(mGetMovementPermissions_Left);  // call .Left

	POP_DE;  // pop de
	INC_D;  // inc d
	CALL(mGetCoordTile);  // call GetCoordTile
	LD_addr_A(wTileRight);  // ld [wTileRight], a
	CALL(mGetMovementPermissions_Right);  // call .Right
	RET;  // ret


Down:
	SET_PC(0x2A7AU);
	CALL(mGetMovementPermissions_CheckHiNybble);  // call .CheckHiNybble
	RET_NZ ;  // ret nz
	LD_A_addr(wTileDown);  // ld a, [wTileDown]
	AND_A(0b111);  // and %111
	CP_A(COLL_UP_WALL & 0b111);  // cp COLL_UP_WALL & %111 ; COLL_UP_BUOY & %111
	IF_Z goto ok_down;  // jr z, .ok_down
	CP_A(COLL_UP_RIGHT_WALL & 0b111);  // cp COLL_UP_RIGHT_WALL & %111 ; COLL_UP_RIGHT_BUOY & %111
	IF_Z goto ok_down;  // jr z, .ok_down
	CP_A(COLL_UP_LEFT_WALL & 0b111);  // cp COLL_UP_LEFT_WALL & %111 ; COLL_UP_LEFT_BUOY & %111
	RET_NZ ;  // ret nz


ok_down:
	SET_PC(0x2A8EU);
	LD_HL(wTilePermissions);  // ld hl, wTilePermissions
	SET_hl(3);  // set 3, [hl]
	RET;  // ret


Up:
	SET_PC(0x2A94U);
	CALL(mGetMovementPermissions_CheckHiNybble);  // call .CheckHiNybble
	RET_NZ ;  // ret nz
	LD_A_addr(wTileUp);  // ld a, [wTileUp]
	AND_A(0b111);  // and %111
	CP_A(COLL_DOWN_WALL & 0b111);  // cp COLL_DOWN_WALL & %111 ; COLL_DOWN_BUOY & %111
	IF_Z goto ok_up;  // jr z, .ok_up
	CP_A(COLL_DOWN_RIGHT_WALL & 0b111);  // cp COLL_DOWN_RIGHT_WALL & %111 ; COLL_DOWN_RIGHT_BUOY & %111
	IF_Z goto ok_up;  // jr z, .ok_up
	CP_A(COLL_DOWN_LEFT_WALL & 0b111);  // cp COLL_DOWN_LEFT_WALL & %111 ; COLL_DOWN_LEFT_BUOY & %111
	RET_NZ ;  // ret nz


ok_up:
	SET_PC(0x2AA8U);
	LD_HL(wTilePermissions);  // ld hl, wTilePermissions
	SET_hl(3);  // set 3, [hl]
	RET;  // ret


Right:
	SET_PC(0x2AAEU);
	CALL(mGetMovementPermissions_CheckHiNybble);  // call .CheckHiNybble
	RET_NZ ;  // ret nz
	LD_A_addr(wTileRight);  // ld a, [wTileRight]
	AND_A(0b111);  // and %111
	CP_A(COLL_LEFT_WALL & 0b111);  // cp COLL_LEFT_WALL & %111 ; COLL_LEFT_BUOY & %111
	IF_Z goto ok_right;  // jr z, .ok_right
	CP_A(COLL_DOWN_LEFT_WALL & 0b111);  // cp COLL_DOWN_LEFT_WALL & %111 ; COLL_DOWN_LEFT_BUOY & %111
	IF_Z goto ok_right;  // jr z, .ok_right
	CP_A(COLL_UP_LEFT_WALL & 0b111);  // cp COLL_UP_LEFT_WALL & %111 ; COLL_UP_LEFT_BUOY & %111
	RET_NZ ;  // ret nz


ok_right:
	SET_PC(0x2AC2U);
	LD_HL(wTilePermissions);  // ld hl, wTilePermissions
	SET_hl(3);  // set 3, [hl]
	RET;  // ret


Left:
	SET_PC(0x2AC8U);
	CALL(mGetMovementPermissions_CheckHiNybble);  // call .CheckHiNybble
	RET_NZ ;  // ret nz
	LD_A_addr(wTileLeft);  // ld a, [wTileLeft]
	AND_A(0b111);  // and %111
	CP_A(COLL_RIGHT_WALL & 0b111);  // cp COLL_RIGHT_WALL & %111 ; COLL_RIGHT_BUOY & %111
	IF_Z goto ok_left;  // jr z, .ok_left
	CP_A(COLL_DOWN_RIGHT_WALL & 0b111);  // cp COLL_DOWN_RIGHT_WALL & %111 ; COLL_DOWN_RIGHT_BUOY & %111
	IF_Z goto ok_left;  // jr z, .ok_left
	CP_A(COLL_UP_RIGHT_WALL & 0b111);  // cp COLL_UP_RIGHT_WALL & %111 ; COLL_UP_RIGHT_BUOY & %111
	RET_NZ ;  // ret nz


ok_left:
	SET_PC(0x2ADCU);
	LD_HL(wTilePermissions);  // ld hl, wTilePermissions
	SET_hl(3);  // set 3, [hl]
	RET;  // ret


CheckHiNybble:
	SET_PC(0x2AE2U);
	AND_A(0xf0);  // and $f0
	CP_A(HI_NYBBLE_SIDE_WALLS);  // cp HI_NYBBLE_SIDE_WALLS
	RET_Z ;  // ret z
	CP_A(HI_NYBBLE_SIDE_BUOYS);  // cp HI_NYBBLE_SIDE_BUOYS
	RET;  // ret

}

int GetFacingTileCoord(){
	SET_PC(0x2AEAU);
//  Return map coordinates in (d, e) and tile id in a
//  of the tile the player is facing.

	LD_A_addr(wPlayerDirection);  // ld a, [wPlayerDirection]
	AND_A(0b1100);  // and %1100
	SRL_A;  // srl a
	SRL_A;  // srl a
	LD_L_A;  // ld l, a
	LD_H(0);  // ld h, 0
	ADD_HL_HL;  // add hl, hl
	ADD_HL_HL;  // add hl, hl
	LD_DE(mGetFacingTileCoord_Directions);  // ld de, .Directions
	ADD_HL_DE;  // add hl, de

	LD_D_hl;  // ld d, [hl]
	INC_HL;  // inc hl
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl

	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a

	LD_A_addr(wPlayerStandingMapX);  // ld a, [wPlayerStandingMapX]
	ADD_A_D;  // add d
	LD_D_A;  // ld d, a
	LD_A_addr(wPlayerStandingMapY);  // ld a, [wPlayerStandingMapY]
	ADD_A_E;  // add e
	LD_E_A;  // ld e, a
	LD_A_hl;  // ld a, [hl]
	RET;  // ret


Directions:
	SET_PC(0x2B0FU);
// ;   x,  y
	//db ['0', '1'];  // db  0,  1
	//dw ['wTileDown'];  // dw wTileDown
	//db ['0', '-1'];  // db  0, -1
	//dw ['wTileUp'];  // dw wTileUp
	//db ['-1', '0'];  // db -1,  0
	//dw ['wTileLeft'];  // dw wTileLeft
	//db ['1', '0'];  // db  1,  0
	//dw ['wTileRight'];  // dw wTileRight

	return GetCoordTile();
}

int GetCoordTile(){
	SET_PC(0x2B1FU);
//  Get the collision byte for tile d, e
	CALL(mGetBlockLocation);  // call GetBlockLocation
	LD_A_hl;  // ld a, [hl]
	AND_A_A;  // and a
	IF_Z goto nope;  // jr z, .nope
	LD_L_A;  // ld l, a
	LD_H(0);  // ld h, 0
	ADD_HL_HL;  // add hl, hl
	ADD_HL_HL;  // add hl, hl
	LD_A_addr(wTilesetCollisionAddress);  // ld a, [wTilesetCollisionAddress]
	LD_C_A;  // ld c, a
	LD_A_addr(wTilesetCollisionAddress + 1);  // ld a, [wTilesetCollisionAddress + 1]
	LD_B_A;  // ld b, a
	ADD_HL_BC;  // add hl, bc
	RR_D;  // rr d
	IF_NC goto nocarry;  // jr nc, .nocarry
	INC_HL;  // inc hl


nocarry:
	SET_PC(0x2B39U);
	RR_E;  // rr e
	IF_NC goto nocarry2;  // jr nc, .nocarry2
	INC_HL;  // inc hl
	INC_HL;  // inc hl


nocarry2:
	SET_PC(0x2B3FU);
	LD_A_addr(wTilesetCollisionBank);  // ld a, [wTilesetCollisionBank]
	CALL(mGetFarByte);  // call GetFarByte
	RET;  // ret


nope:
	SET_PC(0x2B46U);
	LD_A(-1);  // ld a, -1
	RET;  // ret

}

int GetBlockLocation(){
	SET_PC(0x2B49U);
	LD_A_addr(wMapWidth);  // ld a, [wMapWidth]
	ADD_A(6);  // add 6
	LD_C_A;  // ld c, a
	LD_B(0);  // ld b, 0
	LD_HL(wOverworldMapBlocks + 1);  // ld hl, wOverworldMapBlocks + 1
	ADD_HL_BC;  // add hl, bc
	LD_A_E;  // ld a, e
	SRL_A;  // srl a
	IF_Z goto nope;  // jr z, .nope
	AND_A_A;  // and a

loop:
	SET_PC(0x2B5BU);
	SRL_A;  // srl a
	IF_NC goto ok;  // jr nc, .ok
	ADD_HL_BC;  // add hl, bc


ok:
	SET_PC(0x2B60U);
	SLA_C;  // sla c
	RL_B;  // rl b
	AND_A_A;  // and a
	IF_NZ goto loop;  // jr nz, .loop


nope:
	SET_PC(0x2B67U);
	LD_C_D;  // ld c, d
	SRL_C;  // srl c
	LD_B(0);  // ld b, 0
	ADD_HL_BC;  // add hl, bc
	RET;  // ret

}

int CheckFacingBGEvent(){
	SET_PC(0x2B6EU);
	CALL(mGetFacingTileCoord);  // call GetFacingTileCoord
//  Load facing into b.
	LD_B_A;  // ld b, a
//  Convert the coordinates at de to within-boundaries coordinates.
	LD_A_D;  // ld a, d
	SUB_A(4);  // sub 4
	LD_D_A;  // ld d, a
	LD_A_E;  // ld a, e
	SUB_A(4);  // sub 4
	LD_E_A;  // ld e, a
//  If there are no BG events, we don't need to be here.
	LD_A_addr(wCurMapBGEventCount);  // ld a, [wCurMapBGEventCount]
	AND_A_A;  // and a
	RET_Z ;  // ret z

	LD_C_A;  // ld c, a
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	CALL(mSwitchToMapScriptsBank);  // call SwitchToMapScriptsBank
	CALL(mCheckIfFacingTileCoordIsBGEvent);  // call CheckIfFacingTileCoordIsBGEvent
	POP_HL;  // pop hl
	LD_A_H;  // ld a, h
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret

}

int CheckIfFacingTileCoordIsBGEvent(){
	SET_PC(0x2B8DU);
//  Checks to see if you are facing a BG event.  If so, copies it into wCurBGEvent and sets carry.
	LD_HL(wCurMapBGEventsPointer);  // ld hl, wCurMapBGEventsPointer
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a

loop:
	SET_PC(0x2B93U);
	PUSH_HL;  // push hl
	LD_A_hli;  // ld a, [hli]
	CP_A_E;  // cp e
	IF_NZ goto next;  // jr nz, .next
	LD_A_hli;  // ld a, [hli]
	CP_A_D;  // cp d
	IF_NZ goto next;  // jr nz, .next
	goto copysign;  // jr .copysign


next:
	SET_PC(0x2B9EU);
	POP_HL;  // pop hl
	LD_A(BG_EVENT_SIZE);  // ld a, BG_EVENT_SIZE
	ADD_A_L;  // add l
	LD_L_A;  // ld l, a
	IF_NC goto nocarry;  // jr nc, .nocarry
	INC_H;  // inc h


nocarry:
	SET_PC(0x2BA6U);
	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop
	XOR_A_A;  // xor a
	RET;  // ret


copysign:
	SET_PC(0x2BABU);
	POP_HL;  // pop hl
	LD_DE(wCurBGEvent);  // ld de, wCurBGEvent
	LD_BC(BG_EVENT_SIZE);  // ld bc, BG_EVENT_SIZE
	CALL(mCopyBytes);  // call CopyBytes
	SCF;  // scf
	RET;  // ret

}

int CheckCurrentMapCoordEvents(){
	SET_PC(0x2BB7U);
//  If there are no coord events, we don't need to be here.
	LD_A_addr(wCurMapCoordEventCount);  // ld a, [wCurMapCoordEventCount]
	AND_A_A;  // and a
	RET_Z ;  // ret z
//  Copy the coord event count into c.
	LD_C_A;  // ld c, a
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	CALL(mSwitchToMapScriptsBank);  // call SwitchToMapScriptsBank
	CALL(mCheckCurrentMapCoordEvents_CoordEventCheck);  // call .CoordEventCheck
	POP_HL;  // pop hl
	LD_A_H;  // ld a, h
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret


CoordEventCheck:
	SET_PC(0x2BCAU);
//  Checks to see if you are standing on a coord event.  If yes, copies the event to wCurCoordEvent and sets carry.
	LD_HL(wCurMapCoordEventsPointer);  // ld hl, wCurMapCoordEventsPointer
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
//  Load the active scene ID into b
	CALL(mCheckScenes);  // call CheckScenes
	LD_B_A;  // ld b, a
//  Load your current coordinates into de.  This will be used to check if your position is in the coord event table for the current map.
	LD_A_addr(wPlayerStandingMapX);  // ld a, [wPlayerStandingMapX]
	SUB_A(4);  // sub 4
	LD_D_A;  // ld d, a
	LD_A_addr(wPlayerStandingMapY);  // ld a, [wPlayerStandingMapY]
	SUB_A(4);  // sub 4
	LD_E_A;  // ld e, a


loop:
	SET_PC(0x2BE0U);
	PUSH_HL;  // push hl
	LD_A_hli;  // ld a, [hli]
	CP_A_B;  // cp b
	IF_Z goto got_id;  // jr z, .got_id
	CP_A(-1);  // cp -1
	IF_NZ goto next;  // jr nz, .next


got_id:
	SET_PC(0x2BE9U);
	LD_A_hli;  // ld a, [hli]
	CP_A_E;  // cp e
	IF_NZ goto next;  // jr nz, .next
	LD_A_hli;  // ld a, [hli]
	CP_A_D;  // cp d
	IF_NZ goto next;  // jr nz, .next
	goto copy_coord_event;  // jr .copy_coord_event


next:
	SET_PC(0x2BF3U);
	POP_HL;  // pop hl
	LD_A(COORD_EVENT_SIZE);  // ld a, COORD_EVENT_SIZE
	ADD_A_L;  // add l
	LD_L_A;  // ld l, a
	IF_NC goto nocarry;  // jr nc, .nocarry
	INC_H;  // inc h


nocarry:
	SET_PC(0x2BFBU);
	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop
	XOR_A_A;  // xor a
	RET;  // ret


copy_coord_event:
	SET_PC(0x2C00U);
	POP_HL;  // pop hl
	LD_DE(wCurCoordEvent);  // ld de, wCurCoordEvent
	LD_BC(COORD_EVENT_SIZE);  // ld bc, COORD_EVENT_SIZE
	CALL(mCopyBytes);  // call CopyBytes
	SCF;  // scf
	RET;  // ret

}

int FadeToMenu(){
	SET_PC(0x2C0CU);
	XOR_A_A;  // xor a
	LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a
	CALL(mLoadStandardMenuHeader);  // call LoadStandardMenuHeader
	FARCALL(aFadeOutPalettes);  // farcall FadeOutPalettes
	CALL(mClearSprites);  // call ClearSprites
	CALL(mDisableSpriteUpdates);  // call DisableSpriteUpdates
	RET;  // ret

}

int CloseSubmenu(){
	SET_PC(0x2C1FU);
	CALL(mClearBGPalettes);  // call ClearBGPalettes
	CALL(mReloadTilesetAndPalettes);  // call ReloadTilesetAndPalettes
	CALL(mUpdateSprites);  // call UpdateSprites
	CALL(mCall_ExitMenu);  // call Call_ExitMenu
	CALL(mReloadPalettes);  // call ReloadPalettes
	JR(mFinishExitMenu);  // jr FinishExitMenu

}

int ExitAllMenus(){
	SET_PC(0x2C30U);
	CALL(mClearBGPalettes);  // call ClearBGPalettes
	CALL(mCall_ExitMenu);  // call Call_ExitMenu
	CALL(mReloadTilesetAndPalettes);  // call ReloadTilesetAndPalettes
	CALL(mUpdateSprites);  // call UpdateSprites
	CALL(mReloadPalettes);  // call ReloadPalettes
	return FinishExitMenu();
}

int FinishExitMenu(){
	SET_PC(0x2C3FU);
	LD_B(SCGB_MAPPALS);  // ld b, SCGB_MAPPALS
	CALL(mGetSGBLayout);  // call GetSGBLayout
	CALL(mWaitBGMap2);  // call WaitBGMap2
	FARCALL(aFadeInPalettes);  // farcall FadeInPalettes
	CALL(mEnableSpriteUpdates);  // call EnableSpriteUpdates
	RET;  // ret

}

int ReturnToMapWithSpeechTextbox(){
	SET_PC(0x2C51U);
	PUSH_AF;  // push af
	LD_A(0x1);  // ld a, $1
	LD_addr_A(wSpriteUpdatesEnabled);  // ld [wSpriteUpdatesEnabled], a
	CALL(mClearBGPalettes);  // call ClearBGPalettes
	CALL(mClearSprites);  // call ClearSprites
	CALL(mReloadTilesetAndPalettes);  // call ReloadTilesetAndPalettes
	hlcoord(0, 12, wTilemap);  // hlcoord 0, 12
	LD_BC((4 << 8) | 18);  // lb bc, 4, 18
	CALL(mTextbox);  // call Textbox
	LD_HL(wVramState);  // ld hl, wVramState
	SET_hl(0);  // set 0, [hl]
	CALL(mUpdateSprites);  // call UpdateSprites
	CALL(mWaitBGMap2);  // call WaitBGMap2
	LD_B(SCGB_MAPPALS);  // ld b, SCGB_MAPPALS
	CALL(mGetSGBLayout);  // call GetSGBLayout
	CALL(mUpdateTimePals);  // call UpdateTimePals
	CALL(mDelayFrame);  // call DelayFrame
	LD_A(0x1);  // ld a, $1
	LDH_addr_A(hMapAnims);  // ldh [hMapAnims], a
	POP_AF;  // pop af
	RET;  // ret

}

int ReloadTilesetAndPalettes(){
	SET_PC(0x2C85U);
	CALL(mDisableLCD);  // call DisableLCD
	CALL(mClearSprites);  // call ClearSprites
	FARCALL(av_RefreshSprites);  // farcall _RefreshSprites
	CALL(mLoadStandardFont);  // call LoadStandardFont
	CALL(mLoadFontsExtra);  // call LoadFontsExtra
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A_addr(wMapGroup);  // ld a, [wMapGroup]
	LD_B_A;  // ld b, a
	LD_A_addr(wMapNumber);  // ld a, [wMapNumber]
	LD_C_A;  // ld c, a
	CALL(mSwitchToAnyMapAttributesBank);  // call SwitchToAnyMapAttributesBank
	FARCALL(aUpdateTimeOfDayPal);  // farcall UpdateTimeOfDayPal
	CALL(mOverworldTextModeSwitch);  // call OverworldTextModeSwitch
	CALL(mLoadTilesetGFX);  // call LoadTilesetGFX
	LD_A(8);  // ld a, 8
	CALL(mSkipMusic);  // call SkipMusic
	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch

	CALL(mEnableLCD);  // call EnableLCD
	RET;  // ret

}

int GetMapPointer(){
	SET_PC(0x2CBCU);
	LD_A_addr(wMapGroup);  // ld a, [wMapGroup]
	LD_B_A;  // ld b, a
	LD_A_addr(wMapNumber);  // ld a, [wMapNumber]
	LD_C_A;  // ld c, a
	return GetAnyMapPointer();
}

int GetAnyMapPointer(){
	SET_PC(0x2CC4U);
//  Prior to calling this function, you must have switched banks so that
//  MapGroupPointers is visible.

//  inputs:
//  b = map group, c = map number

//  outputs:
//  hl points to the map within its group
	PUSH_BC;  // push bc ; save map number for later

// ; get pointer to map group
	DEC_B;  // dec b
	LD_C_B;  // ld c, b
	LD_B(0);  // ld b, 0
	LD_HL(mMapGroupPointers);  // ld hl, MapGroupPointers
	ADD_HL_BC;  // add hl, bc
	ADD_HL_BC;  // add hl, bc

	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	POP_BC;  // pop bc ; restore map number

// ; find the cth map within the group
	DEC_C;  // dec c
	LD_B(0);  // ld b, 0
	LD_A(9);  // ld a, 9
	CALL(mAddNTimes);  // call AddNTimes
	RET;  // ret

}

int GetMapField(){
	SET_PC(0x2CDBU);
//  Extract data from the current map's group entry.

//  inputs:
//  de = offset of desired data within the map (a MAP_* constant)

//  outputs:
//  bc = data from the current map's field
//  (e.g., de = MAP_TILESET would return a pointer to the tileset id)

	LD_A_addr(wMapGroup);  // ld a, [wMapGroup]
	LD_B_A;  // ld b, a
	LD_A_addr(wMapNumber);  // ld a, [wMapNumber]
	LD_C_A;  // ld c, a
	return GetAnyMapField();
}

int GetAnyMapField(){
	SET_PC(0x2CE3U);
// ; bankswitch
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A(BANK(aMapGroupPointers));  // ld a, BANK(MapGroupPointers)
	RST(mBankswitch);  // rst Bankswitch

	CALL(mGetAnyMapPointer);  // call GetAnyMapPointer
	ADD_HL_DE;  // add hl, de
	LD_C_hl;  // ld c, [hl]
	INC_HL;  // inc hl
	LD_B_hl;  // ld b, [hl]

// ; bankswitch back
	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret

}

int SwitchToMapAttributesBank(){
	SET_PC(0x2CF3U);
	LD_A_addr(wMapGroup);  // ld a, [wMapGroup]
	LD_B_A;  // ld b, a
	LD_A_addr(wMapNumber);  // ld a, [wMapNumber]
	LD_C_A;  // ld c, a
	return SwitchToAnyMapAttributesBank();
}

int SwitchToAnyMapAttributesBank(){
	SET_PC(0x2CFBU);
	CALL(mGetAnyMapAttributesBank);  // call GetAnyMapAttributesBank
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret

}

int GetMapAttributesBank(){
	SET_PC(0x2D00U);
//  //  unreferenced
	LD_A_addr(wMapGroup);  // ld a, [wMapGroup]
	LD_B_A;  // ld b, a
	LD_A_addr(wMapNumber);  // ld a, [wMapNumber]
	LD_C_A;  // ld c, a
	return GetAnyMapAttributesBank();
}

int GetAnyMapAttributesBank(){
	SET_PC(0x2D08U);
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	LD_DE(MAP_MAPATTRIBUTES_BANK);  // ld de, MAP_MAPATTRIBUTES_BANK
	CALL(mGetAnyMapField);  // call GetAnyMapField
	LD_A_C;  // ld a, c
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int CopyMapPartial(){
	SET_PC(0x2D14U);
//  Copy map data bank, tileset, environment, and map data address
//  from the current map's entry within its group.
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A(BANK(aMapGroupPointers));  // ld a, BANK(MapGroupPointers)
	RST(mBankswitch);  // rst Bankswitch

	CALL(mGetMapPointer);  // call GetMapPointer
	LD_DE(wMapPartial);  // ld de, wMapPartial
	LD_BC(wMapPartialEnd - wMapPartial);  // ld bc, wMapPartialEnd - wMapPartial
	CALL(mCopyBytes);  // call CopyBytes

	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret

}

int SwitchToMapScriptsBank(){
	SET_PC(0x2D29U);
	LD_A_addr(wMapScriptsBank);  // ld a, [wMapScriptsBank]
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret

}

int GetMapScriptsBank(){
	SET_PC(0x2D2EU);
	LD_A_addr(wMapScriptsBank);  // ld a, [wMapScriptsBank]
	RET;  // ret

}

int GetAnyMapBlocksBank(){
	SET_PC(0x2D32U);
//  Return the blockdata bank for group b map c.
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc

	PUSH_BC;  // push bc
	LD_DE(MAP_MAPATTRIBUTES);  // ld de, MAP_MAPATTRIBUTES
	CALL(mGetAnyMapField);  // call GetAnyMapField
	LD_L_C;  // ld l, c
	LD_H_B;  // ld h, b
	POP_BC;  // pop bc

	PUSH_HL;  // push hl
	LD_DE(MAP_MAPATTRIBUTES_BANK);  // ld de, MAP_MAPATTRIBUTES_BANK
	CALL(mGetAnyMapField);  // call GetAnyMapField
	POP_HL;  // pop hl

	LD_DE(MAP_MAPATTRIBUTES);  // ld de, MAP_MAPATTRIBUTES ; blockdata bank
	ADD_HL_DE;  // add hl, de
	LD_A_C;  // ld a, c
	CALL(mGetFarByte);  // call GetFarByte
	RST(mBankswitch);  // rst Bankswitch

	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int GetMapAttributesPointer(){
	SET_PC(0x2D54U);
//  returns the current map's data pointer in hl.
	PUSH_BC;  // push bc
	PUSH_DE;  // push de
	LD_DE(MAP_MAPATTRIBUTES);  // ld de, MAP_MAPATTRIBUTES
	CALL(mGetMapField);  // call GetMapField
	LD_L_C;  // ld l, c
	LD_H_B;  // ld h, b
	POP_DE;  // pop de
	POP_BC;  // pop bc
	RET;  // ret

}

int GetMapEnvironment(){
	SET_PC(0x2D61U);
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	LD_DE(MAP_ENVIRONMENT);  // ld de, MAP_ENVIRONMENT
	CALL(mGetMapField);  // call GetMapField
	LD_A_C;  // ld a, c
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int Map_DummyFunction(){
	SET_PC(0x2D6FU);
//  //  unreferenced
	RET;  // ret

}

int GetAnyMapEnvironment(){
	SET_PC(0x2D70U);
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	LD_DE(MAP_ENVIRONMENT);  // ld de, MAP_ENVIRONMENT
	CALL(mGetAnyMapField);  // call GetAnyMapField
	LD_A_C;  // ld a, c
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int GetAnyMapTileset(){
	SET_PC(0x2D7EU);
	LD_DE(MAP_TILESET);  // ld de, MAP_TILESET
	CALL(mGetAnyMapField);  // call GetAnyMapField
	LD_A_C;  // ld a, c
	RET;  // ret

}

int GetWorldMapLocation(){
	SET_PC(0x2D86U);
//  given a map group/id in bc, return its location on the Pokégear map.
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc

	LD_DE(MAP_LOCATION);  // ld de, MAP_LOCATION
	CALL(mGetAnyMapField);  // call GetAnyMapField
	LD_A_C;  // ld a, c

	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int GetMapMusic(){
	SET_PC(0x2D94U);
	PUSH_HL;  // push hl
	PUSH_BC;  // push bc
	LD_DE(MAP_MUSIC);  // ld de, MAP_MUSIC
	CALL(mGetMapField);  // call GetMapField
	LD_A_C;  // ld a, c
	CP_A(MUSIC_MAHOGANY_MART);  // cp MUSIC_MAHOGANY_MART
	IF_Z goto mahoganymart;  // jr z, .mahoganymart
	BIT_C(RADIO_TOWER_MUSIC_F);  // bit RADIO_TOWER_MUSIC_F, c
	IF_NZ goto radiotower;  // jr nz, .radiotower
	LD_E_C;  // ld e, c
	LD_D(0);  // ld d, 0

done:
	SET_PC(0x2DA8U);
	POP_BC;  // pop bc
	POP_HL;  // pop hl
	RET;  // ret


radiotower:
	SET_PC(0x2DABU);
	LD_A_addr(wStatusFlags2);  // ld a, [wStatusFlags2]
	BIT_A(STATUSFLAGS2_ROCKETS_IN_RADIO_TOWER_F);  // bit STATUSFLAGS2_ROCKETS_IN_RADIO_TOWER_F, a
	IF_Z goto clearedradiotower;  // jr z, .clearedradiotower
	LD_DE(MUSIC_ROCKET_OVERTURE);  // ld de, MUSIC_ROCKET_OVERTURE
	goto done;  // jr .done


clearedradiotower:
	SET_PC(0x2DB7U);
// ; the rest of the byte
	LD_A_C;  // ld a, c
	AND_A(RADIO_TOWER_MUSIC - 1);  // and RADIO_TOWER_MUSIC - 1
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
	goto done;  // jr .done


mahoganymart:
	SET_PC(0x2DBFU);
	LD_A_addr(wStatusFlags2);  // ld a, [wStatusFlags2]
	BIT_A(STATUSFLAGS2_ROCKETS_IN_MAHOGANY_F);  // bit STATUSFLAGS2_ROCKETS_IN_MAHOGANY_F, a
	IF_Z goto clearedmahogany;  // jr z, .clearedmahogany
	LD_DE(MUSIC_ROCKET_HIDEOUT);  // ld de, MUSIC_ROCKET_HIDEOUT
	goto done;  // jr .done


clearedmahogany:
	SET_PC(0x2DCBU);
	LD_DE(MUSIC_CHERRYGROVE_CITY);  // ld de, MUSIC_CHERRYGROVE_CITY
	goto done;  // jr .done

	return GetMapTimeOfDay();
}

int GetMapTimeOfDay(){
	SET_PC(0x2DD0U);
	CALL(mGetPhoneServiceTimeOfDayByte);  // call GetPhoneServiceTimeOfDayByte
	AND_A(0xf);  // and $f
	RET;  // ret

}

int GetMapPhoneService(){
	SET_PC(0x2DD6U);
	CALL(mGetPhoneServiceTimeOfDayByte);  // call GetPhoneServiceTimeOfDayByte
	AND_A(0xf0);  // and $f0
	SWAP_A;  // swap a
	RET;  // ret

}

int GetPhoneServiceTimeOfDayByte(){
	SET_PC(0x2DDEU);
	PUSH_HL;  // push hl
	PUSH_BC;  // push bc

	LD_DE(MAP_PALETTE);  // ld de, MAP_PALETTE
	CALL(mGetMapField);  // call GetMapField
	LD_A_C;  // ld a, c

	POP_BC;  // pop bc
	POP_HL;  // pop hl
	RET;  // ret

}

int GetFishingGroup(){
	SET_PC(0x2DEAU);
	PUSH_DE;  // push de
	PUSH_HL;  // push hl
	PUSH_BC;  // push bc

	LD_DE(MAP_FISHGROUP);  // ld de, MAP_FISHGROUP
	CALL(mGetMapField);  // call GetMapField
	LD_A_C;  // ld a, c

	POP_BC;  // pop bc
	POP_HL;  // pop hl
	POP_DE;  // pop de
	RET;  // ret

}

int LoadMapTileset(){
	SET_PC(0x2DF8U);
	PUSH_HL;  // push hl
	PUSH_BC;  // push bc

	LD_HL(mTilesets);  // ld hl, Tilesets
	LD_BC(TILESET_LENGTH);  // ld bc, TILESET_LENGTH
	LD_A_addr(wMapTileset);  // ld a, [wMapTileset]
	CALL(mAddNTimes);  // call AddNTimes

	LD_DE(wTilesetBank);  // ld de, wTilesetBank
	LD_BC(TILESET_LENGTH);  // ld bc, TILESET_LENGTH

	LD_A(BANK(aTilesets));  // ld a, BANK(Tilesets)
	CALL(mFarCopyBytes);  // call FarCopyBytes

	POP_BC;  // pop bc
	POP_HL;  // pop hl
	RET;  // ret

}

int DummyEndPredef(){
	SET_PC(0x2E14U);
//  Unused function at the end of PredefPointers.
for(int rept = 0; rept < 16; rept++){
	NOP;  // nop
}
	RET;  // ret

}
