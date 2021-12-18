// wInputType::
#define AUTO_INPUT 0xff

// wDebugFlags::
enum {
	DEBUG_BATTLE_F = 0,
	DEBUG_FIELD_F
};
// wCurDexMode::
enum {
	DEXMODE_NEW = 0,
	DEXMODE_OLD,
	DEXMODE_ABC,
	DEXMODE_UNOWN
};
// wMonType::
enum {
	PARTYMON   = 0,
	OTPARTYMON = 1,
	BOXMON     = 2,
	TEMPMON    = 3,
	WILDMON    = 4
};
// wGameTimerPaused::
#define GAME_TIMER_PAUSED_F 0

// wJoypadDisable::
#define JOYPAD_DISABLE_MON_FAINT_F    6
#define JOYPAD_DISABLE_SGB_TRANSFER_F 7

// wOptions1::
#define TEXT_DELAY_MASK 0b111
enum {
	NO_TEXT_SCROLL = 4,
	STEREO         = 5,
	BATTLE_SHIFT   = 6,
	BATTLE_SCENE   = 7
};
#define TEXT_DELAY_FAST 0b001 // 1
#define TEXT_DELAY_MED  0b011 // 3
#define TEXT_DELAY_SLOW 0b101 // 5

// wTextboxFrame::
enum {
	FRAME_1 = 0,
	FRAME_2 = 1,
	FRAME_3 = 2,
	FRAME_4 = 3,
	FRAME_5 = 4,
	FRAME_6 = 5,
	FRAME_7 = 6,
	FRAME_8 = 7,
} NUM_FRAMES;

// wTextboxFlags::
enum {
	FAST_TEXT_DELAY_F = 0,
	NO_TEXT_DELAY_F   = 1
};
// wGBPrinterBrightness::
#define GBPRINTER_LIGHTEST 0x00
#define GBPRINTER_LIGHTER  0x20
#define GBPRINTER_NORMAL   0x40
#define GBPRINTER_DARKER   0x60
#define GBPRINTER_DARKEST  0x7f

// wOptions2::
enum {
	MENU_ACCOUNT = 0
};
// wWalkingDirection::
enum {
	STANDING = -1,
	DOWN     = 0,
	UP       = 1,
	LEFT     = 2,
	RIGHT    = 3
} NUM_DIRECTIONS;

#define DOWN_MASK  1 << DOWN
#define UP_MASK    1 << UP
#define LEFT_MASK  1 << LEFT
#define RIGHT_MASK 1 << RIGHT

// wFacingDirection::
enum {
	FACE_DOWN  = 8,
	FACE_UP    = 4,
	FACE_LEFT  = 2,
	FACE_RIGHT = 1
};
#define FACE_CURRENT 0

// wPokemonWithdrawDepositParameter::
#define PC_WITHDRAW       0
#define PC_DEPOSIT        1
#define REMOVE_PARTY      0
#define REMOVE_BOX        1
#define DAY_CARE_WITHDRAW 2
#define DAY_CARE_DEPOSIT  3

// wPlayerStepFlags::
enum {
	PLAYERSTEP_MIDAIR_F   = 4,
	PLAYERSTEP_CONTINUE_F = 5,
	PLAYERSTEP_STOP_F     = 6,
	PLAYERSTEP_START_F    = 7
};

// wInitListType::
#define INIT_ENEMYOT_LIST    1
#define INIT_BAG_ITEM_LIST   2
#define INIT_OTHER_ITEM_LIST 3
#define INIT_PLAYEROT_LIST   4
#define INIT_MON_LIST        5

// wTimeOfDay::
enum {
	MORN_F     = 0,
	DAY_F      = 1,
	NITE_F     = 2,
	DARKNESS_F = 3
} NUM_DAYTIMES;

#define MORN     1 << MORN_F
#define DAY      1 << DAY_F
#define NITE     1 << NITE_F
#define DARKNESS 1 << DARKNESS_F

#define ANYTIME MORN | DAY | NITE

// wTimeOfDayPalset::
#define DARKNESS_PALSET (DARKNESS_F << 6) | (DARKNESS_F << 4) | (DARKNESS_F << 2) | DARKNESS_F

// wBattleAnimFlags::
enum {
	BATTLEANIM_STOP_F          = 0,
	BATTLEANIM_IN_SUBROUTINE_F = 1,
	BATTLEANIM_IN_LOOP_F       = 2,
	BATTLEANIM_KEEPSPRITES_F   = 3
};

// wPlayerSpriteSetupFlags::
#define PLAYERSPRITESETUP_FACING_MASK       0b11
#define PLAYERSPRITESETUP_FEMALE_TO_MALE_F  2
#define PLAYERSPRITESETUP_CUSTOM_FACING_F   5
#define PLAYERSPRITESETUP_SKIP_RELOAD_GFX_F 6
#define PLAYERSPRITESETUP_RESET_ACTION_F    7

// wMapStatus::
enum {
	MAPSTATUS_START  = 0,
	MAPSTATUS_ENTER  = 1,
	MAPSTATUS_HANDLE = 2,
	MAPSTATUS_DONE   = 3
};

// wMapEventStatus::
enum {
	MAPEVENTS_ON  = 0,
	MAPEVENTS_OFF = 1
};
// wScriptFlags::
#define SCRIPT_RUNNING 2

// wScriptMode::
enum {
	SCRIPT_OFF = 0,
	SCRIPT_READ,
	SCRIPT_WAIT_MOVEMENT,
	SCRIPT_WAIT
};
// wSpawnAfterChampion::
#define SPAWN_LANCE 1
#define SPAWN_RED   2

