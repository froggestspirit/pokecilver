#include "../constants.h"
#include "pokemon.h"
#include "audio.h"

void DrawBattleHPBar(void) {
    //  Draw an HP bar d tiles long at hl
    //  Fill it up to e pixels

    PUSH_HL;  // push hl
    PUSH_DE;  // push de
    PUSH_BC;  // push bc

    //  Place 'HP:'
    LD_A(0x60);  // ld a, $60
    LD_hli_A;    // ld [hli], a
    LD_A(0x61);  // ld a, $61
    LD_hli_A;    // ld [hli], a

    //  Draw a template
    PUSH_HL;     // push hl
    LD_A(0x62);  // ld a, $62 ; empty bar

    template : LD_hli_A;  // ld [hli], a
    DEC_D;                // dec d
    IF_NZ goto template;  // jr nz, .template
    LD_A(0x6b);           // ld a, $6b ; bar end
    ADD_A_B;              // add b
    LD_hl_A;              // ld [hl], a
    POP_HL;               // pop hl

    //  Safety check # pixels
    LD_A_E;           // ld a, e
    AND_A_A;          // and a
    IF_NZ goto fill;  // jr nz, .fill
    LD_A_C;           // ld a, c
    AND_A_A;          // and a
    IF_Z goto done;   // jr z, .done
    LD_E(1);          // ld e, 1

fill:
    //  Keep drawing tiles until pixel length is reached
    LD_A_E;             // ld a, e
    SUB_A(TILE_WIDTH);  // sub TILE_WIDTH
    IF_C goto lastbar;  // jr c, .lastbar

    LD_E_A;          // ld e, a
    LD_A(0x6a);      // ld a, $6a ; full bar
    LD_hli_A;        // ld [hli], a
    LD_A_E;          // ld a, e
    AND_A_A;         // and a
    IF_Z goto done;  // jr z, .done
    goto fill;       // jr .fill

lastbar:
    LD_A(0x62);  // ld a, $62  ; empty bar
    ADD_A_E;     // add e      ; + e
    LD_hl_A;     // ld [hl], a

done:
    POP_BC;  // pop bc
    POP_DE;  // pop de
    POP_HL;  // pop hl
    RET;     // ret
}

void PrepMonFrontpic(void) {
    SET_PC(0x39B6U);
    LD_A(0x1);                 // ld a, $1
    LD_addr_A(wBoxAlignment);  // ld [wBoxAlignment], a

    return v_PrepMonFrontpic();
}

void v_PrepMonFrontpic(void) {
    SET_PC(0x39BBU);
    LD_A_addr(wCurPartySpecies);  // ld a, [wCurPartySpecies]
                                  //  is a pokemon?
    AND_A_A;                      // and a
    IF_Z goto not_pokemon;        // jr z, .not_pokemon
    CP_A(EGG);                    // cp EGG
    IF_Z goto egg;                // jr z, .egg
    CP_A(NUM_POKEMON + 1);        // cp NUM_POKEMON + 1
    IF_NC goto not_pokemon;       // jr nc, .not_pokemon

egg:
    SET_PC(0x39C9U);
    PUSH_HL;                        // push hl
    LD_DE(vTiles2);                 // ld de, vTiles2
    PREDEF(pGetMonFrontpic);        // predef GetMonFrontpic
    POP_HL;                         // pop hl
    XOR_A_A;                        // xor a
    LDH_addr_A(hGraphicStartTile);  // ldh [hGraphicStartTile], a
    LD_BC((7 << 8) | 7);            // lb bc, 7, 7
    PREDEF(pPlaceGraphic);          // predef PlaceGraphic
    XOR_A_A;                        // xor a
    LD_addr_A(wBoxAlignment);       // ld [wBoxAlignment], a
    RET;                            // ret

not_pokemon:
    SET_PC(0x39E3U);
    XOR_A_A;                      // xor a
    LD_addr_A(wBoxAlignment);     // ld [wBoxAlignment], a
    INC_A;                        // inc a
    LD_addr_A(wCurPartySpecies);  // ld [wCurPartySpecies], a
    RET;                          // ret
}

