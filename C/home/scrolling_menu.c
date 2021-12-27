#include "../constants.h"

int ScrollingMenu(){
	CALL(mCopyMenuData);  // call CopyMenuData
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af

	LD_A(BANK(a_ScrollingMenu));  // ld a, BANK(_ScrollingMenu) ; aka BANK(_InitScrollingMenu)
	RST(mBankswitch);  // rst Bankswitch

	CALL(m_InitScrollingMenu);  // call _InitScrollingMenu
	CALL(mScrollingMenu_UpdatePalettes);  // call .UpdatePalettes
	CALL(m_ScrollingMenu);  // call _ScrollingMenu

	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch

	LD_A_addr(wMenuJoypad);  // ld a, [wMenuJoypad]
	RET;  // ret


_UpdatePalettes:
	SET_PC(0x3765U);
	LD_HL(wVramState);  // ld hl, wVramState
	BIT_hl(0);  // bit 0, [hl]
	JP_NZ (mUpdateTimePals);  // jp nz, UpdateTimePals
	JP(mSetPalettes);  // jp SetPalettes

}

int InitScrollingMenu(){
	LD_A_addr(wMenuBorderTopCoord);  // ld a, [wMenuBorderTopCoord]
	DEC_A;  // dec a
	LD_B_A;  // ld b, a
	LD_A_addr(wMenuBorderBottomCoord);  // ld a, [wMenuBorderBottomCoord]
	SUB_A_B;  // sub b
	LD_D_A;  // ld d, a
	LD_A_addr(wMenuBorderLeftCoord);  // ld a, [wMenuBorderLeftCoord]
	DEC_A;  // dec a
	LD_C_A;  // ld c, a
	LD_A_addr(wMenuBorderRightCoord);  // ld a, [wMenuBorderRightCoord]
	SUB_A_C;  // sub c
	LD_E_A;  // ld e, a
	PUSH_DE;  // push de
	CALL(mCoord2Tile);  // call Coord2Tile
	POP_BC;  // pop bc
	JP(mTextbox);  // jp Textbox

}

int JoyTextDelay_ForcehJoyDown(){
	CALL(mDelayFrame);  // call DelayFrame

	LDH_A_addr(hInMenu);  // ldh a, [hInMenu]
	PUSH_AF;  // push af
	LD_A(0x1);  // ld a, $1
	LDH_addr_A(hInMenu);  // ldh [hInMenu], a
	CALL(mJoyTextDelay);  // call JoyTextDelay
	POP_AF;  // pop af
	LDH_addr_A(hInMenu);  // ldh [hInMenu], a

	LDH_A_addr(hJoyLast);  // ldh a, [hJoyLast]
	AND_A(D_RIGHT + D_LEFT + D_UP + D_DOWN);  // and D_RIGHT + D_LEFT + D_UP + D_DOWN
	LD_C_A;  // ld c, a
	LDH_A_addr(hJoyPressed);  // ldh a, [hJoyPressed]
	AND_A(A_BUTTON + B_BUTTON + SELECT + START);  // and A_BUTTON + B_BUTTON + SELECT + START
	OR_A_C;  // or c
	LD_C_A;  // ld c, a
	RET;  // ret

}
