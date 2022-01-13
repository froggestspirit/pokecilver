#include "../constants.h"

//  Functions dealing with rendering and interacting with maps.

void ClearUnusedMapBuffer(void){
	LD_HL(wUnusedMapBuffer);  // ld hl, wUnusedMapBuffer
	LD_BC(wUnusedMapBufferEnd - wUnusedMapBuffer);  // ld bc, wUnusedMapBufferEnd - wUnusedMapBuffer
	LD_A(0);  // ld a, 0
	CCALL(aByteFill);  // call ByteFill
	RET;  // ret

}

void CheckScenes(void){
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
	POP_HL;  // pop hl
	RET;  // ret

}

void GetCurrentMapSceneID(void){
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
	CCALL(aGetMapSceneID);  // call GetMapSceneID
	RET_C ;  // ret c ; The map is not in the scene script table
//  Load the scene script pointer from de into wCurMapSceneScriptPointer
	LD_A_E;  // ld a, e
	LD_addr_A(wCurMapSceneScriptPointer);  // ld [wCurMapSceneScriptPointer], a
	LD_A_D;  // ld a, d
	LD_addr_A(wCurMapSceneScriptPointer + 1);  // ld [wCurMapSceneScriptPointer + 1], a
	XOR_A_A;  // xor a
	RET;  // ret

}

void GetMapSceneID(void){
//  Searches the scene_var table for the map group and number loaded in bc, and returns the wram pointer in de.
//  If the map is not in the scene_var table, returns carry.
	PUSH_BC;  // push bc
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A(BANK(aMapScenes));  // ld a, BANK(MapScenes)
	Bankswitch();  // rst Bankswitch

	LD_HL(mMapScenes);  // ld hl, MapScenes

loop:
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
	POP_HL;  // pop hl
	LD_DE(4);  // ld de, 4 ; scene_var size
	ADD_HL_DE;  // add hl, de
	goto loop;  // jr .loop


end:
	SCF;  // scf
	goto done;  // jr .done


found:
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]


done:
	POP_HL;  // pop hl
	POP_BC;  // pop bc
	LD_A_B;  // ld a, b
	Bankswitch();  // rst Bankswitch

	POP_BC;  // pop bc
	RET;  // ret

}

void OverworldTextModeSwitch(void){
	SET_PC(0x1FBFU);
	CALL(mLoadMapPart);  // call LoadMapPart
	CALL(mSwapTextboxPalettes);  // call SwapTextboxPalettes
	RET;  // ret

}

void LoadMapPart(void){
	SET_PC(0x1FC6U);
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af

	LD_A_addr(wTilesetBlocksBank);  // ld a, [wTilesetBlocksBank]
	RST(mBankswitch);  // rst Bankswitch
	CALL(mLoadMetatiles);  // call LoadMetatiles

	LD_A(0x60);  // ld a, "■"
	hlcoord(0, 0, wTilemap);  // hlcoord 0, 0
	LD_BC(SCREEN_WIDTH * SCREEN_HEIGHT);  // ld bc, SCREEN_WIDTH * SCREEN_HEIGHT
	CCALL(aByteFill);  // call ByteFill

	LD_A(BANK(av_LoadMapPart));  // ld a, BANK(_LoadMapPart)
	RST(mBankswitch);  // rst Bankswitch
	CALL(mv_LoadMapPart);  // call _LoadMapPart

	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret

}