void PlayStereoCry(void) {
    PUSH_AF;                        // push af
    LD_A(1);                        // ld a, 1
    LD_addr_A(wStereoPanningMask);  // ld [wStereoPanningMask], a
    POP_AF;                         // pop af
    return v_PlayMonCry();          // jr _PlayMonCry
}

void PlayMonCry(void) {
    PUSH_AF;                        // push af
    XOR_A_A;                        // xor a
    LD_addr_A(wStereoPanningMask);  // ld [wStereoPanningMask], a
    LD_addr_A(wCryTracks);          // ld [wCryTracks], a
    POP_AF;                         // pop af

    return v_PlayMonCry();
}

void v_PlayMonCry(void) {
    PUSH_HL;  // push hl
    PUSH_DE;  // push de
    PUSH_BC;  // push bc

    CCALL(aGetCryIndex);  // call GetCryIndex
    IF_C goto done;       // jr c, .done

    LD_E_C;           // ld e, c
    LD_D_B;           // ld d, b
    CCALL(aPlayCry);  // call PlayCry
    WaitSFX();

done:
    POP_BC;  // pop bc
    POP_DE;  // pop de
    POP_HL;  // pop hl
    RET;     // ret
}

void LoadCry(void) {
    //  Load cry bc.

    CCALL(aGetCryIndex);  // call GetCryIndex
    RET_C;                // ret c

    LDH_A_addr(hROMBank);       // ldh a, [hROMBank]
    PUSH_AF;                    // push af
    LD_A(BANK(aPokemonCries));  // ld a, BANK(PokemonCries)
    Bankswitch();               // rst Bankswitch

    LD_HL(mPokemonCries);  // ld hl, PokemonCries
    for (int rept = 0; rept < MON_CRY_LENGTH; rept++) {
        ADD_HL_BC;  // add hl, bc
    }

    LD_E_hl;  // ld e, [hl]
    INC_HL;   // inc hl
    LD_D_hl;  // ld d, [hl]
    INC_HL;   // inc hl

    LD_A_hli;                   // ld a, [hli]
    LD_addr_A(wCryPitch);       // ld [wCryPitch], a
    LD_A_hli;                   // ld a, [hli]
    LD_addr_A(wCryPitch + 1);   // ld [wCryPitch + 1], a
    LD_A_hli;                   // ld a, [hli]
    LD_addr_A(wCryLength);      // ld [wCryLength], a
    LD_A_hl;                    // ld a, [hl]
    LD_addr_A(wCryLength + 1);  // ld [wCryLength + 1], a

    POP_AF;        // pop af
    Bankswitch();  // rst Bankswitch
    AND_A_A;       // and a
    RET;           // ret
}

void GetCryIndex(void) {
    AND_A_A;                // and a
    IF_Z goto no;           // jr z, .no
    CP_A(NUM_POKEMON + 1);  // cp NUM_POKEMON + 1
    IF_NC goto no;          // jr nc, .no

    DEC_A;    // dec a
    LD_C_A;   // ld c, a
    LD_B(0);  // ld b, 0
    AND_A_A;  // and a
    RET;      // ret

no:
    SCF;  // scf
    RET;  // ret
}

void PrintLevel(void) {
    SET_PC(0x3A4CU);
    //  Print wTempMonLevel at hl

    LD_A_addr(wTempMonLevel);  // ld a, [wTempMonLevel]
    LD_hl(0x6e);               // ld [hl], "<LV>"
    INC_HL;                    // inc hl

    //  How many digits?
    LD_C(2);                       // ld c, 2
    CP_A(100);                     // cp 100 ; This is distinct from MAX_LEVEL.
    JR_C(mPrint8BitNumLeftAlign);  // jr c, Print8BitNumLeftAlign

    //  3-digit numbers overwrite the :L.
    DEC_HL;                      // dec hl
    INC_C;                       // inc c
    JR(mPrint8BitNumLeftAlign);  // jr Print8BitNumLeftAlign
}

void PrintLevel_Force3Digits(void) {
    SET_PC(0x3A5CU);
    //  Print :L and all 3 digits
    LD_hl(0x6e);  // ld [hl], "<LV>"
    INC_HL;       // inc hl
    LD_C(3);      // ld c, 3

    return Print8BitNumLeftAlign();
}

