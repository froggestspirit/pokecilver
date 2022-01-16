#include "../constants.h"
#include "call_regs.h"

//  Register aliases

void v_hl_(void){
    SET_PC(0x30FAU);
    JP_hl;  // jp hl

}

void v_de_(void){
    PUSH_DE;  // push de
    RET;  // ret

}
