// battle_anim_struct members (see macros/wram.asm)
enum {
    BATTLEANIMSTRUCT_INDEX,
    BATTLEANIMSTRUCT_OAMFLAGS,
    BATTLEANIMSTRUCT_FIX_Y,
    BATTLEANIMSTRUCT_FRAMESET_ID,
    BATTLEANIMSTRUCT_FUNCTION,
    BATTLEANIMSTRUCT_PALETTE,
    BATTLEANIMSTRUCT_TILEID,
    BATTLEANIMSTRUCT_XCOORD,
    BATTLEANIMSTRUCT_YCOORD,
    BATTLEANIMSTRUCT_XOFFSET,
    BATTLEANIMSTRUCT_YOFFSET,
    BATTLEANIMSTRUCT_PARAM,
    BATTLEANIMSTRUCT_DURATION,
    BATTLEANIMSTRUCT_FRAME,
    BATTLEANIMSTRUCT_JUMPTABLE_INDEX,
    BATTLEANIMSTRUCT_VAR1,
    BATTLEANIMSTRUCT_VAR2,
};
#define BATTLEANIMOBJ_LENGTH BATTLEANIMSTRUCT_TILEID

#define BATTLEANIMSTRUCT_LENGTH (BATTLEANIMSTRUCT_VAR2 + 8)
#define NUM_ANIM_OBJECTS 10 // see wActiveAnimObjects

// wBattleAnimTileDict size (see wram.asm)
#define NUM_BATTLEANIMTILEDICT_ENTRIES 5

// Start tile for battle animation graphics
#define BATTLEANIM_BASE_TILE (7 * 7)  // Maximum size of a pokemon picture

