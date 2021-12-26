// MenuHeader flags
enum {
};
	shift_const MENU_RESTORE_TILES // Will be set if MENU_BACKUP_TILES(_2) is set.
	shift_const MENU_UNUSED_1
	shift_const MENU_UNUSED_2
	shift_const MENU_NO_CLICK_SFX
	shift_const MENU_SPRITE_ANIMS
	shift_const MENU_UNUSED_3
	shift_const MENU_BACKUP_TILES
	shift_const MENU_BACKUP_TILES_2

// VerticalMenu/DoNthMenu/SetUpMenu/_2DMenu MenuData flags
// Per flag the comment specifies which menus actually implement it
enum {
};
	shift_const STATICMENU_DISABLE_B // VerticalMenu/_2DMenu
	shift_const STATICMENU_ENABLE_SELECT // VerticalMenu/_2DMenu
	shift_const STATICMENU_ENABLE_LEFT_RIGHT // DoNthMenu/SetUpMenu
	shift_const STATICMENU_ENABLE_START // DoNthMenu/SetUpMenu
	shift_const STATICMENU_PLACE_TITLE // VerticalMenu
	shift_const STATICMENU_WRAP // All
	shift_const STATICMENU_NO_TOP_SPACING // VerticalMenu/_2DMenu
	shift_const STATICMENU_CURSOR // VerticalMenu/_2DMenu

// ScrollingMenu MenuData flags
enum {
};
	shift_const SCROLLINGMENU_CALL_FUNCTION1_CANCEL
	shift_const SCROLLINGMENU_CALL_FUNCTION3_NO_SWITCH
	shift_const SCROLLINGMENU_ENABLE_LEFT
	shift_const SCROLLINGMENU_ENABLE_RIGHT
	shift_const SCROLLINGMENU_DISPLAY_ARROWS
	shift_const SCROLLINGMENU_ENABLE_FUNCTION3
	shift_const SCROLLINGMENU_ENABLE_START
	shift_const SCROLLINGMENU_ENABLE_SELECT

// ScrollingMenu items structure format
enum {
};
	SCROLLINGMENU_ITEMS_NORMAL,
	SCROLLINGMENU_ITEMS_QUANTITY,

// MonMenuOptions indexes (see data/mon_menu.asm)
// used by PokemonActionSubmenu (see engine/pokemon/mon_menu.asm)
enum {
};
	// moves
	MONMENUITEM_CUT,        // 1
	MONMENUITEM_FLY,        // 2
	MONMENUITEM_SURF,       // 3
	MONMENUITEM_STRENGTH,   // 4
	MONMENUITEM_WATERFALL,  // 5
	MONMENUITEM_FLASH,      // 6
	MONMENUITEM_WHIRLPOOL,  // 7
	MONMENUITEM_DIG,        // 8
	MONMENUITEM_TELEPORT,   // 9
	MONMENUITEM_SOFTBOILED, // 10
	MONMENUITEM_HEADBUTT,   // 11
	MONMENUITEM_ROCKSMASH,  // 12
	MONMENUITEM_MILKDRINK,  // 13
	MONMENUITEM_SWEETSCENT, // 14
	// options
	MONMENUITEM_STATS,      // 15
	MONMENUITEM_SWITCH,     // 16
	MONMENUITEM_ITEM,       // 17
	MONMENUITEM_CANCEL,     // 18
	MONMENUITEM_MOVE,       // 19
	MONMENUITEM_MAIL,       // 20
	MONMENUITEM_ERROR,      // 21
#define NUM_MONMENUITEMS const_value - 1

// MonMenuOptions categories
#define MONMENU_FIELD_MOVE 0
#define MONMENU_MENUOPTION 1

#define NUM_MONMENU_ITEMS 8

// start/select menu return values
#define HMENURETURN_SCRIPT 0b10000000
#define HMENURETURN_ASM 0b11111111

// PartyMenuQualityPointers indexes (see data/party_menu_qualities.asm)
enum {
};
	PARTYMENUACTION_CHOOSE_POKEMON,
	PARTYMENUACTION_HEALING_ITEM,
	PARTYMENUACTION_SWITCH,
	PARTYMENUACTION_TEACH_TMHM,
	PARTYMENUACTION_MOVE,
	PARTYMENUACTION_EVO_STONE,
	PARTYMENUACTION_GIVE_MON,
	PARTYMENUACTION_GIVE_MON_FEMALE, // unused
	PARTYMENUACTION_GIVE_ITEM,
#define NUM_PARTYMENUACTIONS const_value
// PrintPartyMenuActionText arguments (see engine/pokemon/party_menu.asm)
	_next, 0xf0
	PARTYMENUTEXT_HEAL_PSN,
	PARTYMENUTEXT_HEAL_BRN,
	PARTYMENUTEXT_HEAL_FRZ,
	PARTYMENUTEXT_HEAL_SLP,
	PARTYMENUTEXT_HEAL_PAR,
	PARTYMENUTEXT_HEAL_HP,
	PARTYMENUTEXT_HEAL_ALL,
	PARTYMENUTEXT_REVIVE,
	PARTYMENUTEXT_LEVEL_UP,
	PARTYMENUTEXT_HEAL_CONFUSION,

// Naming types (see engine/menus/naming_screen.asm)
enum {
};
	NAME_MON,
	NAME_PLAYER,
	NAME_RIVAL,
	NAME_MOM,
	NAME_BOX,
	NAME_FRIEND,
	NAME_6, // duplicate of NAME_MON
	NAME_7, // duplicate of NAME_MON
#define NUM_NAME_TYPES const_value
