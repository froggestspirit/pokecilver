#include "../constants.h"

int Load2DMenuData(){
	SET_PC(0x1A4CU);
	PUSH_HL;  // push hl
	PUSH_BC;  // push bc
	LD_HL(w2DMenuData);  // ld hl, w2DMenuData
	LD_B(w2DMenuDataEnd - w2DMenuData);  // ld b, w2DMenuDataEnd - w2DMenuData

loop:
	SET_PC(0x1A53U);
	LD_A_de;  // ld a, [de]
	INC_DE;  // inc de
	LD_hli_A;  // ld [hli], a
	DEC_B;  // dec b
	IF_NZ goto loop;  // jr nz, .loop

// ; Reset menu state
	LD_A(0x1);  // ld a, $1
	LD_hli_A;  // ld [hli], a ; wMenuCursorY
	LD_hli_A;  // ld [hli], a ; wMenuCursorX
	XOR_A_A;  // xor a
	LD_hli_A;  // ld [hli], a ; wCursorOffCharacter
	LD_hli_A;  // ld [hli], a ; wCursorCurrentTile
	LD_hli_A;  // ld [hli], a
	POP_BC;  // pop bc
	POP_HL;  // pop hl
	RET;  // ret

}

int StaticMenuJoypad(){
	SET_PC(0x1A64U);
	CALLFAR(av_StaticMenuJoypad);  // callfar _StaticMenuJoypad
	CALL(mGetMenuJoypad);  // call GetMenuJoypad
	RET;  // ret

}

int ScrollingMenuJoypad(){
	SET_PC(0x1A6EU);
	CALLFAR(av_ScrollingMenuJoypad);  // callfar _ScrollingMenuJoypad
	CALL(mGetMenuJoypad);  // call GetMenuJoypad
	RET;  // ret

}

int GetMenuJoypad(){
	SET_PC(0x1A78U);
	PUSH_BC;  // push bc
	PUSH_AF;  // push af
	LDH_A_addr(hJoyLast);  // ldh a, [hJoyLast]
	AND_A(D_PAD);  // and D_PAD
	LD_B_A;  // ld b, a
	LDH_A_addr(hJoyPressed);  // ldh a, [hJoyPressed]
	AND_A(BUTTONS);  // and BUTTONS
	OR_A_B;  // or b
	LD_B_A;  // ld b, a
	POP_AF;  // pop af
	LD_A_B;  // ld a, b
	POP_BC;  // pop bc
	RET;  // ret

}

int PlaceHollowCursor(){
	SET_PC(0x1A89U);
	LD_HL(wCursorCurrentTile);  // ld hl, wCursorCurrentTile
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	LD_hl(0xec);  // ld [hl], "▷"
	RET;  // ret

}

int HideCursor(){
	SET_PC(0x1A92U);
	LD_HL(wCursorCurrentTile);  // ld hl, wCursorCurrentTile
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	LD_hl(0x7f);  // ld [hl], " "
	RET;  // ret

}

int PushWindow(){
	SET_PC(0x1A9BU);
	CALLFAR(av_PushWindow);  // callfar _PushWindow
	RET;  // ret

}

int ExitMenu(){
	SET_PC(0x1AA2U);
	PUSH_AF;  // push af
	CALLFAR(av_ExitMenu);  // callfar _ExitMenu
	POP_AF;  // pop af
	RET;  // ret

}

int InitVerticalMenuCursor(){
	SET_PC(0x1AABU);
	CALLFAR(av_InitVerticalMenuCursor);  // callfar _InitVerticalMenuCursor
	RET;  // ret

}

int CloseWindow(){
	SET_PC(0x1AB2U);
	PUSH_AF;  // push af
	CALL(mExitMenu);  // call ExitMenu
	CALL(mApplyTilemap);  // call ApplyTilemap
	CALL(mUpdateSprites);  // call UpdateSprites
	POP_AF;  // pop af
	RET;  // ret

}

int RestoreTileBackup(){
	SET_PC(0x1ABEU);
	CALL(mMenuBoxCoord2Tile);  // call MenuBoxCoord2Tile
	CALL(mGetMenuBoxDims);  // call GetMenuBoxDims
	INC_B;  // inc b
	INC_C;  // inc c


row:
	SET_PC(0x1AC6U);
	PUSH_BC;  // push bc
	PUSH_HL;  // push hl


col:
	SET_PC(0x1AC8U);
	LD_A_de;  // ld a, [de]
	LD_hli_A;  // ld [hli], a
	DEC_DE;  // dec de
	DEC_C;  // dec c
	IF_NZ goto col;  // jr nz, .col

	POP_HL;  // pop hl
	LD_BC(SCREEN_WIDTH);  // ld bc, SCREEN_WIDTH
	ADD_HL_BC;  // add hl, bc
	POP_BC;  // pop bc
	DEC_B;  // dec b
	IF_NZ goto row;  // jr nz, .row

	RET;  // ret

}

