#include "../constants.h"

//  The entire sound engine. Uses section "audio" in WRAM.

//  Interfaces are in bank 0.

//  Notable functions:
//  	FadeMusic
//  	PlayStereoSFX

int v_InitSound(){
	SET_PC(0xE8000U);
//  restart sound operation
//  clear all relevant hardware registers & wram
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	PUSH_AF;  // push af
	CALL(mMusicOff);  // call MusicOff
	LD_HL(rNR50);  // ld hl, rNR50 ; channel control registers
	XOR_A_A;  // xor a
	LD_hli_A;  // ld [hli], a ; rNR50 ; volume/vin
	LD_hli_A;  // ld [hli], a ; rNR51 ; sfx channels
	LD_A(0x80);  // ld a, $80 ; all channels on
	LD_hli_A;  // ld [hli], a ; rNR52 ; music channels

	LD_HL(rNR10);  // ld hl, rNR10 ; sound channel registers
	LD_E(NUM_MUSIC_CHANS);  // ld e, NUM_MUSIC_CHANS

clearsound:
	SET_PC(0xE8015U);
//    sound channel   1      2      3      4
	XOR_A_A;  // xor a
	LD_hli_A;  // ld [hli], a ; rNR10, rNR20, rNR30, rNR40 ; sweep = 0

	LD_hli_A;  // ld [hli], a ; rNR11, rNR21, rNR31, rNR41 ; length/wavepattern = 0
	LD_A(0x8);  // ld a, $8
	LD_hli_A;  // ld [hli], a ; rNR12, rNR22, rNR32, rNR42 ; envelope = 0
	XOR_A_A;  // xor a
	LD_hli_A;  // ld [hli], a ; rNR13, rNR23, rNR33, rNR43 ; frequency lo = 0
	LD_A(0x80);  // ld a, $80
	LD_hli_A;  // ld [hli], a ; rNR14, rNR24, rNR34, rNR44 ; restart sound (freq hi = 0)
	DEC_E;  // dec e
	IF_NZ goto clearsound;  // jr nz, .clearsound

	LD_HL(wAudio);  // ld hl, wAudio
	LD_DE(wAudioEnd - wAudio);  // ld de, wAudioEnd - wAudio

clearaudio:
	SET_PC(0xE8029U);
	XOR_A_A;  // xor a
	LD_hli_A;  // ld [hli], a
	DEC_DE;  // dec de
	LD_A_E;  // ld a, e
	OR_A_D;  // or d
	IF_NZ goto clearaudio;  // jr nz, .clearaudio

	LD_A(MAX_VOLUME);  // ld a, MAX_VOLUME
	LD_addr_A(wVolume);  // ld [wVolume], a
	CALL(mMusicOn);  // call MusicOn
	POP_AF;  // pop af
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int MusicFadeRestart(){
	SET_PC(0xE803DU);
//  restart but keep the music id to fade in to
	LD_A_addr(wMusicFadeID + 1);  // ld a, [wMusicFadeID + 1]
	PUSH_AF;  // push af
	LD_A_addr(wMusicFadeID);  // ld a, [wMusicFadeID]
	PUSH_AF;  // push af
	CALL(mv_InitSound);  // call _InitSound
	POP_AF;  // pop af
	LD_addr_A(wMusicFadeID);  // ld [wMusicFadeID], a
	POP_AF;  // pop af
	LD_addr_A(wMusicFadeID + 1);  // ld [wMusicFadeID + 1], a
	RET;  // ret

}

int MusicOn(){
	SET_PC(0xE8051U);
	LD_A(1);  // ld a, 1
	LD_addr_A(wMusicPlaying);  // ld [wMusicPlaying], a
	RET;  // ret

}

int MusicOff(){
	SET_PC(0xE8057U);
	XOR_A_A;  // xor a
	LD_addr_A(wMusicPlaying);  // ld [wMusicPlaying], a
	RET;  // ret

}

int v_UpdateSound(){
	SET_PC(0xE805CU);
//  called once per frame
// ; no use updating audio if it's not playing
	LD_A_addr(wMusicPlaying);  // ld a, [wMusicPlaying]
	AND_A_A;  // and a
	RET_Z ;  // ret z
// ; start at ch1
	XOR_A_A;  // xor a
	LD_addr_A(wCurChannel);  // ld [wCurChannel], a ; just
	LD_addr_A(wSoundOutput);  // ld [wSoundOutput], a ; off
	LD_BC(wChannel1);  // ld bc, wChannel1

loop:
	SET_PC(0xE806BU);
// ; is the channel active?
	LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
	ADD_HL_BC;  // add hl, bc
	BIT_hl(SOUND_CHANNEL_ON);  // bit SOUND_CHANNEL_ON, [hl]
	JP_Z (mv_UpdateSound_nextchannel);  // jp z, .nextchannel
// ; check time left in the current note
	LD_HL(CHANNEL_NOTE_DURATION);  // ld hl, CHANNEL_NOTE_DURATION
	ADD_HL_BC;  // add hl, bc
	LD_A_hl;  // ld a, [hl]
	CP_A(2);  // cp 2 ; 1 or 0?
	IF_C goto noteover;  // jr c, .noteover
	DEC_hl;  // dec [hl]
	goto continue_sound_update;  // jr .continue_sound_update


noteover:
	SET_PC(0xE8080U);
// ; reset vibrato delay
	LD_HL(CHANNEL_VIBRATO_DELAY);  // ld hl, CHANNEL_VIBRATO_DELAY
	ADD_HL_BC;  // add hl, bc
	LD_A_hl;  // ld a, [hl]
	LD_HL(CHANNEL_VIBRATO_DELAY_COUNT);  // ld hl, CHANNEL_VIBRATO_DELAY_COUNT
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a
// ; turn vibrato off for now
	LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
	ADD_HL_BC;  // add hl, bc
	RES_hl(SOUND_PITCH_SLIDE);  // res SOUND_PITCH_SLIDE, [hl]
// ; get next note
	CALL(mParseMusic);  // call ParseMusic

continue_sound_update:
	SET_PC(0xE8093U);
	CALL(mApplyPitchSlide);  // call ApplyPitchSlide
// ; duty cycle
	LD_HL(CHANNEL_DUTY_CYCLE);  // ld hl, CHANNEL_DUTY_CYCLE
	ADD_HL_BC;  // add hl, bc
	LD_A_hli;  // ld a, [hli]
	LD_addr_A(wCurTrackDuty);  // ld [wCurTrackDuty], a
// ; volume envelope
	LD_A_hli;  // ld a, [hli]
	LD_addr_A(wCurTrackVolumeEnvelope);  // ld [wCurTrackVolumeEnvelope], a
// ; frequency
	LD_A_hli;  // ld a, [hli]
	LD_addr_A(wCurTrackFrequency);  // ld [wCurTrackFrequency], a
	LD_A_hl;  // ld a, [hl]
	LD_addr_A(wCurTrackFrequency + 1);  // ld [wCurTrackFrequency + 1], a
// ; vibrato, noise
	CALL(mHandleTrackVibrato);  // call HandleTrackVibrato ; handle vibrato and other things
	CALL(mHandleNoise);  // call HandleNoise
// ; turn off music when playing sfx?
	LD_A_addr(wSFXPriority);  // ld a, [wSFXPriority]
	AND_A_A;  // and a
	IF_Z goto next;  // jr z, .next
// ; are we in a sfx channel right now?
	LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
	CP_A(NUM_MUSIC_CHANS);  // cp NUM_MUSIC_CHANS
	IF_NC goto next;  // jr nc, .next
// ; are any sfx channels active?
// ; if so, mute
	LD_HL(wChannel5Flags1);  // ld hl, wChannel5Flags1
	BIT_hl(SOUND_CHANNEL_ON);  // bit SOUND_CHANNEL_ON, [hl]
	IF_NZ goto restnote;  // jr nz, .restnote
	LD_HL(wChannel6Flags1);  // ld hl, wChannel6Flags1
	BIT_hl(SOUND_CHANNEL_ON);  // bit SOUND_CHANNEL_ON, [hl]
	IF_NZ goto restnote;  // jr nz, .restnote
	LD_HL(wChannel7Flags1);  // ld hl, wChannel7Flags1
	BIT_hl(SOUND_CHANNEL_ON);  // bit SOUND_CHANNEL_ON, [hl]
	IF_NZ goto restnote;  // jr nz, .restnote
	LD_HL(wChannel8Flags1);  // ld hl, wChannel8Flags1
	BIT_hl(SOUND_CHANNEL_ON);  // bit SOUND_CHANNEL_ON, [hl]
	IF_Z goto next;  // jr z, .next

restnote:
	SET_PC(0xE80D9U);
	LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
	ADD_HL_BC;  // add hl, bc
	SET_hl(NOTE_REST);  // set NOTE_REST, [hl] ; Rest

next:
	SET_PC(0xE80DFU);
// ; are we in a sfx channel right now?
	LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
	CP_A(NUM_MUSIC_CHANS);  // cp NUM_MUSIC_CHANS
	IF_NC goto sfx_channel;  // jr nc, .sfx_channel
	LD_HL(CHANNEL_STRUCT_LENGTH * NUM_MUSIC_CHANS + CHANNEL_FLAGS1);  // ld hl, CHANNEL_STRUCT_LENGTH * NUM_MUSIC_CHANS + CHANNEL_FLAGS1
	ADD_HL_BC;  // add hl, bc
	BIT_hl(SOUND_CHANNEL_ON);  // bit SOUND_CHANNEL_ON, [hl]
	IF_NZ goto sound_channel_on;  // jr nz, .sound_channel_on

sfx_channel:
	SET_PC(0xE80EEU);
	CALL(mUpdateChannels);  // call UpdateChannels
	LD_HL(CHANNEL_TRACKS);  // ld hl, CHANNEL_TRACKS
	ADD_HL_BC;  // add hl, bc
	LD_A_addr(wSoundOutput);  // ld a, [wSoundOutput]
	OR_A_hl;  // or [hl]
	LD_addr_A(wSoundOutput);  // ld [wSoundOutput], a

sound_channel_on:
	SET_PC(0xE80FCU);
// ; clear note flags
	LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
	ADD_HL_BC;  // add hl, bc
	XOR_A_A;  // xor a
	LD_hl_A;  // ld [hl], a

nextchannel:
	SET_PC(0xE8102U);
// ; next channel
	LD_HL(CHANNEL_STRUCT_LENGTH);  // ld hl, CHANNEL_STRUCT_LENGTH
	ADD_HL_BC;  // add hl, bc
	LD_C_L;  // ld c, l
	LD_B_H;  // ld b, h
	LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
	INC_A;  // inc a
	LD_addr_A(wCurChannel);  // ld [wCurChannel], a
	CP_A(NUM_CHANNELS);  // cp NUM_CHANNELS ; are we done?
	JP_NZ (mv_UpdateSound_loop);  // jp nz, .loop ; do it all again

	CALL(mPlayDanger);  // call PlayDanger
// ; fade music in/out
	CALL(mFadeMusic);  // call FadeMusic
// ; write volume to hardware register
	LD_A_addr(wVolume);  // ld a, [wVolume]
	LDH_addr_A(rNR50);  // ldh [rNR50], a
// ; write SO on/off to hardware register
	LD_A_addr(wSoundOutput);  // ld a, [wSoundOutput]
	LDH_addr_A(rNR51);  // ldh [rNR51], a
	RET;  // ret

}

int UpdateChannels(){
	SET_PC(0xE8125U);
	LD_HL(mUpdateChannels_ChannelFunctions);  // ld hl, .ChannelFunctions
	LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
	maskbits(NUM_CHANNELS, 0);  // maskbits NUM_CHANNELS
	ADD_A_A;  // add a
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
	ADD_HL_DE;  // add hl, de
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	JP_hl;  // jp hl


ChannelFunctions:
	SET_PC(0xE8136U);
	//table_width ['2', 'UpdateChannels.ChannelFunctions']  // table_width 2, UpdateChannels.ChannelFunctions
//  music channels
	//dw ['.Channel1'];  // dw .Channel1
	//dw ['.Channel2'];  // dw .Channel2
	//dw ['.Channel3'];  // dw .Channel3
	//dw ['.Channel4'];  // dw .Channel4
	//assert_table_length ['NUM_MUSIC_CHANS']  // assert_table_length NUM_MUSIC_CHANS
//  sfx channels
//  identical to music channels, except .Channel5 is not disabled by the low-HP danger sound
//  (instead, PlayDanger does not play the danger sound if sfx is playing)
	//dw ['.Channel5'];  // dw .Channel5
	//dw ['.Channel6'];  // dw .Channel6
	//dw ['.Channel7'];  // dw .Channel7
	//dw ['.Channel8'];  // dw .Channel8
	//assert_table_length ['NUM_CHANNELS']  // assert_table_length NUM_CHANNELS


Channel1:
	SET_PC(0xE8146U);
	LD_A_addr(wLowHealthAlarm);  // ld a, [wLowHealthAlarm]
	BIT_A(DANGER_ON_F);  // bit DANGER_ON_F, a
	RET_NZ ;  // ret nz

Channel5:
	SET_PC(0xE814CU);
	LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
	ADD_HL_BC;  // add hl, bc
	BIT_hl(NOTE_PITCH_SWEEP);  // bit NOTE_PITCH_SWEEP, [hl]
	IF_Z goto noPitchSweep;  // jr z, .noPitchSweep
// ;
	LD_A_addr(wPitchSweep);  // ld a, [wPitchSweep]
	LDH_addr_A(rNR10);  // ldh [rNR10], a

noPitchSweep:
	SET_PC(0xE8159U);
	BIT_hl(NOTE_REST);  // bit NOTE_REST, [hl] ; rest
	IF_NZ goto ch1_rest;  // jr nz, .ch1_rest
	BIT_hl(NOTE_NOISE_SAMPLING);  // bit NOTE_NOISE_SAMPLING, [hl]
	IF_NZ goto ch1_noise_sampling;  // jr nz, .ch1_noise_sampling
	BIT_hl(NOTE_FREQ_OVERRIDE);  // bit NOTE_FREQ_OVERRIDE, [hl]
	IF_NZ goto ch1_frequency_override;  // jr nz, .ch1_frequency_override
	BIT_hl(NOTE_VIBRATO_OVERRIDE);  // bit NOTE_VIBRATO_OVERRIDE, [hl]
	IF_NZ goto ch1_vibrato_override;  // jr nz, .ch1_vibrato_override
	goto ch1_check_duty_override;  // jr .ch1_check_duty_override


ch1_frequency_override:
	SET_PC(0xE816BU);
	LD_A_addr(wCurTrackFrequency);  // ld a, [wCurTrackFrequency]
	LDH_addr_A(rNR13);  // ldh [rNR13], a
	LD_A_addr(wCurTrackFrequency + 1);  // ld a, [wCurTrackFrequency + 1]
	LDH_addr_A(rNR14);  // ldh [rNR14], a

ch1_check_duty_override:
	SET_PC(0xE8175U);
	BIT_hl(NOTE_DUTY_OVERRIDE);  // bit NOTE_DUTY_OVERRIDE, [hl]
	RET_Z ;  // ret z
	LD_A_addr(wCurTrackDuty);  // ld a, [wCurTrackDuty]
	LD_D_A;  // ld d, a
	LDH_A_addr(rNR11);  // ldh a, [rNR11]
	AND_A(0x3f);  // and $3f ; sound length
	OR_A_D;  // or d
	LDH_addr_A(rNR11);  // ldh [rNR11], a
	RET;  // ret


ch1_vibrato_override:
	SET_PC(0xE8184U);
	LD_A_addr(wCurTrackDuty);  // ld a, [wCurTrackDuty]
	LD_D_A;  // ld d, a
	LDH_A_addr(rNR11);  // ldh a, [rNR11]
	AND_A(0x3f);  // and $3f ; sound length
	OR_A_D;  // or d
	LDH_addr_A(rNR11);  // ldh [rNR11], a
	LD_A_addr(wCurTrackFrequency);  // ld a, [wCurTrackFrequency]
	LDH_addr_A(rNR13);  // ldh [rNR13], a
	RET;  // ret


ch1_rest:
	SET_PC(0xE8195U);
	LDH_A_addr(rNR52);  // ldh a, [rNR52]
	AND_A(0b10001110);  // and %10001110 ; ch1 off
	LDH_addr_A(rNR52);  // ldh [rNR52], a
	LD_HL(rNR10);  // ld hl, rNR10
	CALL(mClearChannel);  // call ClearChannel
	RET;  // ret


ch1_noise_sampling:
	SET_PC(0xE81A2U);
	LD_HL(wCurTrackDuty);  // ld hl, wCurTrackDuty
	LD_A(0x3f);  // ld a, $3f ; sound length
	OR_A_hl;  // or [hl]
	LDH_addr_A(rNR11);  // ldh [rNR11], a
	LD_A_addr(wCurTrackVolumeEnvelope);  // ld a, [wCurTrackVolumeEnvelope]
	LDH_addr_A(rNR12);  // ldh [rNR12], a
	LD_A_addr(wCurTrackFrequency);  // ld a, [wCurTrackFrequency]
	LDH_addr_A(rNR13);  // ldh [rNR13], a
	LD_A_addr(wCurTrackFrequency + 1);  // ld a, [wCurTrackFrequency + 1]
	OR_A(0x80);  // or $80
	LDH_addr_A(rNR14);  // ldh [rNR14], a
	RET;  // ret


Channel2:
	SET_PC(0xE81BCU);

Channel6:
	SET_PC(0xE81BCU);
	LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
	ADD_HL_BC;  // add hl, bc
	BIT_hl(NOTE_REST);  // bit NOTE_REST, [hl] ; rest
	IF_NZ goto ch2_rest;  // jr nz, .ch2_rest
	BIT_hl(NOTE_NOISE_SAMPLING);  // bit NOTE_NOISE_SAMPLING, [hl]
	IF_NZ goto ch2_noise_sampling;  // jr nz, .ch2_noise_sampling
	BIT_hl(NOTE_VIBRATO_OVERRIDE);  // bit NOTE_VIBRATO_OVERRIDE, [hl]
	IF_NZ goto ch2_vibrato_override;  // jr nz, .ch2_vibrato_override
	BIT_hl(NOTE_DUTY_OVERRIDE);  // bit NOTE_DUTY_OVERRIDE, [hl]
	RET_Z ;  // ret z
	LD_A_addr(wCurTrackDuty);  // ld a, [wCurTrackDuty]
	LD_D_A;  // ld d, a
	LDH_A_addr(rNR21);  // ldh a, [rNR21]
	AND_A(0x3f);  // and $3f ; sound length
	OR_A_D;  // or d
	LDH_addr_A(rNR21);  // ldh [rNR21], a
	RET;  // ret


ch2_frequency_override:
// 
	SET_PC(0xE81DBU);  //  unreferenced
	LD_A_addr(wCurTrackFrequency);  // ld a, [wCurTrackFrequency]
	LDH_addr_A(rNR23);  // ldh [rNR23], a
	LD_A_addr(wCurTrackFrequency + 1);  // ld a, [wCurTrackFrequency + 1]
	LDH_addr_A(rNR24);  // ldh [rNR24], a
	RET;  // ret


ch2_vibrato_override:
	SET_PC(0xE81E6U);
	LD_A_addr(wCurTrackDuty);  // ld a, [wCurTrackDuty]
	LD_D_A;  // ld d, a
	LDH_A_addr(rNR21);  // ldh a, [rNR21]
	AND_A(0x3f);  // and $3f ; sound length
	OR_A_D;  // or d
	LDH_addr_A(rNR21);  // ldh [rNR21], a
	LD_A_addr(wCurTrackFrequency);  // ld a, [wCurTrackFrequency]
	LDH_addr_A(rNR23);  // ldh [rNR23], a
	RET;  // ret


ch2_rest:
	SET_PC(0xE81F7U);
	LDH_A_addr(rNR52);  // ldh a, [rNR52]
	AND_A(0b10001101);  // and %10001101 ; ch2 off
	LDH_addr_A(rNR52);  // ldh [rNR52], a
	LD_HL(rNR20);  // ld hl, rNR20
	CALL(mClearChannel);  // call ClearChannel
	RET;  // ret


ch2_noise_sampling:
	SET_PC(0xE8204U);
	LD_HL(wCurTrackDuty);  // ld hl, wCurTrackDuty
	LD_A(0x3f);  // ld a, $3f ; sound length
	OR_A_hl;  // or [hl]
	LDH_addr_A(rNR21);  // ldh [rNR21], a
	LD_A_addr(wCurTrackVolumeEnvelope);  // ld a, [wCurTrackVolumeEnvelope]
	LDH_addr_A(rNR22);  // ldh [rNR22], a
	LD_A_addr(wCurTrackFrequency);  // ld a, [wCurTrackFrequency]
	LDH_addr_A(rNR23);  // ldh [rNR23], a
	LD_A_addr(wCurTrackFrequency + 1);  // ld a, [wCurTrackFrequency + 1]
	OR_A(0x80);  // or $80 ; initial (restart)
	LDH_addr_A(rNR24);  // ldh [rNR24], a
	RET;  // ret


Channel3:
	SET_PC(0xE821EU);

Channel7:
	SET_PC(0xE821EU);
	LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
	ADD_HL_BC;  // add hl, bc
	BIT_hl(NOTE_REST);  // bit NOTE_REST, [hl]
	IF_NZ goto ch3_rest;  // jr nz, .ch3_rest
	BIT_hl(NOTE_NOISE_SAMPLING);  // bit NOTE_NOISE_SAMPLING, [hl]
	IF_NZ goto ch3_noise_sampling;  // jr nz, .ch3_noise_sampling
	BIT_hl(NOTE_VIBRATO_OVERRIDE);  // bit NOTE_VIBRATO_OVERRIDE, [hl]
	IF_NZ goto ch3_vibrato_override;  // jr nz, .ch3_vibrato_override
	RET;  // ret


ch3_frequency_override:
// 
	SET_PC(0xE822FU);  //  unreferenced
	LD_A_addr(wCurTrackFrequency);  // ld a, [wCurTrackFrequency]
	LDH_addr_A(rNR33);  // ldh [rNR33], a
	LD_A_addr(wCurTrackFrequency + 1);  // ld a, [wCurTrackFrequency + 1]
	LDH_addr_A(rNR34);  // ldh [rNR34], a
	RET;  // ret


ch3_vibrato_override:
	SET_PC(0xE823AU);
	LD_A_addr(wCurTrackFrequency);  // ld a, [wCurTrackFrequency]
	LDH_addr_A(rNR33);  // ldh [rNR33], a
	RET;  // ret


ch3_rest:
	SET_PC(0xE8240U);
	LDH_A_addr(rNR52);  // ldh a, [rNR52]
	AND_A(0b10001011);  // and %10001011 ; ch3 off
	LDH_addr_A(rNR52);  // ldh [rNR52], a
	LD_HL(rNR30);  // ld hl, rNR30
	CALL(mClearChannel);  // call ClearChannel
	RET;  // ret


ch3_noise_sampling:
	SET_PC(0xE824DU);
	LD_A(0x3f);  // ld a, $3f ; sound length
	LDH_addr_A(rNR31);  // ldh [rNR31], a
	XOR_A_A;  // xor a
	LDH_addr_A(rNR30);  // ldh [rNR30], a
	CALL(mUpdateChannels_load_wave_pattern);  // call .load_wave_pattern
	LD_A(0x80);  // ld a, $80
	LDH_addr_A(rNR30);  // ldh [rNR30], a
	LD_A_addr(wCurTrackFrequency);  // ld a, [wCurTrackFrequency]
	LDH_addr_A(rNR33);  // ldh [rNR33], a
	LD_A_addr(wCurTrackFrequency + 1);  // ld a, [wCurTrackFrequency + 1]
	OR_A(0x80);  // or $80
	LDH_addr_A(rNR34);  // ldh [rNR34], a
	RET;  // ret


load_wave_pattern:
	SET_PC(0xE8268U);
	PUSH_HL;  // push hl
	LD_A_addr(wCurTrackVolumeEnvelope);  // ld a, [wCurTrackVolumeEnvelope]
	AND_A(0xf);  // and $f ; only 0-9 are valid
	LD_L_A;  // ld l, a
	LD_H(0);  // ld h, 0
// ; hl << 4
// ; each wavepattern is $f bytes long
// ; so seeking is done in $10s
for(int rept = 0; rept < 4; rept++){
	ADD_HL_HL;  // add hl, hl
}
	LD_DE(mWaveSamples);  // ld de, WaveSamples
	ADD_HL_DE;  // add hl, de
// ; load wavepattern into rWave_0-rWave_f
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rWave_0);  // ldh [rWave_0], a
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rWave_1);  // ldh [rWave_1], a
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rWave_2);  // ldh [rWave_2], a
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rWave_3);  // ldh [rWave_3], a
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rWave_4);  // ldh [rWave_4], a
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rWave_5);  // ldh [rWave_5], a
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rWave_6);  // ldh [rWave_6], a
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rWave_7);  // ldh [rWave_7], a
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rWave_8);  // ldh [rWave_8], a
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rWave_9);  // ldh [rWave_9], a
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rWave_a);  // ldh [rWave_a], a
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rWave_b);  // ldh [rWave_b], a
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rWave_c);  // ldh [rWave_c], a
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rWave_d);  // ldh [rWave_d], a
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rWave_e);  // ldh [rWave_e], a
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rWave_f);  // ldh [rWave_f], a
	POP_HL;  // pop hl
	LD_A_addr(wCurTrackVolumeEnvelope);  // ld a, [wCurTrackVolumeEnvelope]
	AND_A(0xf0);  // and $f0
	SLA_A;  // sla a
	LDH_addr_A(rNR32);  // ldh [rNR32], a
	RET;  // ret


