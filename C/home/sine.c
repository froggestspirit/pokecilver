#include "../constants.h"

int Cosine(){
	SET_PC(0x19AAU);
//  //  unreferenced
//  a = d * cos(a * pi/32)
	ADD_A(0b010000);  // add %010000 ; cos(x) = sin(x + pi/2)
// ; fallthrough
	return Sine();
}

int Sine(){
	SET_PC(0x19ACU);
//  a = d * sin(a * pi/32)
	LD_E_A;  // ld e, a
	HOMECALL(av_Sine);  // homecall _Sine
	RET;  // ret

}

