#include "../constants.h"

void PrintNum(void){
    SET_PC(0x3239U);
//  Print c digits of the b-byte value from de to hl.
//  Allows 2 to 7 digits. For 1-digit numbers, add
//  the value to char "0" instead of calling PrintNum.
//  The high nybble of the c register specifies how many of the total amount of
//  digits will be in front of the decimal point.
//  Some extra flags can be given in bits 5-7 of b.
//  Bit 5: money if set (unless left-aligned without leading zeros)
//  Bit 6: left-aligned if set
//  Bit 7: print leading zeros if set

    PUSH_BC;  // push bc

    BIT_B(5);  // bit 5, b
    IF_Z goto main;  // jr z, .main
    BIT_B(7);  // bit 7, b
    IF_NZ goto moneyflag;  // jr nz, .moneyflag
    BIT_B(6);  // bit 6, b
    IF_Z goto main;  // jr z, .main


moneyflag:
// 
    SET_PC(0x3246U);  //  101xxxxx or 011xxxxx
    LD_A(0xf0);  // ld a, "¥"
    LD_hli_A;  // ld [hli], a
    RES_B(5);  // res 5, b ; 100xxxxx or 010xxxxx


main:
    SET_PC(0x324BU);
    XOR_A_A;  // xor a
    LDH_addr_A(hPrintNumBuffer + 0);  // ldh [hPrintNumBuffer + 0], a
    LDH_addr_A(hPrintNumBuffer + 1);  // ldh [hPrintNumBuffer + 1], a
    LDH_addr_A(hPrintNumBuffer + 2);  // ldh [hPrintNumBuffer + 2], a
    LD_A_B;  // ld a, b
    AND_A(0xf);  // and $f
    CP_A(1);  // cp 1
    IF_Z goto byte;  // jr z, .byte
    CP_A(2);  // cp 2
    IF_Z goto word;  // jr z, .word
//  maximum 3 bytes
    LD_A_de;  // ld a, [de]
    LDH_addr_A(hPrintNumBuffer + 1);  // ldh [hPrintNumBuffer + 1], a
    INC_DE;  // inc de
    LD_A_de;  // ld a, [de]
    LDH_addr_A(hPrintNumBuffer + 2);  // ldh [hPrintNumBuffer + 2], a
    INC_DE;  // inc de
    LD_A_de;  // ld a, [de]
    LDH_addr_A(hPrintNumBuffer + 3);  // ldh [hPrintNumBuffer + 3], a
    goto start;  // jr .start


word:
    SET_PC(0x326AU);
    LD_A_de;  // ld a, [de]
    LDH_addr_A(hPrintNumBuffer + 2);  // ldh [hPrintNumBuffer + 2], a
    INC_DE;  // inc de
    LD_A_de;  // ld a, [de]
    LDH_addr_A(hPrintNumBuffer + 3);  // ldh [hPrintNumBuffer + 3], a
    goto start;  // jr .start


byte:
    SET_PC(0x3273U);
    LD_A_de;  // ld a, [de]
    LDH_addr_A(hPrintNumBuffer + 3);  // ldh [hPrintNumBuffer + 3], a


start:
    SET_PC(0x3276U);
    PUSH_DE;  // push de

    LD_D_B;  // ld d, b
    LD_A_C;  // ld a, c
    SWAP_A;  // swap a
    AND_A(0xf);  // and $f
    LD_E_A;  // ld e, a
    LD_A_C;  // ld a, c
    AND_A(0xf);  // and $f
    LD_B_A;  // ld b, a
    LD_C(0);  // ld c, 0
    CP_A(2);  // cp 2
    IF_Z goto two;  // jr z, .two
    CP_A(3);  // cp 3
    IF_Z goto three;  // jr z, .three
    CP_A(4);  // cp 4
    IF_Z goto four;  // jr z, .four
    CP_A(5);  // cp 5
    IF_Z goto five;  // jr z, .five
    CP_A(6);  // cp 6
    IF_Z goto six;  // jr z, .six

//  seven
    LD_A(HIGH(1000000 >> 8));  // ld a, HIGH(1000000 >> 8)
    LDH_addr_A(hPrintNumBuffer + 4);  // ldh [hPrintNumBuffer + 4], a
    LD_A(HIGH(1000000));  // ld a, HIGH(1000000) ; mid
    LDH_addr_A(hPrintNumBuffer + 5);  // ldh [hPrintNumBuffer + 5], a
    LD_A(LOW(1000000));  // ld a, LOW(1000000)
    LDH_addr_A(hPrintNumBuffer + 6);  // ldh [hPrintNumBuffer + 6], a
    CALL(mPrintNum_PrintDigit);  // call .PrintDigit
    CALL(mPrintNum_AdvancePointer);  // call .AdvancePointer


six:
    SET_PC(0x32AAU);
    LD_A(HIGH(100000 >> 8));  // ld a, HIGH(100000 >> 8)
    LDH_addr_A(hPrintNumBuffer + 4);  // ldh [hPrintNumBuffer + 4], a
    LD_A(HIGH(100000));  // ld a, HIGH(100000) ; mid
    LDH_addr_A(hPrintNumBuffer + 5);  // ldh [hPrintNumBuffer + 5], a
    LD_A(LOW(100000));  // ld a, LOW(100000)
    LDH_addr_A(hPrintNumBuffer + 6);  // ldh [hPrintNumBuffer + 6], a
    CALL(mPrintNum_PrintDigit);  // call .PrintDigit
    CALL(mPrintNum_AdvancePointer);  // call .AdvancePointer


five:
    SET_PC(0x32BCU);
    XOR_A_A;  // xor a ; HIGH(10000 >> 8)
    LDH_addr_A(hPrintNumBuffer + 4);  // ldh [hPrintNumBuffer + 4], a
    LD_A(HIGH(10000));  // ld a, HIGH(10000) ; mid
    LDH_addr_A(hPrintNumBuffer + 5);  // ldh [hPrintNumBuffer + 5], a
    LD_A(LOW(10000));  // ld a, LOW(10000)
    LDH_addr_A(hPrintNumBuffer + 6);  // ldh [hPrintNumBuffer + 6], a
    CALL(mPrintNum_PrintDigit);  // call .PrintDigit
    CALL(mPrintNum_AdvancePointer);  // call .AdvancePointer


four:
    SET_PC(0x32CDU);
    XOR_A_A;  // xor a ; HIGH(1000 >> 8)
    LDH_addr_A(hPrintNumBuffer + 4);  // ldh [hPrintNumBuffer + 4], a
    LD_A(HIGH(1000));  // ld a, HIGH(1000) ; mid
    LDH_addr_A(hPrintNumBuffer + 5);  // ldh [hPrintNumBuffer + 5], a
    LD_A(LOW(1000));  // ld a, LOW(1000)
    LDH_addr_A(hPrintNumBuffer + 6);  // ldh [hPrintNumBuffer + 6], a
    CALL(mPrintNum_PrintDigit);  // call .PrintDigit
    CALL(mPrintNum_AdvancePointer);  // call .AdvancePointer


three:
    SET_PC(0x32DEU);
    XOR_A_A;  // xor a ; HIGH(100 >> 8)
    LDH_addr_A(hPrintNumBuffer + 4);  // ldh [hPrintNumBuffer + 4], a
    XOR_A_A;  // xor a ; HIGH(100) ; mid
    LDH_addr_A(hPrintNumBuffer + 5);  // ldh [hPrintNumBuffer + 5], a
    LD_A(LOW(100));  // ld a, LOW(100)
    LDH_addr_A(hPrintNumBuffer + 6);  // ldh [hPrintNumBuffer + 6], a
    CALL(mPrintNum_PrintDigit);  // call .PrintDigit
    CALL(mPrintNum_AdvancePointer);  // call .AdvancePointer


two:
    SET_PC(0x32EEU);
    DEC_E;  // dec e
    IF_NZ goto two_skip;  // jr nz, .two_skip
    LD_A(0xf6);  // ld a, "0"
    LDH_addr_A(hPrintNumBuffer + 0);  // ldh [hPrintNumBuffer + 0], a

two_skip:
    SET_PC(0x32F5U);

    LD_C(0);  // ld c, 0
    LDH_A_addr(hPrintNumBuffer + 3);  // ldh a, [hPrintNumBuffer + 3]

mod_10:
    SET_PC(0x32F9U);
    CP_A(10);  // cp 10
    IF_C goto modded_10;  // jr c, .modded_10
    SUB_A(10);  // sub 10
    INC_C;  // inc c
    goto mod_10;  // jr .mod_10

modded_10:
    SET_PC(0x3302U);

    LD_B_A;  // ld b, a
    LDH_A_addr(hPrintNumBuffer + 0);  // ldh a, [hPrintNumBuffer + 0]
    OR_A_C;  // or c
    IF_NZ goto money;  // jr nz, .money
    CALL(mPrintNum_PrintLeadingZero);  // call .PrintLeadingZero
    goto money_leading_zero;  // jr .money_leading_zero


money:
    SET_PC(0x330DU);
    CALL(mPrintNum_PrintYen);  // call .PrintYen
    PUSH_AF;  // push af
    LD_A(0xf6);  // ld a, "0"
    ADD_A_C;  // add c
    LD_hl_A;  // ld [hl], a
    POP_AF;  // pop af
    LDH_addr_A(hPrintNumBuffer + 0);  // ldh [hPrintNumBuffer + 0], a
    INC_E;  // inc e
    DEC_E;  // dec e
    IF_NZ goto money_leading_zero;  // jr nz, .money_leading_zero
    INC_HL;  // inc hl
    LD_hl(0xe8);  // ld [hl], "."


money_leading_zero:
    SET_PC(0x331FU);
    CALL(mPrintNum_AdvancePointer);  // call .AdvancePointer
    CALL(mPrintNum_PrintYen);  // call .PrintYen
    LD_A(0xf6);  // ld a, "0"
    ADD_A_B;  // add b
    LD_hli_A;  // ld [hli], a

    POP_DE;  // pop de
    POP_BC;  // pop bc
    RET;  // ret


PrintYen:
    SET_PC(0x332CU);
    PUSH_AF;  // push af
    LDH_A_addr(hPrintNumBuffer + 0);  // ldh a, [hPrintNumBuffer + 0]
    AND_A_A;  // and a
    IF_NZ goto stop;  // jr nz, .stop
    BIT_D(5);  // bit 5, d
    IF_Z goto stop;  // jr z, .stop
    LD_A(0xf0);  // ld a, "¥"
    LD_hli_A;  // ld [hli], a
    RES_D(5);  // res 5, d


stop:
    SET_PC(0x333BU);
    POP_AF;  // pop af
    RET;  // ret


PrintDigit:
    SET_PC(0x333DU);
    DEC_E;  // dec e
    IF_NZ goto ok;  // jr nz, .ok
    LD_A(0xf6);  // ld a, "0"
    LDH_addr_A(hPrintNumBuffer + 0);  // ldh [hPrintNumBuffer + 0], a

ok:
    SET_PC(0x3344U);
    LD_C(0);  // ld c, 0

loop:
    SET_PC(0x3346U);
    LDH_A_addr(hPrintNumBuffer + 4);  // ldh a, [hPrintNumBuffer + 4]
    LD_B_A;  // ld b, a
    LDH_A_addr(hPrintNumBuffer + 1);  // ldh a, [hPrintNumBuffer + 1]
    LDH_addr_A(hPrintNumBuffer + 7);  // ldh [hPrintNumBuffer + 7], a
    CP_A_B;  // cp b
    IF_C goto skip1;  // jr c, .skip1
    SUB_A_B;  // sub b
    LDH_addr_A(hPrintNumBuffer + 1);  // ldh [hPrintNumBuffer + 1], a
    LDH_A_addr(hPrintNumBuffer + 5);  // ldh a, [hPrintNumBuffer + 5]
    LD_B_A;  // ld b, a
    LDH_A_addr(hPrintNumBuffer + 2);  // ldh a, [hPrintNumBuffer + 2]
    LDH_addr_A(hPrintNumBuffer + 8);  // ldh [hPrintNumBuffer + 8], a
    CP_A_B;  // cp b
    IF_NC goto skip2;  // jr nc, .skip2
    LDH_A_addr(hPrintNumBuffer + 1);  // ldh a, [hPrintNumBuffer + 1]
    OR_A(0);  // or 0
    IF_Z goto skip3;  // jr z, .skip3
    DEC_A;  // dec a
    LDH_addr_A(hPrintNumBuffer + 1);  // ldh [hPrintNumBuffer + 1], a
    LDH_A_addr(hPrintNumBuffer + 2);  // ldh a, [hPrintNumBuffer + 2]

skip2:
    SET_PC(0x3368U);
    SUB_A_B;  // sub b
    LDH_addr_A(hPrintNumBuffer + 2);  // ldh [hPrintNumBuffer + 2], a
    LDH_A_addr(hPrintNumBuffer + 6);  // ldh a, [hPrintNumBuffer + 6]
    LD_B_A;  // ld b, a
    LDH_A_addr(hPrintNumBuffer + 3);  // ldh a, [hPrintNumBuffer + 3]
    LDH_addr_A(hPrintNumBuffer + 9);  // ldh [hPrintNumBuffer + 9], a
    CP_A_B;  // cp b
    IF_NC goto skip4;  // jr nc, .skip4
    LDH_A_addr(hPrintNumBuffer + 2);  // ldh a, [hPrintNumBuffer + 2]
    AND_A_A;  // and a
    IF_NZ goto skip5;  // jr nz, .skip5
    LDH_A_addr(hPrintNumBuffer + 1);  // ldh a, [hPrintNumBuffer + 1]
    AND_A_A;  // and a
    IF_Z goto skip6;  // jr z, .skip6
    DEC_A;  // dec a
    LDH_addr_A(hPrintNumBuffer + 1);  // ldh [hPrintNumBuffer + 1], a
    XOR_A_A;  // xor a

skip5:
    SET_PC(0x3383U);
    DEC_A;  // dec a
    LDH_addr_A(hPrintNumBuffer + 2);  // ldh [hPrintNumBuffer + 2], a
    LDH_A_addr(hPrintNumBuffer + 3);  // ldh a, [hPrintNumBuffer + 3]

skip4:
    SET_PC(0x3388U);
    SUB_A_B;  // sub b
    LDH_addr_A(hPrintNumBuffer + 3);  // ldh [hPrintNumBuffer + 3], a
    INC_C;  // inc c
    goto loop;  // jr .loop

skip6:
    SET_PC(0x338EU);
    LDH_A_addr(hPrintNumBuffer + 8);  // ldh a, [hPrintNumBuffer + 8]
    LDH_addr_A(hPrintNumBuffer + 2);  // ldh [hPrintNumBuffer + 2], a

skip3:
    SET_PC(0x3392U);
    LDH_A_addr(hPrintNumBuffer + 7);  // ldh a, [hPrintNumBuffer + 7]
    LDH_addr_A(hPrintNumBuffer + 1);  // ldh [hPrintNumBuffer + 1], a

skip1:
    SET_PC(0x3396U);
    LDH_A_addr(hPrintNumBuffer + 0);  // ldh a, [hPrintNumBuffer + 0]
    OR_A_C;  // or c
    IF_Z goto PrintLeadingZero;  // jr z, .PrintLeadingZero
    LDH_A_addr(hPrintNumBuffer + 0);  // ldh a, [hPrintNumBuffer + 0]
    AND_A_A;  // and a
    IF_NZ goto done;  // jr nz, .done
    BIT_D(5);  // bit 5, d
    IF_Z goto done;  // jr z, .done
    LD_A(0xf0);  // ld a, "¥"
    LD_hli_A;  // ld [hli], a
    RES_D(5);  // res 5, d

done:
    SET_PC(0x33A9U);
    LD_A(0xf6);  // ld a, "0"
    ADD_A_C;  // add c
    LD_hl_A;  // ld [hl], a
    LDH_addr_A(hPrintNumBuffer + 0);  // ldh [hPrintNumBuffer + 0], a
    INC_E;  // inc e
    DEC_E;  // dec e
    RET_NZ ;  // ret nz
    INC_HL;  // inc hl
    LD_hl(0xe8);  // ld [hl], "."
    RET;  // ret


PrintLeadingZero:
    SET_PC(0x33B6U);
//  prints a leading zero unless they are turned off in the flags
    BIT_D(7);  // bit 7, d ; print leading zeroes?
    RET_Z ;  // ret z
    LD_hl(0xf6);  // ld [hl], "0"
    RET;  // ret


AdvancePointer:
    SET_PC(0x33BCU);
//  increments the pointer unless leading zeroes are not being printed,
//  the number is left-aligned, and no nonzero digits have been printed yet
    BIT_D(7);  // bit 7, d ; print leading zeroes?
    IF_NZ goto inc;  // jr nz, .inc
    BIT_D(6);  // bit 6, d ; left alignment or right alignment?
    IF_Z goto inc;  // jr z, .inc
    LDH_A_addr(hPrintNumBuffer + 0);  // ldh a, [hPrintNumBuffer + 0]
    AND_A_A;  // and a
    RET_Z ;  // ret z

inc:
    SET_PC(0x33C8U);
    INC_HL;  // inc hl
    RET;  // ret

}

