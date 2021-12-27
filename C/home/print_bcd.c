#include "../constants.h"

int PrintBCDNumber(){
//  function to print a BCD (Binary-coded decimal) number
//  de = address of BCD number
//  hl = destination address
//  c = flags and length
//  bit 7: if set, do not print leading zeroes
//         if unset, print leading zeroes
//  bit 6: if set, left-align the string (do not pad empty digits with spaces)
//         if unset, right-align the string
//  bit 5: if set, print currency symbol at the beginning of the string
//         if unset, do not print the currency symbol
//  bits 0-4: length of BCD number in bytes
//  Note that bits 5 and 7 are modified during execution. The above reflects
//  their meaning at the beginning of the functions's execution.
	LD_B_C;  // ld b, c ; save flags in b
	RES_C(PRINTNUM_LEADINGZEROS_F);  // res PRINTNUM_LEADINGZEROS_F, c
	RES_C(PRINTNUM_LEFTALIGN_F);  // res PRINTNUM_LEFTALIGN_F, c
	RES_C(PRINTNUM_MONEY_F);  // res PRINTNUM_MONEY_F, c ; c now holds the length
	BIT_B(PRINTNUM_MONEY_F);  // bit PRINTNUM_MONEY_F, b
	IF_Z goto _loop;  // jr z, .loop
	BIT_B(PRINTNUM_LEADINGZEROS_F);  // bit PRINTNUM_LEADINGZEROS_F, b
	IF_NZ goto _loop;  // jr nz, .loop ; skip currency symbol
	LD_hl("¥");  // ld [hl], "¥"
	INC_HL;  // inc hl

_loop:
	SET_PC(0x3AECU);
	LD_A_de;  // ld a, [de]
	SWAP_A;  // swap a
	CALL(mPrintBCDDigit);  // call PrintBCDDigit ; print upper digit
	LD_A_de;  // ld a, [de]
	CALL(mPrintBCDDigit);  // call PrintBCDDigit ; print lower digit
	INC_DE;  // inc de
	DEC_C;  // dec c
	IF_NZ goto _loop;  // jr nz, .loop
	BIT_B(PRINTNUM_LEADINGZEROS_F);  // bit PRINTNUM_LEADINGZEROS_F, b
	IF_Z goto _done;  // jr z, .done ; if so, we are done
//  every digit of the BCD number is zero
	BIT_B(PRINTNUM_LEFTALIGN_F);  // bit PRINTNUM_LEFTALIGN_F, b
	IF_NZ goto _skipLeftAlignmentAdjustment;  // jr nz, .skipLeftAlignmentAdjustment
//  the string is left-aligned
	DEC_HL;  // dec hl

_skipLeftAlignmentAdjustment:
	SET_PC(0x3B03U);
	BIT_B(PRINTNUM_MONEY_F);  // bit PRINTNUM_MONEY_F, b
	IF_Z goto _skipCurrencySymbol;  // jr z, .skipCurrencySymbol
	LD_hl("¥");  // ld [hl], "¥" ; currency symbol
	INC_HL;  // inc hl

_skipCurrencySymbol:
	SET_PC(0x3B0AU);
	LD_hl("0");  // ld [hl], "0"
	CALL(mPrintLetterDelay);  // call PrintLetterDelay
	INC_HL;  // inc hl

_done:
	SET_PC(0x3B10U);
	RET;  // ret

}

int PrintBCDDigit(){
	AND_A(0b00001111);  // and %00001111
	AND_A_A;  // and a
	IF_Z goto _zeroDigit;  // jr z, .zeroDigit
//  nonzero digit
	BIT_B(PRINTNUM_LEADINGZEROS_F);  // bit PRINTNUM_LEADINGZEROS_F, b ; have any non-space characters been printed?
	IF_Z goto _outputDigit;  // jr z, .outputDigit
//  if bit 7 is set, then no numbers have been printed yet
	BIT_B(PRINTNUM_MONEY_F);  // bit PRINTNUM_MONEY_F, b
	IF_Z goto _skipCurrencySymbol;  // jr z, .skipCurrencySymbol
	LD_hl("¥");  // ld [hl], "¥"
	INC_HL;  // inc hl
	RES_B(PRINTNUM_MONEY_F);  // res PRINTNUM_MONEY_F, b

_skipCurrencySymbol:
	SET_PC(0x3B23U);
	RES_B(PRINTNUM_LEADINGZEROS_F);  // res PRINTNUM_LEADINGZEROS_F, b ; unset 7 to indicate that a nonzero digit has been reached

_outputDigit:
	SET_PC(0x3B25U);
	ADD_A("0");  // add "0"
	LD_hli_A;  // ld [hli], a
	JP(mPrintLetterDelay);  // jp PrintLetterDelay


_zeroDigit:
	SET_PC(0x3B2BU);
	BIT_B(PRINTNUM_LEADINGZEROS_F);  // bit PRINTNUM_LEADINGZEROS_F, b ; either printing leading zeroes or already reached a nonzero digit?
	IF_Z goto _outputDigit;  // jr z, .outputDigit ; if so, print a zero digit
	BIT_B(PRINTNUM_LEFTALIGN_F);  // bit PRINTNUM_LEFTALIGN_F, b
	RET_NZ ;  // ret nz
	LD_A(" ");  // ld a, " "
	LD_hli_A;  // ld [hli], a ; if right-aligned, "print" a space by advancing the pointer
	RET;  // ret

}
