// Tilesets indexes (see data/tilesets.asm)
enum {
    TILESET_JOHTO = 1,            // 01
    TILESET_JOHTO_MODERN,         // 02
    TILESET_KANTO,                // 03
    TILESET_HOUSE,                // 04
    TILESET_PLAYERS_HOUSE,        // 05
    TILESET_POKECENTER,           // 06
    TILESET_GATE,                 // 07
    TILESET_PORT,                 // 08
    TILESET_LAB,                  // 09
    TILESET_FACILITY,             // 0a
    TILESET_MART,                 // 0b
    TILESET_MANSION,              // 0c
    TILESET_GAME_CORNER,          // 0d
    TILESET_ELITE_FOUR_ROOM,      // 0e
    TILESET_TRADITIONAL_HOUSE,    // 0f
    TILESET_TRAIN_STATION,        // 10
    TILESET_CHAMPIONS_ROOM,       // 11
    TILESET_LIGHTHOUSE,           // 12
    TILESET_PLAYERS_ROOM,         // 13
    TILESET_TOWER,                // 14
    TILESET_CAVE,                 // 15
    TILESET_PARK,                 // 16
    TILESET_RUINS_OF_ALPH,        // 17
    TILESET_RADIO_TOWER,          // 18
    TILESET_UNDERGROUND,          // 19
    TILESET_ICE_PATH,             // 1a
    TILESET_DARK_CAVE,            // 1b
    TILESET_FOREST,               // 1c
};
#define NUM_TILESETS TILESET_FOREST

// wTileset struct size
#define TILESET_LENGTH 15

// roof length (see gfx/tilesets/roofs)
#define ROOF_LENGTH 9

// bg palette values (see gfx/tilesets/*_palette_map.asm)
// TilesetBGPalette indexes (see gfx/tilesets/bg_tiles.pal)
enum {
    PAL_BG_GRAY,   // 0
    PAL_BG_RED,    // 1
    PAL_BG_GREEN,  // 2
    PAL_BG_WATER,  // 3
    PAL_BG_YELLOW, // 4
    PAL_BG_BROWN,  // 5
    PAL_BG_ROOF,   // 6
    PAL_BG_TEXT,   // 7
};
