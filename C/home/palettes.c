#include "../constants.h"

//  Functions dealing with palettes.

int UpdatePalsIfCGB(){
//  update bgp data from wBGPals2
//  update obp data from wOBPals2
//  return carry if successful

//  check cgb
	LDH_A_addr(hCGB);  // ldh a, [hCGB]
	AND_A_A;  // and a
	RET_Z ;  // ret z

}

int UpdateCGBPals(){
//  return carry if successful
//  any pals to update?
	LDH_A_addr(hCGBPalUpdate);  // ldh a, [hCGBPalUpdate]
	AND_A_A;  // and a
	RET_Z ;  // ret z

	LD_HL(wBGPals2);  // ld hl, wBGPals2

//  copy 8 pals to bgpd
	LD_A(1 << rBGPI_AUTO_INCREMENT);  // ld a, 1 << rBGPI_AUTO_INCREMENT
	LDH_addr_A(rBGPI);  // ldh [rBGPI], a
	LD_C(8 / 2);  // ld c, 8 / 2

bgp:
	SET_PC(0x0BEEU);
for(int rept = 0; rept < (1 * PALETTE_SIZE) * 2; rept++){
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rBGPD);  // ldh [rBGPD], a
}

	DEC_C;  // dec c
	IF_NZ goto bgp;  // jr nz, .bgp

//  hl is now wOBPals2

//  copy 8 pals to obpd
	LD_A(1 << rOBPI_AUTO_INCREMENT);  // ld a, 1 << rOBPI_AUTO_INCREMENT
	LDH_addr_A(rOBPI);  // ldh [rOBPI], a
	LD_C(8 / 2);  // ld c, 8 / 2

obp:
	SET_PC(0x0C27U);
for(int rept = 0; rept < (1 * PALETTE_SIZE) * 2; rept++){
	LD_A_hli;  // ld a, [hli]
	LDH_addr_A(rOBPD);  // ldh [rOBPD], a
}

	DEC_C;  // dec c
	IF_NZ goto obp;  // jr nz, .obp

//  clear pal update queue
	XOR_A_A;  // xor a
	LDH_addr_A(hCGBPalUpdate);  // ldh [hCGBPalUpdate], a

	SCF;  // scf
	RET;  // ret

}

int DmgToCgbBGPals(){
//  exists to forego reinserting cgb-converted image data

//  input: a -> bgp

	LDH_addr_A(rBGP);  // ldh [rBGP], a
	PUSH_AF;  // push af

//  Don't need to be here if DMG
	LDH_A_addr(hCGB);  // ldh a, [hCGB]
	AND_A_A;  // and a
	IF_Z goto end;  // jr z, .end

	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc

//  copy & reorder bg pal buffer
	LD_HL(wBGPals2);  // ld hl, wBGPals2 ; to
	LD_DE(wBGPals1);  // ld de, wBGPals1 ; from
//  order
	LDH_A_addr(rBGP);  // ldh a, [rBGP]
	LD_B_A;  // ld b, a
//  all pals
	LD_C(8);  // ld c, 8
	CALL(mCopyPals);  // call CopyPals
//  request pal update
	LD_A(TRUE);  // ld a, TRUE
	LDH_addr_A(hCGBPalUpdate);  // ldh [hCGBPalUpdate], a

	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl

end:
	SET_PC(0x0C7FU);
	POP_AF;  // pop af
	RET;  // ret

}

