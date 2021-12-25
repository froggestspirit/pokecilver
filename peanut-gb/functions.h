#pragma once

#define ROM_SIZE 0x200000


int (*func[ROM_SIZE])();

#include "../C/home/init.h"
#include "../C/home/audio.h"


void init_function_pointers(){
    for(int i = 0; i < ROM_SIZE; i++) func[i] = NULL;
	// home/init.c
	func[aReset] = Reset;
	func[a_Start] = _Start;
	func[aInit] = Init;
	func[aClearVRAM] = ClearVRAM;
	func[aBlankBGMap] = BlankBGMap;
	func[aFillBGMap_l] = FillBGMap_l;
	func[aFillBGMap] = FillBGMap;

	// home/audio.c
	func[aInitSound] = InitSound;
	func[aUpdateSound] = UpdateSound;
	func[a_LoadMusicByte] = _LoadMusicByte;
	func[aPlayMusic] = PlayMusic;
	func[aPlayMusic2] = PlayMusic2;
	func[aPlayCry] = PlayCry;
	func[aPlaySFX] = PlaySFX;
	func[aWaitPlaySFX] = WaitPlaySFX;
	func[aWaitSFX] = WaitSFX;
	func[aMaxVolume] = MaxVolume;
	func[aLowVolume] = LowVolume;
	func[aMinVolume] = MinVolume;
	func[aFadeOutToMusic] = FadeOutToMusic;
	func[aFadeInToMusic] = FadeInToMusic;
	func[aSkipMusic] = SkipMusic;
	func[aFadeToMapMusic] = FadeToMapMusic;
	func[aPlayMapMusic] = PlayMapMusic;
	func[aPlayMapMusicBike] = PlayMapMusicBike;
	func[aTryRestartMapMusic] = TryRestartMapMusic;
	func[aRestartMapMusic] = RestartMapMusic;
	func[aSpecialMapMusic] = SpecialMapMusic;
	func[aGetMapMusic_MaybeSpecial] = GetMapMusic_MaybeSpecial;
	func[aPlaceBCDNumberSprite] = PlaceBCDNumberSprite;
	func[aCheckSFX] = CheckSFX;
	func[aTerminateExpBarSound] = TerminateExpBarSound;

	// home/battle.c
	// func[aGetPartyParamLocation] = GetPartyParamLocation;
	// func[aGetPartyLocation] = GetPartyLocation;
	// func[aGetDexNumber] = GetDexNumber;
	// func[aUserPartyAttr] = UserPartyAttr;
	// func[aOpponentPartyAttr] = OpponentPartyAttr;
	// func[aBattlePartyAttr] = BattlePartyAttr;
	// func[aOTPartyAttr] = OTPartyAttr;
	// func[aResetDamage] = ResetDamage;
	// func[aSetPlayerTurn] = SetPlayerTurn;
	// func[aSetEnemyTurn] = SetEnemyTurn;
	// func[aUpdateOpponentInParty] = UpdateOpponentInParty;
	// func[aUpdateUserInParty] = UpdateUserInParty;
	// func[aUpdateBattleMonInParty] = UpdateBattleMonInParty;
	// func[aUpdateBattleMon] = UpdateBattleMon;
	// func[aUpdateEnemyMonInParty] = UpdateEnemyMonInParty;
	// func[aRefreshBattleHuds] = RefreshBattleHuds;
	// func[aUpdateBattleHuds] = UpdateBattleHuds;
	// func[aFarCopyRadioText] = FarCopyRadioText;
	// func[aStdBattleTextbox] = StdBattleTextbox;
	// func[aGetBattleAnimPointer] = GetBattleAnimPointer;
	// func[aGetBattleAnimByte] = GetBattleAnimByte;
	// func[aPushLYOverrides] = PushLYOverrides;

}
