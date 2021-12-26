// object_struct members (see macros/wram.asm)
rsreset
OBJECT_SPRITE              rb // 00
OBJECT_MAP_OBJECT_INDEX    rb // 01
OBJECT_SPRITE_TILE         rb // 02
OBJECT_MOVEMENTTYPE        rb // 03
OBJECT_FLAGS1              rb // 04
OBJECT_FLAGS2              rb // 05
OBJECT_PALETTE             rb // 06
OBJECT_DIRECTION_WALKING   rb // 07
OBJECT_FACING              rb // 08
OBJECT_STEP_TYPE           rb // 09
OBJECT_STEP_DURATION       rb // 0a
OBJECT_ACTION              rb // 0b
OBJECT_STEP_FRAME          rb // 0c
OBJECT_FACING_STEP         rb // 0d
OBJECT_NEXT_TILE           rb // 0e
OBJECT_STANDING_TILE       rb // 0f
OBJECT_NEXT_MAP_X          rb // 10
OBJECT_NEXT_MAP_Y          rb // 11
OBJECT_MAP_X               rb // 12
OBJECT_MAP_Y               rb // 13
OBJECT_INIT_X              rb // 14
OBJECT_INIT_Y              rb // 15
OBJECT_RADIUS              rb // 16
OBJECT_SPRITE_X            rb // 17
OBJECT_SPRITE_Y            rb // 18
OBJECT_SPRITE_X_OFFSET     rb // 19
OBJECT_SPRITE_Y_OFFSET     rb // 1a
OBJECT_MOVEMENT_BYTE_INDEX rb // 1b
OBJECT_1C                  rb // 1c
OBJECT_1D                  rb // 1d
OBJECT_1E                  rb // 1e
OBJECT_1F                  rb // 1f
OBJECT_RANGE               rb // 20
                           rb_skip 7
#define OBJECT_LENGTH _RS
#define NUM_OBJECT_STRUCTS 13 // see wObjectStructs

// object_struct OBJECT_FACING values
#define OW_DOWN DOWN  << 2
#define OW_UP UP    << 2
#define OW_LEFT LEFT  << 2
#define OW_RIGHT RIGHT << 2

// object_struct OBJECT_FLAGS1 bit flags
enum {
	INVISIBLE_F,     // 0
	WONT_DELETE_F,   // 1
	FIXED_FACING_F,  // 2
	SLIDING_F,       // 3
	NOCLIP_TILES_F,  // 4
	MOVE_ANYWHERE_F, // 5
	NOCLIP_OBJS_F,   // 6
	EMOTE_OBJECT_F,  // 7
};

#define INVISIBLE 1 << INVISIBLE_F
#define WONT_DELETE 1 << WONT_DELETE_F
#define FIXED_FACING 1 << FIXED_FACING_F
#define SLIDING 1 << SLIDING_F
#define NOCLIP_TILES 1 << NOCLIP_TILES_F
#define MOVE_ANYWHERE 1 << MOVE_ANYWHERE_F
#define NOCLIP_OBJS 1 << NOCLIP_OBJS_F
#define EMOTE_OBJECT 1 << EMOTE_OBJECT_F

// object_struct OBJECT_FLAGS2 bit flags
enum {
	LOW_PRIORITY_F,  // 0
	HIGH_PRIORITY_F, // 1
	OBJ_FLAGS2_2,    // 2
	OVERHEAD_F,      // 3
	USE_OBP1_F,      // 4
	FROZEN_F,        // 5
	OBJ_FLAGS2_6,    // 6
	OBJ_FLAGS2_7,    // 7
};

#define LOW_PRIORITY 1 << LOW_PRIORITY_F
#define HIGH_PRIORITY 1 << HIGH_PRIORITY_F
#define OVERHEAD 1 << OVERHEAD_F
#define USE_OBP1 1 << USE_OBP1_F

// object_struct OBJECT_PALETTE bit flags
enum {
	SWIMMING_F,         // 5
	STRENGTH_BOULDER_F, // 6
	BIG_OBJECT_F,       // 7
};

