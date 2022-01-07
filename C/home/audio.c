#include "../constants.h"

//  Audio interfaces.

int InitSound(){
	SET_PC(0x3D4BU);
	v_InitSound();  // call _InitSound
	RET;  // ret

}

int UpdateSound(){
	SET_PC(0x3D67U);
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	PUSH_AF;  // push af

	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A(BANK(av_UpdateSound));  // ld a, BANK(_UpdateSound)
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a

	CALL(mv_UpdateSound);  // call _UpdateSound

	POP_AF;  // pop af
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a

	POP_AF;  // pop af
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int v_LoadMusicByte(){
	SET_PC(0x3D83U);
//  [wCurMusicByte] = [a:de]
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a

	LD_A_de;  // ld a, [de]
	LD_addr_A(wCurMusicByte);  // ld [wCurMusicByte], a
	LD_A(BANK(aLoadMusicByte));  // ld a, BANK(LoadMusicByte)

	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a
	RET;  // ret

}

int PlayMusic(){
	SET_PC(0x3D94U);
//  Play music de.

	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	PUSH_AF;  // push af

	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A(BANK(av_PlayMusic));  // ld a, BANK(_PlayMusic) ; aka BANK(_InitSound)
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a

	LD_A_E;  // ld a, e
	AND_A_A;  // and a
	IF_Z goto nomusic;  // jr z, .nomusic

	CALL(mv_PlayMusic);  // call _PlayMusic
	goto end;  // jr .end


nomusic:
	SET_PC(0x3DABU);
	v_InitSound();  // call _InitSound


end:
	SET_PC(0x3DAEU);
	POP_AF;  // pop af
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a
	POP_AF;  // pop af
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int PlayMusic2(){
	SET_PC(0x3DB9U);
//  Stop playing music, then play music de.

	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	PUSH_AF;  // push af

	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A(BANK(av_PlayMusic));  // ld a, BANK(_PlayMusic)
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a

	PUSH_DE;  // push de
	LD_DE(MUSIC_NONE);  // ld de, MUSIC_NONE
	CALL(mv_PlayMusic);  // call _PlayMusic
	CALL(mDelayFrame);  // call DelayFrame
	POP_DE;  // pop de
	CALL(mv_PlayMusic);  // call _PlayMusic

	POP_AF;  // pop af
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a

	POP_AF;  // pop af
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int PlayCry(){
	SET_PC(0x3DE0U);
//  Play cry de.

	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	PUSH_AF;  // push af

	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af

// ; Cries are stuck in one bank.
	LD_A(BANK(aPokemonCries));  // ld a, BANK(PokemonCries)
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a

	LD_HL(mPokemonCries);  // ld hl, PokemonCries
	for(int rept = 0; rept < MON_CRY_LENGTH; rept++){
	ADD_HL_DE;  // add hl, de
	}

	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]
	INC_HL;  // inc hl

	LD_A_hli;  // ld a, [hli]
	LD_addr_A(wCryPitch);  // ld [wCryPitch], a
	LD_A_hli;  // ld a, [hli]
	LD_addr_A(wCryPitch + 1);  // ld [wCryPitch + 1], a
	LD_A_hli;  // ld a, [hli]
	LD_addr_A(wCryLength);  // ld [wCryLength], a
	LD_A_hl;  // ld a, [hl]
	LD_addr_A(wCryLength + 1);  // ld [wCryLength + 1], a

	LD_A(BANK(av_PlayCry));  // ld a, BANK(_PlayCry)
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a

	CALL(mv_PlayCry);  // call _PlayCry

	POP_AF;  // pop af
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a

	POP_AF;  // pop af
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int PlaySFX(){
	SET_PC(0x3E20U);
//  Play sound effect de.
//  Sound effects are ordered by priority (highest to lowest)

	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	PUSH_AF;  // push af

// ; Is something already playing?
	CALL(mCheckSFX);  // call CheckSFX
	IF_NC goto play;  // jr nc, .play

// ; Does it have priority?
	LD_A_addr(wCurSFX);  // ld a, [wCurSFX]
	CP_A_E;  // cp e
	IF_C goto done;  // jr c, .done


play:
	SET_PC(0x3E2FU);
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A(BANK(av_PlaySFX));  // ld a, BANK(_PlaySFX)
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a

	LD_A_E;  // ld a, e
	LD_addr_A(wCurSFX);  // ld [wCurSFX], a
	CALL(mv_PlaySFX);  // call _PlaySFX

	POP_AF;  // pop af
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a


done:
	SET_PC(0x3E46U);
	POP_AF;  // pop af
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int WaitPlaySFX(){
	SET_PC(0x3E4BU);
	CALL(mWaitSFX);  // call WaitSFX
	CALL(mPlaySFX);  // call PlaySFX
	RET;  // ret

}

int WaitSFX(){
	SET_PC(0x3E52U);
//  infinite loop until sfx is done playing

	PUSH_HL;  // push hl


wait:
	SET_PC(0x3E53U);
// ;(port fix)ld hl, wChannel5Flags1
// ;(port fix)bit 0, [hl]
// ;(port fix)jr nz, .wait
// ;(port fix)ld hl, wChannel6Flags1
// ;(port fix)bit 0, [hl]
// ;(port fix)jr nz, .wait
// ;(port fix)ld hl, wChannel7Flags1
// ;(port fix)bit 0, [hl]
// ;(port fix)jr nz, .wait
// ;(port fix)ld hl, wChannel8Flags1
// ;(port fix)bit 0, [hl]
// ;(port fix)jr nz, .wait

	POP_HL;  // pop hl
	RET;  // ret

}

int MaxVolume(){
	SET_PC(0x3E55U);
	LD_A(MAX_VOLUME);  // ld a, MAX_VOLUME
	LD_addr_A(wVolume);  // ld [wVolume], a
	RET;  // ret

}

int LowVolume(){
	SET_PC(0x3E5BU);
	LD_A(0x33);  // ld a, $33 ; 50%
	LD_addr_A(wVolume);  // ld [wVolume], a
	RET;  // ret

}

int MinVolume(){
	SET_PC(0x3E61U);
	XOR_A_A;  // xor a
	LD_addr_A(wVolume);  // ld [wVolume], a
	RET;  // ret

}

int FadeOutToMusic(){
	SET_PC(0x3E66U);
//  //  unreferenced
	LD_A(4);  // ld a, 4
	LD_addr_A(wMusicFade);  // ld [wMusicFade], a
	RET;  // ret

}

int FadeInToMusic(){
	SET_PC(0x3E6CU);
	LD_A(4 | (1 << MUSIC_FADE_IN_F));  // ld a, 4 | (1 << MUSIC_FADE_IN_F)
	LD_addr_A(wMusicFade);  // ld [wMusicFade], a
	RET;  // ret

}

int SkipMusic(){
	SET_PC(0x3E72U);
//  Skip a frames of music.

loop:
	SET_PC(0x3E72U);
	AND_A_A;  // and a
	RET_Z ;  // ret z
	DEC_A;  // dec a
	CALL(mUpdateSound);  // call UpdateSound
	goto loop;  // jr .loop

}

int FadeToMapMusic(){
	SET_PC(0x3E7AU);
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	PUSH_AF;  // push af

	CALL(mGetMapMusic_MaybeSpecial);  // call GetMapMusic_MaybeSpecial
	LD_A_addr(wMapMusic);  // ld a, [wMapMusic]
	CP_A_E;  // cp e
	IF_Z goto done;  // jr z, .done

	LD_A(8);  // ld a, 8
	LD_addr_A(wMusicFade);  // ld [wMusicFade], a
	LD_A_E;  // ld a, e
	LD_addr_A(wMusicFadeID);  // ld [wMusicFadeID], a
	LD_A_D;  // ld a, d
	LD_addr_A(wMusicFadeID + 1);  // ld [wMusicFadeID + 1], a
	LD_A_E;  // ld a, e
	LD_addr_A(wMapMusic);  // ld [wMapMusic], a


done:
	SET_PC(0x3E98U);
	POP_AF;  // pop af
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int PlayMapMusic(){
	SET_PC(0x3E9DU);
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	PUSH_AF;  // push af

	CALL(mGetMapMusic_MaybeSpecial);  // call GetMapMusic_MaybeSpecial
	LD_A_addr(wMapMusic);  // ld a, [wMapMusic]
	CP_A_E;  // cp e
	IF_Z goto done;  // jr z, .done

	PUSH_DE;  // push de
	LD_DE(MUSIC_NONE);  // ld de, MUSIC_NONE
	CALL(mPlayMusic);  // call PlayMusic
	CALL(mDelayFrame);  // call DelayFrame
	POP_DE;  // pop de
	LD_A_E;  // ld a, e
	LD_addr_A(wMapMusic);  // ld [wMapMusic], a
	CALL(mPlayMusic);  // call PlayMusic


done:
	SET_PC(0x3EBCU);
	POP_AF;  // pop af
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int PlayMapMusicBike(){
	SET_PC(0x3EC1U);
//  If the player's on a bike, play the bike music instead of the map music
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	PUSH_AF;  // push af

	XOR_A_A;  // xor a
	LD_addr_A(wDontPlayMapMusicOnReload);  // ld [wDontPlayMapMusicOnReload], a
	LD_DE(MUSIC_BICYCLE);  // ld de, MUSIC_BICYCLE
	LD_A_addr(wPlayerState);  // ld a, [wPlayerState]
	CP_A(PLAYER_BIKE);  // cp PLAYER_BIKE
	IF_Z goto play;  // jr z, .play
	CALL(mGetMapMusic_MaybeSpecial);  // call GetMapMusic_MaybeSpecial

play:
	SET_PC(0x3ED6U);
	PUSH_DE;  // push de
	LD_DE(MUSIC_NONE);  // ld de, MUSIC_NONE
	CALL(mPlayMusic);  // call PlayMusic
	CALL(mDelayFrame);  // call DelayFrame
	POP_DE;  // pop de

	LD_A_E;  // ld a, e
	LD_addr_A(wMapMusic);  // ld [wMapMusic], a
	CALL(mPlayMusic);  // call PlayMusic

	POP_AF;  // pop af
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int TryRestartMapMusic(){
	SET_PC(0x3EEDU);
	LD_A_addr(wDontPlayMapMusicOnReload);  // ld a, [wDontPlayMapMusicOnReload]
	AND_A_A;  // and a
	JR_Z (mRestartMapMusic);  // jr z, RestartMapMusic
	XOR_A_A;  // xor a
	LD_addr_A(wMapMusic);  // ld [wMapMusic], a
	LD_DE(MUSIC_NONE);  // ld de, MUSIC_NONE
	CALL(mPlayMusic);  // call PlayMusic
	CALL(mDelayFrame);  // call DelayFrame
	XOR_A_A;  // xor a
	LD_addr_A(wDontPlayMapMusicOnReload);  // ld [wDontPlayMapMusicOnReload], a
	RET;  // ret

}

int RestartMapMusic(){
	SET_PC(0x3F05U);
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	PUSH_AF;  // push af
	LD_DE(MUSIC_NONE);  // ld de, MUSIC_NONE
	CALL(mPlayMusic);  // call PlayMusic
	CALL(mDelayFrame);  // call DelayFrame
	LD_A_addr(wMapMusic);  // ld a, [wMapMusic]
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
	CALL(mPlayMusic);  // call PlayMusic
	POP_AF;  // pop af
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int SpecialMapMusic(){
	SET_PC(0x3F20U);
	LD_A_addr(wPlayerState);  // ld a, [wPlayerState]
	CP_A(PLAYER_SURF);  // cp PLAYER_SURF
	IF_Z goto surf;  // jr z, .surf
	CP_A(PLAYER_SURF_PIKA);  // cp PLAYER_SURF_PIKA
	IF_Z goto surf;  // jr z, .surf

	LD_A_addr(wStatusFlags2);  // ld a, [wStatusFlags2]
	BIT_A(STATUSFLAGS2_BUG_CONTEST_TIMER_F);  // bit STATUSFLAGS2_BUG_CONTEST_TIMER_F, a
	IF_NZ goto contest;  // jr nz, .contest


no:
	SET_PC(0x3F32U);
	AND_A_A;  // and a
	RET;  // ret


bike:
// 
	SET_PC(0x3F34U);  //  unreferenced
	LD_DE(MUSIC_BICYCLE);  // ld de, MUSIC_BICYCLE
	SCF;  // scf
	RET;  // ret


surf:
	SET_PC(0x3F39U);
	LD_DE(MUSIC_SURF);  // ld de, MUSIC_SURF
	SCF;  // scf
	RET;  // ret


contest:
	SET_PC(0x3F3EU);
	LD_A_addr(wMapGroup);  // ld a, [wMapGroup]
	CP_A(GROUP_ROUTE_35_NATIONAL_PARK_GATE);  // cp GROUP_ROUTE_35_NATIONAL_PARK_GATE
	IF_NZ goto no;  // jr nz, .no
	LD_A_addr(wMapNumber);  // ld a, [wMapNumber]
	CP_A(MAP_ROUTE_35_NATIONAL_PARK_GATE);  // cp MAP_ROUTE_35_NATIONAL_PARK_GATE
	IF_Z goto ranking;  // jr z, .ranking
	CP_A(MAP_ROUTE_36_NATIONAL_PARK_GATE);  // cp MAP_ROUTE_36_NATIONAL_PARK_GATE
	IF_NZ goto no;  // jr nz, .no


ranking:
	SET_PC(0x3F50U);
	LD_DE(MUSIC_BUG_CATCHING_CONTEST_RANKING);  // ld de, MUSIC_BUG_CATCHING_CONTEST_RANKING
	SCF;  // scf
	RET;  // ret

}

int GetMapMusic_MaybeSpecial(){
	SET_PC(0x3F55U);
	CALL(mSpecialMapMusic);  // call SpecialMapMusic
	RET_C ;  // ret c
	CALL(mGetMapMusic);  // call GetMapMusic
	RET;  // ret

}

int PlaceBCDNumberSprite(){
	SET_PC(0x3F5DU);
//  //  unreferenced
//  Places a BCD number at the upper center of the screen.
	LD_A(4 * TILE_WIDTH);  // ld a, 4 * TILE_WIDTH
	LD_addr_A(wVirtualOAMSprite38YCoord);  // ld [wVirtualOAMSprite38YCoord], a
	LD_addr_A(wVirtualOAMSprite39YCoord);  // ld [wVirtualOAMSprite39YCoord], a
	LD_A(10 * TILE_WIDTH);  // ld a, 10 * TILE_WIDTH
	LD_addr_A(wVirtualOAMSprite38XCoord);  // ld [wVirtualOAMSprite38XCoord], a
	LD_A(11 * TILE_WIDTH);  // ld a, 11 * TILE_WIDTH
	LD_addr_A(wVirtualOAMSprite39XCoord);  // ld [wVirtualOAMSprite39XCoord], a
	XOR_A_A;  // xor a
	LD_addr_A(wVirtualOAMSprite38Attributes);  // ld [wVirtualOAMSprite38Attributes], a
	LD_addr_A(wVirtualOAMSprite39Attributes);  // ld [wVirtualOAMSprite39Attributes], a
	LD_A_addr(wUnusedBCDNumber);  // ld a, [wUnusedBCDNumber]
	CP_A(100);  // cp 100
	IF_NC goto max;  // jr nc, .max
	ADD_A(1);  // add 1
	DAA;  // daa
	LD_B_A;  // ld b, a
	SWAP_A;  // swap a
	AND_A(0xf);  // and $f
	ADD_A(0xf6);  // add "0"
	LD_addr_A(wVirtualOAMSprite38TileID);  // ld [wVirtualOAMSprite38TileID], a
	LD_A_B;  // ld a, b
	AND_A(0xf);  // and $f
	ADD_A(0xf6);  // add "0"
	LD_addr_A(wVirtualOAMSprite39TileID);  // ld [wVirtualOAMSprite39TileID], a
	RET;  // ret


max:
	SET_PC(0x3F93U);
	LD_A(0xff);  // ld a, "9"
	LD_addr_A(wVirtualOAMSprite38TileID);  // ld [wVirtualOAMSprite38TileID], a
	LD_addr_A(wVirtualOAMSprite39TileID);  // ld [wVirtualOAMSprite39TileID], a
	RET;  // ret

}

int CheckSFX(){
	SET_PC(0x3F9CU);
//  Return carry if any SFX channels are active.
	LD_A_addr(wChannel5Flags1);  // ld a, [wChannel5Flags1]
	BIT_A(0);  // bit 0, a
	IF_NZ goto playing;  // jr nz, .playing
	LD_A_addr(wChannel6Flags1);  // ld a, [wChannel6Flags1]
	BIT_A(0);  // bit 0, a
	IF_NZ goto playing;  // jr nz, .playing
	LD_A_addr(wChannel7Flags1);  // ld a, [wChannel7Flags1]
	BIT_A(0);  // bit 0, a
	IF_NZ goto playing;  // jr nz, .playing
	LD_A_addr(wChannel8Flags1);  // ld a, [wChannel8Flags1]
	BIT_A(0);  // bit 0, a
	IF_NZ goto playing;  // jr nz, .playing
	AND_A_A;  // and a
	RET;  // ret

playing:
	SET_PC(0x3FBAU);
	SCF;  // scf
	RET;  // ret

}

int TerminateExpBarSound(){
	SET_PC(0x3FBCU);
	XOR_A_A;  // xor a
	LD_addr_A(wChannel5Flags1);  // ld [wChannel5Flags1], a
	LD_addr_A(wPitchSweep);  // ld [wPitchSweep], a
	LDH_addr_A(rNR10);  // ldh [rNR10], a
	LDH_addr_A(rNR11);  // ldh [rNR11], a
	LDH_addr_A(rNR12);  // ldh [rNR12], a
	LDH_addr_A(rNR13);  // ldh [rNR13], a
	LDH_addr_A(rNR14);  // ldh [rNR14], a
	RET;  // ret

}
