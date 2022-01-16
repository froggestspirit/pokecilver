#include "../constants.h"
#include "engine.h"

//  The entire sound engine. Uses section "audio" in WRAM.

//  Interfaces are in bank 0.

//  Notable functions:
//      FadeMusic
//      PlayStereoSFX


struct Channel *chan[8];
struct Channel *curChan;
int curChannel;
uint16_t channelPointers[8] = {wChannel1,
                               wChannel2,
                               wChannel3,
                               wChannel4,
                               wChannel5,
                               wChannel6,
                               wChannel7,
                               wChannel8};

void v_InitSound(void){  //  restart sound operation, clear all relevant hardware registers & wram
    for(int i = 0; i < NUM_CHANNELS; i++) chan[i] = gb_pointer(channelPointers[i]);
    MusicOff();
    ClearChannels();
    for(int i = wAudio; i < wAudioEnd; i++) gb_write(i, 0);
    gb_write(wVolume, MAX_VOLUME);
    MusicOn();
}

void MusicFadeRestart(void){  //  restart but keep the music id to fade in to
    uint16_t musicId = gb_read16(wMusicFadeID);
    v_InitSound();
    gb_write16(wMusicFadeID, musicId);
}

void MusicOn(void){
    gb_write(wMusicPlaying, 1);
}

void MusicOff(void){
    gb_write(wMusicPlaying, 0);
}

void v_UpdateSound(void){  // called once per frame
    if(!gb_read(wMusicPlaying)) return;  // no use updating audio if it's not playing
    gb_write(wSoundOutput, 0);
    for(curChannel = 0; curChannel < NUM_CHANNELS; curChannel++){
        gb_write(wCurChannel, curChannel);
        curChan = chan[curChannel];
        if(curChan->channelOn){  // is the channel active?
            if(curChan->noteDuration < 2){  // check time left in the current note
                curChan->vibratoDelayCount = curChan->vibratoDelay;  // reset vibrato delay
                curChan->pitchSlide = 0;  // turn vibrato off for now
                ParseMusic();  // get next note
            }else curChan->noteDuration--;
            ApplyPitchSlide();
            gb_write(wCurTrackDuty, curChan->dutyCycle);  // duty cycle
            gb_write(wCurTrackVolumeEnvelope, curChan->volumeEnvelope);  // volume envelope
            gb_write16(wCurTrackFrequency, curChan->frequency);  // frequency
            HandleTrackVibrato();  // handle vibrato and other things
            HandleNoise();
            if((gb_read(wSFXPriority)) &&  // turn off music when playing sfx?
            (curChannel < NUM_MUSIC_CHANS)){  // are we in a sfx channel right now?
                for(int i = NUM_MUSIC_CHANS; i < NUM_CHANNELS; i++){  // are any sfx channels active?
                    if(chan[i]->channelOn){  // if so, mute
                        curChan->rest = 1;
                        break;
                    }
                }    
            }
            if((curChannel >= NUM_MUSIC_CHANS) ||
            (!chan[curChannel + NUM_MUSIC_CHANS]->channelOn)){  // are we in a sfx channel right now?
                UpdateChannels();
                gb_write(wSoundOutput, gb_read(wSoundOutput) | curChan->tracks);
            }
            curChan->noteFlags = 0;  // clear note flags
        }
    }
    PlayDanger();
    FadeMusic();  // fade music in/out
    gb_write(rNR50, gb_read(wVolume));  // write volume to hardware register
    gb_write(rNR51, gb_read(wSoundOutput));  // write SO on/off to hardware register
}

void UpdateChannels(void){
    static void (*ChannelFunctions[8])(void) = {UpdateChannels_Channel1_LowHealth,
                                                UpdateChannels_Channel2,
                                                UpdateChannels_Channel3,
                                                UpdateChannels_Channel4,
                                                UpdateChannels_Channel1,
                                                UpdateChannels_Channel2,
                                                UpdateChannels_Channel3,
                                                UpdateChannels_Channel4};
    return ChannelFunctions[curChannel]();
}

void UpdateChannels_Channel1_LowHealth(void){
    if(!gb_read(wLowHealthAlarm) | (1 << DANGER_ON_F)) UpdateChannels_Channel1();
}

void UpdateChannels_Channel1(void){
    if(curChan->pitchSweep) gb_write(rNR10, gb_read(wPitchSweep));
    if(curChan->rest){
        gb_write(rNR52, gb_read(rNR52) & 0b10001110);
        ClearChannel(rNR10);
    }else if(curChan->noiseSampling){
        gb_write(rNR11, (gb_read(rNR11) & 0x3F) | gb_read(wCurTrackDuty));
        gb_write(rNR12, gb_read(wCurTrackVolumeEnvelope));
        gb_write16(rNR13, gb_read16(wCurTrackFrequency) | 0x8000);
    }else if(curChan->freqOverride){
        gb_write16(rNR13, gb_read16(wCurTrackFrequency));
        if(curChan->dutyOverride) gb_write(rNR11, (gb_read(rNR11) & 0x3F) | gb_read(wCurTrackDuty));
    }else if(curChan->vibratoOverride){
        gb_write(rNR11, (gb_read(rNR11) & 0x3F) | gb_read(wCurTrackDuty));
        gb_write(rNR13, gb_read(wCurTrackFrequency));
    }else if(curChan->dutyOverride) gb_write(rNR11, (gb_read(rNR11) & 0x3F) | gb_read(wCurTrackDuty));
}

void UpdateChannels_Channel2(void){
    if(curChan->rest) goto rest;
    if(curChan->noiseSampling) goto noise_sampling;
    if(curChan->freqOverride) goto frequency_override;
    if(curChan->vibratoOverride) goto vibrato_override;
    if(curChan->dutyOverride) gb_write(rNR21, (gb_read(rNR21) & 0x3F) | gb_read(wCurTrackDuty));
    return;

frequency_override:
    gb_write16(rNR23, gb_read16(wCurTrackFrequency));
    return;

vibrato_override:
    gb_write(rNR21, (gb_read(rNR21) & 0x3F) | gb_read(wCurTrackDuty));
    gb_write(rNR23, gb_read(wCurTrackFrequency));
    return;

rest:
    gb_write(rNR52, gb_read(rNR52) & 0b10001101);
    ClearChannel(rNR20);
    return;

noise_sampling:
    gb_write(rNR21, (gb_read(rNR21) & 0x3F) | gb_read(wCurTrackDuty));
    gb_write(rNR22, gb_read(wCurTrackVolumeEnvelope));
    gb_write16(rNR23, gb_read16(wCurTrackFrequency) | 0x8000);
}

void UpdateChannels_Channel3(void){
    LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
    ADD_HL_BC;  // add hl, bc
    BIT_hl(NOTE_REST);  // bit NOTE_REST, [hl]
    IF_NZ goto ch3_rest;  // jr nz, .ch3_rest
    BIT_hl(NOTE_NOISE_SAMPLING);  // bit NOTE_NOISE_SAMPLING, [hl]
    IF_NZ goto ch3_noise_sampling;  // jr nz, .ch3_noise_sampling
    BIT_hl(NOTE_VIBRATO_OVERRIDE);  // bit NOTE_VIBRATO_OVERRIDE, [hl]
    IF_NZ goto ch3_vibrato_override;  // jr nz, .ch3_vibrato_override
    return;


ch3_frequency_override:
// 
    LD_A_addr(wCurTrackFrequency);  // ld a, [wCurTrackFrequency]
    LDH_addr_A(rNR33);  // ldh [rNR33], a
    LD_A_addr(wCurTrackFrequency + 1);  // ld a, [wCurTrackFrequency + 1]
    LDH_addr_A(rNR34);  // ldh [rNR34], a
    return;


ch3_vibrato_override:
    LD_A_addr(wCurTrackFrequency);  // ld a, [wCurTrackFrequency]
    LDH_addr_A(rNR33);  // ldh [rNR33], a
    return;


ch3_rest:
    LDH_A_addr(rNR52);  // ldh a, [rNR52]
    AND_A(0b10001011);  // and %10001011 ; ch3 off
    LDH_addr_A(rNR52);  // ldh [rNR52], a
    LD_HL(rNR30);  // ld hl, rNR30
    ClearChannel(rNR30);
    return;


ch3_noise_sampling:
    LD_A(0x3f);  // ld a, $3f ; sound length
    LDH_addr_A(rNR31);  // ldh [rNR31], a
    XOR_A_A;  // xor a
    LDH_addr_A(rNR30);  // ldh [rNR30], a
    UpdateChannels_load_wave_pattern();  // call .load_wave_pattern
    LD_A(0x80);  // ld a, $80
    LDH_addr_A(rNR30);  // ldh [rNR30], a
    LD_A_addr(wCurTrackFrequency);  // ld a, [wCurTrackFrequency]
    LDH_addr_A(rNR33);  // ldh [rNR33], a
    LD_A_addr(wCurTrackFrequency + 1);  // ld a, [wCurTrackFrequency + 1]
    OR_A(0x80);  // or $80
    LDH_addr_A(rNR34);  // ldh [rNR34], a
    return;
}

void UpdateChannels_load_wave_pattern(void){
    PUSH_HL;  // push hl
    LD_A_addr(wCurTrackVolumeEnvelope);  // ld a, [wCurTrackVolumeEnvelope]
    AND_A(0xf);  // and $f ; only 0-9 are valid
    LD_L_A;  // ld l, a
    LD_H(0);  // ld h, 0
// hl << 4
// each wavepattern is $f bytes long
// so seeking is done in $10s
    for(int rept = 0; rept < 4; rept++){
    ADD_HL_HL;  // add hl, hl
    }
    LD_DE(mWaveSamples);  // ld de, WaveSamples
    ADD_HL_DE;  // add hl, de
// load wavepattern into rWave_0-rWave_f
    for(int i = 0; i < 16; i++) gb_write(rWave_0 + i, gb_read(REG_HL + i));
    POP_HL;  // pop hl
    LD_A_addr(wCurTrackVolumeEnvelope);  // ld a, [wCurTrackVolumeEnvelope]
    AND_A(0xf0);  // and $f0
    SLA_A;  // sla a
    LDH_addr_A(rNR32);  // ldh [rNR32], a
}

