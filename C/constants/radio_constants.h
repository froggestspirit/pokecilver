// radio channel ids
// indexes for:
// - RadioChannelSongs (see data/radio/channel_music.asm)
// - PlayRadioShow/RadioJumptable (see engine/pokegear/radio.asm)
// - RadioChannels (see engine/pokegear/pokegear.asm)
enum {
	OAKS_POKEMON_TALK,      // 00
	POKEDEX_SHOW,           // 01
	POKEMON_MUSIC,          // 02
	LUCKY_CHANNEL,          // 03
	PLACES_AND_PEOPLE,      // 04
	LETS_ALL_SING,          // 05
	ROCKET_RADIO,           // 06
	POKE_FLUTE_RADIO,       // 07
	UNOWN_RADIO,            // 08
	EVOLUTION_RADIO,        // 09
};
	NUM_RADIO_CHANNELS,
// internal indexes for channel segments
	OAKS_POKEMON_TALK_2,    // 0a
	OAKS_POKEMON_TALK_3,    // 0b
	OAKS_POKEMON_TALK_4,    // 0c
	OAKS_POKEMON_TALK_5,    // 0d
	OAKS_POKEMON_TALK_6,    // 0e
	OAKS_POKEMON_TALK_7,    // 0f
	OAKS_POKEMON_TALK_8,    // 10
	OAKS_POKEMON_TALK_9,    // 11
	POKEDEX_SHOW_2,         // 12
	POKEDEX_SHOW_3,         // 13
	POKEDEX_SHOW_4,         // 14
	POKEDEX_SHOW_5,         // 15
	POKEMON_MUSIC_2,        // 16
	POKEMON_MUSIC_3,        // 17
	POKEMON_MUSIC_4,        // 18
	POKEMON_MUSIC_5,        // 19
	POKEMON_MUSIC_6,        // 1a
	POKEMON_MUSIC_7,        // 1b
	LETS_ALL_SING_2,        // 1c
	LUCKY_NUMBER_SHOW_2,    // 1d
	LUCKY_NUMBER_SHOW_3,    // 1e
	LUCKY_NUMBER_SHOW_4,    // 1f
	LUCKY_NUMBER_SHOW_5,    // 20
	LUCKY_NUMBER_SHOW_6,    // 21
	LUCKY_NUMBER_SHOW_7,    // 22
	LUCKY_NUMBER_SHOW_8,    // 23
	LUCKY_NUMBER_SHOW_9,    // 24
	LUCKY_NUMBER_SHOW_10,   // 25
	LUCKY_NUMBER_SHOW_11,   // 26
	LUCKY_NUMBER_SHOW_12,   // 27
	LUCKY_NUMBER_SHOW_13,   // 28
	LUCKY_NUMBER_SHOW_14,   // 29
	LUCKY_NUMBER_SHOW_15,   // 2a
	PLACES_AND_PEOPLE_2,    // 2b
	PLACES_AND_PEOPLE_3,    // 2c
	PLACES_AND_PEOPLE_4,    // 2d
	PLACES_AND_PEOPLE_5,    // 2e
	PLACES_AND_PEOPLE_6,    // 2f
	PLACES_AND_PEOPLE_7,    // 30
	ROCKET_RADIO_2,         // 31
	ROCKET_RADIO_3,         // 32
	ROCKET_RADIO_4,         // 33
	ROCKET_RADIO_5,         // 34
	ROCKET_RADIO_6,         // 35
	ROCKET_RADIO_7,         // 36
	ROCKET_RADIO_8,         // 37
	ROCKET_RADIO_9,         // 38
	ROCKET_RADIO_10,        // 39
	OAKS_POKEMON_TALK_10,   // 3a
	OAKS_POKEMON_TALK_11,   // 3b
	OAKS_POKEMON_TALK_12,   // 3c
	OAKS_POKEMON_TALK_13,   // 3d
	OAKS_POKEMON_TALK_14,   // 3e
	RADIO_SCROLL,           // 3f
	POKEDEX_SHOW_6,         // 40
	POKEDEX_SHOW_7,         // 41
	POKEDEX_SHOW_8,         // 42
	NUM_RADIO_SEGMENTS,

// PlayRadioStationPointers indexes (see engine/pokegear/pokegear.asm)
enum {
};
	MAPRADIO_POKEMON_CHANNEL,
	MAPRADIO_OAKS_POKEMON_TALK,
	MAPRADIO_POKEDEX_SHOW,
	MAPRADIO_POKEMON_MUSIC,
	MAPRADIO_LUCKY_CHANNEL,
	MAPRADIO_UNOWN,
	MAPRADIO_PLACES_PEOPLE,
	MAPRADIO_LETS_ALL_SING,
	MAPRADIO_ROCKET,
	NUM_MAP_RADIO_STATIONS,

// These tables in engine/pokegear/radio.asm are all sized to a power of 2
// so there's no need for a rejection sampling loop
#define NUM_OAKS_POKEMON_TALK_ADVERBS 16 // OaksPKMNTalk8.Adverbs
#define NUM_OAKS_POKEMON_TALK_ADJECTIVES 16 // OaksPKMNTalk9.Adjectives
#define NUM_PNP_PEOPLE_ADJECTIVES 16 // PeoplePlaces5.Adjectives
#define NUM_PNP_PLACES_ADJECTIVES 16 // PeoplePlaces7.Adjectives
