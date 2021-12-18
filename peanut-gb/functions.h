#include "peanut_gb.h"
#include "macros.h"
#include "global.h"

#include "../C/constants/wram_constants.h"

#define ROM_SIZE 0x200000


extern struct gb_s;

int (*func[ROM_SIZE])(struct gb_s *gb);

/*int _Start(struct gb_s *gb){
	CP_A(0x11);
	IF_Z goto _cgb;
	XOR_A_A;
	goto _load;

_cgb:
	LD_A(TRUE);

_load:
	LD_addr_A(hCGB);
    return 0x05D1;  // Function ends without a return, give it the address PC should be set to
}*/

int Reset(struct gb_s *gb){
	CALL(aInitSound);  // call InitSound
	XOR_A_A;  // xor a
	LD_addr_A(hMapAnims);  // ldh [hMapAnims], a
	CALL(aClearPalettes);  // call ClearPalettes
	  // ei

	LD_HL(wJoypadDisable);  // ld hl, wJoypadDisable
	SET_hl(JOYPAD_DISABLE_SGB_TRANSFER_F);  // set JOYPAD_DISABLE_SGB_TRANSFER_F, [hl]

	LD_C(32);  // ld c, 32
	CALL(aDelayFrames);  // call DelayFrames

	return aInit;  // jr Init

}

int _Start(struct gb_s *gb){
	CP_A(0x11);  // cp $11
	IF_Z goto _cgb;  // jr z, .cgb
	XOR_A_A;  // xor a ; FALSE
	goto _load;  // jr .load


_cgb:
	LD_A(TRUE);  // ld a, TRUE


_load:
	LD_addr_A(hCGB);  // ldh [hCGB], a

	return aInit;
}