void UpdateChannels_Channel4(void){
    LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
    ADD_HL_BC;  // add hl, bc
    BIT_hl(NOTE_REST);  // bit NOTE_REST, [hl]
    IF_NZ goto ch4_rest;  // jr nz, .ch4_rest
    BIT_hl(NOTE_NOISE_SAMPLING);  // bit NOTE_NOISE_SAMPLING, [hl]
    IF_NZ goto ch4_noise_sampling;  // jr nz, .ch4_noise_sampling
    return;

ch4_frequency_override:
// 
    LD_A_addr(wCurTrackFrequency);  // ld a, [wCurTrackFrequency]
    LDH_addr_A(rNR43);  // ldh [rNR43], a
    return;

ch4_rest:
    LDH_A_addr(rNR52);  // ldh a, [rNR52]
    AND_A(0b10000111);  // and %10000111 ; ch4 off
    LDH_addr_A(rNR52);  // ldh [rNR52], a
    LD_HL(rNR40);  // ld hl, rNR40
    ClearChannel(rNR40);
    return;

ch4_noise_sampling:
    LD_A(0x3f);  // ld a, $3f ; sound length
    LDH_addr_A(rNR41);  // ldh [rNR41], a
    LD_A_addr(wCurTrackVolumeEnvelope);  // ld a, [wCurTrackVolumeEnvelope]
    LDH_addr_A(rNR42);  // ldh [rNR42], a
    LD_A_addr(wCurTrackFrequency);  // ld a, [wCurTrackFrequency]
    LDH_addr_A(rNR43);  // ldh [rNR43], a
    LD_A(0x80);  // ld a, $80
    LDH_addr_A(rNR44);  // ldh [rNR44], a
}

void v_CheckSFX(void){
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
    return;

sfxon:
    SCF;  // scf
}

void PlayDanger(void){
    LD_A_addr(wLowHealthAlarm);  // ld a, [wLowHealthAlarm]
    BIT_A(DANGER_ON_F);  // bit DANGER_ON_F, a
    IF_Z return;

// Don't do anything if SFX is being played
    AND_A(0xff ^ (1 << DANGER_ON_F));  // and $ff ^ (1 << DANGER_ON_F)
    LD_D_A;  // ld d, a
    v_CheckSFX();  // call _CheckSFX
    IF_C goto increment;  // jr c, .increment

// Play the high tone
    AND_A_A;  // and a
    IF_Z goto begin;  // jr z, .begin

// Play the low tone
    CP_A(16);  // cp 16
    IF_Z goto halfway;  // jr z, .halfway

    goto increment;  // jr .increment


halfway:
    LD_HL(mDangerSoundLow);  // ld hl, DangerSoundLow
    goto applychannel;  // jr .applychannel


begin:
    LD_HL(mDangerSoundHigh);  // ld hl, DangerSoundHigh


applychannel:
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
    LD_A_D;  // ld a, d
    INC_A;  // inc a
    CP_A(30);  // cp 30 ; Ending frame
    IF_C goto noreset;  // jr c, .noreset
    XOR_A_A;  // xor a

noreset:
// Make sure the danger sound is kept on
    OR_A(1 << DANGER_ON_F);  // or 1 << DANGER_ON_F
    LD_addr_A(wLowHealthAlarm);  // ld [wLowHealthAlarm], a

// Enable channel 1 if it's off
    LD_A_addr(wSoundOutput);  // ld a, [wSoundOutput]
    AND_A(0x11);  // and $11
    IF_NZ return;
    LD_A_addr(wSoundOutput);  // ld a, [wSoundOutput]
    OR_A(0x11);  // or $11
    LD_addr_A(wSoundOutput);  // ld [wSoundOutput], a
    return;

}

void DangerSoundHigh(void){
    //db ['0x80'];  // db $80 ; duty 50%
    //db ['0xe2'];  // db $e2 ; volume 14, envelope decrease sweep 2
    //db ['0x50'];  // db $50 ; frequency: $750
    //db ['0x87'];  // db $87 ; restart sound
}

void DangerSoundLow(void){
    //db ['0x80'];  // db $80 ; duty 50%
    //db ['0xe2'];  // db $e2 ; volume 14, envelope decrease sweep 2
    //db ['0xee'];  // db $ee ; frequency: $6ee
    //db ['0x86'];  // db $86 ; restart sound
}

void FadeMusic(void){
//  fade music if applicable
//  usage:
//     write to wMusicFade
//     song fades out at the given rate
//     load song id in wMusicFadeID
//     fade new song in
//  notes:
//     max # frames per volume level is $3f

// fading?
    LD_A_addr(wMusicFade);  // ld a, [wMusicFade]
    AND_A_A;  // and a
    IF_Z return;
// has the count ended?
    LD_A_addr(wMusicFadeCount);  // ld a, [wMusicFadeCount]
    AND_A_A;  // and a
    IF_Z goto update;  // jr z, .update
// count down
    DEC_A;  // dec a
    LD_addr_A(wMusicFadeCount);  // ld [wMusicFadeCount], a
    return;


update:
    LD_A_addr(wMusicFade);  // ld a, [wMusicFade]
    LD_D_A;  // ld d, a
// get new count
    AND_A(0x3f);  // and $3f
    LD_addr_A(wMusicFadeCount);  // ld [wMusicFadeCount], a
// get SO1 volume
    LD_A_addr(wVolume);  // ld a, [wVolume]
    AND_A(VOLUME_SO1_LEVEL);  // and VOLUME_SO1_LEVEL
// which way are we fading?
    BIT_D(MUSIC_FADE_IN_F);  // bit MUSIC_FADE_IN_F, d
    IF_NZ goto fadein;  // jr nz, .fadein
// fading out
    AND_A_A;  // and a
    IF_Z goto novolume;  // jr z, .novolume
    DEC_A;  // dec a
    goto updatevolume;  // jr .updatevolume


novolume:
// make sure volume is off
    XOR_A_A;  // xor a
    LD_addr_A(wVolume);  // ld [wVolume], a
// did we just get on a bike?
    LD_A_addr(wPlayerState);  // ld a, [wPlayerState]
    CP_A(PLAYER_BIKE);  // cp PLAYER_BIKE
    IF_Z goto bicycle;  // jr z, .bicycle
    PUSH_BC;  // push bc
// restart sound
    MusicFadeRestart();
// get new song id
    LD_A_addr(wMusicFadeID);  // ld a, [wMusicFadeID]
    AND_A_A;  // and a
    IF_Z goto quit;  // jr z, .quit ; this assumes there are fewer than 256 songs!
    LD_E_A;  // ld e, a
    LD_A_addr(wMusicFadeID + 1);  // ld a, [wMusicFadeID + 1]
    LD_D_A;  // ld d, a
// load new song
    v_PlayMusic();  // call _PlayMusic

quit:
// cleanup
    POP_BC;  // pop bc
// stop fading
    XOR_A_A;  // xor a
    LD_addr_A(wMusicFade);  // ld [wMusicFade], a
    return;


bicycle:
    PUSH_BC;  // push bc
// restart sound
    MusicFadeRestart();
// this turns the volume up
// turn it back down
    XOR_A_A;  // xor a
    LD_addr_A(wVolume);  // ld [wVolume], a
// get new song id
    LD_A_addr(wMusicFadeID);  // ld a, [wMusicFadeID]
    LD_E_A;  // ld e, a
    LD_A_addr(wMusicFadeID + 1);  // ld a, [wMusicFadeID + 1]
    LD_D_A;  // ld d, a
// load new song
    v_PlayMusic();  // call _PlayMusic
    POP_BC;  // pop bc
// fade in
    LD_HL(wMusicFade);  // ld hl, wMusicFade
    SET_hl(MUSIC_FADE_IN_F);  // set MUSIC_FADE_IN_F, [hl]
    return;


fadein:
// are we done?
    CP_A(MAX_VOLUME & 0xf);  // cp MAX_VOLUME & $f
    IF_NC goto maxvolume;  // jr nc, .maxvolume
// inc volume
    INC_A;  // inc a
    goto updatevolume;  // jr .updatevolume


maxvolume:
// we're done
    XOR_A_A;  // xor a
    LD_addr_A(wMusicFade);  // ld [wMusicFade], a
    return;


updatevolume:
// hi = lo
    LD_D_A;  // ld d, a
    SWAP_A;  // swap a
    OR_A_D;  // or d
    LD_addr_A(wVolume);  // ld [wVolume], a
    return;
}

void LoadNote(void){
// wait for pitch slide to finish
    LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
    ADD_HL_BC;  // add hl, bc
    BIT_hl(SOUND_PITCH_SLIDE);  // bit SOUND_PITCH_SLIDE, [hl]
    IF_Z return;
// get note duration
    LD_HL(CHANNEL_NOTE_DURATION);  // ld hl, CHANNEL_NOTE_DURATION
    ADD_HL_BC;  // add hl, bc
    LD_A_hl;  // ld a, [hl]
    LD_HL(wCurNoteDuration);  // ld hl, wCurNoteDuration
    SUB_A_hl;  // sub [hl]
    IF_NC goto ok;  // jr nc, .ok
    LD_A(1);  // ld a, 1

ok:
    LD_hl_A;  // ld [hl], a
// get frequency
    LD_HL(CHANNEL_FREQUENCY);  // ld hl, CHANNEL_FREQUENCY
    ADD_HL_BC;  // add hl, bc
    LD_E_hl;  // ld e, [hl]
    INC_HL;  // inc hl
    LD_D_hl;  // ld d, [hl]
// get direction of pitch slide
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
// get frequency
    LD_HL(CHANNEL_FREQUENCY);  // ld hl, CHANNEL_FREQUENCY
    ADD_HL_BC;  // add hl, bc
    LD_E_hl;  // ld e, [hl]
    INC_HL;  // inc hl
    LD_D_hl;  // ld d, [hl]
// ????
    LD_HL(CHANNEL_PITCH_SLIDE_TARGET);  // ld hl, CHANNEL_PITCH_SLIDE_TARGET
    ADD_HL_BC;  // add hl, bc
    LD_A_hl;  // ld a, [hl]
    SUB_A_E;  // sub e
    LD_E_A;  // ld e, a
    LD_A_D;  // ld a, d
    SBC_A(0);  // sbc 0
    LD_D_A;  // ld d, a
// ????
    LD_HL(CHANNEL_PITCH_SLIDE_TARGET + 1);  // ld hl, CHANNEL_PITCH_SLIDE_TARGET + 1
    ADD_HL_BC;  // add hl, bc
    LD_A_hl;  // ld a, [hl]
    SUB_A_D;  // sub d
    LD_D_A;  // ld d, a
    goto resume;  // jr .resume


greater_than:
    LD_HL(CHANNEL_FLAGS3);  // ld hl, CHANNEL_FLAGS3
    ADD_HL_BC;  // add hl, bc
    RES_hl(SOUND_PITCH_SLIDE_DIR);  // res SOUND_PITCH_SLIDE_DIR, [hl]
// get frequency
    LD_HL(CHANNEL_FREQUENCY);  // ld hl, CHANNEL_FREQUENCY
    ADD_HL_BC;  // add hl, bc
    LD_E_hl;  // ld e, [hl]
    INC_HL;  // inc hl
    LD_D_hl;  // ld d, [hl]
// get distance from pitch slide target
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
// de = x * [wCurNoteDuration] + y
// x + 1 -> d
// y -> a
    PUSH_BC;  // push bc
    LD_HL(wCurNoteDuration);  // ld hl, wCurNoteDuration
    LD_B(0);  // ld b, 0 ; quotient

loop:
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
    return;
}

