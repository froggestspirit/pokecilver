#include "../constants.h"

void DoItemEffect(void){
    SET_PC(0x3053U);
    FARCALL(av_DoItemEffect);  // farcall _DoItemEffect
    RET;  // ret

}

void CheckTossableItem(void){
    SET_PC(0x305AU);
    PUSH_HL;  // push hl
    PUSH_DE;  // push de
    PUSH_BC;  // push bc
    FARCALL(av_CheckTossableItem);  // farcall _CheckTossableItem
    POP_BC;  // pop bc
    POP_DE;  // pop de
    POP_HL;  // pop hl
    RET;  // ret

}

void TossItem(void){
    SET_PC(0x3067U);
    PUSH_HL;  // push hl
    PUSH_DE;  // push de
    PUSH_BC;  // push bc
    LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
    PUSH_AF;  // push af
    LD_A(BANK(av_TossItem));  // ld a, BANK(_TossItem)
    RST(mBankswitch);  // rst Bankswitch

    CALL(mv_TossItem);  // call _TossItem

    POP_BC;  // pop bc
    LD_A_B;  // ld a, b
    RST(mBankswitch);  // rst Bankswitch
    POP_BC;  // pop bc
    POP_DE;  // pop de
    POP_HL;  // pop hl
    RET;  // ret

}

void ReceiveItem(void){
    SET_PC(0x307AU);
    PUSH_BC;  // push bc
    LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
    PUSH_AF;  // push af
    LD_A(BANK(av_ReceiveItem));  // ld a, BANK(_ReceiveItem)
    RST(mBankswitch);  // rst Bankswitch
    PUSH_HL;  // push hl
    PUSH_DE;  // push de

    CALL(mv_ReceiveItem);  // call _ReceiveItem

    POP_DE;  // pop de
    POP_HL;  // pop hl
    POP_BC;  // pop bc
    LD_A_B;  // ld a, b
    RST(mBankswitch);  // rst Bankswitch
    POP_BC;  // pop bc
    RET;  // ret

}

void CheckItem(void){
    SET_PC(0x308DU);
    PUSH_HL;  // push hl
    PUSH_DE;  // push de
    PUSH_BC;  // push bc
    LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
    PUSH_AF;  // push af
    LD_A(BANK(av_CheckItem));  // ld a, BANK(_CheckItem)
    RST(mBankswitch);  // rst Bankswitch

    CALL(mv_CheckItem);  // call _CheckItem

    POP_BC;  // pop bc
    LD_A_B;  // ld a, b
    RST(mBankswitch);  // rst Bankswitch
    POP_BC;  // pop bc
    POP_DE;  // pop de
    POP_HL;  // pop hl
    RET;  // ret

}
