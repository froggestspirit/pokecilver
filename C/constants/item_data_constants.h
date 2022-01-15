// item_attributes struct members (see data/items/attributes.asm)
enum {
    ITEMATTR_PRICE_LO,
    ITEMATTR_PRICE_HI,
    ITEMATTR_EFFECT,
    ITEMATTR_PARAM,
    ITEMATTR_PERMISSIONS,
    ITEMATTR_POCKET,
    ITEMATTR_HELP,
    ITEMATTR_STRUCT_LENGTH,
};
#define ITEMATTR_PRICE ITEMATTR_PRICE_LO
// item types
enum {
    ITEM = 1,     // 1
    KEY_ITEM, // 2
    BALL,     // 3
    TM_HM,    // 4
};
#define NUM_ITEM_TYPES TM_HM

// item menu types
// UseItem.dw indexes (see engine/items/pack.asm)
// UseRegisteredItem.SwitchTo indexes (see engine/overworld/select_menu.asm)
enum {
    ITEMMENU_NOUSE,   // 0
    ITEMMENU_CURRENT = 4,
    ITEMMENU_PARTY,   // 5
    ITEMMENU_CLOSE,   // 6
};

// item actions
#define CANT_SELECT_F 6
#define CANT_TOSS_F 7

#define NO_LIMITS 0
#define CANT_SELECT (1 << CANT_SELECT_F)
#define CANT_TOSS (1 << CANT_TOSS_F)

// pack pockets
enum {
    ITEM_POCKET,     // 0
    BALL_POCKET,     // 1
    KEY_ITEM_POCKET, // 2
    TM_HM_POCKET,    // 3
    NUM_POCKETS,
};

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
    HELD_NONE,
    HELD_BERRY,
    HELD_2,
    HELD_LEFTOVERS,
    HELD_UNUSED_01,
    HELD_5,
    HELD_RESTORE_PP,
    HELD_UNUSED_02,
    HELD_CLEANSE_TAG,

    HELD_HEAL_POISON = 10,
    HELD_HEAL_FREEZE,
    HELD_HEAL_BURN,
    HELD_HEAL_SLEEP,
    HELD_HEAL_PARALYZE,
    HELD_HEAL_STATUS,
    HELD_HEAL_CONFUSION,

    HELD_PREVENT_POISON = 20,
    HELD_PREVENT_BURN,
    HELD_PREVENT_FREEZE,
    HELD_PREVENT_SLEEP,
    HELD_PREVENT_PARALYZE,
    HELD_PREVENT_CONFUSE,

    HELD_30 = 30,
    HELD_ATTACK_UP,
    HELD_DEFENSE_UP,
    HELD_SPEED_UP,
    HELD_SP_ATTACK_UP,
    HELD_SP_DEFENSE_UP,
    HELD_ACCURACY_UP,
    HELD_EVASION_UP,
    HELD_38,

    HELD_UNUSED_03 = 40,
    HELD_UNUSED_04,
    HELD_METAL_POWDER,

    HELD_NORMAL_BOOST = 50,
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

    HELD_CATCH_CHANCE = 70,
    HELD_71,
    HELD_ESCAPE,
    HELD_CRITICAL_UP,
    HELD_QUICK_CLAW,
    HELD_FLINCH,
    HELD_AMULET_COIN,
    HELD_BRIGHTPOWDER,
    HELD_UNUSED_05,
    HELD_FOCUS_BAND,
};
