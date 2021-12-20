#include "../macros.h"
#include "../global.h"

#include "../constants/hardware_constants.h"
#include "../constants/serial_constants.h"
#include "../constants/wram_constants.h"


//  Audio interfaces.

int InitSound(struct gb_s *gb){
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	PUSH_AF;  // push af

	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A(BANK(a_InitSound));  // ld a, BANK(_InitSound)
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a

	CALL(a_InitSound);  // call _InitSound

	POP_AF;  // pop af
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a

	POP_AF;  // pop af
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int UpdateSound(struct gb_s *gb){
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	PUSH_AF;  // push af

	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A(BANK(a_UpdateSound));  // ld a, BANK(_UpdateSound)
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a

	CALL(a_UpdateSound);  // call _UpdateSound

	POP_AF;  // pop af
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a

	POP_AF;  // pop af
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int _LoadMusicByte(struct gb_s *gb){
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

int PlayMusic(struct gb_s *gb){
//  Play music de.

	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	PUSH_AF;  // push af

	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A(BANK(a_PlayMusic));  // ld a, BANK(_PlayMusic) ; aka BANK(_InitSound)
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a

	LD_A_E;  // ld a, e
	AND_A_A;  // and a
	IF_Z  goto _nomusic;  // jr z, .nomusic

	CALL(a_PlayMusic);  // call _PlayMusic
	 goto _end;  // jr .end


_nomusic:
	SET_PC(0x3DAB);
	CALL(a_InitSound);  // call _InitSound


_end:
	SET_PC(0x3DAE);
	POP_AF;  // pop af
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a
	POP_AF;  // pop af
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int PlayMusic2(struct gb_s *gb){
//  Stop playing music, then play music de.

	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	PUSH_AF;  // push af

	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A(BANK(a_PlayMusic));  // ld a, BANK(_PlayMusic)
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a

	PUSH_DE;  // push de
	LD_DE(MUSIC_NONE);  // ld de, MUSIC_NONE
	CALL(a_PlayMusic);  // call _PlayMusic
	CALL(aDelayFrame);  // call DelayFrame
	POP_DE;  // pop de
	CALL(a_PlayMusic);  // call _PlayMusic

	POP_AF;  // pop af
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a

	POP_AF;  // pop af
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int PlayCry(struct gb_s *gb){
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

	LD_HL(aPokemonCries);  // ld hl, PokemonCries
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

	LD_A(BANK(a_PlayCry));  // ld a, BANK(_PlayCry)
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a

	CALL(a_PlayCry);  // call _PlayCry

	POP_AF;  // pop af
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a

	POP_AF;  // pop af
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int PlaySFX(struct gb_s *gb){
//  Play sound effect de.
//  Sound effects are ordered by priority (highest to lowest)

	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	PUSH_AF;  // push af

// ; Is something already playing?
	CALL(aCheckSFX);  // call CheckSFX
	IF_NC  goto _play;  // jr nc, .play

// ; Does it have priority?
	LD_A_addr(wCurSFX);  // ld a, [wCurSFX]
	CP_A_E;  // cp e
	IF_C  goto _done;  // jr c, .done


_play:
	SET_PC(0x3E2F);
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A(BANK(a_PlaySFX));  // ld a, BANK(_PlaySFX)
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a

	LD_A_E;  // ld a, e
	LD_addr_A(wCurSFX);  // ld [wCurSFX], a
	CALL(a_PlaySFX);  // call _PlaySFX

	POP_AF;  // pop af
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a


_done:
	SET_PC(0x3E46);
	POP_AF;  // pop af
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int WaitPlaySFX(struct gb_s *gb){
	CALL(aWaitSFX);  // call WaitSFX
	CALL(aPlaySFX);  // call PlaySFX
	RET;  // ret

}

int WaitSFX(struct gb_s *gb){
//  infinite loop until sfx is done playing

	PUSH_HL;  // push hl


_wait:
	SET_PC(0x3E53);
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

int MaxVolume(struct gb_s *gb){
	LD_A(MAX_VOLUME);  // ld a, MAX_VOLUME
	LD_addr_A(wVolume);  // ld [wVolume], a
	RET;  // ret

}

int LowVolume(struct gb_s *gb){
	LD_A(0x33);  // ld a, $33 ; 50%
	LD_addr_A(wVolume);  // ld [wVolume], a
	RET;  // ret

}

int MinVolume(struct gb_s *gb){
	XOR_A_A;  // xor a
	LD_addr_A(wVolume);  // ld [wVolume], a
	RET;  // ret

}

int FadeOutToMusic(struct gb_s *gb){  //  unreferenced
	LD_A(4);  // ld a, 4
	LD_addr_A(wMusicFade);  // ld [wMusicFade], a
	RET;  // ret

}

int FadeInToMusic(struct gb_s *gb){
	LD_A(4 | (1 << MUSIC_FADE_IN_F));  // ld a, 4 | (1 << MUSIC_FADE_IN_F)
	LD_addr_A(wMusicFade);  // ld [wMusicFade], a
	RET;  // ret

}

int SkipMusic(struct gb_s *gb){
//  Skip a frames of music.

_loop:
	SET_PC(0x3E72);
	AND_A_A;  // and a
	RET_Z ;  // ret z
	DEC_A;  // dec a
	CALL(aUpdateSound);  // call UpdateSound
	 goto _loop;  // jr .loop

}

int FadeToMapMusic(struct gb_s *gb){
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	PUSH_AF;  // push af

	CALL(aGetMapMusic_MaybeSpecial);  // call GetMapMusic_MaybeSpecial
	LD_A_addr(wMapMusic);  // ld a, [wMapMusic]
	CP_A_E;  // cp e
	IF_Z  goto _done;  // jr z, .done

	LD_A(8);  // ld a, 8
	LD_addr_A(wMusicFade);  // ld [wMusicFade], a
	LD_A_E;  // ld a, e
	LD_addr_A(wMusicFadeID);  // ld [wMusicFadeID], a
	LD_A_D;  // ld a, d
	LD_addr_A(wMusicFadeID + 1);  // ld [wMusicFadeID + 1], a
	LD_A_E;  // ld a, e
	LD_addr_A(wMapMusic);  // ld [wMapMusic], a


_done:
	SET_PC(0x3E98);
	POP_AF;  // pop af
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int PlayMapMusic(struct gb_s *gb){
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	PUSH_AF;  // push af

	CALL(aGetMapMusic_MaybeSpecial);  // call GetMapMusic_MaybeSpecial
	LD_A_addr(wMapMusic);  // ld a, [wMapMusic]
	CP_A_E;  // cp e
	IF_Z  goto _done;  // jr z, .done

	PUSH_DE;  // push de
	LD_DE(MUSIC_NONE);  // ld de, MUSIC_NONE
	CALL(aPlayMusic);  // call PlayMusic
	CALL(aDelayFrame);  // call DelayFrame
	POP_DE;  // pop de
	LD_A_E;  // ld a, e
	LD_addr_A(wMapMusic);  // ld [wMapMusic], a
	CALL(aPlayMusic);  // call PlayMusic


_done:
	SET_PC(0x3EBC);
	POP_AF;  // pop af
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int PlayMapMusicBike(struct gb_s *gb){
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
	IF_Z  goto _play;  // jr z, .play
	CALL(aGetMapMusic_MaybeSpecial);  // call GetMapMusic_MaybeSpecial

_play:
	SET_PC(0x3ED6);
	PUSH_DE;  // push de
	LD_DE(MUSIC_NONE);  // ld de, MUSIC_NONE
	CALL(aPlayMusic);  // call PlayMusic
	CALL(aDelayFrame);  // call DelayFrame
	POP_DE;  // pop de

	LD_A_E;  // ld a, e
	LD_addr_A(wMapMusic);  // ld [wMapMusic], a
	CALL(aPlayMusic);  // call PlayMusic

	POP_AF;  // pop af
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int TryRestartMapMusic(struct gb_s *gb){
	LD_A_addr(wDontPlayMapMusicOnReload);  // ld a, [wDontPlayMapMusicOnReload]
	AND_A_A;  // and a
	JR_Z (aRestartMapMusic);  // jr z, RestartMapMusic
	XOR_A_A;  // xor a
	LD_addr_A(wMapMusic);  // ld [wMapMusic], a
	LD_DE(MUSIC_NONE);  // ld de, MUSIC_NONE
	CALL(aPlayMusic);  // call PlayMusic
	CALL(aDelayFrame);  // call DelayFrame
	XOR_A_A;  // xor a
	LD_addr_A(wDontPlayMapMusicOnReload);  // ld [wDontPlayMapMusicOnReload], a
	RET;  // ret

}

int RestartMapMusic(struct gb_s *gb){
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	PUSH_AF;  // push af
	LD_DE(MUSIC_NONE);  // ld de, MUSIC_NONE
	CALL(aPlayMusic);  // call PlayMusic
	CALL(aDelayFrame);  // call DelayFrame
	LD_A_addr(wMapMusic);  // ld a, [wMapMusic]
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
	CALL(aPlayMusic);  // call PlayMusic
	POP_AF;  // pop af
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int SpecialMapMusic(struct gb_s *gb){
	LD_A_addr(wPlayerState);  // ld a, [wPlayerState]
	CP_A(PLAYER_SURF);  // cp PLAYER_SURF
	IF_Z  goto _surf;  // jr z, .surf
	CP_A(PLAYER_SURF_PIKA);  // cp PLAYER_SURF_PIKA
	IF_Z  goto _surf;  // jr z, .surf

	LD_A_addr(wStatusFlags2);  // ld a, [wStatusFlags2]
	BIT_A(STATUSFLAGS2_BUG_CONTEST_TIMER_F);  // bit STATUSFLAGS2_BUG_CONTEST_TIMER_F, a
	IF_NZ  goto _contest;  // jr nz, .contest


_no:
	SET_PC(0x3F32);
	AND_A_A;  // and a
	RET;  // ret


_bike:
	SET_PC(0x3F34);  //  unreferenced
	LD_DE(MUSIC_BICYCLE);  // ld de, MUSIC_BICYCLE
	SCF;  // scf
	RET;  // ret


_surf:
	SET_PC(0x3F39);
	LD_DE(MUSIC_SURF);  // ld de, MUSIC_SURF
	SCF;  // scf
	RET;  // ret


_contest:
	SET_PC(0x3F3E);
	LD_A_addr(wMapGroup);  // ld a, [wMapGroup]
	CP_A(GROUP_ROUTE_35_NATIONAL_PARK_GATE);  // cp GROUP_ROUTE_35_NATIONAL_PARK_GATE
	IF_NZ  goto _no;  // jr nz, .no
	LD_A_addr(wMapNumber);  // ld a, [wMapNumber]
	CP_A(MAP_ROUTE_35_NATIONAL_PARK_GATE);  // cp MAP_ROUTE_35_NATIONAL_PARK_GATE
	IF_Z  goto _ranking;  // jr z, .ranking
	CP_A(MAP_ROUTE_36_NATIONAL_PARK_GATE);  // cp MAP_ROUTE_36_NATIONAL_PARK_GATE
	IF_NZ  goto _no;  // jr nz, .no


_ranking:
	SET_PC(0x3F50);
	LD_DE(MUSIC_BUG_CATCHING_CONTEST_RANKING);  // ld de, MUSIC_BUG_CATCHING_CONTEST_RANKING
	SCF;  // scf
	RET;  // ret

}

int GetMapMusic_MaybeSpecial(struct gb_s *gb){
	CALL(aSpecialMapMusic);  // call SpecialMapMusic
	RET_C ;  // ret c
	CALL(aGetMapMusic);  // call GetMapMusic
	RET;  // ret

}

int PlaceBCDNumberSprite(struct gb_s *gb){  //  unreferenced
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
	IF_NC  goto _max;  // jr nc, .max
	ADD_A(1);  // add 1
	DAA;  // daa
	LD_B_A;  // ld b, a
	SWAP_A;  // swap a
	AND_A(0xf);  // and $f
	ADD_A("0");  // add "0"
	LD_addr_A(wVirtualOAMSprite38TileID);  // ld [wVirtualOAMSprite38TileID], a
	LD_A_B;  // ld a, b
	AND_A(0xf);  // and $f
	ADD_A("0");  // add "0"
	LD_addr_A(wVirtualOAMSprite39TileID);  // ld [wVirtualOAMSprite39TileID], a
	RET;  // ret


_max:
	SET_PC(0x3F93);
	LD_A("9");  // ld a, "9"
	LD_addr_A(wVirtualOAMSprite38TileID);  // ld [wVirtualOAMSprite38TileID], a
	LD_addr_A(wVirtualOAMSprite39TileID);  // ld [wVirtualOAMSprite39TileID], a
	RET;  // ret

}

int CheckSFX(struct gb_s *gb){
//  Return carry if any SFX channels are active.
	LD_A_addr(wChannel5Flags1);  // ld a, [wChannel5Flags1]
	BIT_A(0);  // bit 0, a
	IF_NZ  goto _playing;  // jr nz, .playing
	LD_A_addr(wChannel6Flags1);  // ld a, [wChannel6Flags1]
	BIT_A(0);  // bit 0, a
	IF_NZ  goto _playing;  // jr nz, .playing
	LD_A_addr(wChannel7Flags1);  // ld a, [wChannel7Flags1]
	BIT_A(0);  // bit 0, a
	IF_NZ  goto _playing;  // jr nz, .playing
	LD_A_addr(wChannel8Flags1);  // ld a, [wChannel8Flags1]
	BIT_A(0);  // bit 0, a
	IF_NZ  goto _playing;  // jr nz, .playing
	AND_A_A;  // and a
	RET;  // ret

_playing:
	SET_PC(0x3FBA);
	SCF;  // scf
	RET;  // ret

}

int TerminateExpBarSound(struct gb_s *gb){
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