#define SWIMMING 1 << SWIMMING_F
#define STRENGTH_BOULDER 1 << STRENGTH_BOULDER_F
#define BIG_OBJECT 1 << BIG_OBJECT_F

// facing attribute bit flags
#define RELATIVE_ATTRIBUTES_F 1
#define ABSOLUTE_TILE_ID_F 2

#define RELATIVE_ATTRIBUTES 1 << RELATIVE_ATTRIBUTES_F
#define ABSOLUTE_TILE_ID 1 << ABSOLUTE_TILE_ID_F

// map_object struct members (see macros/wram.asm)
rsreset
MAPOBJECT_OBJECT_STRUCT_ID rb // 0
MAPOBJECT_SPRITE           rb // 1
MAPOBJECT_Y_COORD          rb // 2
MAPOBJECT_X_COORD          rb // 3
MAPOBJECT_MOVEMENT         rb // 4
MAPOBJECT_RADIUS           rb // 5
MAPOBJECT_HOUR             rb // 6
MAPOBJECT_TIMEOFDAY        rb // 7
MAPOBJECT_COLOR            rb // 8
MAPOBJECT_RANGE            rb // 9
MAPOBJECT_SCRIPT_POINTER   rw // a
MAPOBJECT_EVENT_FLAG       rw // c
                           rb_skip 2
#define MAPOBJECT_LENGTH _RS
#define NUM_OBJECTS 16
#define PLAYER_OBJECT 0

// SpriteMovementData struct members (see data/sprites/map_objects.asm)
rsreset
SPRITEMOVEATTR_MOVEMENT rb // 0
SPRITEMOVEATTR_FACING   rb // 1
SPRITEMOVEATTR_ACTION   rb // 2
SPRITEMOVEATTR_FLAGS1   rb // 3
SPRITEMOVEATTR_FLAGS2   rb // 4
SPRITEMOVEATTR_PALFLAGS rb // 5
#define NUM_SPRITEMOVEDATA_FIELDS _RS

#define MAPOBJECT_SCREEN_WIDTH (SCREEN_WIDTH / 2) + 2
#define MAPOBJECT_SCREEN_HEIGHT (SCREEN_HEIGHT / 2) + 2

// SpriteMovementData indexes (see data/sprites/map_objects.asm)
enum {
	SPRITEMOVEDATA_00,                   // 00
	SPRITEMOVEDATA_STILL,                // 01
	SPRITEMOVEDATA_WANDER,               // 02
	SPRITEMOVEDATA_SPINRANDOM_SLOW,      // 03
	SPRITEMOVEDATA_WALK_UP_DOWN,         // 04
	SPRITEMOVEDATA_WALK_LEFT_RIGHT,      // 05
	SPRITEMOVEDATA_STANDING_DOWN,        // 06
	SPRITEMOVEDATA_STANDING_UP,          // 07
	SPRITEMOVEDATA_STANDING_LEFT,        // 08
	SPRITEMOVEDATA_STANDING_RIGHT,       // 09
	SPRITEMOVEDATA_SPINRANDOM_FAST,      // 0a
	SPRITEMOVEDATA_PLAYER,               // 0b
	SPRITEMOVEDATA_INDEXED_1,            // 0c
	SPRITEMOVEDATA_INDEXED_2,            // 0d
	SPRITEMOVEDATA_0E,                   // 0e
	SPRITEMOVEDATA_0F,                   // 0f
	SPRITEMOVEDATA_10,                   // 10
	SPRITEMOVEDATA_11,                   // 11
	SPRITEMOVEDATA_12,                   // 12
	SPRITEMOVEDATA_FOLLOWING,            // 13
	SPRITEMOVEDATA_SCRIPTED,             // 14
	SPRITEMOVEDATA_BIGDOLLSYM,           // 15
	SPRITEMOVEDATA_POKEMON,              // 16
	SPRITEMOVEDATA_SUDOWOODO,            // 17
	SPRITEMOVEDATA_SMASHABLE_ROCK,       // 18
	SPRITEMOVEDATA_STRENGTH_BOULDER,     // 19
	SPRITEMOVEDATA_FOLLOWNOTEXACT,       // 1a
	SPRITEMOVEDATA_SHADOW,               // 1b
	SPRITEMOVEDATA_EMOTE,                // 1c
	SPRITEMOVEDATA_SCREENSHAKE,          // 1d
	SPRITEMOVEDATA_SPINCOUNTERCLOCKWISE, // 1e
	SPRITEMOVEDATA_SPINCLOCKWISE,        // 1f
	SPRITEMOVEDATA_BIGDOLLASYM,          // 20
	SPRITEMOVEDATA_BIGDOLL,              // 21
	SPRITEMOVEDATA_BOULDERDUST,          // 22
	SPRITEMOVEDATA_GRASS,                // 23
	SPRITEMOVEDATA_SWIM_WANDER,          // 24
};
#define NUM_SPRITEMOVEDATA const_value

