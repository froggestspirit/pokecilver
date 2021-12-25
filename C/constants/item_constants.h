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

// see data/moves/tmhm_moves.asm for moves

#define TM_BASE __COUNTER__
#define TM_ITEM_OFFSET NUM_ITEMS

#define add_tm(tmNum, tmName)   enum {\
                                    tmName##_TMNUM = __COUNTER__ - TM_BASE,\
                                    TM_##tmName = tmName##_TMNUM + TM_ITEM_OFFSET,\
                                    tmNum##_MOVE = TM_##tmName,\
                                };

    add_tm(TM01, DYNAMICPUNCH)
	add_tm(TM02, HEADBUTT)
	add_tm(TM03, CURSE)
	add_tm(TM04, ROLLOUT)
#define ITEM_C3 0xC3
#define TM_ITEM_OFFSET NUM_ITEMS + 1
	add_tm(TM05, ROAR)         // c4
	add_tm(TM06, TOXIC)        // c5
	add_tm(TM07, ZAP_CANNON)   // c6
	add_tm(TM08, ROCK_SMASH)   // c7
	add_tm(TM09, PSYCH_UP)     // c8
	add_tm(TM10, HIDDEN_POWER) // c9
	add_tm(TM11, SUNNY_DAY)    // ca
	add_tm(TM12, SWEET_SCENT)  // cb
	add_tm(TM13, SNORE)        // cc
	add_tm(TM14, BLIZZARD)     // cd
	add_tm(TM15, HYPER_BEAM)   // ce
	add_tm(TM16, ICY_WIND)     // cf
	add_tm(TM17, PROTECT)      // d0
	add_tm(TM18, RAIN_DANCE)   // d1
	add_tm(TM19, GIGA_DRAIN)   // d2
	add_tm(TM20, ENDURE)       // d3
	add_tm(TM21, FRUSTRATION)  // d4
	add_tm(TM22, SOLARBEAM)    // d5
	add_tm(TM23, IRON_TAIL)    // d6
	add_tm(TM24, DRAGONBREATH) // d7
	add_tm(TM25, THUNDER)      // d8
	add_tm(TM26, EARTHQUAKE)   // d9
	add_tm(TM27, RETURN)       // da
	add_tm(TM28, DIG)          // db
#define ITEM_DC 0xDC
#define TM_ITEM_OFFSET NUM_ITEMS + 2
	add_tm(TM29, PSYCHIC_M)    // dd
	add_tm(TM30, SHADOW_BALL)  // de
	add_tm(TM31, MUD_SLAP)     // df
	add_tm(TM32, DOUBLE_TEAM)  // e0
	add_tm(TM33, ICE_PUNCH)    // e1
	add_tm(TM34, SWAGGER)      // e2
	add_tm(TM35, SLEEP_TALK)   // e3
	add_tm(TM36, SLUDGE_BOMB)  // e4
	add_tm(TM37, SANDSTORM)    // e5
	add_tm(TM38, FIRE_BLAST)   // e6
	add_tm(TM39, SWIFT)        // e7
	add_tm(TM40, DEFENSE_CURL) // e8
	add_tm(TM41, THUNDERPUNCH) // e9
	add_tm(TM42, DREAM_EATER)  // ea
	add_tm(TM43, DETECT)       // eb
	add_tm(TM44, REST)         // ec
	add_tm(TM45, ATTRACT)      // ed
	add_tm(TM46, THIEF)        // ee
	add_tm(TM47, STEEL_WING)   // ef
	add_tm(TM48, FIRE_PUNCH)   // f0
	add_tm(TM49, FURY_CUTTER)  // f1
	add_tm(TM50, NIGHTMARE)    // f2

#define TM01 TM_DYNAMICPUNCH
#define NUM_TMS NIGHTMARE_TMNUM

#define add_hm(hmNum, hmName)   enum {\
                                    hmName##_TMNUM = __COUNTER__ - TM_BASE,\
                                    HM_##hmName = hmName##_TMNUM + TM_ITEM_OFFSET,\
                                    hmNum##_MOVE = HM_##hmName,\
                                };

	add_hm(HM01, CUT)          // f3
	add_hm(HM02, FLY)          // f4
	add_hm(HM03, SURF)         // f5
	add_hm(HM04, STRENGTH)     // f6
	add_hm(HM05, FLASH)        // f7
	add_hm(HM06, WHIRLPOOL)    // f8
	add_hm(HM07, WATERFALL)    // f9

#define HM01 const_value
#define NUM_HMS WATERFALL_TMNUM - NUM_TMS

#define NUM_TM_HM NUM_TMS + NUM_HMS

#define ITEM_FA 0xFA

#define USE_SCRIPT_VAR 0x00
#define ITEM_FROM_MEM  0xff

// leftovers from red
#define SAFARI_BALL    0x08 // MOON_STONE
#define MOON_STONE_RED 0x0a // BURN_HEAL
#define FULL_HEAL_RED  0x34 // X_SPEED
