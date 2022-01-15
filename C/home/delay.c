#include "../constants.h"

void DelayFrame(void){
    SET_PC(0x032EU);
//  Wait for one frame
    LD_A(1);  // ld a, 1
    LD_addr_A(wVBlankOccurred);  // ld [wVBlankOccurred], a

//  Wait for the next VBlank, halting to conserve battery

halt:
    SET_PC(0x0333U);
    RST(0x18);  // rst $18 ; rgbasm adds a nop after this instruction by default
    NOP;  // nop
    LD_A_addr(wVBlankOccurred);  // ld a, [wVBlankOccurred]
    AND_A_A;  // and a
    IF_NZ goto halt;  // jr nz, .halt
    RET;  // ret

}

void DelayFrames(void){
    SET_PC(0x033CU);
//  Wait c frames
    CALL(mDelayFrame);  // call DelayFrame
    DEC_C;  // dec c
    JR_NZ (mDelayFrames);  // jr nz, DelayFrames
    RET;  // ret

}
