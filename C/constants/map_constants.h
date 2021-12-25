

// map group ids
// `newgroup` indexes are for:
// - MapGroupPointers (see data/maps/maps.asm)
// - MapGroupRoofs (see data/maps/roofs.asm)
// - OutdoorSprites (see data/maps/outdoor_sprites.asm)
// - RoofPals (see gfx/tilesets/roofs.pal)
// `map_const` indexes are for the sub-tables of MapGroupPointers (see data/maps/maps.asm)
// Each map also has associated data:
// - attributes (see data/maps/attributes.asm)
// - blocks (see data/maps/blocks.asm)
// - scripts and events (see data/maps/scripts.asm)

enum {
    MAPGROUP_OLIVINE = 1,
    MAPGROUP_MAHOGANY,
    MAPGROUP_DUNGEONS,
    MAPGROUP_ECRUTEAK,
    MAPGROUP_BLACKTHORN,
    MAPGROUP_CINNABAR,
    MAPGROUP_CERULEAN,
    MAPGROUP_AZALEA,
    MAPGROUP_LAKE_OF_RAGE,
    MAPGROUP_VIOLET,
    MAPGROUP_GOLDENROD,
    MAPGROUP_VERMILION,
    MAPGROUP_PALLET,
    MAPGROUP_PEWTER,
    MAPGROUP_FAST_SHIP,
    MAPGROUP_INDIGO,
    MAPGROUP_FUCHSIA,
    MAPGROUP_LAVENDER,
    MAPGROUP_SILVER,
    MAPGROUP_CABLE_CLUB,
    MAPGROUP_CELADON,
    MAPGROUP_CIANWOOD,
    MAPGROUP_VIRIDIAN,
    MAPGROUP_NEW_BARK,
    MAPGROUP_SAFFRON,
    MAPGROUP_CHERRYGROVE,
};

#define NUM_MAP_GROUPS MAPGROUP_CHERRYGROVE

#define map_const(name, x, w, h)  enum {\
                                GROUP_##name = CURRENT_MAP_GROUP,\
                                MAP_##name = x,\
                                name##_WIDTH = w,\
                                name##_HEIGHT = h,\
                            };\

#define CURRENT_MAP_GROUP  MAPGROUP_OLIVINE
map_const(OLIVINE_POKECENTER_1F, 1, 5, 4)
map_const(OLIVINE_GYM, 2, 5, 8)
map_const(OLIVINE_TIMS_HOUSE, 3, 4, 4)
map_const(OLIVINE_HOUSE_BETA, 4, 4, 4)
map_const(OLIVINE_PUNISHMENT_SPEECH_HOUSE, 5, 4, 4)
map_const(OLIVINE_GOOD_ROD_HOUSE, 6, 4, 4)
map_const(OLIVINE_CAFE, 7, 4, 4)
map_const(OLIVINE_MART, 8, 6, 4)
map_const(ROUTE_38_ECRUTEAK_GATE, 9, 5, 4)
map_const(ROUTE_39_BARN, 10, 4, 4)
map_const(ROUTE_39_FARMHOUSE, 11, 4, 4)
map_const(ROUTE_38, 12, 20, 9)
map_const(ROUTE_39, 13, 10, 18)
map_const(OLIVINE_CITY, 14, 20, 18)

#define CURRENT_MAP_GROUP  MAPGROUP_MAHOGANY
map_const(MAHOGANY_RED_GYARADOS_SPEECH_HOUSE, 1, 4, 4)
map_const(MAHOGANY_GYM, 2, 5, 9)
map_const(MAHOGANY_POKECENTER_1F, 3, 5, 4)
map_const(ROUTE_42_ECRUTEAK_GATE, 4, 5, 4)
map_const(ROUTE_42, 5, 30, 9)
map_const(ROUTE_44, 6, 30, 9)
map_const(MAHOGANY_TOWN, 7, 10, 9)