int PopWindow(){
	SET_PC(0x1AD8U);
	LD_B(wMenuHeaderEnd - wMenuHeader);  // ld b, wMenuHeaderEnd - wMenuHeader
	LD_DE(wMenuHeader);  // ld de, wMenuHeader

loop:
	SET_PC(0x1ADDU);
	LD_A_hld;  // ld a, [hld]
	LD_de_A;  // ld [de], a
	INC_DE;  // inc de
	DEC_B;  // dec b
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

int GetMenuBoxDims(){
	SET_PC(0x1AE4U);
	LD_A_addr(wMenuBorderTopCoord);  // ld a, [wMenuBorderTopCoord] ; top
	LD_B_A;  // ld b, a
	LD_A_addr(wMenuBorderBottomCoord);  // ld a, [wMenuBorderBottomCoord] ; bottom
	SUB_A_B;  // sub b
	LD_B_A;  // ld b, a
	LD_A_addr(wMenuBorderLeftCoord);  // ld a, [wMenuBorderLeftCoord] ; left
	LD_C_A;  // ld c, a
	LD_A_addr(wMenuBorderRightCoord);  // ld a, [wMenuBorderRightCoord] ; right
	SUB_A_C;  // sub c
	LD_C_A;  // ld c, a
	RET;  // ret

}

int CopyMenuData(){
	SET_PC(0x1AF7U);
	PUSH_HL;  // push hl
	PUSH_DE;  // push de
	PUSH_BC;  // push bc
	PUSH_AF;  // push af
	LD_HL(wMenuDataPointer);  // ld hl, wMenuDataPointer
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	LD_DE(wMenuData);  // ld de, wMenuData
	LD_BC(wMenuDataEnd - wMenuData);  // ld bc, wMenuDataEnd - wMenuData
	CALL(mCopyBytes);  // call CopyBytes
	POP_AF;  // pop af
	POP_BC;  // pop bc
	POP_DE;  // pop de
	POP_HL;  // pop hl
	RET;  // ret

}

int GetWindowStackTop(){
	SET_PC(0x1B0FU);
	LD_HL(wWindowStackPointer);  // ld hl, wWindowStackPointer
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	INC_HL;  // inc hl
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	RET;  // ret

}

int PlaceVerticalMenuItems(){
	SET_PC(0x1B1AU);
	CALL(mCopyMenuData);  // call CopyMenuData
	LD_HL(wMenuDataPointer);  // ld hl, wMenuDataPointer
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]
	CALL(mGetMenuTextStartCoord);  // call GetMenuTextStartCoord
	CALL(mCoord2Tile);  // call Coord2Tile ; hl now contains the tilemap address where we will start printing text.
	INC_DE;  // inc de
	LD_A_de;  // ld a, [de] ; Number of items
	INC_DE;  // inc de
	LD_B_A;  // ld b, a

loop:
	SET_PC(0x1B2DU);
	PUSH_BC;  // push bc
	CALL(mPlaceString);  // call PlaceString
	INC_DE;  // inc de
	LD_BC(2 * SCREEN_WIDTH);  // ld bc, 2 * SCREEN_WIDTH
	ADD_HL_BC;  // add hl, bc
	POP_BC;  // pop bc
	DEC_B;  // dec b
	IF_NZ goto loop;  // jr nz, .loop

	LD_A_addr(wMenuDataFlags);  // ld a, [wMenuDataFlags]
	BIT_A(4);  // bit 4, a
	RET_Z ;  // ret z

	CALL(mMenuBoxCoord2Tile);  // call MenuBoxCoord2Tile
	LD_A_de;  // ld a, [de]
	LD_C_A;  // ld c, a
	INC_DE;  // inc de
	LD_B(0);  // ld b, 0
	ADD_HL_BC;  // add hl, bc
	JP(mPlaceString);  // jp PlaceString

}

int MenuBox(){
	SET_PC(0x1B4CU);
	CALL(mMenuBoxCoord2Tile);  // call MenuBoxCoord2Tile
	CALL(mGetMenuBoxDims);  // call GetMenuBoxDims
	DEC_B;  // dec b
	DEC_C;  // dec c
	JP(mTextbox);  // jp Textbox

}

int GetMenuTextStartCoord(){
	SET_PC(0x1B57U);
	LD_A_addr(wMenuBorderTopCoord);  // ld a, [wMenuBorderTopCoord]
	LD_B_A;  // ld b, a
	INC_B;  // inc b
	LD_A_addr(wMenuBorderLeftCoord);  // ld a, [wMenuBorderLeftCoord]
	LD_C_A;  // ld c, a
	INC_C;  // inc c
//  bit 6: if not set, leave extra room on top
	LD_A_addr(wMenuDataFlags);  // ld a, [wMenuDataFlags]
	BIT_A(6);  // bit 6, a
	IF_NZ goto bit_6_set;  // jr nz, .bit_6_set
	INC_B;  // inc b


bit_6_set:
	SET_PC(0x1B69U);
//  bit 7: if set, leave extra room on the left
	LD_A_addr(wMenuDataFlags);  // ld a, [wMenuDataFlags]
	BIT_A(7);  // bit 7, a
	IF_Z goto bit_7_clear;  // jr z, .bit_7_clear
	INC_C;  // inc c


bit_7_clear:
	SET_PC(0x1B71U);
	RET;  // ret

}

