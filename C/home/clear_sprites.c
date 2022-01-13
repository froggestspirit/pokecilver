#include "../constants.h"

void ClearSprites(void){
//  Erase OAM data
	LD_HL(wVirtualOAM);  // ld hl, wVirtualOAM
	LD_B(wVirtualOAMEnd - wVirtualOAM);  // ld b, wVirtualOAMEnd - wVirtualOAM
	XOR_A_A;  // xor a

loop:
	LD_hli_A;  // ld [hli], a
	DEC_B;  // dec b
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

void HideSprites(void){
//  Set all OAM y-positions to 160 to hide them offscreen
	LD_HL(wVirtualOAMSprite00YCoord);  // ld hl, wVirtualOAMSprite00YCoord
	LD_DE(SPRITEOAMSTRUCT_LENGTH);  // ld de, SPRITEOAMSTRUCT_LENGTH
	LD_B(NUM_SPRITE_OAM_STRUCTS);  // ld b, NUM_SPRITE_OAM_STRUCTS
	LD_A(SCREEN_WIDTH_PX);  // ld a, SCREEN_WIDTH_PX

loop:
	LD_hl_A;  // ld [hl], a ; y
	ADD_HL_DE;  // add hl, de
	DEC_B;  // dec b
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}