// BattleAnimObjects indexes (see data/battle_anims/objects.asm)
enum {
    ANIM_OBJ_HIT_BIG_YFIX,
    ANIM_OBJ_HIT_YFIX,
    ANIM_OBJ_HIT_SMALL_YFIX,
    ANIM_OBJ_HIT_BIG,
    ANIM_OBJ_HIT,
    ANIM_OBJ_HIT_SMALL,
    ANIM_OBJ_PUNCH,
    ANIM_OBJ_KICK,
    ANIM_OBJ_PALM,
    ANIM_OBJ_FANG,
    ANIM_OBJ_PUNCH_SHAKE,
    ANIM_OBJ_EMBER,
    ANIM_OBJ_DRAGON_RAGE,
    ANIM_OBJ_FLAMETHROWER,
    ANIM_OBJ_FIRE_SPIN,
    ANIM_OBJ_FIRE_BLAST,
    ANIM_OBJ_BURNED,
    ANIM_OBJ_BLIZZARD,
    ANIM_OBJ_ICE,
    ANIM_OBJ_ICE_BEAM,
    ANIM_OBJ_RAZOR_LEAF,
    ANIM_OBJ_POKE_BALL,
    ANIM_OBJ_POKE_BALL_BLOCKED,
    ANIM_OBJ_EXPLOSION1,
    ANIM_OBJ_EXPLOSION2,
    ANIM_OBJ_ACID,
    ANIM_OBJ_SLUDGE,
    ANIM_OBJ_BETA_BALL_POOF,
    ANIM_OBJ_BALL_POOF,
    ANIM_OBJ_BIG_ROCK,
    ANIM_OBJ_SMALL_ROCK,
    ANIM_OBJ_STRENGTH,
    ANIM_OBJ_SEISMIC_TOSS,
    ANIM_OBJ_BUBBLE,
    ANIM_OBJ_SURF,
    ANIM_OBJ_SING,
    ANIM_OBJ_WATER_GUN,
    ANIM_OBJ_HYDRO_PUMP,
    ANIM_OBJ_POWDER,
    ANIM_OBJ_BEAM,
    ANIM_OBJ_BEAM_TIP,
    ANIM_OBJ_ICE_BUILDUP,
    ANIM_OBJ_FROZEN,
    ANIM_OBJ_MASTER_BALL_SPARKLE,
    ANIM_OBJ_RECOVER,
    ANIM_OBJ_THUNDER1,
    ANIM_OBJ_THUNDER2,
    ANIM_OBJ_THUNDER3,
    ANIM_OBJ_THUNDER_WAVE,
    ANIM_OBJ_SPARKS_CIRCLE_BIG,
    ANIM_OBJ_THUNDERBOLT_BALL,
    ANIM_OBJ_SPARKS_CIRCLE,
    ANIM_OBJ_THUNDERSHOCK_BALL,
    ANIM_OBJ_CLAMP,
    ANIM_OBJ_BITE,
    ANIM_OBJ_CUT_DOWN_LEFT,
    ANIM_OBJ_CUT_DOWN_RIGHT,
    ANIM_OBJ_CUT_UP_RIGHT,
    ANIM_OBJ_CUT_LONG_DOWN_LEFT,
    ANIM_OBJ_CUT_LONG_DOWN_RIGHT,
    ANIM_OBJ_SOLAR_BEAM_CHARGE,
    ANIM_OBJ_ABSORB_CENTER,
    ANIM_OBJ_GUST,
    ANIM_OBJ_VINE_WHIP1,
    ANIM_OBJ_VINE_WHIP2,
    ANIM_OBJ_RAZOR_WIND1,
    ANIM_OBJ_RAZOR_WIND2,
    ANIM_OBJ_SONICBOOM_JP,
    ANIM_OBJ_WARP,
    ANIM_OBJ_ABSORB,
    ANIM_OBJ_EGG,
    ANIM_OBJ_FOCUS,
    ANIM_OBJ_BIND1,
    ANIM_OBJ_BIND2,
    ANIM_OBJ_LEECH_SEED,
    ANIM_OBJ_SOUND,
    ANIM_OBJ_WAVE,
    ANIM_OBJ_CONFUSE_RAY,
    ANIM_OBJ_LEER,
    ANIM_OBJ_LEER_TIP,
    ANIM_OBJ_SCREEN,
    ANIM_OBJ_HARDEN,
    ANIM_OBJ_CHICK,
    ANIM_OBJ_AMNESIA,
    ANIM_OBJ_ASLEEP,
    ANIM_OBJ_SKULL,
    ANIM_OBJ_DIG_SAND,
    ANIM_OBJ_DIG_PILE,
    ANIM_OBJ_SAND,
    ANIM_OBJ_PARALYZED,
    ANIM_OBJ_STRING_SHOT,
    ANIM_OBJ_HAZE,
    ANIM_OBJ_MIST,
    ANIM_OBJ_SMOG,
    ANIM_OBJ_POISON_GAS,
    ANIM_OBJ_HORN,
    ANIM_OBJ_NEEDLE,
    ANIM_OBJ_PETAL_DANCE,
    ANIM_OBJ_SLUDGE_BOMB,
    ANIM_OBJ_PAY_DAY,
    ANIM_OBJ_SONICBOOM_JP_UNUSED,
    ANIM_OBJ_MIMIC,
    ANIM_OBJ_ATTRACT,
    ANIM_OBJ_BONEMERANG,
    ANIM_OBJ_BONE_CLUB,
    ANIM_OBJ_BONE_RUSH,
    ANIM_OBJ_SWIFT,
    ANIM_OBJ_KINESIS,
    ANIM_OBJ_FLASH,
    ANIM_OBJ_SHINY,
    ANIM_OBJ_SKY_ATTACK,
    ANIM_OBJ_LICK,
    ANIM_OBJ_WITHDRAW,
    ANIM_OBJ_DRAIN,
    ANIM_OBJ_GROWTH,
    ANIM_OBJ_CONVERSION2,
    ANIM_OBJ_SMOKE,
    ANIM_OBJ_SMOKESCREEN,
    ANIM_OBJ_SWORDS_DANCE,
    ANIM_OBJ_SPEED_LINE,
    ANIM_OBJ_SHARPEN,
    ANIM_OBJ_DEFENSE_CURL,
    ANIM_OBJ_METRONOME_HAND,
    ANIM_OBJ_METRONOME_SPARKLE,
    ANIM_OBJ_DISABLE,
    ANIM_OBJ_AGILITY,
    ANIM_OBJ_HEART,
    ANIM_OBJ_FLAME_WHEEL,
    ANIM_OBJ_SACRED_FIRE,
    ANIM_OBJ_COTTON_SPORE,
    ANIM_OBJ_MILK_DRINK,
    ANIM_OBJ_ANGER,
    ANIM_OBJ_HEAL_BELL,
    ANIM_OBJ_HEAL_BELL_NOTE,
    ANIM_OBJ_BATON_PASS,
    ANIM_OBJ_LOCK_ON,
    ANIM_OBJ_MIND_READER,
    ANIM_OBJ_SAFEGUARD,
    ANIM_OBJ_PROTECT,
    ANIM_OBJ_THIEF,
    ANIM_OBJ_OCTAZOOKA,
    ANIM_OBJ_PRESENT,
    ANIM_OBJ_SPIKES,
    ANIM_OBJ_POWDER_SNOW,
    ANIM_OBJ_DRAGONBREATH,
    ANIM_OBJ_CONVERSION,
    ANIM_OBJ_SPIDER_WEB,
    ANIM_OBJ_CAKE_UNUSED,
    ANIM_OBJ_NIGHTMARE,
    ANIM_OBJ_IN_NIGHTMARE,
    ANIM_OBJ_LOVELY_KISS,
    ANIM_OBJ_SWEET_KISS,
    ANIM_OBJ_SKETCH,
    ANIM_OBJ_ENCORE_HAND,
    ANIM_OBJ_ENCORE_STAR,
    ANIM_OBJ_DESTINY_BOND,
    ANIM_OBJ_MORNING_SUN,
    ANIM_OBJ_GLIMMER,
    ANIM_OBJ_MOONLIGHT,
    ANIM_OBJ_HIDDEN_POWER,
    ANIM_OBJ_CROSS_CHOP1,
    ANIM_OBJ_CROSS_CHOP2,
    ANIM_OBJ_SANDSTORM,
    ANIM_OBJ_ZAP_CANNON,
    ANIM_OBJ_SPITE,
    ANIM_OBJ_CURSE,
    ANIM_OBJ_PERISH_SONG,
    ANIM_OBJ_FORESIGHT,
    ANIM_OBJ_RAPID_SPIN,
    ANIM_OBJ_SWAGGER,
    ANIM_OBJ_BELLY_DRUM_HAND,
    ANIM_OBJ_BELLY_DRUM_NOTE,
    ANIM_OBJ_MEAN_LOOK,
    ANIM_OBJ_BETA_PURSUIT,
    ANIM_OBJ_SHOOTING_SPARKLE,
    ANIM_OBJ_RAIN,
    ANIM_OBJ_B0,
    ANIM_OBJ_PSYCH_UP,
    ANIM_OBJ_ANCIENTPOWER,
    ANIM_OBJ_AEROBLAST,
    ANIM_OBJ_SHADOW_BALL,
    ANIM_OBJ_ROCK_SMASH,
    ANIM_OBJ_FLOWER,
    ANIM_OBJ_COTTON,
    ANIM_OBJ_ENEMYFEET_1ROW,
    ANIM_OBJ_PLAYERHEAD_1ROW,
    ANIM_OBJ_ENEMYFEET_2ROW,
    ANIM_OBJ_PLAYERHEAD_2ROW,
    NUM_ANIM_OBJS,
};

