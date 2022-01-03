#include "../constants.h"

int ClearBox(){
	SET_PC(0x0EBBU);
//  Fill a c*b box at hl with blank tiles.
	LD_A(0x7f);  // ld a, " "
	LD_DE(SCREEN_WIDTH);  // ld de, SCREEN_WIDTH

row:
	SET_PC(0x0EC0U);
	PUSH_HL;  // push hl
	PUSH_BC;  // push bc

col:
	SET_PC(0x0EC2U);
	LD_hli_A;  // ld [hli], a
	DEC_C;  // dec c
	IF_NZ goto col;  // jr nz, .col
	POP_BC;  // pop bc
	POP_HL;  // pop hl
	ADD_HL_DE;  // add hl, de
	DEC_B;  // dec b
	IF_NZ goto row;  // jr nz, .row
	RET;  // ret

}

int ClearTilemap(){
	SET_PC(0x0ECDU);
//  Fill wTilemap with blank tiles.

	hlcoord(0, 0, wTilemap);  // hlcoord 0, 0
	LD_A(0x7f);  // ld a, " "
	LD_BC(wTilemapEnd - wTilemap);  // ld bc, wTilemapEnd - wTilemap
	CALL(mByteFill);  // call ByteFill

// ; Update the BG Map.
	LDH_A_addr(rLCDC);  // ldh a, [rLCDC]
	BIT_A(rLCDC_ENABLE);  // bit rLCDC_ENABLE, a
	RET_Z ;  // ret z
	JP(mWaitBGMap);  // jp WaitBGMap

}

int ClearScreen(){
	SET_PC(0x0EE0U);
	LD_A(PAL_BG_TEXT);  // ld a, PAL_BG_TEXT
	hlcoord(0, 0, wAttrmap);  // hlcoord 0, 0, wAttrmap
	LD_BC(SCREEN_WIDTH * SCREEN_HEIGHT);  // ld bc, SCREEN_WIDTH * SCREEN_HEIGHT
	CALL(mByteFill);  // call ByteFill
	JR(mClearTilemap);  // jr ClearTilemap

}

int Textbox(){
	SET_PC(0x0EEDU);
//  Draw a text box at hl with room for b lines of c characters each.
//  Places a border around the textbox, then switches the palette to the
//  text black-and-white scheme.
	PUSH_BC;  // push bc
	PUSH_HL;  // push hl
	CALL(mTextboxBorder);  // call TextboxBorder
	POP_HL;  // pop hl
	POP_BC;  // pop bc
	JR(mTextboxPalette);  // jr TextboxPalette

}

int TextboxBorder(){
	SET_PC(0x0EF6U);
// ; Top
	PUSH_HL;  // push hl
	LD_A(0x79);  // ld a, "┌"
	LD_hli_A;  // ld [hli], a
	INC_A;  // inc a ; "─"
	CALL(mTextboxBorder_PlaceChars);  // call .PlaceChars
	INC_A;  // inc a ; "┐"
	LD_hl_A;  // ld [hl], a
	POP_HL;  // pop hl

// ; Middle
	LD_DE(SCREEN_WIDTH);  // ld de, SCREEN_WIDTH
	ADD_HL_DE;  // add hl, de

row:
	SET_PC(0x0F05U);
	PUSH_HL;  // push hl
	LD_A(0x7c);  // ld a, "│"
	LD_hli_A;  // ld [hli], a
	LD_A(0x7f);  // ld a, " "
	CALL(mTextboxBorder_PlaceChars);  // call .PlaceChars
	LD_hl(0x7c);  // ld [hl], "│"
	POP_HL;  // pop hl

	LD_DE(SCREEN_WIDTH);  // ld de, SCREEN_WIDTH
	ADD_HL_DE;  // add hl, de
	DEC_B;  // dec b
	IF_NZ goto row;  // jr nz, .row

// ; Bottom
	LD_A(0x7d);  // ld a, "└"
	LD_hli_A;  // ld [hli], a
	LD_A(0x7a);  // ld a, "─"
	CALL(mTextboxBorder_PlaceChars);  // call .PlaceChars
	LD_hl(0x7e);  // ld [hl], "┘"

	RET;  // ret


PlaceChars:
	SET_PC(0x0F23U);
//  Place char a c times.
	LD_D_C;  // ld d, c

loop:
	SET_PC(0x0F24U);
	LD_hli_A;  // ld [hli], a
	DEC_D;  // dec d
	IF_NZ goto loop;  // jr nz, .loop
	RET;  // ret

}

int TextboxPalette(){
	SET_PC(0x0F29U);
//  Fill text box width c height b at hl with pal 7
	LD_DE(wAttrmap - wTilemap);  // ld de, wAttrmap - wTilemap
	ADD_HL_DE;  // add hl, de
	INC_B;  // inc b
	INC_B;  // inc b
	INC_C;  // inc c
	INC_C;  // inc c
	LD_A(PAL_BG_TEXT);  // ld a, PAL_BG_TEXT

col:
	SET_PC(0x0F33U);
	PUSH_BC;  // push bc
	PUSH_HL;  // push hl

row:
	SET_PC(0x0F35U);
	LD_hli_A;  // ld [hli], a
	DEC_C;  // dec c
	IF_NZ goto row;  // jr nz, .row
	POP_HL;  // pop hl
	LD_DE(SCREEN_WIDTH);  // ld de, SCREEN_WIDTH
	ADD_HL_DE;  // add hl, de
	POP_BC;  // pop bc
	DEC_B;  // dec b
	IF_NZ goto col;  // jr nz, .col
	RET;  // ret

}

int SpeechTextbox(){
	SET_PC(0x0F43U);
//  Standard textbox.
	hlcoord(TEXTBOX_X, TEXTBOX_Y, wTilemap);  // hlcoord TEXTBOX_X, TEXTBOX_Y
	LD_B(TEXTBOX_INNERH);  // ld b, TEXTBOX_INNERH
	LD_C(TEXTBOX_INNERW);  // ld c, TEXTBOX_INNERW
	JP(mTextbox);  // jp Textbox

}

int GameFreakText(){
	SET_PC(0x0F4DU);
//  //  unreferenced
	//text ['"ゲームフりーク！"']  // text "ゲームフりーク！" ; "GAMEFREAK!"
	//done ['?']  // done

	return mRadioTerminator;
}