void HandleTrackVibrato(void){
//  handle duty, cry pitch, and vibrato
    REG_BC = channelPointers[curChannel];
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
// is vibrato on?
    LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
    ADD_HL_BC;  // add hl, bc
    BIT_hl(SOUND_VIBRATO);  // bit SOUND_VIBRATO, [hl] ; vibrato
    IF_Z goto quit;  // jr z, .quit
// is vibrato active for this note yet?
// is the delay over?
    LD_HL(CHANNEL_VIBRATO_DELAY_COUNT);  // ld hl, CHANNEL_VIBRATO_DELAY_COUNT
    ADD_HL_BC;  // add hl, bc
    LD_A_hl;  // ld a, [hl]
    AND_A_A;  // and a
    IF_NZ goto subexit;  // jr nz, .subexit
// is the extent nonzero?
    LD_HL(CHANNEL_VIBRATO_EXTENT);  // ld hl, CHANNEL_VIBRATO_EXTENT
    ADD_HL_BC;  // add hl, bc
    LD_A_hl;  // ld a, [hl]
    AND_A_A;  // and a
    IF_Z goto quit;  // jr z, .quit
// save it for later
    LD_D_A;  // ld d, a
// is it time to toggle vibrato up/down?
    LD_HL(CHANNEL_VIBRATO_RATE);  // ld hl, CHANNEL_VIBRATO_RATE
    ADD_HL_BC;  // add hl, bc
    LD_A_hl;  // ld a, [hl]
    AND_A(0xf);  // and $f ; count
    IF_Z goto toggle;  // jr z, .toggle

subexit:
    DEC_hl;  // dec [hl]
    goto quit;  // jr .quit


toggle:
// refresh count
    LD_A_hl;  // ld a, [hl]
    SWAP_hl;  // swap [hl]
    OR_A_hl;  // or [hl]
    LD_hl_A;  // ld [hl], a
// ????
    LD_A_addr(wCurTrackFrequency);  // ld a, [wCurTrackFrequency]
    LD_E_A;  // ld e, a
// toggle vibrato up/down
    LD_HL(CHANNEL_FLAGS3);  // ld hl, CHANNEL_FLAGS3
    ADD_HL_BC;  // add hl, bc
    BIT_hl(SOUND_VIBRATO_DIR);  // bit SOUND_VIBRATO_DIR, [hl] ; vibrato up/down
    IF_Z goto down;  // jr z, .down
//  up
// vibrato down
    RES_hl(SOUND_VIBRATO_DIR);  // res SOUND_VIBRATO_DIR, [hl]
// get the delay
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
// vibrato up
    SET_hl(SOUND_VIBRATO_DIR);  // set SOUND_VIBRATO_DIR, [hl]
// get the delay
    LD_A_D;  // ld a, d
    AND_A(0xf0);  // and $f0 ; hi
    SWAP_A;  // swap a ; move it to lo
// ;
    ADD_A_E;  // add e
    IF_NC goto no_carry;  // jr nc, .no_carry
    LD_A(0xff);  // ld a, $ff

no_carry:
    LD_addr_A(wCurTrackFrequency);  // ld [wCurTrackFrequency], a
// ;
    LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
    ADD_HL_BC;  // add hl, bc
    SET_hl(NOTE_VIBRATO_OVERRIDE);  // set NOTE_VIBRATO_OVERRIDE, [hl]

quit:
    return;
}

void ApplyPitchSlide(void){
// quit if pitch slide inactive
    REG_BC = channelPointers[curChannel];
    LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
    ADD_HL_BC;  // add hl, bc
    BIT_hl(SOUND_PITCH_SLIDE);  // bit SOUND_PITCH_SLIDE, [hl]
    IF_Z return;
// de = Frequency
    LD_HL(CHANNEL_FREQUENCY);  // ld hl, CHANNEL_FREQUENCY
    ADD_HL_BC;  // add hl, bc
    LD_E_hl;  // ld e, [hl]
    INC_HL;  // inc hl
    LD_D_hl;  // ld d, [hl]
// check whether pitch slide is going up or down
    LD_HL(CHANNEL_FLAGS3);  // ld hl, CHANNEL_FLAGS3
    ADD_HL_BC;  // add hl, bc
    BIT_hl(SOUND_PITCH_SLIDE_DIR);  // bit SOUND_PITCH_SLIDE_DIR, [hl]
    IF_Z goto decreasing;  // jr z, .decreasing
// frequency += [Channel*PitchSlideAmount]
    LD_HL(CHANNEL_PITCH_SLIDE_AMOUNT);  // ld hl, CHANNEL_PITCH_SLIDE_AMOUNT
    ADD_HL_BC;  // add hl, bc
    LD_L_hl;  // ld l, [hl]
    LD_H(0);  // ld h, 0
    ADD_HL_DE;  // add hl, de
    LD_D_H;  // ld d, h
    LD_E_L;  // ld e, l
// [Channel*Field25] += [Channel*PitchSlideAmountFraction]
// if rollover: Frequency += 1
    LD_HL(CHANNEL_PITCH_SLIDE_AMOUNT_FRACTION);  // ld hl, CHANNEL_PITCH_SLIDE_AMOUNT_FRACTION
    ADD_HL_BC;  // add hl, bc
    LD_A_hl;  // ld a, [hl]
    LD_HL(CHANNEL_FIELD25);  // ld hl, CHANNEL_FIELD25
    ADD_HL_BC;  // add hl, bc
    ADD_A_hl;  // add [hl]
    LD_hl_A;  // ld [hl], a
// could have done "jr nc, .no_rollover / inc de / .no_rollover"
    LD_A(0);  // ld a, 0
    ADC_A_E;  // adc e
    LD_E_A;  // ld e, a
    LD_A(0);  // ld a, 0
    ADC_A_D;  // adc d
    LD_D_A;  // ld d, a
// Compare the dw at [Channel*PitchSlideTarget] to de.
// If frequency is greater, we're finished.
// Otherwise, load the frequency and set two flags.
    LD_HL(CHANNEL_PITCH_SLIDE_TARGET + 1);  // ld hl, CHANNEL_PITCH_SLIDE_TARGET + 1
    ADD_HL_BC;  // add hl, bc
    LD_A_hl;  // ld a, [hl]
    CP_A_D;  // cp d
    IF_C return ApplyPitchSlide_finished_pitch_slide();  // jp c, .finished_pitch_slide
    IF_NZ return ApplyPitchSlide_continue_pitch_slide();  // jr nz, .continue_pitch_slide
    LD_HL(CHANNEL_PITCH_SLIDE_TARGET);  // ld hl, CHANNEL_PITCH_SLIDE_TARGET
    ADD_HL_BC;  // add hl, bc
    LD_A_hl;  // ld a, [hl]
    CP_A_E;  // cp e
    IF_C return ApplyPitchSlide_finished_pitch_slide();  // jp c, .finished_pitch_slide
    return ApplyPitchSlide_continue_pitch_slide();  // jr .continue_pitch_slide

decreasing:
// frequency -= [Channel*PitchSlideAmount]
    LD_A_E;  // ld a, e
    LD_HL(CHANNEL_PITCH_SLIDE_AMOUNT);  // ld hl, CHANNEL_PITCH_SLIDE_AMOUNT
    ADD_HL_BC;  // add hl, bc
    LD_E_hl;  // ld e, [hl]
    SUB_A_E;  // sub e
    LD_E_A;  // ld e, a
    LD_A_D;  // ld a, d
    SBC_A(0);  // sbc 0
    LD_D_A;  // ld d, a
// [Channel*Field25] *= 2
// if rollover: Frequency -= 1
    LD_HL(CHANNEL_PITCH_SLIDE_AMOUNT_FRACTION);  // ld hl, CHANNEL_PITCH_SLIDE_AMOUNT_FRACTION
    ADD_HL_BC;  // add hl, bc
    LD_A_hl;  // ld a, [hl]
    ADD_A_A;  // add a
    LD_hl_A;  // ld [hl], a
// could have done "jr nc, .no_rollover / dec de / .no_rollover"
    LD_A_E;  // ld a, e
    SBC_A(0);  // sbc 0
    LD_E_A;  // ld e, a
    LD_A_D;  // ld a, d
    SBC_A(0);  // sbc 0
    LD_D_A;  // ld d, a
// Compare the dw at [Channel*PitchSlideTarget] to de.
// If frequency is lower, we're finished.
// Otherwise, load the frequency and set two flags.
    LD_HL(CHANNEL_PITCH_SLIDE_TARGET + 1);  // ld hl, CHANNEL_PITCH_SLIDE_TARGET + 1
    ADD_HL_BC;  // add hl, bc
    LD_A_D;  // ld a, d
    CP_A_hl;  // cp [hl]
    IF_C return ApplyPitchSlide_finished_pitch_slide();  // jr c, .finished_pitch_slide
    IF_NZ return ApplyPitchSlide_continue_pitch_slide();  // jr nz, .continue_pitch_slide
    LD_HL(CHANNEL_PITCH_SLIDE_TARGET);  // ld hl, CHANNEL_PITCH_SLIDE_TARGET
    ADD_HL_BC;  // add hl, bc
    LD_A_E;  // ld a, e
    CP_A_hl;  // cp [hl]
    IF_NC return ApplyPitchSlide_continue_pitch_slide();  // jr nc, .continue_pitch_slide
    return ApplyPitchSlide_finished_pitch_slide();
}

void ApplyPitchSlide_finished_pitch_slide(void){
    LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
    ADD_HL_BC;  // add hl, bc
    RES_hl(SOUND_PITCH_SLIDE);  // res SOUND_PITCH_SLIDE, [hl]
    LD_HL(CHANNEL_FLAGS3);  // ld hl, CHANNEL_FLAGS3
    ADD_HL_BC;  // add hl, bc
    RES_hl(SOUND_PITCH_SLIDE_DIR);  // res SOUND_PITCH_SLIDE_DIR, [hl]
    return;
}

void ApplyPitchSlide_continue_pitch_slide(void){
    LD_HL(CHANNEL_FREQUENCY);  // ld hl, CHANNEL_FREQUENCY
    ADD_HL_BC;  // add hl, bc
    LD_hl_E;  // ld [hl], e
    INC_HL;  // inc hl
    LD_hl_D;  // ld [hl], d
    LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
    ADD_HL_BC;  // add hl, bc
    SET_hl(NOTE_FREQ_OVERRIDE);  // set NOTE_FREQ_OVERRIDE, [hl]
    SET_hl(NOTE_DUTY_OVERRIDE);  // set NOTE_DUTY_OVERRIDE, [hl]
    return;
}