int ClearMenuBoxInterior(){
	SET_PC(0x1B72U);
	CALL(mMenuBoxCoord2Tile);  // call MenuBoxCoord2Tile
	LD_BC(SCREEN_WIDTH + 1);  // ld bc, SCREEN_WIDTH + 1
	ADD_HL_BC;  // add hl, bc
	CALL(mGetMenuBoxDims);  // call GetMenuBoxDims
	DEC_B;  // dec b
	DEC_C;  // dec c
	CALL(mClearBox);  // call ClearBox
	RET;  // ret

}

int ClearWholeMenuBox(){
	SET_PC(0x1B82U);
	CALL(mMenuBoxCoord2Tile);  // call MenuBoxCoord2Tile
	CALL(mGetMenuBoxDims);  // call GetMenuBoxDims
	INC_C;  // inc c
	INC_B;  // inc b
	CALL(mClearBox);  // call ClearBox
	RET;  // ret

}

int MenuBoxCoord2Tile(){
	SET_PC(0x1B8EU);
	LD_A_addr(wMenuBorderLeftCoord);  // ld a, [wMenuBorderLeftCoord]
	LD_C_A;  // ld c, a
	LD_A_addr(wMenuBorderTopCoord);  // ld a, [wMenuBorderTopCoord]
	LD_B_A;  // ld b, a
// ; fallthrough

	return mCoord2Tile;
}

int Coord2Tile(){
	SET_PC(0x1B96U);
//  Return the address of wTilemap(c, b) in hl.
	XOR_A_A;  // xor a
	LD_H_A;  // ld h, a
	LD_L_B;  // ld l, b
	LD_A_C;  // ld a, c
	LD_B_H;  // ld b, h
	LD_C_L;  // ld c, l
	ADD_HL_HL;  // add hl, hl
	ADD_HL_HL;  // add hl, hl
	ADD_HL_BC;  // add hl, bc
	ADD_HL_HL;  // add hl, hl
	ADD_HL_HL;  // add hl, hl
	LD_C_A;  // ld c, a
	XOR_A_A;  // xor a
	LD_B_A;  // ld b, a
	ADD_HL_BC;  // add hl, bc
	bccoord(0, 0, wTilemap);  // bccoord 0, 0
	ADD_HL_BC;  // add hl, bc
	RET;  // ret

}

int LoadMenuHeader(){
	SET_PC(0x1BAAU);
	CALL(mCopyMenuHeader);  // call CopyMenuHeader
	CALL(mPushWindow);  // call PushWindow
	RET;  // ret

}

int CopyMenuHeader(){
	SET_PC(0x1BB1U);
	LD_DE(wMenuHeader);  // ld de, wMenuHeader
	LD_BC(wMenuHeaderEnd - wMenuHeader);  // ld bc, wMenuHeaderEnd - wMenuHeader
	CALL(mCopyBytes);  // call CopyBytes
	RET;  // ret

}

int StoreMenuCursorPosition(){
	SET_PC(0x1BBBU);
	LD_addr_A(wMenuCursorPosition);  // ld [wMenuCursorPosition], a
	RET;  // ret

}

int MenuTextbox(){
	SET_PC(0x1BBFU);
	PUSH_HL;  // push hl
	CALL(mLoadMenuTextbox);  // call LoadMenuTextbox
	POP_HL;  // pop hl
	JP(mPrintText);  // jp PrintText

}

int Menu_DummyFunction(){
	SET_PC(0x1BC7U);
//  //  unreferenced
	RET;  // ret

}

int LoadMenuTextbox(){
	SET_PC(0x1BC8U);
	LD_HL(mLoadMenuTextbox_MenuHeader);  // ld hl, .MenuHeader
	CALL(mLoadMenuHeader);  // call LoadMenuHeader
	RET;  // ret


MenuHeader:
	SET_PC(0x1BCFU);
	//db ['MENU_BACKUP_TILES'];  // db MENU_BACKUP_TILES ; flags
	//menu_coords ['0', '12', 'SCREEN_WIDTH - 1', 'SCREEN_HEIGHT - 1'];  // menu_coords 0, 12, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1
	//dw ['vTiles0'];  // dw vTiles0
	//db ['0'];  // db 0 ; default option

	return mMenuTextboxBackup;
}

int MenuTextboxBackup(){
	SET_PC(0x1BD7U);
	CALL(mMenuTextbox);  // call MenuTextbox
	CALL(mCloseWindow);  // call CloseWindow
	RET;  // ret

}

