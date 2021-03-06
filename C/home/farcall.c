#include "../constants.h"
#include "farcall.h"

void FarCall_hl(void){
    SET_PC(0x2E25U);
//  Call a:hl.
//  Preserves other registers.
    LD_addr_A(wTempBank);  // ld [wTempBank], a
    LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
    PUSH_AF;  // push af
    LD_A_addr(wTempBank);  // ld a, [wTempBank]
    RST(mBankswitch);  // rst Bankswitch
    CALL(mFarCall_JumpToHL);  // call FarCall_JumpToHL

//  We want to retain the contents of f.
//  To do this, we can pop to bc instead of af.
    LD_A_B;  // ld a, b
    LD_addr_A(wFarCallBC);  // ld [wFarCallBC], a
    LD_A_C;  // ld a, c
    LD_addr_A(wFarCallBC + 1);  // ld [wFarCallBC + 1], a

//  Restore the working bank.
    POP_BC;  // pop bc
    LD_A_B;  // ld a, b
    RST(mBankswitch);  // rst Bankswitch

//  Restore the contents of bc.
    LD_A_addr(wFarCallBC);  // ld a, [wFarCallBC]
    LD_B_A;  // ld b, a
    LD_A_addr(wFarCallBC + 1);  // ld a, [wFarCallBC + 1]
    LD_C_A;  // ld c, a
    RET;  // ret

}

void FarCall_JumpToHL(void){
    SET_PC(0x2E46U);
    JP_hl;  // jp hl

}