void HandleNoise(void){
// is noise sampling on?
    REG_BC = channelPointers[curChannel];
    LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
    ADD_HL_BC;  // add hl, bc
    BIT_hl(SOUND_NOISE);  // bit SOUND_NOISE, [hl] ; noise sampling
    IF_Z return;
// are we in a sfx channel?
    LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
    BIT_A(NOISE_CHAN_F);  // bit NOISE_CHAN_F, a
    IF_NZ goto next;  // jr nz, .next
// is ch8 on? (noise)
    LD_HL(wChannel8Flags1);  // ld hl, wChannel8Flags1
    BIT_hl(SOUND_CHANNEL_ON);  // bit SOUND_CHANNEL_ON, [hl] ; on?
    IF_Z goto next;  // jr z, .next
// is ch8 playing noise?
    BIT_hl(SOUND_NOISE);  // bit SOUND_NOISE, [hl]
    IF_NZ return;
// ;

next:
    LD_A_addr(wNoiseSampleDelay);  // ld a, [wNoiseSampleDelay]
    AND_A_A;  // and a
    IF_Z return ReadNoiseSample();  // jr z, ReadNoiseSample
    DEC_A;  // dec a
    LD_addr_A(wNoiseSampleDelay);  // ld [wNoiseSampleDelay], a
    return;

}

void ReadNoiseSample(void){
//  sample struct:
//     [wx] [yy] [zz]
//     w: ? either 2 or 3
//     x: duration
//     zz: volume envelope
//        yy: frequency

// de = [wNoiseSampleAddress]
    LD_HL(wNoiseSampleAddress);  // ld hl, wNoiseSampleAddress
    LD_E_hl;  // ld e, [hl]
    INC_HL;  // inc hl
    LD_D_hl;  // ld d, [hl]

// is it empty?
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
    return;

quit:
    return;
}

void ParseMusic(void){
//  parses until a note is read or the song is ended
    REG_BC = channelPointers[curChannel];
    GetMusicByte(); // store next byte in a
    CP_A(sound_ret_cmd);  // cp sound_ret_cmd
    IF_Z goto sound_ret;  // jr z, .sound_ret
    CP_A(FIRST_MUSIC_CMD);  // cp FIRST_MUSIC_CMD
    IF_C goto readnote;  // jr c, .readnote

readcommand:
    ParseMusicCommand();  // call ParseMusicCommand
    return ParseMusic();  // jr ParseMusic ; start over

readnote:
//  wCurMusicByte contains current note
//  special notes
    LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
    ADD_HL_BC;  // add hl, bc
    BIT_hl(SOUND_SFX);  // bit SOUND_SFX, [hl]
    IF_NZ return ParseSFXOrCry();  // jp nz, ParseSFXOrCry
    BIT_hl(SOUND_CRY);  // bit SOUND_CRY, [hl]
    IF_NZ return ParseSFXOrCry();  // jp nz, ParseSFXOrCry
    BIT_hl(SOUND_NOISE);  // bit SOUND_NOISE, [hl]
    IF_NZ return GetNoiseSample();  // jp nz, GetNoiseSample
//  normal note
// set note duration (bottom nybble)
    LD_A_addr(wCurMusicByte);  // ld a, [wCurMusicByte]
    AND_A(0xf);  // and $f
    //return 0x45F1 + 24;
    SetNoteDuration();
// get note pitch (top nybble)
    LD_A_addr(wCurMusicByte);  // ld a, [wCurMusicByte]
    SWAP_A;  // swap a
    AND_A(0xf);  // and $f
    IF_Z goto rest;  // jr z, .rest ; pitch 0 -> rest
// update pitch
    LD_HL(CHANNEL_PITCH);  // ld hl, CHANNEL_PITCH
    ADD_HL_BC;  // add hl, bc
    LD_hl_A;  // ld [hl], a
// store pitch in e
    LD_E_A;  // ld e, a
// store octave in d
    LD_HL(CHANNEL_OCTAVE);  // ld hl, CHANNEL_OCTAVE
    ADD_HL_BC;  // add hl, bc
    LD_D_hl;  // ld d, [hl]
// update frequency
    GetFrequency();
    LD_HL(CHANNEL_FREQUENCY);  // ld hl, CHANNEL_FREQUENCY
    ADD_HL_BC;  // add hl, bc
    LD_hl_E;  // ld [hl], e
    INC_HL;  // inc hl
    LD_hl_D;  // ld [hl], d
// ????
    LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
    ADD_HL_BC;  // add hl, bc
    SET_hl(NOTE_NOISE_SAMPLING);  // set NOTE_NOISE_SAMPLING, [hl]
    return LoadNote();

rest:
//  note = rest
    LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
    ADD_HL_BC;  // add hl, bc
    SET_hl(NOTE_REST);  // set NOTE_REST, [hl] ; Rest
    return;

sound_ret:
//  $ff is reached in music data
    LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
    ADD_HL_BC;  // add hl, bc
    BIT_hl(SOUND_SUBROUTINE);  // bit SOUND_SUBROUTINE, [hl] ; in a subroutine?
    IF_NZ goto readcommand;  // jr nz, .readcommand ; execute
    LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
    CP_A(CHAN5);  // cp CHAN5
    IF_NC goto chan_5to8;  // jr nc, .chan_5to8
// ????
    LD_HL(CHANNEL_STRUCT_LENGTH * NUM_MUSIC_CHANS + CHANNEL_FLAGS1);  // ld hl, CHANNEL_STRUCT_LENGTH * NUM_MUSIC_CHANS + CHANNEL_FLAGS1
    ADD_HL_BC;  // add hl, bc
    BIT_hl(SOUND_CHANNEL_ON);  // bit SOUND_CHANNEL_ON, [hl]
    IF_NZ goto ok;  // jr nz, .ok

chan_5to8:
    LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
    ADD_HL_BC;  // add hl, bc
    BIT_hl(SOUND_CRY);  // bit SOUND_CRY, [hl]
    IF_NZ RestoreVolume();
// end music
    LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
    CP_A(CHAN5);  // cp CHAN5
    IF_NZ goto ok;  // jr nz, .ok
// ????
    XOR_A_A;  // xor a
    LDH_addr_A(rNR10);  // ldh [rNR10], a ; sweep = 0

ok:
//  stop playing
// turn channel off
    LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
    ADD_HL_BC;  // add hl, bc
    RES_hl(SOUND_CHANNEL_ON);  // res SOUND_CHANNEL_ON, [hl]
// note = rest
    LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
    ADD_HL_BC;  // add hl, bc
    SET_hl(NOTE_REST);  // set NOTE_REST, [hl]
// clear music id & bank
    LD_HL(CHANNEL_MUSIC_ID);  // ld hl, CHANNEL_MUSIC_ID
    ADD_HL_BC;  // add hl, bc
    XOR_A_A;  // xor a
    LD_hli_A;  // ld [hli], a ; id hi
    LD_hli_A;  // ld [hli], a ; id lo
    LD_hli_A;  // ld [hli], a ; bank
    return;
}

void RestoreVolume(void){
// ch5 only
    LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
    CP_A(CHAN5);  // cp CHAN5
    IF_NZ return;
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
    return;
}

void ParseSFXOrCry(void){
// turn noise sampling on
    LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
    ADD_HL_BC;  // add hl, bc
    SET_hl(NOTE_NOISE_SAMPLING);  // set NOTE_NOISE_SAMPLING, [hl] ; noise sample
// update note duration
    LD_A_addr(wCurMusicByte);  // ld a, [wCurMusicByte]
    SetNoteDuration();  // top nybble doesnt matter?
// update volume envelope from next param
    GetMusicByte();
    LD_HL(CHANNEL_VOLUME_ENVELOPE);  // ld hl, CHANNEL_VOLUME_ENVELOPE
    ADD_HL_BC;  // add hl, bc
    LD_hl_A;  // ld [hl], a
// update lo frequency from next param
    GetMusicByte();
    LD_HL(CHANNEL_FREQUENCY);  // ld hl, CHANNEL_FREQUENCY
    ADD_HL_BC;  // add hl, bc
    LD_hl_A;  // ld [hl], a
// are we on the last channel? (noise sampling)
    LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
    maskbits(NUM_MUSIC_CHANS, 0);  // maskbits NUM_MUSIC_CHANS
    CP_A(CHAN4);  // cp CHAN4
    IF_Z return;
// update hi frequency from next param
    GetMusicByte();
    LD_HL(CHANNEL_FREQUENCY + 1);  // ld hl, CHANNEL_FREQUENCY + 1
    ADD_HL_BC;  // add hl, bc
    LD_hl_A;  // ld [hl], a
    return;
}

void GetNoiseSample(void){
//  load ptr to sample header in wNoiseSampleAddress
// are we on the last channel?
    LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
    AND_A(NUM_MUSIC_CHANS - 1);  // and NUM_MUSIC_CHANS - 1
    CP_A(CHAN4);  // cp CHAN4
// ret if not
    IF_NZ return;
// update note duration
    LD_A_addr(wCurMusicByte);  // ld a, [wCurMusicByte]
    AND_A(0xf);  // and $f
    SetNoteDuration();
// check current channel
    LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
    BIT_A(NOISE_CHAN_F);  // bit NOISE_CHAN_F, a
    IF_NZ goto sfx;  // jr nz, .sfx
    LD_HL(wChannel8Flags1);  // ld hl, wChannel8Flags1
    BIT_hl(SOUND_CHANNEL_ON);  // bit SOUND_CHANNEL_ON, [hl] ; is ch8 on? (noise)
    IF_NZ return;
    LD_A_addr(wMusicNoiseSampleSet);  // ld a, [wMusicNoiseSampleSet]
    goto next;  // jr .next

sfx:
    LD_A_addr(wSFXNoiseSampleSet);  // ld a, [wSFXNoiseSampleSet]

next:
// load noise sample set id into de
    LD_E_A;  // ld e, a
    LD_D(0);  // ld d, 0
// load ptr to noise sample set in hl
    LD_HL(mDrumkits);  // ld hl, Drumkits
    ADD_HL_DE;  // add hl, de
    ADD_HL_DE;  // add hl, de
    LD_A_hli;  // ld a, [hli]
    LD_H_hl;  // ld h, [hl]
    LD_L_A;  // ld l, a
// get pitch
    LD_A_addr(wCurMusicByte);  // ld a, [wCurMusicByte]
    SWAP_A;  // swap a
// non-rest note?
    AND_A(0xf);  // and $f
    IF_Z return;
// use 'pitch' to seek noise sample set
    LD_E_A;  // ld e, a
    LD_D(0);  // ld d, 0
    ADD_HL_DE;  // add hl, de
    ADD_HL_DE;  // add hl, de
// load sample pointer into wNoiseSampleAddress
    LD_A_hli;  // ld a, [hli]
    LD_addr_A(wNoiseSampleAddress);  // ld [wNoiseSampleAddress], a
    LD_A_hl;  // ld a, [hl]
    LD_addr_A(wNoiseSampleAddress + 1);  // ld [wNoiseSampleAddress + 1], a
// clear ????
    XOR_A_A;  // xor a
    LD_addr_A(wNoiseSampleDelay);  // ld [wNoiseSampleDelay], a
    return;
}

