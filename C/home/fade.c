#include "../constants.h"

//  Functions to fade the screen in and out.

int TimeOfDayFade(){
	SET_PC(0x0360U);
//  //  unreferenced
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
	SET_PC(0x0377U);
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
	SET_PC(0x038AU);
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
	return RotatePalettesRight();
}

int RotatePalettesRight(){
	SET_PC(0x039BU);
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
	SET_PC(0x03B1U);
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
	SET_PC(0x03C4U);
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
	return RotatePalettesLeft();
}

int RotatePalettesLeft(){
	SET_PC(0x03D5U);
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
	SET_PC(0x03EBU);
//dc 3,3,3,3, 3,3,3,3, 3,3,3,3
	return IncGradGBPalTable_01();
}

int IncGradGBPalTable_01(){
	SET_PC(0x03EEU);
//dc 3,3,3,2, 3,3,3,2, 3,3,3,2
	return IncGradGBPalTable_02();
}

int IncGradGBPalTable_02(){
	SET_PC(0x03F1U);
//dc 3,3,2,1, 3,3,2,1, 3,3,2,1
	return IncGradGBPalTable_03();
}

int IncGradGBPalTable_03(){
	SET_PC(0x03F4U);
//dc 3,2,1,0, 3,2,1,0, 3,2,1,0

	return IncGradGBPalTable_04();
}

int IncGradGBPalTable_04(){
	SET_PC(0x03F7U);
//dc 3,2,1,0, 3,2,1,0, 3,2,1,0
	return IncGradGBPalTable_05();
}

int IncGradGBPalTable_05(){
	SET_PC(0x03FAU);
//dc 2,1,0,0, 2,1,0,0, 2,1,0,0
	return IncGradGBPalTable_06();
}

int IncGradGBPalTable_06(){
	SET_PC(0x03FDU);
//dc 1,0,0,0, 1,0,0,0, 1,0,0,0

	return IncGradGBPalTable_07();
}

int IncGradGBPalTable_07(){
	SET_PC(0x0400U);
//dc 0,0,0,0, 0,0,0,0, 0,0,0,0
//                            bgp      obp1     obp2
	return IncGradGBPalTable_08();
}

int IncGradGBPalTable_08(){
	SET_PC(0x0403U);
//dc 3,3,3,3, 3,3,3,3, 3,3,3,3
	return IncGradGBPalTable_09();
}

int IncGradGBPalTable_09(){
	SET_PC(0x0406U);
//dc 3,3,3,2, 3,3,3,2, 3,3,2,0
	return IncGradGBPalTable_10();
}

int IncGradGBPalTable_10(){
	SET_PC(0x0409U);
//dc 3,3,2,1, 3,2,1,0, 3,2,1,0
	return IncGradGBPalTable_11();
}

int IncGradGBPalTable_11(){
	SET_PC(0x040CU);
//dc 3,2,1,0, 3,1,0,0, 3,2,0,0

	return IncGradGBPalTable_12();
}

int IncGradGBPalTable_12(){
	SET_PC(0x040FU);
//dc 3,2,1,0, 3,1,0,0, 3,2,0,0
	return IncGradGBPalTable_13();
}

int IncGradGBPalTable_13(){
	SET_PC(0x0412U);
//dc 2,1,0,0, 2,0,0,0, 2,1,0,0
	return IncGradGBPalTable_14();
}

int IncGradGBPalTable_14(){
	SET_PC(0x0415U);
//dc 1,0,0,0, 1,0,0,0, 1,0,0,0

	return IncGradGBPalTable_15();
}

int IncGradGBPalTable_15(){
	SET_PC(0x0418U);
//dc 0,0,0,0, 0,0,0,0, 0,0,0,0

}

