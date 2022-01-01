#include "../constants.h"

int NamesPointers(){
//  entries correspond to GetName constants (see constants/text_constants.asm)
	//dba ['PokemonNames']  // dba PokemonNames        ; MON_NAME (not used; jumps to GetPokemonName)
	//dba ['MoveNames']  // dba MoveNames           ; MOVE_NAME
	//dba ['NULL']  // dba NULL                ; DUMMY_NAME
	//dba ['ItemNames']  // dba ItemNames           ; ITEM_NAME
	//dbw ['0', 'wPartyMonOTs']  // dbw 0, wPartyMonOTs      ; PARTY_OT_NAME
	//dbw ['0', 'wOTPartyMonOTs']  // dbw 0, wOTPartyMonOTs    ; ENEMY_OT_NAME
	//dba ['TrainerClassNames']  // dba TrainerClassNames   ; TRAINER_NAME
	//dbw ['4', 'MoveDescriptions']  // dbw 4, MoveDescriptions ; MOVE_DESC_NAME_BROKEN (wrong bank)

	return mGetName;
}

int GetName(){
//  Return name wCurSpecies from name list wNamedObjectType in wStringBuffer1.

	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	PUSH_HL;  // push hl
	PUSH_BC;  // push bc
	PUSH_DE;  // push de

	LD_A_addr(wNamedObjectType);  // ld a, [wNamedObjectType]
	CP_A(MON_NAME);  // cp MON_NAME
	IF_NZ goto _NotPokeName;  // jr nz, .NotPokeName

	LD_A_addr(wCurSpecies);  // ld a, [wCurSpecies]
	LD_addr_A(wNamedObjectIndex);  // ld [wNamedObjectIndex], a
	CALL(mGetPokemonName);  // call GetPokemonName
	LD_HL(MON_NAME_LENGTH);  // ld hl, MON_NAME_LENGTH
	ADD_HL_DE;  // add hl, de
	LD_E_L;  // ld e, l
	LD_D_H;  // ld d, h
	goto _done;  // jr .done


_NotPokeName:
	SET_PC(0x3620U);
	LD_A_addr(wNamedObjectType);  // ld a, [wNamedObjectType]
	DEC_A;  // dec a
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
	LD_HL(mNamesPointers);  // ld hl, NamesPointers
	ADD_HL_DE;  // add hl, de
	ADD_HL_DE;  // add hl, de
	ADD_HL_DE;  // add hl, de
	LD_A_hli;  // ld a, [hli]
	RST(mBankswitch);  // rst Bankswitch
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a

	LD_A_addr(wCurSpecies);  // ld a, [wCurSpecies]
	DEC_A;  // dec a
	CALL(mGetNthString);  // call GetNthString

	LD_DE(wStringBuffer1);  // ld de, wStringBuffer1
	LD_BC(ITEM_NAME_LENGTH);  // ld bc, ITEM_NAME_LENGTH
	CALL(mCopyBytes);  // call CopyBytes


_done:
	SET_PC(0x3642U);
	LD_A_E;  // ld a, e
	LD_addr_A(wUnusedNamesPointer);  // ld [wUnusedNamesPointer], a
	LD_A_D;  // ld a, d
	LD_addr_A(wUnusedNamesPointer + 1);  // ld [wUnusedNamesPointer + 1], a

	POP_DE;  // pop de
	POP_BC;  // pop bc
	POP_HL;  // pop hl
	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret

}

int GetNthString(){
//  Return the address of the
//  ath string starting from hl.

	AND_A_A;  // and a
	RET_Z ;  // ret z

	PUSH_BC;  // push bc
	LD_B_A;  // ld b, a
	LD_C(0x50);  // ld c, "@"

_readChar:
	SET_PC(0x3656U);
	LD_A_hli;  // ld a, [hli]
	CP_A_C;  // cp c
	IF_NZ goto _readChar;  // jr nz, .readChar
	DEC_B;  // dec b
	IF_NZ goto _readChar;  // jr nz, .readChar
	POP_BC;  // pop bc
	RET;  // ret

}

int GetBasePokemonName(){
//  Discards gender (Nidoran).

	PUSH_HL;  // push hl
	CALL(mGetPokemonName);  // call GetPokemonName

	LD_HL(wStringBuffer1);  // ld hl, wStringBuffer1

_loop:
	SET_PC(0x3666U);
	LD_A_hl;  // ld a, [hl]
	CP_A(0x50);  // cp "@"
	IF_Z goto _quit;  // jr z, .quit
	CP_A(0xef);  // cp "♂"
	IF_Z goto _end;  // jr z, .end
	CP_A(0xf5);  // cp "♀"
	IF_Z goto _end;  // jr z, .end
	INC_HL;  // inc hl
	goto _loop;  // jr .loop

_end:
	SET_PC(0x3676U);
	LD_hl(0x50);  // ld [hl], "@"

_quit:
	SET_PC(0x3678U);
	POP_HL;  // pop hl
	RET;  // ret

}