void ParseMusicCommand(void){
    static int (*MusicCommands[48])() = {Music_Octave,
                                         Music_Octave,
                                         Music_Octave,
                                         Music_Octave,
                                         Music_Octave,
                                         Music_Octave,
                                         Music_Octave,
                                         Music_Octave,
                                         Music_NoteType,
                                         Music_Transpose,
                                         Music_Tempo,
                                         Music_DutyCycle,
                                         Music_VolumeEnvelope,
                                         Music_PitchSweep,
                                         Music_DutyCyclePattern,
                                         Music_ToggleSFX,
                                         Music_PitchSlide,
                                         Music_Vibrato,
                                         MusicE2,
                                         Music_ToggleNoise,
                                         Music_ForceStereoPanning,
                                         Music_Volume,
                                         Music_PitchOffset,
                                         MusicE7,
                                         MusicE8,
                                         Music_TempoRelative,
                                         Music_RestartChannel,
                                         Music_NewSong,
                                         Music_SFXPriorityOn,
                                         Music_SFXPriorityOff,
                                         MusicEE,
                                         Music_StereoPanning,
                                         Music_SFXToggleNoise,
                                         MusicNone,
                                         MusicNone,
                                         MusicNone,
                                         MusicNone,
                                         MusicNone,
                                         MusicNone,
                                         MusicNone,
                                         MusicNone,
                                         MusicF9,
                                         Music_SetCondition,
                                         Music_JumpIf,
                                         Music_Jump,
                                         Music_Loop,
                                         Music_Call,
                                         Music_Ret};
                                        
    return MusicCommands[gb_read(wCurMusicByte) - FIRST_MUSIC_CMD]();
}

void MusicNone(void){
    return;
}

void Music_Ret(void){
//  called when $ff is encountered w/ subroutine flag set
//  end music stream
//  return to caller of the subroutine
// reset subroutine flag
    LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
    ADD_HL_BC;  // add hl, bc
    RES_hl(SOUND_SUBROUTINE);  // res SOUND_SUBROUTINE, [hl]
// copy LastMusicAddress to MusicAddress
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
    return;
}

void Music_Call(void){
//  call music stream (subroutine)
//  parameters: ll hh
// get pointer from next 2 bytes
    GetMusicByte();
    LD_E_A;  // ld e, a
    GetMusicByte();
    LD_D_A;  // ld d, a
    PUSH_DE;  // push de
// copy MusicAddress to LastMusicAddress
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
// load pointer into MusicAddress
    POP_DE;  // pop de
    LD_HL(CHANNEL_MUSIC_ADDRESS);  // ld hl, CHANNEL_MUSIC_ADDRESS
    ADD_HL_BC;  // add hl, bc
    LD_hl_E;  // ld [hl], e
    INC_HL;  // inc hl
    LD_hl_D;  // ld [hl], d
// set subroutine flag
    LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
    ADD_HL_BC;  // add hl, bc
    SET_hl(SOUND_SUBROUTINE);  // set SOUND_SUBROUTINE, [hl]
    return;
}

void Music_Jump(void){
//  jump
//  parameters: ll hh
// get pointer from next 2 bytes
    GetMusicByte();
    LD_E_A;  // ld e, a
    GetMusicByte();
    LD_D_A;  // ld d, a
    LD_HL(CHANNEL_MUSIC_ADDRESS);  // ld hl, CHANNEL_MUSIC_ADDRESS
    ADD_HL_BC;  // add hl, bc
    LD_hl_E;  // ld [hl], e
    INC_HL;  // inc hl
    LD_hl_D;  // ld [hl], d
    return;
}

void Music_Loop(void){
//  loops xx - 1 times
//      00: infinite
//  params: 3
//     xx ll hh
//         xx : loop count
//        ll hh : pointer

// get loop count
    GetMusicByte();
    LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
    ADD_HL_BC;  // add hl, bc
    BIT_hl(SOUND_LOOPING);  // bit SOUND_LOOPING, [hl] ; has the loop been initiated?
    IF_NZ goto checkloop;  // jr nz, .checkloop
    AND_A_A;  // and a ; loop counter 0 = infinite
    IF_Z goto loop;  // jr z, .loop
// initiate loop
    DEC_A;  // dec a
    SET_hl(SOUND_LOOPING);  // set SOUND_LOOPING, [hl] ; set loop flag
    LD_HL(CHANNEL_LOOP_COUNT);  // ld hl, CHANNEL_LOOP_COUNT
    ADD_HL_BC;  // add hl, bc
    LD_hl_A;  // ld [hl], a ; store loop counter

checkloop:
    LD_HL(CHANNEL_LOOP_COUNT);  // ld hl, CHANNEL_LOOP_COUNT
    ADD_HL_BC;  // add hl, bc
    LD_A_hl;  // ld a, [hl]
    AND_A_A;  // and a ; are we done?
    IF_Z goto endloop;  // jr z, .endloop
    DEC_hl;  // dec [hl]

loop:
// get pointer
    GetMusicByte();
    LD_E_A;  // ld e, a
    GetMusicByte();
    LD_D_A;  // ld d, a
// load new pointer into MusicAddress
    LD_HL(CHANNEL_MUSIC_ADDRESS);  // ld hl, CHANNEL_MUSIC_ADDRESS
    ADD_HL_BC;  // add hl, bc
    LD_hl_E;  // ld [hl], e
    INC_HL;  // inc hl
    LD_hl_D;  // ld [hl], d
    return;

endloop:
// reset loop flag
    LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
    ADD_HL_BC;  // add hl, bc
    RES_hl(SOUND_LOOPING);  // res SOUND_LOOPING, [hl]
// skip to next command
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
    return;
}

void Music_SetCondition(void){
//  set condition for a jump
//  used with FB
//  params: 1
//     xx

// set condition
    GetMusicByte();
    LD_HL(CHANNEL_CONDITION);  // ld hl, CHANNEL_CONDITION
    ADD_HL_BC;  // add hl, bc
    LD_hl_A;  // ld [hl], a
    return;
}

void Music_JumpIf(void){
//  conditional jump
//  used with FA
//  params: 3
//      xx: condition
//     ll hh: pointer

//  check condition
// a = condition
    GetMusicByte();
// if existing condition matches, jump to new address
    LD_HL(CHANNEL_CONDITION);  // ld hl, CHANNEL_CONDITION
    ADD_HL_BC;  // add hl, bc
    CP_A_hl;  // cp [hl]
    IF_Z goto jump;  // jr z, .jump
//  skip to next command
// get address
    LD_HL(CHANNEL_MUSIC_ADDRESS);  // ld hl, CHANNEL_MUSIC_ADDRESS
    ADD_HL_BC;  // add hl, bc
    LD_E_hl;  // ld e, [hl]
    INC_HL;  // inc hl
    LD_D_hl;  // ld d, [hl]
// skip pointer
    INC_DE;  // inc de
    INC_DE;  // inc de
// update address
    LD_hl_D;  // ld [hl], d
    DEC_HL;  // dec hl
    LD_hl_E;  // ld [hl], e
    return;

jump:
//  jump to the new address
// get pointer
    GetMusicByte();
    LD_E_A;  // ld e, a
    GetMusicByte();
    LD_D_A;  // ld d, a
// update pointer in MusicAddress
    LD_HL(CHANNEL_MUSIC_ADDRESS);  // ld hl, CHANNEL_MUSIC_ADDRESS
    ADD_HL_BC;  // add hl, bc
    LD_hl_E;  // ld [hl], e
    INC_HL;  // inc hl
    LD_hl_D;  // ld [hl], d
    return;
}

void MusicEE(void){
//  unused
//  conditional jump
//  checks a byte in ram corresponding to the current channel
//  params: 2
//         ll hh

//  if ????, jump
// get channel
    LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
    maskbits(NUM_MUSIC_CHANS, 0);  // maskbits NUM_MUSIC_CHANS
    LD_E_A;  // ld e, a
    LD_D(0);  // ld d, 0
// hl = wChannel1JumpCondition + channel id
    LD_HL(wChannel1JumpCondition);  // ld hl, wChannel1JumpCondition
    ADD_HL_DE;  // add hl, de
// if set, jump
    LD_A_hl;  // ld a, [hl]
    AND_A_A;  // and a
    IF_NZ goto jump;  // jr nz, .jump
//  skip to next command
// get address
    LD_HL(CHANNEL_MUSIC_ADDRESS);  // ld hl, CHANNEL_MUSIC_ADDRESS
    ADD_HL_BC;  // add hl, bc
    LD_E_hl;  // ld e, [hl]
    INC_HL;  // inc hl
    LD_D_hl;  // ld d, [hl]
// skip pointer
    INC_DE;  // inc de
    INC_DE;  // inc de
// update address
    LD_hl_D;  // ld [hl], d
    DEC_HL;  // dec hl
    LD_hl_E;  // ld [hl], e
    return;

jump:
// reset jump flag
    LD_hl(0);  // ld [hl], 0
// de = pointer
    GetMusicByte();
    LD_E_A;  // ld e, a
    GetMusicByte();
    LD_D_A;  // ld d, a
// update address
    LD_HL(CHANNEL_MUSIC_ADDRESS);  // ld hl, CHANNEL_MUSIC_ADDRESS
    ADD_HL_BC;  // add hl, bc
    LD_hl_E;  // ld [hl], e
    INC_HL;  // inc hl
    LD_hl_D;  // ld [hl], d
    return;
}

void MusicF9(void){
//  unused
//  sets some flag
//  params: 0
    LD_A(TRUE);  // ld a, TRUE
    LD_addr_A(wUnusedMusicF9Flag);  // ld [wUnusedMusicF9Flag], a
    return;
}

void MusicE2(void){
//  unused
//  params: 1
    GetMusicByte();
    LD_HL(CHANNEL_FIELD2C);  // ld hl, CHANNEL_FIELD2C
    ADD_HL_BC;  // add hl, bc
    LD_hl_A;  // ld [hl], a
    LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
    ADD_HL_BC;  // add hl, bc
    SET_hl(SOUND_UNKN_0B);  // set SOUND_UNKN_0B, [hl]
    return;
}

