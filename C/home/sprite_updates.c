#include "../constants.h"
#include "sprite_updates.h"

void DisableSpriteUpdates(void){
    XOR_A_A;  // xor a
    LDH_addr_A(hMapAnims);  // ldh [hMapAnims], a
    LD_A_addr(wVramState);  // ld a, [wVramState]
    RES_A(0);  // res 0, a
    LD_addr_A(wVramState);  // ld [wVramState], a
    LD_A(0x0);  // ld a, $0
    LD_addr_A(wSpriteUpdatesEnabled);  // ld [wSpriteUpdatesEnabled], a
    RET;  // ret

}

void EnableSpriteUpdates(void){
    LD_A(0x1);  // ld a, $1
    LD_addr_A(wSpriteUpdatesEnabled);  // ld [wSpriteUpdatesEnabled], a
    LD_A_addr(wVramState);  // ld a, [wVramState]
    SET_A(0);  // set 0, a
    LD_addr_A(wVramState);  // ld [wVramState], a
    LD_A(0x1);  // ld a, $1
    LDH_addr_A(hMapAnims);  // ldh [hMapAnims], a
    RET;  // ret

}