int RadioTerminator(){
	SET_PC(0x0F57U);
	LD_HL(mRadioTerminator_stop);  // ld hl, .stop
	RET;  // ret


stop:
	SET_PC(0x0F5BU);
	//text_end ['?']  // text_end

	return mPrintText;
}

int PrintText(){
	SET_PC(0x0F5CU);
	CALL(mSetUpTextbox);  // call SetUpTextbox
// ; fallthrough

	return mPrintTextboxText;
}

int PrintTextboxText(){
	SET_PC(0x0F5FU);
	bccoord(TEXTBOX_INNERX, TEXTBOX_INNERY, wTilemap);  // bccoord TEXTBOX_INNERX, TEXTBOX_INNERY
	CALL(mPlaceHLTextAtBC);  // call PlaceHLTextAtBC
	RET;  // ret

}

int SetUpTextbox(){
	SET_PC(0x0F66U);
	PUSH_HL;  // push hl
	CALL(mSpeechTextbox);  // call SpeechTextbox
	CALL(mUpdateSprites);  // call UpdateSprites
	CALL(mApplyTilemap);  // call ApplyTilemap
	POP_HL;  // pop hl
	RET;  // ret

}

int PlaceString(){
	SET_PC(0x0F72U);
	PUSH_HL;  // push hl
// ; fallthrough

	return mPlaceNextChar;
}

int PlaceNextChar(){
	SET_PC(0x0F73U);
	LD_A_de;  // ld a, [de]
	CP_A(0x50);  // cp "@"
	JR_NZ (mCheckDict);  // jr nz, CheckDict
	LD_B_H;  // ld b, h
	LD_C_L;  // ld c, l
	POP_HL;  // pop hl
	RET;  // ret

}

int DummyChar(){
	SET_PC(0x0F7CU);
//  //  unreferenced
	POP_DE;  // pop de
// ; fallthrough

	return mNextChar;
}

int NextChar(){
	SET_PC(0x0F7DU);
	INC_DE;  // inc de
	JP(mPlaceNextChar);  // jp PlaceNextChar

}

int CheckDict(){
	SET_PC(0x0F81U);
// dict: MACRO
// assert CHARLEN(\1) == 1
// if \1 == 0
// 	and a
// else
// 	cp \1
// endc
// if ISCONST(\2)
// 	; Replace a character with another one
// 	jr nz, .not\@
// 	ld a, \2
// .not\@:
// elif STRSUB("\2", 1, 1) == "."
// 	; Locals can use a short jump
// 	jr z, \2
// else
// 	jp z, \2
// endc
// ENDM

	//dict ['"<LINE>"', 'LineChar']  // dict "<LINE>",    LineChar
	//dict ['"<NEXT>"', 'NextLineChar']  // dict "<NEXT>",    NextLineChar
	//dict ['"<NULL>"', 'NullChar']  // dict "<NULL>",    NullChar
	//dict ['"<SCROLL>"', '_ContTextNoPause']  // dict "<SCROLL>",  _ContTextNoPause
	//dict ['"<_CONT>"', '_ContText']  // dict "<_CONT>",   _ContText
	//dict ['"<PARA>"', 'Paragraph']  // dict "<PARA>",    Paragraph
	//dict ['"<MOM>"', 'PrintMomsName']  // dict "<MOM>",     PrintMomsName
	//dict ['"<PLAYER>"', 'PrintPlayerName']  // dict "<PLAYER>",  PrintPlayerName
	//dict ['"<RIVAL>"', 'PrintRivalName']  // dict "<RIVAL>",   PrintRivalName
	//dict ['"<ROUTE>"', 'PlaceJPRoute']  // dict "<ROUTE>",   PlaceJPRoute
	//dict ['"<WATASHI>"', 'PlaceWatashi']  // dict "<WATASHI>", PlaceWatashi
	//dict ['"<KOKO_WA>"', 'PlaceKokoWa']  // dict "<KOKO_WA>", PlaceKokoWa
	//dict ['"<RED>"', 'PrintRedsName']  // dict "<RED>",     PrintRedsName
	//dict ['"<GREEN>"', 'PrintGreensName']  // dict "<GREEN>",   PrintGreensName
	//dict ['"#"', 'PlacePOKe']  // dict "#",         PlacePOKe
	//dict ['"<PC>"', 'PCChar']  // dict "<PC>",      PCChar
	//dict ['"<ROCKET>"', 'RocketChar']  // dict "<ROCKET>",  RocketChar
	//dict ['"<TM>"', 'TMChar']  // dict "<TM>",      TMChar
	//dict ['"<TRAINER>"', 'TrainerChar']  // dict "<TRAINER>", TrainerChar
	//dict ['"<KOUGEKI>"', 'PlaceKougeki']  // dict "<KOUGEKI>", PlaceKougeki
	//dict ['"<LF>"', 'LineFeedChar']  // dict "<LF>",      LineFeedChar
	//dict ['"<CONT>"', 'ContText']  // dict "<CONT>",    ContText
	//dict ['"<……>"', 'SixDotsChar']  // dict "<……>",      SixDotsChar
	//dict ['"<DONE>"', 'DoneText']  // dict "<DONE>",    DoneText
	//dict ['"<PROMPT>"', 'PromptText']  // dict "<PROMPT>",  PromptText
	//dict ['"<PKMN>"', 'PlacePKMN']  // dict "<PKMN>",    PlacePKMN
	//dict ['"<POKE>"', 'PlacePOKE']  // dict "<POKE>",    PlacePOKE
	//dict ['"0b"', 'NextChar']  // dict "%",         NextChar
	//dict ['"¯"', '" "']  // dict "¯",         " "
	//dict ['"<DEXEND>"', 'PlaceDexEnd']  // dict "<DEXEND>",  PlaceDexEnd
	//dict ['"<TARGET>"', 'PlaceMoveTargetsName']  // dict "<TARGET>",  PlaceMoveTargetsName
	//dict ['"<USER>"', 'PlaceMoveUsersName']  // dict "<USER>",    PlaceMoveUsersName
	//dict ['"<ENEMY>"', 'PlaceEnemysName']  // dict "<ENEMY>",   PlaceEnemysName
	//dict ['"ﾟ"', '.diacritic']  // dict "ﾟ",         .diacritic
	CP_A(0xe5);  // cp "ﾞ"
	IF_NZ goto not_diacritic;  // jr nz, .not_diacritic


diacritic:
	SET_PC(0x102EU);
	LD_B_A;  // ld b, a
	CALL(mDiacritic);  // call Diacritic
	JP(mNextChar);  // jp NextChar


not_diacritic:
	SET_PC(0x1035U);
	CP_A(FIRST_REGULAR_TEXT_CHAR);  // cp FIRST_REGULAR_TEXT_CHAR
	IF_NC goto place;  // jr nc, .place
//  dakuten or handakuten
	CP_A(0x40);  // cp "パ"
	IF_NC goto handakuten;  // jr nc, .handakuten
//  dakuten
	CP_A(FIRST_HIRAGANA_DAKUTEN_CHAR);  // cp FIRST_HIRAGANA_DAKUTEN_CHAR
	IF_NC goto hiragana_dakuten;  // jr nc, .hiragana_dakuten
//  katakana dakuten
	ADD_A(0x85 - 0x05);  // add "カ" - "ガ"
	goto place_dakuten;  // jr .place_dakuten


hiragana_dakuten:
	SET_PC(0x1045U);
	ADD_A(0xb6 - 0x26);  // add "か" - "が"

place_dakuten:
	SET_PC(0x1047U);
	LD_B(0xe5);  // ld b, "ﾞ" ; dakuten
	CALL(mDiacritic);  // call Diacritic
	goto place;  // jr .place


handakuten:
	SET_PC(0x104EU);
	CP_A(0x44);  // cp "ぱ"
	IF_NC goto hiragana_handakuten;  // jr nc, .hiragana_handakuten
//  katakana handakuten
	ADD_A(0x99 - 0x40);  // add "ハ" - "パ"
	goto place_handakuten;  // jr .place_handakuten


hiragana_handakuten:
	SET_PC(0x1056U);
	ADD_A(0xca - 0x44);  // add "は" - "ぱ"

place_handakuten:
	SET_PC(0x1058U);
	LD_B(0xe4);  // ld b, "ﾟ" ; handakuten
	CALL(mDiacritic);  // call Diacritic


place:
	SET_PC(0x105DU);
	LD_hli_A;  // ld [hli], a
	CALL(mPrintLetterDelay);  // call PrintLetterDelay
	JP(mNextChar);  // jp NextChar
}

