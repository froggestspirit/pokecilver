__trainer_class__ = 0

trainerclass: MACRO
\#define 1 __trainer_class__
__trainer_class__ = __trainer_class__ + 1
enum {
};
ENDM

// trainer class ids
// `trainerclass` indexes are for:
// - TrainerClassNames (see data/trainers/class_names.asm)
// - TrainerClassAttributes (see data/trainers/attributes.asm)
// - TrainerClassDVs (see data/trainers/dvs.asm)
// - TrainerGroups (see data/trainers/party_pointers.asm)
// - TrainerEncounterMusic (see data/trainers/encounter_music.asm)
// - TrainerPicPointers (see data/trainers/pic_pointers.asm)
// - TrainerPalettes (see data/trainers/palettes.asm)
// trainer constants are Trainers indexes, for the sub-tables of TrainerGroups (see data/trainers/parties.asm)
#define CHRIS __trainer_class__
	trainerclass TRAINER_NONE // 0
	PHONECONTACT_MOM,
	PHONECONTACT_BIKESHOP,
	PHONECONTACT_BILL,
	PHONECONTACT_ELM,
#define NUM_NONTRAINER_PHONECONTACTS const_value - 1

	trainerclass FALKNER // 1
	FALKNER1,

	trainerclass WHITNEY // 2
	WHITNEY1,

	trainerclass BUGSY // 3
	BUGSY1,

	trainerclass MORTY // 4
	MORTY1,

	trainerclass PRYCE // 5
	PRYCE1,

	trainerclass JASMINE // 6
	JASMINE1,

	trainerclass CHUCK // 7
	CHUCK1,

	trainerclass CLAIR // 8
	CLAIR1,

	trainerclass RIVAL1 // 9
	RIVAL1_1_CHIKORITA,
	RIVAL1_1_CYNDAQUIL,
	RIVAL1_1_TOTODILE,
	RIVAL1_2_CHIKORITA,
	RIVAL1_2_CYNDAQUIL,
	RIVAL1_2_TOTODILE,
	RIVAL1_3_CHIKORITA,
	RIVAL1_3_CYNDAQUIL,
	RIVAL1_3_TOTODILE,
	RIVAL1_4_CHIKORITA,
	RIVAL1_4_CYNDAQUIL,
	RIVAL1_4_TOTODILE,
	RIVAL1_5_CHIKORITA,
	RIVAL1_5_CYNDAQUIL,
	RIVAL1_5_TOTODILE,

	trainerclass POKEMON_PROF // a

	trainerclass WILL // b
	WILL1,

	trainerclass CAL // c
	CAL1, // unused
	CAL2,
	CAL3,

	trainerclass BRUNO // d
	BRUNO1,

	trainerclass KAREN // e
	KAREN1,

	trainerclass KOGA // f
	KOGA1,

	trainerclass CHAMPION // 10
	LANCE,

	trainerclass BROCK // 11
	BROCK1,

	trainerclass MISTY // 12
	MISTY1,

	trainerclass LT_SURGE // 13
	LT_SURGE1,

	trainerclass SCIENTIST // 14
	ROSS,
	MITCH,
	JED,
	MARC,
	RICH,

	trainerclass ERIKA // 15
	ERIKA1,

	trainerclass YOUNGSTER // 16
	JOEY1,
	MIKEY,
	ALBERT,
	GORDON,
	SAMUEL,
	IAN,
	JOEY2,
	JOEY3,
	WARREN,
	JIMMY,
	OWEN,
	JASON,

	trainerclass SCHOOLBOY // 17
	JACK1,
	KIPP,
	ALAN1,
	JOHNNY,
	DANNY,
	TOMMY,
	DUDLEY,
	JOE,
	BILLY,
	CHAD1,
	NATE,
	RICKY,
	JACK2,
	JACK3,
	ALAN2,
	ALAN3,
	CHAD2,
	CHAD3,
	JACK4, // unused
	JACK5, // unused
	ALAN4, // unused
	ALAN5, // unused
	CHAD4, // unused
	CHAD5, // unused

	trainerclass BIRD_KEEPER // 18
	ROD,
	ABE,
	BRYAN,
	THEO,
	TOBY,
	DENIS,
	VANCE1,
	HANK,
	ROY,
	BORIS,
	BOB,
	JOSE1,
	PETER,
	JOSE2,
	PERRY,
	BRET,
	JOSE3,
	VANCE2,
	VANCE3,

	trainerclass LASS // 19
	CARRIE,
	BRIDGET,
	ALICE,
	KRISE,
	CONNIE1,
	LINDA,
	LAURA,
	SHANNON,
	MICHELLE,
	DANA1,
	ELLEN,
	CONNIE2, // unused
	CONNIE3, // unused
	DANA2,
	DANA3,

	trainerclass JANINE // 1a
	JANINE1,

	trainerclass COOLTRAINERM // 1b
	NICK,
	AARON,
	PAUL,
	CODY,
	MIKE,
	GAVEN1,
	GAVEN2,
	RYAN,
	JAKE,
	GAVEN3,
	BLAKE,
	BRIAN,
	ERICK, // unused
	ANDY, // unused
	TYLER, // unused
	SEAN,
	KEVIN,
	STEVE, // unused
	ALLEN,

	trainerclass COOLTRAINERF // 1c
	GWEN,
	LOIS,
	FRAN,
	LOLA,
	KATE,
	IRENE,
	KELLY,
	JOYCE,
	BETH1,
	REENA1,
	MEGAN,
	BETH2,
	CAROL,
	QUINN,
	EMMA,
	CYBIL,
	JENN,
	BETH3,
	REENA2,
	REENA3,

	trainerclass BEAUTY // 1d
	VICTORIA,
	SAMANTHA,
	JULIE, // unused
	JACLYN, // unused
	BRENDA, // unused
	CASSIE,
	CAROLINE, // unused
	CARLENE, // unused
	JESSICA, // unused
	RACHAEL, // unused
	ANGELICA, // unused
	KENDRA, // unused
	VERONICA, // unused
	JULIA,
	THERESA, // unused
	VALERIE,

	trainerclass POKEMANIAC // 1e
	LARRY,
	ANDREW,
	CALVIN,
	SHANE,
	BEN,
	BRENT1,
	RON,
	ETHAN,
	BRENT2,
	BRENT3,
	ISSAC,
	DONALD,
	ZACH,

	trainerclass GRUNTM // 1f
	GRUNTM_1,
	GRUNTM_2,
	GRUNTM_3,
	GRUNTM_4,
	GRUNTM_5,
	GRUNTM_6,
	GRUNTM_7,
	GRUNTM_8,
	GRUNTM_9,
	GRUNTM_10,
	GRUNTM_11,
	GRUNTM_12, // unused
	GRUNTM_13,
	GRUNTM_14,
	GRUNTM_15,
	GRUNTM_16,
	GRUNTM_17,
	GRUNTM_18,
	GRUNTM_19,
	GRUNTM_20,
	GRUNTM_21,
	GRUNTM_22, // unused
	GRUNTM_23, // unused
	GRUNTM_24,
	GRUNTM_25,
	GRUNTM_26, // unused
	GRUNTM_27, // unused
	GRUNTM_28,
	GRUNTM_29,
	GRUNTM_30, // unused
	GRUNTM_31,

	trainerclass GENTLEMAN // 20
	PRESTON,
	EDWARD,
	GREGORY,
	VIRGIL, // unused
	ALFRED,

	trainerclass SKIER // 21
	ROXANNE,
	CLARISSA,

	trainerclass TEACHER // 22
	COLETTE,
	HILLARY,
	SHIRLEY,

	trainerclass SABRINA // 23
	SABRINA1,

	trainerclass BUG_CATCHER // 24
	DON,
	ROB,
	ED,
	WADE1,
	BUG_CATCHER_BENNY,
	AL,
	JOSH,
	ARNIE1,
	KEN,
	WADE2,
	WADE3,
	DOUG,
	ARNIE2,
	ARNIE3,

	trainerclass FISHER // 25
	JUSTIN,
	RALPH1,
	ARNOLD,
	KYLE,
	HENRY,
	MARVIN,
	CHRIS1,
	ANDRE,
	RAYMOND,
	WILTON1,
	EDGAR,
	JONAH,
	MARTIN,
	STEPHEN,
	BARNEY,
	RALPH2,
	RALPH3,
	CHRIS2,
	CHRIS3,
	WILTON2,
	SCOTT,
	WILTON3,

	trainerclass SWIMMERM // 26
	HAROLD,
	SIMON,
	RANDALL,
	CHARLIE,
	GEORGE,
	BERKE,
	KIRK,
	MATHEW,
	HAL, // unused
	PATON, // unused
	DARYL, // unused
	WALTER, // unused
	TONY, // unused
	JEROME,
	TUCKER,
	RICK, // unused
	CAMERON,
	SETH,
	JAMES, // unused
	LEWIS, // unused
	PARKER,

	trainerclass SWIMMERF // 27
	ELAINE,
	PAULA,
	KAYLEE,
	SUSIE,
	DENISE,
	KARA,
	WENDY,
	LISA, // unused
	JILL, // unused
	MARY,
	KATIE, // unused
	DAWN,
	TARA, // unused
	NICOLE,
	LORI,
	JODY, // unused
	NIKKI,
	DIANA,
	BRIANA,

	trainerclass SAILOR // 28
	EUGENE,
	HUEY1,
	TERRELL,
	KENT,
	ERNEST,
	JEFF,
	GARRETT,
	KENNETH,
	STANLY,
	HARRY,
	HUEY2,
	HUEY3,

	trainerclass SUPER_NERD // 29
	STAN,
	ERIC,
	GREGG, // unused
	JAY, // unused
	DAVE, // unused
	SAM,
	TOM,
	PAT,
	SHAWN,
	TERU,
	RUSS, // unused
	NORTON, // unused

	trainerclass RIVAL2 // 2a
	RIVAL2_1_CHIKORITA,
	RIVAL2_1_CYNDAQUIL,
	RIVAL2_1_TOTODILE,
	RIVAL2_2_CHIKORITA,
	RIVAL2_2_CYNDAQUIL,
	RIVAL2_2_TOTODILE,

	trainerclass GUITARIST // 2b
	CLYDE,
	VINCENT,

	trainerclass HIKER // 2c
	ANTHONY1,
	RUSSELL,
	PHILLIP,
	LEONARD,
	ANTHONY2,
	BENJAMIN,
	ERIK,
	MICHAEL,
	PARRY1,
	TIMOTHY,
	BAILEY,
	ANTHONY3,
	TIM,
	NOLAND,
	SIDNEY,
	KENNY,
	JIM,
	DANIEL,
	PARRY2,
	PARRY3,

	trainerclass BIKER // 2d
	KAZU1, // unused
	KAZU2, // unused
	DWAYNE,
	HARRIS,
	ZEKE,
	CHARLES,
	RILEY,
	JOEL,
	GLENN,

	trainerclass BLAINE // 2e
	BLAINE1,

	trainerclass BURGLAR // 2f
	DUNCAN,
	EDDIE,
	COREY,

	trainerclass FIREBREATHER // 30
	OTIS,
	DICK,
	NED,
	BURT,
	BILL,
	WALT,
	RAY,
	LYLE,

	trainerclass JUGGLER // 31
	IRWIN1,
	FRITZ,
	HORTON,
	IRWIN2,
	IRWIN3,

	trainerclass BLACKBELT_T // 32
	KENJI1,
	YOSHI,
	KENJI2,
	LAO,
	NOB,
	KIYO,
	LUNG,
	KENJI3,
	WAI,

	trainerclass EXECUTIVEM // 33
	EXECUTIVEM_1,
	EXECUTIVEM_2,
	EXECUTIVEM_3,
	EXECUTIVEM_4,

	trainerclass PSYCHIC_T // 34
	NATHAN,
	FRANKLIN,
	HERMAN,
	FIDEL,
	GREG,
	NORMAN,
	MARK,
	PHIL,
	RICHARD,
	GILBERT,
	JARED,
	RODNEY,

	trainerclass PICNICKER // 35
	LIZ1,
	GINA1,
	BROOKE,
	KIM,
	CINDY,
	HOPE,
	SHARON,
	DEBRA,
	GINA2,
	ERIN1,
	LIZ2,
	LIZ3,
	HEIDI,
	EDNA,
	GINA3,
	TIFFANY1,
	TIFFANY2,
	ERIN2,
	TANYA,
	TIFFANY3,
	ERIN3,

	trainerclass CAMPER // 36
	ROLAND,
	TODD1,
	IVAN,
	ELLIOT,
	BARRY,
	LLOYD,
	DEAN,
	SID,
	HARVEY, // unused
	DALE, // unused
	TED,
	TODD2,
	TODD3,
	THOMAS, // unused
	LEROY, // unused
	DAVID, // unused
	JOHN, // unused
	JERRY,
	SPENCER,

	trainerclass EXECUTIVEF // 37
	EXECUTIVEF_1,
	EXECUTIVEF_2,

	trainerclass SAGE // 38
	CHOW,
	NICO,
	JIN,
	TROY,
	JEFFREY,
	PING,
	EDMOND,
	NEAL,
	LI,

	trainerclass MEDIUM // 39
	MARTHA,
	GRACE,
	BETHANY, // unused
	MARGRET, // unused
	ETHEL, // unused
	REBECCA,
	DORIS,

	trainerclass BOARDER // 3a
	RONALD,
	BRAD,
	DOUGLAS,

	trainerclass POKEFANM // 3b
	WILLIAM,
	DEREK1,
	ROBERT,
	JOSHUA,
	CARTER,
	TREVOR,
	BRANDON,
	JEREMY,
	COLIN,
	DEREK2,
	DEREK3,
	ALEX,

	trainerclass KIMONO_GIRL // 3c
	KYOKO, // unused
	NAOKO,
	SAYO,
	ZUKI,
	KUNI,
	MIKI,

	trainerclass TWINS // 3d
	AMYANDMAY1,
	ANNANDANNE1,
	ANNANDANNE2,
	AMYANDMAY2,
	JOANDZOE1,
	JOANDZOE2,
	MEGANDPEG1,
	MEGANDPEG2,

	trainerclass POKEFANF // 3e
	BEVERLY1,
	RUTH,
	BEVERLY2,
	BEVERLY3,
	GEORGIA,

	trainerclass RED // 3f
	RED1,

	trainerclass BLUE // 40
	BLUE1,

	trainerclass OFFICER // 41
	KEITH,
	DIRK,

	trainerclass GRUNTF // 42
	GRUNTF_1,
	GRUNTF_2,
	GRUNTF_3,
	GRUNTF_4,
	GRUNTF_5,

#define NUM_TRAINER_CLASSES __trainer_class__ - 1