// wCurDay::
enum {
	SUNDAY    = 0,
	MONDAY    = 1,
	TUESDAY   = 2,
	WEDNESDAY = 3,
	THURSDAY  = 4,
	FRIDAY    = 5,
	SATURDAY  = 6
};
// wStatusFlags::
enum {
	STATUSFLAGS_POKEDEX_F                  = 0,
	STATUSFLAGS_UNOWN_DEX_F                = 1,
	STATUSFLAGS_FLASH_F                    = 2,
	STATUSFLAGS_CAUGHT_POKERUS_F           = 3,
	STATUSFLAGS_ROCKET_SIGNAL_F            = 4,
	STATUSFLAGS_NO_WILD_ENCOUNTERS_F       = 5,
	STATUSFLAGS_HALL_OF_FAME_F             = 6
};
// wStatusFlags2::
enum {
	STATUSFLAGS2_ROCKETS_IN_RADIO_TOWER_F = 0,
	STATUSFLAGS2_SAFARI_GAME_F            = 1,
	STATUSFLAGS2_BUG_CONTEST_TIMER_F      = 2,
	STATUSFLAGS2_UNUSED_3_F               = 3,
	STATUSFLAGS2_BIKE_SHOP_CALL_F         = 4,
	STATUSFLAGS2_UNUSED_5_F               = 5,
	STATUSFLAGS2_REACHED_GOLDENROD_F      = 6,
	STATUSFLAGS2_ROCKETS_IN_MAHOGANY_F    = 7
};
// wMomSavingMoney::
#define MOM_SAVING_SOME_MONEY_F 0
#define MOM_SAVING_HALF_MONEY_F 1
#define MOM_SAVING_ALL_MONEY_F  2
#define MOM_ACTIVE_F            7

#define MOM_SAVING_MONEY_MASK (1 << MOM_SAVING_SOME_MONEY_F) | (1 << MOM_SAVING_HALF_MONEY_F) | (1 << MOM_SAVING_ALL_MONEY_F)

// wJohtoBadges::
enum {
	ZEPHYRBADGE = 0,
	HIVEBADGE,
	PLAINBADGE,
	FOGBADGE,
	MINERALBADGE,
	STORMBADGE,
	GLACIERBADGE,
	RISINGBADGE
} NUM_JOHTO_BADGES;

// wKantoBadges::
enum {
	BOULDERBADGE = 0,
	CASCADEBADGE,
	THUNDERBADGE,
	RAINBOWBADGE,
	SOULBADGE,
	MARSHBADGE,
	VOLCANOBADGE,
	EARTHBADGE
} NUM_KANTO_BADGES;
#define NUM_BADGES NUM_JOHTO_BADGES + NUM_KANTO_BADGES

// wPokegearFlags::
enum {
	POKEGEAR_MAP_CARD_F   = 0,
	POKEGEAR_RADIO_CARD_F = 1,
	POKEGEAR_PHONE_CARD_F = 2,
	POKEGEAR_EXPN_CARD_F  = 3,
	POKEGEAR_OBTAINED_F   = 7
};
// wWhichRegisteredItem::
#define REGISTERED_POCKET 0b11000000
#define REGISTERED_NUMBER 0b00111111

// wPlayerState::
#define PLAYER_NORMAL    0
#define PLAYER_BIKE      1
#define PLAYER_SKATE     2
#define PLAYER_SURF      4
#define PLAYER_SURF_PIKA 8

// wBikeFlags::
enum {
	BIKEFLAGS_STRENGTH_ACTIVE_F = 0,
	BIKEFLAGS_ALWAYS_ON_BIKE_F  = 1,
	BIKEFLAGS_DOWNHILL_F        = 2
};
// wDailyFlags1::
enum {
	DAILYFLAGS1_KURT_MAKING_BALLS_F             = 0,
	DAILYFLAGS1_BUG_CONTEST_F                   = 1,
	DAILYFLAGS1_SWARM_F                         = 2,
	DAILYFLAGS1_TIME_CAPSULE_F                  = 3,
	DAILYFLAGS1_ALL_FRUIT_TREES_F               = 4,
	DAILYFLAGS1_GOT_SHUCKIE_TODAY_F             = 5,
	DAILYFLAGS1_GOLDENROD_UNDERGROUND_BARGAIN_F = 6,
	DAILYFLAGS1_TRAINER_HOUSE_F                 = 7
};
// wDailyFlags2::
enum {
	DAILYFLAGS2_MT_MOON_SQUARE_CLEFAIRY_F           = 0,
	DAILYFLAGS2_UNION_CAVE_LAPRAS_F                 = 1,
	DAILYFLAGS2_GOLDENROD_UNDERGROUND_GOT_HAIRCUT_F = 2,
	DAILYFLAGS2_GOLDENROD_DEPT_STORE_TM27_RETURN_F  = 3,
	DAILYFLAGS2_DAISYS_GROOMING_F                   = 4,
	DAILYFLAGS2_INDIGO_PLATEAU_RIVAL_FIGHT_F        = 5
};
// wLuckyNumberShowFlag::
#define LUCKYNUMBERSHOW_GAME_OVER_F 0

// wDayCareMan::
#define DAYCAREMAN_HAS_MON_F         0
#define DAYCAREMAN_MONS_COMPATIBLE_F 5
#define DAYCAREMAN_HAS_EGG_F         6

// wDayCareLady::
#define DAYCARELADY_HAS_MON_F        0

// shared flag between wDayCareMan and wDayCareLady
#define DAYCARE_INTRO_SEEN_F         7
