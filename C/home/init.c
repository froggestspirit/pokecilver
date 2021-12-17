Reset::
	call InitSound
	XOR_A_A;
	LD_addr_A(hMapAnims);
	call ClearPalettes
	// ei

	ld hl, wJoypadDisable
	SET_hl(JOYPAD_DISABLE_SGB_TRANSFER_F);

	ld c, 32
	call DelayFrames

	jr Init

_Start::
	cp 0x11
	jr z, .cgb
	XOR_A_A; // FALSE
	jr .load

.cgb
	ld a, TRUE

.load
	LD_addr_A(hCGB);

Init::
	// di

	XOR_A_A;
	LD_addr_A(rIF);
	LD_addr_A(rIE);
	LD_addr_A(rRP);
	LD_addr_A(rSCX);
	LD_addr_A(rSCY);
	LD_addr_A(rSB);
	LD_addr_A(rSC);
	LD_addr_A(rWX);
	LD_addr_A(rWY);
	LD_addr_A(rBGP);
	LD_addr_A(rOBP0);
	LD_addr_A(rOBP1);
	LD_addr_A(rTMA);
	LD_addr_A(rTAC);
	LD_addr_A(wBetaTitleSequenceOpeningType);

	ld a, %100 // Start timer at 4096Hz
	LD_addr_A(rTAC);

.wait
	ldh a, [rLY]
	cp LY_VBLANK + 1
	jr nz, .wait

	XOR_A_A;
	LD_addr_A(rLCDC);

// Clear WRAM
	ld hl, WRAM0_Begin // comment
	ld bc, WRAM1_End - WRAM0_Begin
.ByteFill:
	ld [hl], 0
	inc hl
	dec bc
	ld a, b
	or c
	jr nz, .ByteFill

	ld sp, wStackTop

	call ClearVRAM

// Clear HRAM
	ldh a, [hCGB]
	push af
	XOR_A_A;
	ld hl, HRAM_Begin
	ld bc, HRAM_End - HRAM_Begin
	call ByteFill
	pop af
	LD_addr_A(hCGB);

	call ClearSprites

	ld a, BANK(WriteOAMDMACodeToHRAM) // aka BANK(GameInit)
	rst Bankswitch

	call WriteOAMDMACodeToHRAM

	XOR_A_A;
	LD_addr_A(hMapAnims);
	LD_addr_A(hSCX);
	LD_addr_A(hSCY);
	LD_addr_A(rJOYP);

	ld a, 0x8 // HBlank int enable
	LD_addr_A(rSTAT);

	ld a, 0x90
	LD_addr_A(hWY);
	LD_addr_A(rWY);

	ld a, 7
	LD_addr_A(hWX);
	LD_addr_A(rWX);

	ld a, CONNECTION_NOT_ESTABLISHED
	LD_addr_A(hSerialConnectionStatus);

	ld h, HIGH(vBGMap0)
	call BlankBGMap
	ld h, HIGH(vBGMap1)
	call BlankBGMap

	callfar InitCGBPals

	ld a, HIGH(vBGMap1)
	LD_addr_A(hBGMapAddress + 1);
	XOR_A_A; // LOW(vBGMap1)
	LD_addr_A(hBGMapAddress);

	farcall StartClock

	ld a, SRAM_ENABLE
	LD_addr_A(MBC3SRamEnable);
	ld a, SRAM_DISABLE
	LD_addr_A(MBC3LatchClock);
	LD_addr_A(MBC3SRamEnable);

	ld a, LCDC_DEFAULT // %11100011
	// LCD on
	// Win tilemap 1
	// Win on
	// BG/Win tiledata 0
	// BG Tilemap 0
	// OBJ 8x8
	// OBJ on
	// BG on
	LD_addr_A(rLCDC);

	ld a, IE_DEFAULT
	LD_addr_A(rIE);
	// ei

	call DelayFrame

	predef InitSGBBorder

	call InitSound
	XOR_A_A;
	LD_addr_A(wMapMusic);
	jp GameInit

ClearVRAM::
	ld hl, VRAM_Begin
	ld bc, VRAM_End - VRAM_Begin
	XOR_A_A;
	call ByteFill
	ret

BlankBGMap::
	ld a, " "
	jr FillBGMap

FillBGMap_l:: // unreferenced
	ld a, l
	// fallthrough

FillBGMap::
	ld de, vBGMap1 - vBGMap0
	ld l, e
.loop
	LD_addr_A(hli);
	dec e
	jr nz, .loop
	dec d
	jr nz, .loop
	ret
