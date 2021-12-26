// TypeNames indexes (see data/types/names.asm)
// also used in:
// - PokedexTypeSearchConversionTable (see data/types/search_types.asm)
// - PokedexTypeSearchStrings (see data/types/search_strings.asm)
// - TypeMatchups (see data/types/type_matchups.asm)
// - TypeBoostItems (see data/types/type_boost_items.asm)
enum {
};

#define PHYSICAL const_value
	NORMAL,
	FIGHTING,
	FLYING,
	POISON,
	GROUND,
	ROCK,
	BIRD,
	BUG,
	GHOST,
	STEEL,

#define UNUSED_TYPES const_value
	_next, 19
	CURSE_TYPE,
#define UNUSED_TYPES_END const_value

#define SPECIAL const_value
	FIRE,
	WATER,
	GRASS,
	ELECTRIC,
	PSYCHIC_TYPE,
	ICE,
	DRAGON,
	DARK,
#define TYPES_END const_value

#define NUM_TYPES TYPES_END + UNUSED_TYPES - UNUSED_TYPES_END - 1 // discount BIRD

#define POKEDEX_TYPE_STRING_LENGTH 9