#define CURRENT_MAP_GROUP  MAPGROUP_DUNGEONS
map_const(SPROUT_TOWER_1F, 1, 10, 8)
map_const(SPROUT_TOWER_2F, 2, 10, 8)
map_const(SPROUT_TOWER_3F, 3, 10, 8)
map_const(TIN_TOWER_1F, 4, 10, 9)
map_const(TIN_TOWER_2F, 5, 10, 9)
map_const(TIN_TOWER_3F, 6, 10, 9)
map_const(TIN_TOWER_4F, 7, 10, 9)
map_const(TIN_TOWER_5F, 8, 10, 9)
map_const(TIN_TOWER_6F, 9, 10, 9)
map_const(TIN_TOWER_7F, 10, 10, 9)
map_const(TIN_TOWER_8F, 11, 10, 9)
map_const(TIN_TOWER_9F, 12, 10, 9)
map_const(BURNED_TOWER_1F, 13, 10, 9)
map_const(BURNED_TOWER_B1F, 14, 10, 9)
map_const(NATIONAL_PARK, 15, 20, 27)
map_const(NATIONAL_PARK_BUG_CONTEST, 16, 20, 27)
map_const(RADIO_TOWER_1F, 17, 9, 4)
map_const(RADIO_TOWER_2F, 18, 9, 4)
map_const(RADIO_TOWER_3F, 19, 9, 4)
map_const(RADIO_TOWER_4F, 20, 9, 4)
map_const(RADIO_TOWER_5F, 21, 9, 4)
map_const(RUINS_OF_ALPH_OUTSIDE, 22, 10, 18)
map_const(RUINS_OF_ALPH_HO_OH_CHAMBER, 23, 4, 5)
map_const(RUINS_OF_ALPH_KABUTO_CHAMBER, 24, 4, 5)
map_const(RUINS_OF_ALPH_OMANYTE_CHAMBER, 25, 4, 5)
map_const(RUINS_OF_ALPH_AERODACTYL_CHAMBER, 26, 4, 5)
map_const(RUINS_OF_ALPH_INNER_CHAMBER, 27, 10, 14)
map_const(RUINS_OF_ALPH_RESEARCH_CENTER, 28, 4, 4)
map_const(UNION_CAVE_1F, 29, 10, 18)
map_const(UNION_CAVE_B1F, 30, 10, 18)
map_const(UNION_CAVE_B2F, 31, 10, 18)
map_const(SLOWPOKE_WELL_B1F, 32, 10, 9)
map_const(SLOWPOKE_WELL_B2F, 33, 10, 9)
map_const(OLIVINE_LIGHTHOUSE_1F, 34, 10, 9)
map_const(OLIVINE_LIGHTHOUSE_2F, 35, 10, 9)
map_const(OLIVINE_LIGHTHOUSE_3F, 36, 10, 9)
map_const(OLIVINE_LIGHTHOUSE_4F, 37, 10, 9)
map_const(OLIVINE_LIGHTHOUSE_5F, 38, 10, 9)
map_const(OLIVINE_LIGHTHOUSE_6F, 39, 10, 9)
map_const(MAHOGANY_MART_1F, 40, 4, 4)
map_const(TEAM_ROCKET_BASE_B1F, 41, 15, 9)
map_const(TEAM_ROCKET_BASE_B2F, 42, 15, 9)
map_const(TEAM_ROCKET_BASE_B3F, 43, 15, 9)
map_const(ILEX_FOREST, 44, 15, 27)
map_const(GOLDENROD_UNDERGROUND, 45, 15, 18)
map_const(GOLDENROD_UNDERGROUND_SWITCH_ROOM_ENTRANCES, 46, 15, 18)
map_const(GOLDENROD_DEPT_STORE_B1F, 47, 10, 9)
map_const(GOLDENROD_UNDERGROUND_WAREHOUSE, 48, 10, 9)
map_const(MOUNT_MORTAR_1F_OUTSIDE, 49, 20, 18)
map_const(MOUNT_MORTAR_1F_INSIDE, 50, 20, 27)
map_const(MOUNT_MORTAR_2F_INSIDE, 51, 20, 18)
map_const(MOUNT_MORTAR_B1F, 52, 20, 18)
map_const(ICE_PATH_1F, 53, 20, 18)
map_const(ICE_PATH_B1F, 54, 10, 18)
map_const(ICE_PATH_B2F_MAHOGANY_SIDE, 55, 10, 9)
map_const(ICE_PATH_B2F_BLACKTHORN_SIDE, 56, 5, 9)
map_const(ICE_PATH_B3F, 57, 10, 9)
map_const(WHIRL_ISLAND_NW, 58, 5, 9)
map_const(WHIRL_ISLAND_NE, 59, 10, 9)
map_const(WHIRL_ISLAND_SW, 60, 10, 9)
map_const(WHIRL_ISLAND_CAVE, 61, 5, 9)
map_const(WHIRL_ISLAND_SE, 62, 5, 9)
map_const(WHIRL_ISLAND_B1F, 63, 20, 18)
map_const(WHIRL_ISLAND_B2F, 64, 10, 18)
map_const(WHIRL_ISLAND_LUGIA_CHAMBER, 65, 10, 9)
map_const(SILVER_CAVE_ROOM_1, 66, 10, 18)
map_const(SILVER_CAVE_ROOM_2, 67, 15, 18)
map_const(SILVER_CAVE_ROOM_3, 68, 10, 18)
map_const(SILVER_CAVE_ITEM_ROOMS, 69, 10, 9)
map_const(DARK_CAVE_VIOLET_ENTRANCE, 70, 20, 18)
map_const(DARK_CAVE_BLACKTHORN_ENTRANCE, 71, 15, 18)
map_const(DRAGONS_DEN_1F, 72, 5, 9)
map_const(DRAGONS_DEN_B1F, 73, 20, 18)
map_const(TOHJO_FALLS, 74, 15, 9)
map_const(DIGLETTS_CAVE, 75, 10, 18)
map_const(MOUNT_MOON, 76, 15, 9)
map_const(UNDERGROUND_PATH, 77, 3, 14)
map_const(ROCK_TUNNEL_1F, 78, 15, 18)
map_const(ROCK_TUNNEL_B1F, 79, 15, 18)
map_const(SAFARI_ZONE_FUCHSIA_GATE_BETA, 80, 5, 4)
map_const(SAFARI_ZONE_BETA, 81, 10, 18)
map_const(VICTORY_ROAD, 82, 10, 36)