void PrintHexNumber(void){
    SET_PC(0x33CAU);
//  Print the c-byte value from de to hl as hexadecimal digits.

loop:
    SET_PC(0x33CAU);
    PUSH_BC;  // push bc
    CALL(mPrintHexNumber_HandleByte);  // call .HandleByte
    POP_BC;  // pop bc
    DEC_C;  // dec c
    IF_NZ goto loop;  // jr nz, .loop
    RET;  // ret


HandleByte:
    SET_PC(0x33D3U);
    LD_A_de;  // ld a, [de]
    SWAP_A;  // swap a
    AND_A(0xf);  // and $f
    CALL(mPrintHexNumber_PrintDigit);  // call .PrintDigit
    LD_hli_A;  // ld [hli], a
    LD_A_de;  // ld a, [de]
    AND_A(0xf);  // and $f
    CALL(mPrintHexNumber_PrintDigit);  // call .PrintDigit
    LD_hli_A;  // ld [hli], a
    INC_DE;  // inc de
    RET;  // ret


PrintDigit:
    SET_PC(0x33E5U);
    LD_BC(mPrintHexNumber_HexDigits);  // ld bc, .HexDigits
    ADD_A_C;  // add c
    LD_C_A;  // ld c, a
    LD_A(0);  // ld a, 0
    ADC_A_B;  // adc b
    LD_B_A;  // ld b, a
    LD_A_bc;  // ld a, [bc]
    RET;  // ret


HexDigits:
    SET_PC(0x33F0U);
    //db ['"0123456789ABCDEF"'];  // db "0123456789ABCDEF"

}
