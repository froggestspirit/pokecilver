// TrainerClassAttributes struct members (see data/trainers/attributes.asm)
rsreset
TRNATTR_ITEM1           rb // 0
TRNATTR_ITEM2           rb // 1
TRNATTR_BASEMONEY       rb // 2
TRNATTR_AI_MOVE_WEIGHTS rw // 3
TRNATTR_AI_ITEM_SWITCH  rw // 5
#define NUM_TRAINER_ATTRIBUTES _RS

// TRNATTR_AI_MOVE_WEIGHTS bit flags (wEnemyTrainerAIFlags)
// AIScoringPointers indexes (see engine/battle/ai/move.asm)
enum {
};
	shift_const AI_BASIC
	shift_const AI_SETUP
	shift_const AI_TYPES
	shift_const AI_OFFENSIVE
	shift_const AI_SMART
	shift_const AI_OPPORTUNIST
	shift_const AI_AGGRESSIVE
	shift_const AI_CAUTIOUS
	shift_const AI_STATUS
	shift_const AI_RISKY
#define NO_AI 0

// TRNATTR_AI_ITEM_SWITCH bit flags
enum {
	SWITCH_OFTEN_F,     // 0
	SWITCH_RARELY_F,    // 1
	SWITCH_SOMETIMES_F, // 2
};
	_skip,               // 3
	ALWAYS_USE_F,       // 4
	UNKNOWN_USE_F,      // 5
	CONTEXT_USE_F,      // 6

#define SWITCH_OFTEN 1 << SWITCH_OFTEN_F
#define SWITCH_RARELY 1 << SWITCH_RARELY_F
#define SWITCH_SOMETIMES 1 << SWITCH_SOMETIMES_F
#define ALWAYS_USE 1 << ALWAYS_USE_F
#define UNKNOWN_USE 1 << UNKNOWN_USE_F
#define CONTEXT_USE 1 << CONTEXT_USE_F

// TrainerTypes indexes (see engine/battle/read_trainer_party.asm)
enum {
};
	TRAINERTYPE_NORMAL,
	TRAINERTYPE_MOVES,
	TRAINERTYPE_ITEM,
	TRAINERTYPE_ITEM_MOVES,
