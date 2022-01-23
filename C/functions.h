#define REDIRECT(x) func[a##x] = x

#include "home.h"
#include "audio/engine.h"
#include "engine/tilesets/tileset_anims.h"

void init_function_pointers() {
    for (int i = 0; i < ROM_SIZE; i++) func[i] = NULL;

    // home/init.c
    REDIRECT(Reset);
    REDIRECT(v_Start);
    REDIRECT(Init);
    REDIRECT(ClearVRAM);
    REDIRECT(BlankBGMap);
    REDIRECT(FillBGMap_l);
    REDIRECT(FillBGMap);

    // home/audio.c
    REDIRECT(InitSound);
    REDIRECT(UpdateSound);
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
    REDIRECT(v_hl_);
    REDIRECT(v_de_);

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
    //REDIRECT(DelayFrame);
    //REDIRECT(DelayFrames);

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
    REDIRECT(v_InitString);

    // home/time_palettes.c
    REDIRECT(UpdateTimeAndPals);
    REDIRECT(TimeOfDayPals);
    REDIRECT(UpdateTimePals);

    // home/trainers.c
    REDIRECT(CheckTrainerBattle);
    REDIRECT(v_CheckTrainerBattle);
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
    REDIRECT(v_OpenAndCloseMenu_HDMATransferTilemapAndAttrmap);
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
    REDIRECT(Joypad);
    REDIRECT(ClearJoypad);
    REDIRECT(UpdateJoypad);
    REDIRECT(GetJoypad);
    REDIRECT(StartAutoInput);
    REDIRECT(StopAutoInput);
    REDIRECT(JoyTitleScreenInput);
    REDIRECT(JoyWaitAorB);
    REDIRECT(WaitButton);
    REDIRECT(JoyTextDelay);
    REDIRECT(WaitPressAorB_BlinkCursor);
    REDIRECT(SimpleWaitPressAorB);
    REDIRECT(PromptButton);
    REDIRECT(BlinkCursor);

    // home/decompress.c
    REDIRECT(FarDecompress);
    REDIRECT(Decompress);

    // home/palettes.c
    REDIRECT(UpdatePalsIfCGB);
    REDIRECT(UpdateCGBPals);
    REDIRECT(DmgToCgbBGPals);
    REDIRECT(DmgToCgbObjPals);
    REDIRECT(DmgToCgbObjPal0);
    REDIRECT(DmgToCgbObjPal1);
    REDIRECT(CopyPals);
    REDIRECT(ClearVBank1);
    REDIRECT(ReloadPalettes);
    REDIRECT(ReloadSpritesNoPalettes);
    REDIRECT(SwapTextboxPalettes);
    REDIRECT(ScrollBGMapPalettes);

    // home/video.c
    REDIRECT(UpdateBGMapBuffer);
    REDIRECT(WaitTop);
    REDIRECT(UpdateBGMap);
    REDIRECT(Serve1bppRequest);
    REDIRECT(Serve2bppRequest);
    REDIRECT(AnimateTileset);
    REDIRECT(Video_DummyFunction);
    REDIRECT(EnableSpriteDisplay);
    REDIRECT(FillBGMap0WithBlack);

    // home/movement.c
    REDIRECT(InitMovementBuffer);
    REDIRECT(DecrementMovementBufferCount);
    REDIRECT(AppendToMovementBuffer);
    REDIRECT(AppendToMovementBufferNTimes);
    REDIRECT(ComputePathToWalkToPlayer);

    // home/copy_tilemap.c
    REDIRECT(LoadTilemapToTempTilemap);
    REDIRECT(SafeLoadTempTilemapToTilemap);
    REDIRECT(LoadTempTilemapToTilemap);

    // home/names.c
    REDIRECT(NamesPointers);
    REDIRECT(GetName);
    REDIRECT(GetNthString);
    REDIRECT(GetBasePokemonName);
    REDIRECT(GetPokemonName);
    REDIRECT(GetItemName);
    REDIRECT(GetTMHMName);
    REDIRECT(GetMoveName);

    // home/pokemon.c
    REDIRECT(DrawBattleHPBar);
    REDIRECT(PrepMonFrontpic);
    REDIRECT(v_PrepMonFrontpic);
    REDIRECT(PlayStereoCry);
    REDIRECT(PlayMonCry);
    REDIRECT(v_PlayMonCry);
    REDIRECT(LoadCry);
    REDIRECT(GetCryIndex);
    REDIRECT(PrintLevel);
    REDIRECT(PrintLevel_Force3Digits);
    REDIRECT(Print8BitNumLeftAlign);
    REDIRECT(GetNthMove);
    REDIRECT(GetBaseData);
    REDIRECT(GetCurNickname);
    REDIRECT(GetNickname);

    // home/hm_moves.c
    REDIRECT(IsHM);
    REDIRECT(IsHMMove);

    // home/print_num.c
    REDIRECT(PrintNum);
    REDIRECT(PrintHexNumber);

    // home/text.c
    REDIRECT(ClearBox);
    REDIRECT(ClearTilemap);
    REDIRECT(ClearScreen);
    REDIRECT(Textbox);
    REDIRECT(TextboxBorder);
    REDIRECT(TextboxPalette);
    REDIRECT(SpeechTextbox);
    REDIRECT(GameFreakText);
    REDIRECT(RadioTerminator);
    REDIRECT(PrintText);
    REDIRECT(PrintTextboxText);
    REDIRECT(SetUpTextbox);
    REDIRECT(PlaceString);
    REDIRECT(PlaceNextChar);
    REDIRECT(DummyChar);
    REDIRECT(NextChar);
    //REDIRECT(CheckDict);  // This one will probably need to be manually converted
    REDIRECT(PrintMomsName);
    REDIRECT(PrintPlayerName);
    REDIRECT(PrintRivalName);
    REDIRECT(PrintRedsName);
    REDIRECT(PrintGreensName);
    REDIRECT(TrainerChar);
    REDIRECT(TMChar);
    REDIRECT(PCChar);
    REDIRECT(RocketChar);
    REDIRECT(PlacePOKe);
    REDIRECT(PlaceKougeki);
    REDIRECT(SixDotsChar);
    REDIRECT(PlacePKMN);
    REDIRECT(PlacePOKE);
    REDIRECT(PlaceJPRoute);
    REDIRECT(PlaceWatashi);
    REDIRECT(PlaceKokoWa);
    REDIRECT(PlaceMoveTargetsName);
    REDIRECT(PlaceMoveUsersName);
    REDIRECT(PlaceBattlersName);
    REDIRECT(PlaceEnemysName);
    REDIRECT(PlaceCommandCharacter);
    REDIRECT(TMCharText);
    REDIRECT(TrainerCharText);
    REDIRECT(PCCharText);
    REDIRECT(RocketCharText);
    REDIRECT(PlacePOKeText);
    REDIRECT(KougekiText);
    REDIRECT(SixDotsCharText);
    REDIRECT(EnemyText);
    REDIRECT(PlacePKMNText);
    REDIRECT(PlacePOKEText);
    REDIRECT(String_Space);
    REDIRECT(PlaceJPRouteText);
    REDIRECT(PlaceWatashiText);
    REDIRECT(PlaceKokoWaText);
    REDIRECT(NextLineChar);
    REDIRECT(LineFeedChar);
    REDIRECT(LineChar);
    REDIRECT(Paragraph);
    REDIRECT(v_ContText);
    REDIRECT(v_ContTextNoPause);
    REDIRECT(ContText);
    REDIRECT(PlaceDexEnd);
    REDIRECT(PromptText);
    REDIRECT(DoneText);
    REDIRECT(NullChar);
    REDIRECT(TextScroll);
    REDIRECT(Text_WaitBGMap);
    REDIRECT(Diacritic);
    REDIRECT(LoadBlinkingCursor);
    REDIRECT(UnloadBlinkingCursor);
    REDIRECT(PlaceFarString);
    REDIRECT(PokeFluteTerminator);
    REDIRECT(PlaceHLTextAtBC);
    REDIRECT(DoTextUntilTerminator);
    REDIRECT(TextCommands);
    REDIRECT(TextCommand_START);
    REDIRECT(TextCommand_RAM);
    REDIRECT(TextCommand_FAR);
    REDIRECT(TextCommand_BCD);
    REDIRECT(TextCommand_MOVE);
    REDIRECT(TextCommand_BOX);
    REDIRECT(TextCommand_LOW);
    REDIRECT(TextCommand_PROMPT_BUTTON);
    REDIRECT(TextCommand_SCROLL);
    REDIRECT(TextCommand_START_ASM);
    REDIRECT(TextCommand_DECIMAL);
    REDIRECT(TextCommand_PAUSE);
    REDIRECT(TextCommand_SOUND);
    REDIRECT(TextCommand_CRY);
    REDIRECT(TextSFX);
    REDIRECT(TextCommand_DOTS);
    REDIRECT(TextCommand_WAIT_BUTTON);
    REDIRECT(TextCommand_STRINGBUFFER);
    REDIRECT(TextCommand_DAY);

    // home/map_objects.c
    REDIRECT(GetSpritePalette);
    REDIRECT(GetSpriteVTile);
    REDIRECT(DoesSpriteHaveFacings);
    REDIRECT(GetPlayerStandingTile);
    REDIRECT(CheckOnWater);
    REDIRECT(GetTileCollision);
    REDIRECT(CheckGrassTile);
    REDIRECT(CheckSuperTallGrassTile);
    REDIRECT(CheckCutTreeTile);
    REDIRECT(CheckHeadbuttTreeTile);
    REDIRECT(CheckCounterTile);
    REDIRECT(CheckPitTile);
    REDIRECT(CheckIceTile);
    REDIRECT(CheckWhirlpoolTile);
    REDIRECT(CheckWaterfallTile);
    REDIRECT(CheckStandingOnEntrance);
    REDIRECT(GetMapObject);
    REDIRECT(CheckObjectVisibility);
    REDIRECT(CheckObjectTime);
    REDIRECT(CopyMapObjectStruct);
    REDIRECT(UnmaskCopyMapObjectStruct);
    REDIRECT(ApplyDeletionToMapObject);
    REDIRECT(DeleteObjectStruct);
    REDIRECT(CopyPlayerObjectTemplate);
    REDIRECT(DeleteFollowerMapObject);
    REDIRECT(LoadMovementDataPointer);
    REDIRECT(FindFirstEmptyObjectStruct);
    REDIRECT(GetSpriteMovementFunction);
    REDIRECT(GetInitialFacing);
    REDIRECT(CopySpriteMovementData);
    REDIRECT(v_GetMovementByte);
    REDIRECT(SetVramState_Bit0);
    REDIRECT(ResetVramState_Bit0);
    REDIRECT(UpdateSprites);
    REDIRECT(GetObjectStruct);
    REDIRECT(DoesObjectHaveASprite);
    REDIRECT(SetSpriteDirection);
    REDIRECT(GetSpriteDirection);

    // home/menu.c
    REDIRECT(Load2DMenuData);
    REDIRECT(StaticMenuJoypad);
    REDIRECT(ScrollingMenuJoypad);
    REDIRECT(GetMenuJoypad);
    REDIRECT(PlaceHollowCursor);
    REDIRECT(HideCursor);
    REDIRECT(PushWindow);
    REDIRECT(ExitMenu);
    REDIRECT(InitVerticalMenuCursor);
    REDIRECT(CloseWindow);
    REDIRECT(RestoreTileBackup);
    REDIRECT(PopWindow);
    REDIRECT(GetMenuBoxDims);
    REDIRECT(CopyMenuData);
    REDIRECT(GetWindowStackTop);
    REDIRECT(PlaceVerticalMenuItems);
    REDIRECT(MenuBox);
    REDIRECT(GetMenuTextStartCoord);
    REDIRECT(ClearMenuBoxInterior);
    REDIRECT(ClearWholeMenuBox);
    REDIRECT(MenuBoxCoord2Tile);
    REDIRECT(Coord2Tile);
    REDIRECT(LoadMenuHeader);
    REDIRECT(CopyMenuHeader);
    REDIRECT(StoreMenuCursorPosition);
    REDIRECT(MenuTextbox);
    REDIRECT(Menu_DummyFunction);
    REDIRECT(LoadMenuTextbox);
    REDIRECT(MenuTextboxBackup);
    REDIRECT(LoadStandardMenuHeader);
    REDIRECT(Call_ExitMenu);
    REDIRECT(VerticalMenu);
    REDIRECT(GetMenu2);
    REDIRECT(CopyNameFromMenu);
    REDIRECT(YesNoBox);
    REDIRECT(PlaceYesNoBox);
    REDIRECT(PlaceGenericTwoOptionBox);
    REDIRECT(v_YesNoBox);
    REDIRECT(InterpretTwoOptionMenu);
    REDIRECT(YesNoMenuHeader);
    REDIRECT(OffsetMenuHeader);
    REDIRECT(v_OffsetMenuHeader);
    REDIRECT(DoNthMenu);
    REDIRECT(SetUpMenu);
    REDIRECT(DrawVariableLengthMenuBox);
    REDIRECT(MenuWriteText);
    REDIRECT(AutomaticGetMenuBottomCoord);
    REDIRECT(GetMenuIndexSet);
    REDIRECT(RunMenuItemPrintingFunction);
    REDIRECT(InitMenuCursorAndButtonPermissions);
    REDIRECT(GetScrollingMenuJoypad);
    REDIRECT(GetStaticMenuJoypad);
    REDIRECT(ContinueGettingMenuJoypad);
    REDIRECT(PlaceMenuStrings);
    REDIRECT(PlaceNthMenuStrings);
    REDIRECT(GetNthMenuStrings);
    REDIRECT(MenuJumptable);
    REDIRECT(GetMenuDataPointerTableEntry);
    REDIRECT(ClearWindowData);
    REDIRECT(MenuClickSound);
    REDIRECT(PlayClickSFX);
    REDIRECT(MenuTextboxWaitButton);
    REDIRECT(Place2DMenuItemName);
    REDIRECT(v_2DMenu);
    REDIRECT(ResetBGWindow);

    // home/map.c
    REDIRECT(ClearUnusedMapBuffer);
    REDIRECT(CheckScenes);
    REDIRECT(GetCurrentMapSceneID);
    REDIRECT(GetMapSceneID);
    REDIRECT(OverworldTextModeSwitch);
    REDIRECT(LoadMapPart);
    REDIRECT(LoadMetatiles);
    REDIRECT(ReturnToMapFromSubmenu);
    REDIRECT(CheckOutdoorMap);
    REDIRECT(CheckIndoorMap);
    REDIRECT(CheckUnknownMap);
    REDIRECT(LoadMapAttributes);
    REDIRECT(LoadMapAttributes_SkipObjects);
    REDIRECT(CopyMapPartialAndAttributes);
    REDIRECT(ReadMapEvents);
    REDIRECT(ReadMapScripts);
    REDIRECT(CopyMapAttributes);
    REDIRECT(GetMapConnections);
    REDIRECT(GetMapConnection);
    REDIRECT(ReadMapSceneScripts);
    REDIRECT(ReadMapCallbacks);
    REDIRECT(ReadWarps);
    REDIRECT(ReadCoordEvents);
    REDIRECT(ReadBGEvents);
    REDIRECT(ReadObjectEvents);
    REDIRECT(CopyMapObjectEvents);
    REDIRECT(ClearObjectStructs);
    REDIRECT(GetWarpDestCoords);
    REDIRECT(GetMapScreenCoords);
    REDIRECT(LoadBlockData);
    REDIRECT(ChangeMap);
    REDIRECT(FillMapConnections);
    REDIRECT(FillNorthConnectionStrip);
    REDIRECT(FillSouthConnectionStrip);
    REDIRECT(FillWestConnectionStrip);
    REDIRECT(FillEastConnectionStrip);
    REDIRECT(LoadMapStatus);
    REDIRECT(CallScript);
    REDIRECT(CallMapScript);
    REDIRECT(RunMapCallback);
    REDIRECT(ExecuteCallbackScript);
    REDIRECT(MapTextbox);
    REDIRECT(Call_a_de);
    REDIRECT(GetMovementData);
    REDIRECT(GetScriptByte);
    REDIRECT(ObjectEvent);
    REDIRECT(ObjectEventText);
    REDIRECT(BGEvent);
    REDIRECT(BGEventText);
    REDIRECT(CoordinatesEvent);
    REDIRECT(CoordinatesEventText);
    REDIRECT(CheckObjectMask);
    REDIRECT(MaskObject);
    REDIRECT(UnmaskObject);
    REDIRECT(ScrollMapUp);
    REDIRECT(ScrollMapDown);
    REDIRECT(ScrollMapLeft);
    REDIRECT(ScrollMapRight);
    REDIRECT(BackupBGMapRow);
    REDIRECT(BackupBGMapColumn);
    REDIRECT(UpdateBGMapRow);
    REDIRECT(UpdateBGMapColumn);
    REDIRECT(ClearBGMapBuffer);
    REDIRECT(LoadTilesetGFX);
    REDIRECT(BufferScreen);
    REDIRECT(SaveScreen);
    REDIRECT(LoadConnectionBlockData);
    REDIRECT(SaveScreen_LoadConnection);
    REDIRECT(GetMovementPermissions);
    REDIRECT(GetFacingTileCoord);
    REDIRECT(GetCoordTile);
    REDIRECT(GetBlockLocation);
    REDIRECT(CheckFacingBGEvent);
    REDIRECT(CheckIfFacingTileCoordIsBGEvent);
    REDIRECT(CheckCurrentMapCoordEvents);
    REDIRECT(FadeToMenu);
    REDIRECT(CloseSubmenu);
    REDIRECT(ExitAllMenus);
    REDIRECT(FinishExitMenu);
    REDIRECT(ReturnToMapWithSpeechTextbox);
    REDIRECT(ReloadTilesetAndPalettes);
    REDIRECT(GetMapPointer);
    REDIRECT(GetAnyMapPointer);
    REDIRECT(GetMapField);
    REDIRECT(GetAnyMapField);
    REDIRECT(SwitchToMapAttributesBank);
    REDIRECT(SwitchToAnyMapAttributesBank);
    REDIRECT(GetMapAttributesBank);
    REDIRECT(GetAnyMapAttributesBank);
    REDIRECT(CopyMapPartial);
    REDIRECT(SwitchToMapScriptsBank);
    REDIRECT(GetMapScriptsBank);
    REDIRECT(GetAnyMapBlocksBank);
    REDIRECT(GetMapAttributesPointer);
    REDIRECT(GetMapEnvironment);
    REDIRECT(Map_DummyFunction);
    REDIRECT(GetAnyMapEnvironment);
    REDIRECT(GetAnyMapTileset);
    REDIRECT(GetWorldMapLocation);
    REDIRECT(GetMapMusic);
    REDIRECT(GetMapTimeOfDay);
    REDIRECT(GetMapPhoneService);
    REDIRECT(GetPhoneServiceTimeOfDayByte);
    REDIRECT(GetFishingGroup);
    REDIRECT(LoadMapTileset);
    REDIRECT(DummyEndPredef);

    // home/tilemap.c
    REDIRECT(ClearBGPalettes);
    REDIRECT(WaitBGMap);
    REDIRECT(WaitBGMap2);
    REDIRECT(IsCGB);
    REDIRECT(ApplyTilemap);
    REDIRECT(CGBOnly_CopyTilemapAtOnce);
    REDIRECT(CopyTilemapAtOnce);
    REDIRECT(SetPalettes);
    REDIRECT(ClearPalettes);
    REDIRECT(GetMemSGBLayout);
    REDIRECT(GetSGBLayout);
    REDIRECT(SetHPPal);
    REDIRECT(GetHPPal);

    // home/battle_vars.c
    REDIRECT(GetBattleVar);
    REDIRECT(GetBattleVarAddr);
    REDIRECT(BattleVarPairs);
    REDIRECT(BattleVarLocations);

    // home/header.c
    REDIRECT(FarCall);
    REDIRECT(JumpTable);
    REDIRECT(IHandler);
    REDIRECT(Start);

    // home/warp_connection.c
    REDIRECT(HandleNewMap);
    REDIRECT(HandleContinueMap);
    REDIRECT(LoadMapTimeOfDay);
    REDIRECT(LoadMapGraphics);
    REDIRECT(LoadMapPalettes);
    REDIRECT(RefreshMapSprites);
    REDIRECT(CheckMovingOffEdgeOfMap);
    REDIRECT(EnterMapConnection);
    REDIRECT(CheckWarpTile);
    REDIRECT(WarpCheck);
    REDIRECT(GetDestinationWarpNumber);
    REDIRECT(CopyWarpData);
    REDIRECT(EnterMapWarp);

    // audio/engine.c
    REDIRECT(PlayTrainerEncounterMusic);

    // engine/tilesets/tileset_anims.c
    REDIRECT(DummyPredef3A_DummyData);
    REDIRECT(DummyPredef3A);
    REDIRECT(DummyPredef3A_DummyFunction);
    REDIRECT(v_AnimateTileset);
    REDIRECT(Tileset0Anim);
    REDIRECT(TilesetJohtoModernAnim);
    REDIRECT(TilesetKantoAnim);
    REDIRECT(TilesetParkAnim);
    REDIRECT(TilesetForestAnim);
    REDIRECT(TilesetJohtoAnim);
    REDIRECT(UnusedTilesetAnim1);
    REDIRECT(UnusedTilesetAnim2);
    REDIRECT(TilesetPortAnim);
    REDIRECT(TilesetEliteFourRoomAnim);
    REDIRECT(UnusedTilesetAnim3);
    REDIRECT(UnusedTilesetAnim4);
    REDIRECT(TilesetCaveAnim);
    REDIRECT(TilesetDarkCaveAnim);
    REDIRECT(TilesetIcePathAnim);
    REDIRECT(TilesetTowerAnim);
    REDIRECT(UnusedTilesetAnim5);
    REDIRECT(TilesetHouseAnim);
    REDIRECT(TilesetPlayersHouseAnim);
    REDIRECT(TilesetPokecenterAnim);
    REDIRECT(TilesetGateAnim);
    REDIRECT(TilesetLabAnim);
    REDIRECT(TilesetFacilityAnim);
    REDIRECT(TilesetMartAnim);
    REDIRECT(TilesetMansionAnim);
    REDIRECT(TilesetGameCornerAnim);
    REDIRECT(TilesetTraditionalHouseAnim);
    REDIRECT(TilesetTrainStationAnim);
    REDIRECT(TilesetChampionsRoomAnim);
    REDIRECT(TilesetLighthouseAnim);
    REDIRECT(TilesetPlayersRoomAnim);
    REDIRECT(TilesetRuinsOfAlphAnim);
    REDIRECT(TilesetRadioTowerAnim);
    REDIRECT(TilesetUndergroundAnim);
    REDIRECT(DoneTileAnimation);
    REDIRECT(WaitTileAnimation);
    REDIRECT(StandingTileFrame8);
    REDIRECT(ScrollTileRightLeft);
    REDIRECT(ScrollTileUpDown);
    REDIRECT(ScrollTileLeft);
    REDIRECT(ScrollTileRight);
    REDIRECT(ScrollTileUp);
    REDIRECT(ScrollTileDown);
    REDIRECT(AnimateWaterTile);
    REDIRECT(AnimateFlowerTile);
    REDIRECT(AnimateLavaBubbleTile1);
    REDIRECT(AnimateLavaBubbleTile2);
    REDIRECT(LavaBubbleTileFrames);
    REDIRECT(AnimateTowerPillarTile);
    REDIRECT(StandingTileFrame);
    REDIRECT(AnimateWhirlpoolTile);
    REDIRECT(WriteTileFromAnimBuffer);
    REDIRECT(ReadTileToAnimBuffer);
    REDIRECT(WriteTile);
    REDIRECT(AnimateWaterPalette);
    REDIRECT(FlickeringCaveEntrancePalette);
    REDIRECT(TowerPillarTilePointer1);
    REDIRECT(TowerPillarTilePointer2);
    REDIRECT(TowerPillarTilePointer3);
    REDIRECT(TowerPillarTilePointer4);
    REDIRECT(TowerPillarTilePointer5);
    REDIRECT(TowerPillarTilePointer6);
    REDIRECT(TowerPillarTilePointer7);
    REDIRECT(TowerPillarTilePointer8);
    REDIRECT(TowerPillarTilePointer9);
    REDIRECT(TowerPillarTilePointer10);
    REDIRECT(TowerPillarTile1);
    REDIRECT(TowerPillarTile2);
    REDIRECT(TowerPillarTile3);
    REDIRECT(TowerPillarTile4);
    REDIRECT(TowerPillarTile5);
    REDIRECT(TowerPillarTile6);
    REDIRECT(TowerPillarTile7);
    REDIRECT(TowerPillarTile8);
    REDIRECT(TowerPillarTile9);
    REDIRECT(TowerPillarTile10);
    REDIRECT(WhirlpoolFrames1);
    REDIRECT(WhirlpoolFrames2);
    REDIRECT(WhirlpoolFrames3);
    REDIRECT(WhirlpoolFrames4);
    REDIRECT(WhirlpoolTiles1);
    REDIRECT(WhirlpoolTiles2);
    REDIRECT(WhirlpoolTiles3);
    REDIRECT(WhirlpoolTiles4);
    REDIRECT(TilesetAnims_DummyFunction);
}