// MapObjectMovementPattern.Pointers indexes (see engine/overworld/map_objects.asm)
enum {
	SPRITEMOVEFN_00,                    // 00
	SPRITEMOVEFN_RANDOM_WALK_Y,         // 01
	SPRITEMOVEFN_RANDOM_WALK_X,         // 02
	SPRITEMOVEFN_RANDOM_WALK_XY,        // 03
	SPRITEMOVEFN_SLOW_RANDOM_SPIN,      // 04
	SPRITEMOVEFN_FAST_RANDOM_SPIN,      // 05
	SPRITEMOVEFN_STANDING,              // 06
	SPRITEMOVEFN_OBEY_DPAD,             // 07
	SPRITEMOVEFN_INDEXED_1,             // 08
	SPRITEMOVEFN_INDEXED_2,             // 09
	SPRITEMOVEFN_0A,                    // 0a
	SPRITEMOVEFN_0B,                    // 0b
	SPRITEMOVEFN_0C,                    // 0c
	SPRITEMOVEFN_0D,                    // 0d
	SPRITEMOVEFN_0E,                    // 0e
	SPRITEMOVEFN_FOLLOW,                // 0f
	SPRITEMOVEFN_SCRIPTED,              // 10
	SPRITEMOVEFN_STRENGTH,              // 11
	SPRITEMOVEFN_FOLLOWNOTEXACT,        // 12
	SPRITEMOVEFN_SHADOW,                // 13
	SPRITEMOVEFN_EMOTE,                 // 14
	SPRITEMOVEFN_BIG_SNORLAX,           // 15
	SPRITEMOVEFN_BOUNCE,                // 16
	SPRITEMOVEFN_SCREENSHAKE,           // 17
	SPRITEMOVEFN_SPIN_CLOCKWISE,        // 18
	SPRITEMOVEFN_SPIN_COUNTERCLOCKWISE, // 19
	SPRITEMOVEFN_BOULDERDUST,           // 1a
	SPRITEMOVEFN_GRASS,                 // 1b
};
#define NUM_SPRITEMOVEFN const_value

// StepTypesJumptable indexes (see engine/overworld/map_objects.asm)
enum {
	STEP_TYPE_RESET,            // 00
	STEP_TYPE_FROM_MOVEMENT,    // 01
	STEP_TYPE_NPC_WALK,         // 02
	STEP_TYPE_SLEEP,            // 03
	STEP_TYPE_STANDING,         // 04
	STEP_TYPE_RESTORE,          // 05
	STEP_TYPE_PLAYER_WALK,      // 06
	STEP_TYPE_CONTINUE_WALK,    // 07
	STEP_TYPE_NPC_JUMP,         // 08
	STEP_TYPE_PLAYER_JUMP,      // 09
	STEP_TYPE_TURN,             // 0a
	STEP_TYPE_BUMP,             // 0b
	STEP_TYPE_TELEPORT_FROM,    // 0c
	STEP_TYPE_TELEPORT_TO,      // 0d
	STEP_TYPE_SKYFALL,          // 0e
	STEP_TYPE_STRENGTH_BOULDER, // 0f
	STEP_TYPE_GOT_BITE,         // 10
	STEP_TYPE_ROCK_SMASH,       // 11
	STEP_TYPE_RETURN_DIG,       // 12
	STEP_TYPE_TRACKING_OBJECT,  // 13
	STEP_TYPE_14,               // 14
	STEP_TYPE_SCREENSHAKE,      // 15
	STEP_TYPE_16,               // 16
	STEP_TYPE_17,               // 17
	STEP_TYPE_DELETE,           // 18
};
#define NUM_STEP_TYPES const_value

