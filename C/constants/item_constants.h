// item ids
// indexes for:
// - ItemNames (see data/items/names.asm)
// - ItemDescriptions (see data/items/descriptions.asm)
// - ItemAttributes (see data/items/attributes.asm)
// - ItemEffects (see engine/items/item_effects.asm)
enum {
    NO_ITEM,
    MASTER_BALL,
    ULTRA_BALL,
    BRIGHTPOWDER,
    GREAT_BALL,
    POKE_BALL,
    TOWN_MAP,
    BICYCLE,
    MOON_STONE,
    ANTIDOTE,
    BURN_HEAL,
    ICE_HEAL,
    AWAKENING,
    PARLYZ_HEAL,
    FULL_RESTORE,
    MAX_POTION,
    HYPER_POTION,
    SUPER_POTION,
    POTION,
    ESCAPE_ROPE,
    REPEL,
    MAX_ELIXER,
    FIRE_STONE,
    THUNDERSTONE,
    WATER_STONE,
    ITEM_19,
    HP_UP,
    PROTEIN,
    IRON,
    CARBOS,
    LUCKY_PUNCH,
    CALCIUM,
    RARE_CANDY,
    X_ACCURACY,
    LEAF_STONE,
    METAL_POWDER,
    NUGGET,
    POKE_DOLL,
    FULL_HEAL,
    REVIVE,
    MAX_REVIVE,
    GUARD_SPEC,
    SUPER_REPEL,
    MAX_REPEL,
    DIRE_HIT,
    ITEM_2D,
    FRESH_WATER,
    SODA_POP,
    LEMONADE,
    X_ATTACK,
    ITEM_32,
    X_DEFEND,
    X_SPEED,
    X_SPECIAL,
    COIN_CASE,
    ITEMFINDER,
    POKE_FLUTE,
    EXP_SHARE,
    OLD_ROD,
    GOOD_ROD,
    SILVER_LEAF,
    SUPER_ROD,
    PP_UP,
    ETHER,
    MAX_ETHER,
    ELIXER,
    RED_SCALE,
    SECRETPOTION,
    S_S_TICKET,
    MYSTERY_EGG,
    ITEM_46,
    SILVER_WING,
    MOOMOO_MILK,
    QUICK_CLAW,
    PSNCUREBERRY,
    GOLD_LEAF,
    SOFT_SAND,
    SHARP_BEAK,
    PRZCUREBERRY,
    BURNT_BERRY,
    ICE_BERRY,
    POISON_BARB,
    KINGS_ROCK,
    BITTER_BERRY,
    MINT_BERRY,
    RED_APRICORN,
    TINYMUSHROOM,
    BIG_MUSHROOM,
    SILVERPOWDER,
    BLU_APRICORN,
    ITEM_5A,
    AMULET_COIN,
    YLW_APRICORN,
    GRN_APRICORN,
    CLEANSE_TAG,
    MYSTIC_WATER,
    TWISTEDSPOON,
    WHT_APRICORN,
    BLACKBELT_I,
    BLK_APRICORN,
    ITEM_64,
    PNK_APRICORN,
    BLACKGLASSES,
    SLOWPOKETAIL,
    PINK_BOW,
    STICK,
    SMOKE_BALL,
    NEVERMELTICE,
    MAGNET,
    MIRACLEBERRY,
    PEARL,
    BIG_PEARL,
    EVERSTONE,
    SPELL_TAG,
    RAGECANDYBAR,
    ITEM_73,
    ITEM_74,
    MIRACLE_SEED,
    THICK_CLUB,
    FOCUS_BAND,
    ITEM_78,
    ENERGYPOWDER,
    ENERGY_ROOT,
    HEAL_POWDER,
    REVIVAL_HERB,
    HARD_STONE,
    LUCKY_EGG,
    CARD_KEY,
    MACHINE_PART,
    ITEM_81,
    LOST_ITEM,
    STARDUST,
    STAR_PIECE,
    BASEMENT_KEY,
    PASS,
    ITEM_87,
    ITEM_88,
    ITEM_89,
    CHARCOAL,
    BERRY_JUICE,
    SCOPE_LENS,
    ITEM_8D,
    ITEM_8E,
    METAL_COAT,
    DRAGON_FANG,
    ITEM_91,
    LEFTOVERS,
    ITEM_93,
    ITEM_94,
    ITEM_95,
    MYSTERYBERRY,
    DRAGON_SCALE,
    BERSERK_GENE,
    ITEM_99,
    ITEM_9A,
    ITEM_9B,
    SACRED_ASH,
    HEAVY_BALL,
    FLOWER_MAIL,
    LEVEL_BALL,
    LURE_BALL,
    FAST_BALL,
    ITEM_A2,
    LIGHT_BALL,
    FRIEND_BALL,
    MOON_BALL,
    LOVE_BALL,
    NORMAL_BOX,
    GORGEOUS_BOX,
    SUN_STONE,
    POLKADOT_BOW,
    ITEM_AB,
    UP_GRADE,
    BERRY,
    GOLD_BERRY,
    SQUIRTBOTTLE,
    ITEM_B0,
    PARK_BALL,
    RAINBOW_WING,
    ITEM_B3,
    BRICK_PIECE,
    SURF_MAIL,
    LITEBLUEMAIL,
    PORTRAITMAIL,
    LOVELY_MAIL,
    EON_MAIL,
    MORPH_MAIL,
    BLUESKY_MAIL,
    MUSIC_MAIL,
    MIRAGE_MAIL,
    ITEM_BE,
};