int DmgToCgbObjPals(){
//  exists to forego reinserting cgb-converted image data

//  input: d -> obp1
//         e -> obp2

	LD_A_E;  // ld a, e
	LDH_addr_A(rOBP0);  // ldh [rOBP0], a
	LD_A_D;  // ld a, d
	LDH_addr_A(rOBP1);  // ldh [rOBP1], a

	LDH_A_addr(hCGB);  // ldh a, [hCGB]
	AND_A_A;  // and a
	RET_Z ;  // ret z

	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc

//  copy & reorder obj pal buffer
	LD_HL(wOBPals2);  // ld hl, wOBPals2 ; to
	LD_DE(wOBPals1);  // ld de, wOBPals1 ; from
//  order
	LDH_A_addr(rOBP0);  // ldh a, [rOBP0]
	LD_B_A;  // ld b, a
//  all pals
	LD_C(8);  // ld c, 8
	CALL(mCopyPals);  // call CopyPals
//  request pal update
	LD_A(TRUE);  // ld a, TRUE
	LDH_addr_A(hCGBPalUpdate);  // ldh [hCGBPalUpdate], a

	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int DmgToCgbObjPal0(){
	LDH_addr_A(rOBP0);  // ldh [rOBP0], a
	PUSH_AF;  // push af

//  Don't need to be here if not CGB
	LDH_A_addr(hCGB);  // ldh a, [hCGB]
	AND_A_A;  // and a
	IF_Z goto dmg;  // jr z, .dmg

	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc

	LD_HL(wOBPals2 + PALETTE_SIZE * 0);  // ld hl, wOBPals2 palette 0
	LD_DE(wOBPals1 + PALETTE_SIZE * 0);  // ld de, wOBPals1 palette 0
	LDH_A_addr(rOBP0);  // ldh a, [rOBP0]
	LD_B_A;  // ld b, a
	LD_C(1);  // ld c, 1
	CALL(mCopyPals);  // call CopyPals
	LD_A(TRUE);  // ld a, TRUE
	LDH_addr_A(hCGBPalUpdate);  // ldh [hCGBPalUpdate], a

	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl


dmg:
	SET_PC(0x0CC4U);
	POP_AF;  // pop af
	RET;  // ret

}

int DmgToCgbObjPal1(){
	LDH_addr_A(rOBP1);  // ldh [rOBP1], a
	PUSH_AF;  // push af

	LDH_A_addr(hCGB);  // ldh a, [hCGB]
	AND_A_A;  // and a
	IF_Z goto dmg;  // jr z, .dmg

	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc

	LD_HL(wOBPals2 + PALETTE_SIZE * 1);  // ld hl, wOBPals2 palette 1
	LD_DE(wOBPals1 + PALETTE_SIZE * 1);  // ld de, wOBPals1 palette 1
	LDH_A_addr(rOBP1);  // ldh a, [rOBP1]
	LD_B_A;  // ld b, a
	LD_C(1);  // ld c, 1
	CALL(mCopyPals);  // call CopyPals
	LD_A(TRUE);  // ld a, TRUE
	LDH_addr_A(hCGBPalUpdate);  // ldh [hCGBPalUpdate], a

	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl


dmg:
	SET_PC(0x0CE6U);
	POP_AF;  // pop af
	RET;  // ret

}

int CopyPals(){
//  copy c palettes in order b from de to hl

	PUSH_BC;  // push bc
	LD_C(NUM_PAL_COLORS);  // ld c, NUM_PAL_COLORS

loop:
	SET_PC(0x0CEBU);
	PUSH_DE;  // push de
	PUSH_HL;  // push hl

//  get pal color
	LD_A_B;  // ld a, b
	maskbits(1 << PAL_COLOR_SIZE, 0);  // maskbits 1 << PAL_COLOR_SIZE
//  2 bytes per color
	ADD_A_A;  // add a
	LD_L_A;  // ld l, a
	LD_H(0);  // ld h, 0
	ADD_HL_DE;  // add hl, de
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]

//  dest
	POP_HL;  // pop hl
//  write color
	LD_hl_E;  // ld [hl], e
	INC_HL;  // inc hl
	LD_hl_D;  // ld [hl], d
	INC_HL;  // inc hl
//  next pal color
for(int rept = 0; rept < PAL_COLOR_SIZE; rept++){
	SRL_B;  // srl b
}
//  source
	POP_DE;  // pop de
//  done pal?
	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop

//  de += 8 (next pal)
	LD_A(PALETTE_SIZE);  // ld a, PALETTE_SIZE
	ADD_A_E;  // add e
	IF_NC goto ok;  // jr nc, .ok
	INC_D;  // inc d

ok:
	SET_PC(0x0D0BU);
	LD_E_A;  // ld e, a

//  how many more pals?
	POP_BC;  // pop bc
	DEC_C;  // dec c
	JR_NZ (mCopyPals);  // jr nz, CopyPals
	RET;  // ret

}

int ClearVBank1(){
	LDH_A_addr(hCGB);  // ldh a, [hCGB]
	AND_A_A;  // and a
	RET_Z ;  // ret z

	LD_A(1);  // ld a, 1
	LDH_addr_A(rVBK);  // ldh [rVBK], a

	LD_HL(VRAM_Begin);  // ld hl, VRAM_Begin
	LD_BC(VRAM_End - VRAM_Begin);  // ld bc, VRAM_End - VRAM_Begin
	XOR_A_A;  // xor a
	CALL(mByteFill);  // call ByteFill

	LD_A(0);  // ld a, 0
	LDH_addr_A(rVBK);  // ldh [rVBK], a
	RET;  // ret

}

int ReloadPalettes(){
	hlcoord(0, 0, wTilemap);  // hlcoord 0, 0
	decoord(0, 0, wAttrmap);  // decoord 0, 0, wAttrmap
	LD_BC(SCREEN_WIDTH * SCREEN_HEIGHT);  // ld bc, SCREEN_WIDTH * SCREEN_HEIGHT

loop:
	SET_PC(0x0D31U);
	LD_A_hli;  // ld a, [hli]
	CP_A(0x60);  // cp "■"
	IF_C goto skip;  // jr c, .skip
	LD_A(PAL_BG_TEXT);  // ld a, PAL_BG_TEXT
	LD_de_A;  // ld [de], a

skip:
	SET_PC(0x0D39U);
	INC_DE;  // inc de
	DEC_BC;  // dec bc
	LD_A_B;  // ld a, b
	OR_A_C;  // or c
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

int ReloadSpritesNoPalettes(){
	LDH_A_addr(hCGB);  // ldh a, [hCGB]
	AND_A_A;  // and a
	RET_Z ;  // ret z
	LD_HL(wBGPals2);  // ld hl, wBGPals2
	LD_BC((8 * PALETTE_SIZE) + (2 * PALETTE_SIZE));  // ld bc, (8 palettes) + (2 palettes)
	XOR_A_A;  // xor a
	CALL(mByteFill);  // call ByteFill
	LD_A(TRUE);  // ld a, TRUE
	LDH_addr_A(hCGBPalUpdate);  // ldh [hCGBPalUpdate], a
	CALL(mDelayFrame);  // call DelayFrame
	RET;  // ret

}

int SwapTextboxPalettes(){
	HOMECALL(av_SwapTextboxPalettes);  // homecall _SwapTextboxPalettes
	RET;  // ret

}

int ScrollBGMapPalettes(){
	HOMECALL(av_ScrollBGMapPalettes);  // homecall _ScrollBGMapPalettes
	RET;  // ret

}
