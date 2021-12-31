
#define ROM_SIZE 0x200000

#define REDIRECT(x) func[a##x] = x

int (*func[ROM_SIZE])();

#include "home.h"

void init_function_pointers(){
    for(int i = 0; i < ROM_SIZE; i++) func[i] = NULL;
	
	// home/init.c
	REDIRECT(Reset);
	REDIRECT(_Start);
	REDIRECT(Init);
	REDIRECT(ClearVRAM);
	REDIRECT(BlankBGMap);
	REDIRECT(FillBGMap_l);
	REDIRECT(FillBGMap);

	// home/audio.c
	REDIRECT(InitSound);
	REDIRECT(UpdateSound);
	REDIRECT(_LoadMusicByte);
	REDIRECT(PlayMusic);
	REDIRECT(PlayMusic2);
	REDIRECT(PlayCry);
	REDIRECT(PlaySFX);
	REDIRECT(WaitPlaySFX);
	REDIRECT(WaitSFX);
	REDIRECT(MaxVolume);
	REDIRECT(LowVolume);
	REDIRECT(MinVolume);
	REDIRECT(FadeOutToMusic);
	REDIRECT(FadeInToMusic);
	REDIRECT(SkipMusic);
	REDIRECT(FadeToMapMusic);
	REDIRECT(PlayMapMusic);
	REDIRECT(PlayMapMusicBike);
	REDIRECT(TryRestartMapMusic);
	REDIRECT(RestartMapMusic);
	REDIRECT(SpecialMapMusic);
	REDIRECT(GetMapMusic_MaybeSpecial);
	REDIRECT(PlaceBCDNumberSprite);
	REDIRECT(CheckSFX);
	REDIRECT(TerminateExpBarSound);

	// home/battle.c
	REDIRECT(GetPartyParamLocation);
	REDIRECT(GetPartyLocation);
	REDIRECT(GetDexNumber);
	REDIRECT(UserPartyAttr);
	REDIRECT(OpponentPartyAttr);
	REDIRECT(BattlePartyAttr);
	REDIRECT(OTPartyAttr);
	REDIRECT(ResetDamage);
	REDIRECT(SetPlayerTurn);
	REDIRECT(SetEnemyTurn);
	REDIRECT(UpdateOpponentInParty);
	REDIRECT(UpdateUserInParty);
	REDIRECT(UpdateBattleMonInParty);
	REDIRECT(UpdateBattleMon);
	REDIRECT(UpdateEnemyMonInParty);
	REDIRECT(RefreshBattleHuds);
	REDIRECT(UpdateBattleHuds);
	REDIRECT(FarCopyRadioText);
	REDIRECT(StdBattleTextbox);
	REDIRECT(GetBattleAnimPointer);
	REDIRECT(GetBattleAnimByte);
	REDIRECT(PushLYOverrides);

	// home/array.c
	REDIRECT(IsInArray);
	REDIRECT(SkipNames);
	REDIRECT(AddNTimes);

	// home/call_regs.c
	REDIRECT(_hl_);
	REDIRECT(_de_);

	// home/clear_sprites.c
	REDIRECT(ClearSprites);
	REDIRECT(HideSprites);

	// home/compare.c
	REDIRECT(CompareBytes);
	REDIRECT(CompareBytesLong);

	// home/copy.c
	REDIRECT(CopyBytes);
	REDIRECT(GetFarByte);
	REDIRECT(GetFarWord);
	REDIRECT(ByteFill);

	// home/copy_name.c
	REDIRECT(CopyName1);
	REDIRECT(CopyName2);

	// home/delay.c
	REDIRECT(DelayFrame);
	REDIRECT(DelayFrames);

	// home/fade.c
	REDIRECT(TimeOfDayFade);
	REDIRECT(RotateFourPalettesRight);
	REDIRECT(RotateThreePalettesRight);
	REDIRECT(RotatePalettesRight);
	REDIRECT(RotateFourPalettesLeft);
	REDIRECT(RotateThreePalettesLeft);
	REDIRECT(RotatePalettesLeft);
	REDIRECT(IncGradGBPalTable_00);
	REDIRECT(IncGradGBPalTable_01);
	REDIRECT(IncGradGBPalTable_02);
	REDIRECT(IncGradGBPalTable_03);
	REDIRECT(IncGradGBPalTable_04);
	REDIRECT(IncGradGBPalTable_05);
	REDIRECT(IncGradGBPalTable_06);
	REDIRECT(IncGradGBPalTable_07);
	REDIRECT(IncGradGBPalTable_08);
	REDIRECT(IncGradGBPalTable_09);
	REDIRECT(IncGradGBPalTable_10);
	REDIRECT(IncGradGBPalTable_11);
	REDIRECT(IncGradGBPalTable_12);
	REDIRECT(IncGradGBPalTable_13);
	REDIRECT(IncGradGBPalTable_14);
	REDIRECT(IncGradGBPalTable_15);

	// home/farcall.c
	REDIRECT(FarCall_hl);
	REDIRECT(FarCall_JumpToHL);

	// home/flag.c
	REDIRECT(ResetMapBufferEventFlags);
	REDIRECT(ResetBikeFlags);
	REDIRECT(ResetFlashIfOutOfCave);
	REDIRECT(EventFlagAction);
	REDIRECT(FlagAction);
	REDIRECT(CheckReceivedDex);
	REDIRECT(CheckBPressedDebug);
	REDIRECT(xor_a);
	REDIRECT(xor_a_dec_a);
	REDIRECT(CheckFieldDebug);

	// home/game_time.c
	REDIRECT(ResetGameTime);
	REDIRECT(GameTimer);

	// home/gfx.c
	REDIRECT(FarDecompressBufferedPic);
	REDIRECT(UpdatePlayerSprite);
	REDIRECT(LoadStandardFont);
	REDIRECT(LoadFontsBattleExtra);
	REDIRECT(LoadFontsExtra);
	REDIRECT(DecompressRequest2bpp);
	REDIRECT(FarCopyBytes);
	REDIRECT(FarCopyBytesDouble);
	REDIRECT(Request2bpp);
	REDIRECT(Request1bpp);
	REDIRECT(Get2bpp);
	REDIRECT(Get1bpp);
	REDIRECT(DuplicateGet2bpp);

	// home/item.c
	REDIRECT(DoItemEffect);
	REDIRECT(CheckTossableItem);
	REDIRECT(TossItem);
	REDIRECT(ReceiveItem);
	REDIRECT(CheckItem);

	// home/lcd.c
	REDIRECT(LCD);
	REDIRECT(DisableLCD);
	REDIRECT(EnableLCD);

	// home/math.c
	REDIRECT(SimpleMultiply);
	REDIRECT(SimpleDivide);
	REDIRECT(Multiply);
	REDIRECT(Divide);
	REDIRECT(SubtractAbsolute);

	// home/pokedex_flags.c
	REDIRECT(CountSetBits);
	REDIRECT(GetWeekday);
	REDIRECT(SetSeenAndCaughtMon);
	REDIRECT(SetSeenMon);
	REDIRECT(CheckCaughtMon);
	REDIRECT(CheckSeenMon);
	REDIRECT(PokedexFlagAction);

	// home/predef.c
	REDIRECT(Predef);

	// home/print_bcd.c
	REDIRECT(PrintBCDNumber);
	REDIRECT(PrintBCDDigit);

	// home/printer.c
	REDIRECT(PrinterReceive);
	REDIRECT(AskSerial);

	// home/print_text.c
	REDIRECT(PrintLetterDelay);
	REDIRECT(CopyDataUntil);
	REDIRECT(FarPrintText);
	REDIRECT(CallPointerAt);

	// home/queue_script.c
	REDIRECT(QueueScript);
	REDIRECT(FarQueueScript);

	// home/random.c
	REDIRECT(Random);
	REDIRECT(BattleRandom);
	REDIRECT(RandomRange);

	// home/region.c
	REDIRECT(IsInJohto);
	REDIRECT(SetXYCompareFlags);

	// home/scrolling_menu.c
	REDIRECT(ScrollingMenu);
	REDIRECT(InitScrollingMenu);
	REDIRECT(JoyTextDelay_ForcehJoyDown);

	// home/sine.c
	REDIRECT(Cosine);
	REDIRECT(Sine);

	// home/sprite_anims.c
	REDIRECT(InitSpriteAnimStruct);
	REDIRECT(ReinitSpriteAnimFrame);

	// home/sprite_updates.c
	REDIRECT(DisableSpriteUpdates);
	REDIRECT(EnableSpriteUpdates);

	// home/sram.c
	REDIRECT(OpenSRAM);
	REDIRECT(CloseSRAM);

	// home/stone_queue.c
	REDIRECT(HandleStoneQueue);

	// home/string.c
	REDIRECT(InitString);
	REDIRECT(InitName);
	REDIRECT(_InitString);

	// home/time_palettes.c
	REDIRECT(UpdateTimeAndPals);
	REDIRECT(TimeOfDayPals);
	REDIRECT(UpdateTimePals);

	// home/trainers.c
	REDIRECT(CheckTrainerBattle);
	REDIRECT(_CheckTrainerBattle);
	REDIRECT(TalkToTrainer);
	REDIRECT(LoadTrainer_continue);
	REDIRECT(FacingPlayerDistance_bc);
	REDIRECT(FacingPlayerDistance);
	REDIRECT(CheckTrainerFlag);
	REDIRECT(PrintWinLossText);

	// home/window.c
	REDIRECT(RefreshScreen);
	REDIRECT(CloseText);
	REDIRECT(OpenText);
	REDIRECT(_OpenAndCloseMenu_HDMATransferTilemapAndAttrmap);
	REDIRECT(SafeUpdateSprites);
	REDIRECT(SetCarryFlag);

	// home/vblank.c
	REDIRECT(VBlank);
	REDIRECT(VBlank0);
	REDIRECT(VBlank1);
	REDIRECT(UpdatePals);
	REDIRECT(VBlank4);
	REDIRECT(VBlank5);
	REDIRECT(VBlank2);
	REDIRECT(VBlank3);

	// home/time.c
	REDIRECT(Timer);
	REDIRECT(LatchClock);
	REDIRECT(UpdateTime);
	REDIRECT(GetClock);
	REDIRECT(FixDays);
	REDIRECT(FixTime);
	REDIRECT(InitTimeOfDay);
	REDIRECT(InitDayOfWeek);
	REDIRECT(InitTime);
	REDIRECT(ClearClock);
	REDIRECT(SetClock);
	REDIRECT(ClearRTCStatus);
	REDIRECT(RecordRTCStatus);
	REDIRECT(CheckRTCStatus);

	// home/serial.c
	REDIRECT(Serial);
	REDIRECT(Serial_ExchangeBytes);
	REDIRECT(Serial_ExchangeByte);
	REDIRECT(CheckLinkTimeoutFramesNonzero);
	REDIRECT(SerialDisconnected);
	REDIRECT(Serial_ExchangeSyncBytes);
	REDIRECT(Serial_PrintWaitingTextAndSyncAndExchangeNybble);
	REDIRECT(Serial_SyncAndExchangeNybble);
	REDIRECT(WaitLinkTransfer);
	REDIRECT(LinkTransfer);
	REDIRECT(LinkDataReceived);
	REDIRECT(SetBitsForTimeCapsuleRequestIfNotLinked);

	// home/joypad.c
	//REDIRECT(Joypad);
	//REDIRECT(ClearJoypad);
	//REDIRECT(UpdateJoypad);
	//REDIRECT(GetJoypad);
	//REDIRECT(StartAutoInput);
	//REDIRECT(StopAutoInput);
	//REDIRECT(JoyTitleScreenInput);
	//REDIRECT(JoyWaitAorB);
	//REDIRECT(WaitButton);
	//REDIRECT(JoyTextDelay);
	//REDIRECT(WaitPressAorB_BlinkCursor);
	//REDIRECT(SimpleWaitPressAorB);
	//REDIRECT(PromptButton);
	//REDIRECT(BlinkCursor);

}