Channel4:
	SET_PC(0xE82B4U);

Channel8:
	SET_PC(0xE82B4U);
	LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
	ADD_HL_BC;  // add hl, bc
	BIT_hl(NOTE_REST);  // bit NOTE_REST, [hl]
	IF_NZ goto ch4_rest;  // jr nz, .ch4_rest
	BIT_hl(NOTE_NOISE_SAMPLING);  // bit NOTE_NOISE_SAMPLING, [hl]
	IF_NZ goto ch4_noise_sampling;  // jr nz, .ch4_noise_sampling
	RET;  // ret


ch4_frequency_override:
// 
	SET_PC(0xE82C1U);  //  unreferenced
	LD_A_addr(wCurTrackFrequency);  // ld a, [wCurTrackFrequency]
	LDH_addr_A(rNR43);  // ldh [rNR43], a
	RET;  // ret


ch4_rest:
	SET_PC(0xE82C7U);
	LDH_A_addr(rNR52);  // ldh a, [rNR52]
	AND_A(0b10000111);  // and %10000111 ; ch4 off
	LDH_addr_A(rNR52);  // ldh [rNR52], a
	LD_HL(rNR40);  // ld hl, rNR40
	CALL(mClearChannel);  // call ClearChannel
	RET;  // ret


ch4_noise_sampling:
	SET_PC(0xE82D4U);
	LD_A(0x3f);  // ld a, $3f ; sound length
	LDH_addr_A(rNR41);  // ldh [rNR41], a
	LD_A_addr(wCurTrackVolumeEnvelope);  // ld a, [wCurTrackVolumeEnvelope]
	LDH_addr_A(rNR42);  // ldh [rNR42], a
	LD_A_addr(wCurTrackFrequency);  // ld a, [wCurTrackFrequency]
	LDH_addr_A(rNR43);  // ldh [rNR43], a
	LD_A(0x80);  // ld a, $80
	LDH_addr_A(rNR44);  // ldh [rNR44], a
	RET;  // ret

}

int v_CheckSFX(){
	SET_PC(0xE82E7U);
//  return carry if any sfx channels are active
	LD_HL(wChannel5Flags1);  // ld hl, wChannel5Flags1
	BIT_hl(SOUND_CHANNEL_ON);  // bit SOUND_CHANNEL_ON, [hl]
	IF_NZ goto sfxon;  // jr nz, .sfxon
	LD_HL(wChannel6Flags1);  // ld hl, wChannel6Flags1
	BIT_hl(SOUND_CHANNEL_ON);  // bit SOUND_CHANNEL_ON, [hl]
	IF_NZ goto sfxon;  // jr nz, .sfxon
	LD_HL(wChannel7Flags1);  // ld hl, wChannel7Flags1
	BIT_hl(SOUND_CHANNEL_ON);  // bit SOUND_CHANNEL_ON, [hl]
	IF_NZ goto sfxon;  // jr nz, .sfxon
	LD_HL(wChannel8Flags1);  // ld hl, wChannel8Flags1
	BIT_hl(SOUND_CHANNEL_ON);  // bit SOUND_CHANNEL_ON, [hl]
	IF_NZ goto sfxon;  // jr nz, .sfxon
	AND_A_A;  // and a
	RET;  // ret


sfxon:
	SET_PC(0xE8305U);
	SCF;  // scf
	RET;  // ret

}

int PlayDanger(){
	SET_PC(0xE8307U);
	LD_A_addr(wLowHealthAlarm);  // ld a, [wLowHealthAlarm]
	BIT_A(DANGER_ON_F);  // bit DANGER_ON_F, a
	RET_Z ;  // ret z

// ; Don't do anything if SFX is being played
	AND_A(0xff ^ (1 << DANGER_ON_F));  // and $ff ^ (1 << DANGER_ON_F)
	LD_D_A;  // ld d, a
	CALL(mv_CheckSFX);  // call _CheckSFX
	IF_C goto increment;  // jr c, .increment

// ; Play the high tone
	AND_A_A;  // and a
	IF_Z goto begin;  // jr z, .begin

// ; Play the low tone
	CP_A(16);  // cp 16
	IF_Z goto halfway;  // jr z, .halfway

	goto increment;  // jr .increment


halfway:
	SET_PC(0xE831EU);
	LD_HL(mDangerSoundLow);  // ld hl, DangerSoundLow
	goto applychannel;  // jr .applychannel


begin:
	SET_PC(0xE8323U);
	LD_HL(mDangerSoundHigh);  // ld hl, DangerSoundHigh


applychannel:
	SET_PC(0xE8326U);
	XOR_A_A;  // xor a
	LDH_addr_A(rNR10);  // ldh [rNR10], a
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rNR11);  // ldh [rNR11], a
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rNR12);  // ldh [rNR12], a
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rNR13);  // ldh [rNR13], a
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rNR14);  // ldh [rNR14], a


increment:
	SET_PC(0xE8335U);
	LD_A_D;  // ld a, d
	INC_A;  // inc a
	CP_A(30);  // cp 30 ; Ending frame
	IF_C goto noreset;  // jr c, .noreset
	XOR_A_A;  // xor a

noreset:
	SET_PC(0xE833CU);
// ; Make sure the danger sound is kept on
	OR_A(1 << DANGER_ON_F);  // or 1 << DANGER_ON_F
	LD_addr_A(wLowHealthAlarm);  // ld [wLowHealthAlarm], a

// ; Enable channel 1 if it's off
	LD_A_addr(wSoundOutput);  // ld a, [wSoundOutput]
	AND_A(0x11);  // and $11
	RET_NZ ;  // ret nz
	LD_A_addr(wSoundOutput);  // ld a, [wSoundOutput]
	OR_A(0x11);  // or $11
	LD_addr_A(wSoundOutput);  // ld [wSoundOutput], a
	RET;  // ret

}

int DangerSoundHigh(){
	SET_PC(0xE8350U);
	//db ['0x80'];  // db $80 ; duty 50%
	//db ['0xe2'];  // db $e2 ; volume 14, envelope decrease sweep 2
	//db ['0x50'];  // db $50 ; frequency: $750
	//db ['0x87'];  // db $87 ; restart sound

	return mDangerSoundLow;
}

int DangerSoundLow(){
	SET_PC(0xE8354U);
	//db ['0x80'];  // db $80 ; duty 50%
	//db ['0xe2'];  // db $e2 ; volume 14, envelope decrease sweep 2
	//db ['0xee'];  // db $ee ; frequency: $6ee
	//db ['0x86'];  // db $86 ; restart sound

	return mFadeMusic;
}

