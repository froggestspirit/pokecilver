#include "../constants.h"

//  Functions relating to the timer interrupt and the real-time-clock.

void Timer(void){
//  //  unreferenced
	RET;  // ret

}

void LatchClock(void){
//  latch clock counter data
	LD_A(0);  // ld a, 0
	LD_addr_A(MBC3LatchClock);  // ld [MBC3LatchClock], a
	LD_A(1);  // ld a, 1
	LD_addr_A(MBC3LatchClock);  // ld [MBC3LatchClock], a
	RET;  // ret

}

void UpdateTime(void){
	SET_PC(0x0467U);
	CCALL(aGetClock);  // call GetClock
	CCALL(aFixDays);  // call FixDays
	CCALL(aFixTime);  // call FixTime
	FARCALL(aGetTimeOfDay);  // farcall GetTimeOfDay
	RET;  // ret

}

void GetClock(void){
//  store clock data in hRTCDayHi-hRTCSeconds

//  enable clock r/w
	LD_A(SRAM_ENABLE);  // ld a, SRAM_ENABLE
	LD_addr_A(MBC3SRamEnable);  // ld [MBC3SRamEnable], a

//  clock data is 'backwards' in hram

	CCALL(aLatchClock);  // call LatchClock
	LD_HL(MBC3SRamBank);  // ld hl, MBC3SRamBank
	LD_DE(MBC3RTC);  // ld de, MBC3RTC

	LD_hl(RTC_S);  // ld [hl], RTC_S
	LD_A_de;  // ld a, [de]
	maskbits(60, 0);  // maskbits 60
	LDH_addr_A(hRTCSeconds);  // ldh [hRTCSeconds], a

	LD_hl(RTC_M);  // ld [hl], RTC_M
	LD_A_de;  // ld a, [de]
	maskbits(60, 0);  // maskbits 60
	LDH_addr_A(hRTCMinutes);  // ldh [hRTCMinutes], a

	LD_hl(RTC_H);  // ld [hl], RTC_H
	LD_A_de;  // ld a, [de]
	maskbits(24, 0);  // maskbits 24
	LDH_addr_A(hRTCHours);  // ldh [hRTCHours], a

	LD_hl(RTC_DL);  // ld [hl], RTC_DL
	LD_A_de;  // ld a, [de]
	LDH_addr_A(hRTCDayLo);  // ldh [hRTCDayLo], a

	LD_hl(RTC_DH);  // ld [hl], RTC_DH
	LD_A_de;  // ld a, [de]
	LDH_addr_A(hRTCDayHi);  // ldh [hRTCDayHi], a

//  unlatch clock / disable clock r/w
	CCALL(aCloseSRAM);  // call CloseSRAM
	RET;  // ret

}

void FixDays(void){
//  fix day count
//  mod by 140

//  check if day count > 255 (bit 8 set)
	LDH_A_addr(hRTCDayHi);  // ldh a, [hRTCDayHi] ; DH
	BIT_A(0);  // bit 0, a
	IF_Z goto daylo;  // jr z, .daylo
//  reset dh (bit 8)
	RES_A(0);  // res 0, a
	LDH_addr_A(hRTCDayHi);  // ldh [hRTCDayHi], a

//  mod 140
//  mod twice since bit 8 (DH) was set
	LDH_A_addr(hRTCDayLo);  // ldh a, [hRTCDayLo]

modh:
	SUB_A(140);  // sub 140
	IF_NC goto modh;  // jr nc, .modh

modl:
	SUB_A(140);  // sub 140
	IF_NC goto modl;  // jr nc, .modl
	ADD_A(140);  // add 140

//  update dl
	LDH_addr_A(hRTCDayLo);  // ldh [hRTCDayLo], a

//  flag for sRTCStatusFlags
	LD_A(0b01000000);  // ld a, %01000000
	goto set;  // jr .set


daylo:
//  quit if fewer than 140 days have passed
	LDH_A_addr(hRTCDayLo);  // ldh a, [hRTCDayLo]
	CP_A(140);  // cp 140
	IF_C goto quit;  // jr c, .quit

//  mod 140

mod:
	SUB_A(140);  // sub 140
	IF_NC goto mod;  // jr nc, .mod
	ADD_A(140);  // add 140

//  update dl
	LDH_addr_A(hRTCDayLo);  // ldh [hRTCDayLo], a

//  flag for sRTCStatusFlags
	LD_A(0b00100000);  // ld a, %00100000


set:
//  update clock with modded day value
	PUSH_AF;  // push af
	CCALL(aSetClock);  // call SetClock
	POP_AF;  // pop af
	SCF;  // scf
	RET;  // ret


quit:
	CCF;  // ccf
	XOR_A_A;  // xor a
	RET;  // ret

}

