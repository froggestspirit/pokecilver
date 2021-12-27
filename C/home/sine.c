#include "../constants.h"

int Cosine(){  //  unreferenced
//  a = d * cos(a * pi/32)
	ADD_A(0b010000);  // add %010000 ; cos(x) = sin(x + pi/2)
// ; fallthrough
	return mSine;
}

int Sine(){
//  a = d * sin(a * pi/32)
	LD_E_A;  // ld e, a
	HOMECALL(a_Sine);  // homecall _Sine
	RET;  // ret

}
