MAJOR EQU 0
MINOR EQU 5
PATCH EQU 1

wrong_rgbds: MACRO
	fail "pokegold requires rgbds v0.5.1 or newer."
ENDM

IF !DEF(__RGBDS_MAJOR__) || !DEF(__RGBDS_MINOR__) || !DEF(__RGBDS_PATCH__)
	wrong_rgbds
ELSE
IF (__RGBDS_MAJOR__ < MAJOR) || \
	(__RGBDS_MAJOR__ == MAJOR && __RGBDS_MINOR__ < MINOR) || \
	(__RGBDS_MAJOR__ == MAJOR && __RGBDS_MINOR__ == MINOR && __RGBDS_PATCH__ < PATCH) || \
	(__RGBDS_MAJOR__ == MAJOR && __RGBDS_MINOR__ == MINOR && __RGBDS_PATCH__ == PATCH && DEF(__RGBDS_RC__))
	wrong_rgbds
ENDC
ENDC
