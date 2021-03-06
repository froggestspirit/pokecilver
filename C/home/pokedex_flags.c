#include "../constants.h"
#include "pokedex_flags.h"

void CountSetBits(void){
//  Count the number of set bits in b bytes starting from hl.
//  Return in a, c and [wNumSetBits].
    LD_C(0);  // ld c, 0

next:
    LD_A_hli;  // ld a, [hli]
    LD_E_A;  // ld e, a
    LD_D(8);  // ld d, 8


count:
    SRL_E;  // srl e
    LD_A(0);  // ld a, 0
    ADC_A_C;  // adc c
    LD_C_A;  // ld c, a
    DEC_D;  // dec d
    IF_NZ goto count;  // jr nz, .count

    DEC_B;  // dec b
    IF_NZ goto next;  // jr nz, .next

    LD_A_C;  // ld a, c
    LD_addr_A(wNumSetBits);  // ld [wNumSetBits], a
    RET;  // ret

}

void GetWeekday(void){
    LD_A_addr(wCurDay);  // ld a, [wCurDay]

mod:
    SUB_A(7);  // sub 7
    IF_NC goto mod;  // jr nc, .mod
    ADD_A(7);  // add 7
    RET;  // ret

}

void SetSeenAndCaughtMon(void){
    SET_PC(0x35BFU);
    PUSH_AF;  // push af
    LD_C_A;  // ld c, a
    LD_HL(wPokedexCaught);  // ld hl, wPokedexCaught
    LD_B(SET_FLAG);  // ld b, SET_FLAG
    CALL(mPokedexFlagAction);  // call PokedexFlagAction
    POP_AF;  // pop af
// ; fallthrough

    return SetSeenMon();
}

void SetSeenMon(void){
    SET_PC(0x35CAU);
    LD_C_A;  // ld c, a
    LD_HL(wPokedexSeen);  // ld hl, wPokedexSeen
    LD_B(SET_FLAG);  // ld b, SET_FLAG
    JR(mPokedexFlagAction);  // jr PokedexFlagAction

}

void CheckCaughtMon(void){
    SET_PC(0x35D2U);
    LD_C_A;  // ld c, a
    LD_HL(wPokedexCaught);  // ld hl, wPokedexCaught
    LD_B(CHECK_FLAG);  // ld b, CHECK_FLAG
    JR(mPokedexFlagAction);  // jr PokedexFlagAction

}

void CheckSeenMon(void){
    SET_PC(0x35DAU);
    LD_C_A;  // ld c, a
    LD_HL(wPokedexSeen);  // ld hl, wPokedexSeen
    LD_B(CHECK_FLAG);  // ld b, CHECK_FLAG
// ; fallthrough

    return PokedexFlagAction();
}

void PokedexFlagAction(void){
    SET_PC(0x35E0U);
    LD_D(0);  // ld d, 0
    PREDEF(pSmallFarFlagAction);  // predef SmallFarFlagAction
    LD_A_C;  // ld a, c
    AND_A_A;  // and a
    RET;  // ret

}