// DoBattleAnimFrame arguments (see engine/battle_anims/functions.asm)
enum {
    BATTLEANIMFUNC_NULL,
    BATTLEANIMFUNC_USER_TO_TARGET,
    BATTLEANIMFUNC_USER_TO_TARGET_DISAPPEAR,
    BATTLEANIMFUNC_MOVE_IN_CIRCLE,
    BATTLEANIMFUNC_WAVE_TO_TARGET,
    BATTLEANIMFUNC_THROW_TO_TARGET,
    BATTLEANIMFUNC_THROW_TO_TARGET_DISAPPEAR,
    BATTLEANIMFUNC_DROP,
    BATTLEANIMFUNC_USER_TO_TARGET_SPIN,
    BATTLEANIMFUNC_SHAKE,
    BATTLEANIMFUNC_FIRE_BLAST,
    BATTLEANIMFUNC_RAZOR_LEAF,
    BATTLEANIMFUNC_BUBBLE,
    BATTLEANIMFUNC_SURF,
    BATTLEANIMFUNC_SING,
    BATTLEANIMFUNC_WATER_GUN,
    BATTLEANIMFUNC_EMBER,
    BATTLEANIMFUNC_POWDER,
    BATTLEANIMFUNC_POKEBALL,
    BATTLEANIMFUNC_POKEBALL_BLOCKED,
    BATTLEANIMFUNC_RECOVER,
    BATTLEANIMFUNC_THUNDER_WAVE,
    BATTLEANIMFUNC_CLAMP_ENCORE,
    BATTLEANIMFUNC_BITE,
    BATTLEANIMFUNC_SOLAR_BEAM,
    BATTLEANIMFUNC_GUST,
    BATTLEANIMFUNC_RAZOR_WIND,
    BATTLEANIMFUNC_KICK,
    BATTLEANIMFUNC_ABSORB,
    BATTLEANIMFUNC_EGG,
    BATTLEANIMFUNC_MOVE_UP,
    BATTLEANIMFUNC_WRAP,
    BATTLEANIMFUNC_LEECH_SEED,
    BATTLEANIMFUNC_SOUND,
    BATTLEANIMFUNC_CONFUSE_RAY,
    BATTLEANIMFUNC_DIZZY,
    BATTLEANIMFUNC_AMNESIA,
    BATTLEANIMFUNC_FLOAT_UP,
    BATTLEANIMFUNC_DIG,
    BATTLEANIMFUNC_STRING,
    BATTLEANIMFUNC_PARALYZED,
    BATTLEANIMFUNC_SPIRAL_DESCENT,
    BATTLEANIMFUNC_POISON_GAS,
    BATTLEANIMFUNC_HORN,
    BATTLEANIMFUNC_NEEDLE,
    BATTLEANIMFUNC_PETAL_DANCE,
    BATTLEANIMFUNC_THIEF_PAYDAY,
    BATTLEANIMFUNC_ABSORB_CIRCLE,
    BATTLEANIMFUNC_BONEMERANG,
    BATTLEANIMFUNC_SHINY,
    BATTLEANIMFUNC_SKY_ATTACK,
    BATTLEANIMFUNC_GROWTH_SWORDS_DANCE,
    BATTLEANIMFUNC_SMOKE_FLAME_WHEEL,
    BATTLEANIMFUNC_PRESENT_SMOKESCREEN,
    BATTLEANIMFUNC_STRENGTH_SEISMIC_TOSS,
    BATTLEANIMFUNC_SPEED_LINE,
    BATTLEANIMFUNC_SLUDGE,
    BATTLEANIMFUNC_METRONOME_HAND,
    BATTLEANIMFUNC_METRONOME_SPARKLE_SKETCH,
    BATTLEANIMFUNC_AGILITY,
    BATTLEANIMFUNC_SACRED_FIRE,
    BATTLEANIMFUNC_SAFEGUARD_PROTECT,
    BATTLEANIMFUNC_LOCK_ON_MIND_READER,
    BATTLEANIMFUNC_SPIKES,
    BATTLEANIMFUNC_HEAL_BELL_NOTES,
    BATTLEANIMFUNC_BATON_PASS,
    BATTLEANIMFUNC_CONVERSION,
    BATTLEANIMFUNC_ENCORE_BELLY_DRUM,
    BATTLEANIMFUNC_SWAGGER_MORNING_SUN,
    BATTLEANIMFUNC_HIDDEN_POWER,
    BATTLEANIMFUNC_CURSE,
    BATTLEANIMFUNC_PERISH_SONG,
    BATTLEANIMFUNC_RAPID_SPIN,
    BATTLEANIMFUNC_BETA_PURSUIT,
    BATTLEANIMFUNC_RAIN_SANDSTORM,
    BATTLEANIMFUNC_ANIM_OBJ_B0,
    BATTLEANIMFUNC_PSYCH_UP,
    BATTLEANIMFUNC_ANCIENT_POWER,
    BATTLEANIMFUNC_ROCK_SMASH,
    BATTLEANIMFUNC_COTTON,
    NUM_BATTLEANIMFUNCS,
};