void FixTime(void){
//  add ingame time (set at newgame) to current time
//  store time in wCurDay, hHours, hMinutes, hSeconds

//  second
	LDH_A_addr(hRTCSeconds);  // ldh a, [hRTCSeconds]
	LD_C_A;  // ld c, a
	LD_A_addr(wStartSecond);  // ld a, [wStartSecond]
	ADD_A_C;  // add c
	SUB_A(60);  // sub 60
	IF_NC goto updatesec;  // jr nc, .updatesec
	ADD_A(60);  // add 60

updatesec:
	LDH_addr_A(hSeconds);  // ldh [hSeconds], a

//  minute
	CCF;  // ccf ; carry is set, so turn it off
	LDH_A_addr(hRTCMinutes);  // ldh a, [hRTCMinutes]
	LD_C_A;  // ld c, a
	LD_A_addr(wStartMinute);  // ld a, [wStartMinute]
	ADC_A_C;  // adc c
	SUB_A(60);  // sub 60
	IF_NC goto updatemin;  // jr nc, .updatemin
	ADD_A(60);  // add 60

updatemin:
	LDH_addr_A(hMinutes);  // ldh [hMinutes], a

//  hour
	CCF;  // ccf ; carry is set, so turn it off
	LDH_A_addr(hRTCHours);  // ldh a, [hRTCHours]
	LD_C_A;  // ld c, a
	LD_A_addr(wStartHour);  // ld a, [wStartHour]
	ADC_A_C;  // adc c
	SUB_A(24);  // sub 24
	IF_NC goto updatehr;  // jr nc, .updatehr
	ADD_A(24);  // add 24

updatehr:
	LDH_addr_A(hHours);  // ldh [hHours], a

//  day
	CCF;  // ccf ; carry is set, so turn it off
	LDH_A_addr(hRTCDayLo);  // ldh a, [hRTCDayLo]
	LD_C_A;  // ld c, a
	LD_A_addr(wStartDay);  // ld a, [wStartDay]
	ADC_A_C;  // adc c
	LD_addr_A(wCurDay);  // ld [wCurDay], a
	RET;  // ret

}

void InitTimeOfDay(void){
	SET_PC(0x0519U);
	XOR_A_A;  // xor a
	LD_addr_A(wStringBuffer2);  // ld [wStringBuffer2], a
	LD_A(0);  // ld a, 0 ; useless
	LD_addr_A(wStringBuffer2 + 3);  // ld [wStringBuffer2 + 3], a
	JR(mInitTime);  // jr InitTime

}

void InitDayOfWeek(void){
	SET_PC(0x0524U);
	CALL(mUpdateTime);  // call UpdateTime
	LDH_A_addr(hHours);  // ldh a, [hHours]
	LD_addr_A(wStringBuffer2 + 1);  // ld [wStringBuffer2 + 1], a
	LDH_A_addr(hMinutes);  // ldh a, [hMinutes]
	LD_addr_A(wStringBuffer2 + 2);  // ld [wStringBuffer2 + 2], a
	LDH_A_addr(hSeconds);  // ldh a, [hSeconds]
	LD_addr_A(wStringBuffer2 + 3);  // ld [wStringBuffer2 + 3], a
	JR(mInitTime);  // jr InitTime ; useless

}

void InitTime(void){
	SET_PC(0x0538U);
	FARCALL(av_InitTime);  // farcall _InitTime
	RET;  // ret

}