int LoadStandardMenuHeader(){
	SET_PC(0x1BDEU);
	LD_HL(mLoadStandardMenuHeader_MenuHeader);  // ld hl, .MenuHeader
	CALL(mLoadMenuHeader);  // call LoadMenuHeader
	RET;  // ret


MenuHeader:
	SET_PC(0x1BE5U);
	//db ['MENU_BACKUP_TILES'];  // db MENU_BACKUP_TILES ; flags
	//menu_coords ['0', '0', 'SCREEN_WIDTH - 1', 'SCREEN_HEIGHT - 1'];  // menu_coords 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1
	//dw ['0'];  // dw 0
	//db ['1'];  // db 1 ; default option

	return mCall_ExitMenu;
}

int Call_ExitMenu(){
	SET_PC(0x1BEDU);
	CALL(mExitMenu);  // call ExitMenu
	RET;  // ret

}

int VerticalMenu(){
	SET_PC(0x1BF1U);
	XOR_A_A;  // xor a
	LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a
	CALL(mMenuBox);  // call MenuBox
	CALL(mUpdateSprites);  // call UpdateSprites
	CALL(mPlaceVerticalMenuItems);  // call PlaceVerticalMenuItems
	CALL(mApplyTilemap);  // call ApplyTilemap
	CALL(mCopyMenuData);  // call CopyMenuData
	LD_A_addr(wMenuDataFlags);  // ld a, [wMenuDataFlags]
	BIT_A(7);  // bit 7, a
	IF_Z goto cancel;  // jr z, .cancel
	CALL(mInitVerticalMenuCursor);  // call InitVerticalMenuCursor
	CALL(mStaticMenuJoypad);  // call StaticMenuJoypad
	CALL(mMenuClickSound);  // call MenuClickSound
	BIT_A(1);  // bit 1, a
	IF_Z goto okay;  // jr z, .okay

cancel:
	SET_PC(0x1C17U);
	SCF;  // scf
	RET;  // ret


okay:
	SET_PC(0x1C19U);
	AND_A_A;  // and a
	RET;  // ret

}

int GetMenu2(){
	SET_PC(0x1C1BU);
	CALL(mLoadMenuHeader);  // call LoadMenuHeader
	CALL(mVerticalMenu);  // call VerticalMenu
	CALL(mCloseWindow);  // call CloseWindow
	LD_A_addr(wMenuCursorY);  // ld a, [wMenuCursorY]
	RET;  // ret

}

int CopyNameFromMenu(){
	SET_PC(0x1C28U);
	PUSH_HL;  // push hl
	PUSH_BC;  // push bc
	PUSH_AF;  // push af
	LD_HL(wMenuDataPointer);  // ld hl, wMenuDataPointer
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	INC_HL;  // inc hl
	INC_HL;  // inc hl
	POP_AF;  // pop af
	CALL(mGetNthString);  // call GetNthString
	LD_D_H;  // ld d, h
	LD_E_L;  // ld e, l
	CALL(mCopyName1);  // call CopyName1
	POP_BC;  // pop bc
	POP_HL;  // pop hl
	RET;  // ret

}

int YesNoBox(){
	SET_PC(0x1C3FU);
	LD_BC((SCREEN_WIDTH - 6 << 8) | 7);  // lb bc, SCREEN_WIDTH - 6, 7

	return mPlaceYesNoBox;
}

int PlaceYesNoBox(){
	SET_PC(0x1C42U);
	JR(mv_YesNoBox);  // jr _YesNoBox

}

int PlaceGenericTwoOptionBox(){
	SET_PC(0x1C44U);
//  //  unreferenced
	CALL(mLoadMenuHeader);  // call LoadMenuHeader
	JR(mInterpretTwoOptionMenu);  // jr InterpretTwoOptionMenu

}

int v_YesNoBox(){
	SET_PC(0x1C49U);
//  Return nc (yes) or c (no).
	PUSH_BC;  // push bc
	LD_HL(mYesNoMenuHeader);  // ld hl, YesNoMenuHeader
	CALL(mCopyMenuHeader);  // call CopyMenuHeader
	POP_BC;  // pop bc

	LD_A_B;  // ld a, b
	LD_addr_A(wMenuBorderLeftCoord);  // ld [wMenuBorderLeftCoord], a
	ADD_A(5);  // add 5
	LD_addr_A(wMenuBorderRightCoord);  // ld [wMenuBorderRightCoord], a
	LD_A_C;  // ld a, c
	LD_addr_A(wMenuBorderTopCoord);  // ld [wMenuBorderTopCoord], a
	ADD_A(4);  // add 4
	LD_addr_A(wMenuBorderBottomCoord);  // ld [wMenuBorderBottomCoord], a
	CALL(mPushWindow);  // call PushWindow

	return mInterpretTwoOptionMenu;
}