void Music_Vibrato(void){
//  vibrato
//  params: 2
//     1: [xx]
// delay in frames
//     2: [yz]
// y: extent
// z: rate (# frames per cycle)

// set vibrato flag?
    LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
    ADD_HL_BC;  // add hl, bc
    SET_hl(SOUND_VIBRATO);  // set SOUND_VIBRATO, [hl]
// start at lower frequency (extent is positive)
    LD_HL(CHANNEL_FLAGS3);  // ld hl, CHANNEL_FLAGS3
    ADD_HL_BC;  // add hl, bc
    RES_hl(SOUND_VIBRATO_DIR);  // res SOUND_VIBRATO_DIR, [hl]
// get delay
    GetMusicByte();
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
// get extent/rate
    GetMusicByte();
    LD_HL(CHANNEL_VIBRATO_EXTENT);  // ld hl, CHANNEL_VIBRATO_EXTENT
    ADD_HL_BC;  // add hl, bc
    LD_D_A;  // ld d, a
// get top nybble
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
// get bottom nybble
    LD_A_D;  // ld a, d
    AND_A(0xf);  // and $f
    LD_D_A;  // ld d, a
    SWAP_A;  // swap a
    OR_A_D;  // or d
    LD_hl_A;  // ld [hl], a
    return;
}

void Music_PitchSlide(void){
//  set the target for pitch slide
//  params: 2
//  note duration
//  target note
    GetMusicByte();
    LD_addr_A(wCurNoteDuration);  // ld [wCurNoteDuration], a

    GetMusicByte();
// pitch in e
    LD_D_A;  // ld d, a
    AND_A(0xf);  // and $f
    LD_E_A;  // ld e, a

// octave in d
    LD_A_D;  // ld a, d
    SWAP_A;  // swap a
    AND_A(0xf);  // and $f
    LD_D_A;  // ld d, a
    GetFrequency();
    LD_HL(CHANNEL_PITCH_SLIDE_TARGET);  // ld hl, CHANNEL_PITCH_SLIDE_TARGET
    ADD_HL_BC;  // add hl, bc
    LD_hl_E;  // ld [hl], e
    LD_HL(CHANNEL_PITCH_SLIDE_TARGET + 1);  // ld hl, CHANNEL_PITCH_SLIDE_TARGET + 1
    ADD_HL_BC;  // add hl, bc
    LD_hl_D;  // ld [hl], d
    LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
    ADD_HL_BC;  // add hl, bc
    SET_hl(SOUND_PITCH_SLIDE);  // set SOUND_PITCH_SLIDE, [hl]
    return;
}

void Music_PitchOffset(void){
//  tone
//  params: 1 (dw)
//  offset to add to each note frequency
    LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
    ADD_HL_BC;  // add hl, bc
    SET_hl(SOUND_PITCH_OFFSET);  // set SOUND_PITCH_OFFSET, [hl]
    LD_HL(CHANNEL_PITCH_OFFSET + 1);  // ld hl, CHANNEL_PITCH_OFFSET + 1
    ADD_HL_BC;  // add hl, bc
    GetMusicByte();
    LD_hld_A;  // ld [hld], a
    GetMusicByte();
    LD_hl_A;  // ld [hl], a
    return;
}

void MusicE7(void){
//  unused
//  params: 1
    LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
    ADD_HL_BC;  // add hl, bc
    SET_hl(SOUND_UNKN_0E);  // set SOUND_UNKN_0E, [hl]
    GetMusicByte();
    LD_HL(CHANNEL_FIELD29);  // ld hl, CHANNEL_FIELD29
    ADD_HL_BC;  // add hl, bc
    LD_hl_A;  // ld [hl], a
    return;
}

void Music_DutyCyclePattern(void){
//  sequence of 4 duty cycles to be looped
//  params: 1 (4 2-bit duty cycle arguments)
    LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
    ADD_HL_BC;  // add hl, bc
    SET_hl(SOUND_DUTY_LOOP);  // set SOUND_DUTY_LOOP, [hl] ; duty cycle looping
// sound duty sequence
    GetMusicByte();
    RRCA;  // rrca
    RRCA;  // rrca
    LD_HL(CHANNEL_DUTY_CYCLE_PATTERN);  // ld hl, CHANNEL_DUTY_CYCLE_PATTERN
    ADD_HL_BC;  // add hl, bc
    LD_hl_A;  // ld [hl], a
// update duty cycle
    AND_A(0xc0);  // and $c0 ; only uses top 2 bits
    LD_HL(CHANNEL_DUTY_CYCLE);  // ld hl, CHANNEL_DUTY_CYCLE
    ADD_HL_BC;  // add hl, bc
    LD_hl_A;  // ld [hl], a
    return;
}

void MusicE8(void){
//  unused
//  params: 1
    LD_HL(CHANNEL_FLAGS2);  // ld hl, CHANNEL_FLAGS2
    ADD_HL_BC;  // add hl, bc
    SET_hl(SOUND_UNKN_0D);  // set SOUND_UNKN_0D, [hl]
    GetMusicByte();
    LD_HL(CHANNEL_FIELD2A);  // ld hl, CHANNEL_FIELD2A
    ADD_HL_BC;  // add hl, bc
    LD_hl_A;  // ld [hl], a
    return;
}

void Music_ToggleSFX(void){
//  toggle something
//  params: none
    LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
    ADD_HL_BC;  // add hl, bc
    BIT_hl(SOUND_SFX);  // bit SOUND_SFX, [hl]
    IF_Z goto on;  // jr z, .on
    RES_hl(SOUND_SFX);  // res SOUND_SFX, [hl]
    return;

on:
    SET_hl(SOUND_SFX);  // set SOUND_SFX, [hl]
    return;
}

void Music_ToggleNoise(void){
//  toggle music noise sampling
//  can't be used as a straight toggle since the param is not read from on->off
//  params:
//      noise on: 1
//      noise off: 0
// check if noise sampling is on
    LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
    ADD_HL_BC;  // add hl, bc
    BIT_hl(SOUND_NOISE);  // bit SOUND_NOISE, [hl]
    IF_Z goto on;  // jr z, .on
// turn noise sampling off
    RES_hl(SOUND_NOISE);  // res SOUND_NOISE, [hl]
    return;

on:
// turn noise sampling on
    SET_hl(SOUND_NOISE);  // set SOUND_NOISE, [hl]
    GetMusicByte();
    LD_addr_A(wMusicNoiseSampleSet);  // ld [wMusicNoiseSampleSet], a
    return;
}

void Music_SFXToggleNoise(void){
//  toggle sfx noise sampling
//  params:
//     on: 1
//      off: 0
// check if noise sampling is on
    LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
    ADD_HL_BC;  // add hl, bc
    BIT_hl(SOUND_NOISE);  // bit SOUND_NOISE, [hl]
    IF_Z goto on;  // jr z, .on
// turn noise sampling off
    RES_hl(SOUND_NOISE);  // res SOUND_NOISE, [hl]
    return;

on:
// turn noise sampling on
    SET_hl(SOUND_NOISE);  // set SOUND_NOISE, [hl]
    GetMusicByte();
    LD_addr_A(wSFXNoiseSampleSet);  // ld [wSFXNoiseSampleSet], a
    return;
}

void Music_NoteType(void){
//  note length
//     # frames per 16th note
//  volume envelope: see Music_VolumeEnvelope
//  params: 2
// note length
    GetMusicByte();
    LD_HL(CHANNEL_NOTE_LENGTH);  // ld hl, CHANNEL_NOTE_LENGTH
    ADD_HL_BC;  // add hl, bc
    LD_hl_A;  // ld [hl], a
    LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
    maskbits(NUM_MUSIC_CHANS, 0);  // maskbits NUM_MUSIC_CHANS
    CP_A(CHAN4);  // cp CHAN4
    IF_Z return;
// volume envelope
    Music_VolumeEnvelope();  // call Music_VolumeEnvelope
    return;
}

void Music_PitchSweep(void){
//  update pitch sweep
//  params: 1
    GetMusicByte();
    LD_addr_A(wPitchSweep);  // ld [wPitchSweep], a
    LD_HL(CHANNEL_NOTE_FLAGS);  // ld hl, CHANNEL_NOTE_FLAGS
    ADD_HL_BC;  // add hl, bc
    SET_hl(NOTE_PITCH_SWEEP);  // set NOTE_PITCH_SWEEP, [hl]
    return;
}

void Music_DutyCycle(void){
//  duty cycle
//  params: 1
    GetMusicByte();
    RRCA;  // rrca
    RRCA;  // rrca
    AND_A(0xc0);  // and $c0
    LD_HL(CHANNEL_DUTY_CYCLE);  // ld hl, CHANNEL_DUTY_CYCLE
    ADD_HL_BC;  // add hl, bc
    LD_hl_A;  // ld [hl], a
    return;
}

void Music_VolumeEnvelope(void){
//  volume envelope
//  params: 1
//     hi: volume
//    lo: fade
    GetMusicByte();
    LD_HL(CHANNEL_VOLUME_ENVELOPE);  // ld hl, CHANNEL_VOLUME_ENVELOPE
    ADD_HL_BC;  // add hl, bc
    LD_hl_A;  // ld [hl], a
    return;
}

void Music_Tempo(void){
//  global tempo
//  params: 2
//     de: tempo
    GetMusicByte();
    LD_D_A;  // ld d, a
    GetMusicByte();
    LD_E_A;  // ld e, a
    SetGlobalTempo();
    return;
}

void Music_Octave(void){
//  set octave based on lo nybble of the command
    LD_HL(CHANNEL_OCTAVE);  // ld hl, CHANNEL_OCTAVE
    ADD_HL_BC;  // add hl, bc
    LD_A_addr(wCurMusicByte);  // ld a, [wCurMusicByte]
    AND_A(7);  // and 7
    LD_hl_A;  // ld [hl], a
    return;
}

void Music_Transpose(void){
//  set starting octave
//  this forces all notes up by the starting octave
//  params: 1
    GetMusicByte();
    LD_HL(CHANNEL_TRANSPOSITION);  // ld hl, CHANNEL_TRANSPOSITION
    ADD_HL_BC;  // add hl, bc
    LD_hl_A;  // ld [hl], a
    return;
}

void Music_StereoPanning(void){
//  stereo panning
//  params: 1
// stereo on?
    LD_A_addr(wOptions);  // ld a, [wOptions]
    BIT_A(STEREO);  // bit STEREO, a
    IF_NZ return Music_ForceStereoPanning();  // jr nz, Music_ForceStereoPanning
// skip param
    GetMusicByte();
    return;
}

void Music_ForceStereoPanning(void){
//  force panning
//  params: 1
    SetLRTracks();
    GetMusicByte();
    LD_HL(CHANNEL_TRACKS);  // ld hl, CHANNEL_TRACKS
    ADD_HL_BC;  // add hl, bc
    AND_A_hl;  // and [hl]
    LD_hl_A;  // ld [hl], a
    return;
}

void Music_Volume(void){
//  set volume
//  params: 1
//     see Volume
// read param even if it's not used
    GetMusicByte();
// is the song fading?
    LD_A_addr(wMusicFade);  // ld a, [wMusicFade]
    AND_A_A;  // and a
    IF_NZ return;
// reload param
    LD_A_addr(wCurMusicByte);  // ld a, [wCurMusicByte]
// set volume
    LD_addr_A(wVolume);  // ld [wVolume], a
    return;
}