#define CURRENT_MAP_GROUP  MAPGROUP_ECRUTEAK
map_const(ECRUTEAK_TIN_TOWER_ENTRANCE, 1, 10, 9)
map_const(ECRUTEAK_TIN_TOWER_BACK_ENTRANCE, 2, 4, 4)
map_const(ECRUTEAK_POKECENTER_1F, 3, 5, 4)
map_const(ECRUTEAK_LUGIA_SPEECH_HOUSE, 4, 4, 4)
map_const(DANCE_THEATRE, 5, 6, 7)
map_const(ECRUTEAK_MART, 6, 6, 4)
map_const(ECRUTEAK_GYM, 7, 5, 9)
map_const(ECRUTEAK_ITEMFINDER_HOUSE, 8, 4, 4)
map_const(ECRUTEAK_CITY, 9, 20, 18)

#define CURRENT_MAP_GROUP  MAPGROUP_BLACKTHORN
map_const(BLACKTHORN_GYM_1F, 1, 5, 9)
map_const(BLACKTHORN_GYM_2F, 2, 5, 9)
map_const(BLACKTHORN_DRAGON_SPEECH_HOUSE, 3, 4, 4)
map_const(BLACKTHORN_EMYS_HOUSE, 4, 4, 4)
map_const(BLACKTHORN_MART, 5, 6, 4)
map_const(BLACKTHORN_POKECENTER_1F, 6, 5, 4)
map_const(MOVE_DELETERS_HOUSE, 7, 4, 4)
map_const(ROUTE_45, 8, 10, 45)
map_const(ROUTE_46, 9, 10, 18)
map_const(BLACKTHORN_CITY, 10, 20, 18)

