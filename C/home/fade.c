#include "../constants.h"

//  Functions to fade the screen in and out.

int TimeOfDayFade(){  //  unreferenced
	LD_A_addr(wTimeOfDayPal);  // ld a, [wTimeOfDayPal]
	LD_B_A;  // ld b, a
	LD_HL(mIncGradGBPalTable_11);  // ld hl, IncGradGBPalTable_11
	LD_A_L;  // ld a, l
	SUB_A_B;  // sub b
	LD_L_A;  // ld l, a
	IF_NC goto okay;  // jr nc, .okay
	DEC_H;  // dec h


okay:
	SET_PC(0x036DU);
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rBGP);  // ldh [rBGP], a
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rOBP0);  // ldh [rOBP0], a
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rOBP1);  // ldh [rOBP1], a
	RET;  // ret

}

int RotateFourPalettesRight(){
	LDH_A_addr(hCGB);  // ldh a, [hCGB]
	AND_A_A;  // and a
	IF_Z goto dmg;  // jr z, .dmg
	LD_HL(mIncGradGBPalTable_00);  // ld hl, IncGradGBPalTable_00
	LD_B(4);  // ld b, 4
	JR(mRotatePalettesRight);  // jr RotatePalettesRight


dmg:
	SET_PC(0x0383U);
	LD_HL(mIncGradGBPalTable_08);  // ld hl, IncGradGBPalTable_08
	LD_B(4);  // ld b, 4
	JR(mRotatePalettesRight);  // jr RotatePalettesRight

}

int RotateThreePalettesRight(){
	LDH_A_addr(hCGB);  // ldh a, [hCGB]
	AND_A_A;  // and a
	IF_Z goto dmg;  // jr z, .dmg
	LD_HL(mIncGradGBPalTable_05);  // ld hl, IncGradGBPalTable_05
	LD_B(3);  // ld b, 3
	JR(mRotatePalettesRight);  // jr RotatePalettesRight


dmg:
	SET_PC(0x0396U);
	LD_HL(mIncGradGBPalTable_13);  // ld hl, IncGradGBPalTable_13
	LD_B(3);  // ld b, 3
	return mRotatePalettesRight;
}

int RotatePalettesRight(){
//  Rotate palettes to the right and fill with loaded colors from the left
//  If we're already at the leftmost color, fill with the leftmost color
	PUSH_DE;  // push de
	LD_A_hli;  // ld a, [hli]
	CALL(mDmgToCgbBGPals);  // call DmgToCgbBGPals
	LD_A_hli;  // ld a, [hli]
	LD_E_A;  // ld e, a
	LD_A_hli;  // ld a, [hli]
	LD_D_A;  // ld d, a
	CALL(mDmgToCgbObjPals);  // call DmgToCgbObjPals
	LD_C(8);  // ld c, 8
	CALL(mDelayFrames);  // call DelayFrames
	POP_DE;  // pop de
	DEC_B;  // dec b
	JR_NZ (mRotatePalettesRight);  // jr nz, RotatePalettesRight
	RET;  // ret

}

int RotateFourPalettesLeft(){
	LDH_A_addr(hCGB);  // ldh a, [hCGB]
	AND_A_A;  // and a
	IF_Z goto dmg;  // jr z, .dmg
	LD_HL(mIncGradGBPalTable_04 - 1);  // ld hl, IncGradGBPalTable_04 - 1
	LD_B(4);  // ld b, 4
	JR(mRotatePalettesLeft);  // jr RotatePalettesLeft


dmg:
	SET_PC(0x03BDU);
	LD_HL(mIncGradGBPalTable_12 - 1);  // ld hl, IncGradGBPalTable_12 - 1
	LD_B(4);  // ld b, 4
	JR(mRotatePalettesLeft);  // jr RotatePalettesLeft

}

int RotateThreePalettesLeft(){
	LDH_A_addr(hCGB);  // ldh a, [hCGB]
	AND_A_A;  // and a
	IF_Z goto dmg;  // jr z, .dmg
	LD_HL(mIncGradGBPalTable_07 - 1);  // ld hl, IncGradGBPalTable_07 - 1
	LD_B(3);  // ld b, 3
	JR(mRotatePalettesLeft);  // jr RotatePalettesLeft


dmg:
	SET_PC(0x03D0U);
	LD_HL(mIncGradGBPalTable_15 - 1);  // ld hl, IncGradGBPalTable_15 - 1
	LD_B(3);  // ld b, 3
	return mRotatePalettesLeft;
}

int RotatePalettesLeft(){
//  Rotate palettes to the left and fill with loaded colors from the right
//  If we're already at the rightmost color, fill with the rightmost color
	PUSH_DE;  // push de
	LD_A_hld;  // ld a, [hld]
	LD_D_A;  // ld d, a
	LD_A_hld;  // ld a, [hld]
	LD_E_A;  // ld e, a
	CALL(mDmgToCgbObjPals);  // call DmgToCgbObjPals
	LD_A_hld;  // ld a, [hld]
	CALL(mDmgToCgbBGPals);  // call DmgToCgbBGPals
	LD_C(8);  // ld c, 8
	CALL(mDelayFrames);  // call DelayFrames
	POP_DE;  // pop de
	DEC_B;  // dec b
	JR_NZ (mRotatePalettesLeft);  // jr nz, RotatePalettesLeft
	RET;  // ret

}

int IncGradGBPalTable_00(){
	return mIncGradGBPalTable_01;
}

int IncGradGBPalTable_01(){
	return mIncGradGBPalTable_02;
}

int IncGradGBPalTable_02(){
	return mIncGradGBPalTable_03;
}

int IncGradGBPalTable_03(){

	return mIncGradGBPalTable_04;
}

int IncGradGBPalTable_04(){
	return mIncGradGBPalTable_05;
}

int IncGradGBPalTable_05(){
	return mIncGradGBPalTable_06;
}

int IncGradGBPalTable_06(){

	return mIncGradGBPalTable_07;
}

int IncGradGBPalTable_07(){
//                            bgp      obp1     obp2
	return mIncGradGBPalTable_08;
}

int IncGradGBPalTable_08(){
	return mIncGradGBPalTable_09;
}

int IncGradGBPalTable_09(){
	return mIncGradGBPalTable_10;
}

int IncGradGBPalTable_10(){
	return mIncGradGBPalTable_11;
}

int IncGradGBPalTable_11(){

	return mIncGradGBPalTable_12;
}

int IncGradGBPalTable_12(){
	return mIncGradGBPalTable_13;
}

int IncGradGBPalTable_13(){
	return mIncGradGBPalTable_14;
}

int IncGradGBPalTable_14(){

	return mIncGradGBPalTable_15;
}

int IncGradGBPalTable_15(){

}