int InterpretTwoOptionMenu(){
	SET_PC(0x1C66U);
	CALL(mVerticalMenu);  // call VerticalMenu
	PUSH_AF;  // push af
	LD_C(0xf);  // ld c, $f
	CALL(mDelayFrames);  // call DelayFrames
	CALL(mCloseWindow);  // call CloseWindow
	POP_AF;  // pop af
	IF_C goto no;  // jr c, .no
	LD_A_addr(wMenuCursorY);  // ld a, [wMenuCursorY]
	CP_A(2);  // cp 2 ; no
	IF_Z goto no;  // jr z, .no
	AND_A_A;  // and a
	RET;  // ret


no:
	SET_PC(0x1C7EU);
	LD_A(2);  // ld a, 2
	LD_addr_A(wMenuCursorY);  // ld [wMenuCursorY], a
	SCF;  // scf
	RET;  // ret

}

int YesNoMenuHeader(){
	SET_PC(0x1C85U);
	//db ['MENU_BACKUP_TILES'];  // db MENU_BACKUP_TILES ; flags
	//menu_coords ['10', '5', '15', '9'];  // menu_coords 10, 5, 15, 9
	//dw ['.MenuData'];  // dw .MenuData
	//db ['1'];  // db 1 ; default option


MenuData:
	SET_PC(0x1C8DU);
	//db ['STATICMENU_CURSOR | STATICMENU_NO_TOP_SPACING'];  // db STATICMENU_CURSOR | STATICMENU_NO_TOP_SPACING ; flags
	//db ['2'];  // db 2
	//db ['"YES@"'];  // db "YES@"
	//db ['"NO@"'];  // db "NO@"

	return mOffsetMenuHeader;
}

int OffsetMenuHeader(){
	SET_PC(0x1C96U);
	CALL(mv_OffsetMenuHeader);  // call _OffsetMenuHeader
	CALL(mPushWindow);  // call PushWindow
	RET;  // ret

}

int v_OffsetMenuHeader(){
	SET_PC(0x1C9DU);
	PUSH_DE;  // push de
	CALL(mCopyMenuHeader);  // call CopyMenuHeader
	POP_DE;  // pop de
	LD_A_addr(wMenuBorderLeftCoord);  // ld a, [wMenuBorderLeftCoord]
	LD_H_A;  // ld h, a
	LD_A_addr(wMenuBorderRightCoord);  // ld a, [wMenuBorderRightCoord]
	SUB_A_H;  // sub h
	LD_H_A;  // ld h, a
	LD_A_D;  // ld a, d
	LD_addr_A(wMenuBorderLeftCoord);  // ld [wMenuBorderLeftCoord], a
	ADD_A_H;  // add h
	LD_addr_A(wMenuBorderRightCoord);  // ld [wMenuBorderRightCoord], a
	LD_A_addr(wMenuBorderTopCoord);  // ld a, [wMenuBorderTopCoord]
	LD_L_A;  // ld l, a
	LD_A_addr(wMenuBorderBottomCoord);  // ld a, [wMenuBorderBottomCoord]
	SUB_A_L;  // sub l
	LD_L_A;  // ld l, a
	LD_A_E;  // ld a, e
	LD_addr_A(wMenuBorderTopCoord);  // ld [wMenuBorderTopCoord], a
	ADD_A_L;  // add l
	LD_addr_A(wMenuBorderBottomCoord);  // ld [wMenuBorderBottomCoord], a
	RET;  // ret

}

int DoNthMenu(){
	SET_PC(0x1CC5U);
	CALL(mDrawVariableLengthMenuBox);  // call DrawVariableLengthMenuBox
	CALL(mMenuWriteText);  // call MenuWriteText
	CALL(mInitMenuCursorAndButtonPermissions);  // call InitMenuCursorAndButtonPermissions
	CALL(mGetStaticMenuJoypad);  // call GetStaticMenuJoypad
	CALL(mGetMenuJoypad);  // call GetMenuJoypad
	CALL(mMenuClickSound);  // call MenuClickSound
	RET;  // ret

}

int SetUpMenu(){
	SET_PC(0x1CD8U);
	CALL(mDrawVariableLengthMenuBox);  // call DrawVariableLengthMenuBox
	CALL(mMenuWriteText);  // call MenuWriteText
	CALL(mInitMenuCursorAndButtonPermissions);  // call InitMenuCursorAndButtonPermissions
	LD_HL(w2DMenuFlags1);  // ld hl, w2DMenuFlags1
	SET_hl(7);  // set 7, [hl]
	RET;  // ret

}

int DrawVariableLengthMenuBox(){
	SET_PC(0x1CE7U);
	CALL(mCopyMenuData);  // call CopyMenuData
	CALL(mGetMenuIndexSet);  // call GetMenuIndexSet
	CALL(mAutomaticGetMenuBottomCoord);  // call AutomaticGetMenuBottomCoord
	CALL(mMenuBox);  // call MenuBox
	RET;  // ret

}

