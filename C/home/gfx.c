#include "../constants.h"

#define TILES_PER_CYCLE 8

int FarDecompressBufferedPic(){
	SET_PC(0x0D6EU);
//  //  unreferenced
	LD_B_A;  // ld b, a
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A_B;  // ld a, b
	RST(mBankswitch);  // rst Bankswitch

	LD_A(BANK(sDecompressBuffer));  // ld a, BANK(sDecompressBuffer)
	CCALL(aOpenSRAM);  // call OpenSRAM
	LD_HL(sDecompressBuffer);  // ld hl, sDecompressBuffer
	LD_BC(7 * 7 * LEN_2BPP_TILE);  // ld bc, 7 * 7 tiles
	XOR_A_A;  // xor a
	CCALL(aByteFill);  // call ByteFill

	LD_HL(wFarDecompressPicPointer);  // ld hl, wFarDecompressPicPointer
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	LD_DE(sDecompressBuffer);  // ld de, sDecompressBuffer
	CALL(mDecompress);  // call Decompress

	CCALL(aCloseSRAM);  // call CloseSRAM
	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret

}

int UpdatePlayerSprite(){
	SET_PC(0x0D95U);
	FARCALL(av_UpdatePlayerSprite);  // farcall _UpdatePlayerSprite
	RET;  // ret

}

int LoadStandardFont(){
	SET_PC(0x0D9CU);
	FARCALL(av_LoadStandardFont);  // farcall _LoadStandardFont
	RET;  // ret

}

int LoadFontsBattleExtra(){
	SET_PC(0x0DA3U);
	FARCALL(av_LoadFontsBattleExtra);  // farcall _LoadFontsBattleExtra
	RET;  // ret

}

int LoadFontsExtra(){
	SET_PC(0x0DAAU);
	FARCALL(av_LoadFontsExtra);  // farcall _LoadFontsExtra
	RET;  // ret

}

int DecompressRequest2bpp(){
	SET_PC(0x0DB1U);
	PUSH_DE;  // push de
	LD_A(BANK(sScratch));  // ld a, BANK(sScratch)
	CCALL(aOpenSRAM);  // call OpenSRAM
	PUSH_BC;  // push bc

	LD_DE(sScratch);  // ld de, sScratch
	LD_A_B;  // ld a, b
	CALL(mFarDecompress);  // call FarDecompress

	POP_BC;  // pop bc
	POP_HL;  // pop hl

	LD_DE(sScratch);  // ld de, sScratch
	CALL(mRequest2bpp);  // call Request2bpp
	CCALL(aCloseSRAM);  // call CloseSRAM
	RET;  // ret

}

int FarCopyBytes(){
//  copy bc bytes from a:hl to de

	LD_addr_A(wTempBank);  // ld [wTempBank], a
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A_addr(wTempBank);  // ld a, [wTempBank]
	Bankswitch();  // rst Bankswitch

	CCALL(aCopyBytes);  // call CopyBytes

	POP_AF;  // pop af
	Bankswitch();  // rst Bankswitch
	RET;  // ret

}

int FarCopyBytesDouble(){
//  Copy bc bytes from a:hl to bc*2 bytes at de,
//  doubling each byte in the process.

	LD_addr_A(wTempBank);  // ld [wTempBank], a
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A_addr(wTempBank);  // ld a, [wTempBank]
	Bankswitch();  // rst Bankswitch

//  switcheroo, de <> hl
	LD_A_H;  // ld a, h
	LD_H_D;  // ld h, d
	LD_D_A;  // ld d, a
	LD_A_L;  // ld a, l
	LD_L_E;  // ld l, e
	LD_E_A;  // ld e, a

	INC_B;  // inc b
	INC_C;  // inc c
	goto dec;  // jr .dec


loop:
	LD_A_de;  // ld a, [de]
	INC_DE;  // inc de
	LD_hli_A;  // ld [hli], a
	LD_hli_A;  // ld [hli], a

dec:
	DEC_C;  // dec c
	IF_NZ goto loop;  // jr nz, .loop
	DEC_B;  // dec b
	IF_NZ goto loop;  // jr nz, .loop

	POP_AF;  // pop af
	Bankswitch();  // rst Bankswitch
	RET;  // ret

}

int Request2bpp(){
	SET_PC(0x0DFCU);
//  Load 2bpp at b:de to occupy c tiles of hl.
	LDH_A_addr(hBGMapMode);  // ldh a, [hBGMapMode]
	PUSH_AF;  // push af
	XOR_A_A;  // xor a
	LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a

	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A_B;  // ld a, b
	RST(mBankswitch);  // rst Bankswitch

	LD_A_E;  // ld a, e
	LD_addr_A(wRequested2bppSource);  // ld [wRequested2bppSource], a
	LD_A_D;  // ld a, d
	LD_addr_A(wRequested2bppSource + 1);  // ld [wRequested2bppSource + 1], a
	LD_A_L;  // ld a, l
	LD_addr_A(wRequested2bppDest);  // ld [wRequested2bppDest], a
	LD_A_H;  // ld a, h
	LD_addr_A(wRequested2bppDest + 1);  // ld [wRequested2bppDest + 1], a

loop:
	SET_PC(0x0E17U);
	LD_A_C;  // ld a, c
	CP_A(TILES_PER_CYCLE);  // cp TILES_PER_CYCLE
	IF_NC goto cycle;  // jr nc, .cycle

	LD_addr_A(wRequested2bppSize);  // ld [wRequested2bppSize], a
	CALL(mDelayFrame);  // call DelayFrame

	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch

	POP_AF;  // pop af
	LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a
	RET;  // ret


cycle:
	SET_PC(0x0E28U);
	LD_A(TILES_PER_CYCLE);  // ld a, TILES_PER_CYCLE
	LD_addr_A(wRequested2bppSize);  // ld [wRequested2bppSize], a

	CALL(mDelayFrame);  // call DelayFrame
	LD_A_C;  // ld a, c
	SUB_A(TILES_PER_CYCLE);  // sub TILES_PER_CYCLE
	LD_C_A;  // ld c, a
	goto loop;  // jr .loop

	return Request1bpp();
}

