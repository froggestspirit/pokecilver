
#define ROM_SIZE 0x200000

#define REDIRECT(x) func[a##x] = x

int (*func[ROM_SIZE])();

#include "../C/home/array.h"
#include "../C/home/audio.h"
#include "../C/home/battle.h"
// #include "../C/home/battle_vars.h"
#include "../C/home/call_regs.h"
#include "../C/home/clear_sprites.h"
#include "../C/home/compare.h"
#include "../C/home/copy.h"
#include "../C/home/copy_name.h"
// #include "../C/home/copy_tilemap.h"
// #include "../C/home/decompress.h"
#include "../C/home/delay.h"
#include "../C/home/fade.h"
#include "../C/home/farcall.h"
#include "../C/home/flag.h"
#include "../C/home/game_time.h"
#include "../C/home/gfx.h"
// #include "../C/home/header.h"
// #include "../C/home/hm_moves.h"
#include "../C/home/init.h"
#include "../C/home/item.h"
// #include "../C/home/joypad.h"
#include "../C/home/lcd.h"
// #include "../C/home/map.h"
// #include "../C/home/map_objects.h"
#include "../C/home/math.h"
// #include "../C/home/menu.h"
// #include "../C/home/movement.h"
// #include "../C/home/names.h"
// #include "../C/home/palettes.h"
#include "../C/home/pokedex_flags.h"
// #include "../C/home/pokemon.h"
// #include "../C/home/predef.h"
// #include "../C/home/print_bcd.h"
// #include "../C/home/printer.h"
// #include "../C/home/print_num.h"
// #include "../C/home/print_text.h"
// #include "../C/home/queue_script.h"
// #include "../C/home/random.h"
// #include "../C/home/region.h"
// #include "../C/home/scrolling_menu.h"
// #include "../C/home/serial.h"
// #include "../C/home/sine.h"
// #include "../C/home/sprite_anims.h"
// #include "../C/home/sprite_updates.h"
// #include "../C/home/sram.h"
// #include "../C/home/stone_queue.h"
// #include "../C/home/string.h"
// #include "../C/home/text.h"
// #include "../C/home/tilemap.h"
// #include "../C/home/time.h"
// #include "../C/home/time_palettes.h"
// #include "../C/home/trainers.h"
// #include "../C/home/vblank.h"
// #include "../C/home/video.h"
// #include "../C/home/warp_connection.h"
// #include "../C/home/window.h"


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
	//REDIRECT(FarDecompressBufferedPic);
	//REDIRECT(UpdatePlayerSprite);
	REDIRECT(LoadStandardFont);
	//REDIRECT(LoadFontsBattleExtra);
	//REDIRECT(LoadFontsExtra);
	//REDIRECT(DecompressRequest2bpp);
	//REDIRECT(FarCopyBytes);
	//REDIRECT(FarCopyBytesDouble);
	//REDIRECT(Request2bpp);
	//REDIRECT(Request1bpp);
	//REDIRECT(Get2bpp);
	//REDIRECT(Get1bpp);
	//REDIRECT(DuplicateGet2bpp);

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

}