#define print_name(x)   do {PUSH_DE;\
                            LD_DE(x);\
                            JP(mPlaceCommandCharacter);} while(0)
							
int PrintMomsName(){
	SET_PC(0x1064U);
	print_name(wMomsName);
}

int PrintPlayerName(){
	SET_PC(0x106BU);
	print_name(wPlayerName);
}

int PrintRivalName(){
	SET_PC(0x1072U);
	print_name(wRivalName);
}

int PrintRedsName(){
	SET_PC(0x1079U);
	print_name(wRedsName);
}

int PrintGreensName(){
	SET_PC(0x1080U);
	print_name(wGreensName);
}

int TrainerChar(){
	SET_PC(0x1087U);
	print_name(mTrainerCharText);
}

int TMChar(){
	SET_PC(0x108EU);
	print_name(mTMCharText);
}

int PCChar(){
	SET_PC(0x1095U);
	print_name(mPCCharText);
}

int RocketChar(){
	SET_PC(0x109CU);
	print_name(mRocketCharText);
}

int PlacePOKe(){
	SET_PC(0x10A3U);
	print_name(mPlacePOKeText);
}

int PlaceKougeki(){
	SET_PC(0x10AAU);
	print_name(mKougekiText);
}

int SixDotsChar(){
	SET_PC(0x10B1U);
	print_name(mSixDotsCharText);
}

int PlacePKMN(){
	SET_PC(0x10B8U);
	print_name(mPlacePKMNText);
}

int PlacePOKE(){
	SET_PC(0x10BFU);
	print_name(mPlacePOKEText);
}

int PlaceJPRoute(){
	SET_PC(0x10C6U);
	print_name(mPlaceJPRouteText);
}

int PlaceWatashi(){
	SET_PC(0x10CDU);
	print_name(mPlaceWatashiText);
}

int PlaceKokoWa(){
	SET_PC(0x10D4U);
	print_name(mPlaceKokoWaText);
}

int PlaceMoveTargetsName(){
	SET_PC(0x10DBU);
	LDH_A_addr(hBattleTurn);  // ldh a, [hBattleTurn]
	XOR_A(1);  // xor 1
	JR(mPlaceBattlersName);  // jr PlaceBattlersName

}

int PlaceMoveUsersName(){
	SET_PC(0x10E1U);
	LDH_A_addr(hBattleTurn);  // ldh a, [hBattleTurn]
// ; fallthrough

	return mPlaceBattlersName;
}

int PlaceBattlersName(){
	SET_PC(0x10E3U);
	PUSH_DE;  // push de
	AND_A_A;  // and a
	IF_NZ goto enemy;  // jr nz, .enemy

	LD_DE(wBattleMonNickname);  // ld de, wBattleMonNickname
	JR(mPlaceCommandCharacter);  // jr PlaceCommandCharacter


enemy:
	SET_PC(0x10ECU);
	LD_DE(mEnemyText);  // ld de, EnemyText
	CALL(mPlaceString);  // call PlaceString
	LD_H_B;  // ld h, b
	LD_L_C;  // ld l, c
	LD_DE(wEnemyMonNickname);  // ld de, wEnemyMonNickname
	JR(mPlaceCommandCharacter);  // jr PlaceCommandCharacter

}

int PlaceEnemysName(){
	SET_PC(0x10F9U);
	PUSH_DE;  // push de

	LD_A_addr(wLinkMode);  // ld a, [wLinkMode]
	AND_A_A;  // and a
	IF_NZ goto linkbattle;  // jr nz, .linkbattle

	LD_A_addr(wTrainerClass);  // ld a, [wTrainerClass]
	CP_A(RIVAL1);  // cp RIVAL1
	IF_Z goto rival;  // jr z, .rival
	CP_A(RIVAL2);  // cp RIVAL2
	IF_Z goto rival;  // jr z, .rival

	LD_DE(wOTClassName);  // ld de, wOTClassName
	CALL(mPlaceString);  // call PlaceString
	LD_H_B;  // ld h, b
	LD_L_C;  // ld l, c
	LD_DE(mString_Space);  // ld de, String_Space
	CALL(mPlaceString);  // call PlaceString
	PUSH_BC;  // push bc
	CALLFAR(aBattle_GetTrainerName);  // callfar Battle_GetTrainerName
	POP_HL;  // pop hl
	LD_DE(wStringBuffer1);  // ld de, wStringBuffer1
	JR(mPlaceCommandCharacter);  // jr PlaceCommandCharacter


rival:
	SET_PC(0x1126U);
	LD_DE(wRivalName);  // ld de, wRivalName
	JR(mPlaceCommandCharacter);  // jr PlaceCommandCharacter


linkbattle:
	SET_PC(0x112BU);
	LD_DE(wOTClassName);  // ld de, wOTClassName
	JR(mPlaceCommandCharacter);  // jr PlaceCommandCharacter

}