void LoadMetatiles(void){
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

void ReturnToMapFromSubmenu(void){
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

void CheckOutdoorMap(void){
	CP_A(ROUTE);  // cp ROUTE
	RET_Z ;  // ret z
	CP_A(TOWN);  // cp TOWN
	RET;  // ret

}

void CheckIndoorMap(void){
	CP_A(INDOOR);  // cp INDOOR
	RET_Z ;  // ret z
	CP_A(CAVE);  // cp CAVE
	RET_Z ;  // ret z
	CP_A(DUNGEON);  // cp DUNGEON
	RET_Z ;  // ret z
	CP_A(GATE);  // cp GATE
	RET;  // ret

}

void CheckUnknownMap(void){
//  //  unreferenced
	CP_A(INDOOR);  // cp INDOOR
	RET_Z ;  // ret z
	CP_A(GATE);  // cp GATE
	RET_Z ;  // ret z
	CP_A(ENVIRONMENT_5);  // cp ENVIRONMENT_5
	RET;  // ret

}

void LoadMapAttributes(void){
	CCALL(aCopyMapPartialAndAttributes);  // call CopyMapPartialAndAttributes
	CCALL(aSwitchToMapScriptsBank);  // call SwitchToMapScriptsBank
	CCALL(aReadMapScripts);  // call ReadMapScripts
	XOR_A_A;  // xor a ; do not skip object events
	CCALL(aReadMapEvents);  // call ReadMapEvents
	RET;  // ret

}

void LoadMapAttributes_SkipObjects(void){
	CCALL(aCopyMapPartialAndAttributes);  // call CopyMapPartialAndAttributes
	CCALL(aSwitchToMapScriptsBank);  // call SwitchToMapScriptsBank
	CCALL(aReadMapScripts);  // call ReadMapScripts
	LD_A(TRUE);  // ld a, TRUE ; skip object events
	CCALL(aReadMapEvents);  // call ReadMapEvents
	RET;  // ret

}

void CopyMapPartialAndAttributes(void){
	CCALL(aCopyMapPartial);  // call CopyMapPartial
	CCALL(aSwitchToMapAttributesBank);  // call SwitchToMapAttributesBank
	CCALL(aGetMapAttributesPointer);  // call GetMapAttributesPointer
	CCALL(aCopyMapAttributes);  // call CopyMapAttributes
	CCALL(aGetMapConnections);  // call GetMapConnections
	RET;  // ret

}

void ReadMapEvents(void){
	PUSH_AF;  // push af
	LD_HL(wMapEventsPointer);  // ld hl, wMapEventsPointer
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	INC_HL;  // inc hl
	INC_HL;  // inc hl
	CCALL(aReadWarps);  // call ReadWarps
	CCALL(aReadCoordEvents);  // call ReadCoordEvents
	CCALL(aReadBGEvents);  // call ReadBGEvents

	POP_AF;  // pop af
	AND_A_A;  // and a ; skip object events?
	RET_NZ ;  // ret nz

	CCALL(aReadObjectEvents);  // call ReadObjectEvents
	RET;  // ret

}

void ReadMapScripts(void){
	LD_HL(wMapScriptsPointer);  // ld hl, wMapScriptsPointer
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	CCALL(aReadMapSceneScripts);  // call ReadMapSceneScripts
	CCALL(aReadMapCallbacks);  // call ReadMapCallbacks
	RET;  // ret

}

void CopyMapAttributes(void){
	LD_DE(wMapAttributes);  // ld de, wMapAttributes
	LD_C(wMapAttributesEnd - wMapAttributes);  // ld c, wMapAttributesEnd - wMapAttributes

loop:
	LD_A_hli;  // ld a, [hli]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

void GetMapConnections(void){
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
	CCALL(aGetMapConnection);  // call GetMapConnection

no_north:

	BIT_B(SOUTH_F);  // bit SOUTH_F, b
	IF_Z goto no_south;  // jr z, .no_south
	LD_DE(wSouthMapConnection);  // ld de, wSouthMapConnection
	CCALL(aGetMapConnection);  // call GetMapConnection

no_south:

	BIT_B(WEST_F);  // bit WEST_F, b
	IF_Z goto no_west;  // jr z, .no_west
	LD_DE(wWestMapConnection);  // ld de, wWestMapConnection
	CCALL(aGetMapConnection);  // call GetMapConnection

no_west:

	BIT_B(EAST_F);  // bit EAST_F, b
	IF_Z goto no_east;  // jr z, .no_east
	LD_DE(wEastMapConnection);  // ld de, wEastMapConnection
	CCALL(aGetMapConnection);  // call GetMapConnection

no_east:

	RET;  // ret

}

void GetMapConnection(void){
//  Load map connection struct at hl into de.
	LD_C(wSouthMapConnection - wNorthMapConnection);  // ld c, wSouthMapConnection - wNorthMapConnection

loop:
	LD_A_hli;  // ld a, [hli]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

void ReadMapSceneScripts(void){
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
	CCALL(aAddNTimes);  // call AddNTimes
	RET;  // ret

}

void ReadMapCallbacks(void){
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
	CCALL(aAddNTimes);  // call AddNTimes
	RET;  // ret

}

void ReadWarps(void){
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
	CCALL(aAddNTimes);  // call AddNTimes
	RET;  // ret

}

void ReadCoordEvents(void){
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
	CCALL(aAddNTimes);  // call AddNTimes
	RET;  // ret

}

void ReadBGEvents(void){
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
	CCALL(aAddNTimes);  // call AddNTimes
	RET;  // ret

}

void ReadObjectEvents(void){
	PUSH_HL;  // push hl
	CCALL(aClearObjectStructs);  // call ClearObjectStructs
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
	CCALL(aCopyMapObjectEvents);  // call CopyMapObjectEvents

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
	LD_hl(0);  // ld [hl],  0
	INC_HL;  // inc hl
	LD_hl(-1);  // ld [hl], -1
	DEC_HL;  // dec hl
	ADD_HL_BC;  // add hl, bc
	DEC_A;  // dec a
	IF_NZ goto loop;  // jr nz, .loop


skip:
	LD_H_D;  // ld h, d
	LD_L_E;  // ld l, e
	RET;  // ret

}

void CopyMapObjectEvents(void){
	AND_A_A;  // and a
	RET_Z ;  // ret z

	LD_C_A;  // ld c, a

loop:
	PUSH_BC;  // push bc
	PUSH_HL;  // push hl
	LD_A(0xff);  // ld a, $ff
	LD_hli_A;  // ld [hli], a
	LD_B(OBJECT_EVENT_SIZE);  // ld b, OBJECT_EVENT_SIZE

loop2:
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

void ClearObjectStructs(void){
	LD_HL(wObject1Struct);  // ld hl, wObject1Struct
	LD_BC(OBJECT_LENGTH * (NUM_OBJECT_STRUCTS - 1));  // ld bc, OBJECT_LENGTH * (NUM_OBJECT_STRUCTS - 1)
	XOR_A_A;  // xor a
	CCALL(aByteFill);  // call ByteFill

//  Just to make sure (this is rather pointless)
	LD_HL(wObject1Struct);  // ld hl, wObject1Struct
	LD_DE(OBJECT_LENGTH);  // ld de, OBJECT_LENGTH
	LD_C(NUM_OBJECT_STRUCTS - 1);  // ld c, NUM_OBJECT_STRUCTS - 1
	XOR_A_A;  // xor a

loop:
	LD_hl_A;  // ld [hl], a
	ADD_HL_DE;  // add hl, de
	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

void GetWarpDestCoords(void){
	SET_PC(0x2567U);
	CCALL(aGetMapScriptsBank);  // call GetMapScriptsBank
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
	CCALL(aAddNTimes);  // call AddNTimes
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
	CCALL(aGetMapScreenCoords);  // call GetMapScreenCoords
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

void GetMapScreenCoords(void){
	LD_HL(wOverworldMapBlocks);  // ld hl, wOverworldMapBlocks
	LD_A_addr(wXCoord);  // ld a, [wXCoord]
	BIT_A(0);  // bit 0, a
	IF_NZ goto odd_x;  // jr nz, .odd_x
//  even x
	SRL_A;  // srl a
	ADD_A(1);  // add 1
	goto got_block_x;  // jr .got_block_x

odd_x:
	ADD_A(1);  // add 1
	SRL_A;  // srl a

got_block_x:
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
	ADD_A(1);  // add 1
	SRL_A;  // srl a

got_block_y:
	CCALL(aAddNTimes);  // call AddNTimes
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

void LoadBlockData(void){
	SET_PC(0x25F4U);
	LD_HL(wOverworldMapBlocks);  // ld hl, wOverworldMapBlocks
	LD_BC(wOverworldMapBlocksEnd - wOverworldMapBlocks);  // ld bc, wOverworldMapBlocksEnd - wOverworldMapBlocks
	LD_A(0);  // ld a, 0
	CCALL(aByteFill);  // call ByteFill
	CCALL(aChangeMap);  // call ChangeMap
	CCALL(aFillMapConnections);  // call FillMapConnections
	LD_A(MAPCALLBACK_TILES);  // ld a, MAPCALLBACK_TILES
	CALL(mRunMapCallback);  // call RunMapCallback
	RET;  // ret

}

void ChangeMap(void){
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
	Bankswitch();  // rst Bankswitch

	LD_A_addr(wMapBlocksPointer);  // ld a, [wMapBlocksPointer]
	LD_E_A;  // ld e, a
	LD_A_addr(wMapBlocksPointer + 1);  // ld a, [wMapBlocksPointer + 1]
	LD_D_A;  // ld d, a
	LD_A_addr(wMapHeight);  // ld a, [wMapHeight]
	LD_B_A;  // ld b, a

row:
	PUSH_HL;  // push hl
	LDH_A_addr(hConnectedMapWidth);  // ldh a, [hConnectedMapWidth]
	LD_C_A;  // ld c, a

col:
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
	DEC_B;  // dec b
	IF_NZ goto row;  // jr nz, .row

	POP_AF;  // pop af
	Bankswitch();  // rst Bankswitch
	RET;  // ret

}

void FillMapConnections(void){
//  North
	LD_A_addr(wNorthConnectedMapGroup);  // ld a, [wNorthConnectedMapGroup]
	CP_A(0xff);  // cp $ff
	IF_Z goto South;  // jr z, .South
	LD_B_A;  // ld b, a
	LD_A_addr(wNorthConnectedMapNumber);  // ld a, [wNorthConnectedMapNumber]
	LD_C_A;  // ld c, a
	CCALL(aGetAnyMapBlocksBank);  // call GetAnyMapBlocksBank

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
	CCALL(aFillNorthConnectionStrip);  // call FillNorthConnectionStrip


South:
	LD_A_addr(wSouthConnectedMapGroup);  // ld a, [wSouthConnectedMapGroup]
	CP_A(0xff);  // cp $ff
	IF_Z goto West;  // jr z, .West
	LD_B_A;  // ld b, a
	LD_A_addr(wSouthConnectedMapNumber);  // ld a, [wSouthConnectedMapNumber]
	LD_C_A;  // ld c, a
	CCALL(aGetAnyMapBlocksBank);  // call GetAnyMapBlocksBank

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
	CCALL(aFillSouthConnectionStrip);  // call FillSouthConnectionStrip


West:
	LD_A_addr(wWestConnectedMapGroup);  // ld a, [wWestConnectedMapGroup]
	CP_A(0xff);  // cp $ff
	IF_Z goto East;  // jr z, .East
	LD_B_A;  // ld b, a
	LD_A_addr(wWestConnectedMapNumber);  // ld a, [wWestConnectedMapNumber]
	LD_C_A;  // ld c, a
	CCALL(aGetAnyMapBlocksBank);  // call GetAnyMapBlocksBank

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
	CCALL(aFillWestConnectionStrip);  // call FillWestConnectionStrip


East:
	LD_A_addr(wEastConnectedMapGroup);  // ld a, [wEastConnectedMapGroup]
	CP_A(0xff);  // cp $ff
	IF_Z goto Done;  // jr z, .Done
	LD_B_A;  // ld b, a
	LD_A_addr(wEastConnectedMapNumber);  // ld a, [wEastConnectedMapNumber]
	LD_C_A;  // ld c, a
	CCALL(aGetAnyMapBlocksBank);  // call GetAnyMapBlocksBank

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
	CCALL(aFillEastConnectionStrip);  // call FillEastConnectionStrip


Done:
	RET;  // ret

}

void FillNorthConnectionStrip(void){
	return FillSouthConnectionStrip();
}

void FillSouthConnectionStrip(void){
	LD_C(3);  // ld c, 3

y:
	PUSH_DE;  // push de

	PUSH_HL;  // push hl
	LDH_A_addr(hConnectionStripLength);  // ldh a, [hConnectionStripLength]
	LD_B_A;  // ld b, a

x:
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
	DEC_C;  // dec c
	IF_NZ goto y;  // jr nz, .y
	RET;  // ret

}

void FillWestConnectionStrip(void){
	return FillEastConnectionStrip();
}

void FillEastConnectionStrip(void){

loop:
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
	DEC_B;  // dec b
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

void LoadMapStatus(void){
	LD_addr_A(wMapStatus);  // ld [wMapStatus], a
	RET;  // ret

}

void CallScript(void){
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

void CallMapScript(void){
	SET_PC(0x2758U);
//  Call a script at hl in the current bank if there isn't already a script running
	LD_A_addr(wScriptRunning);  // ld a, [wScriptRunning]
	AND_A_A;  // and a
	RET_NZ ;  // ret nz
	CCALL(aGetMapScriptsBank);  // call GetMapScriptsBank
	return CallScript();  // jr CallScript

}

void RunMapCallback(void){
	SET_PC(0x2762U);
//  Will run the first callback found with execution index equal to a.
	LD_B_A;  // ld b, a
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	CCALL(aSwitchToMapScriptsBank);  // call SwitchToMapScriptsBank
	CALL(mRunMapCallback_FindCallback);  // call .FindCallback
	IF_NC goto done;  // jr nc, .done

	CCALL(aGetMapScriptsBank);  // call GetMapScriptsBank
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

void ExecuteCallbackScript(void){
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

void MapTextbox(void){
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

void Call_a_de(void){
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

void GetMovementData(void){
//  Initialize the movement data for object c at b:hl
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A_B;  // ld a, b
	Bankswitch();  // rst Bankswitch

	LD_A_C;  // ld a, c
	CCALL(aLoadMovementDataPointer);  // call LoadMovementDataPointer

	POP_HL;  // pop hl
	LD_A_H;  // ld a, h
	Bankswitch();  // rst Bankswitch
	RET;  // ret

}

void GetScriptByte(void){
//  Return byte at wScriptBank:wScriptPos in a.

	PUSH_HL;  // push hl
	PUSH_BC;  // push bc
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A_addr(wScriptBank);  // ld a, [wScriptBank]
	Bankswitch();  // rst Bankswitch

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
	Bankswitch();  // rst Bankswitch
	LD_A_B;  // ld a, b
	POP_BC;  // pop bc
	POP_HL;  // pop hl
	RET;  // ret

}

void ObjectEvent(void){
	//jumptextfaceplayer ['ObjectEventText']  // jumptextfaceplayer ObjectEventText

	return ObjectEventText();
}

void ObjectEventText(void){
	//text_far ['_ObjectEventText']  // text_far _ObjectEventText
	//text_end ['?']  // text_end

	return BGEvent();
}

void BGEvent(void){
//  //  unreferenced
	//jumptext ['BGEventText']  // jumptext BGEventText

	return BGEventText();
}

void BGEventText(void){
	//text_far ['_BGEventText']  // text_far _BGEventText
	//text_end ['?']  // text_end

	return CoordinatesEvent();
}

void CoordinatesEvent(void){
//  //  unreferenced
	//jumptext ['CoordinatesEventText']  // jumptext CoordinatesEventText

	return CoordinatesEventText();
}

void CoordinatesEventText(void){
	//text_far ['_CoordinatesEventText']  // text_far _CoordinatesEventText
	//text_end ['?']  // text_end

	return CheckObjectMask();
}

void CheckObjectMask(void){
	LDH_A_addr(hMapObjectIndex);  // ldh a, [hMapObjectIndex]
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
	LD_HL(wObjectMasks);  // ld hl, wObjectMasks
	ADD_HL_DE;  // add hl, de
	LD_A_hl;  // ld a, [hl]
	RET;  // ret

}

void MaskObject(void){
	LDH_A_addr(hMapObjectIndex);  // ldh a, [hMapObjectIndex]
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
	LD_HL(wObjectMasks);  // ld hl, wObjectMasks
	ADD_HL_DE;  // add hl, de
	LD_hl(-1);  // ld [hl], -1 ; masked
	RET;  // ret

}

void UnmaskObject(void){
	LDH_A_addr(hMapObjectIndex);  // ldh a, [hMapObjectIndex]
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
	LD_HL(wObjectMasks);  // ld hl, wObjectMasks
	ADD_HL_DE;  // add hl, de
	LD_hl(0);  // ld [hl], 0 ; unmasked
	RET;  // ret

}

void ScrollMapUp(void){
	SET_PC(0x284BU);
	hlcoord(0, 0, wTilemap);  // hlcoord 0, 0
	LD_DE(wBGMapBuffer);  // ld de, wBGMapBuffer
	CCALL(aBackupBGMapRow);  // call BackupBGMapRow
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

void ScrollMapDown(void){
	SET_PC(0x2869U);
	hlcoord(0, SCREEN_HEIGHT - 2, wTilemap);  // hlcoord 0, SCREEN_HEIGHT - 2
	LD_DE(wBGMapBuffer);  // ld de, wBGMapBuffer
	CCALL(aBackupBGMapRow);  // call BackupBGMapRow
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

void ScrollMapLeft(void){
	SET_PC(0x2892U);
	hlcoord(0, 0, wTilemap);  // hlcoord 0, 0
	LD_DE(wBGMapBuffer);  // ld de, wBGMapBuffer
	CCALL(aBackupBGMapColumn);  // call BackupBGMapColumn
	LD_C(2 * SCREEN_HEIGHT);  // ld c, 2 * SCREEN_HEIGHT
	CALL(mScrollBGMapPalettes);  // call ScrollBGMapPalettes
	LD_A_addr(wBGMapAnchor);  // ld a, [wBGMapAnchor]
	LD_E_A;  // ld e, a
	LD_A_addr(wBGMapAnchor + 1);  // ld a, [wBGMapAnchor + 1]
	LD_D_A;  // ld d, a
	CCALL(aUpdateBGMapColumn);  // call UpdateBGMapColumn
	LD_A(0x1);  // ld a, $1
	LDH_addr_A(hBGMapUpdate);  // ldh [hBGMapUpdate], a
	RET;  // ret

}

void ScrollMapRight(void){
	SET_PC(0x28B0U);
	hlcoord(SCREEN_WIDTH - 2, 0, wTilemap);  // hlcoord SCREEN_WIDTH - 2, 0
	LD_DE(wBGMapBuffer);  // ld de, wBGMapBuffer
	CCALL(aBackupBGMapColumn);  // call BackupBGMapColumn
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
	CCALL(aUpdateBGMapColumn);  // call UpdateBGMapColumn
	LD_A(0x1);  // ld a, $1
	LDH_addr_A(hBGMapUpdate);  // ldh [hBGMapUpdate], a
	RET;  // ret

}

void BackupBGMapRow(void){
	LD_C(2 * SCREEN_WIDTH);  // ld c, 2 * SCREEN_WIDTH

loop:
	LD_A_hli;  // ld a, [hli]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

void BackupBGMapColumn(void){
	LD_C(SCREEN_HEIGHT);  // ld c, SCREEN_HEIGHT

loop:
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
	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

void UpdateBGMapRow(void){
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

void UpdateBGMapColumn(void){
	LD_HL(wBGMapBufferPointers);  // ld hl, wBGMapBufferPointers
	LD_C(SCREEN_HEIGHT);  // ld c, SCREEN_HEIGHT

loop:
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
	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop
	LD_A(SCREEN_HEIGHT);  // ld a, SCREEN_HEIGHT
	LDH_addr_A(hBGMapTileCount);  // ldh [hBGMapTileCount], a
	RET;  // ret

}

void ClearBGMapBuffer(void){
//  //  unreferenced
	LD_HL(wBGMapBuffer);  // ld hl, wBGMapBuffer
	LD_BC(wBGMapBufferEnd - wBGMapBuffer);  // ld bc, wBGMapBufferEnd - wBGMapBuffer
	XOR_A_A;  // xor a
	CCALL(aByteFill);  // call ByteFill
	RET;  // ret

}

void LoadTilesetGFX(void){
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

void BufferScreen(void){
	LD_HL(wOverworldMapAnchor);  // ld hl, wOverworldMapAnchor
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	LD_DE(wScreenSave);  // ld de, wScreenSave
	LD_C(SCREEN_META_HEIGHT);  // ld c, SCREEN_META_HEIGHT
	LD_B(SCREEN_META_WIDTH);  // ld b, SCREEN_META_WIDTH

row:
	PUSH_BC;  // push bc
	PUSH_HL;  // push hl

col:
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

void SaveScreen(void){
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
	LD_DE(wScreenSave + SCREEN_META_WIDTH);  // ld de, wScreenSave + SCREEN_META_WIDTH
	LDH_A_addr(hMapObjectIndex);  // ldh a, [hMapObjectIndex]
	LD_C_A;  // ld c, a
	LD_B(0);  // ld b, 0
	ADD_HL_BC;  // add hl, bc
	goto vertical;  // jr .vertical


down:
	LD_DE(wScreenSave);  // ld de, wScreenSave

vertical:
	LD_B(SCREEN_META_WIDTH);  // ld b, SCREEN_META_WIDTH
	LD_C(SCREEN_META_HEIGHT - 1);  // ld c, SCREEN_META_HEIGHT - 1
	return SaveScreen_LoadConnection();  // jr SaveScreen_LoadConnection


left:
	LD_DE(wScreenSave + 1);  // ld de, wScreenSave + 1
	INC_HL;  // inc hl
	goto horizontal;  // jr .horizontal


right:
	LD_DE(wScreenSave);  // ld de, wScreenSave

horizontal:
	LD_B(SCREEN_META_WIDTH - 1);  // ld b, SCREEN_META_WIDTH - 1
	LD_C(SCREEN_META_HEIGHT);  // ld c, SCREEN_META_HEIGHT
	return SaveScreen_LoadConnection();  // jr SaveScreen_LoadConnection

}

void LoadConnectionBlockData(void){
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

void SaveScreen_LoadConnection(void){

row:
	PUSH_BC;  // push bc
	PUSH_HL;  // push hl
	PUSH_DE;  // push de

col:
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

void GetMovementPermissions(void){
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
	CCALL(aGetCoordTile);  // call GetCoordTile
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
	CCALL(aGetCoordTile);  // call GetCoordTile
	LD_addr_A(wTileDown);  // ld [wTileDown], a
	CALL(mGetMovementPermissions_Down);  // call .Down

	POP_DE;  // pop de
	DEC_E;  // dec e
	CCALL(aGetCoordTile);  // call GetCoordTile
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
	CCALL(aGetCoordTile);  // call GetCoordTile
	LD_addr_A(wTileLeft);  // ld [wTileLeft], a
	CALL(mGetMovementPermissions_Left);  // call .Left

	POP_DE;  // pop de
	INC_D;  // inc d
	CCALL(aGetCoordTile);  // call GetCoordTile
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

void GetFacingTileCoord(void){
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

void GetCoordTile(void){
//  Get the collision byte for tile d, e
	CCALL(aGetBlockLocation);  // call GetBlockLocation
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
	RR_E;  // rr e
	IF_NC goto nocarry2;  // jr nc, .nocarry2
	INC_HL;  // inc hl
	INC_HL;  // inc hl


nocarry2:
	LD_A_addr(wTilesetCollisionBank);  // ld a, [wTilesetCollisionBank]
	CCALL(aGetFarByte);  // call GetFarByte
	RET;  // ret


nope:
	LD_A(-1);  // ld a, -1
	RET;  // ret

}

void GetBlockLocation(void){
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
	SRL_A;  // srl a
	IF_NC goto ok;  // jr nc, .ok
	ADD_HL_BC;  // add hl, bc


ok:
	SLA_C;  // sla c
	RL_B;  // rl b
	AND_A_A;  // and a
	IF_NZ goto loop;  // jr nz, .loop


nope:
	LD_C_D;  // ld c, d
	SRL_C;  // srl c
	LD_B(0);  // ld b, 0
	ADD_HL_BC;  // add hl, bc
	RET;  // ret

}

void CheckFacingBGEvent(void){
	CCALL(aGetFacingTileCoord);  // call GetFacingTileCoord
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
	CCALL(aSwitchToMapScriptsBank);  // call SwitchToMapScriptsBank
	CCALL(aCheckIfFacingTileCoordIsBGEvent);  // call CheckIfFacingTileCoordIsBGEvent
	POP_HL;  // pop hl
	LD_A_H;  // ld a, h
	Bankswitch();  // rst Bankswitch
	RET;  // ret

}

void CheckIfFacingTileCoordIsBGEvent(void){
//  Checks to see if you are facing a BG event.  If so, copies it into wCurBGEvent and sets carry.
	LD_HL(wCurMapBGEventsPointer);  // ld hl, wCurMapBGEventsPointer
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a

loop:
	PUSH_HL;  // push hl
	LD_A_hli;  // ld a, [hli]
	CP_A_E;  // cp e
	IF_NZ goto next;  // jr nz, .next
	LD_A_hli;  // ld a, [hli]
	CP_A_D;  // cp d
	IF_NZ goto next;  // jr nz, .next
	goto copysign;  // jr .copysign


next:
	POP_HL;  // pop hl
	LD_A(BG_EVENT_SIZE);  // ld a, BG_EVENT_SIZE
	ADD_A_L;  // add l
	LD_L_A;  // ld l, a
	IF_NC goto nocarry;  // jr nc, .nocarry
	INC_H;  // inc h


nocarry:
	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop
	XOR_A_A;  // xor a
	RET;  // ret


copysign:
	POP_HL;  // pop hl
	LD_DE(wCurBGEvent);  // ld de, wCurBGEvent
	LD_BC(BG_EVENT_SIZE);  // ld bc, BG_EVENT_SIZE
	CCALL(aCopyBytes);  // call CopyBytes
	SCF;  // scf
	RET;  // ret

}

void CheckCurrentMapCoordEvents(void){
	SET_PC(0x2BB7U);
//  If there are no coord events, we don't need to be here.
	LD_A_addr(wCurMapCoordEventCount);  // ld a, [wCurMapCoordEventCount]
	AND_A_A;  // and a
	RET_Z ;  // ret z
//  Copy the coord event count into c.
	LD_C_A;  // ld c, a
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	CCALL(aSwitchToMapScriptsBank);  // call SwitchToMapScriptsBank
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
	CCALL(aCheckScenes);  // call CheckScenes
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
	CCALL(aCopyBytes);  // call CopyBytes
	SCF;  // scf
	RET;  // ret

}

void FadeToMenu(void){
	SET_PC(0x2C0CU);
	XOR_A_A;  // xor a
	LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a
	CALL(mLoadStandardMenuHeader);  // call LoadStandardMenuHeader
	FARCALL(aFadeOutPalettes);  // farcall FadeOutPalettes
	CCALL(aClearSprites);  // call ClearSprites
	CCALL(aDisableSpriteUpdates);  // call DisableSpriteUpdates
	RET;  // ret

}

void CloseSubmenu(void){
	SET_PC(0x2C1FU);
	CALL(mClearBGPalettes);  // call ClearBGPalettes
	CALL(mReloadTilesetAndPalettes);  // call ReloadTilesetAndPalettes
	CALL(mUpdateSprites);  // call UpdateSprites
	CALL(mCall_ExitMenu);  // call Call_ExitMenu
	CCALL(aReloadPalettes);  // call ReloadPalettes
	JR(mFinishExitMenu);  // jr FinishExitMenu

}

void ExitAllMenus(void){
	SET_PC(0x2C30U);
	CALL(mClearBGPalettes);  // call ClearBGPalettes
	CALL(mCall_ExitMenu);  // call Call_ExitMenu
	CALL(mReloadTilesetAndPalettes);  // call ReloadTilesetAndPalettes
	CALL(mUpdateSprites);  // call UpdateSprites
	CCALL(aReloadPalettes);  // call ReloadPalettes
	return FinishExitMenu();
}

void FinishExitMenu(void){
	SET_PC(0x2C3FU);
	LD_B(SCGB_MAPPALS);  // ld b, SCGB_MAPPALS
	CALL(mGetSGBLayout);  // call GetSGBLayout
	CALL(mWaitBGMap2);  // call WaitBGMap2
	FARCALL(aFadeInPalettes);  // farcall FadeInPalettes
	CCALL(aEnableSpriteUpdates);  // call EnableSpriteUpdates
	RET;  // ret

}

void ReturnToMapWithSpeechTextbox(void){
	SET_PC(0x2C51U);
	PUSH_AF;  // push af
	LD_A(0x1);  // ld a, $1
	LD_addr_A(wSpriteUpdatesEnabled);  // ld [wSpriteUpdatesEnabled], a
	CALL(mClearBGPalettes);  // call ClearBGPalettes
	CCALL(aClearSprites);  // call ClearSprites
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

void ReloadTilesetAndPalettes(void){
	SET_PC(0x2C85U);
	CCALL(aDisableLCD);  // call DisableLCD
	CCALL(aClearSprites);  // call ClearSprites
	FARCALL(av_RefreshSprites);  // farcall _RefreshSprites
	CALL(mLoadStandardFont);  // call LoadStandardFont
	CALL(mLoadFontsExtra);  // call LoadFontsExtra
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A_addr(wMapGroup);  // ld a, [wMapGroup]
	LD_B_A;  // ld b, a
	LD_A_addr(wMapNumber);  // ld a, [wMapNumber]
	LD_C_A;  // ld c, a
	CCALL(aSwitchToAnyMapAttributesBank);  // call SwitchToAnyMapAttributesBank
	FARCALL(aUpdateTimeOfDayPal);  // farcall UpdateTimeOfDayPal
	CALL(mOverworldTextModeSwitch);  // call OverworldTextModeSwitch
	CALL(mLoadTilesetGFX);  // call LoadTilesetGFX
	LD_A(8);  // ld a, 8
	CCALL(aSkipMusic);  // call SkipMusic
	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch

	CCALL(aEnableLCD);  // call EnableLCD
	RET;  // ret

}

void GetMapPointer(void){
	LD_A_addr(wMapGroup);  // ld a, [wMapGroup]
	LD_B_A;  // ld b, a
	LD_A_addr(wMapNumber);  // ld a, [wMapNumber]
	LD_C_A;  // ld c, a
	return GetAnyMapPointer();
}

void GetAnyMapPointer(void){
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
	CCALL(aAddNTimes);  // call AddNTimes
	RET;  // ret

}

void GetMapField(void){
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

void GetAnyMapField(void){
// ; bankswitch
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A(BANK(aMapGroupPointers));  // ld a, BANK(MapGroupPointers)
	Bankswitch();  // rst Bankswitch

	CCALL(aGetAnyMapPointer);  // call GetAnyMapPointer
	ADD_HL_DE;  // add hl, de
	LD_C_hl;  // ld c, [hl]
	INC_HL;  // inc hl
	LD_B_hl;  // ld b, [hl]

// ; bankswitch back
	POP_AF;  // pop af
	Bankswitch();  // rst Bankswitch
	RET;  // ret

}

void SwitchToMapAttributesBank(void){
	LD_A_addr(wMapGroup);  // ld a, [wMapGroup]
	LD_B_A;  // ld b, a
	LD_A_addr(wMapNumber);  // ld a, [wMapNumber]
	LD_C_A;  // ld c, a
	return SwitchToAnyMapAttributesBank();
}

void SwitchToAnyMapAttributesBank(void){
	CCALL(aGetAnyMapAttributesBank);  // call GetAnyMapAttributesBank
	Bankswitch();  // rst Bankswitch
	RET;  // ret

}

void GetMapAttributesBank(void){
//  //  unreferenced
	LD_A_addr(wMapGroup);  // ld a, [wMapGroup]
	LD_B_A;  // ld b, a
	LD_A_addr(wMapNumber);  // ld a, [wMapNumber]
	LD_C_A;  // ld c, a
	return GetAnyMapAttributesBank();
}

void GetAnyMapAttributesBank(void){
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	LD_DE(MAP_MAPATTRIBUTES_BANK);  // ld de, MAP_MAPATTRIBUTES_BANK
	CCALL(aGetAnyMapField);  // call GetAnyMapField
	LD_A_C;  // ld a, c
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

void CopyMapPartial(void){
//  Copy map data bank, tileset, environment, and map data address
//  from the current map's entry within its group.
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A(BANK(aMapGroupPointers));  // ld a, BANK(MapGroupPointers)
	Bankswitch();  // rst Bankswitch

	CCALL(aGetMapPointer);  // call GetMapPointer
	LD_DE(wMapPartial);  // ld de, wMapPartial
	LD_BC(wMapPartialEnd - wMapPartial);  // ld bc, wMapPartialEnd - wMapPartial
	CCALL(aCopyBytes);  // call CopyBytes

	POP_AF;  // pop af
	Bankswitch();  // rst Bankswitch
	RET;  // ret

}

void SwitchToMapScriptsBank(void){
	LD_A_addr(wMapScriptsBank);  // ld a, [wMapScriptsBank]
	Bankswitch();  // rst Bankswitch
	RET;  // ret

}

void GetMapScriptsBank(void){
	LD_A_addr(wMapScriptsBank);  // ld a, [wMapScriptsBank]
	RET;  // ret

}

void GetAnyMapBlocksBank(void){
//  Return the blockdata bank for group b map c.
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc

	PUSH_BC;  // push bc
	LD_DE(MAP_MAPATTRIBUTES);  // ld de, MAP_MAPATTRIBUTES
	CCALL(aGetAnyMapField);  // call GetAnyMapField
	LD_L_C;  // ld l, c
	LD_H_B;  // ld h, b
	POP_BC;  // pop bc

	PUSH_HL;  // push hl
	LD_DE(MAP_MAPATTRIBUTES_BANK);  // ld de, MAP_MAPATTRIBUTES_BANK
	CCALL(aGetAnyMapField);  // call GetAnyMapField
	POP_HL;  // pop hl

	LD_DE(MAP_MAPATTRIBUTES);  // ld de, MAP_MAPATTRIBUTES ; blockdata bank
	ADD_HL_DE;  // add hl, de
	LD_A_C;  // ld a, c
	CCALL(aGetFarByte);  // call GetFarByte
	Bankswitch();  // rst Bankswitch

	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

void GetMapAttributesPointer(void){
//  returns the current map's data pointer in hl.
	PUSH_BC;  // push bc
	PUSH_DE;  // push de
	LD_DE(MAP_MAPATTRIBUTES);  // ld de, MAP_MAPATTRIBUTES
	CCALL(aGetMapField);  // call GetMapField
	LD_L_C;  // ld l, c
	LD_H_B;  // ld h, b
	POP_DE;  // pop de
	POP_BC;  // pop bc
	RET;  // ret

}

void GetMapEnvironment(void){
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	LD_DE(MAP_ENVIRONMENT);  // ld de, MAP_ENVIRONMENT
	CCALL(aGetMapField);  // call GetMapField
	LD_A_C;  // ld a, c
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

void Map_DummyFunction(void){
//  //  unreferenced
	RET;  // ret

}

void GetAnyMapEnvironment(void){
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	LD_DE(MAP_ENVIRONMENT);  // ld de, MAP_ENVIRONMENT
	CCALL(aGetAnyMapField);  // call GetAnyMapField
	LD_A_C;  // ld a, c
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

void GetAnyMapTileset(void){
	LD_DE(MAP_TILESET);  // ld de, MAP_TILESET
	CCALL(aGetAnyMapField);  // call GetAnyMapField
	LD_A_C;  // ld a, c
	RET;  // ret

}

void GetWorldMapLocation(void){
//  given a map group/id in bc, return its location on the Pokégear map.
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc

	LD_DE(MAP_LOCATION);  // ld de, MAP_LOCATION
	CCALL(aGetAnyMapField);  // call GetAnyMapField
	LD_A_C;  // ld a, c

	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

void GetMapMusic(void){
	PUSH_HL;  // push hl
	PUSH_BC;  // push bc
	LD_DE(MAP_MUSIC);  // ld de, MAP_MUSIC
	CCALL(aGetMapField);  // call GetMapField
	LD_A_C;  // ld a, c
	CP_A(MUSIC_MAHOGANY_MART);  // cp MUSIC_MAHOGANY_MART
	IF_Z goto mahoganymart;  // jr z, .mahoganymart
	BIT_C(RADIO_TOWER_MUSIC_F);  // bit RADIO_TOWER_MUSIC_F, c
	IF_NZ goto radiotower;  // jr nz, .radiotower
	LD_E_C;  // ld e, c
	LD_D(0);  // ld d, 0

done:
	POP_BC;  // pop bc
	POP_HL;  // pop hl
	RET;  // ret


radiotower:
	LD_A_addr(wStatusFlags2);  // ld a, [wStatusFlags2]
	BIT_A(STATUSFLAGS2_ROCKETS_IN_RADIO_TOWER_F);  // bit STATUSFLAGS2_ROCKETS_IN_RADIO_TOWER_F, a
	IF_Z goto clearedradiotower;  // jr z, .clearedradiotower
	LD_DE(MUSIC_ROCKET_OVERTURE);  // ld de, MUSIC_ROCKET_OVERTURE
	goto done;  // jr .done


clearedradiotower:
// ; the rest of the byte
	LD_A_C;  // ld a, c
	AND_A(RADIO_TOWER_MUSIC - 1);  // and RADIO_TOWER_MUSIC - 1
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
	goto done;  // jr .done


mahoganymart:
	LD_A_addr(wStatusFlags2);  // ld a, [wStatusFlags2]
	BIT_A(STATUSFLAGS2_ROCKETS_IN_MAHOGANY_F);  // bit STATUSFLAGS2_ROCKETS_IN_MAHOGANY_F, a
	IF_Z goto clearedmahogany;  // jr z, .clearedmahogany
	LD_DE(MUSIC_ROCKET_HIDEOUT);  // ld de, MUSIC_ROCKET_HIDEOUT
	goto done;  // jr .done


clearedmahogany:
	LD_DE(MUSIC_CHERRYGROVE_CITY);  // ld de, MUSIC_CHERRYGROVE_CITY
	goto done;  // jr .done

	return GetMapTimeOfDay();
}

void GetMapTimeOfDay(void){
	CCALL(aGetPhoneServiceTimeOfDayByte);  // call GetPhoneServiceTimeOfDayByte
	AND_A(0xf);  // and $f
	RET;  // ret

}

void GetMapPhoneService(void){
	CCALL(aGetPhoneServiceTimeOfDayByte);  // call GetPhoneServiceTimeOfDayByte
	AND_A(0xf0);  // and $f0
	SWAP_A;  // swap a
	RET;  // ret

}

void GetPhoneServiceTimeOfDayByte(void){
	PUSH_HL;  // push hl
	PUSH_BC;  // push bc

	LD_DE(MAP_PALETTE);  // ld de, MAP_PALETTE
	CCALL(aGetMapField);  // call GetMapField
	LD_A_C;  // ld a, c

	POP_BC;  // pop bc
	POP_HL;  // pop hl
	RET;  // ret

}

void GetFishingGroup(void){
	PUSH_DE;  // push de
	PUSH_HL;  // push hl
	PUSH_BC;  // push bc

	LD_DE(MAP_FISHGROUP);  // ld de, MAP_FISHGROUP
	CCALL(aGetMapField);  // call GetMapField
	LD_A_C;  // ld a, c

	POP_BC;  // pop bc
	POP_HL;  // pop hl
	POP_DE;  // pop de
	RET;  // ret

}

void LoadMapTileset(void){
	PUSH_HL;  // push hl
	PUSH_BC;  // push bc

	LD_HL(mTilesets);  // ld hl, Tilesets
	LD_BC(TILESET_LENGTH);  // ld bc, TILESET_LENGTH
	LD_A_addr(wMapTileset);  // ld a, [wMapTileset]
	CCALL(aAddNTimes);  // call AddNTimes

	LD_DE(wTilesetBank);  // ld de, wTilesetBank
	LD_BC(TILESET_LENGTH);  // ld bc, TILESET_LENGTH

	LD_A(BANK(aTilesets));  // ld a, BANK(Tilesets)
	CCALL(aFarCopyBytes);  // call FarCopyBytes

	POP_BC;  // pop bc
	POP_HL;  // pop hl
	RET;  // ret

}

void DummyEndPredef(void){
//  Unused function at the end of PredefPointers.
	for(int rept = 0; rept < 16; rept++){
	NOP;  // nop
	}
	RET;  // ret

}
