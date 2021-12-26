// hMapEntryMethod values
// MapSetupScripts indexes (see data/maps/setup_scripts.asm)
enum {
	MAPSETUP_WARP,       // f1
	MAPSETUP_CONTINUE,   // f2
	MAPSETUP_RELOADMAP,  // f3
	MAPSETUP_TELEPORT,   // f4
	MAPSETUP_DOOR,       // f5
	MAPSETUP_FALL,       // f6
	MAPSETUP_CONNECTION, // f7
	MAPSETUP_LINKRETURN, // f8
	MAPSETUP_TRAIN,      // f9
	MAPSETUP_SUBMENU,    // fa
	MAPSETUP_BADWARP,    // fb
};
#define NUM_MAPSETUP_SCRIPTS const_value - 0xf1

// callback types
enum {
};
	MAPCALLBACK_TILES,
	MAPCALLBACK_OBJECTS,
	MAPCALLBACK_CMDQUEUE,
	MAPCALLBACK_SPRITES,
	MAPCALLBACK_NEWMAP,