#define CURRENT_MAP_GROUP  MAPGROUP_CINNABAR
map_const(CINNABAR_POKECENTER_1F, 1, 5, 4)
map_const(CINNABAR_POKECENTER_2F_BETA, 2, 8, 4)
map_const(ROUTE_19_FUCHSIA_GATE, 3, 5, 4)
map_const(SEAFOAM_GYM, 4, 5, 4)
map_const(ROUTE_19, 5, 10, 18)
map_const(ROUTE_20, 6, 30, 9)
map_const(ROUTE_21, 7, 10, 18)
map_const(CINNABAR_ISLAND, 8, 10, 9)

#define CURRENT_MAP_GROUP  MAPGROUP_CERULEAN
map_const(CERULEAN_GYM_BADGE_SPEECH_HOUSE, 1, 4, 4)
map_const(CERULEAN_POLICE_STATION, 2, 4, 4)
map_const(CERULEAN_TRADE_SPEECH_HOUSE, 3, 4, 4)
map_const(CERULEAN_POKECENTER_1F, 4, 5, 4)
map_const(CERULEAN_POKECENTER_2F_BETA, 5, 8, 4)
map_const(CERULEAN_GYM, 6, 5, 8)
map_const(CERULEAN_MART, 7, 6, 4)
map_const(ROUTE_10_POKECENTER_1F, 8, 5, 4)
map_const(ROUTE_10_POKECENTER_2F_BETA, 9, 8, 4)
map_const(POWER_PLANT, 10, 10, 9)
map_const(BILLS_HOUSE, 11, 4, 4)
map_const(ROUTE_4, 12, 20, 9)
map_const(ROUTE_9, 13, 30, 9)
map_const(ROUTE_10_NORTH, 14, 10, 9)
map_const(ROUTE_24, 15, 10, 9)
map_const(ROUTE_25, 16, 30, 9)
map_const(CERULEAN_CITY, 17, 20, 18)

#define CURRENT_MAP_GROUP  MAPGROUP_AZALEA
map_const(AZALEA_POKECENTER_1F, 1, 5, 4)
map_const(CHARCOAL_KILN, 2, 4, 4)
map_const(AZALEA_MART, 3, 6, 4)
map_const(KURTS_HOUSE, 4, 8, 4)
map_const(AZALEA_GYM, 5, 5, 8)
map_const(ROUTE_33, 6, 10, 9)
map_const(AZALEA_TOWN, 7, 20, 9)

#define CURRENT_MAP_GROUP  MAPGROUP_LAKE_OF_RAGE
map_const(LAKE_OF_RAGE_HIDDEN_POWER_HOUSE, 1, 4, 4)
map_const(LAKE_OF_RAGE_MAGIKARP_HOUSE, 2, 4, 4)
map_const(ROUTE_43_MAHOGANY_GATE, 3, 5, 4)
map_const(ROUTE_43_GATE, 4, 5, 4)
map_const(ROUTE_43, 5, 10, 27)
map_const(LAKE_OF_RAGE, 6, 20, 18)

#define CURRENT_MAP_GROUP  MAPGROUP_VIOLET
map_const(ROUTE_32, 1, 10, 45)
map_const(ROUTE_35, 2, 10, 18)
map_const(ROUTE_36, 3, 30, 9)
map_const(ROUTE_37, 4, 10, 9)
map_const(VIOLET_CITY, 5, 20, 18)
map_const(VIOLET_MART, 6, 6, 4)
map_const(VIOLET_GYM, 7, 5, 8)
map_const(EARLS_POKEMON_ACADEMY, 8, 4, 8)
map_const(VIOLET_NICKNAME_SPEECH_HOUSE, 9, 4, 4)
map_const(VIOLET_POKECENTER_1F, 10, 5, 4)
map_const(VIOLET_KYLES_HOUSE, 11, 4, 4)
map_const(ROUTE_32_RUINS_OF_ALPH_GATE, 12, 5, 4)
map_const(ROUTE_32_POKECENTER_1F, 13, 5, 4)
map_const(ROUTE_35_GOLDENROD_GATE, 14, 5, 4)
map_const(ROUTE_35_NATIONAL_PARK_GATE, 15, 4, 4)
map_const(ROUTE_36_RUINS_OF_ALPH_GATE, 16, 5, 4)
map_const(ROUTE_36_NATIONAL_PARK_GATE, 17, 5, 4)