int PlaceCommandCharacter(){
	SET_PC(0x1130U);
	CALL(mPlaceString);  // call PlaceString
	LD_H_B;  // ld h, b
	LD_L_C;  // ld l, c
	POP_DE;  // pop de
	JP(mNextChar);  // jp NextChar

}

int TMCharText(){
	SET_PC(0x1139U);
//     db "TM@"
	return mTrainerCharText;
}

int TrainerCharText(){
	SET_PC(0x113CU);
//db "TRAINER@"
	return mPCCharText;
}

int PCCharText(){
	SET_PC(0x1144U);
//     db "PC@"
	return mRocketCharText;
}

int RocketCharText(){
	SET_PC(0x1147U);
// db "ROCKET@"
	return mPlacePOKeText;
}

int PlacePOKeText(){
	SET_PC(0x114EU);
//  db "POKé@"
	return mKougekiText;
}

int KougekiText(){
	SET_PC(0x1153U);
//    db "こうげき@"
	return mSixDotsCharText;
}

int SixDotsCharText(){
	SET_PC(0x1158U);
//db "……@"
	return mEnemyText;
}

int EnemyText(){
	SET_PC(0x115BU);
//      db "Enemy @"
	return mPlacePKMNText;
}

int PlacePKMNText(){
	SET_PC(0x1162U);
//  db "<PK><MN>@"
	return mPlacePOKEText;
}

int PlacePOKEText(){
	SET_PC(0x1165U);
//  db "<PO><KE>@"
	return mString_Space;
}

int String_Space(){
	SET_PC(0x1168U);
//   db " @"
//  These strings have been dummied out.
	return mPlaceJPRouteText;
}

int PlaceJPRouteText(){
	SET_PC(0x116AU);
	return mPlaceWatashiText;
}

int PlaceWatashiText(){
	SET_PC(0x116AU);
	return mPlaceKokoWaText;
}

int PlaceKokoWaText(){
	SET_PC(0x116AU);
//db "@"

	return mNextLineChar;
}

int NextLineChar(){
	SET_PC(0x116BU);
	POP_HL;  // pop hl
	LD_BC(SCREEN_WIDTH * 2);  // ld bc, SCREEN_WIDTH * 2
	ADD_HL_BC;  // add hl, bc
	PUSH_HL;  // push hl
	JP(mNextChar);  // jp NextChar

}

int LineFeedChar(){
	SET_PC(0x1174U);
	POP_HL;  // pop hl
	LD_BC(SCREEN_WIDTH);  // ld bc, SCREEN_WIDTH
	ADD_HL_BC;  // add hl, bc
	PUSH_HL;  // push hl
	JP(mNextChar);  // jp NextChar

}

int LineChar(){
	SET_PC(0x117DU);
	POP_HL;  // pop hl
	hlcoord(TEXTBOX_INNERX, TEXTBOX_INNERY + 2, wTilemap);  // hlcoord TEXTBOX_INNERX, TEXTBOX_INNERY + 2
	PUSH_HL;  // push hl
	JP(mNextChar);  // jp NextChar

}

int Paragraph(){
	SET_PC(0x1185U);
	PUSH_DE;  // push de

	LD_A_addr(wLinkMode);  // ld a, [wLinkMode]
	CP_A(LINK_COLOSSEUM);  // cp LINK_COLOSSEUM
	IF_Z goto linkbattle;  // jr z, .linkbattle
	CALL(mLoadBlinkingCursor);  // call LoadBlinkingCursor


linkbattle:
	SET_PC(0x1190U);
	CALL(mText_WaitBGMap);  // call Text_WaitBGMap
	CALL(mPromptButton);  // call PromptButton
	hlcoord(TEXTBOX_INNERX, TEXTBOX_INNERY, wTilemap);  // hlcoord TEXTBOX_INNERX, TEXTBOX_INNERY
	LD_BC((TEXTBOX_INNERH - 1 << 8) | TEXTBOX_INNERW);  // lb bc, TEXTBOX_INNERH - 1, TEXTBOX_INNERW
	CALL(mClearBox);  // call ClearBox
	CALL(mUnloadBlinkingCursor);  // call UnloadBlinkingCursor
	LD_C(20);  // ld c, 20
	CALL(mDelayFrames);  // call DelayFrames
	hlcoord(TEXTBOX_INNERX, TEXTBOX_INNERY, wTilemap);  // hlcoord TEXTBOX_INNERX, TEXTBOX_INNERY
	POP_DE;  // pop de
	JP(mNextChar);  // jp NextChar

}

int v_ContText(){
	SET_PC(0x11AEU);
	LD_A_addr(wLinkMode);  // ld a, [wLinkMode]
	OR_A_A;  // or a
	IF_NZ goto communication;  // jr nz, .communication
	CALL(mLoadBlinkingCursor);  // call LoadBlinkingCursor


communication:
	SET_PC(0x11B7U);
	CALL(mText_WaitBGMap);  // call Text_WaitBGMap

	PUSH_DE;  // push de
	CALL(mPromptButton);  // call PromptButton
	POP_DE;  // pop de

	LD_A_addr(wLinkMode);  // ld a, [wLinkMode]
	OR_A_A;  // or a
	CALL_Z (mUnloadBlinkingCursor);  // call z, UnloadBlinkingCursor
// ; fallthrough

	return mv_ContTextNoPause;
}

int v_ContTextNoPause(){
	SET_PC(0x11C6U);
	PUSH_DE;  // push de
	CALL(mTextScroll);  // call TextScroll
	CALL(mTextScroll);  // call TextScroll
	hlcoord(TEXTBOX_INNERX, TEXTBOX_INNERY + 2, wTilemap);  // hlcoord TEXTBOX_INNERX, TEXTBOX_INNERY + 2
	POP_DE;  // pop de
	JP(mNextChar);  // jp NextChar

}

