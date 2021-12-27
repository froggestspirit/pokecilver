// item_attributes struct members (see data/items/attributes.asm)
rsreset
ITEMATTR_PRICE       rw
rsset ITEMATTR_PRICE
ITEMATTR_PRICE_LO    rb
ITEMATTR_PRICE_HI    rb
ITEMATTR_EFFECT      rb
ITEMATTR_PARAM       rb
ITEMATTR_PERMISSIONS rb
ITEMATTR_POCKET      rb
ITEMATTR_HELP        rb
#define ITEMATTR_STRUCT_LENGTH _RS

// item types
enum {
	ITEM,     // 1
	KEY_ITEM, // 2
	BALL,     // 3
	TM_HM,    // 4
};
#define NUM_ITEM_TYPES const_value - 1

// item menu types
// UseItem.dw indexes (see engine/items/pack.asm)
// UseRegisteredItem.SwitchTo indexes (see engine/overworld/select_menu.asm)
enum {
	ITEMMENU_NOUSE,   // 0
};
	_skip, 3
	ITEMMENU_CURRENT, // 4
	ITEMMENU_PARTY,   // 5
	ITEMMENU_CLOSE,   // 6

// item actions
#define CANT_SELECT_F 6
#define CANT_TOSS_F 7

#define NO_LIMITS 0
#define CANT_SELECT 1 << CANT_SELECT_F
#define CANT_TOSS 1 << CANT_TOSS_F

// pack pockets
enum {
	ITEM_POCKET,     // 0
	BALL_POCKET,     // 1
	KEY_ITEM_POCKET, // 2
	TM_HM_POCKET,    // 3
};
	NUM_POCKETS,

#define MAX_ITEMS 20
#define MAX_BALLS 12
#define MAX_KEY_ITEMS 25
#define MAX_PC_ITEMS 50

#define MAX_ITEM_STACK 99

// mail
#define MAIL_LINE_LENGTH 0x10
#define MAIL_MSG_LENGTH 0x20
#define MAILBOX_CAPACITY 10
#define MAIL_STRUCT_LENGTH 0x2f // mailmsg struct

// held item effects
enum {
};
	HELD_NONE,
	HELD_BERRY,
	HELD_2,
	HELD_LEFTOVERS,
	_skip,
	HELD_5,
	HELD_RESTORE_PP,
	_skip,
	HELD_CLEANSE_TAG,

	_next, 10
	HELD_HEAL_POISON,
	HELD_HEAL_FREEZE,
	HELD_HEAL_BURN,
	HELD_HEAL_SLEEP,
	HELD_HEAL_PARALYZE,
	HELD_HEAL_STATUS,
	HELD_HEAL_CONFUSION,

	_next, 20
	HELD_PREVENT_POISON,
	HELD_PREVENT_BURN,
	HELD_PREVENT_FREEZE,
	HELD_PREVENT_SLEEP,
	HELD_PREVENT_PARALYZE,
	HELD_PREVENT_CONFUSE,

	_next, 30
	HELD_30,
	HELD_ATTACK_UP,
	HELD_DEFENSE_UP,
	HELD_SPEED_UP,
	HELD_SP_ATTACK_UP,
	HELD_SP_DEFENSE_UP,
	HELD_ACCURACY_UP,
	HELD_EVASION_UP,
	HELD_38,

	_next, 40
	_skip,
	_skip,
	HELD_METAL_POWDER,

	_next, 50
	HELD_NORMAL_BOOST,
	HELD_FIGHTING_BOOST,
	HELD_FLYING_BOOST,
	HELD_POISON_BOOST,
	HELD_GROUND_BOOST,
	HELD_ROCK_BOOST,
	HELD_BUG_BOOST,
	HELD_GHOST_BOOST,
	HELD_FIRE_BOOST,
	HELD_WATER_BOOST,
	HELD_GRASS_BOOST,
	HELD_ELECTRIC_BOOST,
	HELD_PSYCHIC_BOOST,
	HELD_ICE_BOOST,
	HELD_DRAGON_BOOST,
	HELD_DARK_BOOST,
	HELD_STEEL_BOOST,

	_next, 70
	HELD_CATCH_CHANCE,
	HELD_71,
	HELD_ESCAPE,
	HELD_CRITICAL_UP,
	HELD_QUICK_CLAW,
	HELD_FLINCH,
	HELD_AMULET_COIN,
	HELD_BRIGHTPOWDER,
	_skip,
	HELD_FOCUS_BAND,