#define CURRENT_MAP_GROUP  MAPGROUP_GOLDENROD
map_const(ROUTE_34, 1, 10, 27)
map_const(GOLDENROD_CITY, 2, 20, 18)
map_const(GOLDENROD_GYM, 3, 10, 9)
map_const(GOLDENROD_BIKE_SHOP, 4, 4, 4)
map_const(GOLDENROD_HAPPINESS_RATER, 5, 4, 4)
map_const(BILLS_FAMILYS_HOUSE, 6, 4, 4)
map_const(GOLDENROD_MAGNET_TRAIN_STATION, 7, 10, 9)
map_const(GOLDENROD_FLOWER_SHOP, 8, 4, 4)
map_const(GOLDENROD_POKECENTER_1F, 9, 5, 4)
map_const(GOLDENROD_PP_SPEECH_HOUSE, 10, 4, 4)
map_const(GOLDENROD_NAME_RATER, 11, 4, 4)
map_const(GOLDENROD_DEPT_STORE_1F, 12, 8, 4)
map_const(GOLDENROD_DEPT_STORE_2F, 13, 8, 4)
map_const(GOLDENROD_DEPT_STORE_3F, 14, 8, 4)
map_const(GOLDENROD_DEPT_STORE_4F, 15, 8, 4)
map_const(GOLDENROD_DEPT_STORE_5F, 16, 8, 4)
map_const(GOLDENROD_DEPT_STORE_6F, 17, 8, 4)
map_const(GOLDENROD_DEPT_STORE_ELEVATOR, 18, 2, 2)
map_const(GOLDENROD_GAME_CORNER, 19, 10, 7)
map_const(ILEX_FOREST_AZALEA_GATE, 20, 5, 4)
map_const(ROUTE_34_ILEX_FOREST_GATE, 21, 5, 4)
map_const(DAY_CARE, 22, 5, 4)

#define CURRENT_MAP_GROUP  MAPGROUP_VERMILION
map_const(ROUTE_6, 1, 10, 9)
map_const(ROUTE_11, 2, 20, 9)
map_const(VERMILION_CITY, 3, 20, 18)
map_const(VERMILION_FISHING_SPEECH_HOUSE, 4, 4, 4)
map_const(VERMILION_POKECENTER_1F, 5, 5, 4)
map_const(VERMILION_POKECENTER_2F_BETA, 6, 8, 4)
map_const(POKEMON_FAN_CLUB, 7, 5, 4)
map_const(VERMILION_MAGNET_TRAIN_SPEECH_HOUSE, 8, 4, 4)
map_const(VERMILION_MART, 9, 6, 4)
map_const(VERMILION_DIGLETTS_CAVE_SPEECH_HOUSE, 10, 4, 4)
map_const(VERMILION_GYM, 11, 5, 9)
map_const(ROUTE_6_SAFFRON_GATE, 12, 5, 4)
map_const(ROUTE_6_UNDERGROUND_PATH_ENTRANCE, 13, 4, 4)

#define CURRENT_MAP_GROUP  MAPGROUP_PALLET
map_const(ROUTE_1, 1, 10, 18)
map_const(PALLET_TOWN, 2, 10, 9)
map_const(REDS_HOUSE_1F, 3, 4, 4)
map_const(REDS_HOUSE_2F, 4, 4, 4)
map_const(BLUES_HOUSE, 5, 4, 4)
map_const(OAKS_LAB, 6, 5, 6)