int ContText(){
	SET_PC(0x11D4U);
	PUSH_DE;  // push de
	LD_DE(mContText_cont);  // ld de, .cont
	LD_B_H;  // ld b, h
	LD_C_L;  // ld c, l
	CALL(mPlaceString);  // call PlaceString
	LD_H_B;  // ld h, b
	LD_L_C;  // ld l, c
	POP_DE;  // pop de
	JP(mNextChar);  // jp NextChar


cont:
// db "<_CONT>@"
	SET_PC(0x11E3U);

	return mPlaceDexEnd;
}

int PlaceDexEnd(){
	SET_PC(0x11E5U);
//  Ends a Pokédex entry in Gen 1.
//  Dex entries are now regular strings.
	LD_hl(0xe8);  // ld [hl], "."
	POP_HL;  // pop hl
	RET;  // ret

}

int PromptText(){
	SET_PC(0x11E9U);
	LD_A_addr(wLinkMode);  // ld a, [wLinkMode]
	CP_A(LINK_COLOSSEUM);  // cp LINK_COLOSSEUM
	IF_Z goto ok;  // jr z, .ok
	CALL(mLoadBlinkingCursor);  // call LoadBlinkingCursor


ok:
	SET_PC(0x11F3U);
	CALL(mText_WaitBGMap);  // call Text_WaitBGMap
	CALL(mPromptButton);  // call PromptButton
	LD_A_addr(wLinkMode);  // ld a, [wLinkMode]
	CP_A(LINK_COLOSSEUM);  // cp LINK_COLOSSEUM
	JR_Z (mDoneText);  // jr z, DoneText
	CALL(mUnloadBlinkingCursor);  // call UnloadBlinkingCursor

}

int DoneText(){
	SET_PC(0x1203U);
	POP_HL;  // pop hl
	LD_DE(mDoneText_stop);  // ld de, .stop
	DEC_DE;  // dec de
	RET;  // ret


stop:
	SET_PC(0x1209U);
	//text_end ['?']  // text_end

	return mNullChar;
}

int NullChar(){
	SET_PC(0x120AU);
	LD_B_H;  // ld b, h
	LD_C_L;  // ld c, l
	POP_HL;  // pop hl
	LD_DE(mNullChar_ErrorText);  // ld de, .ErrorText
	DEC_DE;  // dec de
	RET;  // ret


ErrorText:
	SET_PC(0x1212U);
	//text_decimal ['hObjectStructIndex', '1', '2']  // text_decimal hObjectStructIndex, 1, 2
	//text ['"エラー"']  // text "エラー"
	//done ['?']  // done

	return mTextScroll;
}

int TextScroll(){
	SET_PC(0x121BU);
	hlcoord(TEXTBOX_X, TEXTBOX_INNERY, wTilemap);  // hlcoord TEXTBOX_X, TEXTBOX_INNERY
	decoord(TEXTBOX_X, TEXTBOX_INNERY - 1, wTilemap);  // decoord TEXTBOX_X, TEXTBOX_INNERY - 1
	LD_BC(3 * SCREEN_WIDTH);  // ld bc, 3 * SCREEN_WIDTH
	CALL(mCopyBytes);  // call CopyBytes
	hlcoord(TEXTBOX_INNERX, TEXTBOX_INNERY + 2, wTilemap);  // hlcoord TEXTBOX_INNERX, TEXTBOX_INNERY + 2
	LD_A(0x7f);  // ld a, " "
	LD_BC(TEXTBOX_INNERW);  // ld bc, TEXTBOX_INNERW
	CALL(mByteFill);  // call ByteFill
	LD_C(5);  // ld c, 5
	CALL(mDelayFrames);  // call DelayFrames
	RET;  // ret

}

int Text_WaitBGMap(){
	SET_PC(0x1238U);
	PUSH_BC;  // push bc
	LDH_A_addr(hOAMUpdate);  // ldh a, [hOAMUpdate]
	PUSH_AF;  // push af
	LD_A(1);  // ld a, 1
	LDH_addr_A(hOAMUpdate);  // ldh [hOAMUpdate], a

	CALL(mWaitBGMap);  // call WaitBGMap

	POP_AF;  // pop af
	LDH_addr_A(hOAMUpdate);  // ldh [hOAMUpdate], a
	POP_BC;  // pop bc
	RET;  // ret

}

int Diacritic(){
	SET_PC(0x1248U);
	PUSH_AF;  // push af
	PUSH_HL;  // push hl
	LD_A_B;  // ld a, b
	LD_BC(-SCREEN_WIDTH);  // ld bc, -SCREEN_WIDTH
	ADD_HL_BC;  // add hl, bc
	LD_hl_A;  // ld [hl], a
	POP_HL;  // pop hl
	POP_AF;  // pop af
	RET;  // ret

}

int LoadBlinkingCursor(){
	SET_PC(0x1253U);
	LD_A(0xee);  // ld a, "▼"
	ldcoord_a(18, 17, wTilemap);  // ldcoord_a 18, 17
	RET;  // ret

}

int UnloadBlinkingCursor(){
	SET_PC(0x1259U);
	LD_A(0x7a);  // ld a, "─"
	ldcoord_a(18, 17, wTilemap);  // ldcoord_a 18, 17
	RET;  // ret

}

int PlaceFarString(){
	SET_PC(0x125FU);
	LD_B_A;  // ld b, a
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af

	LD_A_B;  // ld a, b
	RST(mBankswitch);  // rst Bankswitch
	CALL(mPlaceString);  // call PlaceString

	POP_AF;  // pop af
	RST(mBankswitch);  // rst Bankswitch
	RET;  // ret

}

int PokeFluteTerminator(){
	SET_PC(0x126BU);
//  //  unreferenced
	LD_HL(mPokeFluteTerminator_stop);  // ld hl, .stop
	RET;  // ret


stop:
	SET_PC(0x126FU);
	//text_end ['?']  // text_end

	return mPlaceHLTextAtBC;
}

int PlaceHLTextAtBC(){
	SET_PC(0x1270U);
	LD_A_addr(wTextboxFlags);  // ld a, [wTextboxFlags]
	PUSH_AF;  // push af
	SET_A(NO_TEXT_DELAY_F);  // set NO_TEXT_DELAY_F, a
	LD_addr_A(wTextboxFlags);  // ld [wTextboxFlags], a

	CALL(mDoTextUntilTerminator);  // call DoTextUntilTerminator

	POP_AF;  // pop af
	LD_addr_A(wTextboxFlags);  // ld [wTextboxFlags], a
	RET;  // ret

}

