// sprite ids
// OverworldSprites indexes (see data/sprites/sprites.asm)
enum {
    SPRITE_NONE, // 00
    SPRITE_CHRIS, // 01
    SPRITE_CHRIS_BIKE, // 02
    SPRITE_GAMEBOY_KID, // 03
    SPRITE_SILVER, // 04
    SPRITE_OAK, // 05
    SPRITE_RED, // 06
    SPRITE_BLUE, // 07
    SPRITE_BILL, // 08
    SPRITE_ELDER, // 09
    SPRITE_JANINE, // 0a
    SPRITE_KURT, // 0b
    SPRITE_MOM, // 0c
    SPRITE_BLAINE, // 0d
    SPRITE_REDS_MOM, // 0e
    SPRITE_DAISY, // 0f
    SPRITE_ELM, // 10
    SPRITE_WILL, // 11
    SPRITE_FALKNER, // 12
    SPRITE_WHITNEY, // 13
    SPRITE_BUGSY, // 14
    SPRITE_MORTY, // 15
    SPRITE_CHUCK, // 16
    SPRITE_JASMINE, // 17
    SPRITE_PRYCE, // 18
    SPRITE_CLAIR, // 19
    SPRITE_BROCK, // 1a
    SPRITE_KAREN, // 1b
    SPRITE_BRUNO, // 1c
    SPRITE_MISTY, // 1d
    SPRITE_LANCE, // 1e
    SPRITE_SURGE, // 1f
    SPRITE_ERIKA, // 20
    SPRITE_KOGA, // 21
    SPRITE_SABRINA, // 22
    SPRITE_COOLTRAINER_M, // 23
    SPRITE_COOLTRAINER_F, // 24
    SPRITE_BUG_CATCHER, // 25
    SPRITE_TWIN, // 26
    SPRITE_YOUNGSTER, // 27
    SPRITE_LASS, // 28
    SPRITE_TEACHER, // 29
    SPRITE_BEAUTY, // 2a
    SPRITE_SUPER_NERD, // 2b
    SPRITE_ROCKER, // 2c
    SPRITE_POKEFAN_M, // 2d
    SPRITE_POKEFAN_F, // 2e
    SPRITE_GRAMPS, // 2f
    SPRITE_GRANNY, // 30
    SPRITE_SWIMMER_GUY, // 31
    SPRITE_SWIMMER_GIRL, // 32
    SPRITE_BIG_SNORLAX, // 33
    SPRITE_SURFING_PIKACHU, // 34
    SPRITE_ROCKET, // 35
    SPRITE_ROCKET_GIRL, // 36
    SPRITE_NURSE, // 37
    SPRITE_LINK_RECEPTIONIST, // 38
    SPRITE_CLERK, // 39
    SPRITE_FISHER, // 3a
    SPRITE_FISHING_GURU, // 3b
    SPRITE_SCIENTIST, // 3c
    SPRITE_KIMONO_GIRL, // 3d
    SPRITE_SAGE, // 3e
    SPRITE_UNUSED_GUY, // 3f
    SPRITE_GENTLEMAN, // 40
    SPRITE_BLACK_BELT, // 41
    SPRITE_RECEPTIONIST, // 42
    SPRITE_OFFICER, // 43
    SPRITE_CAL, // 44
    SPRITE_SLOWPOKE, // 45
    SPRITE_CAPTAIN, // 46
    SPRITE_BIG_LAPRAS, // 47
    SPRITE_GYM_GUIDE, // 48
    SPRITE_SAILOR, // 49
    SPRITE_BIKER, // 4a
    SPRITE_PHARMACIST, // 4b
    SPRITE_MONSTER, // 4c
    SPRITE_FAIRY, // 4d
    SPRITE_BIRD, // 4e
    SPRITE_DRAGON, // 4f
    SPRITE_BIG_ONIX, // 50
    SPRITE_N64, // 51
    SPRITE_SUDOWOODO, // 52
    SPRITE_SURF, // 53
    SPRITE_POKE_BALL, // 54
    SPRITE_POKEDEX, // 55
    SPRITE_PAPER, // 56
    SPRITE_VIRTUAL_BOY, // 57
    SPRITE_OLD_LINK_RECEPTIONIST, // 58
    SPRITE_ROCK, // 59
    SPRITE_BOULDER, // 5a
    SPRITE_SNES, // 5b
    SPRITE_FAMICOM, // 5c
    SPRITE_FRUIT_TREE, // 5d
    SPRITE_GOLD_TROPHY, // 5e
    SPRITE_SILVER_TROPHY, // 5f

// SpriteMons indexes (see data/sprites/sprite_mons.asm)
    SPRITE_UNOWN = 0x80, // 80
    SPRITE_GEODUDE, // 81
    SPRITE_GROWLITHE, // 82
    SPRITE_WEEDLE, // 83
    SPRITE_SHELLDER, // 84
    SPRITE_ODDISH, // 85
    SPRITE_GENGAR, // 86
    SPRITE_ZUBAT, // 87
    SPRITE_MAGIKARP, // 88
    SPRITE_SQUIRTLE, // 89
    SPRITE_TOGEPI, // 8a
    SPRITE_BUTTERFREE, // 8b
    SPRITE_DIGLETT, // 8c
    SPRITE_POLIWAG, // 8d
    SPRITE_PIKACHU, // 8e
    SPRITE_CLEFAIRY, // 8f
    SPRITE_CHARMANDER, // 90
    SPRITE_JYNX, // 91
    SPRITE_STARMIE, // 92
    SPRITE_BULBASAUR, // 93
    SPRITE_JIGGLYPUFF, // 94
    SPRITE_GRIMER, // 95
    SPRITE_EKANS, // 96
    SPRITE_PARAS, // 97
    SPRITE_TENTACOOL, // 98
    SPRITE_TAUROS, // 99
    SPRITE_MACHOP, // 9a
    SPRITE_VOLTORB, // 9b
    SPRITE_LAPRAS, // 9c
    SPRITE_RHYDON, // 9d
    SPRITE_MOLTRES, // 9e
    SPRITE_SNORLAX, // 9f
    SPRITE_GYARADOS, // a0
    SPRITE_LUGIA, // a1
    SPRITE_HO_OH, // a2

// special GetMonSprite values (see engine/overworld/overworld.asm)
    SPRITE_DAY_CARE_MON_1 = 0xE0, // e0
    SPRITE_DAY_CARE_MON_2, // e1

// wVariableSprites indexes (see wram.asm)
    SPRITE_CONSOLE = 0xF0, // f0
    SPRITE_DOLL_1, // f1
    SPRITE_DOLL_2, // f2
    SPRITE_BIG_DOLL, // f3
    SPRITE_WEIRD_TREE, // f4
    SPRITE_OLIVINE_RIVAL, // f5
    SPRITE_AZALEA_ROCKET, // f6
    SPRITE_FUCHSIA_GYM_1, // f7
    SPRITE_FUCHSIA_GYM_2, // f8
    SPRITE_FUCHSIA_GYM_3, // f9
    SPRITE_FUCHSIA_GYM_4, // fa
    SPRITE_COPYCAT, // fb
    SPRITE_JANINE_IMPERSONATOR, // fc
};
#define NUM_OVERWORLD_SPRITES SPRITE_SILVER_TROPHY
#define SPRITE_POKEMON SPRITE_UNOWN
#define NUM_POKEMON_SPRITES (SPRITE_HO_OH + 1) - SPRITE_POKEMON
#define SPRITE_VARS SPRITE_CONSOLE