void Music_TempoRelative(void){
//  set global tempo to current channel tempo +/- param
//  params: 1 signed
    GetMusicByte();
    LD_E_A;  // ld e, a
// check sign
    CP_A(0x80);  // cp $80
    IF_NC goto negative;  // jr nc, .negative
// positive
    LD_D(0);  // ld d, 0
    goto ok;  // jr .ok


negative:
    LD_D(-1);  // ld d, -1

ok:
    LD_HL(CHANNEL_TEMPO);  // ld hl, CHANNEL_TEMPO
    ADD_HL_BC;  // add hl, bc
    LD_A_hli;  // ld a, [hli]
    LD_H_hl;  // ld h, [hl]
    LD_L_A;  // ld l, a
    ADD_HL_DE;  // add hl, de
    LD_E_L;  // ld e, l
    LD_D_H;  // ld d, h
    SetGlobalTempo();
    return;
}

void Music_SFXPriorityOn(void){
//  turn sfx priority on
//  params: none
    LD_A(1);  // ld a, 1
    LD_addr_A(wSFXPriority);  // ld [wSFXPriority], a
    return;
}

void Music_SFXPriorityOff(void){
//  turn sfx priority off
//  params: none
    XOR_A_A;  // xor a
    LD_addr_A(wSFXPriority);  // ld [wSFXPriority], a
    return;
}

void Music_RestartChannel(void){
//  restart current channel from channel header (same bank)
//  params: 2 (5)
//  ll hh: pointer to new channel header
//     header format: 0x yy zz
//         x: channel # (0-3)
//         zzyy: pointer to new music data

// update music id
    LD_HL(CHANNEL_MUSIC_ID);  // ld hl, CHANNEL_MUSIC_ID
    ADD_HL_BC;  // add hl, bc
    LD_A_hli;  // ld a, [hli]
    LD_addr_A(wMusicID);  // ld [wMusicID], a
    LD_A_hl;  // ld a, [hl]
    LD_addr_A(wMusicID + 1);  // ld [wMusicID + 1], a
// update music bank
    LD_HL(CHANNEL_MUSIC_BANK);  // ld hl, CHANNEL_MUSIC_BANK
    ADD_HL_BC;  // add hl, bc
    LD_A_hl;  // ld a, [hl]
    LD_addr_A(wMusicBank);  // ld [wMusicBank], a
// get pointer to new channel header
    GetMusicByte();
    LD_L_A;  // ld l, a
    GetMusicByte();
    LD_H_A;  // ld h, a
    LD_E_hl;  // ld e, [hl]
    INC_HL;  // inc hl
    LD_D_hl;  // ld d, [hl]
    PUSH_BC;  // push bc ; save current channel
    LoadChannel();
    StartChannel();
    POP_BC;  // pop bc ; restore current channel
    return;
}

void Music_NewSong(void){
//  new song
//  params: 2
//     de: song id
    GetMusicByte();
    LD_E_A;  // ld e, a
    GetMusicByte();
    LD_D_A;  // ld d, a
    PUSH_BC;  // push bc
    v_PlayMusic();  // call _PlayMusic
    POP_BC;  // pop bc
    return;
}

void GetMusicByte(void){
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
    CCALL(av_LoadMusicByte);  // call _LoadMusicByte ; load data into [wCurMusicByte]
    INC_DE;  // inc de ; advance to next byte for next time this is called
    LD_HL(CHANNEL_MUSIC_ADDRESS);  // ld hl, CHANNEL_MUSIC_ADDRESS
    ADD_HL_BC;  // add hl, bc
    LD_A_E;  // ld a, e
    LD_hli_A;  // ld [hli], a
    LD_hl_D;  // ld [hl], d
    POP_DE;  // pop de
    POP_HL;  // pop hl
    LD_A_addr(wCurMusicByte);  // ld a, [wCurMusicByte]
    return;
}

void GetFrequency(void){
//  generate frequency
//  input:
//      d: octave
//     e: pitch
//  output:
//      de: frequency

//  get octave
// get starting octave
    LD_HL(CHANNEL_TRANSPOSITION);  // ld hl, CHANNEL_TRANSPOSITION
    ADD_HL_BC;  // add hl, bc
    LD_A_hl;  // ld a, [hl]
    SWAP_A;  // swap a ; hi nybble
    AND_A(0xf);  // and $f
// add current octave
    ADD_A_D;  // add d
    PUSH_AF;  // push af ; we'll use this later
// get starting octave
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
// get our octave
    POP_AF;  // pop af
// shift right by [7 - octave] bits

loop:
// [7 - octave] loops
    CP_A(0x7);  // cp $7
    IF_NC goto ok;  // jr nc, .ok
// sra de
    SRA_D;  // sra d
    RR_E;  // rr e
    INC_A;  // inc a
    goto loop;  // jr .loop


ok:
    LD_A_D;  // ld a, d
    AND_A(0x7);  // and $7 ; top 3 bits for frequency (11 total)
    LD_D_A;  // ld d, a
    return;
}

void SetNoteDuration(void){
//  input: a = note duration in 16ths
// store delay units in de
    INC_A;  // inc a
    LD_E_A;  // ld e, a
    LD_D(0);  // ld d, 0
    LD_HL(CHANNEL_NOTE_LENGTH);  // ld hl, CHANNEL_NOTE_LENGTH
    ADD_HL_BC;  // add hl, bc
    LD_A_hl;  // ld a, [hl]
// multiply NoteLength by delay units
    LD_L(0);  // ld l, 0 ; just multiply
    SetNoteDuration_Multiply();
    LD_A_L;  // ld a, l ; low
// store Tempo in de
    LD_HL(CHANNEL_TEMPO);  // ld hl, CHANNEL_TEMPO
    ADD_HL_BC;  // add hl, bc
    LD_E_hl;  // ld e, [hl]
    INC_HL;  // inc hl
    LD_D_hl;  // ld d, [hl]
// add ???? to the next result
    LD_HL(CHANNEL_FIELD16);  // ld hl, CHANNEL_FIELD16
    ADD_HL_BC;  // add hl, bc
    LD_L_hl;  // ld l, [hl]
// multiply Tempo by last result (NoteLength * LOW(delay))
    SetNoteDuration_Multiply();
// copy result to de
    LD_E_L;  // ld e, l
    LD_D_H;  // ld d, h
// store result in ????
    LD_HL(CHANNEL_FIELD16);  // ld hl, CHANNEL_FIELD16
    ADD_HL_BC;  // add hl, bc
    LD_hl_E;  // ld [hl], e
// store result in NoteDuration
    LD_HL(CHANNEL_NOTE_DURATION);  // ld hl, CHANNEL_NOTE_DURATION
    ADD_HL_BC;  // add hl, bc
    LD_hl_D;  // ld [hl], d
    return;
}

void SetNoteDuration_Multiply(void){
//  multiplies a and de
//  adds the result to l
//  stores the result in hl
    LD_H(0);  // ld h, 0

loop:
// halve a
    SRL_A;  // srl a
// is there a remainder?
    IF_NC goto skip;  // jr nc, .skip
// add it to the result
    ADD_HL_DE;  // add hl, de

skip:
// add de, de
    SLA_E;  // sla e
    RL_D;  // rl d
// are we done?
    AND_A_A;  // and a
    IF_NZ goto loop;  // jr nz, .loop
    return;
}

void SetGlobalTempo(void){
    PUSH_BC;  // push bc ; save current channel
// are we dealing with music or sfx?
    LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
    CP_A(CHAN5);  // cp CHAN5
    IF_NC goto sfxchannels;  // jr nc, .sfxchannels
    LD_BC(wChannel1);  // ld bc, wChannel1
    Tempo();
    LD_BC(wChannel2);  // ld bc, wChannel2
    Tempo();
    LD_BC(wChannel3);  // ld bc, wChannel3
    Tempo();
    LD_BC(wChannel4);  // ld bc, wChannel4
    Tempo();
    goto end;  // jr .end


sfxchannels:
    LD_BC(wChannel5);  // ld bc, wChannel5
    Tempo();
    LD_BC(wChannel6);  // ld bc, wChannel6
    Tempo();
    LD_BC(wChannel7);  // ld bc, wChannel7
    Tempo();
    LD_BC(wChannel8);  // ld bc, wChannel8
    Tempo();

end:
    POP_BC;  // pop bc ; restore current channel
    return;
}

void Tempo(void){
//  input:
//      de: note length
// update Tempo
    LD_HL(CHANNEL_TEMPO);  // ld hl, CHANNEL_TEMPO
    ADD_HL_BC;  // add hl, bc
    LD_hl_E;  // ld [hl], e
    INC_HL;  // inc hl
    LD_hl_D;  // ld [hl], d
// clear ????
    XOR_A_A;  // xor a
    LD_HL(CHANNEL_FIELD16);  // ld hl, CHANNEL_FIELD16
    ADD_HL_BC;  // add hl, bc
    LD_hl_A;  // ld [hl], a
    return;
}

void StartChannel(void){
    SetLRTracks();
    LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
    ADD_HL_BC;  // add hl, bc
    SET_hl(SOUND_CHANNEL_ON);  // set SOUND_CHANNEL_ON, [hl] ; turn channel on
    return;
}

void SetLRTracks(void){
//  set tracks for a the current channel to default
//  seems to be redundant since this is overwritten by stereo data later
    PUSH_DE;  // push de
// store current channel in de
    LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
    maskbits(NUM_MUSIC_CHANS, 0);  // maskbits NUM_MUSIC_CHANS
    LD_E_A;  // ld e, a
    LD_D(0);  // ld d, 0
    GetLRTracks();
    ADD_HL_DE;  // add hl, de ; de = channel 0-3
    LD_A_hl;  // ld a, [hl]
// load lr tracks into Tracks
    LD_HL(CHANNEL_TRACKS);  // ld hl, CHANNEL_TRACKS
    ADD_HL_BC;  // add hl, bc
    LD_hl_A;  // ld [hl], a
    POP_DE;  // pop de
    return;
}

void v_PlayMusic(void){
//  load music
    MusicOff();
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
    LoadMusicByte(); // store first byte of music header in a
    RLCA;  // rlca
    RLCA;  // rlca
    maskbits(NUM_MUSIC_CHANS, 0);  // maskbits NUM_MUSIC_CHANS
    INC_A;  // inc a

loop:
//  start playing channels
    PUSH_AF;  // push af
    LoadChannel();
    StartChannel();
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
    MusicOn();
    return;
}

void v_PlayCry(void){
//  Play cry de using parameters:
//     wCryPitch
//     wCryLength

    MusicOff();

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
    LoadMusicByte();
// Top 2 bits contain the number of channels
    RLCA;  // rlca
    RLCA;  // rlca
    maskbits(NUM_MUSIC_CHANS, 0);  // maskbits NUM_MUSIC_CHANS

//  For each channel:
    INC_A;  // inc a

loop:
    PUSH_AF;  // push af
    LoadChannel(); // bc = current channel

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
    StartChannel();
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
    LD_A(1);  // ld a, 1 ; stop playing music
    LD_addr_A(wSFXPriority);  // ld [wSFXPriority], a
    MusicOn();
    return;
}