int DoTextUntilTerminator(){
	SET_PC(0x1281U);
	LD_A_hli;  // ld a, [hli]
	CP_A(TX_END);  // cp TX_END
	RET_Z ;  // ret z
	CALL(mDoTextUntilTerminator_TextCommand);  // call .TextCommand
	JR(mDoTextUntilTerminator);  // jr DoTextUntilTerminator


TextCommand:
	SET_PC(0x128AU);
	PUSH_HL;  // push hl
	PUSH_BC;  // push bc
	LD_C_A;  // ld c, a
	LD_B(0);  // ld b, 0
	LD_HL(mTextCommands);  // ld hl, TextCommands
	ADD_HL_BC;  // add hl, bc
	ADD_HL_BC;  // add hl, bc
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]
	POP_BC;  // pop bc
	POP_HL;  // pop hl

// ; jp de
	PUSH_DE;  // push de
	RET;  // ret

}

int TextCommands(){
	SET_PC(0x129BU);
//  entries correspond to TX_* constants (see macros/scripts/text.asm)
	//table_width ['2', 'TextCommands']  // table_width 2, TextCommands
	//dw ['TextCommand_START'];  // dw TextCommand_START         ; TX_START
	//dw ['TextCommand_RAM'];  // dw TextCommand_RAM           ; TX_RAM
	//dw ['TextCommand_BCD'];  // dw TextCommand_BCD           ; TX_BCD
	//dw ['TextCommand_MOVE'];  // dw TextCommand_MOVE          ; TX_MOVE
	//dw ['TextCommand_BOX'];  // dw TextCommand_BOX           ; TX_BOX
	//dw ['TextCommand_LOW'];  // dw TextCommand_LOW           ; TX_LOW
	//dw ['TextCommand_PROMPT_BUTTON'];  // dw TextCommand_PROMPT_BUTTON ; TX_PROMPT_BUTTON
	//dw ['TextCommand_SCROLL'];  // dw TextCommand_SCROLL        ; TX_SCROLL
	//dw ['TextCommand_START_ASM'];  // dw TextCommand_START_ASM     ; TX_START_ASM
	//dw ['TextCommand_DECIMAL'];  // dw TextCommand_DECIMAL       ; TX_DECIMAL
	//dw ['TextCommand_PAUSE'];  // dw TextCommand_PAUSE         ; TX_PAUSE
	//dw ['TextCommand_SOUND'];  // dw TextCommand_SOUND         ; TX_SOUND_DEX_FANFARE_50_79
	//dw ['TextCommand_DOTS'];  // dw TextCommand_DOTS          ; TX_DOTS
	//dw ['TextCommand_WAIT_BUTTON'];  // dw TextCommand_WAIT_BUTTON   ; TX_WAIT_BUTTON
	//dw ['TextCommand_SOUND'];  // dw TextCommand_SOUND         ; TX_SOUND_DEX_FANFARE_20_49
	//dw ['TextCommand_SOUND'];  // dw TextCommand_SOUND         ; TX_SOUND_ITEM
	//dw ['TextCommand_SOUND'];  // dw TextCommand_SOUND         ; TX_SOUND_CAUGHT_MON
	//dw ['TextCommand_SOUND'];  // dw TextCommand_SOUND         ; TX_SOUND_DEX_FANFARE_80_109
	//dw ['TextCommand_SOUND'];  // dw TextCommand_SOUND         ; TX_SOUND_FANFARE
	//dw ['TextCommand_SOUND'];  // dw TextCommand_SOUND         ; TX_SOUND_SLOT_MACHINE_START
	//dw ['TextCommand_STRINGBUFFER'];  // dw TextCommand_STRINGBUFFER  ; TX_STRINGBUFFER
	//dw ['TextCommand_DAY'];  // dw TextCommand_DAY           ; TX_DAY
	//dw ['TextCommand_FAR'];  // dw TextCommand_FAR           ; TX_FAR
	//assert_table_length ['NUM_TEXT_CMDS']  // assert_table_length NUM_TEXT_CMDS

	return mTextCommand_START;
}

int TextCommand_START(){
	SET_PC(0x12C9U);
//  write text until "@"
	LD_D_H;  // ld d, h
	LD_E_L;  // ld e, l
	LD_H_B;  // ld h, b
	LD_L_C;  // ld l, c
	CALL(mPlaceString);  // call PlaceString
	LD_H_D;  // ld h, d
	LD_L_E;  // ld l, e
	INC_HL;  // inc hl
	RET;  // ret

}

int TextCommand_RAM(){
	SET_PC(0x12D4U);
//  write text from a ram address (little endian)
	LD_A_hli;  // ld a, [hli]
	LD_E_A;  // ld e, a
	LD_A_hli;  // ld a, [hli]
	LD_D_A;  // ld d, a
	PUSH_HL;  // push hl
	LD_H_B;  // ld h, b
	LD_L_C;  // ld l, c
	CALL(mPlaceString);  // call PlaceString
	POP_HL;  // pop hl
	RET;  // ret

}

int TextCommand_FAR(){
	SET_PC(0x12E0U);
//  write text from a different bank (little endian)
	LDH_A_addr(hROMBank);  // ldh a, [hROMBank]
	PUSH_AF;  // push af

	LD_A_hli;  // ld a, [hli]
	LD_E_A;  // ld e, a
	LD_A_hli;  // ld a, [hli]
	LD_D_A;  // ld d, a
	LD_A_hli;  // ld a, [hli]

	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a

	PUSH_HL;  // push hl
	LD_H_D;  // ld h, d
	LD_L_E;  // ld l, e
	CALL(mDoTextUntilTerminator);  // call DoTextUntilTerminator
	POP_HL;  // pop hl

	POP_AF;  // pop af
	LDH_addr_A(hROMBank);  // ldh [hROMBank], a
	LD_addr_A(MBC3RomBank);  // ld [MBC3RomBank], a
	RET;  // ret

}

int TextCommand_BCD(){
	SET_PC(0x12FBU);
//  write bcd from address, typically ram
	LD_A_hli;  // ld a, [hli]
	LD_E_A;  // ld e, a
	LD_A_hli;  // ld a, [hli]
	LD_D_A;  // ld d, a
	LD_A_hli;  // ld a, [hli]
	PUSH_HL;  // push hl
	LD_H_B;  // ld h, b
	LD_L_C;  // ld l, c
	LD_C_A;  // ld c, a
	CALL(mPrintBCDNumber);  // call PrintBCDNumber
	LD_B_H;  // ld b, h
	LD_C_L;  // ld c, l
	POP_HL;  // pop hl
	RET;  // ret

}

