#include "../constants.h"

//  HM moves can't be forgotten

int IsHM(){
	SET_PC(0x3720U);
	CP_A(HM01);  // cp HM01
	IF_C goto NotHM;  // jr c, .NotHM
	SCF;  // scf
	RET;  // ret

NotHM:
	SET_PC(0x3726U);
	AND_A_A;  // and a
	RET;  // ret

}

int IsHMMove(){
	SET_PC(0x3728U);
	LD_HL(mIsHMMove_HMMoves);  // ld hl, .HMMoves
	LD_DE(1);  // ld de, 1
	JP(mIsInArray);  // jp IsInArray


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