// CGBLayoutJumptable indexes (see engine/gfx/cgb_layouts.asm)
// SGBLayoutJumptable indexes (see engine/gfx/sgb_layouts.asm)
enum {
    SCGB_BATTLE_GRAYSCALE,
    SCGB_BATTLE_COLORS,
    SCGB_POKEGEAR_PALS,
    SCGB_STATS_SCREEN_HP_PALS,
    SCGB_POKEDEX,
    SCGB_SLOT_MACHINE,
    SCGB_BETA_TITLE_SCREEN,
    SCGB_GS_INTRO,
    SCGB_DIPLOMA,
    SCGB_MAPPALS,
    SCGB_PARTY_MENU,
    SCGB_EVOLUTION,
    SCGB_GS_TITLE_SCREEN,
    SCGB_0D,
    SCGB_MOVE_LIST,
    SCGB_BETA_PIKACHU_MINIGAME,
    SCGB_POKEDEX_SEARCH_OPTION,
    SCGB_BETA_POKER,
    SCGB_POKEPIC,
    SCGB_MAGNET_TRAIN,
    SCGB_PACKPALS,
    SCGB_TRAINER_CARD,
    SCGB_POKEDEX_UNOWN_MODE,
    SCGB_BILLS_PC,
    SCGB_UNOWN_PUZZLE,
    SCGB_GAMEFREAK_LOGO,
    SCGB_PLAYER_OR_MON_FRONTPIC_PALS,
    SCGB_TRADE_TUBE,
    SCGB_TRAINER_OR_MON_FRONTPIC_PALS,
    SCGB_MYSTERY_GIFT,
    SCGB_1E,
    SCGB_POKEDEX_5X5,
    NUM_SCGB_LAYOUTS,
};

#define SCGB_PARTY_MENU_HP_BARS 0xfc
#define SCGB_DEFAULT 0xff

// PredefPals indexes (see gfx/sgb/predef.pal)
// GetPredefPal arguments (see engine/gfx/color.asm)
enum {
    PREDEFPAL_ROUTES,
    PREDEFPAL_PALLET,
    PREDEFPAL_VIRIDIAN,
    PREDEFPAL_PEWTER,
    PREDEFPAL_CERULEAN,
    PREDEFPAL_LAVENDER,
    PREDEFPAL_VERMILION,
    PREDEFPAL_CELADON,
    PREDEFPAL_FUCHSIA,
    PREDEFPAL_CINNABAR,
    PREDEFPAL_SAFFRON,
    PREDEFPAL_INDIGO,
    PREDEFPAL_NEW_BARK,
    PREDEFPAL_CHERRYGROVE,
    PREDEFPAL_VIOLET,
    PREDEFPAL_AZALEA,
    PREDEFPAL_GOLDENROD,
    PREDEFPAL_ECRUTEAK,
    PREDEFPAL_OLIVINE,
    PREDEFPAL_CIANWOOD,
    PREDEFPAL_MAHOGANY,
    PREDEFPAL_BLACKTHORN,
    PREDEFPAL_LAKE_OF_RAGE,
    PREDEFPAL_SILVER_CAVE,
    PREDEFPAL_DUNGEONS,
    PREDEFPAL_NITE,
    PREDEFPAL_BLACKOUT,
    PREDEFPAL_DIPLOMA, // RB_MEWMON
    PREDEFPAL_TRADE_TUBE, // RB_BLUEMON
    PREDEFPAL_POKEDEX, // RB_REDMON
    PREDEFPAL_RB_CYANMON,
    PREDEFPAL_RB_PURPLEMON,
    PREDEFPAL_RB_BROWNMON,
    PREDEFPAL_RB_GREENMON,
    PREDEFPAL_RB_PINKMON,
    PREDEFPAL_RB_YELLOWMON,
    PREDEFPAL_CGB_BADGE, // RB_GRAYMON
    PREDEFPAL_BETA_SHINY_MEWMON,
    PREDEFPAL_BETA_SHINY_BLUEMON,
    PREDEFPAL_BETA_SHINY_REDMON,
    PREDEFPAL_BETA_SHINY_CYANMON,
    PREDEFPAL_BETA_SHINY_PURPLEMON,
    PREDEFPAL_BETA_SHINY_BROWNMON,
    PREDEFPAL_BETA_SHINY_GREENMON,
    PREDEFPAL_BETA_SHINY_PINKMON,
    PREDEFPAL_BETA_SHINY_YELLOWMON,
    PREDEFPAL_PARTY_ICON, // BETA_SHINY_GRAYMON
    PREDEFPAL_HP_GREEN,
    PREDEFPAL_HP_YELLOW,
    PREDEFPAL_HP_RED,
    PREDEFPAL_POKEGEAR,
    PREDEFPAL_BETA_LOGO_1,
    PREDEFPAL_BETA_LOGO_2,
    PREDEFPAL_GS_INTRO_GAMEFREAK_LOGO,
    PREDEFPAL_GS_INTRO_SHELLDER_LAPRAS,
    PREDEFPAL_BETA_INTRO_LAPRAS,
    PREDEFPAL_GS_INTRO_JIGGLYPUFF_PIKACHU_BG,
    PREDEFPAL_GS_INTRO_JIGGLYPUFF_PIKACHU_OB,
    PREDEFPAL_GS_INTRO_STARTERS_TRANSITION,
    PREDEFPAL_BETA_INTRO_VENUSAUR,
    PREDEFPAL_PACK, // GS_INTRO_CHARIZARD
    PREDEFPAL_SLOT_MACHINE_0,
    PREDEFPAL_SLOT_MACHINE_1,
    PREDEFPAL_SLOT_MACHINE_2,
    PREDEFPAL_SLOT_MACHINE_3,
    PREDEFPAL_BETA_POKER_0,
    PREDEFPAL_BETA_POKER_1,
    PREDEFPAL_BETA_POKER_2,
    PREDEFPAL_BETA_POKER_3,
    PREDEFPAL_BETA_RADIO,
    PREDEFPAL_BETA_POKEGEAR,
    PREDEFPAL_47,
    PREDEFPAL_GS_TITLE_SCREEN_0,
    PREDEFPAL_GS_TITLE_SCREEN_1,
    PREDEFPAL_GS_TITLE_SCREEN_2,
    PREDEFPAL_GS_TITLE_SCREEN_3,
    PREDEFPAL_UNOWN_PUZZLE,
    PREDEFPAL_GAMEFREAK_LOGO_OB,
    PREDEFPAL_GAMEFREAK_LOGO_BG,
    NUM_PREDEF_PALS,
};

// SGB system command codes
// http://gbdev.gg8.se/wiki/articles/SGB_Functions#SGB_System_Command_Table
enum {
    SGB_PAL01,
    SGB_PAL23,
    SGB_PAL03,
    SGB_PAL12,
    SGB_ATTR_BLK,
    SGB_ATTR_LIN,
    SGB_ATTR_DIV,
    SGB_ATTR_CHR,
    SGB_SOUND,
    SGB_SOU_TRN,
    SGB_PAL_SET,
    SGB_PAL_TRN,
    SGB_ATRC_EN,
    SGB_TEST_EN,
    SGB_ICON_EN,
    SGB_DATA_SND,
    SGB_DATA_TRN,
    SGB_MLT_REQ,
    SGB_JUMP,
    SGB_CHR_TRN,
    SGB_PCT_TRN,
    SGB_ATTR_TRN,
    SGB_ATTR_SET,
    SGB_MASK_EN,
    SGB_OBJ_TRN,
};

#define PALPACKET_LENGTH 0x10