#define CURRENT_MAP_GROUP  MAPGROUP_PEWTER
map_const(ROUTE_3, 1, 30, 9)
map_const(PEWTER_CITY, 2, 20, 18)
map_const(PEWTER_NIDORAN_SPEECH_HOUSE, 3, 4, 4)
map_const(PEWTER_GYM, 4, 5, 7)
map_const(PEWTER_MART, 5, 6, 4)
map_const(PEWTER_POKECENTER_1F, 6, 5, 4)
map_const(PEWTER_POKECENTER_2F_BETA, 7, 8, 4)
map_const(PEWTER_SNOOZE_SPEECH_HOUSE, 8, 4, 4)

#define CURRENT_MAP_GROUP  MAPGROUP_FAST_SHIP
map_const(OLIVINE_PORT, 1, 10, 18)
map_const(VERMILION_PORT, 2, 10, 18)
map_const(FAST_SHIP_1F, 3, 16, 9)
map_const(FAST_SHIP_CABINS_NNW_NNE_NE, 4, 4, 16)
map_const(FAST_SHIP_CABINS_SW_SSW_NW, 5, 4, 16)
map_const(FAST_SHIP_CABINS_SE_SSE_CAPTAINS_CABIN, 6, 5, 17)
map_const(FAST_SHIP_B1F, 7, 16, 8)
map_const(OLIVINE_PORT_PASSAGE, 8, 10, 9)
map_const(VERMILION_PORT_PASSAGE, 9, 10, 9)
map_const(MOUNT_MOON_SQUARE, 10, 15, 9)
map_const(MOUNT_MOON_GIFT_SHOP, 11, 4, 4)
map_const(TIN_TOWER_ROOF, 12, 10, 9)

#define CURRENT_MAP_GROUP  MAPGROUP_INDIGO
map_const(ROUTE_23, 1, 10, 9)
map_const(INDIGO_PLATEAU_POKECENTER_1F, 2, 9, 7)
map_const(WILLS_ROOM, 3, 5, 9)
map_const(KOGAS_ROOM, 4, 5, 9)
map_const(BRUNOS_ROOM, 5, 5, 9)
map_const(KARENS_ROOM, 6, 5, 9)
map_const(LANCES_ROOM, 7, 5, 12)
map_const(HALL_OF_FAME, 8, 5, 7)

#define CURRENT_MAP_GROUP  MAPGROUP_FUCHSIA
map_const(ROUTE_13, 1, 30, 9)
map_const(ROUTE_14, 2, 10, 18)
map_const(ROUTE_15, 3, 20, 9)
map_const(ROUTE_18, 4, 10, 9)
map_const(FUCHSIA_CITY, 5, 20, 18)
map_const(FUCHSIA_MART, 6, 6, 4)
map_const(SAFARI_ZONE_MAIN_OFFICE, 7, 4, 4)
map_const(FUCHSIA_GYM, 8, 5, 9)
map_const(BILLS_BROTHERS_HOUSE, 9, 4, 4)
map_const(FUCHSIA_POKECENTER_1F, 10, 5, 4)
map_const(FUCHSIA_POKECENTER_2F_BETA, 11, 8, 4)
map_const(SAFARI_ZONE_WARDENS_HOME, 12, 5, 4)
map_const(ROUTE_15_FUCHSIA_GATE, 13, 5, 4)

#define CURRENT_MAP_GROUP  MAPGROUP_LAVENDER
map_const(ROUTE_8, 1, 20, 9)
map_const(ROUTE_12, 2, 10, 27)
map_const(ROUTE_10_SOUTH, 3, 10, 9)
map_const(LAVENDER_TOWN, 4, 10, 9)
map_const(LAVENDER_POKECENTER_1F, 5, 5, 4)
map_const(LAVENDER_POKECENTER_2F_BETA, 6, 8, 4)
map_const(MR_FUJIS_HOUSE, 7, 5, 4)
map_const(LAVENDER_SPEECH_HOUSE, 8, 4, 4)
map_const(LAVENDER_NAME_RATER, 9, 4, 4)
map_const(LAVENDER_MART, 10, 6, 4)
map_const(SOUL_HOUSE, 11, 5, 4)
map_const(LAV_RADIO_TOWER_1F, 12, 10, 4)
map_const(ROUTE_8_SAFFRON_GATE, 13, 5, 4)
map_const(ROUTE_12_SUPER_ROD_HOUSE, 14, 4, 4)