int FadeMusic(){
	SET_PC(0xE8358U);
//  fade music if applicable
//  usage:
// 	write to wMusicFade
// 	song fades out at the given rate
// 	load song id in wMusicFadeID
// 	fade new song in
//  notes:
// 	max # frames per volume level is $3f

// ; fading?
	LD_A_addr(wMusicFade);  // ld a, [wMusicFade]
	AND_A_A;  // and a
	RET_Z ;  // ret z
// ; has the count ended?
	LD_A_addr(wMusicFadeCount);  // ld a, [wMusicFadeCount]
	AND_A_A;  // and a
	IF_Z goto update;  // jr z, .update
// ; count down
	DEC_A;  // dec a
	LD_addr_A(wMusicFadeCount);  // ld [wMusicFadeCount], a
	RET;  // ret


update:
	SET_PC(0xE8368U);
	LD_A_addr(wMusicFade);  // ld a, [wMusicFade]
	LD_D_A;  // ld d, a
// ; get new count
	AND_A(0x3f);  // and $3f
	LD_addr_A(wMusicFadeCount);  // ld [wMusicFadeCount], a
// ; get SO1 volume
	LD_A_addr(wVolume);  // ld a, [wVolume]
	AND_A(VOLUME_SO1_LEVEL);  // and VOLUME_SO1_LEVEL
// ; which way are we fading?
	BIT_D(MUSIC_FADE_IN_F);  // bit MUSIC_FADE_IN_F, d
	IF_NZ goto fadein;  // jr nz, .fadein
// ; fading out
	AND_A_A;  // and a
	IF_Z goto novolume;  // jr z, .novolume
	DEC_A;  // dec a
	goto updatevolume;  // jr .updatevolume


novolume:
	SET_PC(0xE8380U);
// ; make sure volume is off
	XOR_A_A;  // xor a
	LD_addr_A(wVolume);  // ld [wVolume], a
// ; did we just get on a bike?
	LD_A_addr(wPlayerState);  // ld a, [wPlayerState]
	CP_A(PLAYER_BIKE);  // cp PLAYER_BIKE
	IF_Z goto bicycle;  // jr z, .bicycle
	PUSH_BC;  // push bc
// ; restart sound
	CALL(mMusicFadeRestart);  // call MusicFadeRestart
// ; get new song id
	LD_A_addr(wMusicFadeID);  // ld a, [wMusicFadeID]
	AND_A_A;  // and a
	IF_Z goto quit;  // jr z, .quit ; this assumes there are fewer than 256 songs!
	LD_E_A;  // ld e, a
	LD_A_addr(wMusicFadeID + 1);  // ld a, [wMusicFadeID + 1]
	LD_D_A;  // ld d, a
// ; load new song
	CALL(mv_PlayMusic);  // call _PlayMusic

quit:
	SET_PC(0xE839DU);
// ; cleanup
	POP_BC;  // pop bc
// ; stop fading
	XOR_A_A;  // xor a
	LD_addr_A(wMusicFade);  // ld [wMusicFade], a
	RET;  // ret


bicycle:
	SET_PC(0xE83A3U);
	PUSH_BC;  // push bc
// ; restart sound
	CALL(mMusicFadeRestart);  // call MusicFadeRestart
// ; this turns the volume up
// ; turn it back down
	XOR_A_A;  // xor a
	LD_addr_A(wVolume);  // ld [wVolume], a
// ; get new song id
	LD_A_addr(wMusicFadeID);  // ld a, [wMusicFadeID]
	LD_E_A;  // ld e, a
	LD_A_addr(wMusicFadeID + 1);  // ld a, [wMusicFadeID + 1]
	LD_D_A;  // ld d, a
// ; load new song
	CALL(mv_PlayMusic);  // call _PlayMusic
	POP_BC;  // pop bc
// ; fade in
	LD_HL(wMusicFade);  // ld hl, wMusicFade
	SET_hl(MUSIC_FADE_IN_F);  // set MUSIC_FADE_IN_F, [hl]
	RET;  // ret


fadein:
	SET_PC(0xE83BDU);
// ; are we done?
	CP_A(MAX_VOLUME & 0xf);  // cp MAX_VOLUME & $f
	IF_NC goto maxvolume;  // jr nc, .maxvolume
// ; inc volume
	INC_A;  // inc a
	goto updatevolume;  // jr .updatevolume


maxvolume:
	SET_PC(0xE83C4U);
// ; we're done
	XOR_A_A;  // xor a
	LD_addr_A(wMusicFade);  // ld [wMusicFade], a
	RET;  // ret


updatevolume:
	SET_PC(0xE83C9U);
// ; hi = lo
	LD_D_A;  // ld d, a
	SWAP_A;  // swap a
	OR_A_D;  // or d
	LD_addr_A(wVolume);  // ld [wVolume], a
	RET;  // ret

}

int LoadNote(){
	SET_PC(0xE83D1U);
// ; wait for pitch slide to finish
	LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
	ADD_HL_BC;  // add hl, bc
	BIT_hl(SOUND_PITCH_SLIDE);  // bit SOUND_PITCH_SLIDE, [hl]
	RET_Z ;  // ret z
// ; get note duration
	LD_HL(CHANNEL_NOTE_DURATION);  // ld hl, CHANNEL_NOTE_DURATION
	ADD_HL_BC;  // add hl, bc
	LD_A_hl;  // ld a, [hl]
	LD_HL(wCurNoteDuration);  // ld hl, wCurNoteDuration
	SUB_A_hl;  // sub [hl]
	IF_NC goto ok;  // jr nc, .ok
	LD_A(1);  // ld a, 1

ok:
	SET_PC(0xE83E5U);
	LD_hl_A;  // ld [hl], a
// ; get frequency
	LD_HL(CHANNEL_FREQUENCY);  // ld hl, CHANNEL_FREQUENCY
	ADD_HL_BC;  // add hl, bc
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]
// ; get direction of pitch slide
	LD_HL(CHANNEL_PITCH_SLIDE_TARGET);  // ld hl, CHANNEL_PITCH_SLIDE_TARGET
	ADD_HL_BC;  // add hl, bc
	LD_A_E;  // ld a, e
	SUB_A_hl;  // sub [hl]
	LD_E_A;  // ld e, a
	LD_A_D;  // ld a, d
	SBC_A(0);  // sbc 0
	LD_D_A;  // ld d, a
	LD_HL(CHANNEL_PITCH_SLIDE_TARGET + 1);  // ld hl, CHANNEL_PITCH_SLIDE_TARGET + 1
	ADD_HL_BC;  // add hl, bc
	SUB_A_hl;  // sub [hl]
	IF_NC goto greater_than;  // jr nc, .greater_than
	LD_HL(CHANNEL_FLAGS3);  // ld hl, CHANNEL_FLAGS3
	ADD_HL_BC;  // add hl, bc
	SET_hl(SOUND_PITCH_SLIDE_DIR);  // set SOUND_PITCH_SLIDE_DIR, [hl]
// ; get frequency
	LD_HL(CHANNEL_FREQUENCY);  // ld hl, CHANNEL_FREQUENCY
	ADD_HL_BC;  // add hl, bc
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]
// ; ????
	LD_HL(CHANNEL_PITCH_SLIDE_TARGET);  // ld hl, CHANNEL_PITCH_SLIDE_TARGET
	ADD_HL_BC;  // add hl, bc
	LD_A_hl;  // ld a, [hl]
	SUB_A_E;  // sub e
	LD_E_A;  // ld e, a
	LD_A_D;  // ld a, d
	SBC_A(0);  // sbc 0
	LD_D_A;  // ld d, a
// ; ????
	LD_HL(CHANNEL_PITCH_SLIDE_TARGET + 1);  // ld hl, CHANNEL_PITCH_SLIDE_TARGET + 1
	ADD_HL_BC;  // add hl, bc
	LD_A_hl;  // ld a, [hl]
	SUB_A_D;  // sub d
	LD_D_A;  // ld d, a
	goto resume;  // jr .resume


greater_than:
	SET_PC(0xE8420U);
	LD_HL(CHANNEL_FLAGS3);  // ld hl, CHANNEL_FLAGS3
	ADD_HL_BC;  // add hl, bc
	RES_hl(SOUND_PITCH_SLIDE_DIR);  // res SOUND_PITCH_SLIDE_DIR, [hl]
// ; get frequency
	LD_HL(CHANNEL_FREQUENCY);  // ld hl, CHANNEL_FREQUENCY
	ADD_HL_BC;  // add hl, bc
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]
// ; get distance from pitch slide target
	LD_HL(CHANNEL_PITCH_SLIDE_TARGET);  // ld hl, CHANNEL_PITCH_SLIDE_TARGET
	ADD_HL_BC;  // add hl, bc
	LD_A_E;  // ld a, e
	SUB_A_hl;  // sub [hl]
	LD_E_A;  // ld e, a
	LD_A_D;  // ld a, d
	SBC_A(0);  // sbc 0
	LD_D_A;  // ld d, a
	LD_HL(CHANNEL_PITCH_SLIDE_TARGET + 1);  // ld hl, CHANNEL_PITCH_SLIDE_TARGET + 1
	ADD_HL_BC;  // add hl, bc
	SUB_A_hl;  // sub [hl]
	LD_D_A;  // ld d, a

resume:
	SET_PC(0xE843EU);
// ; de = x * [wCurNoteDuration] + y
// ; x + 1 -> d
// ; y -> a
	PUSH_BC;  // push bc
	LD_HL(wCurNoteDuration);  // ld hl, wCurNoteDuration
	LD_B(0);  // ld b, 0 ; quotient

loop:
	SET_PC(0xE8444U);
	INC_B;  // inc b
	LD_A_E;  // ld a, e
	SUB_A_hl;  // sub [hl]
	LD_E_A;  // ld e, a
	IF_NC goto loop;  // jr nc, .loop
	LD_A_D;  // ld a, d
	AND_A_A;  // and a
	IF_Z goto quit;  // jr z, .quit
	DEC_D;  // dec d
	goto loop;  // jr .loop


quit:
	SET_PC(0xE8451U);
	LD_A_E;  // ld a, e ; remainder
	ADD_A_hl;  // add [hl]
	LD_D_B;  // ld d, b ; quotient
	POP_BC;  // pop bc
	LD_HL(CHANNEL_PITCH_SLIDE_AMOUNT);  // ld hl, CHANNEL_PITCH_SLIDE_AMOUNT
	ADD_HL_BC;  // add hl, bc
	LD_hl_D;  // ld [hl], d ; quotient
	LD_HL(CHANNEL_PITCH_SLIDE_AMOUNT_FRACTION);  // ld hl, CHANNEL_PITCH_SLIDE_AMOUNT_FRACTION
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a ; remainder
	LD_HL(CHANNEL_FIELD25);  // ld hl, CHANNEL_FIELD25
	ADD_HL_BC;  // add hl, bc
	XOR_A_A;  // xor a
	LD_hl_A;  // ld [hl], a
	RET;  // ret

}

int HandleTrackVibrato(){
	SET_PC(0xE8466U);
//  handle duty, cry pitch, and vibrato
	LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
	ADD_HL_BC;  // add hl, bc
	BIT_hl(SOUND_DUTY_LOOP);  // bit SOUND_DUTY_LOOP, [hl] ; duty cycle looping
	IF_Z goto next;  // jr z, .next
	LD_HL(CHANNEL_DUTY_CYCLE_PATTERN);  // ld hl, CHANNEL_DUTY_CYCLE_PATTERN
	ADD_HL_BC;  // add hl, bc
	LD_A_hl;  // ld a, [hl]
	RLCA;  // rlca
	RLCA;  // rlca
	LD_hl_A;  // ld [hl], a
	AND_A(0xc0);  // and $c0
	LD_addr_A(wCurTrackDuty);  // ld [wCurTrackDuty], a
	LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
	ADD_HL_BC;  // add hl, bc
	SET_hl(NOTE_DUTY_OVERRIDE);  // set NOTE_DUTY_OVERRIDE, [hl]

next:
	SET_PC(0xE8481U);
	LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
	ADD_HL_BC;  // add hl, bc
	BIT_hl(SOUND_PITCH_OFFSET);  // bit SOUND_PITCH_OFFSET, [hl]
	IF_Z goto vibrato;  // jr z, .vibrato
	LD_HL(CHANNEL_PITCH_OFFSET);  // ld hl, CHANNEL_PITCH_OFFSET
	ADD_HL_BC;  // add hl, bc
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]
	LD_HL(wCurTrackFrequency);  // ld hl, wCurTrackFrequency
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	ADD_HL_DE;  // add hl, de
	LD_E_L;  // ld e, l
	LD_D_H;  // ld d, h
	LD_HL(wCurTrackFrequency);  // ld hl, wCurTrackFrequency
	LD_hl_E;  // ld [hl], e
	INC_HL;  // inc hl
	LD_hl_D;  // ld [hl], d

vibrato:
	SET_PC(0xE849FU);
// ; is vibrato on?
	LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
	ADD_HL_BC;  // add hl, bc
	BIT_hl(SOUND_VIBRATO);  // bit SOUND_VIBRATO, [hl] ; vibrato
	IF_Z goto quit;  // jr z, .quit
// ; is vibrato active for this note yet?
// ; is the delay over?
	LD_HL(CHANNEL_VIBRATO_DELAY_COUNT);  // ld hl, CHANNEL_VIBRATO_DELAY_COUNT
	ADD_HL_BC;  // add hl, bc
	LD_A_hl;  // ld a, [hl]
	AND_A_A;  // and a
	IF_NZ goto subexit;  // jr nz, .subexit
// ; is the extent nonzero?
	LD_HL(CHANNEL_VIBRATO_EXTENT);  // ld hl, CHANNEL_VIBRATO_EXTENT
	ADD_HL_BC;  // add hl, bc
	LD_A_hl;  // ld a, [hl]
	AND_A_A;  // and a
	IF_Z goto quit;  // jr z, .quit
// ; save it for later
	LD_D_A;  // ld d, a
// ; is it time to toggle vibrato up/down?
	LD_HL(CHANNEL_VIBRATO_RATE);  // ld hl, CHANNEL_VIBRATO_RATE
	ADD_HL_BC;  // add hl, bc
	LD_A_hl;  // ld a, [hl]
	AND_A(0xf);  // and $f ; count
	IF_Z goto toggle;  // jr z, .toggle

subexit:
	SET_PC(0xE84C1U);
	DEC_hl;  // dec [hl]
	goto quit;  // jr .quit


toggle:
	SET_PC(0xE84C4U);
// ; refresh count
	LD_A_hl;  // ld a, [hl]
	SWAP_hl;  // swap [hl]
	OR_A_hl;  // or [hl]
	LD_hl_A;  // ld [hl], a
// ; ????
	LD_A_addr(wCurTrackFrequency);  // ld a, [wCurTrackFrequency]
	LD_E_A;  // ld e, a
// ; toggle vibrato up/down
	LD_HL(CHANNEL_FLAGS3);  // ld hl, CHANNEL_FLAGS3
	ADD_HL_BC;  // add hl, bc
	BIT_hl(SOUND_VIBRATO_DIR);  // bit SOUND_VIBRATO_DIR, [hl] ; vibrato up/down
	IF_Z goto down;  // jr z, .down
//  up
// ; vibrato down
	RES_hl(SOUND_VIBRATO_DIR);  // res SOUND_VIBRATO_DIR, [hl]
// ; get the delay
	LD_A_D;  // ld a, d
	AND_A(0xf);  // and $f ; lo
// ;
	LD_D_A;  // ld d, a
	LD_A_E;  // ld a, e
	SUB_A_D;  // sub d
	IF_NC goto no_carry;  // jr nc, .no_carry
	LD_A(0);  // ld a, 0
	goto no_carry;  // jr .no_carry


down:
	SET_PC(0xE84E3U);
// ; vibrato up
	SET_hl(SOUND_VIBRATO_DIR);  // set SOUND_VIBRATO_DIR, [hl]
// ; get the delay
	LD_A_D;  // ld a, d
	AND_A(0xf0);  // and $f0 ; hi
	SWAP_A;  // swap a ; move it to lo
// ;
	ADD_A_E;  // add e
	IF_NC goto no_carry;  // jr nc, .no_carry
	LD_A(0xff);  // ld a, $ff

no_carry:
	SET_PC(0xE84EFU);
	LD_addr_A(wCurTrackFrequency);  // ld [wCurTrackFrequency], a
// ;
	LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
	ADD_HL_BC;  // add hl, bc
	SET_hl(NOTE_VIBRATO_OVERRIDE);  // set NOTE_VIBRATO_OVERRIDE, [hl]

quit:
	SET_PC(0xE84F8U);
	RET;  // ret

}

int ApplyPitchSlide(){
	SET_PC(0xE84F9U);
// ; quit if pitch slide inactive
	LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
	ADD_HL_BC;  // add hl, bc
	BIT_hl(SOUND_PITCH_SLIDE);  // bit SOUND_PITCH_SLIDE, [hl]
	RET_Z ;  // ret z
// ; de = Frequency
	LD_HL(CHANNEL_FREQUENCY);  // ld hl, CHANNEL_FREQUENCY
	ADD_HL_BC;  // add hl, bc
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]
// ; check whether pitch slide is going up or down
	LD_HL(CHANNEL_FLAGS3);  // ld hl, CHANNEL_FLAGS3
	ADD_HL_BC;  // add hl, bc
	BIT_hl(SOUND_PITCH_SLIDE_DIR);  // bit SOUND_PITCH_SLIDE_DIR, [hl]
	IF_Z goto decreasing;  // jr z, .decreasing
// ; frequency += [Channel*PitchSlideAmount]
	LD_HL(CHANNEL_PITCH_SLIDE_AMOUNT);  // ld hl, CHANNEL_PITCH_SLIDE_AMOUNT
	ADD_HL_BC;  // add hl, bc
	LD_L_hl;  // ld l, [hl]
	LD_H(0);  // ld h, 0
	ADD_HL_DE;  // add hl, de
	LD_D_H;  // ld d, h
	LD_E_L;  // ld e, l
