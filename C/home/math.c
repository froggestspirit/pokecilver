#include "../constants.h"

int SimpleMultiply(){
//  Return a * c.
	AND_A_A;  // and a
	RET_Z ;  // ret z

	PUSH_BC;  // push bc
	LD_B_A;  // ld b, a
	XOR_A_A;  // xor a

_loop:
	SET_PC(0x31ADU);
	ADD_A_C;  // add c
	DEC_B;  // dec b
	IF_NZ goto _loop;  // jr nz, .loop
	POP_BC;  // pop bc
	RET;  // ret

}

int SimpleDivide(){
//  Divide a by c. Return quotient b and remainder a.
	LD_B(0);  // ld b, 0

_loop:
	SET_PC(0x31B5U);
	INC_B;  // inc b
	SUB_A_C;  // sub c
	IF_NC goto _loop;  // jr nc, .loop
	DEC_B;  // dec b
	ADD_A_C;  // add c
	RET;  // ret

}

int Multiply(){
//  Multiply hMultiplicand (3 bytes) by hMultiplier. Result in hProduct.
//  All values are big endian.
	PUSH_HL;  // push hl
	PUSH_BC;  // push bc

	CALLFAR(a_Multiply);  // callfar _Multiply

	POP_BC;  // pop bc
	POP_HL;  // pop hl
	RET;  // ret

}

int Divide(){
//  Divide hDividend length b (max 4 bytes) by hDivisor. Result in hQuotient.
//  All values are big endian.
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	HOMECALL(a_Divide);  // homecall _Divide
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int SubtractAbsolute(){  //  unreferenced
//  Return |a - b|, sign in carry.
	SUB_A_B;  // sub b
	RET_NC ;  // ret nc
	CPL;  // cpl
	ADD_A(1);  // add 1
	SCF;  // scf
	RET;  // ret

}