void v_PlaySFX(void){
//  clear channels if they aren't already
    MusicOff();
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
//  start reading sfx header for # chs
    LD_HL(wMusicID);  // ld hl, wMusicID
    LD_hl_E;  // ld [hl], e
    INC_HL;  // inc hl
    LD_hl_D;  // ld [hl], d
    LD_HL(mSFX);  // ld hl, SFX
    ADD_HL_DE;  // add hl, de ; three
    ADD_HL_DE;  // add hl, de ; byte
    ADD_HL_DE;  // add hl, de ; pointers
// get bank
    LD_A_hli;  // ld a, [hli]
    LD_addr_A(wMusicBank);  // ld [wMusicBank], a
// get address
    LD_E_hl;  // ld e, [hl]
    INC_HL;  // inc hl
    LD_D_hl;  // ld d, [hl]
// get # channels
    LoadMusicByte();
    RLCA;  // rlca ; top 2
    RLCA;  // rlca ; bits
    maskbits(NUM_MUSIC_CHANS, 0);  // maskbits NUM_MUSIC_CHANS
    INC_A;  // inc a ; # channels -> # loops

startchannels:
    PUSH_AF;  // push af
    LoadChannel(); // bc = current channel
    LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
    ADD_HL_BC;  // add hl, bc
    SET_hl(SOUND_SFX);  // set SOUND_SFX, [hl]
    StartChannel();
    POP_AF;  // pop af
    DEC_A;  // dec a
    IF_NZ goto startchannels;  // jr nz, .startchannels
    MusicOn();
    XOR_A_A;  // xor a
    LD_addr_A(wSFXPriority);  // ld [wSFXPriority], a
    return;
}

void PlayStereoSFX(void){
//  play sfx de

    MusicOff();

//  standard procedure if stereo's off
    LD_A_addr(wOptions);  // ld a, [wOptions]
    BIT_A(STEREO);  // bit STEREO, a
    IF_Z return v_PlaySFX();  // jp z, _PlaySFX

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
    LoadMusicByte();
    RLCA;  // rlca
    RLCA;  // rlca
    maskbits(NUM_MUSIC_CHANS, 0);  // maskbits NUM_MUSIC_CHANS
    INC_A;  // inc a


loop:
    PUSH_AF;  // push af
    LoadChannel();

    LD_HL(CHANNEL_FLAGS1);  // ld hl, CHANNEL_FLAGS1
    ADD_HL_BC;  // add hl, bc
    SET_hl(SOUND_SFX);  // set SOUND_SFX, [hl]

    PUSH_DE;  // push de
// get tracks for this channel
    LD_A_addr(wCurChannel);  // ld a, [wCurChannel]
    maskbits(NUM_MUSIC_CHANS, 0);  // maskbits NUM_MUSIC_CHANS
    LD_E_A;  // ld e, a
    LD_D(0);  // ld d, 0
    GetLRTracks();
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
    MusicOn();
    return;
}

void LoadChannel(void){
//  input: de = audio pointer
//  sets bc to current channel pointer
    LoadMusicByte();
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
    ChannelInit();
// load music pointer
    LD_HL(CHANNEL_MUSIC_ADDRESS);  // ld hl, CHANNEL_MUSIC_ADDRESS
    ADD_HL_BC;  // add hl, bc
    LoadMusicByte();
    LD_hli_A;  // ld [hli], a
    INC_DE;  // inc de
    LoadMusicByte();
    LD_hl_A;  // ld [hl], a
    INC_DE;  // inc de
// load music id
    LD_HL(CHANNEL_MUSIC_ID);  // ld hl, CHANNEL_MUSIC_ID
    ADD_HL_BC;  // add hl, bc
    LD_A_addr(wMusicID);  // ld a, [wMusicID]
    LD_hli_A;  // ld [hli], a
    LD_A_addr(wMusicID + 1);  // ld a, [wMusicID + 1]
    LD_hl_A;  // ld [hl], a
// load music bank
    LD_HL(CHANNEL_MUSIC_BANK);  // ld hl, CHANNEL_MUSIC_BANK
    ADD_HL_BC;  // add hl, bc
    LD_A_addr(wMusicBank);  // ld a, [wMusicBank]
    LD_hl_A;  // ld [hl], a
    return;
}

void ChannelInit(void){
//  make sure channel is cleared
//  set default tempo and note length in case nothing is loaded
//  input:
//    bc = channel struct pointer
    PUSH_DE;  // push de
    XOR_A_A;  // xor a
// get channel struct location and length
    LD_HL(CHANNEL_MUSIC_ID);  // ld hl, CHANNEL_MUSIC_ID ; start
    ADD_HL_BC;  // add hl, bc
    LD_E(CHANNEL_STRUCT_LENGTH);  // ld e, CHANNEL_STRUCT_LENGTH ; channel struct length
// clear channel

loop:
    LD_hli_A;  // ld [hli], a
    DEC_E;  // dec e
    IF_NZ goto loop;  // jr nz, .loop
// set tempo to default ($100)
    LD_HL(CHANNEL_TEMPO);  // ld hl, CHANNEL_TEMPO
    ADD_HL_BC;  // add hl, bc
    XOR_A_A;  // xor a
    LD_hli_A;  // ld [hli], a
    INC_A;  // inc a
    LD_hl_A;  // ld [hl], a
// set note length to default ($1) (fast)
    LD_HL(CHANNEL_NOTE_LENGTH);  // ld hl, CHANNEL_NOTE_LENGTH
    ADD_HL_BC;  // add hl, bc
    LD_hl_A;  // ld [hl], a
    POP_DE;  // pop de
}

void LoadMusicByte(void){
//  input:
//    de = current music address
//  output:
//    a = wCurMusicByte
    LD_A_addr(wMusicBank);  // ld a, [wMusicBank]
    CCALL(av_LoadMusicByte);  // call _LoadMusicByte
    LD_A_addr(wCurMusicByte);  // ld a, [wCurMusicByte]
}

// INCLUDE "audio/notes.asm"

// INCLUDE "audio/wave_samples.asm"

// INCLUDE "audio/drumkits.asm"

void GetLRTracks(void){
//  gets the default sound l/r channels
//  stores mono/stereo table in hl
    LD_A_addr(wOptions);  // ld a, [wOptions]
    BIT_A(STEREO);  // bit STEREO, a
// made redundant, could have had a purpose in gold
    IF_NZ goto stereo;  // jr nz, .stereo
    LD_HL(mMonoTracks);  // ld hl, MonoTracks
    return;

stereo:
    LD_HL(mStereoTracks);  // ld hl, StereoTracks
    return;
}

void MonoTracks(void){
//  bit corresponds to track #
//  hi: left channel
//  lo: right channel
    //db ['0x11', '0x22', '0x44', '0x88'];  // db $11, $22, $44, $88
}

void StereoTracks(void){
//  made redundant
//  seems to be modified on a per-song basis
    //db ['0x11', '0x22', '0x44', '0x88'];  // db $11, $22, $44, $88
}

void ChannelPointers(void){
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
}

void ClearChannels(void){
//  runs ClearChannel for all 4 channels
//  doesn't seem to be used, but functionally identical to InitSound
    static uint16_t noiseReg[4] = {rNR10,
                                    rNR20,
                                    rNR30,
                                    rNR40};
    /*LD_HL(rNR50);  // ld hl, rNR50
    XOR_A_A;  // xor a
    LD_hli_A;  // ld [hli], a
    LD_hli_A;  // ld [hli], a
    LD_A(0x80);  // ld a, $80
    LD_hli_A;  // ld [hli], a
    LD_HL(rNR10);  // ld hl, rNR10
    LD_E(NUM_MUSIC_CHANS);  // ld e, NUM_MUSIC_CHANS

loop:
    ClearChannel();
    DEC_E;  // dec e
    IF_NZ goto loop;  // jr nz, .loop
    return;*/
    gb_write(rNR50, 0);
    gb_write(rNR51, 0);
    gb_write(rNR52, 0x80);
    for(int i = 0; i < NUM_MUSIC_CHANS; i++) ClearChannel(noiseReg[i]);
}

void ClearChannel(uint16_t nReg){
//  input: hl = beginning hw sound register (rNR10, rNR20, rNR30, rNR40)
//  output: 00 00 80 00 80

//    sound channel   1      2      3      4
    /*XOR_A_A;  // xor a
    LD_hli_A;  // ld [hli], a ; rNR10, rNR20, rNR30, rNR40 ; sweep = 0

    LD_hli_A;  // ld [hli], a ; rNR11, rNR21, rNR31, rNR41 ; length/wavepattern = 0
    LD_A(0x8);  // ld a, $8
    LD_hli_A;  // ld [hli], a ; rNR12, rNR22, rNR32, rNR42 ; envelope = 0
    XOR_A_A;  // xor a
    LD_hli_A;  // ld [hli], a ; rNR13, rNR23, rNR33, rNR43 ; frequency lo = 0
    LD_A(0x80);  // ld a, $80
    LD_hli_A;  // ld [hli], a ; rNR14, rNR24, rNR34, rNR44 ; restart sound (freq hi = 0)*/
    gb_write(nReg++, 0);  // rNR10, rNR20, rNR30, rNR40 ; sweep = 0
    gb_write(nReg++, 0);  // rNR11, rNR21, rNR31, rNR41 ; length/wavepattern = 0
    gb_write(nReg++, 8);  // rNR12, rNR22, rNR32, rNR42 ; envelope = 0
    gb_write(nReg++, 0);  // rNR13, rNR23, rNR33, rNR43 ; frequency lo = 0
    gb_write(nReg++, 0x80);  // rNR14, rNR24, rNR34, rNR44 ; restart sound (freq hi = 0)
}

void PlayTrainerEncounterMusic(void){
    SET_PC(0xE900AU);
//  input: e = trainer type
// turn fade off
    XOR_A_A;  // xor a
    LD_addr_A(wMusicFade);  // ld [wMusicFade], a
// play nothing for one frame
    PUSH_DE;  // push de
    LD_DE(MUSIC_NONE);  // ld de, MUSIC_NONE
    CCALL(aPlayMusic);  // call PlayMusic
    CALL(mDelayFrame);  // call DelayFrame
// play new song
    CCALL(aMaxVolume);  // call MaxVolume
    POP_DE;  // pop de
    LD_D(0x00);  // ld d, $00
    LD_HL(mTrainerEncounterMusic);  // ld hl, TrainerEncounterMusic
    ADD_HL_DE;  // add hl, de
    LD_E_hl;  // ld e, [hl]
    CCALL(aPlayMusic);  // call PlayMusic
    RET;  // ret
}