int MenuWriteText(){
	SET_PC(0x1CF4U);
	XOR_A_A;  // xor a
	LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a
	CALL(mGetMenuIndexSet);  // call GetMenuIndexSet ; sort out the text
	CALL(mRunMenuItemPrintingFunction);  // call RunMenuItemPrintingFunction ; actually write it
	CALL(mSafeUpdateSprites);  // call SafeUpdateSprites
	LDH_A_addr(hOAMUpdate);  // ldh a, [hOAMUpdate]
	PUSH_AF;  // push af
	LD_A(0x1);  // ld a, $1
	LDH_addr_A(hOAMUpdate);  // ldh [hOAMUpdate], a
	CALL(mApplyTilemap);  // call ApplyTilemap
	POP_AF;  // pop af
	LDH_addr_A(hOAMUpdate);  // ldh [hOAMUpdate], a
	RET;  // ret

}

int AutomaticGetMenuBottomCoord(){
	SET_PC(0x1D0EU);
	LD_A_addr(wMenuBorderLeftCoord);  // ld a, [wMenuBorderLeftCoord]
	LD_C_A;  // ld c, a
	LD_A_addr(wMenuBorderRightCoord);  // ld a, [wMenuBorderRightCoord]
	SUB_A_C;  // sub c
	LD_C_A;  // ld c, a
	LD_A_addr(wMenuDataItems);  // ld a, [wMenuDataItems]
	ADD_A_A;  // add a
	INC_A;  // inc a
	LD_B_A;  // ld b, a
	LD_A_addr(wMenuBorderTopCoord);  // ld a, [wMenuBorderTopCoord]
	ADD_A_B;  // add b
	LD_addr_A(wMenuBorderBottomCoord);  // ld [wMenuBorderBottomCoord], a
	RET;  // ret

}

int GetMenuIndexSet(){
	SET_PC(0x1D25U);
	LD_HL(wMenuDataIndicesPointer);  // ld hl, wMenuDataIndicesPointer
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	LD_A_addr(wWhichIndexSet);  // ld a, [wWhichIndexSet]
	AND_A_A;  // and a
	IF_Z goto skip;  // jr z, .skip
	LD_B_A;  // ld b, a
	LD_C(-1);  // ld c, -1

loop:
	SET_PC(0x1D34U);
	LD_A_hli;  // ld a, [hli]
	CP_A_C;  // cp c
	IF_NZ goto loop;  // jr nz, .loop
	DEC_B;  // dec b
	IF_NZ goto loop;  // jr nz, .loop


skip:
	SET_PC(0x1D3BU);
	LD_D_H;  // ld d, h
	LD_E_L;  // ld e, l
	LD_A_hl;  // ld a, [hl]
	LD_addr_A(wMenuDataItems);  // ld [wMenuDataItems], a
	RET;  // ret

}

int RunMenuItemPrintingFunction(){
	SET_PC(0x1D42U);
	CALL(mMenuBoxCoord2Tile);  // call MenuBoxCoord2Tile
	LD_BC(2 * SCREEN_WIDTH + 2);  // ld bc, 2 * SCREEN_WIDTH + 2
	ADD_HL_BC;  // add hl, bc

loop:
	SET_PC(0x1D49U);
	INC_DE;  // inc de
	LD_A_de;  // ld a, [de]
	CP_A(-1);  // cp -1
	RET_Z ;  // ret z
	LD_addr_A(wMenuSelection);  // ld [wMenuSelection], a
	PUSH_DE;  // push de
	PUSH_HL;  // push hl
	LD_D_H;  // ld d, h
	LD_E_L;  // ld e, l
	LD_HL(wMenuDataDisplayFunctionPointer);  // ld hl, wMenuDataDisplayFunctionPointer
	CALL(mRunMenuItemPrintingFunction__hl_);  // call ._hl_
	POP_HL;  // pop hl
	LD_DE(2 * SCREEN_WIDTH);  // ld de, 2 * SCREEN_WIDTH
	ADD_HL_DE;  // add hl, de
	POP_DE;  // pop de
	goto loop;  // jr .loop


_hl_:
	SET_PC(0x1D63U);
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	JP_hl;  // jp hl

}

int InitMenuCursorAndButtonPermissions(){
	SET_PC(0x1D67U);
	CALL(mInitVerticalMenuCursor);  // call InitVerticalMenuCursor
	LD_HL(wMenuJoypadFilter);  // ld hl, wMenuJoypadFilter
	LD_A_addr(wMenuDataFlags);  // ld a, [wMenuDataFlags]
	BIT_A(3);  // bit 3, a
	IF_Z goto disallow_select;  // jr z, .disallow_select
	SET_hl(START_F);  // set START_F, [hl]


disallow_select:
	SET_PC(0x1D76U);
	LD_A_addr(wMenuDataFlags);  // ld a, [wMenuDataFlags]
	BIT_A(2);  // bit 2, a
	IF_Z goto disallow_left_right;  // jr z, .disallow_left_right
	SET_hl(D_LEFT_F);  // set D_LEFT_F, [hl]
	SET_hl(D_RIGHT_F);  // set D_RIGHT_F, [hl]


disallow_left_right:
	SET_PC(0x1D81U);
	RET;  // ret

}

