#include "../constants.h"

int UpdateTimeAndPals(){
	SET_PC(0x0343U);
//  update time and time-sensitive palettes

//  rtc enabled?
	LD_A_addr(wSpriteUpdatesEnabled);  // ld a, [wSpriteUpdatesEnabled]
	CP_A(0);  // cp 0
	RET_Z ;  // ret z

	CALL(mUpdateTime);  // call UpdateTime

//  obj update on?
	LD_A_addr(wVramState);  // ld a, [wVramState]
	BIT_A(0);  // bit 0, a ; obj update
	RET_Z ;  // ret z

}

int TimeOfDayPals(){
	SET_PC(0x0352U);
	CALLFAR(av_TimeOfDayPals);  // callfar _TimeOfDayPals
	RET;  // ret

}

int UpdateTimePals(){
	SET_PC(0x0359U);
	CALLFAR(av_UpdateTimePals);  // callfar _UpdateTimePals
	RET;  // ret

}