// BattleAnimFrameData indexes (see data/battle_anims/framesets.asm)
enum {
    BATTLEANIMFRAMESET_00,
    BATTLEANIMFRAMESET_01,
    BATTLEANIMFRAMESET_02,
    BATTLEANIMFRAMESET_03,
    BATTLEANIMFRAMESET_04,
    BATTLEANIMFRAMESET_05,
    BATTLEANIMFRAMESET_06,
    BATTLEANIMFRAMESET_07,
    BATTLEANIMFRAMESET_08,
    BATTLEANIMFRAMESET_09,
    BATTLEANIMFRAMESET_0A,
    BATTLEANIMFRAMESET_0B,
    BATTLEANIMFRAMESET_0C,
    BATTLEANIMFRAMESET_0D,
    BATTLEANIMFRAMESET_0E,
    BATTLEANIMFRAMESET_0F,
    BATTLEANIMFRAMESET_10,
    BATTLEANIMFRAMESET_11,
    BATTLEANIMFRAMESET_12,
    BATTLEANIMFRAMESET_13,
    BATTLEANIMFRAMESET_14,
    BATTLEANIMFRAMESET_15,
    BATTLEANIMFRAMESET_16,
    BATTLEANIMFRAMESET_17,
    BATTLEANIMFRAMESET_18,
    BATTLEANIMFRAMESET_19,
    BATTLEANIMFRAMESET_1A,
    BATTLEANIMFRAMESET_1B,
    BATTLEANIMFRAMESET_1C,
    BATTLEANIMFRAMESET_1D,
    BATTLEANIMFRAMESET_1E,
    BATTLEANIMFRAMESET_1F,
    BATTLEANIMFRAMESET_20,
    BATTLEANIMFRAMESET_21,
    BATTLEANIMFRAMESET_22,
    BATTLEANIMFRAMESET_23,
    BATTLEANIMFRAMESET_24,
    BATTLEANIMFRAMESET_25,
    BATTLEANIMFRAMESET_26,
    BATTLEANIMFRAMESET_27,
    BATTLEANIMFRAMESET_28,
    BATTLEANIMFRAMESET_29,
    BATTLEANIMFRAMESET_2A,
    BATTLEANIMFRAMESET_2B,
    BATTLEANIMFRAMESET_2C,
    BATTLEANIMFRAMESET_2D,
    BATTLEANIMFRAMESET_2E,
    BATTLEANIMFRAMESET_2F,
    BATTLEANIMFRAMESET_30,
    BATTLEANIMFRAMESET_31,
    BATTLEANIMFRAMESET_32,
    BATTLEANIMFRAMESET_33,
    BATTLEANIMFRAMESET_34,
    BATTLEANIMFRAMESET_35,
    BATTLEANIMFRAMESET_36,
    BATTLEANIMFRAMESET_37,
    BATTLEANIMFRAMESET_38,
    BATTLEANIMFRAMESET_39,
    BATTLEANIMFRAMESET_3A,
    BATTLEANIMFRAMESET_3B,
    BATTLEANIMFRAMESET_3C,
    BATTLEANIMFRAMESET_3D,
    BATTLEANIMFRAMESET_3E,
    BATTLEANIMFRAMESET_3F,
    BATTLEANIMFRAMESET_40,
    BATTLEANIMFRAMESET_41,
    BATTLEANIMFRAMESET_42,
    BATTLEANIMFRAMESET_43,
    BATTLEANIMFRAMESET_44,
    BATTLEANIMFRAMESET_45,
    BATTLEANIMFRAMESET_46,
    BATTLEANIMFRAMESET_47,
    BATTLEANIMFRAMESET_48,
    BATTLEANIMFRAMESET_49,
    BATTLEANIMFRAMESET_4A,
    BATTLEANIMFRAMESET_4B,
    BATTLEANIMFRAMESET_4C,
    BATTLEANIMFRAMESET_4D,
    BATTLEANIMFRAMESET_4E,
    BATTLEANIMFRAMESET_4F,
    BATTLEANIMFRAMESET_50,
    BATTLEANIMFRAMESET_51,
    BATTLEANIMFRAMESET_52,
    BATTLEANIMFRAMESET_53,
    BATTLEANIMFRAMESET_54,
    BATTLEANIMFRAMESET_55,
    BATTLEANIMFRAMESET_56,
    BATTLEANIMFRAMESET_57,
    BATTLEANIMFRAMESET_58,
    BATTLEANIMFRAMESET_59,
    BATTLEANIMFRAMESET_5A,
    BATTLEANIMFRAMESET_5B,
    BATTLEANIMFRAMESET_5C,
    BATTLEANIMFRAMESET_5D,
    BATTLEANIMFRAMESET_5E,
    BATTLEANIMFRAMESET_5F,
    BATTLEANIMFRAMESET_60,
    BATTLEANIMFRAMESET_61,
    BATTLEANIMFRAMESET_62,
    BATTLEANIMFRAMESET_63,
    BATTLEANIMFRAMESET_64,
    BATTLEANIMFRAMESET_65,
    BATTLEANIMFRAMESET_66,
    BATTLEANIMFRAMESET_67,
    BATTLEANIMFRAMESET_68,
    BATTLEANIMFRAMESET_69,
    BATTLEANIMFRAMESET_6A,
    BATTLEANIMFRAMESET_6B,
    BATTLEANIMFRAMESET_6C,
    BATTLEANIMFRAMESET_6D,
    BATTLEANIMFRAMESET_6E,
    BATTLEANIMFRAMESET_6F,
    BATTLEANIMFRAMESET_70,
    BATTLEANIMFRAMESET_71,
    BATTLEANIMFRAMESET_72,
    BATTLEANIMFRAMESET_73,
    BATTLEANIMFRAMESET_74,
    BATTLEANIMFRAMESET_75,
    BATTLEANIMFRAMESET_76,
    BATTLEANIMFRAMESET_77,
    BATTLEANIMFRAMESET_78,
    BATTLEANIMFRAMESET_79,
    BATTLEANIMFRAMESET_7A,
    BATTLEANIMFRAMESET_7B,
    BATTLEANIMFRAMESET_7C,
    BATTLEANIMFRAMESET_7D,
    BATTLEANIMFRAMESET_7E,
    BATTLEANIMFRAMESET_7F,
    BATTLEANIMFRAMESET_80,
    BATTLEANIMFRAMESET_81,
    BATTLEANIMFRAMESET_82,
    BATTLEANIMFRAMESET_83,
    BATTLEANIMFRAMESET_84,
    BATTLEANIMFRAMESET_85,
    BATTLEANIMFRAMESET_86,
    BATTLEANIMFRAMESET_87,
    BATTLEANIMFRAMESET_88,
    BATTLEANIMFRAMESET_89,
    BATTLEANIMFRAMESET_8A,
    BATTLEANIMFRAMESET_8B,
    BATTLEANIMFRAMESET_8C,
    BATTLEANIMFRAMESET_8D,
    BATTLEANIMFRAMESET_8E,
    BATTLEANIMFRAMESET_8F,
    BATTLEANIMFRAMESET_90,
    BATTLEANIMFRAMESET_91,
    BATTLEANIMFRAMESET_92,
    BATTLEANIMFRAMESET_93,
    BATTLEANIMFRAMESET_94,
    BATTLEANIMFRAMESET_95,
    BATTLEANIMFRAMESET_96,
    BATTLEANIMFRAMESET_97,
    BATTLEANIMFRAMESET_98,
    BATTLEANIMFRAMESET_99,
    BATTLEANIMFRAMESET_9A,
    BATTLEANIMFRAMESET_9B,
    BATTLEANIMFRAMESET_9C,
    BATTLEANIMFRAMESET_9D,
    BATTLEANIMFRAMESET_9E,
    BATTLEANIMFRAMESET_9F,
    BATTLEANIMFRAMESET_A0,
    BATTLEANIMFRAMESET_A1,
    BATTLEANIMFRAMESET_A2,
    BATTLEANIMFRAMESET_A3,
    BATTLEANIMFRAMESET_A4,
    BATTLEANIMFRAMESET_A5,
    BATTLEANIMFRAMESET_A6,
    BATTLEANIMFRAMESET_A7,
    BATTLEANIMFRAMESET_A8,
    BATTLEANIMFRAMESET_A9,
    BATTLEANIMFRAMESET_AA,
    BATTLEANIMFRAMESET_AB,
    BATTLEANIMFRAMESET_AC,
    BATTLEANIMFRAMESET_AD,
    BATTLEANIMFRAMESET_AE,
    BATTLEANIMFRAMESET_AF,
    BATTLEANIMFRAMESET_B0,
    BATTLEANIMFRAMESET_B1,
    BATTLEANIMFRAMESET_B2,
    BATTLEANIMFRAMESET_B3,
    BATTLEANIMFRAMESET_B4,
    BATTLEANIMFRAMESET_B5,
    BATTLEANIMFRAMESET_B6,
    BATTLEANIMFRAMESET_B7,
    BATTLEANIMFRAMESET_B8,
    NUM_BATTLEANIMFRAMESETS,
};