// ; [Channel*Field25] += [Channel*PitchSlideAmountFraction]
// ; if rollover: Frequency += 1
	LD_HL(CHANNEL_PITCH_SLIDE_AMOUNT_FRACTION);  // ld hl, CHANNEL_PITCH_SLIDE_AMOUNT_FRACTION
	ADD_HL_BC;  // add hl, bc
	LD_A_hl;  // ld a, [hl]
	LD_HL(CHANNEL_FIELD25);  // ld hl, CHANNEL_FIELD25
	ADD_HL_BC;  // add hl, bc
	ADD_A_hl;  // add [hl]
	LD_hl_A;  // ld [hl], a
// ; could have done "jr nc, .no_rollover / inc de / .no_rollover"
	LD_A(0);  // ld a, 0
	ADC_A_E;  // adc e
	LD_E_A;  // ld e, a
	LD_A(0);  // ld a, 0
	ADC_A_D;  // adc d
	LD_D_A;  // ld d, a
// ; Compare the dw at [Channel*PitchSlideTarget] to de.
// ; If frequency is greater, we're finished.
// ; Otherwise, load the frequency and set two flags.
	LD_HL(CHANNEL_PITCH_SLIDE_TARGET + 1);  // ld hl, CHANNEL_PITCH_SLIDE_TARGET + 1
	ADD_HL_BC;  // add hl, bc
	LD_A_hl;  // ld a, [hl]
	CP_A_D;  // cp d
	JP_C (mApplyPitchSlide_finished_pitch_slide);  // jp c, .finished_pitch_slide
	IF_NZ goto continue_pitch_slide;  // jr nz, .continue_pitch_slide
	LD_HL(CHANNEL_PITCH_SLIDE_TARGET);  // ld hl, CHANNEL_PITCH_SLIDE_TARGET
	ADD_HL_BC;  // add hl, bc
	LD_A_hl;  // ld a, [hl]
	CP_A_E;  // cp e
	JP_C (mApplyPitchSlide_finished_pitch_slide);  // jp c, .finished_pitch_slide
	goto continue_pitch_slide;  // jr .continue_pitch_slide


decreasing:
	SET_PC(0xE8542U);
// ; frequency -= [Channel*PitchSlideAmount]
	LD_A_E;  // ld a, e
	LD_HL(CHANNEL_PITCH_SLIDE_AMOUNT);  // ld hl, CHANNEL_PITCH_SLIDE_AMOUNT
	ADD_HL_BC;  // add hl, bc
	LD_E_hl;  // ld e, [hl]
	SUB_A_E;  // sub e
	LD_E_A;  // ld e, a
	LD_A_D;  // ld a, d
	SBC_A(0);  // sbc 0
	LD_D_A;  // ld d, a
// ; [Channel*Field25] *= 2
// ; if rollover: Frequency -= 1
	LD_HL(CHANNEL_PITCH_SLIDE_AMOUNT_FRACTION);  // ld hl, CHANNEL_PITCH_SLIDE_AMOUNT_FRACTION
	ADD_HL_BC;  // add hl, bc
	LD_A_hl;  // ld a, [hl]
	ADD_A_A;  // add a
	LD_hl_A;  // ld [hl], a
// ; could have done "jr nc, .no_rollover / dec de / .no_rollover"
	LD_A_E;  // ld a, e
	SBC_A(0);  // sbc 0
	LD_E_A;  // ld e, a
	LD_A_D;  // ld a, d
	SBC_A(0);  // sbc 0
	LD_D_A;  // ld d, a
// ; Compare the dw at [Channel*PitchSlideTarget] to de.
// ; If frequency is lower, we're finished.
// ; Otherwise, load the frequency and set two flags.
	LD_HL(CHANNEL_PITCH_SLIDE_TARGET + 1);  // ld hl, CHANNEL_PITCH_SLIDE_TARGET + 1
	ADD_HL_BC;  // add hl, bc
	LD_A_D;  // ld a, d
	CP_A_hl;  // cp [hl]
	IF_C goto finished_pitch_slide;  // jr c, .finished_pitch_slide
	IF_NZ goto continue_pitch_slide;  // jr nz, .continue_pitch_slide
	LD_HL(CHANNEL_PITCH_SLIDE_TARGET);  // ld hl, CHANNEL_PITCH_SLIDE_TARGET
	ADD_HL_BC;  // add hl, bc
	LD_A_E;  // ld a, e
	CP_A_hl;  // cp [hl]
	IF_NC goto continue_pitch_slide;  // jr nc, .continue_pitch_slide

finished_pitch_slide:
	SET_PC(0xE856FU);
	LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
	ADD_HL_BC;  // add hl, bc
	RES_hl(SOUND_PITCH_SLIDE);  // res SOUND_PITCH_SLIDE, [hl]
	LD_HL(CHANNEL_FLAGS3);  // ld hl, CHANNEL_FLAGS3
	ADD_HL_BC;  // add hl, bc
	RES_hl(SOUND_PITCH_SLIDE_DIR);  // res SOUND_PITCH_SLIDE_DIR, [hl]
	RET;  // ret


continue_pitch_slide:
	SET_PC(0xE857CU);
	LD_HL(CHANNEL_FREQUENCY);  // ld hl, CHANNEL_FREQUENCY
	ADD_HL_BC;  // add hl, bc
	LD_hl_E;  // ld [hl], e
	INC_HL;  // inc hl
	LD_hl_D;  // ld [hl], d
	LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
	ADD_HL_BC;  // add hl, bc
	SET_hl(NOTE_FREQ_OVERRIDE);  // set NOTE_FREQ_OVERRIDE, [hl]
	SET_hl(NOTE_DUTY_OVERRIDE);  // set NOTE_DUTY_OVERRIDE, [hl]
	RET;  // ret

}

int HandleNoise(){
	SET_PC(0xE858CU);
// ; is noise sampling on?
	LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
	ADD_HL_BC;  // add hl, bc
	BIT_hl(SOUND_NOISE);  // bit SOUND_NOISE, [hl] ; noise sampling
	RET_Z ;  // ret z
// ; are we in a sfx channel?
	LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
	BIT_A(NOISE_CHAN_F);  // bit NOISE_CHAN_F, a
	IF_NZ goto next;  // jr nz, .next
// ; is ch8 on? (noise)
	LD_HL(wChannel8Flags1);  // ld hl, wChannel8Flags1
	BIT_hl(SOUND_CHANNEL_ON);  // bit SOUND_CHANNEL_ON, [hl] ; on?
	IF_Z goto next;  // jr z, .next
// ; is ch8 playing noise?
	BIT_hl(SOUND_NOISE);  // bit SOUND_NOISE, [hl]
	RET_NZ ;  // ret nz ; quit if so
// ;

next:
	SET_PC(0xE85A4U);
	LD_A_addr(wNoiseSampleDelay);  // ld a, [wNoiseSampleDelay]
	AND_A_A;  // and a
	JR_Z (mReadNoiseSample);  // jr z, ReadNoiseSample
	DEC_A;  // dec a
	LD_addr_A(wNoiseSampleDelay);  // ld [wNoiseSampleDelay], a
	RET;  // ret

}

int ReadNoiseSample(){
	SET_PC(0xE85AFU);
//  sample struct:
// 	[wx] [yy] [zz]
// 	w: ? either 2 or 3
// 	x: duration
// 	zz: volume envelope
//        yy: frequency

// ; de = [wNoiseSampleAddress]
	LD_HL(wNoiseSampleAddress);  // ld hl, wNoiseSampleAddress
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]

// ; is it empty?
	LD_A_E;  // ld a, e
	OR_A_D;  // or d
	IF_Z goto quit;  // jr z, .quit

	LD_A_de;  // ld a, [de]
	INC_DE;  // inc de

	CP_A(sound_ret_cmd);  // cp sound_ret_cmd
	IF_Z goto quit;  // jr z, .quit

	AND_A(0xf);  // and $f
	INC_A;  // inc a
	LD_addr_A(wNoiseSampleDelay);  // ld [wNoiseSampleDelay], a
	LD_A_de;  // ld a, [de]
	INC_DE;  // inc de
	LD_addr_A(wCurTrackVolumeEnvelope);  // ld [wCurTrackVolumeEnvelope], a
	LD_A_de;  // ld a, [de]
	INC_DE;  // inc de
	LD_addr_A(wCurTrackFrequency);  // ld [wCurTrackFrequency], a
	XOR_A_A;  // xor a
	LD_addr_A(wCurTrackFrequency + 1);  // ld [wCurTrackFrequency + 1], a

	LD_HL(wNoiseSampleAddress);  // ld hl, wNoiseSampleAddress
	LD_hl_E;  // ld [hl], e
	INC_HL;  // inc hl
	LD_hl_D;  // ld [hl], d

	LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
	ADD_HL_BC;  // add hl, bc
	SET_hl(NOTE_NOISE_SAMPLING);  // set NOTE_NOISE_SAMPLING, [hl]
	RET;  // ret


quit:
	SET_PC(0xE85E0U);
	RET;  // ret

}

int ParseMusic(){
	SET_PC(0xE85E1U);
//  parses until a note is read or the song is ended
	CALL(mGetMusicByte);  // call GetMusicByte ; store next byte in a
	CP_A(sound_ret_cmd);  // cp sound_ret_cmd
	IF_Z goto sound_ret;  // jr z, .sound_ret
	CP_A(FIRST_MUSIC_CMD);  // cp FIRST_MUSIC_CMD
	IF_C goto readnote;  // jr c, .readnote

readcommand:
	SET_PC(0xE85ECU);
	CALL(mParseMusicCommand);  // call ParseMusicCommand
	JR(mParseMusic);  // jr ParseMusic ; start over


readnote:
	SET_PC(0xE85F1U);
//  wCurMusicByte contains current note
//  special notes
	LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
	ADD_HL_BC;  // add hl, bc
	BIT_hl(SOUND_SFX);  // bit SOUND_SFX, [hl]
	JP_NZ (mParseSFXOrCry);  // jp nz, ParseSFXOrCry
	BIT_hl(SOUND_CRY);  // bit SOUND_CRY, [hl]
	JP_NZ (mParseSFXOrCry);  // jp nz, ParseSFXOrCry
	BIT_hl(SOUND_NOISE);  // bit SOUND_NOISE, [hl]
	JP_NZ (mGetNoiseSample);  // jp nz, GetNoiseSample
//  normal note
// ; set note duration (bottom nybble)
	LD_A_addr(wCurMusicByte);  // ld a, [wCurMusicByte]
	AND_A(0xf);  // and $f
	CALL(mSetNoteDuration);  // call SetNoteDuration
// ; get note pitch (top nybble)
	LD_A_addr(wCurMusicByte);  // ld a, [wCurMusicByte]
	SWAP_A;  // swap a
	AND_A(0xf);  // and $f
	IF_Z goto rest;  // jr z, .rest ; pitch 0 -> rest
// ; update pitch
	LD_HL(CHANNEL_PITCH);  // ld hl, CHANNEL_PITCH
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a
// ; store pitch in e
	LD_E_A;  // ld e, a
// ; store octave in d
	LD_HL(CHANNEL_OCTAVE);  // ld hl, CHANNEL_OCTAVE
	ADD_HL_BC;  // add hl, bc
	LD_D_hl;  // ld d, [hl]
// ; update frequency
	CALL(mGetFrequency);  // call GetFrequency
	LD_HL(CHANNEL_FREQUENCY);  // ld hl, CHANNEL_FREQUENCY
	ADD_HL_BC;  // add hl, bc
	LD_hl_E;  // ld [hl], e
	INC_HL;  // inc hl
	LD_hl_D;  // ld [hl], d
// ; ????
	LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
	ADD_HL_BC;  // add hl, bc
	SET_hl(NOTE_NOISE_SAMPLING);  // set NOTE_NOISE_SAMPLING, [hl]
	JP(mLoadNote);  // jp LoadNote


rest:
	SET_PC(0xE8633U);
//  note = rest
	LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
	ADD_HL_BC;  // add hl, bc
	SET_hl(NOTE_REST);  // set NOTE_REST, [hl] ; Rest
	RET;  // ret


sound_ret:
	SET_PC(0xE863AU);
//  $ff is reached in music data
	LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
	ADD_HL_BC;  // add hl, bc
	BIT_hl(SOUND_SUBROUTINE);  // bit SOUND_SUBROUTINE, [hl] ; in a subroutine?
	IF_NZ goto readcommand;  // jr nz, .readcommand ; execute
	LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
	CP_A(CHAN5);  // cp CHAN5
	IF_NC goto chan_5to8;  // jr nc, .chan_5to8
// ; ????
	LD_HL(CHANNEL_STRUCT_LENGTH * NUM_MUSIC_CHANS + CHANNEL_FLAGS1);  // ld hl, CHANNEL_STRUCT_LENGTH * NUM_MUSIC_CHANS + CHANNEL_FLAGS1
	ADD_HL_BC;  // add hl, bc
	BIT_hl(SOUND_CHANNEL_ON);  // bit SOUND_CHANNEL_ON, [hl]
	IF_NZ goto ok;  // jr nz, .ok

chan_5to8:
	SET_PC(0xE8651U);
	LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
	ADD_HL_BC;  // add hl, bc
	BIT_hl(SOUND_CRY);  // bit SOUND_CRY, [hl]
	CALL_NZ (mRestoreVolume);  // call nz, RestoreVolume
// ; end music
	LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
	CP_A(CHAN5);  // cp CHAN5
	IF_NZ goto ok;  // jr nz, .ok
// ; ????
	XOR_A_A;  // xor a
	LDH_addr_A(rNR10);  // ldh [rNR10], a ; sweep = 0

ok:
	SET_PC(0xE8664U);
//  stop playing
// ; turn channel off
	LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
	ADD_HL_BC;  // add hl, bc
	RES_hl(SOUND_CHANNEL_ON);  // res SOUND_CHANNEL_ON, [hl]
// ; note = rest
	LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
	ADD_HL_BC;  // add hl, bc
	SET_hl(NOTE_REST);  // set NOTE_REST, [hl]
// ; clear music id & bank
	LD_HL(CHANNEL_MUSIC_ID);  // ld hl, CHANNEL_MUSIC_ID
	ADD_HL_BC;  // add hl, bc
	XOR_A_A;  // xor a
	LD_hli_A;  // ld [hli], a ; id hi
	LD_hli_A;  // ld [hli], a ; id lo
	LD_hli_A;  // ld [hli], a ; bank
	RET;  // ret

}

int RestoreVolume(){
	SET_PC(0xE8679U);
// ; ch5 only
	LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
	CP_A(CHAN5);  // cp CHAN5
	RET_NZ ;  // ret nz
	XOR_A_A;  // xor a
	LD_HL(wChannel6PitchOffset);  // ld hl, wChannel6PitchOffset
	LD_hli_A;  // ld [hli], a
	LD_hl_A;  // ld [hl], a
	LD_HL(wChannel8PitchOffset);  // ld hl, wChannel8PitchOffset
	LD_hli_A;  // ld [hli], a
	LD_hl_A;  // ld [hl], a
	LD_A_addr(wLastVolume);  // ld a, [wLastVolume]
	LD_addr_A(wVolume);  // ld [wVolume], a
	XOR_A_A;  // xor a
	LD_addr_A(wLastVolume);  // ld [wLastVolume], a
	LD_addr_A(wSFXPriority);  // ld [wSFXPriority], a
	RET;  // ret

}

int ParseSFXOrCry(){
	SET_PC(0xE8698U);
// ; turn noise sampling on
	LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
	ADD_HL_BC;  // add hl, bc
	SET_hl(NOTE_NOISE_SAMPLING);  // set NOTE_NOISE_SAMPLING, [hl] ; noise sample
// ; update note duration
	LD_A_addr(wCurMusicByte);  // ld a, [wCurMusicByte]
	CALL(mSetNoteDuration);  // call SetNoteDuration ; top nybble doesnt matter?
// ; update volume envelope from next param
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_HL(CHANNEL_VOLUME_ENVELOPE);  // ld hl, CHANNEL_VOLUME_ENVELOPE
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a
// ; update lo frequency from next param
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_HL(CHANNEL_FREQUENCY);  // ld hl, CHANNEL_FREQUENCY
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a
// ; are we on the last channel? (noise sampling)
	LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
	maskbits(NUM_MUSIC_CHANS, 0);  // maskbits NUM_MUSIC_CHANS
	CP_A(CHAN4);  // cp CHAN4
	RET_Z ;  // ret z
// ; update hi frequency from next param
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_HL(CHANNEL_FREQUENCY + 1);  // ld hl, CHANNEL_FREQUENCY + 1
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a
	RET;  // ret

}

