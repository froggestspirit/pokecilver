#include "../constants.h"

//  HM moves can't be forgotten

void IsHM(void){
    CP_A(HM01);  // cp HM01
    IF_C goto NotHM;  // jr c, .NotHM
    SCF;  // scf
    RET;  // ret

NotHM:
    AND_A_A;  // and a
    RET;  // ret

}

void IsHMMove(void){
    SET_PC(0x3728U);
    LD_HL(mIsHMMove_HMMoves);  // ld hl, .HMMoves
    LD_DE(1);  // ld de, 1
    return IsInArray();  // jp IsInArray


HMMoves:
    SET_PC(0x3731U);
    //db ['CUT'];  // db CUT
    //db ['FLY'];  // db FLY
    //db ['SURF'];  // db SURF
    //db ['STRENGTH'];  // db STRENGTH
    //db ['FLASH'];  // db FLASH
    //db ['WATERFALL'];  // db WATERFALL
    //db ['WHIRLPOOL'];  // db WHIRLPOOL
    //db ['-1'];  // db -1 ; end

}