// BattleAnimOAMData indexes (see data/battle_anims/oam.asm)
enum {
    BATTLEANIMOAMSET_00,
    BATTLEANIMOAMSET_01,
    BATTLEANIMOAMSET_02,
    BATTLEANIMOAMSET_03,
    BATTLEANIMOAMSET_04,
    BATTLEANIMOAMSET_05,
    BATTLEANIMOAMSET_06,
    BATTLEANIMOAMSET_07,
    BATTLEANIMOAMSET_08,
    BATTLEANIMOAMSET_09,
    BATTLEANIMOAMSET_0A,
    BATTLEANIMOAMSET_0B,
    BATTLEANIMOAMSET_0C,
    BATTLEANIMOAMSET_0D,
    BATTLEANIMOAMSET_0E,
    BATTLEANIMOAMSET_0F,
    BATTLEANIMOAMSET_10,
    BATTLEANIMOAMSET_11,
    BATTLEANIMOAMSET_12,
    BATTLEANIMOAMSET_13,
    BATTLEANIMOAMSET_14,
    BATTLEANIMOAMSET_15,
    BATTLEANIMOAMSET_16,
    BATTLEANIMOAMSET_17,
    BATTLEANIMOAMSET_18,
    BATTLEANIMOAMSET_19,
    BATTLEANIMOAMSET_1A,
    BATTLEANIMOAMSET_1B,
    BATTLEANIMOAMSET_1C,
    BATTLEANIMOAMSET_1D,
    BATTLEANIMOAMSET_1E,
    BATTLEANIMOAMSET_1F,
    BATTLEANIMOAMSET_20,
    BATTLEANIMOAMSET_21,
    BATTLEANIMOAMSET_22,
    BATTLEANIMOAMSET_23,
    BATTLEANIMOAMSET_24,
    BATTLEANIMOAMSET_25,
    BATTLEANIMOAMSET_26,
    BATTLEANIMOAMSET_27,
    BATTLEANIMOAMSET_28,
    BATTLEANIMOAMSET_29,
    BATTLEANIMOAMSET_2A,
    BATTLEANIMOAMSET_2B,
    BATTLEANIMOAMSET_2C,
    BATTLEANIMOAMSET_2D,
    BATTLEANIMOAMSET_2E,
    BATTLEANIMOAMSET_2F,
    BATTLEANIMOAMSET_30,
    BATTLEANIMOAMSET_31,
    BATTLEANIMOAMSET_32,
    BATTLEANIMOAMSET_33,
    BATTLEANIMOAMSET_34,
    BATTLEANIMOAMSET_35,
    BATTLEANIMOAMSET_36,
    BATTLEANIMOAMSET_37,
    BATTLEANIMOAMSET_38,
    BATTLEANIMOAMSET_39,
    BATTLEANIMOAMSET_3A,
    BATTLEANIMOAMSET_3B,
    BATTLEANIMOAMSET_3C,
    BATTLEANIMOAMSET_3D,
    BATTLEANIMOAMSET_3E,
    BATTLEANIMOAMSET_3F,
    BATTLEANIMOAMSET_40,
    BATTLEANIMOAMSET_41,
    BATTLEANIMOAMSET_42,
    BATTLEANIMOAMSET_43,
    BATTLEANIMOAMSET_44,
    BATTLEANIMOAMSET_45,
    BATTLEANIMOAMSET_46,
    BATTLEANIMOAMSET_47,
    BATTLEANIMOAMSET_48,
    BATTLEANIMOAMSET_49,
    BATTLEANIMOAMSET_4A,
    BATTLEANIMOAMSET_4B,
    BATTLEANIMOAMSET_4C,
    BATTLEANIMOAMSET_4D,
    BATTLEANIMOAMSET_4E,
    BATTLEANIMOAMSET_4F,
    BATTLEANIMOAMSET_50,
    BATTLEANIMOAMSET_51,
    BATTLEANIMOAMSET_52,
    BATTLEANIMOAMSET_53,
    BATTLEANIMOAMSET_54,
    BATTLEANIMOAMSET_55,
    BATTLEANIMOAMSET_56,
    BATTLEANIMOAMSET_57,
    BATTLEANIMOAMSET_58,
    BATTLEANIMOAMSET_59,
    BATTLEANIMOAMSET_5A,
    BATTLEANIMOAMSET_5B,
    BATTLEANIMOAMSET_5C,
    BATTLEANIMOAMSET_5D,
    BATTLEANIMOAMSET_5E,
    BATTLEANIMOAMSET_5F,
    BATTLEANIMOAMSET_60,
    BATTLEANIMOAMSET_61,
    BATTLEANIMOAMSET_62,
    BATTLEANIMOAMSET_63,
    BATTLEANIMOAMSET_64,
    BATTLEANIMOAMSET_65,
    BATTLEANIMOAMSET_66,
    BATTLEANIMOAMSET_67,
    BATTLEANIMOAMSET_68,
    BATTLEANIMOAMSET_69,
    BATTLEANIMOAMSET_6A,
    BATTLEANIMOAMSET_6B,
    BATTLEANIMOAMSET_6C,
    BATTLEANIMOAMSET_6D,
    BATTLEANIMOAMSET_6E,
    BATTLEANIMOAMSET_6F,
    BATTLEANIMOAMSET_70,
    BATTLEANIMOAMSET_71,
    BATTLEANIMOAMSET_72,
    BATTLEANIMOAMSET_73,
    BATTLEANIMOAMSET_74,
    BATTLEANIMOAMSET_75,
    BATTLEANIMOAMSET_76,
    BATTLEANIMOAMSET_77,
    BATTLEANIMOAMSET_78,
    BATTLEANIMOAMSET_79,
    BATTLEANIMOAMSET_7A,
    BATTLEANIMOAMSET_7B,
    BATTLEANIMOAMSET_7C,
    BATTLEANIMOAMSET_7D,
    BATTLEANIMOAMSET_7E,
    BATTLEANIMOAMSET_7F,
    BATTLEANIMOAMSET_80,
    BATTLEANIMOAMSET_81,
    BATTLEANIMOAMSET_82,
    BATTLEANIMOAMSET_83,
    BATTLEANIMOAMSET_84,
    BATTLEANIMOAMSET_85,
    BATTLEANIMOAMSET_86,
    BATTLEANIMOAMSET_87,
    BATTLEANIMOAMSET_88,
    BATTLEANIMOAMSET_89,
    BATTLEANIMOAMSET_8A,
    BATTLEANIMOAMSET_8B,
    BATTLEANIMOAMSET_8C,
    BATTLEANIMOAMSET_8D,
    BATTLEANIMOAMSET_8E,
    BATTLEANIMOAMSET_8F,
    BATTLEANIMOAMSET_90,
    BATTLEANIMOAMSET_91,
    BATTLEANIMOAMSET_92,
    BATTLEANIMOAMSET_93,
    BATTLEANIMOAMSET_94,
    BATTLEANIMOAMSET_95,
    BATTLEANIMOAMSET_96,
    BATTLEANIMOAMSET_97,
    BATTLEANIMOAMSET_98,
    BATTLEANIMOAMSET_99,
    BATTLEANIMOAMSET_9A,
    BATTLEANIMOAMSET_9B,
    BATTLEANIMOAMSET_9C,
    BATTLEANIMOAMSET_9D,
    BATTLEANIMOAMSET_9E,
    BATTLEANIMOAMSET_9F,
    BATTLEANIMOAMSET_A0,
    BATTLEANIMOAMSET_A1,
    BATTLEANIMOAMSET_A2,
    BATTLEANIMOAMSET_A3,
    BATTLEANIMOAMSET_A4,
    BATTLEANIMOAMSET_A5,
    BATTLEANIMOAMSET_A6,
    BATTLEANIMOAMSET_A7,
    BATTLEANIMOAMSET_A8,
    BATTLEANIMOAMSET_A9,
    BATTLEANIMOAMSET_AA,
    BATTLEANIMOAMSET_AB,
    BATTLEANIMOAMSET_AC,
    BATTLEANIMOAMSET_AD,
    BATTLEANIMOAMSET_AE,
    BATTLEANIMOAMSET_AF,
    BATTLEANIMOAMSET_B0,
    BATTLEANIMOAMSET_B1,
    BATTLEANIMOAMSET_B2,
    BATTLEANIMOAMSET_B3,
    BATTLEANIMOAMSET_B4,
    BATTLEANIMOAMSET_B5,
    BATTLEANIMOAMSET_B6,
    BATTLEANIMOAMSET_B7,
    BATTLEANIMOAMSET_B8,
    BATTLEANIMOAMSET_B9,
    BATTLEANIMOAMSET_BA,
    BATTLEANIMOAMSET_BB,
    BATTLEANIMOAMSET_BC,
    BATTLEANIMOAMSET_BD,
    BATTLEANIMOAMSET_BE,
    BATTLEANIMOAMSET_BF,
    BATTLEANIMOAMSET_C0,
    BATTLEANIMOAMSET_C1,
    BATTLEANIMOAMSET_C2,
    BATTLEANIMOAMSET_C3,
    BATTLEANIMOAMSET_C4,
    BATTLEANIMOAMSET_C5,
    BATTLEANIMOAMSET_C6,
    BATTLEANIMOAMSET_C7,
    BATTLEANIMOAMSET_C8,
    BATTLEANIMOAMSET_C9,
    BATTLEANIMOAMSET_CA,
    BATTLEANIMOAMSET_CB,
    BATTLEANIMOAMSET_CC,
    BATTLEANIMOAMSET_CD,
    BATTLEANIMOAMSET_CE,
    BATTLEANIMOAMSET_CF,
    BATTLEANIMOAMSET_D0,
    BATTLEANIMOAMSET_D1,
    BATTLEANIMOAMSET_D2,
    BATTLEANIMOAMSET_D3,
    BATTLEANIMOAMSET_D4,
    BATTLEANIMOAMSET_D5,
    BATTLEANIMOAMSET_D6,
    BATTLEANIMOAMSET_D7,
    NUM_BATTLEANIMOAMSETS,
};