void ClearClock(void){
	SET_PC(0x053FU);
	CALL(mClearClock_ClearhRTC);  // call .ClearhRTC
	CCALL(aSetClock);  // call SetClock
	RET;  // ret


ClearhRTC:
	SET_PC(0x0546U);
	XOR_A_A;  // xor a
	LDH_addr_A(hRTCSeconds);  // ldh [hRTCSeconds], a
	LDH_addr_A(hRTCMinutes);  // ldh [hRTCMinutes], a
	LDH_addr_A(hRTCHours);  // ldh [hRTCHours], a
	LDH_addr_A(hRTCDayLo);  // ldh [hRTCDayLo], a
	LDH_addr_A(hRTCDayHi);  // ldh [hRTCDayHi], a
	RET;  // ret

}

void SetClock(void){
//  set clock data from hram

//  enable clock r/w
	LD_A(SRAM_ENABLE);  // ld a, SRAM_ENABLE
	LD_addr_A(MBC3SRamEnable);  // ld [MBC3SRamEnable], a

//  set clock data
//  stored 'backwards' in hram

	CCALL(aLatchClock);  // call LatchClock
	LD_HL(MBC3SRamBank);  // ld hl, MBC3SRamBank
	LD_DE(MBC3RTC);  // ld de, MBC3RTC

//  seems to be a halt check that got partially commented out
//  this block is totally pointless
	LD_hl(RTC_DH);  // ld [hl], RTC_DH
	LD_A_de;  // ld a, [de]
	BIT_A(6);  // bit 6, a ; halt
	LD_de_A;  // ld [de], a

//  seconds
	LD_hl(RTC_S);  // ld [hl], RTC_S
	LDH_A_addr(hRTCSeconds);  // ldh a, [hRTCSeconds]
	LD_de_A;  // ld [de], a
//  minutes
	LD_hl(RTC_M);  // ld [hl], RTC_M
	LDH_A_addr(hRTCMinutes);  // ldh a, [hRTCMinutes]
	LD_de_A;  // ld [de], a
//  hours
	LD_hl(RTC_H);  // ld [hl], RTC_H
	LDH_A_addr(hRTCHours);  // ldh a, [hRTCHours]
	LD_de_A;  // ld [de], a
//  day lo
	LD_hl(RTC_DL);  // ld [hl], RTC_DL
	LDH_A_addr(hRTCDayLo);  // ldh a, [hRTCDayLo]
	LD_de_A;  // ld [de], a
//  day hi
	LD_hl(RTC_DH);  // ld [hl], RTC_DH
	LDH_A_addr(hRTCDayHi);  // ldh a, [hRTCDayHi]
	RES_A(6);  // res 6, a ; make sure timer is active
	LD_de_A;  // ld [de], a

//  cleanup
	CCALL(aCloseSRAM);  // call CloseSRAM ; unlatch clock, disable clock r/w
	RET;  // ret

}

void ClearRTCStatus(void){
//  //  unreferenced
//  clear sRTCStatusFlags
	XOR_A_A;  // xor a
	PUSH_AF;  // push af
	LD_A(BANK(sRTCStatusFlags));  // ld a, BANK(sRTCStatusFlags)
	CCALL(aOpenSRAM);  // call OpenSRAM
	POP_AF;  // pop af
	LD_addr_A(sRTCStatusFlags);  // ld [sRTCStatusFlags], a
	CCALL(aCloseSRAM);  // call CloseSRAM
	RET;  // ret

}

void RecordRTCStatus(void){
//  append flags to sRTCStatusFlags
	LD_HL(sRTCStatusFlags);  // ld hl, sRTCStatusFlags
	PUSH_AF;  // push af
	LD_A(BANK(sRTCStatusFlags));  // ld a, BANK(sRTCStatusFlags)
	CCALL(aOpenSRAM);  // call OpenSRAM
	POP_AF;  // pop af
	OR_A_hl;  // or [hl]
	LD_hl_A;  // ld [hl], a
	CCALL(aCloseSRAM);  // call CloseSRAM
	RET;  // ret

}

void CheckRTCStatus(void){
//  check sRTCStatusFlags
	LD_A(BANK(sRTCStatusFlags));  // ld a, BANK(sRTCStatusFlags)
	CCALL(aOpenSRAM);  // call OpenSRAM
	LD_A_addr(sRTCStatusFlags);  // ld a, [sRTCStatusFlags]
	CCALL(aCloseSRAM);  // call CloseSRAM
	RET;  // ret

}
