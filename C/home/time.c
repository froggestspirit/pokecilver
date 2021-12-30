#include "../constants.h"

//  Functions relating to the timer interrupt and the real-time-clock.

int Timer(){  //  unreferenced
	RET;  // ret

}

int LatchClock(){
//  latch clock counter data
	LD_A(0);  // ld a, 0
	LD_addr_A(MBC3LatchClock);  // ld [MBC3LatchClock], a
	LD_A(1);  // ld a, 1
	LD_addr_A(MBC3LatchClock);  // ld [MBC3LatchClock], a
	RET;  // ret

}

int UpdateTime(){
	CALL(mGetClock);  // call GetClock
	CALL(mFixDays);  // call FixDays
	CALL(mFixTime);  // call FixTime
	FARCALL(aGetTimeOfDay);  // farcall GetTimeOfDay
	RET;  // ret

}

int GetClock(){
//  store clock data in hRTCDayHi-hRTCSeconds

//  enable clock r/w
	LD_A(SRAM_ENABLE);  // ld a, SRAM_ENABLE
	LD_addr_A(MBC3SRamEnable);  // ld [MBC3SRamEnable], a

//  clock data is 'backwards' in hram

	CALL(mLatchClock);  // call LatchClock
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
	CALL(mCloseSRAM);  // call CloseSRAM
	RET;  // ret

}

int FixDays(){
//  fix day count
//  mod by 140

//  check if day count > 255 (bit 8 set)
	LDH_A_addr(hRTCDayHi);  // ldh a, [hRTCDayHi] ; DH
	BIT_A(0);  // bit 0, a
	IF_Z goto _daylo;  // jr z, .daylo
//  reset dh (bit 8)
	RES_A(0);  // res 0, a
	LDH_addr_A(hRTCDayHi);  // ldh [hRTCDayHi], a

//  mod 140
//  mod twice since bit 8 (DH) was set
	LDH_A_addr(hRTCDayLo);  // ldh a, [hRTCDayLo]

_modh:
	SET_PC(0x04B4U);
	SUB_A(140);  // sub 140
	IF_NC goto _modh;  // jr nc, .modh

_modl:
	SET_PC(0x04B8U);
	SUB_A(140);  // sub 140
	IF_NC goto _modl;  // jr nc, .modl
	ADD_A(140);  // add 140

//  update dl
	LDH_addr_A(hRTCDayLo);  // ldh [hRTCDayLo], a

//  flag for sRTCStatusFlags
	LD_A(0b01000000);  // ld a, %01000000
	goto _set;  // jr .set


_daylo:
	SET_PC(0x04C4U);
//  quit if fewer than 140 days have passed
	LDH_A_addr(hRTCDayLo);  // ldh a, [hRTCDayLo]
	CP_A(140);  // cp 140
	IF_C goto _quit;  // jr c, .quit

//  mod 140

_mod:
	SET_PC(0x04CAU);
	SUB_A(140);  // sub 140
	IF_NC goto _mod;  // jr nc, .mod
	ADD_A(140);  // add 140

//  update dl
	LDH_addr_A(hRTCDayLo);  // ldh [hRTCDayLo], a

//  flag for sRTCStatusFlags
	LD_A(0b00100000);  // ld a, %00100000


_set:
	SET_PC(0x04D4U);
//  update clock with modded day value
	PUSH_AF;  // push af
	CALL(mSetClock);  // call SetClock
	POP_AF;  // pop af
	SCF;  // scf
	RET;  // ret


_quit:
	SET_PC(0x04DBU);
	CCF;  // ccf
	XOR_A_A;  // xor a
	RET;  // ret

}

