	object_const_def
	const SILVERCAVEROOM1_POKE_BALL1
	const SILVERCAVEROOM1_POKE_BALL2
	const SILVERCAVEROOM1_POKE_BALL3

SilverCaveRoom1_MapScripts:
	def_scene_scripts

	def_callbacks

SilverCaveRoom1MaxElixer:
	itemball MAX_ELIXER

SilverCaveRoom1XAccuracy:
	itemball X_ACCURACY

SilverCaveRoom1EscapeRope:
	itemball ESCAPE_ROPE

SilverCaveRoom1HiddenDireHit:
	hiddenitem DIRE_HIT, EVENT_SILVER_CAVE_ROOM_1_HIDDEN_DIRE_HIT

SilverCaveRoom1HiddenUltraBall:
	hiddenitem ULTRA_BALL, EVENT_SILVER_CAVE_ROOM_1_HIDDEN_ULTRA_BALL

SilverCaveRoom1_MapEvents:
	db 0, 0 ; filler

	def_warp_events
	warp_event  9, 33, SILVER_CAVE_OUTSIDE, 2
	warp_event 15,  1, SILVER_CAVE_ROOM_2, 1

	def_coord_events

	def_bg_events
	bg_event 16, 23, BGEVENT_ITEM, SilverCaveRoom1HiddenDireHit
	bg_event 17, 12, BGEVENT_ITEM, SilverCaveRoom1HiddenUltraBall

	def_object_events
	object_event  4,  9, SPRITE_POKE_BALL, SPRITEMOVEDATA_STILL, 0, 0, -1, -1, 0, OBJECTTYPE_ITEMBALL, 0, SilverCaveRoom1MaxElixer, EVENT_SILVER_CAVE_ROOM_1_MAX_ELIXER
	object_event 15, 29, SPRITE_POKE_BALL, SPRITEMOVEDATA_STILL, 0, 0, -1, -1, 0, OBJECTTYPE_ITEMBALL, 0, SilverCaveRoom1XAccuracy, EVENT_SILVER_CAVE_ROOM_1_X_ACCURACY
	object_event  5, 30, SPRITE_POKE_BALL, SPRITEMOVEDATA_STILL, 0, 0, -1, -1, 0, OBJECTTYPE_ITEMBALL, 0, SilverCaveRoom1EscapeRope, EVENT_SILVER_CAVE_ROOM_1_ESCAPE_ROPE