// ObjectActionPairPointers indexes (see engine/overworld/map_object_action.asm)
enum {
	OBJECT_ACTION_00,            // 00
	OBJECT_ACTION_STAND,         // 01
	OBJECT_ACTION_STEP,          // 02
	OBJECT_ACTION_BUMP,          // 03
	OBJECT_ACTION_SPIN,          // 04
	OBJECT_ACTION_SPIN_FLICKER,  // 05
	OBJECT_ACTION_FISHING,       // 06
	OBJECT_ACTION_SHADOW,        // 07
	OBJECT_ACTION_EMOTE,         // 08
	OBJECT_ACTION_BIG_DOLL_SYM,  // 09
	OBJECT_ACTION_BOUNCE,        // 0a
	OBJECT_ACTION_WEIRD_TREE,    // 0b
	OBJECT_ACTION_BIG_DOLL_ASYM, // 0c
	OBJECT_ACTION_BIG_DOLL,      // 0d
	OBJECT_ACTION_BOULDER_DUST,  // 0e
	OBJECT_ACTION_GRASS_SHAKE,   // 0f
};
#define NUM_OBJECT_ACTIONS const_value

// Facings indexes (see data/sprites/facings.asm)
enum {
	FACING_STEP_DOWN_0,    // 00
	FACING_STEP_DOWN_1,    // 01
	FACING_STEP_DOWN_2,    // 02
	FACING_STEP_DOWN_3,    // 03
	FACING_STEP_UP_0,      // 04
	FACING_STEP_UP_1,      // 05
	FACING_STEP_UP_2,      // 06
	FACING_STEP_UP_3,      // 07
	FACING_STEP_LEFT_0,    // 08
	FACING_STEP_LEFT_1,    // 09
	FACING_STEP_LEFT_2,    // 0a
	FACING_STEP_LEFT_3,    // 0b
	FACING_STEP_RIGHT_0,   // 0c
	FACING_STEP_RIGHT_1,   // 0d
	FACING_STEP_RIGHT_2,   // 0e
	FACING_STEP_RIGHT_3,   // 0f
	FACING_FISH_DOWN,      // 10
	FACING_FISH_UP,        // 11
	FACING_FISH_LEFT,      // 12
	FACING_FISH_RIGHT,     // 13
	FACING_EMOTE,          // 14
	FACING_SHADOW,         // 15
	FACING_BIG_DOLL_ASYM,  // 16
	FACING_BIG_DOLL_SYM,   // 17
	FACING_WEIRD_TREE_0,   // 18
	FACING_WEIRD_TREE_1,   // 19
	FACING_WEIRD_TREE_2,   // 1a
	FACING_WEIRD_TREE_3,   // 1b
	FACING_BOULDER_DUST_1, // 1c
	FACING_BOULDER_DUST_2, // 1d
	FACING_GRASS_1,        // 1e
	FACING_GRASS_2,        // 1f
};
#define NUM_FACINGS const_value

// DoPlayerMovement.DoStep arguments (see engine/overworld/player_movement.asm)
enum {
	STEP_SLOW,          // 0
	STEP_WALK,          // 1
	STEP_BIKE,          // 2
	STEP_LEDGE,         // 3
	STEP_ICE,           // 4
	STEP_TURN,          // 5
	STEP_BACK_LEDGE,    // 6
	STEP_WALK_IN_PLACE, // 7
};
#define NUM_STEPS const_value
