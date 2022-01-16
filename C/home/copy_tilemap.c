#include "../constants.h"
#include "copy_tilemap.h"

void LoadTilemapToTempTilemap(void){
    SET_PC(0x3156U);
//  Load wTilemap into wTempTilemap
    hlcoord(0, 0, wTilemap);  // hlcoord 0, 0
    decoord(0, 0, wTempTilemap);  // decoord 0, 0, wTempTilemap
    LD_BC(wTilemapEnd - wTilemap);  // ld bc, wTilemapEnd - wTilemap
    return CopyBytes();  // jp CopyBytes

}

void SafeLoadTempTilemapToTilemap(void){
    SET_PC(0x3162U);
    XOR_A_A;  // xor a
    LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a
    CALL(mLoadTempTilemapToTilemap);  // call LoadTempTilemapToTilemap
    LD_A(1);  // ld a, 1
    LDH_addr_A(hBGMapMode);  // ldh [hBGMapMode], a
    RET;  // ret

}

void LoadTempTilemapToTilemap(void){
    SET_PC(0x316DU);
//  Load wTempTilemap into wTilemap
    hlcoord(0, 0, wTempTilemap);  // hlcoord 0, 0, wTempTilemap
    decoord(0, 0, wTilemap);  // decoord 0, 0
    LD_BC(wTilemapEnd - wTilemap);  // ld bc, wTilemapEnd - wTilemap
    return CopyBytes();  // jp CopyBytes

}
