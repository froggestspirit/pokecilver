#include "../constants.h"

int FarDecompress(){
	SET_PC(0x0AEEU);
//  Decompress graphics data from a:hl to de.

	LD_addr_A(wLZBank);  // ld [wLZBank], a
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A_addr(wLZBank);  // ld a, [wLZBank]
	RST(mBankswitch);  // rst Bankswitch

	CALL(mDecompress);  // call Decompress

	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret

}

int Decompress(){
	SET_PC(0x0AFEU);
//  Pokemon GSC uses an lz variant (lz3) for compression.
//  This is mainly (but not necessarily) used for graphics.

//  This function decompresses lz-compressed data from hl to de.

#define LZ_END 0xff  //  Compressed data is terminated with $ff.

//  A typical control command consists of:

#define LZ_CMD 0b11100000  //  command id (bits 5-7)
#define LZ_LEN 0b00011111  //  length n   (bits 0-4)

//  Additional parameters are read during command execution.

//  Commands:

#define LZ_LITERAL 0 << 5  //  Read literal data for n bytes.
#define LZ_ITERATE 1 << 5  //  Write the same byte for n bytes.
#define LZ_ALTERNATE 2 << 5  //  Alternate two bytes for n bytes.
#define LZ_ZERO 3 << 5  //  Write 0 for n bytes.

//  Another class of commands reuses data from the decompressed output.
#define LZ_RW 2 + 5  //  bit

//  These commands take a signed offset to start copying from.
//  Wraparound is simulated.
//  Positive offsets (15-bit) are added to the start address.
//  Negative offsets (7-bit) are subtracted from the current position.

#define LZ_REPEAT 4 << 5  //  Repeat n bytes from the offset.
#define LZ_FLIP 5 << 5  //  Repeat n bitflipped bytes.
#define LZ_REVERSE 6 << 5  //  Repeat n bytes in reverse.

//  If the value in the count needs to be larger than 5 bits,
//  LZ_LONG can be used to expand the count to 10 bits.
#define LZ_LONG 7 << 5

//  A new control command is read in bits 2-4.
//  The top two bits of the length are bits 0-1.
//  Another byte is read containing the bottom 8 bits.
#define LZ_LONG_HI 0b00000011

//  In other words, the structure of the command becomes
//  111xxxyy yyyyyyyy
//  x: the new control command
//  y: the length

//  For more information, refer to the code below and in extras/gfx.py.

// ; Save the output address
// ; for rewrite commands.
	LD_A_E;  // ld a, e
	LD_addr_A(wLZAddress);  // ld [wLZAddress], a
	LD_A_D;  // ld a, d
	LD_addr_A(wLZAddress + 1);  // ld [wLZAddress + 1], a


Main:
	SET_PC(0x0B06U);
	LD_A_hl;  // ld a, [hl]
	CP_A(LZ_END);  // cp LZ_END
	RET_Z ;  // ret z

	AND_A(LZ_CMD);  // and LZ_CMD

	CP_A(LZ_LONG);  // cp LZ_LONG
	IF_NZ goto l_short;  // jr nz, .short

//  The count is now 10 bits.

// ; Read the next 3 bits.
// ; %00011100 -> %11100000
	LD_A_hl;  // ld a, [hl]
	ADD_A_A;  // add a
	ADD_A_A;  // add a ; << 3
	ADD_A_A;  // add a

// ; This is our new control code.
	AND_A(LZ_CMD);  // and LZ_CMD
	PUSH_AF;  // push af

	LD_A_hli;  // ld a, [hli]
	AND_A(LZ_LONG_HI);  // and LZ_LONG_HI
	LD_B_A;  // ld b, a
	LD_A_hli;  // ld a, [hli]
	LD_C_A;  // ld c, a

// ; read at least 1 byte
	INC_BC;  // inc bc
	goto command;  // jr .command


l_short:
	SET_PC(0x0B20U);
	PUSH_AF;  // push af

	LD_A_hli;  // ld a, [hli]
	AND_A(LZ_LEN);  // and LZ_LEN
	LD_C_A;  // ld c, a
	LD_B(0);  // ld b, 0

// ; read at least 1 byte
	INC_C;  // inc c


command:
	SET_PC(0x0B28U);
// ; Increment loop counts.
// ; We bail the moment they hit 0.
	INC_B;  // inc b
	INC_C;  // inc c

	POP_AF;  // pop af

	BIT_A(LZ_RW);  // bit LZ_RW, a
	IF_NZ goto rewrite;  // jr nz, .rewrite

	CP_A(LZ_ITERATE);  // cp LZ_ITERATE
	IF_Z goto Iter;  // jr z, .Iter
	CP_A(LZ_ALTERNATE);  // cp LZ_ALTERNATE
	IF_Z goto Alt;  // jr z, .Alt
	CP_A(LZ_ZERO);  // cp LZ_ZERO
	IF_Z goto Zero;  // jr z, .Zero

//  Literal
//  Read literal data for bc bytes.

lloop:
	SET_PC(0x0B3BU);
	DEC_C;  // dec c
	IF_NZ goto lnext;  // jr nz, .lnext
	DEC_B;  // dec b
	JP_Z (mDecompress_Main);  // jp z, .Main


lnext:
	SET_PC(0x0B42U);
	LD_A_hli;  // ld a, [hli]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
	goto lloop;  // jr .lloop


Iter:
	SET_PC(0x0B47U);
//  Write the same byte for bc bytes.
	LD_A_hli;  // ld a, [hli]


iloop:
	SET_PC(0x0B48U);
	DEC_C;  // dec c
	IF_NZ goto inext;  // jr nz, .inext
	DEC_B;  // dec b
	JP_Z (mDecompress_Main);  // jp z, .Main


inext:
	SET_PC(0x0B4FU);
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
	goto iloop;  // jr .iloop


Alt:
	SET_PC(0x0B53U);
//  Alternate two bytes for bc bytes.
	DEC_C;  // dec c
	IF_NZ goto anext1;  // jr nz, .anext1
	DEC_B;  // dec b
	JP_Z (mDecompress_adone1);  // jp z, .adone1

anext1:
	SET_PC(0x0B5AU);
	LD_A_hli;  // ld a, [hli]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de

	DEC_C;  // dec c
	IF_NZ goto anext2;  // jr nz, .anext2
	DEC_B;  // dec b
	JP_Z (mDecompress_adone2);  // jp z, .adone2

anext2:
	SET_PC(0x0B64U);
	LD_A_hld;  // ld a, [hld]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de

	goto Alt;  // jr .Alt

// ; Skip past the bytes we were alternating.

adone1:
	SET_PC(0x0B69U);
	INC_HL;  // inc hl

adone2:
	SET_PC(0x0B6AU);
	INC_HL;  // inc hl
	goto Main;  // jr .Main


Zero:
	SET_PC(0x0B6DU);
//  Write 0 for bc bytes.
	XOR_A_A;  // xor a


zloop:
	SET_PC(0x0B6EU);
	DEC_C;  // dec c
	IF_NZ goto znext;  // jr nz, .znext
	DEC_B;  // dec b
	JP_Z (mDecompress_Main);  // jp z, .Main


znext:
	SET_PC(0x0B75U);
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
	goto zloop;  // jr .zloop


rewrite:
	SET_PC(0x0B79U);
//  Repeat decompressed data from output.
	PUSH_HL;  // push hl
	PUSH_AF;  // push af

	LD_A_hli;  // ld a, [hli]
	BIT_A(7);  // bit 7, a ; sign
	IF_Z goto positive;  // jr z, .positive

//  negative
// ; hl = de + -a
	AND_A(0b01111111);  // and %01111111
	CPL;  // cpl
	ADD_A_E;  // add e
	LD_L_A;  // ld l, a
	LD_A(-1);  // ld a, -1
	ADC_A_D;  // adc d
	LD_H_A;  // ld h, a
	goto ok;  // jr .ok


positive:
	SET_PC(0x0B8BU);
//  Positive offsets are two bytes.
	LD_L_hl;  // ld l, [hl]
	LD_H_A;  // ld h, a
// ; add to starting output address
	LD_A_addr(wLZAddress);  // ld a, [wLZAddress]
	ADD_A_L;  // add l
	LD_L_A;  // ld l, a
	LD_A_addr(wLZAddress + 1);  // ld a, [wLZAddress + 1]
	ADC_A_H;  // adc h
	LD_H_A;  // ld h, a


ok:
	SET_PC(0x0B97U);
	POP_AF;  // pop af

	CP_A(LZ_REPEAT);  // cp LZ_REPEAT
	IF_Z goto Repeat;  // jr z, .Repeat
	CP_A(LZ_FLIP);  // cp LZ_FLIP
	IF_Z goto Flip;  // jr z, .Flip
	CP_A(LZ_REVERSE);  // cp LZ_REVERSE
	IF_Z goto Reverse;  // jr z, .Reverse

//  Since LZ_LONG is command 7,
//  only commands 0-6 are passed in.
//  This leaves room for an extra command 7.
//  However, lengths longer than 768
//  would be interpreted as LZ_END.

//  More practically, LZ_LONG is not recursive.
//  For now, it defaults to LZ_REPEAT.


Repeat:
	SET_PC(0x0BA4U);
//  Copy decompressed data for bc bytes.
	DEC_C;  // dec c
	IF_NZ goto rnext;  // jr nz, .rnext
	DEC_B;  // dec b
	IF_Z goto donerw;  // jr z, .donerw


rnext:
	SET_PC(0x0BAAU);
	LD_A_hli;  // ld a, [hli]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
	goto Repeat;  // jr .Repeat


Flip:
	SET_PC(0x0BAFU);
//  Copy bitflipped decompressed data for bc bytes.
	DEC_C;  // dec c
	IF_NZ goto fnext;  // jr nz, .fnext
	DEC_B;  // dec b
	JP_Z (mDecompress_donerw);  // jp z, .donerw


fnext:
	SET_PC(0x0BB6U);
	LD_A_hli;  // ld a, [hli]
	PUSH_BC;  // push bc
	LD_BC((0 << 8) | 8);  // lb bc, 0, 8


floop:
	SET_PC(0x0BBBU);
	RRA;  // rra
	RL_B;  // rl b
	DEC_C;  // dec c
	IF_NZ goto floop;  // jr nz, .floop

	LD_A_B;  // ld a, b
	POP_BC;  // pop bc

	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
	goto Flip;  // jr .Flip


Reverse:
	SET_PC(0x0BC7U);
//  Copy reversed decompressed data for bc bytes.
	DEC_C;  // dec c
	IF_NZ goto rvnext;  // jr nz, .rvnext

	DEC_B;  // dec b
	JP_Z (mDecompress_donerw);  // jp z, .donerw


rvnext:
	SET_PC(0x0BCEU);
	LD_A_hld;  // ld a, [hld]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
	goto Reverse;  // jr .Reverse


donerw:
	SET_PC(0x0BD3U);
	POP_HL;  // pop hl

	BIT_hl(7);  // bit 7, [hl]
	IF_NZ goto next;  // jr nz, .next
	INC_HL;  // inc hl ; positive offset is two bytes

next:
	SET_PC(0x0BD9U);
	INC_HL;  // inc hl
	JP(mDecompress_Main);  // jp .Main

}