int GetScrollingMenuJoypad(){
	SET_PC(0x1D82U);
	CALL(mScrollingMenuJoypad);  // call ScrollingMenuJoypad
	LD_HL(wMenuJoypadFilter);  // ld hl, wMenuJoypadFilter
	AND_A_hl;  // and [hl]
	JR(mContinueGettingMenuJoypad);  // jr ContinueGettingMenuJoypad

}

int GetStaticMenuJoypad(){
	SET_PC(0x1D8BU);
	XOR_A_A;  // xor a
	LD_addr_A(wMenuJoypad);  // ld [wMenuJoypad], a
	CALL(mStaticMenuJoypad);  // call StaticMenuJoypad

	return mContinueGettingMenuJoypad;
}

int ContinueGettingMenuJoypad(){
	SET_PC(0x1D92U);
	BIT_A(A_BUTTON_F);  // bit A_BUTTON_F, a
	IF_NZ goto a_button;  // jr nz, .a_button
	BIT_A(B_BUTTON_F);  // bit B_BUTTON_F, a
	IF_NZ goto b_start;  // jr nz, .b_start
	BIT_A(START_F);  // bit START_F, a
	IF_NZ goto b_start;  // jr nz, .b_start
	BIT_A(D_RIGHT_F);  // bit D_RIGHT_F, a
	IF_NZ goto d_right;  // jr nz, .d_right
	BIT_A(D_LEFT_F);  // bit D_LEFT_F, a
	IF_NZ goto d_left;  // jr nz, .d_left
	XOR_A_A;  // xor a
	LD_addr_A(wMenuJoypad);  // ld [wMenuJoypad], a
	goto done;  // jr .done


d_right:
	SET_PC(0x1DACU);
	LD_A(D_RIGHT);  // ld a, D_RIGHT
	LD_addr_A(wMenuJoypad);  // ld [wMenuJoypad], a
	goto done;  // jr .done


d_left:
	SET_PC(0x1DB3U);
	LD_A(D_LEFT);  // ld a, D_LEFT
	LD_addr_A(wMenuJoypad);  // ld [wMenuJoypad], a
	goto done;  // jr .done


a_button:
	SET_PC(0x1DBAU);
	LD_A(A_BUTTON);  // ld a, A_BUTTON
	LD_addr_A(wMenuJoypad);  // ld [wMenuJoypad], a


done:
	SET_PC(0x1DBFU);
	CALL(mGetMenuIndexSet);  // call GetMenuIndexSet
	LD_A_addr(wMenuCursorY);  // ld a, [wMenuCursorY]
	LD_L_A;  // ld l, a
	LD_H(0);  // ld h, 0
	ADD_HL_DE;  // add hl, de
	LD_A_hl;  // ld a, [hl]
	LD_addr_A(wMenuSelection);  // ld [wMenuSelection], a
	LD_A_addr(wMenuCursorY);  // ld a, [wMenuCursorY]
	LD_addr_A(wMenuCursorPosition);  // ld [wMenuCursorPosition], a
	AND_A_A;  // and a
	RET;  // ret


b_start:
	SET_PC(0x1DD5U);
	LD_A(B_BUTTON);  // ld a, B_BUTTON
	LD_addr_A(wMenuJoypad);  // ld [wMenuJoypad], a
	LD_A(-1);  // ld a, -1
	LD_addr_A(wMenuSelection);  // ld [wMenuSelection], a
	SCF;  // scf
	RET;  // ret

}

int PlaceMenuStrings(){
	SET_PC(0x1DE1U);
	PUSH_DE;  // push de
	LD_HL(wMenuDataPointerTableAddr);  // ld hl, wMenuDataPointerTableAddr
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	LD_A_addr(wMenuSelection);  // ld a, [wMenuSelection]
	CALL(mGetNthString);  // call GetNthString
	LD_D_H;  // ld d, h
	LD_E_L;  // ld e, l
	POP_HL;  // pop hl
	CALL(mPlaceString);  // call PlaceString
	RET;  // ret

}

int PlaceNthMenuStrings(){
	SET_PC(0x1DF5U);
	PUSH_DE;  // push de
	LD_A_addr(wMenuSelection);  // ld a, [wMenuSelection]
	CALL(mGetMenuDataPointerTableEntry);  // call GetMenuDataPointerTableEntry
	INC_HL;  // inc hl
	INC_HL;  // inc hl
	LD_A_hli;  // ld a, [hli]
	LD_D_hl;  // ld d, [hl]
	LD_E_A;  // ld e, a
	POP_HL;  // pop hl
	CALL(mPlaceString);  // call PlaceString
	RET;  // ret

}

int GetNthMenuStrings(){
	SET_PC(0x1E06U);
//  //  unreferenced
	CALL(mGetMenuDataPointerTableEntry);  // call GetMenuDataPointerTableEntry
	INC_HL;  // inc hl
	INC_HL;  // inc hl
	LD_A_hli;  // ld a, [hli]
	LD_D_hl;  // ld d, [hl]
	LD_E_A;  // ld e, a
	RET;  // ret

}