// BattleBGEffects indexes (see engine/battle_anims/bg_effects.asm)
enum {
    ANIM_BG_FLASH_INVERTED = 1,
    ANIM_BG_FLASH_WHITE,
    ANIM_BG_WHITE_HUES,
    ANIM_BG_BLACK_HUES,
    ANIM_BG_ALTERNATE_HUES,
    ANIM_BG_CYCLE_OBPALS_GRAY_AND_YELLOW,
    ANIM_BG_CYCLE_MID_OBPALS_GRAY_AND_YELLOW,
    ANIM_BG_CYCLE_BGPALS_INVERTED,
    ANIM_BG_HIDE_MON,
    ANIM_BG_SHOW_MON,
    ANIM_BG_ENTER_MON,
    ANIM_BG_RETURN_MON,
    ANIM_BG_SURF,
    ANIM_BG_WHIRLPOOL,
    ANIM_BG_TELEPORT,
    ANIM_BG_NIGHT_SHADE,
    ANIM_BG_BATTLEROBJ_1ROW,
    ANIM_BG_BATTLEROBJ_2ROW,
    ANIM_BG_DOUBLE_TEAM,
    ANIM_BG_ACID_ARMOR,
    ANIM_BG_RAPID_FLASH,
    ANIM_BG_FADE_MON_TO_LIGHT,
    ANIM_BG_FADE_MON_TO_BLACK,
    ANIM_BG_FADE_MON_TO_LIGHT_REPEATING,
    ANIM_BG_FADE_MON_TO_BLACK_REPEATING,
    ANIM_BG_CYCLE_MON_LIGHT_DARK_REPEATING,
    ANIM_BG_FLASH_MON_REPEATING,
    ANIM_BG_FADE_MONS_TO_BLACK_REPEATING,
    ANIM_BG_FADE_MON_TO_WHITE_WAIT_FADE_BACK,
    ANIM_BG_FADE_MON_FROM_WHITE,
    ANIM_BG_SHAKE_SCREEN_X,
    ANIM_BG_SHAKE_SCREEN_Y,
    ANIM_BG_WITHDRAW,
    ANIM_BG_BOUNCE_DOWN,
    ANIM_BG_DIG,
    ANIM_BG_TACKLE,
    ANIM_BG_WOBBLE_MON,
    ANIM_BG_REMOVE_MON,
    ANIM_BG_WAVE_DEFORM_MON,
    ANIM_BG_PSYCHIC,
    ANIM_BG_BETA_SEND_OUT_MON1,
    ANIM_BG_BETA_SEND_OUT_MON2,
    ANIM_BG_FLAIL,
    ANIM_BG_BETA_PURSUIT,
    ANIM_BG_ROLLOUT,
    ANIM_BG_VITAL_THROW,
    ANIM_BG_START_WATER,
    ANIM_BG_WATER,
    ANIM_BG_END_WATER,
    ANIM_BG_VIBRATE_MON,
    ANIM_BG_WOBBLE_PLAYER,
    ANIM_BG_WOBBLE_SCREEN,
};
#define NUM_ANIM_BGS ANIM_BG_WOBBLE_SCREEN