void Print8BitNumLeftAlign(void) {
    SET_PC(0x3A61U);
    LD_addr_A(wTextDecimalByte);   // ld [wTextDecimalByte], a
    LD_DE(wTextDecimalByte);       // ld de, wTextDecimalByte
    LD_B(PRINTNUM_LEFTALIGN | 1);  // ld b, PRINTNUM_LEFTALIGN | 1
    JP(mPrintNum);                 // jp PrintNum
}

void GetNthMove(void) {
    //  //  unreferenced
    LD_HL(wListMoves_MoveIndicesBuffer);  // ld hl, wListMoves_MoveIndicesBuffer
    LD_C_A;                               // ld c, a
    LD_B(0);                              // ld b, 0
    ADD_HL_BC;                            // add hl, bc
    LD_A_hl;                              // ld a, [hl]
    RET;                                  // ret
}

void GetBaseData(void) {
    PUSH_BC;                // push bc
    PUSH_DE;                // push de
    PUSH_HL;                // push hl
    LDH_A_addr(hROMBank);   // ldh a, [hROMBank]
    PUSH_AF;                // push af
    LD_A(BANK(aBaseData));  // ld a, BANK(BaseData)
    Bankswitch();           // rst Bankswitch

    //  Egg doesn't have BaseData
    LD_A_addr(wCurSpecies);  // ld a, [wCurSpecies]
    CP_A(EGG);               // cp EGG
    IF_Z goto egg;           // jr z, .egg

    //  Get BaseData
    DEC_A;                  // dec a
    LD_BC(BASE_DATA_SIZE);  // ld bc, BASE_DATA_SIZE
    LD_HL(mBaseData);       // ld hl, BaseData
    CCALL(aAddNTimes);      // call AddNTimes
    LD_DE(wCurBaseData);    // ld de, wCurBaseData
    LD_BC(BASE_DATA_SIZE);  // ld bc, BASE_DATA_SIZE
    CCALL(aCopyBytes);      // call CopyBytes
    goto end;               // jr .end

egg:
    LD_DE(mEggPic);  // ld de, EggPic

    //  Sprite dimensions
    LD_B(0x55);           // ld b, $55 ; 5x5
    LD_HL(wBasePicSize);  // ld hl, wBasePicSize
    LD_hl_B;              // ld [hl], b

    //  Beta front and back sprites
    //  (see pokegold-spaceworld's data/pokemon/base_stats/*)
    LD_HL(wBaseUnusedFrontpic);  // ld hl, wBaseUnusedFrontpic
    LD_hl_E;                     // ld [hl], e
    INC_HL;                      // inc hl
    LD_hl_D;                     // ld [hl], d
    INC_HL;                      // inc hl
    LD_hl_E;                     // ld [hl], e
    INC_HL;                      // inc hl
    LD_hl_D;                     // ld [hl], d
    goto end;                    // jr .end ; useless

end:
    //  Replace Pokedex # with species
    LD_A_addr(wCurSpecies);  // ld a, [wCurSpecies]
    LD_addr_A(wBaseDexNo);   // ld [wBaseDexNo], a

    POP_AF;        // pop af
    Bankswitch();  // rst Bankswitch
    POP_HL;        // pop hl
    POP_DE;        // pop de
    POP_BC;        // pop bc
    RET;           // ret
}

void GetCurNickname(void) {
    SET_PC(0x3ABBU);
    LD_A_addr(wCurPartyMon);    // ld a, [wCurPartyMon]
    LD_HL(wPartyMonNicknames);  // ld hl, wPartyMonNicknames

    return GetNickname();
}

void GetNickname(void) {
    SET_PC(0x3AC1U);
    //  Get nickname a from list hl.

    PUSH_HL;  // push hl
    PUSH_BC;  // push bc

    CCALL(aSkipNames);      // call SkipNames
    LD_DE(wStringBuffer1);  // ld de, wStringBuffer1

    PUSH_DE;                 // push de
    LD_BC(MON_NAME_LENGTH);  // ld bc, MON_NAME_LENGTH
    CCALL(aCopyBytes);       // call CopyBytes
    POP_DE;                  // pop de

    CALLFAR(aCorrectNickErrors);  // callfar CorrectNickErrors

    POP_BC;  // pop bc
    POP_HL;  // pop hl
    RET;     // ret
}