int Request1bpp(){
	SET_PC(0x0E36U);
//  Load 1bpp at b:de to occupy c tiles of hl.
	LDH_A_addr(hBGMapMode);  // ldh a, [hBGMapMode]
	PUSH_AF;  // push af
	XOR_A_A;  // xor a
	LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a

	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A_B;  // ld a, b
	RST(mBankswitch);  // rst Bankswitch

	LD_A_E;  // ld a, e
	LD_addr_A(wRequested1bppSource);  // ld [wRequested1bppSource], a
	LD_A_D;  // ld a, d
	LD_addr_A(wRequested1bppSource + 1);  // ld [wRequested1bppSource + 1], a
	LD_A_L;  // ld a, l
	LD_addr_A(wRequested1bppDest);  // ld [wRequested1bppDest], a
	LD_A_H;  // ld a, h
	LD_addr_A(wRequested1bppDest + 1);  // ld [wRequested1bppDest + 1], a

loop:
	SET_PC(0x0E51U);
	LD_A_C;  // ld a, c
	CP_A(TILES_PER_CYCLE);  // cp TILES_PER_CYCLE
	IF_NC goto cycle;  // jr nc, .cycle

	LD_addr_A(wRequested1bppSize);  // ld [wRequested1bppSize], a
	CALL(mDelayFrame);  // call DelayFrame

	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch

	POP_AF;  // pop af
	LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a
	RET;  // ret


cycle:
	SET_PC(0x0E62U);
	LD_A(TILES_PER_CYCLE);  // ld a, TILES_PER_CYCLE
	LD_addr_A(wRequested1bppSize);  // ld [wRequested1bppSize], a

	CALL(mDelayFrame);  // call DelayFrame
	LD_A_C;  // ld a, c
	SUB_A(TILES_PER_CYCLE);  // sub TILES_PER_CYCLE
	LD_C_A;  // ld c, a
	goto loop;  // jr .loop

	return Get2bpp();
}

int Get2bpp(){
	SET_PC(0x0E70U);
//  copy c 2bpp tiles from b:de to hl
	LDH_A_addr(rLCDC);  // ldh a, [rLCDC]
	BIT_A(rLCDC_ENABLE);  // bit rLCDC_ENABLE, a
	JP_NZ (mRequest2bpp);  // jp nz, Request2bpp

	PUSH_HL;  // push hl
	LD_H_D;  // ld h, d
	LD_L_E;  // ld l, e
	POP_DE;  // pop de

//  bank
	LD_A_B;  // ld a, b

//  bc = c * LEN_2BPP_TILE
	PUSH_AF;  // push af
	SWAP_C;  // swap c
	LD_A(0xf);  // ld a, $f
	AND_A_C;  // and c
	LD_B_A;  // ld b, a
	LD_A(0xf0);  // ld a, $f0
	AND_A_C;  // and c
	LD_C_A;  // ld c, a
	POP_AF;  // pop af

	return FarCopyBytes();  // jp FarCopyBytes

}

int Get1bpp(){
	SET_PC(0x0E8BU);
//  copy c 1bpp tiles from b:de to hl
	LDH_A_addr(rLCDC);  // ldh a, [rLCDC]
	BIT_A(rLCDC_ENABLE);  // bit rLCDC_ENABLE, a
	JP_NZ (mRequest1bpp);  // jp nz, Request1bpp

	PUSH_DE;  // push de
	LD_D_H;  // ld d, h
	LD_E_L;  // ld e, l

//  bank
	LD_A_B;  // ld a, b

//  bc = c * LEN_1BPP_TILE
	PUSH_AF;  // push af
	LD_H(0);  // ld h, 0
	LD_L_C;  // ld l, c
	ADD_HL_HL;  // add hl, hl
	ADD_HL_HL;  // add hl, hl
	ADD_HL_HL;  // add hl, hl
	LD_B_H;  // ld b, h
	LD_C_L;  // ld c, l
	POP_AF;  // pop af

	POP_HL;  // pop hl
	return FarCopyBytesDouble();  // jp FarCopyBytesDouble

}

int DuplicateGet2bpp(){
	SET_PC(0x0EA4U);
//  //  unreferenced
	LDH_A_addr(rLCDC);  // ldh a, [rLCDC]
	ADD_A_A;  // add a
	JP_C (mRequest2bpp);  // jp c, Request2bpp

	PUSH_DE;  // push de
	PUSH_HL;  // push hl

//  bank
	LD_A_B;  // ld a, b

//  bc = c * LEN_2BPP_TILE
	LD_H(0);  // ld h, 0
	LD_L_C;  // ld l, c
	ADD_HL_HL;  // add hl, hl
	ADD_HL_HL;  // add hl, hl
	ADD_HL_HL;  // add hl, hl
	ADD_HL_HL;  // add hl, hl
	LD_B_H;  // ld b, h
	LD_C_L;  // ld c, l

	POP_DE;  // pop de
	POP_HL;  // pop hl
	return FarCopyBytes();  // jp FarCopyBytes

}