int TextCommand_MOVE(){
	SET_PC(0x130BU);
//  move to a new tile
	LD_A_hli;  // ld a, [hli]
	LD_addr_A(wMenuScrollPosition + 2);  // ld [wMenuScrollPosition + 2], a
	LD_C_A;  // ld c, a
	LD_A_hli;  // ld a, [hli]
	LD_addr_A(wMenuScrollPosition + 2 + 1);  // ld [wMenuScrollPosition + 2 + 1], a
	LD_B_A;  // ld b, a
	RET;  // ret

}

int TextCommand_BOX(){
	SET_PC(0x1316U);
//  draw a box (height, width)
	LD_A_hli;  // ld a, [hli]
	LD_E_A;  // ld e, a
	LD_A_hli;  // ld a, [hli]
	LD_D_A;  // ld d, a
	LD_A_hli;  // ld a, [hli]
	LD_B_A;  // ld b, a
	LD_A_hli;  // ld a, [hli]
	LD_C_A;  // ld c, a
	PUSH_HL;  // push hl
	LD_H_D;  // ld h, d
	LD_L_E;  // ld l, e
	CALL(mTextbox);  // call Textbox
	POP_HL;  // pop hl
	RET;  // ret

}

int TextCommand_LOW(){
	SET_PC(0x1326U);
//  write text at (1,16)
	bccoord(TEXTBOX_INNERX, TEXTBOX_INNERY + 2, wTilemap);  // bccoord TEXTBOX_INNERX, TEXTBOX_INNERY + 2
	RET;  // ret

}

int TextCommand_PROMPT_BUTTON(){
	SET_PC(0x132AU);
//  wait for button press
	LD_A_addr(wLinkMode);  // ld a, [wLinkMode]
	CP_A(LINK_COLOSSEUM);  // cp LINK_COLOSSEUM
	JP_Z (mTextCommand_WAIT_BUTTON);  // jp z, TextCommand_WAIT_BUTTON

	PUSH_HL;  // push hl
	CALL(mLoadBlinkingCursor);  // call LoadBlinkingCursor
	PUSH_BC;  // push bc
	CALL(mPromptButton);  // call PromptButton
	POP_BC;  // pop bc
	CALL(mUnloadBlinkingCursor);  // call UnloadBlinkingCursor
	POP_HL;  // pop hl
	RET;  // ret

}

int TextCommand_SCROLL(){
	SET_PC(0x1340U);
//  pushes text up two lines and sets the BC cursor to the border tile
//  below the first character column of the text box.
	PUSH_HL;  // push hl
	CALL(mUnloadBlinkingCursor);  // call UnloadBlinkingCursor
	CALL(mTextScroll);  // call TextScroll
	CALL(mTextScroll);  // call TextScroll
	POP_HL;  // pop hl
	bccoord(TEXTBOX_INNERX, TEXTBOX_INNERY + 2, wTilemap);  // bccoord TEXTBOX_INNERX, TEXTBOX_INNERY + 2
	RET;  // ret

}

int TextCommand_START_ASM(){
	SET_PC(0x134FU);
//  run assembly code
	JP_hl;  // jp hl

}

int TextCommand_DECIMAL(){
	SET_PC(0x1350U);
//  print a decimal number
	LD_A_hli;  // ld a, [hli]
	LD_E_A;  // ld e, a
	LD_A_hli;  // ld a, [hli]
	LD_D_A;  // ld d, a
	LD_A_hli;  // ld a, [hli]
	PUSH_HL;  // push hl
	LD_H_B;  // ld h, b
	LD_L_C;  // ld l, c
	LD_B_A;  // ld b, a
	AND_A(0xf);  // and $f
	LD_C_A;  // ld c, a
	LD_A_B;  // ld a, b
	AND_A(0xf0);  // and $f0
	SWAP_A;  // swap a
	SET_A(PRINTNUM_LEFTALIGN_F);  // set PRINTNUM_LEFTALIGN_F, a
	LD_B_A;  // ld b, a
	CALL(mPrintNum);  // call PrintNum
	LD_B_H;  // ld b, h
	LD_C_L;  // ld c, l
	POP_HL;  // pop hl
	RET;  // ret

}

int TextCommand_PAUSE(){
	SET_PC(0x136BU);
//  wait for button press or 30 frames
	PUSH_HL;  // push hl
	PUSH_BC;  // push bc
	CALL(mGetJoypad);  // call GetJoypad
	LDH_A_addr(hJoyDown);  // ldh a, [hJoyDown]
	AND_A(A_BUTTON | B_BUTTON);  // and A_BUTTON | B_BUTTON
	IF_NZ goto done;  // jr nz, .done
	LD_C(30);  // ld c, 30
	CALL(mDelayFrames);  // call DelayFrames

done:
	SET_PC(0x137BU);
	POP_BC;  // pop bc
	POP_HL;  // pop hl
	RET;  // ret

}

int TextCommand_SOUND(){
	SET_PC(0x137EU);
//  play a sound effect from TextSFX
	PUSH_BC;  // push bc
	DEC_HL;  // dec hl
	LD_A_hli;  // ld a, [hli]
	LD_B_A;  // ld b, a
	PUSH_HL;  // push hl
	LD_HL(mTextSFX);  // ld hl, TextSFX

loop:
	SET_PC(0x1386U);
	LD_A_hli;  // ld a, [hli]
	CP_A(-1);  // cp -1
	IF_Z goto done;  // jr z, .done
	CP_A_B;  // cp b
	IF_Z goto play;  // jr z, .play
	INC_HL;  // inc hl
	INC_HL;  // inc hl
	goto loop;  // jr .loop


play:
	SET_PC(0x1392U);
	PUSH_DE;  // push de
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]
	CALL(mPlaySFX);  // call PlaySFX
	CALL(mWaitSFX);  // call WaitSFX
	POP_DE;  // pop de


done:
	SET_PC(0x139DU);
	POP_HL;  // pop hl
	POP_BC;  // pop bc
	RET;  // ret

}

int TextCommand_CRY(){
	SET_PC(0x13A0U);
//  //  unreferenced
//  play a pokemon cry
	PUSH_DE;  // push de
	LD_E_hl;  // ld e, [hl]
	INC_HL;  // inc hl
	LD_D_hl;  // ld d, [hl]
	CALL(mPlayMonCry);  // call PlayMonCry
	POP_DE;  // pop de
	POP_HL;  // pop hl
	POP_BC;  // pop bc
	RET;  // ret

}