int GetPokemonName(){
//  Get Pokemon name for wNamedObjectIndex.

	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	PUSH_HL;  // push hl
	LD_A(BANK(aPokemonNames));  // ld a, BANK(PokemonNames)
	RST(mBankswitch);  // rst Bankswitch

//  Each name is ten characters
	LD_A_addr(wNamedObjectIndex);  // ld a, [wNamedObjectIndex]
	DEC_A;  // dec a
	LD_HL(mPokemonNames);  // ld hl, PokemonNames
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0

for(int rept = 0; rept < MON_NAME_LENGTH - 1; rept++){
	ADD_HL_DE;  // add hl, de
}

//  Terminator
	LD_DE(wStringBuffer1);  // ld de, wStringBuffer1
	PUSH_DE;  // push de
	LD_BC(MON_NAME_LENGTH - 1);  // ld bc, MON_NAME_LENGTH - 1
	CALL(mCopyBytes);  // call CopyBytes
	LD_HL(wStringBuffer1 + MON_NAME_LENGTH - 1);  // ld hl, wStringBuffer1 + MON_NAME_LENGTH - 1
	LD_hl(0x50);  // ld [hl], "@"
	POP_DE;  // pop de

	POP_HL;  // pop hl
	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret

}

int GetItemName(){
//  Get item name for wNamedObjectIndex.

	PUSH_HL;  // push hl
	PUSH_BC;  // push bc
	LD_A_addr(wNamedObjectIndex);  // ld a, [wNamedObjectIndex]

	CP_A(TM01);  // cp TM01
	IF_NC goto _TM;  // jr nc, .TM

	LD_addr_A(wCurSpecies);  // ld [wCurSpecies], a
	LD_A(ITEM_NAME);  // ld a, ITEM_NAME
	LD_addr_A(wNamedObjectType);  // ld [wNamedObjectType], a
	CALL(mGetName);  // call GetName
	goto _Copied;  // jr .Copied

_TM:
	SET_PC(0x36BFU);
	CALL(mGetTMHMName);  // call GetTMHMName

_Copied:
	SET_PC(0x36C2U);
	LD_DE(wStringBuffer1);  // ld de, wStringBuffer1
	POP_BC;  // pop bc
	POP_HL;  // pop hl
	RET;  // ret

}

int GetTMHMName(){
//  Get TM/HM name for item wNamedObjectIndex.

	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	LD_A_addr(wNamedObjectIndex);  // ld a, [wNamedObjectIndex]
	PUSH_AF;  // push af

//  TM/HM prefix
	CP_A(HM01);  // cp HM01
	PUSH_AF;  // push af
	IF_C goto _TM;  // jr c, .TM

	LD_HL(mGetTMHMName_HMText);  // ld hl, .HMText
	LD_BC(mGetTMHMName_HMTextEnd - mGetTMHMName_HMText);  // ld bc, .HMTextEnd - .HMText
	goto _copy;  // jr .copy


_TM:
	SET_PC(0x36DCU);
	LD_HL(mGetTMHMName_TMText);  // ld hl, .TMText
	LD_BC(mGetTMHMName_TMTextEnd - mGetTMHMName_TMText);  // ld bc, .TMTextEnd - .TMText


_copy:
	SET_PC(0x36E2U);
	LD_DE(wStringBuffer1);  // ld de, wStringBuffer1
	CALL(mCopyBytes);  // call CopyBytes

//  TM/HM number
	PUSH_DE;  // push de
	LD_A_addr(wNamedObjectIndex);  // ld a, [wNamedObjectIndex]
	LD_C_A;  // ld c, a
	CALLFAR(aGetTMHMNumber);  // callfar GetTMHMNumber
	POP_DE;  // pop de

//  HM numbers start from 51, not 1
	POP_AF;  // pop af
	LD_A_C;  // ld a, c
	IF_C goto _not_hm;  // jr c, .not_hm
	SUB_A(NUM_TMS);  // sub NUM_TMS

_not_hm:
	SET_PC(0x36FAU);

//  Divide and mod by 10 to get the top and bottom digits respectively
	LD_B(0xf6);  // ld b, "0"

_mod10:
	SET_PC(0x36FCU);
	SUB_A(10);  // sub 10
	IF_C goto _done_mod;  // jr c, .done_mod
	INC_B;  // inc b
	goto _mod10;  // jr .mod10


_done_mod:
	SET_PC(0x3703U);
	ADD_A(10);  // add 10
	PUSH_AF;  // push af
	LD_A_B;  // ld a, b
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
	POP_AF;  // pop af

	LD_B(0xf6);  // ld b, "0"
	ADD_A_B;  // add b
	LD_de_A;  // ld [de], a

//  End the string
	INC_DE;  // inc de
	LD_A(0x50);  // ld a, "@"
	LD_de_A;  // ld [de], a

	POP_AF;  // pop af
	LD_addr_A(wNamedObjectIndex);  // ld [wNamedObjectIndex], a
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret


_TMText:
	SET_PC(0x371AU);
	//db ['"TM"'];  // db "TM"

_TMTextEnd:
	SET_PC(0x371CU);
	//db ['"@"'];  // db "@"


_HMText:
	SET_PC(0x371DU);
	//db ['"HM"'];  // db "HM"

_HMTextEnd:
	SET_PC(0x371FU);
	//db ['"@"'];  // db "@"

//#include "hm_moves.h"
	return mGetMoveName;
}

int GetMoveName(){
	PUSH_HL;  // push hl

	LD_A(MOVE_NAME);  // ld a, MOVE_NAME
	LD_addr_A(wNamedObjectType);  // ld [wNamedObjectType], a

	LD_A_addr(wNamedObjectIndex);  // ld a, [wNamedObjectIndex] ; move id
	LD_addr_A(wCurSpecies);  // ld [wCurSpecies], a

	CALL(mGetName);  // call GetName
	LD_DE(wStringBuffer1);  // ld de, wStringBuffer1

	POP_HL;  // pop hl
	RET;  // ret

}