int GetNoiseSample(){
	SET_PC(0xE86C5U);
//  load ptr to sample header in wNoiseSampleAddress
// ; are we on the last channel?
	LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
	AND_A(NUM_MUSIC_CHANS - 1);  // and NUM_MUSIC_CHANS - 1
	CP_A(CHAN4);  // cp CHAN4
// ; ret if not
	RET_NZ ;  // ret nz
// ; update note duration
	LD_A_addr(wCurMusicByte);  // ld a, [wCurMusicByte]
	AND_A(0xf);  // and $f
	CALL(mSetNoteDuration);  // call SetNoteDuration
// ; check current channel
	LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
	BIT_A(NOISE_CHAN_F);  // bit NOISE_CHAN_F, a
	IF_NZ goto sfx;  // jr nz, .sfx
	LD_HL(wChannel8Flags1);  // ld hl, wChannel8Flags1
	BIT_hl(SOUND_CHANNEL_ON);  // bit SOUND_CHANNEL_ON, [hl] ; is ch8 on? (noise)
	RET_NZ ;  // ret nz
	LD_A_addr(wMusicNoiseSampleSet);  // ld a, [wMusicNoiseSampleSet]
	goto next;  // jr .next


sfx:
	SET_PC(0xE86E7U);
	LD_A_addr(wSFXNoiseSampleSet);  // ld a, [wSFXNoiseSampleSet]

next:
	SET_PC(0xE86EAU);
// ; load noise sample set id into de
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
// ; load ptr to noise sample set in hl
	LD_HL(mDrumkits);  // ld hl, Drumkits
	ADD_HL_DE;  // add hl, de
	ADD_HL_DE;  // add hl, de
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
// ; get pitch
	LD_A_addr(wCurMusicByte);  // ld a, [wCurMusicByte]
	SWAP_A;  // swap a
// ; non-rest note?
	AND_A(0xf);  // and $f
	RET_Z ;  // ret z
// ; use 'pitch' to seek noise sample set
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
	ADD_HL_DE;  // add hl, de
	ADD_HL_DE;  // add hl, de
// ; load sample pointer into wNoiseSampleAddress
	LD_A_hli;  // ld a, [hli]
	LD_addr_A(wNoiseSampleAddress);  // ld [wNoiseSampleAddress], a
	LD_A_hl;  // ld a, [hl]
	LD_addr_A(wNoiseSampleAddress + 1);  // ld [wNoiseSampleAddress + 1], a
// ; clear ????
	XOR_A_A;  // xor a
	LD_addr_A(wNoiseSampleDelay);  // ld [wNoiseSampleDelay], a
	RET;  // ret

}

int ParseMusicCommand(){
	SET_PC(0xE870FU);
// ; reload command
	LD_A_addr(wCurMusicByte);  // ld a, [wCurMusicByte]
// ; get command #
	SUB_A(FIRST_MUSIC_CMD);  // sub FIRST_MUSIC_CMD
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
// ; seek command pointer
	LD_HL(mMusicCommands);  // ld hl, MusicCommands
	ADD_HL_DE;  // add hl, de
	ADD_HL_DE;  // add hl, de
// ; jump to the new pointer
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	JP_hl;  // jp hl

}

int MusicCommands(){
	SET_PC(0xE8720U);
//  entries correspond to audio constants (see macros/scripts/audio.asm)
	//table_width ['2', 'MusicCommands']  // table_width 2, MusicCommands
	//dw ['Music_Octave8'];  // dw Music_Octave8
	//dw ['Music_Octave7'];  // dw Music_Octave7
	//dw ['Music_Octave6'];  // dw Music_Octave6
	//dw ['Music_Octave5'];  // dw Music_Octave5
	//dw ['Music_Octave4'];  // dw Music_Octave4
	//dw ['Music_Octave3'];  // dw Music_Octave3
	//dw ['Music_Octave2'];  // dw Music_Octave2
	//dw ['Music_Octave1'];  // dw Music_Octave1
	//dw ['Music_NoteType'];  // dw Music_NoteType ; note length + volume envelope
	//dw ['Music_Transpose'];  // dw Music_Transpose
	//dw ['Music_Tempo'];  // dw Music_Tempo
	//dw ['Music_DutyCycle'];  // dw Music_DutyCycle
	//dw ['Music_VolumeEnvelope'];  // dw Music_VolumeEnvelope
	//dw ['Music_PitchSweep'];  // dw Music_PitchSweep
	//dw ['Music_DutyCyclePattern'];  // dw Music_DutyCyclePattern
	//dw ['Music_ToggleSFX'];  // dw Music_ToggleSFX
	//dw ['Music_PitchSlide'];  // dw Music_PitchSlide
	//dw ['Music_Vibrato'];  // dw Music_Vibrato
	//dw ['MusicE2'];  // dw MusicE2 ; unused
	//dw ['Music_ToggleNoise'];  // dw Music_ToggleNoise
	//dw ['Music_ForceStereoPanning'];  // dw Music_ForceStereoPanning
	//dw ['Music_Volume'];  // dw Music_Volume
	//dw ['Music_PitchOffset'];  // dw Music_PitchOffset
	//dw ['MusicE7'];  // dw MusicE7 ; unused
	//dw ['MusicE8'];  // dw MusicE8 ; unused
	//dw ['Music_TempoRelative'];  // dw Music_TempoRelative
	//dw ['Music_RestartChannel'];  // dw Music_RestartChannel
	//dw ['Music_NewSong'];  // dw Music_NewSong
	//dw ['Music_SFXPriorityOn'];  // dw Music_SFXPriorityOn
	//dw ['Music_SFXPriorityOff'];  // dw Music_SFXPriorityOff
	//dw ['MusicEE'];  // dw MusicEE ; unused
	//dw ['Music_StereoPanning'];  // dw Music_StereoPanning
	//dw ['Music_SFXToggleNoise'];  // dw Music_SFXToggleNoise
	//dw ['MusicF1'];  // dw MusicF1 ; nothing
	//dw ['MusicF2'];  // dw MusicF2 ; nothing
	//dw ['MusicF3'];  // dw MusicF3 ; nothing
	//dw ['MusicF4'];  // dw MusicF4 ; nothing
	//dw ['MusicF5'];  // dw MusicF5 ; nothing
	//dw ['MusicF6'];  // dw MusicF6 ; nothing
	//dw ['MusicF7'];  // dw MusicF7 ; nothing
	//dw ['MusicF8'];  // dw MusicF8 ; nothing
	//dw ['MusicF9'];  // dw MusicF9 ; unused
	//dw ['Music_SetCondition'];  // dw Music_SetCondition
	//dw ['Music_JumpIf'];  // dw Music_JumpIf
	//dw ['Music_Jump'];  // dw Music_Jump
	//dw ['Music_Loop'];  // dw Music_Loop
	//dw ['Music_Call'];  // dw Music_Call
	//dw ['Music_Ret'];  // dw Music_Ret
	//assert_table_length ['0x100 - FIRST_MUSIC_CMD']  // assert_table_length $100 - FIRST_MUSIC_CMD

	return mMusicF1;
}

int MusicF1(){
	SET_PC(0xE8780U);
	return mMusicF2;
}

int MusicF2(){
	SET_PC(0xE8780U);
	return mMusicF3;
}

int MusicF3(){
	SET_PC(0xE8780U);
	return mMusicF4;
}

int MusicF4(){
	SET_PC(0xE8780U);
	return mMusicF5;
}

int MusicF5(){
	SET_PC(0xE8780U);
	return mMusicF6;
}

int MusicF6(){
	SET_PC(0xE8780U);
	return mMusicF7;
}

int MusicF7(){
	SET_PC(0xE8780U);
	return mMusicF8;
}

int MusicF8(){
	SET_PC(0xE8780U);
	RET;  // ret

}

int Music_Ret(){
	SET_PC(0xE8781U);
//  called when $ff is encountered w/ subroutine flag set
//  end music stream
//  return to caller of the subroutine
// ; reset subroutine flag
	LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
	ADD_HL_BC;  // add hl, bc
	RES_hl(SOUND_SUBROUTINE);  // res SOUND_SUBROUTINE, [hl]
// ; copy LastMusicAddress to MusicAddress
	LD_HL(CHANNEL_LAST_MUSIC_ADDRESS);  // ld hl, CHANNEL_LAST_MUSIC_ADDRESS
	ADD_HL_BC;  // add hl, bc
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]
	LD_HL(CHANNEL_MUSIC_ADDRESS);  // ld hl, CHANNEL_MUSIC_ADDRESS
	ADD_HL_BC;  // add hl, bc
	LD_hl_E;  // ld [hl], e
	INC_HL;  // inc hl
	LD_hl_D;  // ld [hl], d
	RET;  // ret

}

int Music_Call(){
	SET_PC(0xE8796U);
//  call music stream (subroutine)
//  parameters: ll hh
// ; get pointer from next 2 bytes
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_E_A;  // ld e, a
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_D_A;  // ld d, a
	PUSH_DE;  // push de
// ; copy MusicAddress to LastMusicAddress
	LD_HL(CHANNEL_MUSIC_ADDRESS);  // ld hl, CHANNEL_MUSIC_ADDRESS
	ADD_HL_BC;  // add hl, bc
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]
	LD_HL(CHANNEL_LAST_MUSIC_ADDRESS);  // ld hl, CHANNEL_LAST_MUSIC_ADDRESS
	ADD_HL_BC;  // add hl, bc
	LD_hl_E;  // ld [hl], e
	INC_HL;  // inc hl
	LD_hl_D;  // ld [hl], d
// ; load pointer into MusicAddress
	POP_DE;  // pop de
	LD_HL(CHANNEL_MUSIC_ADDRESS);  // ld hl, CHANNEL_MUSIC_ADDRESS
	ADD_HL_BC;  // add hl, bc
	LD_hl_E;  // ld [hl], e
	INC_HL;  // inc hl
	LD_hl_D;  // ld [hl], d
// ; set subroutine flag
	LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
	ADD_HL_BC;  // add hl, bc
	SET_hl(SOUND_SUBROUTINE);  // set SOUND_SUBROUTINE, [hl]
	RET;  // ret

}

int Music_Jump(){
	SET_PC(0xE87BCU);
//  jump
//  parameters: ll hh
// ; get pointer from next 2 bytes
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_E_A;  // ld e, a
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_D_A;  // ld d, a
	LD_HL(CHANNEL_MUSIC_ADDRESS);  // ld hl, CHANNEL_MUSIC_ADDRESS
	ADD_HL_BC;  // add hl, bc
	LD_hl_E;  // ld [hl], e
	INC_HL;  // inc hl
	LD_hl_D;  // ld [hl], d
	RET;  // ret

}

int Music_Loop(){
	SET_PC(0xE87CCU);
//  loops xx - 1 times
//  	00: infinite
//  params: 3
// 	xx ll hh
// 		xx : loop count
//    	ll hh : pointer

// ; get loop count
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
	ADD_HL_BC;  // add hl, bc
	BIT_hl(SOUND_LOOPING);  // bit SOUND_LOOPING, [hl] ; has the loop been initiated?
	IF_NZ goto checkloop;  // jr nz, .checkloop
	AND_A_A;  // and a ; loop counter 0 = infinite
	IF_Z goto loop;  // jr z, .loop
// ; initiate loop
	DEC_A;  // dec a
	SET_hl(SOUND_LOOPING);  // set SOUND_LOOPING, [hl] ; set loop flag
	LD_HL(CHANNEL_LOOP_COUNT);  // ld hl, CHANNEL_LOOP_COUNT
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a ; store loop counter

checkloop:
	SET_PC(0xE87E2U);
	LD_HL(CHANNEL_LOOP_COUNT);  // ld hl, CHANNEL_LOOP_COUNT
	ADD_HL_BC;  // add hl, bc
	LD_A_hl;  // ld a, [hl]
	AND_A_A;  // and a ; are we done?
	IF_Z goto endloop;  // jr z, .endloop
	DEC_hl;  // dec [hl]

loop:
	SET_PC(0xE87EBU);
// ; get pointer
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_E_A;  // ld e, a
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_D_A;  // ld d, a
// ; load new pointer into MusicAddress
	LD_HL(CHANNEL_MUSIC_ADDRESS);  // ld hl, CHANNEL_MUSIC_ADDRESS
	ADD_HL_BC;  // add hl, bc
	LD_hl_E;  // ld [hl], e
	INC_HL;  // inc hl
	LD_hl_D;  // ld [hl], d
	RET;  // ret


endloop:
	SET_PC(0xE87FBU);
// ; reset loop flag
	LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
	ADD_HL_BC;  // add hl, bc
	RES_hl(SOUND_LOOPING);  // res SOUND_LOOPING, [hl]
// ; skip to next command
	LD_HL(CHANNEL_MUSIC_ADDRESS);  // ld hl, CHANNEL_MUSIC_ADDRESS
	ADD_HL_BC;  // add hl, bc
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]
	INC_DE;  // inc de ; skip
	INC_DE;  // inc de ; pointer
	LD_hl_D;  // ld [hl], d
	DEC_HL;  // dec hl
	LD_hl_E;  // ld [hl], e
	RET;  // ret

}

int Music_SetCondition(){
	SET_PC(0xE880EU);
//  set condition for a jump
//  used with FB
//  params: 1
// 	xx

// ; set condition
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_HL(CHANNEL_CONDITION);  // ld hl, CHANNEL_CONDITION
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a
	RET;  // ret

}

int Music_JumpIf(){
	SET_PC(0xE8817U);
//  conditional jump
//  used with FA
//  params: 3
//  	xx: condition
// 	ll hh: pointer

//  check condition
// ; a = condition
	CALL(mGetMusicByte);  // call GetMusicByte
// ; if existing condition matches, jump to new address
	LD_HL(CHANNEL_CONDITION);  // ld hl, CHANNEL_CONDITION
	ADD_HL_BC;  // add hl, bc
	CP_A_hl;  // cp [hl]
	IF_Z goto jump;  // jr z, .jump
//  skip to next command
// ; get address
	LD_HL(CHANNEL_MUSIC_ADDRESS);  // ld hl, CHANNEL_MUSIC_ADDRESS
	ADD_HL_BC;  // add hl, bc
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]
// ; skip pointer
	INC_DE;  // inc de
	INC_DE;  // inc de
// ; update address
	LD_hl_D;  // ld [hl], d
	DEC_HL;  // dec hl
	LD_hl_E;  // ld [hl], e
	RET;  // ret


jump:
	SET_PC(0xE882EU);
//  jump to the new address
// ; get pointer
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_E_A;  // ld e, a
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_D_A;  // ld d, a
// ; update pointer in MusicAddress
	LD_HL(CHANNEL_MUSIC_ADDRESS);  // ld hl, CHANNEL_MUSIC_ADDRESS
	ADD_HL_BC;  // add hl, bc
	LD_hl_E;  // ld [hl], e
	INC_HL;  // inc hl
	LD_hl_D;  // ld [hl], d
	RET;  // ret

}

int MusicEE(){
	SET_PC(0xE883EU);
//  unused
//  conditional jump
//  checks a byte in ram corresponding to the current channel
//  params: 2
// 		ll hh

//  if ????, jump
// ; get channel
	LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
	maskbits(NUM_MUSIC_CHANS, 0);  // maskbits NUM_MUSIC_CHANS
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
// ; hl = wChannel1JumpCondition + channel id
	LD_HL(wChannel1JumpCondition);  // ld hl, wChannel1JumpCondition
	ADD_HL_DE;  // add hl, de
// ; if set, jump
	LD_A_hl;  // ld a, [hl]
	AND_A_A;  // and a
	IF_NZ goto jump;  // jr nz, .jump
//  skip to next command
// ; get address
	LD_HL(CHANNEL_MUSIC_ADDRESS);  // ld hl, CHANNEL_MUSIC_ADDRESS
	ADD_HL_BC;  // add hl, bc
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]
// ; skip pointer
	INC_DE;  // inc de
	INC_DE;  // inc de
// ; update address
	LD_hl_D;  // ld [hl], d
	DEC_HL;  // dec hl
	LD_hl_E;  // ld [hl], e
	RET;  // ret


jump:
	SET_PC(0xE885BU);
// ; reset jump flag
	LD_hl(0);  // ld [hl], 0
// ; de = pointer
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_E_A;  // ld e, a
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_D_A;  // ld d, a
// ; update address
	LD_HL(CHANNEL_MUSIC_ADDRESS);  // ld hl, CHANNEL_MUSIC_ADDRESS
	ADD_HL_BC;  // add hl, bc
	LD_hl_E;  // ld [hl], e
	INC_HL;  // inc hl
	LD_hl_D;  // ld [hl], d
	RET;  // ret

}

int MusicF9(){
	SET_PC(0xE886DU);
//  unused
//  sets some flag
//  params: 0
	LD_A(TRUE);  // ld a, TRUE
	LD_addr_A(wUnusedMusicF9Flag);  // ld [wUnusedMusicF9Flag], a
	RET;  // ret

}

int MusicE2(){
	SET_PC(0xE8873U);
//  unused
//  params: 1
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_HL(CHANNEL_FIELD2C);  // ld hl, CHANNEL_FIELD2C
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a
	LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
	ADD_HL_BC;  // add hl, bc
	SET_hl(SOUND_UNKN_0B);  // set SOUND_UNKN_0B, [hl]
	RET;  // ret

}

