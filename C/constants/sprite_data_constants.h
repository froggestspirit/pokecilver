// overworld_sprite struct members (see data/sprites/sprites.asm)
rsreset
SPRITEDATA_ADDR    rw // 0
SPRITEDATA_SIZE    rb // 2
SPRITEDATA_BANK    rb // 3
SPRITEDATA_TYPE    rb // 4
SPRITEDATA_PALETTE rb // 5
#define NUM_SPRITEDATA_FIELDS _RS

// sprite types
enum {
	WALKING_SPRITE,  // 1
	STANDING_SPRITE, // 2
	STILL_SPRITE,    // 3
};

// sprite palettes
enum {
	PAL_OW_RED,    // 0
	PAL_OW_BLUE,   // 1
	PAL_OW_GREEN,  // 2
	PAL_OW_BROWN,  // 3
	PAL_OW_PINK,   // 4
	PAL_OW_SILVER, // 5
	PAL_OW_TREE,   // 6
	PAL_OW_ROCK,   // 7
};

// object_events set bit 3 so as not to use the sprite's default palette
// MapObjectPals indexes (see gfx/overworld/npc_sprites.pal)
enum {
}; << 3
	PAL_NPC_RED,    // 8
	PAL_NPC_BLUE,   // 9
	PAL_NPC_GREEN,  // a
	PAL_NPC_BROWN,  // b
	PAL_NPC_PINK,   // c
	PAL_NPC_SILVER, // d
	PAL_NPC_TREE,   // e
	PAL_NPC_ROCK,   // f
