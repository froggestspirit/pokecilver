#include "../constants.h"

//  Register aliases

int _hl_(){
	JP_hl;  // jp hl

}

int _de_(){
	PUSH_DE;  // push de
	RET;  // ret

}
