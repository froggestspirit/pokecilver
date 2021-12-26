#include "../constants.h"

int DelayFrame(){
//  Wait for one frame
	LD_A(1);  // ld a, 1
	LD_addr_A(wVBlankOccurred);  // ld [wVBlankOccurred], a

//  Wait for the next VBlank, halting to conserve battery

_halt:
	SET_PC(0x0333U);
	RST(0x18);  // rst $18 ; rgbasm adds a nop after this instruction by default
	  // nop
	LD_A_addr(wVBlankOccurred);  // ld a, [wVBlankOccurred]
	AND_A_A;  // and a
	IF_NZ goto _halt;  // jr nz, .halt
	RET;  // ret

}

int DelayFrames(){
//  Wait c frames
	CALL(mDelayFrame);  // call DelayFrame
	DEC_C;  // dec c
	JR_NZ (mDelayFrames);  // jr nz, DelayFrames
	RET;  // ret

}