int MenuJumptable(){
	SET_PC(0x1E0FU);
	LD_A_addr(wMenuSelection);  // ld a, [wMenuSelection]
	CALL(mGetMenuDataPointerTableEntry);  // call GetMenuDataPointerTableEntry
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	JP_hl;  // jp hl

}

int GetMenuDataPointerTableEntry(){
	SET_PC(0x1E19U);
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
	LD_HL(wMenuDataPointerTableAddr);  // ld hl, wMenuDataPointerTableAddr
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	ADD_HL_DE;  // add hl, de
	ADD_HL_DE;  // add hl, de
	ADD_HL_DE;  // add hl, de
	ADD_HL_DE;  // add hl, de
	RET;  // ret

}

int ClearWindowData(){
	SET_PC(0x1E27U);
	LD_HL(wWindowStackPointer);  // ld hl, wWindowStackPointer
	CALL(mClearWindowData_bytefill);  // call .bytefill
	LD_HL(wMenuHeader);  // ld hl, wMenuHeader
	CALL(mClearWindowData_bytefill);  // call .bytefill
	LD_HL(wMenuDataFlags);  // ld hl, wMenuDataFlags
	CALL(mClearWindowData_bytefill);  // call .bytefill
	LD_HL(w2DMenuCursorInitY);  // ld hl, w2DMenuCursorInitY
	CALL(mClearWindowData_bytefill);  // call .bytefill

	XOR_A_A;  // xor a
	CALL(mOpenSRAM);  // call OpenSRAM

	XOR_A_A;  // xor a
	LD_HL(sWindowStackTop);  // ld hl, sWindowStackTop
	LD_hld_A;  // ld [hld], a
	LD_hld_A;  // ld [hld], a
	LD_A_L;  // ld a, l
	LD_addr_A(wWindowStackPointer);  // ld [wWindowStackPointer], a
	LD_A_H;  // ld a, h
	LD_addr_A(wWindowStackPointer + 1);  // ld [wWindowStackPointer + 1], a

	CALL(mCloseSRAM);  // call CloseSRAM
	RET;  // ret


bytefill:
	SET_PC(0x1E55U);
	LD_BC(0x10);  // ld bc, $10
	XOR_A_A;  // xor a
	CALL(mByteFill);  // call ByteFill
	RET;  // ret

}

int MenuClickSound(){
	SET_PC(0x1E5DU);
	PUSH_AF;  // push af
	AND_A(A_BUTTON | B_BUTTON);  // and A_BUTTON | B_BUTTON
	IF_Z goto nosound;  // jr z, .nosound
	LD_HL(wMenuFlags);  // ld hl, wMenuFlags
	BIT_A(3);  // bit 3, a
	IF_NZ goto nosound;  // jr nz, .nosound
	CALL(mPlayClickSFX);  // call PlayClickSFX

nosound:
	SET_PC(0x1E6CU);
	POP_AF;  // pop af
	RET;  // ret

}

int PlayClickSFX(){
	SET_PC(0x1E6EU);
	PUSH_DE;  // push de
	LD_DE(SFX_READ_TEXT_2);  // ld de, SFX_READ_TEXT_2
	CALL(mPlaySFX);  // call PlaySFX
	POP_DE;  // pop de
	RET;  // ret

}

int MenuTextboxWaitButton(){
	SET_PC(0x1E77U);
	CALL(mMenuTextbox);  // call MenuTextbox
	CALL(mWaitButton);  // call WaitButton
	CALL(mExitMenu);  // call ExitMenu
	RET;  // ret

}

int Place2DMenuItemName(){
	SET_PC(0x1E81U);
	LD_addr_A(wTempBank);  // ld [wTempBank], a
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af
	LD_A_addr(wTempBank);  // ld a, [wTempBank]
	RST(mBankswitch);  // rst Bankswitch

	CALL(mPlaceString);  // call PlaceString
	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch

	RET;  // ret

}

int v_2DMenu(){
	SET_PC(0x1E91U);
	CALL(mCopyMenuData);  // call CopyMenuData
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	LD_addr_A(wMenuData_2DMenuItemStringsBank);  // ld [wMenuData_2DMenuItemStringsBank], a
	PUSH_AF;  // push af
	LD_A(BANK(av_2DMenu_));  // ld a, BANK(_2DMenu_)
	RST(mBankswitch);  // rst Bankswitch
	CALL(mv_2DMenu_);  // call _2DMenu_
	POP_BC;  // pop bc
	LD_A_B;  // ld a, b
	RST(mBankswitch);  // rst Bankswitch
	LD_A_addr(wMenuCursorPosition);  // ld a, [wMenuCursorPosition]
	RET;  // ret

}

int ResetBGWindow(){
	SET_PC(0x1EA7U);
	XOR_A_A;  // xor a
	LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a
	LD_A(0x90);  // ld a, $90
	LDH_addr_A(rWY);  // ldh [rWY], a
	LDH_addr_A(hWY);  // ldh [hWY], a
	RET;  // ret

}