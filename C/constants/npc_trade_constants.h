// npctrade struct members (see data/events/npc_trades.asm)
rsreset
NPCTRADE_DIALOG   rb
NPCTRADE_GIVEMON  rb
NPCTRADE_GETMON   rb
NPCTRADE_NICKNAME rb MON_NAME_LENGTH
NPCTRADE_DVS      rw
NPCTRADE_ITEM     rb
NPCTRADE_OT_ID    rw
NPCTRADE_OT_NAME  rb NAME_LENGTH
NPCTRADE_GENDER   rb
                  rb_skip
#define NPCTRADE_STRUCT_LENGTH _RS

// NPCTrades indexes (see data/events/npc_trades.asm)
enum {
	NPC_TRADE_MIKE,   // 0
	NPC_TRADE_KYLE,   // 1
	NPC_TRADE_TIM,    // 2
	NPC_TRADE_EMY,    // 3
	NPC_TRADE_CHRIS,  // 4
	NPC_TRADE_KIM,    // 5
};
	NUM_NPC_TRADES,

// trade gender limits
enum {
};
	TRADE_GENDER_EITHER,
	TRADE_GENDER_MALE,
	TRADE_GENDER_FEMALE,

// TradeTexts indexes (see engine/events/npc_trade.asm)

// trade dialogs
enum {
};
	TRADE_DIALOG_INTRO,
	TRADE_DIALOG_CANCEL,
	TRADE_DIALOG_WRONG,
	TRADE_DIALOG_COMPLETE,
	TRADE_DIALOG_AFTER,

// trade dialog sets
enum {
};
	TRADE_DIALOGSET_COLLECTOR,
	TRADE_DIALOGSET_HAPPY,
	TRADE_DIALOGSET_NEWBIE,