int Music_Vibrato(){
	SET_PC(0xE8882U);
//  vibrato
//  params: 2
// 	1: [xx]
// ; delay in frames
// 	2: [yz]
// ; y: extent
// ; z: rate (# frames per cycle)

// ; set vibrato flag?
	LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
	ADD_HL_BC;  // add hl, bc
	SET_hl(SOUND_VIBRATO);  // set SOUND_VIBRATO, [hl]
// ; start at lower frequency (extent is positive)
	LD_HL(CHANNEL_FLAGS3);  // ld hl, CHANNEL_FLAGS3
	ADD_HL_BC;  // add hl, bc
	RES_hl(SOUND_VIBRATO_DIR);  // res SOUND_VIBRATO_DIR, [hl]
// ; get delay
	CALL(mGetMusicByte);  // call GetMusicByte
//  update delay
	LD_HL(CHANNEL_VIBRATO_DELAY);  // ld hl, CHANNEL_VIBRATO_DELAY
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a
//  update delay count
	LD_HL(CHANNEL_VIBRATO_DELAY_COUNT);  // ld hl, CHANNEL_VIBRATO_DELAY_COUNT
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a
//  update extent
//  this is split into halves only to get added back together at the last second
// ; get extent/rate
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_HL(CHANNEL_VIBRATO_EXTENT);  // ld hl, CHANNEL_VIBRATO_EXTENT
	ADD_HL_BC;  // add hl, bc
	LD_D_A;  // ld d, a
// ; get top nybble
	AND_A(0xf0);  // and $f0
	SWAP_A;  // swap a
	SRL_A;  // srl a ; halve
	LD_E_A;  // ld e, a
	ADC_A(0);  // adc 0 ; round up
	SWAP_A;  // swap a
	OR_A_E;  // or e
	LD_hl_A;  // ld [hl], a
//  update rate
	LD_HL(CHANNEL_VIBRATO_RATE);  // ld hl, CHANNEL_VIBRATO_RATE
	ADD_HL_BC;  // add hl, bc
// ; get bottom nybble
	LD_A_D;  // ld a, d
	AND_A(0xf);  // and $f
	LD_D_A;  // ld d, a
	SWAP_A;  // swap a
	OR_A_D;  // or d
	LD_hl_A;  // ld [hl], a
	RET;  // ret

}

int Music_PitchSlide(){
	SET_PC(0xE88BDU);
//  set the target for pitch slide
//  params: 2
//  note duration
//  target note
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_addr_A(wCurNoteDuration);  // ld [wCurNoteDuration], a

	CALL(mGetMusicByte);  // call GetMusicByte
// ; pitch in e
	LD_D_A;  // ld d, a
	AND_A(0xf);  // and $f
	LD_E_A;  // ld e, a

// ; octave in d
	LD_A_D;  // ld a, d
	SWAP_A;  // swap a
	AND_A(0xf);  // and $f
	LD_D_A;  // ld d, a
	CALL(mGetFrequency);  // call GetFrequency
	LD_HL(CHANNEL_PITCH_SLIDE_TARGET);  // ld hl, CHANNEL_PITCH_SLIDE_TARGET
	ADD_HL_BC;  // add hl, bc
	LD_hl_E;  // ld [hl], e
	LD_HL(CHANNEL_PITCH_SLIDE_TARGET + 1);  // ld hl, CHANNEL_PITCH_SLIDE_TARGET + 1
	ADD_HL_BC;  // add hl, bc
	LD_hl_D;  // ld [hl], d
	LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
	ADD_HL_BC;  // add hl, bc
	SET_hl(SOUND_PITCH_SLIDE);  // set SOUND_PITCH_SLIDE, [hl]
	RET;  // ret

}

int Music_PitchOffset(){
	SET_PC(0xE88E4U);
//  tone
//  params: 1 (dw)
//  offset to add to each note frequency
	LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
	ADD_HL_BC;  // add hl, bc
	SET_hl(SOUND_PITCH_OFFSET);  // set SOUND_PITCH_OFFSET, [hl]
	LD_HL(CHANNEL_PITCH_OFFSET + 1);  // ld hl, CHANNEL_PITCH_OFFSET + 1
	ADD_HL_BC;  // add hl, bc
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_hld_A;  // ld [hld], a
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_hl_A;  // ld [hl], a
	RET;  // ret

}

int MusicE7(){
	SET_PC(0xE88F7U);
//  unused
//  params: 1
	LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
	ADD_HL_BC;  // add hl, bc
	SET_hl(SOUND_UNKN_0E);  // set SOUND_UNKN_0E, [hl]
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_HL(CHANNEL_FIELD29);  // ld hl, CHANNEL_FIELD29
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a
	RET;  // ret

}

int Music_DutyCyclePattern(){
	SET_PC(0xE8906U);
//  sequence of 4 duty cycles to be looped
//  params: 1 (4 2-bit duty cycle arguments)
	LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
	ADD_HL_BC;  // add hl, bc
	SET_hl(SOUND_DUTY_LOOP);  // set SOUND_DUTY_LOOP, [hl] ; duty cycle looping
// ; sound duty sequence
	CALL(mGetMusicByte);  // call GetMusicByte
	RRCA;  // rrca
	RRCA;  // rrca
	LD_HL(CHANNEL_DUTY_CYCLE_PATTERN);  // ld hl, CHANNEL_DUTY_CYCLE_PATTERN
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a
// ; update duty cycle
	AND_A(0xc0);  // and $c0 ; only uses top 2 bits
	LD_HL(CHANNEL_DUTY_CYCLE);  // ld hl, CHANNEL_DUTY_CYCLE
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a
	RET;  // ret

}

int MusicE8(){
	SET_PC(0xE891EU);
//  unused
//  params: 1
	LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
	ADD_HL_BC;  // add hl, bc
	SET_hl(SOUND_UNKN_0D);  // set SOUND_UNKN_0D, [hl]
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_HL(CHANNEL_FIELD2A);  // ld hl, CHANNEL_FIELD2A
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a
	RET;  // ret

}

int Music_ToggleSFX(){
	SET_PC(0xE892DU);
//  toggle something
//  params: none
	LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
	ADD_HL_BC;  // add hl, bc
	BIT_hl(SOUND_SFX);  // bit SOUND_SFX, [hl]
	IF_Z goto on;  // jr z, .on
	RES_hl(SOUND_SFX);  // res SOUND_SFX, [hl]
	RET;  // ret


on:
	SET_PC(0xE8938U);
	SET_hl(SOUND_SFX);  // set SOUND_SFX, [hl]
	RET;  // ret

}

int Music_ToggleNoise(){
	SET_PC(0xE893BU);
//  toggle music noise sampling
//  can't be used as a straight toggle since the param is not read from on->off
//  params:
//  	noise on: 1
//  	noise off: 0
// ; check if noise sampling is on
	LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
	ADD_HL_BC;  // add hl, bc
	BIT_hl(SOUND_NOISE);  // bit SOUND_NOISE, [hl]
	IF_Z goto on;  // jr z, .on
// ; turn noise sampling off
	RES_hl(SOUND_NOISE);  // res SOUND_NOISE, [hl]
	RET;  // ret


on:
	SET_PC(0xE8946U);
// ; turn noise sampling on
	SET_hl(SOUND_NOISE);  // set SOUND_NOISE, [hl]
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_addr_A(wMusicNoiseSampleSet);  // ld [wMusicNoiseSampleSet], a
	RET;  // ret

}

int Music_SFXToggleNoise(){
	SET_PC(0xE894FU);
//  toggle sfx noise sampling
//  params:
// 	on: 1
//  	off: 0
// ; check if noise sampling is on
	LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
	ADD_HL_BC;  // add hl, bc
	BIT_hl(SOUND_NOISE);  // bit SOUND_NOISE, [hl]
	IF_Z goto on;  // jr z, .on
// ; turn noise sampling off
	RES_hl(SOUND_NOISE);  // res SOUND_NOISE, [hl]
	RET;  // ret


on:
	SET_PC(0xE895AU);
// ; turn noise sampling on
	SET_hl(SOUND_NOISE);  // set SOUND_NOISE, [hl]
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_addr_A(wSFXNoiseSampleSet);  // ld [wSFXNoiseSampleSet], a
	RET;  // ret

}

int Music_NoteType(){
	SET_PC(0xE8963U);
//  note length
// 	# frames per 16th note
//  volume envelope: see Music_VolumeEnvelope
//  params: 2
// ; note length
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_HL(CHANNEL_NOTE_LENGTH);  // ld hl, CHANNEL_NOTE_LENGTH
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a
	LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
	maskbits(NUM_MUSIC_CHANS, 0);  // maskbits NUM_MUSIC_CHANS
	CP_A(CHAN4);  // cp CHAN4
	RET_Z ;  // ret z
// ; volume envelope
	CALL(mMusic_VolumeEnvelope);  // call Music_VolumeEnvelope
	RET;  // ret

}

int Music_PitchSweep(){
	SET_PC(0xE8977U);
//  update pitch sweep
//  params: 1
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_addr_A(wPitchSweep);  // ld [wPitchSweep], a
	LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
	ADD_HL_BC;  // add hl, bc
	SET_hl(NOTE_PITCH_SWEEP);  // set NOTE_PITCH_SWEEP, [hl]
	RET;  // ret

}

int Music_DutyCycle(){
	SET_PC(0xE8984U);
//  duty cycle
//  params: 1
	CALL(mGetMusicByte);  // call GetMusicByte
	RRCA;  // rrca
	RRCA;  // rrca
	AND_A(0xc0);  // and $c0
	LD_HL(CHANNEL_DUTY_CYCLE);  // ld hl, CHANNEL_DUTY_CYCLE
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a
	RET;  // ret

}

int Music_VolumeEnvelope(){
	SET_PC(0xE8991U);
//  volume envelope
//  params: 1
// 	hi: volume
//    lo: fade
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_HL(CHANNEL_VOLUME_ENVELOPE);  // ld hl, CHANNEL_VOLUME_ENVELOPE
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a
	RET;  // ret

}

int Music_Tempo(){
	SET_PC(0xE899AU);
//  global tempo
//  params: 2
// 	de: tempo
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_D_A;  // ld d, a
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_E_A;  // ld e, a
	CALL(mSetGlobalTempo);  // call SetGlobalTempo
	RET;  // ret

}

int Music_Octave8(){
	SET_PC(0xE89A6U);
	return mMusic_Octave7;
}

int Music_Octave7(){
	SET_PC(0xE89A6U);
	return mMusic_Octave6;
}

int Music_Octave6(){
	SET_PC(0xE89A6U);
	return mMusic_Octave5;
}

int Music_Octave5(){
	SET_PC(0xE89A6U);
	return mMusic_Octave4;
}

int Music_Octave4(){
	SET_PC(0xE89A6U);
	return mMusic_Octave3;
}

int Music_Octave3(){
	SET_PC(0xE89A6U);
	return mMusic_Octave2;
}

int Music_Octave2(){
	SET_PC(0xE89A6U);
	return mMusic_Octave1;
}

int Music_Octave1(){
	SET_PC(0xE89A6U);
//  set octave based on lo nybble of the command
	LD_HL(CHANNEL_OCTAVE);  // ld hl, CHANNEL_OCTAVE
	ADD_HL_BC;  // add hl, bc
	LD_A_addr(wCurMusicByte);  // ld a, [wCurMusicByte]
	AND_A(7);  // and 7
	LD_hl_A;  // ld [hl], a
	RET;  // ret

}

int Music_Transpose(){
	SET_PC(0xE89B1U);
//  set starting octave
//  this forces all notes up by the starting octave
//  params: 1
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_HL(CHANNEL_TRANSPOSITION);  // ld hl, CHANNEL_TRANSPOSITION
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a
	RET;  // ret

}

int Music_StereoPanning(){
	SET_PC(0xE89BAU);
//  stereo panning
//  params: 1
// ; stereo on?
	LD_A_addr(wOptions);  // ld a, [wOptions]
	BIT_A(STEREO);  // bit STEREO, a
	JR_NZ (mMusic_ForceStereoPanning);  // jr nz, Music_ForceStereoPanning
// ; skip param
	CALL(mGetMusicByte);  // call GetMusicByte
	RET;  // ret

}

int Music_ForceStereoPanning(){
	SET_PC(0xE89C5U);
//  force panning
//  params: 1
	CALL(mSetLRTracks);  // call SetLRTracks
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_HL(CHANNEL_TRACKS);  // ld hl, CHANNEL_TRACKS
	ADD_HL_BC;  // add hl, bc
	AND_A_hl;  // and [hl]
	LD_hl_A;  // ld [hl], a
	RET;  // ret

}

int Music_Volume(){
	SET_PC(0xE89D2U);
//  set volume
//  params: 1
// 	see Volume
// ; read param even if it's not used
	CALL(mGetMusicByte);  // call GetMusicByte
// ; is the song fading?
	LD_A_addr(wMusicFade);  // ld a, [wMusicFade]
	AND_A_A;  // and a
	RET_NZ ;  // ret nz
// ; reload param
	LD_A_addr(wCurMusicByte);  // ld a, [wCurMusicByte]
// ; set volume
	LD_addr_A(wVolume);  // ld [wVolume], a
	RET;  // ret

}

int Music_TempoRelative(){
	SET_PC(0xE89E1U);
//  set global tempo to current channel tempo +/- param
//  params: 1 signed
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_E_A;  // ld e, a
// ; check sign
	CP_A(0x80);  // cp $80
	IF_NC goto negative;  // jr nc, .negative
// positive
	LD_D(0);  // ld d, 0
	goto ok;  // jr .ok


negative:
	SET_PC(0xE89EDU);
	LD_D(-1);  // ld d, -1

ok:
	SET_PC(0xE89EFU);
	LD_HL(CHANNEL_TEMPO);  // ld hl, CHANNEL_TEMPO
	ADD_HL_BC;  // add hl, bc
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	ADD_HL_DE;  // add hl, de
	LD_E_L;  // ld e, l
	LD_D_H;  // ld d, h
	CALL(mSetGlobalTempo);  // call SetGlobalTempo
	RET;  // ret

}

int Music_SFXPriorityOn(){
	SET_PC(0xE89FDU);
//  turn sfx priority on
//  params: none
	LD_A(1);  // ld a, 1
	LD_addr_A(wSFXPriority);  // ld [wSFXPriority], a
	RET;  // ret

}

int Music_SFXPriorityOff(){
	SET_PC(0xE8A03U);
//  turn sfx priority off
//  params: none
	XOR_A_A;  // xor a
	LD_addr_A(wSFXPriority);  // ld [wSFXPriority], a
	RET;  // ret

}

int Music_RestartChannel(){
	SET_PC(0xE8A08U);
//  restart current channel from channel header (same bank)
//  params: 2 (5)
//  ll hh: pointer to new channel header
// 	header format: 0x yy zz
// 		x: channel # (0-3)
// 		zzyy: pointer to new music data

// ; update music id
	LD_HL(CHANNEL_MUSIC_ID);  // ld hl, CHANNEL_MUSIC_ID
	ADD_HL_BC;  // add hl, bc
	LD_A_hli;  // ld a, [hli]
	LD_addr_A(wMusicID);  // ld [wMusicID], a
	LD_A_hl;  // ld a, [hl]
	LD_addr_A(wMusicID + 1);  // ld [wMusicID + 1], a
// ; update music bank
	LD_HL(CHANNEL_MUSIC_BANK);  // ld hl, CHANNEL_MUSIC_BANK
	ADD_HL_BC;  // add hl, bc
	LD_A_hl;  // ld a, [hl]
	LD_addr_A(wMusicBank);  // ld [wMusicBank], a
// ; get pointer to new channel header
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_L_A;  // ld l, a
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_H_A;  // ld h, a
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]
	PUSH_BC;  // push bc ; save current channel
	CALL(mLoadChannel);  // call LoadChannel
	CALL(mStartChannel);  // call StartChannel
	POP_BC;  // pop bc ; restore current channel
	RET;  // ret

}

int Music_NewSong(){
	SET_PC(0xE8A30U);
//  new song
//  params: 2
// 	de: song id
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_E_A;  // ld e, a
	CALL(mGetMusicByte);  // call GetMusicByte
	LD_D_A;  // ld d, a
	PUSH_BC;  // push bc
	CALL(mv_PlayMusic);  // call _PlayMusic
	POP_BC;  // pop bc
	RET;  // ret

}

