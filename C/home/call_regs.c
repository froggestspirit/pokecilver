#include "../constants.h"

//  Register aliases

int v_hl_(){
	JP_hl;  // jp hl

}

int v_de_(){
	PUSH_DE;  // push de
	RET;  // ret

}