int TextSFX(){
	SET_PC(0x13ABU);
	//dbw ['TX_SOUND_DEX_FANFARE_50_79', 'SFX_DEX_FANFARE_50_79']  // dbw TX_SOUND_DEX_FANFARE_50_79,  SFX_DEX_FANFARE_50_79
	//dbw ['TX_SOUND_FANFARE', 'SFX_FANFARE']  // dbw TX_SOUND_FANFARE,            SFX_FANFARE
	//dbw ['TX_SOUND_DEX_FANFARE_20_49', 'SFX_DEX_FANFARE_20_49']  // dbw TX_SOUND_DEX_FANFARE_20_49,  SFX_DEX_FANFARE_20_49
	//dbw ['TX_SOUND_ITEM', 'SFX_ITEM']  // dbw TX_SOUND_ITEM,               SFX_ITEM
	//dbw ['TX_SOUND_CAUGHT_MON', 'SFX_CAUGHT_MON']  // dbw TX_SOUND_CAUGHT_MON,         SFX_CAUGHT_MON
	//dbw ['TX_SOUND_DEX_FANFARE_80_109', 'SFX_DEX_FANFARE_80_109']  // dbw TX_SOUND_DEX_FANFARE_80_109, SFX_DEX_FANFARE_80_109
	//dbw ['TX_SOUND_SLOT_MACHINE_START', 'SFX_SLOT_MACHINE_START']  // dbw TX_SOUND_SLOT_MACHINE_START, SFX_SLOT_MACHINE_START
	//db ['-1'];  // db -1

	return mTextCommand_DOTS;
}

int TextCommand_DOTS(){
	SET_PC(0x13C1U);
//  wait for button press or 30 frames while printing "…"s
	LD_A_hli;  // ld a, [hli]
	LD_D_A;  // ld d, a
	PUSH_HL;  // push hl
	LD_H_B;  // ld h, b
	LD_L_C;  // ld l, c


loop:
	SET_PC(0x13C6U);
	PUSH_DE;  // push de
	LD_A(0x75);  // ld a, "…"
	LD_hli_A;  // ld [hli], a
	CALL(mGetJoypad);  // call GetJoypad
	LDH_A_addr(hJoyDown);  // ldh a, [hJoyDown]
	AND_A(A_BUTTON | B_BUTTON);  // and A_BUTTON | B_BUTTON
	IF_NZ goto next;  // jr nz, .next
	LD_C(10);  // ld c, 10
	CALL(mDelayFrames);  // call DelayFrames

next:
	SET_PC(0x13D8U);
	POP_DE;  // pop de
	DEC_D;  // dec d
	IF_NZ goto loop;  // jr nz, .loop

	LD_B_H;  // ld b, h
	LD_C_L;  // ld c, l
	POP_HL;  // pop hl
	RET;  // ret

}

int TextCommand_WAIT_BUTTON(){
	SET_PC(0x13E0U);
//  wait for button press
	PUSH_HL;  // push hl
	PUSH_BC;  // push bc
	CALL(mPromptButton);  // call PromptButton
	POP_BC;  // pop bc
	POP_HL;  // pop hl
	RET;  // ret

}

int TextCommand_STRINGBUFFER(){
	SET_PC(0x13E8U);
//  Print a string from one of the following:
//  0: wStringBuffer3
//  1: wStringBuffer4
//  2: wStringBuffer5
//  3: wStringBuffer2
//  4: wStringBuffer1
//  5: wEnemyMonNickname
//  6: wBattleMonNickname
	LD_A_hli;  // ld a, [hli]
	PUSH_HL;  // push hl
	LD_E_A;  // ld e, a
	LD_D(0);  // ld d, 0
	LD_HL(mStringBufferPointers);  // ld hl, StringBufferPointers
	ADD_HL_DE;  // add hl, de
	ADD_HL_DE;  // add hl, de
	LD_A(BANK(aStringBufferPointers));  // ld a, BANK(StringBufferPointers)
	CALL(mGetFarWord);  // call GetFarWord
	LD_D_H;  // ld d, h
	LD_E_L;  // ld e, l
	LD_H_B;  // ld h, b
	LD_L_C;  // ld l, c
	CALL(mPlaceString);  // call PlaceString
	POP_HL;  // pop hl
	RET;  // ret

}

int TextCommand_DAY(){
	SET_PC(0x1400U);
//  print the day of the week
	CALL(mGetWeekday);  // call GetWeekday
	PUSH_HL;  // push hl
	PUSH_BC;  // push bc
	LD_C_A;  // ld c, a
	LD_B(0);  // ld b, 0
	LD_HL(mTextCommand_DAY_Days);  // ld hl, .Days
	ADD_HL_BC;  // add hl, bc
	ADD_HL_BC;  // add hl, bc
	LD_A_hli;  // ld a, [hli]
	LD_H_hl;  // ld h, [hl]
	LD_L_A;  // ld l, a
	LD_D_H;  // ld d, h
	LD_E_L;  // ld e, l
	POP_HL;  // pop hl
	CALL(mPlaceString);  // call PlaceString
	LD_H_B;  // ld h, b
	LD_L_C;  // ld l, c
	LD_DE(mTextCommand_DAY_Day);  // ld de, .Day
	CALL(mPlaceString);  // call PlaceString
	POP_HL;  // pop hl
	RET;  // ret


Days:
	SET_PC(0x1420U);
	//dw ['.Sun'];  // dw .Sun
	//dw ['.Mon'];  // dw .Mon
	//dw ['.Tues'];  // dw .Tues
	//dw ['.Wednes'];  // dw .Wednes
	//dw ['.Thurs'];  // dw .Thurs
	//dw ['.Fri'];  // dw .Fri
	//dw ['.Satur'];  // dw .Satur


Sun:
//    db "SUN@"
	SET_PC(0x142EU);

Mon:
//    db "MON@"
	SET_PC(0x1432U);

Tues:
//   db "TUES@"
	SET_PC(0x1436U);

Wednes:
// db "WEDNES@"
	SET_PC(0x143BU);

Thurs:
//  db "THURS@"
	SET_PC(0x1442U);

Fri:
//    db "FRI@"
	SET_PC(0x1448U);

Satur:
//  db "SATUR@"
	SET_PC(0x144CU);

Day:
//    db "DAY@"
	SET_PC(0x1452U);

}