#define CURRENT_MAP_GROUP  MAPGROUP_SILVER
map_const(ROUTE_28, 1, 20, 9)
map_const(SILVER_CAVE_OUTSIDE, 2, 20, 18)
map_const(SILVER_CAVE_POKECENTER_1F, 3, 5, 4)
map_const(ROUTE_28_STEEL_WING_HOUSE, 4, 4, 4)

#define CURRENT_MAP_GROUP  MAPGROUP_CABLE_CLUB
map_const(POKECENTER_2F, 1, 8, 4)
map_const(TRADE_CENTER, 2, 5, 4)
map_const(COLOSSEUM, 3, 5, 4)
map_const(TIME_CAPSULE, 4, 5, 4)

#define CURRENT_MAP_GROUP  MAPGROUP_CELADON
map_const(ROUTE_7, 1, 10, 9)
map_const(ROUTE_16, 2, 10, 9)
map_const(ROUTE_17, 3, 10, 45)
map_const(CELADON_CITY, 4, 20, 18)
map_const(CELADON_DEPT_STORE_1F, 5, 8, 4)
map_const(CELADON_DEPT_STORE_2F, 6, 8, 4)
map_const(CELADON_DEPT_STORE_3F, 7, 8, 4)
map_const(CELADON_DEPT_STORE_4F, 8, 8, 4)
map_const(CELADON_DEPT_STORE_5F, 9, 8, 4)
map_const(CELADON_DEPT_STORE_6F, 10, 8, 4)
map_const(CELADON_DEPT_STORE_ELEVATOR, 11, 2, 2)
map_const(CELADON_MANSION_1F, 12, 4, 5)
map_const(CELADON_MANSION_2F, 13, 4, 5)
map_const(CELADON_MANSION_3F, 14, 4, 5)
map_const(CELADON_MANSION_ROOF, 15, 4, 5)
map_const(CELADON_MANSION_ROOF_HOUSE, 16, 4, 4)
map_const(CELADON_POKECENTER_1F, 17, 5, 4)
map_const(CELADON_POKECENTER_2F_BETA, 18, 8, 4)
map_const(CELADON_GAME_CORNER, 19, 10, 7)
map_const(CELADON_GAME_CORNER_PRIZE_ROOM, 20, 3, 3)
map_const(CELADON_GYM, 21, 5, 9)
map_const(CELADON_CAFE, 22, 6, 4)
map_const(ROUTE_16_FUCHSIA_SPEECH_HOUSE, 23, 4, 4)
map_const(ROUTE_16_GATE, 24, 5, 4)
map_const(ROUTE_7_SAFFRON_GATE, 25, 5, 4)
map_const(ROUTE_17_ROUTE_18_GATE, 26, 5, 4)

#define CURRENT_MAP_GROUP  MAPGROUP_CIANWOOD
map_const(ROUTE_40, 1, 10, 18)
map_const(ROUTE_41, 2, 25, 27)
map_const(CIANWOOD_CITY, 3, 15, 27)
map_const(MANIAS_HOUSE, 4, 4, 4)
map_const(CIANWOOD_GYM, 5, 5, 9)
map_const(CIANWOOD_POKECENTER_1F, 6, 5, 4)
map_const(CIANWOOD_PHARMACY, 7, 4, 4)
map_const(CIANWOOD_PHOTO_STUDIO, 8, 4, 4)
map_const(CIANWOOD_LUGIA_SPEECH_HOUSE, 9, 4, 4)

