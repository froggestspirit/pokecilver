StageDataForMysteryGift:
	ld de, wMysteryGiftStaging
	ld a, GS_VERSION + 1
	ld [de], a
	inc de ; wMysteryGiftStaging+1
	ld a, BANK(sGameData)
	call OpenSRAM
	ld hl, sPlayerData + wPlayerID - wPlayerData
	ld a, [hli]
	ld [de], a
	ld b, a
	inc de ; wMysteryGiftStaging+2
	ld a, [hl]
	ld [de], a
	ld c, a
	inc de ; wMysteryGiftStaging+3
	push bc
	ld hl, sPlayerData + wPlayerName - wPlayerData
	ld bc, NAME_LENGTH
	call CopyBytes
	push de ; wMysteryGiftStaging+14
	ld hl, sPokemonData + wPokedexCaught - wPokemonData
	ld b, wEndPokedexCaught - wPokedexCaught
	call CountSetBits
	pop de
	pop bc
	ld a, [wNumSetBits]
	ld [de], a
	inc de ; wMysteryGiftStaging+15
	call CloseSRAM
	call Random
	and 1
	ld [de], a
	inc de ; wMysteryGiftStaging+16
	call .RandomSample
	ld [de], a
	inc de ; wMysteryGiftStaging+17
	ld a, c
	ld c, b
	ld b, a
	call .RandomSample
	ld [de], a
	inc de ; wMysteryGiftStaging+18
	ld a, BANK(sBackupMysteryGiftItem)
	call OpenSRAM
	ld a, [sBackupMysteryGiftItem]
	ld [de], a
	inc de
	ld a, [sNumDailyMysteryGiftPartnerIDs]
	ld [de], a
	ld a, wMysteryGiftPlayerDataEnd - wMysteryGiftPlayerData
	ld [wUnusedMysteryGiftStagedDataLength], a
	call CloseSRAM
	ld hl, wMysteryGiftStaging
	ld de, wMysteryGiftPlayerData
	ld bc, wMysteryGiftPlayerDataEnd - wMysteryGiftPlayerData
	jp CopyBytes

.RandomSample:
	push de
	call Random
	cp 10 percent
	jr c, .tenpercent
	call Random
	and %111
	ld d, a
	rl d
	ld e, $80
.loop
	rlc e
	dec a
	jr nz, .loop
	ld a, e
	and c
	jr z, .skip
	ld a, $1
.skip
	add d
	jr .done

.tenpercent
	call Random
	cp 20 percent - 1
	jr c, .twopercent
	call Random
	and %011
	ld d, a
	rl d
	ld e, $80
.loop2
	rlc e
	dec a
	jr nz, .loop2
	ld a, e
	and b
	jr z, .skip2
	ld a, $1
.skip2
	add d
	add $10
	jr .done

.twopercent
	call Random
	cp 20 percent - 1
	jr c, .pointfourpercent
	ld a, b
	swap a
	and $7
	add $18
	jr .done

.pointfourpercent
	ld a, b
	and $80
	ld a, $20
	jr z, .done
	ld a, $21

.done
	pop de
	ret

MysteryGiftGetItem:
	ld a, c
	cp MysteryGiftItems.End - MysteryGiftItems
	jr nc, MysteryGiftFallbackItem
	ld hl, MysteryGiftItems
	ld b, 0
	add hl, bc
	ld c, [hl]
	ret

MysteryGiftGetDecoration:
	ld a, c
	cp MysteryGiftDecos.End - MysteryGiftDecos
	jr nc, MysteryGiftFallbackItem
	ld hl, MysteryGiftDecos
	ld b, 0
	add hl, bc
	ld c, [hl]
	ret

MysteryGiftFallbackItem:
	ld c, DECO_POLKADOT_BED ; GREAT_BALL
	ret

INCLUDE "data/items/mystery_gift_items.asm"

INCLUDE "data/decorations/mystery_gift_decos.asm"