int GetMusicByte(){
	SET_PC(0xE8A3EU);
//  returns byte from current address in a
//  advances to next byte in music data
//  input: bc = start of current channel
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	LD_HL(CHANNEL_MUSIC_ADDRESS);  // ld hl, CHANNEL_MUSIC_ADDRESS
	ADD_HL_BC;  // add hl, bc
	LD_A_hli;  // ld a, [hli]
	LD_E_A;  // ld e, a
	LD_D_hl;  // ld d, [hl]
	LD_HL(CHANNEL_MUSIC_BANK);  // ld hl, CHANNEL_MUSIC_BANK
	ADD_HL_BC;  // add hl, bc
	LD_A_hl;  // ld a, [hl]
	CALL(mv_LoadMusicByte);  // call _LoadMusicByte ; load data into [wCurMusicByte]
	INC_DE;  // inc de ; advance to next byte for next time this is called
	LD_HL(CHANNEL_MUSIC_ADDRESS);  // ld hl, CHANNEL_MUSIC_ADDRESS
	ADD_HL_BC;  // add hl, bc
	LD_A_E;  // ld a, e
	LD_hli_A;  // ld [hli], a
	LD_hl_D;  // ld [hl], d
	POP_DE;  // pop de
	POP_HL;  // pop hl
	LD_A_addr(wCurMusicByte);  // ld a, [wCurMusicByte]
	RET;  // ret

}

int GetFrequency(){
	SET_PC(0xE8A5DU);
//  generate frequency
//  input:
//  	d: octave
// 	e: pitch
//  output:
//  	de: frequency

//  get octave
// ; get starting octave
	LD_HL(CHANNEL_TRANSPOSITION);  // ld hl, CHANNEL_TRANSPOSITION
	ADD_HL_BC;  // add hl, bc
	LD_A_hl;  // ld a, [hl]
	SWAP_A;  // swap a ; hi nybble
	AND_A(0xf);  // and $f
// ; add current octave
	ADD_A_D;  // add d
	PUSH_AF;  // push af ; we'll use this later
// ; get starting octave
	LD_HL(CHANNEL_TRANSPOSITION);  // ld hl, CHANNEL_TRANSPOSITION
	ADD_HL_BC;  // add hl, bc
	LD_A_hl;  // ld a, [hl]
	AND_A(0xf);  // and $f ; lo nybble
	LD_L_A;  // ld l, a ; ok
	LD_D(0);  // ld d, 0
	LD_H_D;  // ld h, d
	ADD_HL_DE;  // add hl, de ; add current pitch
	ADD_HL_HL;  // add hl, hl ; skip 2 bytes for each
	LD_DE(mFrequencyTable);  // ld de, FrequencyTable
	ADD_HL_DE;  // add hl, de
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]
// ; get our octave
	POP_AF;  // pop af
// ; shift right by [7 - octave] bits

loop:
	SET_PC(0xE8A7DU);
// ; [7 - octave] loops
	CP_A(0x7);  // cp $7
	IF_NC goto ok;  // jr nc, .ok
// ; sra de
	SRA_D;  // sra d
	RR_E;  // rr e
	INC_A;  // inc a
	goto loop;  // jr .loop


ok:
	SET_PC(0xE8A88U);
	LD_A_D;  // ld a, d
	AND_A(0x7);  // and $7 ; top 3 bits for frequency (11 total)
	LD_D_A;  // ld d, a
	RET;  // ret

}

int SetNoteDuration(){
	SET_PC(0xE8A8DU);
//  input: a = note duration in 16ths
// ; store delay units in de
	INC_A;  // inc a
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
	LD_HL(CHANNEL_NOTE_LENGTH);  // ld hl, CHANNEL_NOTE_LENGTH
	ADD_HL_BC;  // add hl, bc
	LD_A_hl;  // ld a, [hl]
// ; multiply NoteLength by delay units
	LD_L(0);  // ld l, 0 ; just multiply
	CALL(mSetNoteDuration_Multiply);  // call .Multiply
	LD_A_L;  // ld a, l ; low
// ; store Tempo in de
	LD_HL(CHANNEL_TEMPO);  // ld hl, CHANNEL_TEMPO
	ADD_HL_BC;  // add hl, bc
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]
// ; add ???? to the next result
	LD_HL(CHANNEL_FIELD16);  // ld hl, CHANNEL_FIELD16
	ADD_HL_BC;  // add hl, bc
	LD_L_hl;  // ld l, [hl]
// ; multiply Tempo by last result (NoteLength * LOW(delay))
	CALL(mSetNoteDuration_Multiply);  // call .Multiply
// ; copy result to de
	LD_E_L;  // ld e, l
	LD_D_H;  // ld d, h
// ; store result in ????
	LD_HL(CHANNEL_FIELD16);  // ld hl, CHANNEL_FIELD16
	ADD_HL_BC;  // add hl, bc
	LD_hl_E;  // ld [hl], e
// ; store result in NoteDuration
	LD_HL(CHANNEL_NOTE_DURATION);  // ld hl, CHANNEL_NOTE_DURATION
	ADD_HL_BC;  // add hl, bc
	LD_hl_D;  // ld [hl], d
	RET;  // ret


Multiply:
	SET_PC(0xE8AB8U);
//  multiplies a and de
//  adds the result to l
//  stores the result in hl
	LD_H(0);  // ld h, 0

loop:
	SET_PC(0xE8ABAU);
// ; halve a
	SRL_A;  // srl a
// ; is there a remainder?
	IF_NC goto skip;  // jr nc, .skip
// ; add it to the result
	ADD_HL_DE;  // add hl, de

skip:
	SET_PC(0xE8ABFU);
// ; add de, de
	SLA_E;  // sla e
	RL_D;  // rl d
// ; are we done?
	AND_A_A;  // and a
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

int SetGlobalTempo(){
	SET_PC(0xE8AC7U);
	PUSH_BC;  // push bc ; save current channel
// ; are we dealing with music or sfx?
	LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
	CP_A(CHAN5);  // cp CHAN5
	IF_NC goto sfxchannels;  // jr nc, .sfxchannels
	LD_BC(wChannel1);  // ld bc, wChannel1
	CALL(mTempo);  // call Tempo
	LD_BC(wChannel2);  // ld bc, wChannel2
	CALL(mTempo);  // call Tempo
	LD_BC(wChannel3);  // ld bc, wChannel3
	CALL(mTempo);  // call Tempo
	LD_BC(wChannel4);  // ld bc, wChannel4
	CALL(mTempo);  // call Tempo
	goto end;  // jr .end


sfxchannels:
	SET_PC(0xE8AE9U);
	LD_BC(wChannel5);  // ld bc, wChannel5
	CALL(mTempo);  // call Tempo
	LD_BC(wChannel6);  // ld bc, wChannel6
	CALL(mTempo);  // call Tempo
	LD_BC(wChannel7);  // ld bc, wChannel7
	CALL(mTempo);  // call Tempo
	LD_BC(wChannel8);  // ld bc, wChannel8
	CALL(mTempo);  // call Tempo

end:
	SET_PC(0xE8B01U);
	POP_BC;  // pop bc ; restore current channel
	RET;  // ret

}

int Tempo(){
	SET_PC(0xE8B03U);
//  input:
//  	de: note length
// ; update Tempo
	LD_HL(CHANNEL_TEMPO);  // ld hl, CHANNEL_TEMPO
	ADD_HL_BC;  // add hl, bc
	LD_hl_E;  // ld [hl], e
	INC_HL;  // inc hl
	LD_hl_D;  // ld [hl], d
// ; clear ????
	XOR_A_A;  // xor a
	LD_HL(CHANNEL_FIELD16);  // ld hl, CHANNEL_FIELD16
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a
	RET;  // ret

}

int StartChannel(){
	SET_PC(0xE8B11U);
	CALL(mSetLRTracks);  // call SetLRTracks
	LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
	ADD_HL_BC;  // add hl, bc
	SET_hl(SOUND_CHANNEL_ON);  // set SOUND_CHANNEL_ON, [hl] ; turn channel on
	RET;  // ret

}

int SetLRTracks(){
	SET_PC(0xE8B1BU);
//  set tracks for a the current channel to default
//  seems to be redundant since this is overwritten by stereo data later
	PUSH_DE;  // push de
// ; store current channel in de
	LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
	maskbits(NUM_MUSIC_CHANS, 0);  // maskbits NUM_MUSIC_CHANS
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
	CALL(mGetLRTracks);  // call GetLRTracks
	ADD_HL_DE;  // add hl, de ; de = channel 0-3
	LD_A_hl;  // ld a, [hl]
// ; load lr tracks into Tracks
	LD_HL(CHANNEL_TRACKS);  // ld hl, CHANNEL_TRACKS
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a
	POP_DE;  // pop de
	RET;  // ret

}

int v_PlayMusic(){
	SET_PC(0xE8B30U);
//  load music
	CALL(mMusicOff);  // call MusicOff
	LD_HL(wMusicID);  // ld hl, wMusicID
	LD_hl_E;  // ld [hl], e ; song number
	INC_HL;  // inc hl
	LD_hl_D;  // ld [hl], d ; (always 0)
	LD_HL(mMusic);  // ld hl, Music
	ADD_HL_DE;  // add hl, de ; three
	ADD_HL_DE;  // add hl, de ; byte
	ADD_HL_DE;  // add hl, de ; pointer
	LD_A_hli;  // ld a, [hli]
	LD_addr_A(wMusicBank);  // ld [wMusicBank], a
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl] ; music header address
	CALL(mLoadMusicByte);  // call LoadMusicByte ; store first byte of music header in a
	RLCA;  // rlca
	RLCA;  // rlca
	maskbits(NUM_MUSIC_CHANS, 0);  // maskbits NUM_MUSIC_CHANS
	INC_A;  // inc a

loop:
	SET_PC(0xE8B4EU);
//  start playing channels
	PUSH_AF;  // push af
	CALL(mLoadChannel);  // call LoadChannel
	CALL(mStartChannel);  // call StartChannel
	POP_AF;  // pop af
	DEC_A;  // dec a
	IF_NZ goto loop;  // jr nz, .loop
	XOR_A_A;  // xor a
	LD_addr_A(wUnusedMusicF9Flag);  // ld [wUnusedMusicF9Flag], a
	LD_addr_A(wChannel1JumpCondition);  // ld [wChannel1JumpCondition], a
	LD_addr_A(wChannel2JumpCondition);  // ld [wChannel2JumpCondition], a
	LD_addr_A(wChannel3JumpCondition);  // ld [wChannel3JumpCondition], a
	LD_addr_A(wChannel4JumpCondition);  // ld [wChannel4JumpCondition], a
	LD_addr_A(wNoiseSampleAddress);  // ld [wNoiseSampleAddress], a
	LD_addr_A(wNoiseSampleAddress + 1);  // ld [wNoiseSampleAddress + 1], a
	LD_addr_A(wNoiseSampleDelay);  // ld [wNoiseSampleDelay], a
	LD_addr_A(wMusicNoiseSampleSet);  // ld [wMusicNoiseSampleSet], a
	CALL(mMusicOn);  // call MusicOn
	RET;  // ret

}

int v_PlayCry(){
	SET_PC(0xE8B79U);
//  Play cry de using parameters:
// 	wCryPitch
// 	wCryLength

	CALL(mMusicOff);  // call MusicOff

//  Overload the music id with the cry id
	LD_HL(wMusicID);  // ld hl, wMusicID
	LD_hl_E;  // ld [hl], e
	INC_HL;  // inc hl
	LD_hl_D;  // ld [hl], d

//  3-byte pointers (bank, address)
	LD_HL(mCries);  // ld hl, Cries
	ADD_HL_DE;  // add hl, de
	ADD_HL_DE;  // add hl, de
	ADD_HL_DE;  // add hl, de

	LD_A_hli;  // ld a, [hli]
	LD_addr_A(wMusicBank);  // ld [wMusicBank], a

	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]

//  Read the cry's sound header
	CALL(mLoadMusicByte);  // call LoadMusicByte
// ; Top 2 bits contain the number of channels
	RLCA;  // rlca
	RLCA;  // rlca
	maskbits(NUM_MUSIC_CHANS, 0);  // maskbits NUM_MUSIC_CHANS

//  For each channel:
	INC_A;  // inc a

loop:
	SET_PC(0xE8B97U);
	PUSH_AF;  // push af
	CALL(mLoadChannel);  // call LoadChannel ; bc = current channel

	LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
	ADD_HL_BC;  // add hl, bc
	SET_hl(SOUND_CRY);  // set SOUND_CRY, [hl]

	LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
	ADD_HL_BC;  // add hl, bc
	SET_hl(SOUND_PITCH_OFFSET);  // set SOUND_PITCH_OFFSET, [hl]

	LD_HL(CHANNEL_PITCH_OFFSET);  // ld hl, CHANNEL_PITCH_OFFSET
	ADD_HL_BC;  // add hl, bc
	LD_A_addr(wCryPitch);  // ld a, [wCryPitch]
	LD_hli_A;  // ld [hli], a
	LD_A_addr(wCryPitch + 1);  // ld a, [wCryPitch + 1]
	LD_hl_A;  // ld [hl], a

//  No tempo for channel 4
	LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
	maskbits(NUM_MUSIC_CHANS, 0);  // maskbits NUM_MUSIC_CHANS
	CP_A(CHAN4);  // cp CHAN4
	IF_NC goto start;  // jr nc, .start

//  Tempo is effectively length
	LD_HL(CHANNEL_TEMPO);  // ld hl, CHANNEL_TEMPO
	ADD_HL_BC;  // add hl, bc
	LD_A_addr(wCryLength);  // ld a, [wCryLength]
	LD_hli_A;  // ld [hli], a
	LD_A_addr(wCryLength + 1);  // ld a, [wCryLength + 1]
	LD_hl_A;  // ld [hl], a

start:
	SET_PC(0xE8BC8U);
	CALL(mStartChannel);  // call StartChannel
	LD_A_addr(wStereoPanningMask);  // ld a, [wStereoPanningMask]
	AND_A_A;  // and a
	IF_Z goto next;  // jr z, .next

//  Stereo only: Play cry from the monster's side.
//  This only applies in-battle.

	LD_A_addr(wOptions);  // ld a, [wOptions]
	BIT_A(STEREO);  // bit STEREO, a
	IF_Z goto next;  // jr z, .next

//  [CHANNEL_TRACKS] &= [wCryTracks]
	LD_HL(CHANNEL_TRACKS);  // ld hl, CHANNEL_TRACKS
	ADD_HL_BC;  // add hl, bc
	LD_A_hl;  // ld a, [hl]
	LD_HL(wCryTracks);  // ld hl, wCryTracks
	AND_A_hl;  // and [hl]
	LD_HL(CHANNEL_TRACKS);  // ld hl, CHANNEL_TRACKS
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a


next:
	SET_PC(0xE8BE6U);
	POP_AF;  // pop af
	DEC_A;  // dec a
	IF_NZ goto loop;  // jr nz, .loop

//  Cries play at max volume, so we save the current volume for later.
	LD_A_addr(wLastVolume);  // ld a, [wLastVolume]
	AND_A_A;  // and a
	IF_NZ goto end;  // jr nz, .end

	LD_A_addr(wVolume);  // ld a, [wVolume]
	LD_addr_A(wLastVolume);  // ld [wLastVolume], a
	LD_A(MAX_VOLUME);  // ld a, MAX_VOLUME
	LD_addr_A(wVolume);  // ld [wVolume], a


end:
	SET_PC(0xE8BFBU);
	LD_A(1);  // ld a, 1 ; stop playing music
	LD_addr_A(wSFXPriority);  // ld [wSFXPriority], a
	CALL(mMusicOn);  // call MusicOn
	RET;  // ret

}