#define CURRENT_MAP_GROUP  MAPGROUP_VIRIDIAN
map_const(ROUTE_2, 1, 10, 27)
map_const(ROUTE_22, 2, 20, 9)
map_const(VIRIDIAN_CITY, 3, 20, 18)
map_const(VIRIDIAN_GYM, 4, 5, 9)
map_const(VIRIDIAN_NICKNAME_SPEECH_HOUSE, 5, 4, 4)
map_const(TRAINER_HOUSE_1F, 6, 5, 7)
map_const(TRAINER_HOUSE_B1F, 7, 5, 8)
map_const(VIRIDIAN_MART, 8, 6, 4)
map_const(VIRIDIAN_POKECENTER_1F, 9, 5, 4)
map_const(VIRIDIAN_POKECENTER_2F_BETA, 10, 8, 4)
map_const(ROUTE_2_NUGGET_HOUSE, 11, 4, 4)
map_const(ROUTE_2_GATE, 12, 5, 4)
map_const(VICTORY_ROAD_GATE, 13, 10, 9)

#define CURRENT_MAP_GROUP  MAPGROUP_NEW_BARK
map_const(ROUTE_26, 1, 10, 54)
map_const(ROUTE_27, 2, 40, 9)
map_const(ROUTE_29, 3, 30, 9)
map_const(NEW_BARK_TOWN, 4, 10, 9)
map_const(ELMS_LAB, 5, 5, 6)
map_const(PLAYERS_HOUSE_1F, 6, 5, 4)
map_const(PLAYERS_HOUSE_2F, 7, 4, 3)
map_const(PLAYERS_NEIGHBORS_HOUSE, 8, 4, 4)
map_const(ELMS_HOUSE, 9, 4, 4)
map_const(ROUTE_26_HEAL_HOUSE, 10, 4, 4)
map_const(DAY_OF_WEEK_SIBLINGS_HOUSE, 11, 4, 4)
map_const(ROUTE_27_SANDSTORM_HOUSE, 12, 4, 4)
map_const(ROUTE_29_ROUTE_46_GATE, 13, 5, 4)

#define CURRENT_MAP_GROUP  MAPGROUP_SAFFRON
map_const(ROUTE_5, 1, 10, 9)
map_const(SAFFRON_CITY, 2, 20, 18)
map_const(FIGHTING_DOJO, 3, 5, 6)
map_const(SAFFRON_GYM, 4, 10, 9)
map_const(SAFFRON_MART, 5, 6, 4)
map_const(SAFFRON_POKECENTER_1F, 6, 5, 4)
map_const(SAFFRON_POKECENTER_2F_BETA, 7, 8, 4)
map_const(MR_PSYCHICS_HOUSE, 8, 4, 4)
map_const(SAFFRON_MAGNET_TRAIN_STATION, 9, 10, 9)
map_const(SILPH_CO_1F, 10, 8, 4)
map_const(COPYCATS_HOUSE_1F, 11, 4, 4)
map_const(COPYCATS_HOUSE_2F, 12, 5, 3)
map_const(ROUTE_5_UNDERGROUND_PATH_ENTRANCE, 13, 4, 4)
map_const(ROUTE_5_SAFFRON_GATE, 14, 5, 4)
map_const(ROUTE_5_CLEANSE_TAG_HOUSE, 15, 4, 4)

#define CURRENT_MAP_GROUP  MAPGROUP_CHERRYGROVE
map_const(ROUTE_30, 1, 10, 27)
map_const(ROUTE_31, 2, 20, 9)
map_const(CHERRYGROVE_CITY, 3, 20, 9)
map_const(CHERRYGROVE_MART, 4, 6, 4)
map_const(CHERRYGROVE_POKECENTER_1F, 5, 5, 4)
map_const(CHERRYGROVE_GYM_SPEECH_HOUSE, 6, 4, 4)
map_const(GUIDE_GENTS_HOUSE, 7, 4, 4)
map_const(CHERRYGROVE_EVOLUTION_SPEECH_HOUSE, 8, 4, 4)
map_const(ROUTE_30_BERRY_HOUSE, 9, 4, 4)
map_const(MR_POKEMONS_HOUSE, 10, 4, 4)
map_const(ROUTE_31_VIOLET_GATE, 11, 5, 4)