// wBattleAnimTileDict keys (see wram.asm)
// AnimObjGFX indexes (see data/battle_anims/object_gfx.asm)
enum {
    ANIM_GFX_HIT = 1,
    ANIM_GFX_CUT,
    ANIM_GFX_FIRE,
    ANIM_GFX_WATER,
    ANIM_GFX_LIGHTNING,
    ANIM_GFX_PLANT,
    ANIM_GFX_SMOKE,
    ANIM_GFX_EXPLOSION,
    ANIM_GFX_ROCKS,
    ANIM_GFX_ICE,
    ANIM_GFX_POKE_BALL,
    ANIM_GFX_POISON,
    ANIM_GFX_BUBBLE,
    ANIM_GFX_NOISE,
    ANIM_GFX_POWDER,
    ANIM_GFX_BEAM,
    ANIM_GFX_SPEED,
    ANIM_GFX_CHARGE,
    ANIM_GFX_WIND,
    ANIM_GFX_WHIP,
    ANIM_GFX_EGG,
    ANIM_GFX_ROPE,
    ANIM_GFX_PSYCHIC,
    ANIM_GFX_REFLECT,
    ANIM_GFX_STATUS,
    ANIM_GFX_SAND,
    ANIM_GFX_WEB,
    ANIM_GFX_HAZE,
    ANIM_GFX_HORN,
    ANIM_GFX_FLOWER,
    ANIM_GFX_MISC,
    ANIM_GFX_SKY_ATTACK,
    ANIM_GFX_GLOBE,
    ANIM_GFX_SHAPES,
    ANIM_GFX_OBJECTS,
    ANIM_GFX_SHINE,
    ANIM_GFX_ANGELS,
    ANIM_GFX_WAVE,
    ANIM_GFX_AEROBLAST,
    ANIM_GFX_PLAYERHEAD,
    ANIM_GFX_ENEMYFEET,
};
#define NUM_ANIM_GFX ANIM_GFX_ENEMYFEET