int Init(struct gb_s *gb){
	  // di

	XOR_A_A;  // xor a
	LD_addr_A(rIF);  // ldh [rIF], a
	LD_addr_A(rIE);  // ldh [rIE], a
	LD_addr_A(rRP);  // ldh [rRP], a
	LD_addr_A(rSCX);  // ldh [rSCX], a
	LD_addr_A(rSCY);  // ldh [rSCY], a
	LD_addr_A(rSB);  // ldh [rSB], a
	LD_addr_A(rSC);  // ldh [rSC], a
	LD_addr_A(rWX);  // ldh [rWX], a
	LD_addr_A(rWY);  // ldh [rWY], a
	LD_addr_A(rBGP);  // ldh [rBGP], a
	LD_addr_A(rOBP0);  // ldh [rOBP0], a
	LD_addr_A(rOBP1);  // ldh [rOBP1], a
	LD_addr_A(rTMA);  // ldh [rTMA], a
	LD_addr_A(rTAC);  // ldh [rTAC], a
	LD_addr_A(wBetaTitleSequenceOpeningType);  // ld [wBetaTitleSequenceOpeningType], a

	LD_A(0b100);  // ld a, %100 ; Start timer at 4096Hz
	LD_addr_A(rTAC);  // ldh [rTAC], a


_wait:
	LD_A(rLY);  // ldh a, [rLY]
	CP_A(LY_VBLANK + 1);  // cp LY_VBLANK + 1
	IF_NZ goto _wait;  // jr nz, .wait

	XOR_A_A;  // xor a
	LD_addr_A(rLCDC);  // ldh [rLCDC], a

//  Clear WRAM
	LD_HL(WRAM0_Begin);  // ld hl, WRAM0_Begin
	LD_BC(WRAM1_End - WRAM0_Begin);  // ld bc, WRAM1_End - WRAM0_Begin

_ByteFill:
	LD_hl(0);  // ld [hl], 0
	INC_HL;  // inc hl
	DEC_BC;  // dec bc
	LD_A_B;  // ld a, b
	OR_A_C;  // or c
	IF_NZ goto _ByteFill;  // jr nz, .ByteFill

	LD_SP(wStackTop);  // ld sp, wStackTop

	CALL(aClearVRAM);  // call ClearVRAM

//  Clear HRAM
	LD_A(hCGB);  // ldh a, [hCGB]
	PUSH_AF;  // push af
	XOR_A_A;  // xor a
	LD_HL(HRAM_Begin);  // ld hl, HRAM_Begin
	LD_BC(HRAM_End - HRAM_Begin);  // ld bc, HRAM_End - HRAM_Begin
	CALL(aByteFill);  // call ByteFill
	POP_AF;  // pop af
	LD_addr_A(hCGB);  // ldh [hCGB], a

	CALL(aClearSprites);  // call ClearSprites

	LD_A(BANK(WriteOAMDMACodeToHRAM));  // ld a, BANK(WriteOAMDMACodeToHRAM) ; aka BANK(GameInit)
	CALL(aBankswitch);  // rst Bankswitch

	CALL(aWriteOAMDMACodeToHRAM);  // call WriteOAMDMACodeToHRAM

	XOR_A_A;  // xor a
	LD_addr_A(hMapAnims);  // ldh [hMapAnims], a
	LD_addr_A(hSCX);  // ldh [hSCX], a
	LD_addr_A(hSCY);  // ldh [hSCY], a
	LD_addr_A(rJOYP);  // ldh [rJOYP], a

	LD_A(0x8);  // ld a, $8 ; HBlank int enable
	LD_addr_A(rSTAT);  // ldh [rSTAT], a

	LD_A(0x90);  // ld a, $90
	LD_addr_A(hWY);  // ldh [hWY], a
	LD_addr_A(rWY);  // ldh [rWY], a

	LD_A(7);  // ld a, 7
	LD_addr_A(hWX);  // ldh [hWX], a
	LD_addr_A(rWX);  // ldh [rWX], a

	LD_A(CONNECTION_NOT_ESTABLISHED);  // ld a, CONNECTION_NOT_ESTABLISHED
	LD_addr_A(hSerialConnectionStatus);  // ldh [hSerialConnectionStatus], a

	LD_H(HIGH(vBGMap0));  // ld h, HIGH(vBGMap0)
	CALL(aBlankBGMap);  // call BlankBGMap
	LD_H(HIGH(vBGMap1));  // ld h, HIGH(vBGMap1)
	CALL(aBlankBGMap);  // call BlankBGMap

	CALLFAR(aInitCGBPals);  // callfar InitCGBPals

	LD_A(HIGH(vBGMap1));  // ld a, HIGH(vBGMap1)
	LD_addr_A(hBGMapAddress + 1);  // ldh [hBGMapAddress + 1], a
	XOR_A_A;  // xor a ; LOW(vBGMap1)
	LD_addr_A(hBGMapAddress);  // ldh [hBGMapAddress], a

	FARCALL(aStartClock);  // farcall StartClock

	LD_A(SRAM_ENABLE);  // ld a, SRAM_ENABLE
	LD_addr_A(MBC3SRamEnable);  // ld [MBC3SRamEnable], a
	LD_A(SRAM_DISABLE);  // ld a, SRAM_DISABLE
	LD_addr_A(MBC3LatchClock);  // ld [MBC3LatchClock], a
	LD_addr_A(MBC3SRamEnable);  // ld [MBC3SRamEnable], a

	LD_A(LCDC_DEFAULT);  // ld a, LCDC_DEFAULT ; %11100011
// ; LCD on
// ; Win tilemap 1
// ; Win on
// ; BG/Win tiledata 0
// ; BG Tilemap 0
// ; OBJ 8x8
// ; OBJ on
// ; BG on
	LD_addr_A(rLCDC);  // ldh [rLCDC], a

	LD_A(IE_DEFAULT);  // ld a, IE_DEFAULT
	LD_addr_A(rIE);  // ldh [rIE], a
	  // ei

	CALL(aDelayFrame);  // call DelayFrame

	//PREDEF(aInitSGBBorder);  // predef InitSGBBorder

	CALL(aInitSound);  // call InitSound
	XOR_A_A;  // xor a
	LD_addr_A(wMapMusic);  // ld [wMapMusic], a
	return aGameInit;  // jp GameInit

}

int ClearVRAM(struct gb_s *gb){
	LD_HL(VRAM_Begin);  // ld hl, VRAM_Begin
	LD_BC(VRAM_End - VRAM_Begin);  // ld bc, VRAM_End - VRAM_Begin
	XOR_A_A;  // xor a
	CALL(aByteFill);  // call ByteFill
	return -1;  // ret

}

int BlankBGMap(struct gb_s *gb){
	LD_A(" ");  // ld a, " "
	return aFillBGMap;  // jr FillBGMap

}

int FillBGMap_l(struct gb_s *gb){  //  unreferenced
	LD_A_L;  // ld a, l
// ; fallthrough

	return aFillBGMap;
}

int FillBGMap(struct gb_s *gb){
	LD_DE(vBGMap1 - vBGMap0);  // ld de, vBGMap1 - vBGMap0
	LD_L_E;  // ld l, e

_loop:
	LD_hli_A;  // ld [hli], a
	DEC_E;  // dec e
	IF_NZ goto _loop;  // jr nz, .loop
	DEC_D;  // dec d
	IF_NZ goto _loop;  // jr nz, .loop
	return -1;  // ret

}



void init_function_pointers(){
    for(int i = 0; i < ROM_SIZE; i++) func[i] = NULL;
	func[aReset] = Reset;
	func[a_Start] = _Start;
	func[aInit] = Init;
	func[aClearVRAM] = ClearVRAM;
	func[aBlankBGMap] = BlankBGMap;
	func[aFillBGMap_l] = FillBGMap_l;
	func[aFillBGMap] = FillBGMap;
}
