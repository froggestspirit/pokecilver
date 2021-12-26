// PhoneContacts indexes (see data/phone/phone_contacts.asm)
enum {
};
	PHONE_00,
	PHONE_MOM,
	PHONE_OAK,
	PHONE_BILL,
	PHONE_ELM,
	PHONE_SCHOOLBOY_JACK,
	PHONE_POKEFAN_BEVERLY,
	PHONE_SAILOR_HUEY,
	_skip,
	_skip,
	_skip,
	PHONE_COOLTRAINERM_GAVEN,
	PHONE_COOLTRAINERF_BETH,
	PHONE_BIRDKEEPER_JOSE,
	PHONE_COOLTRAINERF_REENA,
	PHONE_YOUNGSTER_JOEY,
	PHONE_BUG_CATCHER_WADE,
	PHONE_FISHER_RALPH,
	PHONE_PICNICKER_LIZ,
	PHONE_HIKER_ANTHONY,
	PHONE_CAMPER_TODD,
	PHONE_PICNICKER_GINA,
	PHONE_JUGGLER_IRWIN,
	PHONE_BUG_CATCHER_ARNIE,
	PHONE_SCHOOLBOY_ALAN,
	_skip,
	PHONE_LASS_DANA,
	PHONE_SCHOOLBOY_CHAD,
	PHONE_POKEFANM_DEREK,
	PHONE_FISHER_CHRIS,
	PHONE_POKEMANIAC_BRENT,
	PHONE_PICNICKER_TIFFANY,
	PHONE_BIRDKEEPER_VANCE,
	PHONE_FISHER_WILTON,
	PHONE_BLACKBELT_KENJI,
	PHONE_HIKER_PARRY,
	PHONE_PICNICKER_ERIN,
#define NUM_PHONE_CONTACTS const_value - 1

// SpecialPhoneCallList indexes (see data/phone/special_calls.asm)
enum {
};
	SPECIALCALL_NONE,
	SPECIALCALL_POKERUS,
	SPECIALCALL_ROBBED,
	SPECIALCALL_ASSISTANT,
	SPECIALCALL_WEIRDBROADCAST,
	SPECIALCALL_SSTICKET,
	SPECIALCALL_BIKESHOP,
	SPECIALCALL_WORRIED,
	SPECIALCALL_MASTERBALL,
#define NUM_SPECIALCALLS const_value - 1
#define SPECIALCALL_SIZE 6

// phone struct members
rsreset
PHONE_CONTACT_TRAINER_CLASS  rb
PHONE_CONTACT_TRAINER_NUMBER rb
PHONE_CONTACT_MAP_GROUP      rb
PHONE_CONTACT_MAP_NUMBER     rb
PHONE_CONTACT_SCRIPT1_TIME   rb
PHONE_CONTACT_SCRIPT1_BANK   rb
PHONE_CONTACT_SCRIPT1_ADDR   rw
PHONE_CONTACT_SCRIPT2_TIME   rb
PHONE_CONTACT_SCRIPT2_BANK   rb
PHONE_CONTACT_SCRIPT2_ADDR   rw
#define PHONE_CONTACT_SIZE _RS

// maximum number of pokegear contacts
#define CONTACT_LIST_SIZE 10