int FixTime(){
//  add ingame time (set at newgame) to current time
//  store time in wCurDay, hHours, hMinutes, hSeconds

//  second
	LDH_A_addr(hRTCSeconds);  // ldh a, [hRTCSeconds]
	LD_C_A;  // ld c, a
	LD_A_addr(wStartSecond);  // ld a, [wStartSecond]
	ADD_A_C;  // add c
	SUB_A(60);  // sub 60
	IF_NC goto _updatesec;  // jr nc, .updatesec
	ADD_A(60);  // add 60

_updatesec:
	SET_PC(0x04EBU);
	LDH_addr_A(hSeconds);  // ldh [hSeconds], a

//  minute
	CCF;  // ccf ; carry is set, so turn it off
	LDH_A_addr(hRTCMinutes);  // ldh a, [hRTCMinutes]
	LD_C_A;  // ld c, a
	LD_A_addr(wStartMinute);  // ld a, [wStartMinute]
	ADC_A_C;  // adc c
	SUB_A(60);  // sub 60
	IF_NC goto _updatemin;  // jr nc, .updatemin
	ADD_A(60);  // add 60

_updatemin:
	SET_PC(0x04FBU);
	LDH_addr_A(hMinutes);  // ldh [hMinutes], a

//  hour
	CCF;  // ccf ; carry is set, so turn it off
	LDH_A_addr(hRTCHours);  // ldh a, [hRTCHours]
	LD_C_A;  // ld c, a
	LD_A_addr(wStartHour);  // ld a, [wStartHour]
	ADC_A_C;  // adc c
	SUB_A(24);  // sub 24
	IF_NC goto _updatehr;  // jr nc, .updatehr
	ADD_A(24);  // add 24

_updatehr:
	SET_PC(0x050BU);
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

int InitTimeOfDay(){
	XOR_A_A;  // xor a
	LD_addr_A(wStringBuffer2);  // ld [wStringBuffer2], a
	LD_A(0);  // ld a, 0 ; useless
	LD_addr_A(wStringBuffer2 + 3);  // ld [wStringBuffer2 + 3], a
	JR(mInitTime);  // jr InitTime

}

int InitDayOfWeek(){
	CALL(mUpdateTime);  // call UpdateTime
	LDH_A_addr(hHours);  // ldh a, [hHours]
	LD_addr_A(wStringBuffer2 + 1);  // ld [wStringBuffer2 + 1], a
	LDH_A_addr(hMinutes);  // ldh a, [hMinutes]
	LD_addr_A(wStringBuffer2 + 2);  // ld [wStringBuffer2 + 2], a
	LDH_A_addr(hSeconds);  // ldh a, [hSeconds]
	LD_addr_A(wStringBuffer2 + 3);  // ld [wStringBuffer2 + 3], a
	JR(mInitTime);  // jr InitTime ; useless

}

int InitTime(){
	FARCALL(a_InitTime);  // farcall _InitTime
	RET;  // ret

}

int ClearClock(){
	CALL(mClearClock_ClearhRTC);  // call .ClearhRTC
	CALL(mSetClock);  // call SetClock
	RET;  // ret


_ClearhRTC:
	SET_PC(0x0546U);
	XOR_A_A;  // xor a
	LDH_addr_A(hRTCSeconds);  // ldh [hRTCSeconds], a
	LDH_addr_A(hRTCMinutes);  // ldh [hRTCMinutes], a
	LDH_addr_A(hRTCHours);  // ldh [hRTCHours], a
	LDH_addr_A(hRTCDayLo);  // ldh [hRTCDayLo], a
	LDH_addr_A(hRTCDayHi);  // ldh [hRTCDayHi], a
	RET;  // ret

}

int SetClock(){
//  set clock data from hram

//  enable clock r/w
	LD_A(SRAM_ENABLE);  // ld a, SRAM_ENABLE
	LD_addr_A(MBC3SRamEnable);  // ld [MBC3SRamEnable], a

//  set clock data
//  stored 'backwards' in hram

	CALL(mLatchClock);  // call LatchClock
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
	CALL(mCloseSRAM);  // call CloseSRAM ; unlatch clock, disable clock r/w
	RET;  // ret

}

int ClearRTCStatus(){  //  unreferenced
//  clear sRTCStatusFlags
	XOR_A_A;  // xor a
	PUSH_AF;  // push af
	LD_A(BANK(sRTCStatusFlags));  // ld a, BANK(sRTCStatusFlags)
	CALL(mOpenSRAM);  // call OpenSRAM
	POP_AF;  // pop af
	LD_addr_A(sRTCStatusFlags);  // ld [sRTCStatusFlags], a
	CALL(mCloseSRAM);  // call CloseSRAM
	RET;  // ret

}

int RecordRTCStatus(){
//  append flags to sRTCStatusFlags
	LD_HL(sRTCStatusFlags);  // ld hl, sRTCStatusFlags
	PUSH_AF;  // push af
	LD_A(BANK(sRTCStatusFlags));  // ld a, BANK(sRTCStatusFlags)
	CALL(mOpenSRAM);  // call OpenSRAM
	POP_AF;  // pop af
	OR_A_hl;  // or [hl]
	LD_hl_A;  // ld [hl], a
	CALL(mCloseSRAM);  // call CloseSRAM
	RET;  // ret

}

int CheckRTCStatus(){
//  check sRTCStatusFlags
	LD_A(BANK(sRTCStatusFlags));  // ld a, BANK(sRTCStatusFlags)
	CALL(mOpenSRAM);  // call OpenSRAM
	LD_A_addr(sRTCStatusFlags);  // ld a, [sRTCStatusFlags]
	CALL(mCloseSRAM);  // call CloseSRAM
	RET;  // ret

}