// battle_bg_effect struct members (see macros/wram.asm)
enum {
    BG_EFFECT_STRUCT_FUNCTION,
    BG_EFFECT_STRUCT_JT_INDEX,
    BG_EFFECT_STRUCT_BATTLE_TURN,
    BG_EFFECT_STRUCT_PARAM,
    BG_EFFECT_STRUCT_LENGTH,
};
#define NUM_BG_EFFECTS 5 // see wActiveBGEffects

// anim_bgeffect battle turn values for some effects
enum {
    BG_EFFECT_TARGET, // 0
    BG_EFFECT_USER,   // 1
};

// battle palettes
enum {
    PAL_BATTLE_BG_PLAYER,    // 0
    PAL_BATTLE_BG_ENEMY,     // 1
    PAL_BATTLE_BG_ENEMY_HP,  // 2
    PAL_BATTLE_BG_PLAYER_HP, // 3
    PAL_BATTLE_BG_EXP,       // 4
    PAL_BATTLE_BG_5,         // 5
    PAL_BATTLE_BG_6,         // 6
    PAL_BATTLE_BG_TEXT,      // 7
};

// animation object palettes
enum {
    PAL_BATTLE_OB_ENEMY,  // 0
    PAL_BATTLE_OB_PLAYER, // 1
    PAL_BATTLE_OB_GRAY,   // 2
    PAL_BATTLE_OB_YELLOW, // 3
    PAL_BATTLE_OB_RED,    // 4
    PAL_BATTLE_OB_GREEN,  // 5
    PAL_BATTLE_OB_BLUE,   // 6
    PAL_BATTLE_OB_BROWN,  // 7
};
