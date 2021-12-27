// decoration attributes
rsreset
DECOATTR_TYPE       rb
DECOATTR_NAME       rb
DECOATTR_ACTION     rb
DECOATTR_EVENT_FLAG rw
DECOATTR_SPRITE     rb
#define DECOATTR_STRUCT_LENGTH _RS

// decoration types
enum {
};
	DECO_PLANT,
	DECO_BED,
	DECO_CARPET,
	DECO_POSTER,
	DECO_DOLL,
	DECO_BIGDOLL,
#define NUM_DECO_TYPES const_value - 1

// DecorationNames indexes (see data/decorations/names.asm)
enum {
};
	CANCEL_DECO,
	PUT_IT_AWAY,
	MAGNAPLANT,
	TROPICPLANT,
	JUMBOPLANT,
	TOWN_MAP_POSTER,
	FAMICOM,
	SUPER_NES,
	NINTENDO_64,
	VIRTUAL_BOY,
	GOLD_TROPHY,
	SILVER_TROPHY,
	SURF_PIKA_DOLL,
	_BED,
	_CARPET,
	_POSTER,
	_DOLL,
	BIG_,
	FEATHERY_BED,
	PIKACHU_BED,
	PINK_BED,
	POLKADOT_BED,
	RED_CARPET,
	BLUE_CARPET,
	YELLOW_CARPET,
	GREEN_CARPET,
	NUM_DECO_NAMES,

// DoDecorationAction2.DecoActions indexes (see engine/overworld/decorations.asm)
enum {
};
	SET_UP_BED,
	PUT_AWAY_BED,
	SET_UP_CARPET,
	PUT_AWAY_CARPET,
	SET_UP_PLANT,
	PUT_AWAY_PLANT,
	SET_UP_POSTER,
	PUT_AWAY_POSTER,
	SET_UP_CONSOLE,
	PUT_AWAY_CONSOLE,
	SET_UP_BIG_DOLL,
	PUT_AWAY_BIG_DOLL,
	SET_UP_DOLL,
	PUT_AWAY_DOLL,
#define NUM_DECO_ACTIONS const_value - 1

__deco_value__ = 0

deco: MACRO
	DECO_,\1
DECOFLAG_\#define 1 __deco_value__
__deco_value__ = __deco_value__ + 1
ENDM

// decorations:
// - DecorationAttributes (see data/decorations/attributes.asm)
// - DecorationIDs (see data/decorations/decorations.asm)
enum {
};
// FindOwnedBeds.beds values (see engine/overworld/decorations.asm)
	BEDS,
	deco  FEATHERY_BED
	deco  PINK_BED
	deco  POLKADOT_BED
	deco  PIKACHU_BED
// FindOwnedCarpets.carpets values (see engine/overworld/decorations.asm)
	CARPETS,
	deco  RED_CARPET
	deco  BLUE_CARPET
	deco  YELLOW_CARPET
	deco  GREEN_CARPET
// FindOwnedPlants.plants values (see engine/overworld/decorations.asm)
	PLANTS,
	deco  MAGNAPLANT
	deco  TROPICPLANT
	deco  JUMBOPLANT
// FindOwnedPosters.posters values (see engine/overworld/decorations.asm)
	POSTERS,
	deco  TOWN_MAP
	deco  PIKACHU_POSTER
	deco  CLEFAIRY_POSTER
	deco  JIGGLYPUFF_POSTER
// FindOwnedConsoles.consoles values (see engine/overworld/decorations.asm)
	CONSOLES,
	deco  FAMICOM
	deco  SNES
	deco  N64
	deco  VIRTUAL_BOY
// FindOwnedBigDolls.big_dolls values (see engine/overworld/decorations.asm)
	BIG_DOLLS,
	deco  BIG_SNORLAX_DOLL
	deco  BIG_ONIX_DOLL
	deco  BIG_LAPRAS_DOLL
// FindOwnedOrnaments.ornaments values (see engine/overworld/decorations.asm)
	DOLLS,
	deco  PIKACHU_DOLL
	deco  SURF_PIKACHU_DOLL
	deco  CLEFAIRY_DOLL
	deco  JIGGLYPUFF_DOLL
	deco  BULBASAUR_DOLL
	deco  CHARMANDER_DOLL
	deco  SQUIRTLE_DOLL
	deco  POLIWAG_DOLL
	deco  DIGLETT_DOLL
	deco  STARMIE_DOLL
	deco  MAGIKARP_DOLL
	deco  ODDISH_DOLL
	deco  GENGAR_DOLL
	deco  SHELLDER_DOLL
	deco  GRIMER_DOLL
	deco  VOLTORB_DOLL
	deco  WEEDLE_DOLL
	deco  UNOWN_DOLL
	deco  GEODUDE_DOLL
	deco  MACHOP_DOLL
	deco  TENTACOOL_DOLL
#define NUM_NON_TROPHY_DECOS __deco_value__
	deco  GOLD_TROPHY_DOLL
	deco  SILVER_TROPHY_DOLL
#define NUM_DECOS __deco_value__
#define NUM_DECO_CATEGORIES const_value - 1 - NUM_DECOS