#define NUM_ITEMS ITEM_BE

__tmhm_value__ = 1

add_tmnum: MACRO
\1_TMNUM EQU __tmhm_value__
__tmhm_value__ = __tmhm_value__ + 1
ENDM

add_tm: MACRO
; Defines three constants:
; - TM_\1: the item id, starting at $bf
; - \1_TMNUM: the learnable TM/HM flag, starting at 1
; - TM##_MOVE: alias for the move id, equal to the value of \1
	const TM_\1
TM{02d:__tmhm_value__}_MOVE = \1
	add_tmnum \1
ENDM

// see data/moves/tmhm_moves.asm for moves
#define TM01 NUM_ITEMS + 1
	add_tm DYNAMICPUNCH // bf
	add_tm HEADBUTT     // c0
	add_tm CURSE        // c1
	add_tm ROLLOUT      // c2
	const ITEM_C3       // c3
	add_tm ROAR         // c4
	add_tm TOXIC        // c5
	add_tm ZAP_CANNON   // c6
	add_tm ROCK_SMASH   // c7
	add_tm PSYCH_UP     // c8
	add_tm HIDDEN_POWER // c9
	add_tm SUNNY_DAY    // ca
	add_tm SWEET_SCENT  // cb
	add_tm SNORE        // cc
	add_tm BLIZZARD     // cd
	add_tm HYPER_BEAM   // ce
	add_tm ICY_WIND     // cf
	add_tm PROTECT      // d0
	add_tm RAIN_DANCE   // d1
	add_tm GIGA_DRAIN   // d2
	add_tm ENDURE       // d3
	add_tm FRUSTRATION  // d4
	add_tm SOLARBEAM    // d5
	add_tm IRON_TAIL    // d6
	add_tm DRAGONBREATH // d7
	add_tm THUNDER      // d8
	add_tm EARTHQUAKE   // d9
	add_tm RETURN       // da
	add_tm DIG          // db
	const ITEM_DC       // dc
	add_tm PSYCHIC_M    // dd
	add_tm SHADOW_BALL  // de
	add_tm MUD_SLAP     // df
	add_tm DOUBLE_TEAM  // e0
	add_tm ICE_PUNCH    // e1
	add_tm SWAGGER      // e2
	add_tm SLEEP_TALK   // e3
	add_tm SLUDGE_BOMB  // e4
	add_tm SANDSTORM    // e5
	add_tm FIRE_BLAST   // e6
	add_tm SWIFT        // e7
	add_tm DEFENSE_CURL // e8
	add_tm THUNDERPUNCH // e9
	add_tm DREAM_EATER  // ea
	add_tm DETECT       // eb
	add_tm REST         // ec
	add_tm ATTRACT      // ed
	add_tm THIEF        // ee
	add_tm STEEL_WING   // ef
	add_tm FIRE_PUNCH   // f0
	add_tm FURY_CUTTER  // f1
	add_tm NIGHTMARE    // f2
#define NUM_TMS __tmhm_value__ - 1

add_hm: MACRO
// Defines three constants:
// - HM_\1: the item id, starting at 0xf3
// - \1_TMNUM: the learnable TM/HM flag, starting at 51
#define // - HM##_MOVE: alias for the move id,al to the value of \1
	const HM_\1
HM_VALUE = __tmhm_value__ - NUM_TMS
HM{02d:HM_VALUE}_MOVE = \1
	add_tmnum \1
ENDM

#define HM01 const_value
	add_hm CUT          // f3
	add_hm FLY          // f4
	add_hm SURF         // f5
	add_hm STRENGTH     // f6
	add_hm FLASH        // f7
	add_hm WHIRLPOOL    // f8
	add_hm WATERFALL    // f9
#define NUM_HMS __tmhm_value__ - NUM_TMS - 1

#define NUM_TM_HM NUM_TMS + NUM_HMS

	const ITEM_FA       // fa

#define USE_SCRIPT_VAR 0x00
#define ITEM_FROM_MEM  0xff

// leftovers from red
#define SAFARI_BALL    0x08 // MOON_STONE
#define MOON_STONE_RED 0x0a // BURN_HEAL
#define FULL_HEAL_RED  0x34 // X_SPEED