int v_PlaySFX(){
	SET_PC(0xE8C04U);
//  clear channels if they aren't already
	CALL(mMusicOff);  // call MusicOff
	LD_HL(wChannel5Flags1);  // ld hl, wChannel5Flags1
	BIT_hl(SOUND_CHANNEL_ON);  // bit SOUND_CHANNEL_ON, [hl] ; ch5 on?
	IF_Z goto ch6;  // jr z, .ch6
	RES_hl(SOUND_CHANNEL_ON);  // res SOUND_CHANNEL_ON, [hl] ; turn it off
	XOR_A_A;  // xor a
	LDH_addr_A(rNR11);  // ldh [rNR11], a ; length/wavepattern = 0
	LD_A(0x8);  // ld a, $8
	LDH_addr_A(rNR12);  // ldh [rNR12], a ; envelope = 0
	XOR_A_A;  // xor a
	LDH_addr_A(rNR13);  // ldh [rNR13], a ; frequency lo = 0
	LD_A(0x80);  // ld a, $80
	LDH_addr_A(rNR14);  // ldh [rNR14], a ; restart sound (freq hi = 0)
	XOR_A_A;  // xor a
	LD_addr_A(wPitchSweep);  // ld [wPitchSweep], a ; pitch sweep off
	LDH_addr_A(rNR10);  // ldh [rNR10], a ; pitch sweep off

ch6:
	SET_PC(0xE8C24U);
	LD_HL(wChannel6Flags1);  // ld hl, wChannel6Flags1
	BIT_hl(SOUND_CHANNEL_ON);  // bit SOUND_CHANNEL_ON, [hl]
	IF_Z goto ch7;  // jr z, .ch7
	RES_hl(SOUND_CHANNEL_ON);  // res SOUND_CHANNEL_ON, [hl] ; turn it off
	XOR_A_A;  // xor a
	LDH_addr_A(rNR21);  // ldh [rNR21], a ; length/wavepattern = 0
	LD_A(0x8);  // ld a, $8
	LDH_addr_A(rNR22);  // ldh [rNR22], a ; envelope = 0
	XOR_A_A;  // xor a
	LDH_addr_A(rNR23);  // ldh [rNR23], a ; frequency lo = 0
	LD_A(0x80);  // ld a, $80
	LDH_addr_A(rNR24);  // ldh [rNR24], a ; restart sound (freq hi = 0)

ch7:
	SET_PC(0xE8C3BU);
	LD_HL(wChannel7Flags1);  // ld hl, wChannel7Flags1
	BIT_hl(SOUND_CHANNEL_ON);  // bit SOUND_CHANNEL_ON, [hl]
	IF_Z goto ch8;  // jr z, .ch8
	RES_hl(SOUND_CHANNEL_ON);  // res SOUND_CHANNEL_ON, [hl] ; turn it off
	XOR_A_A;  // xor a
	LDH_addr_A(rNR30);  // ldh [rNR30], a ; sound mode #3 off
	LDH_addr_A(rNR31);  // ldh [rNR31], a ; length/wavepattern = 0
	LD_A(0x8);  // ld a, $8
	LDH_addr_A(rNR32);  // ldh [rNR32], a ; envelope = 0
	XOR_A_A;  // xor a
	LDH_addr_A(rNR33);  // ldh [rNR33], a ; frequency lo = 0
	LD_A(0x80);  // ld a, $80
	LDH_addr_A(rNR34);  // ldh [rNR34], a ; restart sound (freq hi = 0)

ch8:
	SET_PC(0xE8C54U);
	LD_HL(wChannel8Flags1);  // ld hl, wChannel8Flags1
	BIT_hl(SOUND_CHANNEL_ON);  // bit SOUND_CHANNEL_ON, [hl]
	IF_Z goto chscleared;  // jr z, .chscleared
	RES_hl(SOUND_CHANNEL_ON);  // res SOUND_CHANNEL_ON, [hl] ; turn it off
	XOR_A_A;  // xor a
	LDH_addr_A(rNR41);  // ldh [rNR41], a ; length/wavepattern = 0
	LD_A(0x8);  // ld a, $8
	LDH_addr_A(rNR42);  // ldh [rNR42], a ; envelope = 0
	XOR_A_A;  // xor a
	LDH_addr_A(rNR43);  // ldh [rNR43], a ; frequency lo = 0
	LD_A(0x80);  // ld a, $80
	LDH_addr_A(rNR44);  // ldh [rNR44], a ; restart sound (freq hi = 0)
	XOR_A_A;  // xor a
	LD_addr_A(wNoiseSampleAddress);  // ld [wNoiseSampleAddress], a
	LD_addr_A(wNoiseSampleAddress + 1);  // ld [wNoiseSampleAddress + 1], a

chscleared:
	SET_PC(0xE8C72U);
//  start reading sfx header for # chs
	LD_HL(wMusicID);  // ld hl, wMusicID
	LD_hl_E;  // ld [hl], e
	INC_HL;  // inc hl
	LD_hl_D;  // ld [hl], d
	LD_HL(mSFX);  // ld hl, SFX
	ADD_HL_DE;  // add hl, de ; three
	ADD_HL_DE;  // add hl, de ; byte
	ADD_HL_DE;  // add hl, de ; pointers
// ; get bank
	LD_A_hli;  // ld a, [hli]
	LD_addr_A(wMusicBank);  // ld [wMusicBank], a
// ; get address
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]
// ; get # channels
	CALL(mLoadMusicByte);  // call LoadMusicByte
	RLCA;  // rlca ; top 2
	RLCA;  // rlca ; bits
	maskbits(NUM_MUSIC_CHANS, 0);  // maskbits NUM_MUSIC_CHANS
	INC_A;  // inc a ; # channels -> # loops

startchannels:
	SET_PC(0xE8C8DU);
	PUSH_AF;  // push af
	CALL(mLoadChannel);  // call LoadChannel ; bc = current channel
	LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
	ADD_HL_BC;  // add hl, bc
	SET_hl(SOUND_SFX);  // set SOUND_SFX, [hl]
	CALL(mStartChannel);  // call StartChannel
	POP_AF;  // pop af
	DEC_A;  // dec a
	IF_NZ goto startchannels;  // jr nz, .startchannels
	CALL(mMusicOn);  // call MusicOn
	XOR_A_A;  // xor a
	LD_addr_A(wSFXPriority);  // ld [wSFXPriority], a
	RET;  // ret

}

int PlayStereoSFX(){
	SET_PC(0xE8CA6U);
//  play sfx de

	CALL(mMusicOff);  // call MusicOff

//  standard procedure if stereo's off
	LD_A_addr(wOptions);  // ld a, [wOptions]
	BIT_A(STEREO);  // bit STEREO, a
	JP_Z (mv_PlaySFX);  // jp z, _PlaySFX

//  else, let's go ahead with this
	LD_HL(wMusicID);  // ld hl, wMusicID
	LD_hl_E;  // ld [hl], e
	INC_HL;  // inc hl
	LD_hl_D;  // ld [hl], d

//  get sfx ptr
	LD_HL(mSFX);  // ld hl, SFX
	ADD_HL_DE;  // add hl, de
	ADD_HL_DE;  // add hl, de
	ADD_HL_DE;  // add hl, de

//  bank
	LD_A_hli;  // ld a, [hli]
	LD_addr_A(wMusicBank);  // ld [wMusicBank], a
//  address
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]

//  bit 2-3
	CALL(mLoadMusicByte);  // call LoadMusicByte
	RLCA;  // rlca
	RLCA;  // rlca
	maskbits(NUM_MUSIC_CHANS, 0);  // maskbits NUM_MUSIC_CHANS
	INC_A;  // inc a


loop:
	SET_PC(0xE8CCCU);
	PUSH_AF;  // push af
	CALL(mLoadChannel);  // call LoadChannel

	LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
	ADD_HL_BC;  // add hl, bc
	SET_hl(SOUND_SFX);  // set SOUND_SFX, [hl]

	PUSH_DE;  // push de
// ; get tracks for this channel
	LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
	maskbits(NUM_MUSIC_CHANS, 0);  // maskbits NUM_MUSIC_CHANS
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
	CALL(mGetLRTracks);  // call GetLRTracks
	ADD_HL_DE;  // add hl, de
	LD_A_hl;  // ld a, [hl]
	LD_HL(wStereoPanningMask);  // ld hl, wStereoPanningMask
	AND_A_hl;  // and [hl]

	LD_HL(CHANNEL_TRACKS);  // ld hl, CHANNEL_TRACKS
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a

	LD_HL(CHANNEL_FIELD30);  // ld hl, CHANNEL_FIELD30
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a

	LD_A_addr(wCryTracks);  // ld a, [wCryTracks]
	CP_A(2);  // cp 2 ; ch 1-2
	IF_C goto skip;  // jr c, .skip

//  ch3-4
	LD_A_addr(wSFXDuration);  // ld a, [wSFXDuration]

	LD_HL(CHANNEL_FIELD2E);  // ld hl, CHANNEL_FIELD2E
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a

	LD_HL(CHANNEL_FIELD2F);  // ld hl, CHANNEL_FIELD2F
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a

	LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
	ADD_HL_BC;  // add hl, bc
	SET_hl(SOUND_UNKN_0F);  // set SOUND_UNKN_0F, [hl]


skip:
	SET_PC(0xE8D0CU);
	POP_DE;  // pop de

//  turn channel on
	LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
	ADD_HL_BC;  // add hl, bc
	SET_hl(SOUND_CHANNEL_ON);  // set SOUND_CHANNEL_ON, [hl] ; on

//  done?
	POP_AF;  // pop af
	DEC_A;  // dec a
	IF_NZ goto loop;  // jr nz, .loop

//  we're done
	CALL(mMusicOn);  // call MusicOn
	RET;  // ret

}

int LoadChannel(){
	SET_PC(0xE8D1BU);
//  input: de = audio pointer
//  sets bc to current channel pointer
	CALL(mLoadMusicByte);  // call LoadMusicByte
	INC_DE;  // inc de
	maskbits(NUM_CHANNELS, 0);  // maskbits NUM_CHANNELS
	LD_addr_A(wCurChannel);  // ld [wCurChannel], a
	LD_C_A;  // ld c, a
	LD_B(0);  // ld b, 0
	LD_HL(mChannelPointers);  // ld hl, ChannelPointers
	ADD_HL_BC;  // add hl, bc
	ADD_HL_BC;  // add hl, bc
	LD_C_hl;  // ld c, [hl]
	INC_HL;  // inc hl
	LD_B_hl;  // ld b, [hl] ; bc = channel pointer
	LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
	ADD_HL_BC;  // add hl, bc
	RES_hl(SOUND_CHANNEL_ON);  // res SOUND_CHANNEL_ON, [hl] ; channel off
	CALL(mChannelInit);  // call ChannelInit
// ; load music pointer
	LD_HL(CHANNEL_MUSIC_ADDRESS);  // ld hl, CHANNEL_MUSIC_ADDRESS
	ADD_HL_BC;  // add hl, bc
	CALL(mLoadMusicByte);  // call LoadMusicByte
	LD_hli_A;  // ld [hli], a
	INC_DE;  // inc de
	CALL(mLoadMusicByte);  // call LoadMusicByte
	LD_hl_A;  // ld [hl], a
	INC_DE;  // inc de
// ; load music id
	LD_HL(CHANNEL_MUSIC_ID);  // ld hl, CHANNEL_MUSIC_ID
	ADD_HL_BC;  // add hl, bc
	LD_A_addr(wMusicID);  // ld a, [wMusicID]
	LD_hli_A;  // ld [hli], a
	LD_A_addr(wMusicID + 1);  // ld a, [wMusicID + 1]
	LD_hl_A;  // ld [hl], a
// ; load music bank
	LD_HL(CHANNEL_MUSIC_BANK);  // ld hl, CHANNEL_MUSIC_BANK
	ADD_HL_BC;  // add hl, bc
	LD_A_addr(wMusicBank);  // ld a, [wMusicBank]
	LD_hl_A;  // ld [hl], a
	RET;  // ret

}

int ChannelInit(){
	SET_PC(0xE8D5BU);
//  make sure channel is cleared
//  set default tempo and note length in case nothing is loaded
//  input:
//    bc = channel struct pointer
	PUSH_DE;  // push de
	XOR_A_A;  // xor a
// ; get channel struct location and length
	LD_HL(CHANNEL_MUSIC_ID);  // ld hl, CHANNEL_MUSIC_ID ; start
	ADD_HL_BC;  // add hl, bc
	LD_E(CHANNEL_STRUCT_LENGTH);  // ld e, CHANNEL_STRUCT_LENGTH ; channel struct length
// ; clear channel

loop:
	SET_PC(0xE8D63U);
	LD_hli_A;  // ld [hli], a
	DEC_E;  // dec e
	IF_NZ goto loop;  // jr nz, .loop
// ; set tempo to default ($100)
	LD_HL(CHANNEL_TEMPO);  // ld hl, CHANNEL_TEMPO
	ADD_HL_BC;  // add hl, bc
	XOR_A_A;  // xor a
	LD_hli_A;  // ld [hli], a
	INC_A;  // inc a
	LD_hl_A;  // ld [hl], a
// ; set note length to default ($1) (fast)
	LD_HL(CHANNEL_NOTE_LENGTH);  // ld hl, CHANNEL_NOTE_LENGTH
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a
	POP_DE;  // pop de
	RET;  // ret

}

int LoadMusicByte(){
	SET_PC(0xE8D76U);
//  input:
//    de = current music address
//  output:
//    a = wCurMusicByte
	LD_A_addr(wMusicBank);  // ld a, [wMusicBank]
	CALL(mv_LoadMusicByte);  // call _LoadMusicByte
	LD_A_addr(wCurMusicByte);  // ld a, [wCurMusicByte]
	RET;  // ret

// INCLUDE "audio/notes.asm"

// INCLUDE "audio/wave_samples.asm"

// INCLUDE "audio/drumkits.asm"

	return mGetLRTracks;
}

int GetLRTracks(){
	SET_PC(0xE8FC2U);
//  gets the default sound l/r channels
//  stores mono/stereo table in hl
	LD_A_addr(wOptions);  // ld a, [wOptions]
	BIT_A(STEREO);  // bit STEREO, a
// ; made redundant, could have had a purpose in gold
	IF_NZ goto stereo;  // jr nz, .stereo
	LD_HL(mMonoTracks);  // ld hl, MonoTracks
	RET;  // ret


stereo:
	SET_PC(0xE8FCDU);
	LD_HL(mStereoTracks);  // ld hl, StereoTracks
	RET;  // ret

}

int MonoTracks(){
	SET_PC(0xE8FD1U);
//  bit corresponds to track #
//  hi: left channel
//  lo: right channel
	//db ['0x11', '0x22', '0x44', '0x88'];  // db $11, $22, $44, $88

	return mStereoTracks;
}

int StereoTracks(){
	SET_PC(0xE8FD5U);
//  made redundant
//  seems to be modified on a per-song basis
	//db ['0x11', '0x22', '0x44', '0x88'];  // db $11, $22, $44, $88

	return mChannelPointers;
}

int ChannelPointers(){
	SET_PC(0xE8FD9U);
	//table_width ['2', 'ChannelPointers']  // table_width 2, ChannelPointers
//  music channels
	//dw ['wChannel1'];  // dw wChannel1
	//dw ['wChannel2'];  // dw wChannel2
	//dw ['wChannel3'];  // dw wChannel3
	//dw ['wChannel4'];  // dw wChannel4
	//assert_table_length ['NUM_MUSIC_CHANS']  // assert_table_length NUM_MUSIC_CHANS
//  sfx channels
	//dw ['wChannel5'];  // dw wChannel5
	//dw ['wChannel6'];  // dw wChannel6
	//dw ['wChannel7'];  // dw wChannel7
	//dw ['wChannel8'];  // dw wChannel8
	//assert_table_length ['NUM_CHANNELS']  // assert_table_length NUM_CHANNELS

	return mClearChannels;
}

int ClearChannels(){
	SET_PC(0xE8FE9U);
//  runs ClearChannel for all 4 channels
//  doesn't seem to be used, but functionally identical to InitSound
	LD_HL(rNR50);  // ld hl, rNR50
	XOR_A_A;  // xor a
	LD_hli_A;  // ld [hli], a
	LD_hli_A;  // ld [hli], a
	LD_A(0x80);  // ld a, $80
	LD_hli_A;  // ld [hli], a
	LD_HL(rNR10);  // ld hl, rNR10
	LD_E(NUM_MUSIC_CHANS);  // ld e, NUM_MUSIC_CHANS

loop:
	SET_PC(0xE8FF7U);
	CALL(mClearChannel);  // call ClearChannel
	DEC_E;  // dec e
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

int ClearChannel(){
	SET_PC(0xE8FFEU);
//  input: hl = beginning hw sound register (rNR10, rNR20, rNR30, rNR40)
//  output: 00 00 80 00 80

//    sound channel   1      2      3      4
	XOR_A_A;  // xor a
	LD_hli_A;  // ld [hli], a ; rNR10, rNR20, rNR30, rNR40 ; sweep = 0

	LD_hli_A;  // ld [hli], a ; rNR11, rNR21, rNR31, rNR41 ; length/wavepattern = 0
	LD_A(0x8);  // ld a, $8
	LD_hli_A;  // ld [hli], a ; rNR12, rNR22, rNR32, rNR42 ; envelope = 0
	XOR_A_A;  // xor a
	LD_hli_A;  // ld [hli], a ; rNR13, rNR23, rNR33, rNR43 ; frequency lo = 0
	LD_A(0x80);  // ld a, $80
	LD_hli_A;  // ld [hli], a ; rNR14, rNR24, rNR34, rNR44 ; restart sound (freq hi = 0)
	RET;  // ret

}

int PlayTrainerEncounterMusic(){
	SET_PC(0xE900AU);
//  input: e = trainer type
// ; turn fade off
	XOR_A_A;  // xor a
	LD_addr_A(wMusicFade);  // ld [wMusicFade], a
// ; play nothing for one frame
	PUSH_DE;  // push de
	LD_DE(MUSIC_NONE);  // ld de, MUSIC_NONE
	CALL(mPlayMusic);  // call PlayMusic
	CALL(mDelayFrame);  // call DelayFrame
// ; play new song
	CALL(mMaxVolume);  // call MaxVolume
	POP_DE;  // pop de
	LD_D(0x00);  // ld d, $00
	LD_HL(mTrainerEncounterMusic);  // ld hl, TrainerEncounterMusic
	ADD_HL_DE;  // add hl, de
	LD_E_hl;  // ld e, [hl]
	CALL(mPlayMusic);  // call PlayMusic
	RET;  // ret

}
