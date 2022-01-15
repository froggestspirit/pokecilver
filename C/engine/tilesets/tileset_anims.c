#include "../../constants.h"

void DummyPredef3A_DummyData(void){
    SET_PC(0xFC000U);
//  //  unreferenced
    //db ['0'];  // db 0

    DummyPredef3A();
    return;
}

void DummyPredef3A(void){
    SET_PC(0xFC001U);
    RET;  // ret

}

void DummyPredef3A_DummyFunction(void){
    SET_PC(0xFC002U);
//  //  unreferenced
    RET;  // ret


}

void v_AnimateTileset(void){
    SET_PC(0xFC003U);
//  Increment [hTileAnimFrame] and run that frame's function
//  from the array pointed to by wTilesetAnim.

//  Called in WRAM bank 1.

    LD_A_addr(wTilesetAnim);  // ld a, [wTilesetAnim]
    LD_E_A;  // ld e, a
    LD_A_addr(wTilesetAnim + 1);  // ld a, [wTilesetAnim + 1]
    LD_D_A;  // ld d, a

    LDH_A_addr(hTileAnimFrame);  // ldh a, [hTileAnimFrame]
    LD_L_A;  // ld l, a
    INC_A;  // inc a
    LDH_addr_A(hTileAnimFrame);  // ldh [hTileAnimFrame], a

    LD_H(0);  // ld h, 0
    ADD_HL_HL;  // add hl, hl
    ADD_HL_HL;  // add hl, hl
    ADD_HL_DE;  // add hl, de

//  2-byte parameter
//  All functions take input de
    LD_E_hl;  // ld e, [hl]
    INC_HL;  // inc hl
    LD_D_hl;  // ld d, [hl]
    INC_HL;  // inc hl

//  Function address
    LD_A_hli;  // ld a, [hli]
    LD_H_hl;  // ld h, [hl]
    LD_L_A;  // ld l, a

    JP_hl;  // jp hl

}

void Tileset0Anim(void){
    SET_PC(0xFC01EU);
    TilesetJohtoModernAnim();
    return;
}

void TilesetJohtoModernAnim(void){
    SET_PC(0xFC01EU);
    TilesetKantoAnim();
    return;
}

void TilesetKantoAnim(void){
    SET_PC(0xFC01EU);
    TilesetParkAnim();
    return;
}

void TilesetParkAnim(void){
    SET_PC(0xFC01EU);
    TilesetForestAnim();
    return;
}

void TilesetForestAnim(void){
    SET_PC(0xFC01EU);
    //dw ['vTiles2 tile 0x14', 'AnimateWaterTile'];  // dw vTiles2 tile $14, AnimateWaterTile
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'AnimateWaterPalette'];  // dw NULL,  AnimateWaterPalette
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'AnimateFlowerTile'];  // dw NULL,  AnimateFlowerTile
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'StandingTileFrame8'];  // dw NULL,  StandingTileFrame8
    //dw ['NULL', 'DoneTileAnimation'];  // dw NULL,  DoneTileAnimation

    TilesetJohtoAnim();
    return;
}

void TilesetJohtoAnim(void){
    SET_PC(0xFC04AU);
    //dw ['vTiles2 tile 0x14', 'AnimateWaterTile'];  // dw vTiles2 tile $14, AnimateWaterTile
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'AnimateWaterPalette'];  // dw NULL,  AnimateWaterPalette
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'AnimateFlowerTile'];  // dw NULL,  AnimateFlowerTile
    //dw ['WhirlpoolFrames1', 'AnimateWhirlpoolTile'];  // dw WhirlpoolFrames1, AnimateWhirlpoolTile
    //dw ['WhirlpoolFrames2', 'AnimateWhirlpoolTile'];  // dw WhirlpoolFrames2, AnimateWhirlpoolTile
    //dw ['WhirlpoolFrames3', 'AnimateWhirlpoolTile'];  // dw WhirlpoolFrames3, AnimateWhirlpoolTile
    //dw ['WhirlpoolFrames4', 'AnimateWhirlpoolTile'];  // dw WhirlpoolFrames4, AnimateWhirlpoolTile
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'StandingTileFrame8'];  // dw NULL,  StandingTileFrame8
    //dw ['NULL', 'DoneTileAnimation'];  // dw NULL,  DoneTileAnimation

    UnusedTilesetAnim1();
    return;
}

void UnusedTilesetAnim1(void){
    SET_PC(0xFC07EU);
//  //  unreferenced
//  Scrolls tile $03 like cave water, but also has the standard $03 flower tile.
    //dw ['vTiles2 tile 0x03', 'ReadTileToAnimBuffer'];  // dw vTiles2 tile $03, ReadTileToAnimBuffer
    //dw ['wTileAnimBuffer', 'ScrollTileRightLeft'];  // dw wTileAnimBuffer, ScrollTileRightLeft
    //dw ['vTiles2 tile 0x03', 'WriteTileFromAnimBuffer'];  // dw vTiles2 tile $03, WriteTileFromAnimBuffer
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'AnimateFlowerTile'];  // dw NULL,  AnimateFlowerTile
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'DoneTileAnimation'];  // dw NULL,  DoneTileAnimation

    UnusedTilesetAnim2();
    return;
}

void UnusedTilesetAnim2(void){
    SET_PC(0xFC0AAU);
//  //  unreferenced
//  Scrolls tile $14 like cave water.
    //dw ['vTiles2 tile 0x14', 'ReadTileToAnimBuffer'];  // dw vTiles2 tile $14, ReadTileToAnimBuffer
    //dw ['wTileAnimBuffer', 'ScrollTileRightLeft'];  // dw wTileAnimBuffer, ScrollTileRightLeft
    //dw ['vTiles2 tile 0x14', 'WriteTileFromAnimBuffer'];  // dw vTiles2 tile $14, WriteTileFromAnimBuffer
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'DoneTileAnimation'];  // dw NULL,  DoneTileAnimation

    TilesetPortAnim();
    return;
}

void TilesetPortAnim(void){
    SET_PC(0xFC0D6U);
    //dw ['vTiles2 tile 0x14', 'AnimateWaterTile'];  // dw vTiles2 tile $14, AnimateWaterTile
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'AnimateWaterPalette'];  // dw NULL,  AnimateWaterPalette
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'StandingTileFrame8'];  // dw NULL,  StandingTileFrame8
    //dw ['NULL', 'DoneTileAnimation'];  // dw NULL,  DoneTileAnimation

    TilesetEliteFourRoomAnim();
    return;
}

void TilesetEliteFourRoomAnim(void){
    SET_PC(0xFC106U);
    //dw ['NULL', 'AnimateLavaBubbleTile2'];  // dw NULL,  AnimateLavaBubbleTile2
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'AnimateLavaBubbleTile1'];  // dw NULL,  AnimateLavaBubbleTile1
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'StandingTileFrame8'];  // dw NULL,  StandingTileFrame8
    //dw ['NULL', 'DoneTileAnimation'];  // dw NULL,  DoneTileAnimation

    UnusedTilesetAnim3();
    return;
}

void UnusedTilesetAnim3(void){
    SET_PC(0xFC126U);
//  //  unreferenced
//  Scrolls tile $53 like a waterfall
    //dw ['vTiles2 tile 0x53', 'ReadTileToAnimBuffer'];  // dw vTiles2 tile $53, ReadTileToAnimBuffer
    //dw ['wTileAnimBuffer', 'ScrollTileDown'];  // dw wTileAnimBuffer, ScrollTileDown
    //dw ['wTileAnimBuffer', 'ScrollTileDown'];  // dw wTileAnimBuffer, ScrollTileDown
    //dw ['vTiles2 tile 0x53', 'WriteTileFromAnimBuffer'];  // dw vTiles2 tile $53, WriteTileFromAnimBuffer
    //dw ['vTiles2 tile 0x03', 'ReadTileToAnimBuffer'];  // dw vTiles2 tile $03, ReadTileToAnimBuffer
    //dw ['wTileAnimBuffer', 'ScrollTileRightLeft'];  // dw wTileAnimBuffer, ScrollTileRightLeft
    //dw ['vTiles2 tile 0x03', 'WriteTileFromAnimBuffer'];  // dw vTiles2 tile $03, WriteTileFromAnimBuffer
    //dw ['vTiles2 tile 0x53', 'ReadTileToAnimBuffer'];  // dw vTiles2 tile $53, ReadTileToAnimBuffer
    //dw ['wTileAnimBuffer', 'ScrollTileDown'];  // dw wTileAnimBuffer, ScrollTileDown
    //dw ['wTileAnimBuffer', 'ScrollTileDown'];  // dw wTileAnimBuffer, ScrollTileDown
    //dw ['vTiles2 tile 0x53', 'WriteTileFromAnimBuffer'];  // dw vTiles2 tile $53, WriteTileFromAnimBuffer
    //dw ['NULL', 'DoneTileAnimation'];  // dw NULL,  DoneTileAnimation

    UnusedTilesetAnim4();
    return;
}

void UnusedTilesetAnim4(void){
    SET_PC(0xFC156U);
//  //  unreferenced
//  Scrolls tile $54 like a waterfall
    //dw ['vTiles2 tile 0x54', 'ReadTileToAnimBuffer'];  // dw vTiles2 tile $54, ReadTileToAnimBuffer
    //dw ['wTileAnimBuffer', 'ScrollTileDown'];  // dw wTileAnimBuffer, ScrollTileDown
    //dw ['wTileAnimBuffer', 'ScrollTileDown'];  // dw wTileAnimBuffer, ScrollTileDown
    //dw ['vTiles2 tile 0x54', 'WriteTileFromAnimBuffer'];  // dw vTiles2 tile $54, WriteTileFromAnimBuffer
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['vTiles2 tile 0x03', 'ReadTileToAnimBuffer'];  // dw vTiles2 tile $03, ReadTileToAnimBuffer
    //dw ['wTileAnimBuffer', 'ScrollTileRightLeft'];  // dw wTileAnimBuffer, ScrollTileRightLeft
    //dw ['vTiles2 tile 0x03', 'WriteTileFromAnimBuffer'];  // dw vTiles2 tile $03, WriteTileFromAnimBuffer
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['vTiles2 tile 0x54', 'ReadTileToAnimBuffer'];  // dw vTiles2 tile $54, ReadTileToAnimBuffer
    //dw ['wTileAnimBuffer', 'ScrollTileDown'];  // dw wTileAnimBuffer, ScrollTileDown
    //dw ['wTileAnimBuffer', 'ScrollTileDown'];  // dw wTileAnimBuffer, ScrollTileDown
    //dw ['vTiles2 tile 0x54', 'WriteTileFromAnimBuffer'];  // dw vTiles2 tile $54, WriteTileFromAnimBuffer
    //dw ['NULL', 'DoneTileAnimation'];  // dw NULL,  DoneTileAnimation

    TilesetCaveAnim();
    return;
}

void TilesetCaveAnim(void){
    SET_PC(0xFC18EU);
    TilesetDarkCaveAnim();
    return;
}

void TilesetDarkCaveAnim(void){
    SET_PC(0xFC18EU);
    //dw ['vTiles2 tile 0x14', 'ReadTileToAnimBuffer'];  // dw vTiles2 tile $14, ReadTileToAnimBuffer
    //dw ['NULL', 'FlickeringCaveEntrancePalette'];  // dw NULL,  FlickeringCaveEntrancePalette
    //dw ['wTileAnimBuffer', 'ScrollTileRightLeft'];  // dw wTileAnimBuffer, ScrollTileRightLeft
    //dw ['NULL', 'FlickeringCaveEntrancePalette'];  // dw NULL,  FlickeringCaveEntrancePalette
    //dw ['vTiles2 tile 0x14', 'WriteTileFromAnimBuffer'];  // dw vTiles2 tile $14, WriteTileFromAnimBuffer
    //dw ['NULL', 'FlickeringCaveEntrancePalette'];  // dw NULL,  FlickeringCaveEntrancePalette
    //dw ['NULL', 'AnimateWaterPalette'];  // dw NULL,  AnimateWaterPalette
    //dw ['NULL', 'FlickeringCaveEntrancePalette'];  // dw NULL,  FlickeringCaveEntrancePalette
    //dw ['vTiles2 tile 0x40', 'ReadTileToAnimBuffer'];  // dw vTiles2 tile $40, ReadTileToAnimBuffer
    //dw ['NULL', 'FlickeringCaveEntrancePalette'];  // dw NULL,  FlickeringCaveEntrancePalette
    //dw ['wTileAnimBuffer', 'ScrollTileDown'];  // dw wTileAnimBuffer, ScrollTileDown
    //dw ['NULL', 'FlickeringCaveEntrancePalette'];  // dw NULL,  FlickeringCaveEntrancePalette
    //dw ['wTileAnimBuffer', 'ScrollTileDown'];  // dw wTileAnimBuffer, ScrollTileDown
    //dw ['NULL', 'FlickeringCaveEntrancePalette'];  // dw NULL,  FlickeringCaveEntrancePalette
    //dw ['wTileAnimBuffer', 'ScrollTileDown'];  // dw wTileAnimBuffer, ScrollTileDown
    //dw ['NULL', 'FlickeringCaveEntrancePalette'];  // dw NULL,  FlickeringCaveEntrancePalette
    //dw ['vTiles2 tile 0x40', 'WriteTileFromAnimBuffer'];  // dw vTiles2 tile $40, WriteTileFromAnimBuffer
    //dw ['NULL', 'FlickeringCaveEntrancePalette'];  // dw NULL,  FlickeringCaveEntrancePalette
    //dw ['NULL', 'DoneTileAnimation'];  // dw NULL,  DoneTileAnimation

    TilesetIcePathAnim();
    return;
}

void TilesetIcePathAnim(void){
    SET_PC(0xFC1DAU);
    //dw ['vTiles2 tile 0x35', 'ReadTileToAnimBuffer'];  // dw vTiles2 tile $35, ReadTileToAnimBuffer
    //dw ['NULL', 'FlickeringCaveEntrancePalette'];  // dw NULL,  FlickeringCaveEntrancePalette
    //dw ['wTileAnimBuffer', 'ScrollTileRightLeft'];  // dw wTileAnimBuffer, ScrollTileRightLeft
    //dw ['NULL', 'FlickeringCaveEntrancePalette'];  // dw NULL,  FlickeringCaveEntrancePalette
    //dw ['vTiles2 tile 0x35', 'WriteTileFromAnimBuffer'];  // dw vTiles2 tile $35, WriteTileFromAnimBuffer
    //dw ['NULL', 'FlickeringCaveEntrancePalette'];  // dw NULL,  FlickeringCaveEntrancePalette
    //dw ['NULL', 'AnimateWaterPalette'];  // dw NULL,  AnimateWaterPalette
    //dw ['NULL', 'FlickeringCaveEntrancePalette'];  // dw NULL,  FlickeringCaveEntrancePalette
    //dw ['vTiles2 tile 0x31', 'ReadTileToAnimBuffer'];  // dw vTiles2 tile $31, ReadTileToAnimBuffer
    //dw ['NULL', 'FlickeringCaveEntrancePalette'];  // dw NULL,  FlickeringCaveEntrancePalette
    //dw ['wTileAnimBuffer', 'ScrollTileDown'];  // dw wTileAnimBuffer, ScrollTileDown
    //dw ['NULL', 'FlickeringCaveEntrancePalette'];  // dw NULL,  FlickeringCaveEntrancePalette
    //dw ['wTileAnimBuffer', 'ScrollTileDown'];  // dw wTileAnimBuffer, ScrollTileDown
    //dw ['NULL', 'FlickeringCaveEntrancePalette'];  // dw NULL,  FlickeringCaveEntrancePalette
    //dw ['wTileAnimBuffer', 'ScrollTileDown'];  // dw wTileAnimBuffer, ScrollTileDown
    //dw ['NULL', 'FlickeringCaveEntrancePalette'];  // dw NULL,  FlickeringCaveEntrancePalette
    //dw ['vTiles2 tile 0x31', 'WriteTileFromAnimBuffer'];  // dw vTiles2 tile $31, WriteTileFromAnimBuffer
    //dw ['NULL', 'FlickeringCaveEntrancePalette'];  // dw NULL,  FlickeringCaveEntrancePalette
    //dw ['NULL', 'DoneTileAnimation'];  // dw NULL,  DoneTileAnimation

    TilesetTowerAnim();
    return;
}

void TilesetTowerAnim(void){
    SET_PC(0xFC226U);
    //dw ['TowerPillarTilePointer9', 'AnimateTowerPillarTile'];  // dw TowerPillarTilePointer9, AnimateTowerPillarTile
    //dw ['TowerPillarTilePointer10', 'AnimateTowerPillarTile'];  // dw TowerPillarTilePointer10, AnimateTowerPillarTile
    //dw ['TowerPillarTilePointer7', 'AnimateTowerPillarTile'];  // dw TowerPillarTilePointer7, AnimateTowerPillarTile
    //dw ['TowerPillarTilePointer8', 'AnimateTowerPillarTile'];  // dw TowerPillarTilePointer8, AnimateTowerPillarTile
    //dw ['TowerPillarTilePointer5', 'AnimateTowerPillarTile'];  // dw TowerPillarTilePointer5, AnimateTowerPillarTile
    //dw ['TowerPillarTilePointer6', 'AnimateTowerPillarTile'];  // dw TowerPillarTilePointer6, AnimateTowerPillarTile
    //dw ['TowerPillarTilePointer3', 'AnimateTowerPillarTile'];  // dw TowerPillarTilePointer3, AnimateTowerPillarTile
    //dw ['TowerPillarTilePointer4', 'AnimateTowerPillarTile'];  // dw TowerPillarTilePointer4, AnimateTowerPillarTile
    //dw ['TowerPillarTilePointer1', 'AnimateTowerPillarTile'];  // dw TowerPillarTilePointer1, AnimateTowerPillarTile
    //dw ['TowerPillarTilePointer2', 'AnimateTowerPillarTile'];  // dw TowerPillarTilePointer2, AnimateTowerPillarTile
    //dw ['NULL', 'StandingTileFrame'];  // dw NULL,  StandingTileFrame
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'DoneTileAnimation'];  // dw NULL,  DoneTileAnimation

    UnusedTilesetAnim5();
    return;
}

void UnusedTilesetAnim5(void){
    SET_PC(0xFC266U);
//  //  unreferenced
//  Scrolls tile $4f like cave water.
    //dw ['vTiles2 tile 0x4f', 'ReadTileToAnimBuffer'];  // dw vTiles2 tile $4f, ReadTileToAnimBuffer
    //dw ['wTileAnimBuffer', 'ScrollTileRightLeft'];  // dw wTileAnimBuffer, ScrollTileRightLeft
    //dw ['vTiles2 tile 0x4f', 'WriteTileFromAnimBuffer'];  // dw vTiles2 tile $4f, WriteTileFromAnimBuffer
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'DoneTileAnimation'];  // dw NULL,  DoneTileAnimation

    TilesetHouseAnim();
    return;
}

void TilesetHouseAnim(void){
    SET_PC(0xFC28EU);
    TilesetPlayersHouseAnim();
    return;
}

void TilesetPlayersHouseAnim(void){
    SET_PC(0xFC28EU);
    TilesetPokecenterAnim();
    return;
}

void TilesetPokecenterAnim(void){
    SET_PC(0xFC28EU);
    TilesetGateAnim();
    return;
}

void TilesetGateAnim(void){
    SET_PC(0xFC28EU);
    TilesetLabAnim();
    return;
}

void TilesetLabAnim(void){
    SET_PC(0xFC28EU);
    TilesetFacilityAnim();
    return;
}

void TilesetFacilityAnim(void){
    SET_PC(0xFC28EU);
    TilesetMartAnim();
    return;
}

void TilesetMartAnim(void){
    SET_PC(0xFC28EU);
    TilesetMansionAnim();
    return;
}

void TilesetMansionAnim(void){
    SET_PC(0xFC28EU);
    TilesetGameCornerAnim();
    return;
}

void TilesetGameCornerAnim(void){
    SET_PC(0xFC28EU);
    TilesetTraditionalHouseAnim();
    return;
}

void TilesetTraditionalHouseAnim(void){
    SET_PC(0xFC28EU);
    TilesetTrainStationAnim();
    return;
}

void TilesetTrainStationAnim(void){
    SET_PC(0xFC28EU);
    TilesetChampionsRoomAnim();
    return;
}

void TilesetChampionsRoomAnim(void){
    SET_PC(0xFC28EU);
    TilesetLighthouseAnim();
    return;
}

void TilesetLighthouseAnim(void){
    SET_PC(0xFC28EU);
    TilesetPlayersRoomAnim();
    return;
}

void TilesetPlayersRoomAnim(void){
    SET_PC(0xFC28EU);
    TilesetRuinsOfAlphAnim();
    return;
}

void TilesetRuinsOfAlphAnim(void){
    SET_PC(0xFC28EU);
    TilesetRadioTowerAnim();
    return;
}

void TilesetRadioTowerAnim(void){
    SET_PC(0xFC28EU);
    TilesetUndergroundAnim();
    return;
}

void TilesetUndergroundAnim(void){
    SET_PC(0xFC28EU);
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'WaitTileAnimation'];  // dw NULL,  WaitTileAnimation
    //dw ['NULL', 'DoneTileAnimation'];  // dw NULL,  DoneTileAnimation

    DoneTileAnimation();
    return;
}

void DoneTileAnimation(void){
    SET_PC(0xFC2A2U);
//  Reset the animation command loop.
    XOR_A_A;  // xor a
    LDH_addr_A(hTileAnimFrame);  // ldh [hTileAnimFrame], a

    WaitTileAnimation();
    return;
}

void WaitTileAnimation(void){
    SET_PC(0xFC2A5U);
//  Do nothing this frame.
    RET;  // ret

}

void StandingTileFrame8(void){
    SET_PC(0xFC2A6U);
//  Tick the wTileAnimationTimer, wrapping from 7 to 0.
    LD_A_addr(wTileAnimationTimer);  // ld a, [wTileAnimationTimer]
    INC_A;  // inc a
    AND_A(0b111);  // and %111
    LD_addr_A(wTileAnimationTimer);  // ld [wTileAnimationTimer], a
    RET;  // ret

}

void ScrollTileRightLeft(void){
    SET_PC(0xFC2B0U);
//  Scroll right for 4 ticks, then left for 4 ticks.
    LD_A_addr(wTileAnimationTimer);  // ld a, [wTileAnimationTimer]
    INC_A;  // inc a
    AND_A(0b111);  // and %111
    LD_addr_A(wTileAnimationTimer);  // ld [wTileAnimationTimer], a
    AND_A(0b100);  // and %100
    JR_NZ (mScrollTileLeft);  // jr nz, ScrollTileLeft
    JR(mScrollTileRight);  // jr ScrollTileRight

}

void ScrollTileUpDown(void){
    SET_PC(0xFC2BFU);
//  //  unreferenced
//  Scroll up for 4 ticks, then down for 4 ticks.
    LD_A_addr(wTileAnimationTimer);  // ld a, [wTileAnimationTimer]
    INC_A;  // inc a
    AND_A(0b111);  // and %111
    LD_addr_A(wTileAnimationTimer);  // ld [wTileAnimationTimer], a
    AND_A(0b100);  // and %100
    JR_NZ (mScrollTileDown);  // jr nz, ScrollTileDown
    JR(mScrollTileUp);  // jr ScrollTileUp

}

void ScrollTileLeft(void){
    SET_PC(0xFC2CEU);
    LD_H_D;  // ld h, d
    LD_L_E;  // ld l, e
    LD_C(LEN_2BPP_TILE / 4);  // ld c, LEN_2BPP_TILE / 4

loop:
    SET_PC(0xFC2D2U);
    for(int rept = 0; rept < 4; rept++){
    LD_A_hl;  // ld a, [hl]
    RLCA;  // rlca
    LD_hli_A;  // ld [hli], a
    }
    DEC_C;  // dec c
    IF_NZ goto loop;  // jr nz, .loop
    RET;  // ret

}

void ScrollTileRight(void){
    SET_PC(0xFC2E2U);
    LD_H_D;  // ld h, d
    LD_L_E;  // ld l, e
    LD_C(LEN_2BPP_TILE / 4);  // ld c, LEN_2BPP_TILE / 4

loop:
    SET_PC(0xFC2E6U);
    for(int rept = 0; rept < 4; rept++){
    LD_A_hl;  // ld a, [hl]
    RRCA;  // rrca
    LD_hli_A;  // ld [hli], a
    }
    DEC_C;  // dec c
    IF_NZ goto loop;  // jr nz, .loop
    RET;  // ret

}

void ScrollTileUp(void){
    SET_PC(0xFC2F6U);
    LD_H_D;  // ld h, d
    LD_L_E;  // ld l, e
    LD_D_hl;  // ld d, [hl]
    INC_HL;  // inc hl
    LD_E_hl;  // ld e, [hl]
    LD_BC(LEN_2BPP_TILE - 2);  // ld bc, LEN_2BPP_TILE - 2
    ADD_HL_BC;  // add hl, bc
    LD_A(LEN_2BPP_TILE / 4);  // ld a, LEN_2BPP_TILE / 4

loop:
    SET_PC(0xFC301U);
    LD_C_hl;  // ld c, [hl]
    LD_hl_E;  // ld [hl], e
    DEC_HL;  // dec hl
    LD_B_hl;  // ld b, [hl]
    LD_hl_D;  // ld [hl], d
    DEC_HL;  // dec hl
    LD_E_hl;  // ld e, [hl]
    LD_hl_C;  // ld [hl], c
    DEC_HL;  // dec hl
    LD_D_hl;  // ld d, [hl]
    LD_hl_B;  // ld [hl], b
    DEC_HL;  // dec hl
    DEC_A;  // dec a
    IF_NZ goto loop;  // jr nz, .loop
    RET;  // ret

}

void ScrollTileDown(void){
    SET_PC(0xFC311U);
    LD_H_D;  // ld h, d
    LD_L_E;  // ld l, e
    LD_DE(LEN_2BPP_TILE - 2);  // ld de, LEN_2BPP_TILE - 2
    PUSH_HL;  // push hl
    ADD_HL_DE;  // add hl, de
    LD_D_hl;  // ld d, [hl]
    INC_HL;  // inc hl
    LD_E_hl;  // ld e, [hl]
    POP_HL;  // pop hl
    LD_A(LEN_2BPP_TILE / 4);  // ld a, LEN_2BPP_TILE / 4

loop:
    SET_PC(0xFC31EU);
    LD_B_hl;  // ld b, [hl]
    LD_hl_D;  // ld [hl], d
    INC_HL;  // inc hl
    LD_C_hl;  // ld c, [hl]
    LD_hl_E;  // ld [hl], e
    INC_HL;  // inc hl
    LD_D_hl;  // ld d, [hl]
    LD_hl_B;  // ld [hl], b
    INC_HL;  // inc hl
    LD_E_hl;  // ld e, [hl]
    LD_hl_C;  // ld [hl], c
    INC_HL;  // inc hl
    DEC_A;  // dec a
    IF_NZ goto loop;  // jr nz, .loop
    RET;  // ret

}

void AnimateWaterTile(void){
    SET_PC(0xFC32EU);
//  Save the stack pointer in bc for WriteTile to restore
    LD_HL_SP(0);  // ld hl, sp+0
    LD_B_H;  // ld b, h
    LD_C_L;  // ld c, l

//  A cycle of 4 frames, updating every other tick
    LD_A_addr(wTileAnimationTimer);  // ld a, [wTileAnimationTimer]
    AND_A(0b110);  // and %110

//  hl = .WaterTileFrames + a * 8
//  (a was pre-multiplied by 2 from 'and %110')
    ADD_A_A;  // add a
    ADD_A_A;  // add a
    ADD_A_A;  // add a
    ADD_A(LOW(aAnimateWaterTile_WaterTileFrames));  // add LOW(.WaterTileFrames)
    LD_L_A;  // ld l, a
    LD_A(0);  // ld a, 0
    ADC_A(HIGH(aAnimateWaterTile_WaterTileFrames));  // adc HIGH(.WaterTileFrames)
    LD_H_A;  // ld h, a

//  Write the tile graphic from hl (now sp) to de (now hl)
    LD_SP_HL;  // ld sp, hl
    LD_L_E;  // ld l, e
    LD_H_D;  // ld h, d
    JP(mWriteTile);  // jp WriteTile


WaterTileFrames:
    SET_PC(0xFC348U);
    //incbin ['"gfx/tilesets/water/water.2bpp"']  // INCBIN "gfx/tilesets/water/water.2bpp"

    AnimateFlowerTile();
    return;
}

void AnimateFlowerTile(void){
    SET_PC(0xFC388U);
//  Save the stack pointer in bc for WriteTile to restore
    LD_HL_SP(0);  // ld hl, sp+0
    LD_B_H;  // ld b, h
    LD_C_L;  // ld c, l

//  A cycle of 2 frames, updating every other tick
    LD_A_addr(wTileAnimationTimer);  // ld a, [wTileAnimationTimer]
    AND_A(0b10);  // and %10

//  CGB has different tile graphics for flowers
    LD_E_A;  // ld e, a
    LDH_A_addr(hCGB);  // ldh a, [hCGB]
    AND_A(1);  // and 1
    ADD_A_E;  // add e

//  hl = .FlowerTileFrames + a * 16
    SWAP_A;  // swap a
    LD_E_A;  // ld e, a
    LD_D(0);  // ld d, 0
    LD_HL(mAnimateFlowerTile_FlowerTileFrames);  // ld hl, .FlowerTileFrames
    ADD_HL_DE;  // add hl, de

//  Write the tile graphic from hl (now sp) to tile $03 (now hl)
    LD_SP_HL;  // ld sp, hl
    LD_HL(vTiles2 + LEN_2BPP_TILE * 0x03);  // ld hl, vTiles2 tile $03
    JP(mWriteTile);  // jp WriteTile


FlowerTileFrames:
    SET_PC(0xFC3A7U);
    //incbin ['"gfx/tilesets/flower/dmg_1.2bpp"']  // INCBIN "gfx/tilesets/flower/dmg_1.2bpp"
    //incbin ['"gfx/tilesets/flower/cgb_1.2bpp"']  // INCBIN "gfx/tilesets/flower/cgb_1.2bpp"
    //incbin ['"gfx/tilesets/flower/dmg_2.2bpp"']  // INCBIN "gfx/tilesets/flower/dmg_2.2bpp"
    //incbin ['"gfx/tilesets/flower/cgb_2.2bpp"']  // INCBIN "gfx/tilesets/flower/cgb_2.2bpp"

    AnimateLavaBubbleTile1();
    return;
}

void AnimateLavaBubbleTile1(void){
    SET_PC(0xFC3E7U);
//  Save the stack pointer in bc for WriteTile to restore
    LD_HL_SP(0);  // ld hl, sp+0
    LD_B_H;  // ld b, h
    LD_C_L;  // ld c, l

//  A cycle of 4 frames, updating every other tick
    LD_A_addr(wTileAnimationTimer);  // ld a, [wTileAnimationTimer]
    AND_A(0b110);  // and %110

//  Offset by 2 frames from AnimateLavaBubbleTile2
    SRL_A;  // srl a
    INC_A;  // inc a
    INC_A;  // inc a
    AND_A(0b011);  // and %011

//  hl = LavaBubbleTileFrames + a * 16
    SWAP_A;  // swap a
    LD_E_A;  // ld e, a
    LD_D(0);  // ld d, 0
    LD_HL(mLavaBubbleTileFrames);  // ld hl, LavaBubbleTileFrames
    ADD_HL_DE;  // add hl, de

//  Write the tile graphic from hl (now sp) to tile $5b (now hl)
    LD_SP_HL;  // ld sp, hl
    LD_HL(vTiles2 + LEN_2BPP_TILE * 0x5b);  // ld hl, vTiles2 tile $5b
    JP(mWriteTile);  // jp WriteTile

}

void AnimateLavaBubbleTile2(void){
    SET_PC(0xFC406U);
//  Save the stack pointer in bc for WriteTile to restore
    LD_HL_SP(0);  // ld hl, sp+0
    LD_B_H;  // ld b, h
    LD_C_L;  // ld c, l

//  A cycle of 4 frames, updating every other tick
    LD_A_addr(wTileAnimationTimer);  // ld a, [wTileAnimationTimer]
    AND_A(0b110);  // and %110

//  hl = LavaBubbleTileFrames + a * 8
//  (a was pre-multiplied by 2 from 'and %110')
    ADD_A_A;  // add a
    ADD_A_A;  // add a
    ADD_A_A;  // add a
    LD_E_A;  // ld e, a
    LD_D(0);  // ld d, 0
    LD_HL(mLavaBubbleTileFrames);  // ld hl, LavaBubbleTileFrames
    ADD_HL_DE;  // add hl, de

//  Write the tile graphic from hl (now sp) to tile $38 (now hl)
    LD_SP_HL;  // ld sp, hl
    LD_HL(vTiles2 + LEN_2BPP_TILE * 0x38);  // ld hl, vTiles2 tile $38
    JP(mWriteTile);  // jp WriteTile

}

void LavaBubbleTileFrames(void){
    SET_PC(0xFC420U);
    //incbin ['"gfx/tilesets/lava/1.2bpp"']  // INCBIN "gfx/tilesets/lava/1.2bpp"
    //incbin ['"gfx/tilesets/lava/2.2bpp"']  // INCBIN "gfx/tilesets/lava/2.2bpp"
    //incbin ['"gfx/tilesets/lava/3.2bpp"']  // INCBIN "gfx/tilesets/lava/3.2bpp"
    //incbin ['"gfx/tilesets/lava/4.2bpp"']  // INCBIN "gfx/tilesets/lava/4.2bpp"

    AnimateTowerPillarTile();
    return;
}

void AnimateTowerPillarTile(void){
    SET_PC(0xFC460U);
//  Input de points to the destination in VRAM, then the source tile frames

//  Save the stack pointer in bc for WriteTile to restore
    LD_HL_SP(0);  // ld hl, sp+0
    LD_B_H;  // ld b, h
    LD_C_L;  // ld c, l

//  A cycle of 8 frames, updating every tick
    LD_A_addr(wTileAnimationTimer);  // ld a, [wTileAnimationTimer]
    AND_A(0b111);  // and %111

//  a = [.TowerPillarTileFrameOffsets + a]
    LD_HL(mAnimateTowerPillarTile_TowerPillarTileFrameOffsets);  // ld hl, .TowerPillarTileFrameOffsets
    ADD_A_L;  // add l
    LD_L_A;  // ld l, a
    LD_A(0);  // ld a, 0
    ADC_A_H;  // adc h
    LD_H_A;  // ld h, a
    LD_A_hl;  // ld a, [hl]

//  de = the destination in VRAM
    LD_L_E;  // ld l, e
    LD_H_D;  // ld h, d
    LD_E_hl;  // ld e, [hl]
    INC_HL;  // inc hl
    LD_D_hl;  // ld d, [hl]
    INC_HL;  // inc hl

//  hl = the source tile frames + offset a
    ADD_A_hl;  // add [hl]
    INC_HL;  // inc hl
    LD_H_hl;  // ld h, [hl]
    LD_L_A;  // ld l, a
    LD_A(0);  // ld a, 0
    ADC_A_H;  // adc h
    LD_H_A;  // ld h, a

//  Write the tile graphic from hl (now sp) to de (now hl)
    LD_SP_HL;  // ld sp, hl
    LD_L_E;  // ld l, e
    LD_H_D;  // ld h, d
    JR(mWriteTile);  // jr WriteTile


TowerPillarTileFrameOffsets:
    SET_PC(0xFC486U);
    //db ['0 tiles'];  // db 0 tiles
    //db ['1 tiles'];  // db 1 tiles
    //db ['2 tiles'];  // db 2 tiles
    //db ['3 tiles'];  // db 3 tiles
    //db ['4 tiles'];  // db 4 tiles
    //db ['3 tiles'];  // db 3 tiles
    //db ['2 tiles'];  // db 2 tiles
    //db ['1 tiles'];  // db 1 tiles

    StandingTileFrame();
    return;
}

void StandingTileFrame(void){
    SET_PC(0xFC48EU);
//  Tick the wTileAnimationTimer.
    LD_HL(wTileAnimationTimer);  // ld hl, wTileAnimationTimer
    INC_hl;  // inc [hl]
    RET;  // ret

}

void AnimateWhirlpoolTile(void){
    SET_PC(0xFC493U);
//  Input de points to the destination in VRAM, then the source tile frames

//  Save the stack pointer in bc for WriteTile to restore
    LD_HL_SP(0);  // ld hl, sp+0
    LD_B_H;  // ld b, h
    LD_C_L;  // ld c, l

//  de = the destination in VRAM
    LD_L_E;  // ld l, e
    LD_H_D;  // ld h, d
    LD_E_hl;  // ld e, [hl]
    INC_HL;  // inc hl
    LD_D_hl;  // ld d, [hl]
    INC_HL;  // inc hl

//  A cycle of 4 frames, updating every tick
    LD_A_addr(wTileAnimationTimer);  // ld a, [wTileAnimationTimer]
    AND_A(0b11);  // and %11

//  hl = the source tile frames + a * 16
    SWAP_A;  // swap a
    ADD_A_hl;  // add [hl]
    INC_HL;  // inc hl
    LD_H_hl;  // ld h, [hl]
    LD_L_A;  // ld l, a
    LD_A(0);  // ld a, 0
    ADC_A_H;  // adc h
    LD_H_A;  // ld h, a

//  Write the tile graphic from hl (now sp) to de (now hl)
    LD_SP_HL;  // ld sp, hl
    LD_L_E;  // ld l, e
    LD_H_D;  // ld h, d
    JR(mWriteTile);  // jr WriteTile

}

void WriteTileFromAnimBuffer(void){
    SET_PC(0xFC4B1U);
//  Save the stack pointer in bc for WriteTile to restore
    LD_HL_SP(0);  // ld hl, sp+0
    LD_B_H;  // ld b, h
    LD_C_L;  // ld c, l

//  Write the tile graphic from wTileAnimBuffer (now sp) to de (now hl)
    LD_HL(wTileAnimBuffer);  // ld hl, wTileAnimBuffer
    LD_SP_HL;  // ld sp, hl
    LD_H_D;  // ld h, d
    LD_L_E;  // ld l, e
    JR(mWriteTile);  // jr WriteTile

}

void ReadTileToAnimBuffer(void){
    SET_PC(0xFC4BDU);
//  Save the stack pointer in bc for WriteTile to restore
    LD_HL_SP(0);  // ld hl, sp+0
    LD_B_H;  // ld b, h
    LD_C_L;  // ld c, l

//  Write the tile graphic from de (now sp) to wTileAnimBuffer (now hl)
    LD_H_D;  // ld h, d
    LD_L_E;  // ld l, e
    LD_SP_HL;  // ld sp, hl
    LD_HL(wTileAnimBuffer);  // ld hl, wTileAnimBuffer
// ; fallthrough

    WriteTile();
    return;
}

void WriteTile(void){
    SET_PC(0xFC4C7U);
//  Write one tile from sp to hl.
//  The stack pointer has been saved in bc.

//  This function cannot be called, only jumped to,
//  because it relocates the stack pointer to quickly
//  copy data with a "pop slide".

    POP_DE;  // pop de
    LD_hl_E;  // ld [hl], e
    INC_HL;  // inc hl
    LD_hl_D;  // ld [hl], d
    for(int rept = 0; rept < (LEN_2BPP_TILE - 2) / 2; rept++){
    POP_DE;  // pop de
    INC_HL;  // inc hl
    LD_hl_E;  // ld [hl], e
    INC_HL;  // inc hl
    LD_hl_D;  // ld [hl], d
    }

//  Restore the stack pointer from bc
    LD_H_B;  // ld h, b
    LD_L_C;  // ld l, c
    LD_SP_HL;  // ld sp, hl
    RET;  // ret

}

void AnimateWaterPalette(void){
    SET_PC(0xFC4F2U);
//  Transition between color values 0-2 for color 0 in palette 3.

//  Don't update the palette on DMG
    LDH_A_addr(hCGB);  // ldh a, [hCGB]
    AND_A_A;  // and a
    RET_Z ;  // ret z

//  Don't update a non-standard palette order
    LDH_A_addr(rBGP);  // ldh a, [rBGP]
    CP_A(0b11100100);  // cp %11100100
    RET_NZ ;  // ret nz

//  Only update on even ticks
    LD_A_addr(wTileAnimationTimer);  // ld a, [wTileAnimationTimer]
    LD_L_A;  // ld l, a
    AND_A(1);  // and 1 ; odd
    RET_NZ ;  // ret nz

//  Ready for BGPD input
    LD_A((1 << rBGPI_AUTO_INCREMENT) + PALETTE_SIZE * PAL_BG_WATER + PAL_COLOR_SIZE * 0);  // ld a, (1 << rBGPI_AUTO_INCREMENT) palette PAL_BG_WATER color 0
    LDH_addr_A(rBGPI);  // ldh [rBGPI], a

//  A cycle of 4 colors (0 1 2 1), updating every other tick
    LD_A_L;  // ld a, l
    AND_A(0b110);  // and %110
    IF_Z goto color0;  // jr z, .color0
    CP_A(0b100);  // cp %100
    IF_Z goto color2;  // jr z, .color2

//  Copy one color from hl to rBGPI via rBGPD

//  color1
    LD_HL(wBGPals1 + PALETTE_SIZE * PAL_BG_WATER + PAL_COLOR_SIZE * 1);  // ld hl, wBGPals1 palette PAL_BG_WATER color 1
    LD_A_hli;  // ld a, [hli]
    LDH_addr_A(rBGPD);  // ldh [rBGPD], a
    LD_A_hli;  // ld a, [hli]
    LDH_addr_A(rBGPD);  // ldh [rBGPD], a
    RET;  // ret


color0:
    SET_PC(0xFC519U);
    LD_HL(wBGPals1 + PALETTE_SIZE * PAL_BG_WATER + PAL_COLOR_SIZE * 0);  // ld hl, wBGPals1 palette PAL_BG_WATER color 0
    LD_A_hli;  // ld a, [hli]
    LDH_addr_A(rBGPD);  // ldh [rBGPD], a
    LD_A_hli;  // ld a, [hli]
    LDH_addr_A(rBGPD);  // ldh [rBGPD], a
    RET;  // ret


color2:
    SET_PC(0xFC523U);
    LD_HL(wBGPals1 + PALETTE_SIZE * PAL_BG_WATER + PAL_COLOR_SIZE * 2);  // ld hl, wBGPals1 palette PAL_BG_WATER color 2
    LD_A_hli;  // ld a, [hli]
    LDH_addr_A(rBGPD);  // ldh [rBGPD], a
    LD_A_hli;  // ld a, [hli]
    LDH_addr_A(rBGPD);  // ldh [rBGPD], a
    RET;  // ret

}

void FlickeringCaveEntrancePalette(void){
    SET_PC(0xFC52DU);
//  Don't update the palette on DMG
    LDH_A_addr(hCGB);  // ldh a, [hCGB]
    AND_A_A;  // and a
    RET_Z ;  // ret z

//  Don't update a non-standard palette order
    LDH_A_addr(rBGP);  // ldh a, [rBGP]
    CP_A(0b11100100);  // cp %11100100
    RET_NZ ;  // ret nz

//  We only want to be here if we're in a dark cave.
    LD_A_addr(wTimeOfDayPalset);  // ld a, [wTimeOfDayPalset]
    CP_A(DARKNESS_PALSET);  // cp DARKNESS_PALSET
    RET_NZ ;  // ret nz

//  Ready for BGPD input
    LD_A((1 << rBGPI_AUTO_INCREMENT) + PALETTE_SIZE * PAL_BG_YELLOW + PAL_COLOR_SIZE * 0);  // ld a, (1 << rBGPI_AUTO_INCREMENT) palette PAL_BG_YELLOW color 0
    LDH_addr_A(rBGPI);  // ldh [rBGPI], a

//  A cycle of 2 colors (0 2), updating every other vblank
    LDH_A_addr(hVBlankCounter);  // ldh a, [hVBlankCounter]
    AND_A(0b10);  // and %10
    IF_NZ goto color1;  // jr nz, .color1

//  Copy one color from hl to rBGPI via rBGPD

//  color0
    LD_HL(wBGPals1 + PALETTE_SIZE * PAL_BG_YELLOW + PAL_COLOR_SIZE * 0);  // ld hl, wBGPals1 palette PAL_BG_YELLOW color 0
    goto okay;  // jr .okay


color1:
    SET_PC(0xFC54BU);
    LD_HL(wBGPals1 + PALETTE_SIZE * PAL_BG_YELLOW + PAL_COLOR_SIZE * 1);  // ld hl, wBGPals1 palette PAL_BG_YELLOW color 1


okay:
    SET_PC(0xFC54EU);
    LD_A_hli;  // ld a, [hli]
    LDH_addr_A(rBGPD);  // ldh [rBGPD], a
    LD_A_hli;  // ld a, [hli]
    LDH_addr_A(rBGPD);  // ldh [rBGPD], a
    RET;  // ret

}

void TowerPillarTilePointer1(void){
    SET_PC(0xFC555U);
// dw vTiles2 tile $2d, TowerPillarTile1
    TowerPillarTilePointer2();
    return;
}

void TowerPillarTilePointer2(void){
    SET_PC(0xFC559U);
// dw vTiles2 tile $2f, TowerPillarTile2
    TowerPillarTilePointer3();
    return;
}

void TowerPillarTilePointer3(void){
    SET_PC(0xFC55DU);
// dw vTiles2 tile $3d, TowerPillarTile3
    TowerPillarTilePointer4();
    return;
}

void TowerPillarTilePointer4(void){
    SET_PC(0xFC561U);
// dw vTiles2 tile $3f, TowerPillarTile4
    TowerPillarTilePointer5();
    return;
}

void TowerPillarTilePointer5(void){
    SET_PC(0xFC565U);
// dw vTiles2 tile $3c, TowerPillarTile5
    TowerPillarTilePointer6();
    return;
}

void TowerPillarTilePointer6(void){
    SET_PC(0xFC569U);
// dw vTiles2 tile $2c, TowerPillarTile6
    TowerPillarTilePointer7();
    return;
}

void TowerPillarTilePointer7(void){
    SET_PC(0xFC56DU);
// dw vTiles2 tile $4d, TowerPillarTile7
    TowerPillarTilePointer8();
    return;
}

void TowerPillarTilePointer8(void){
    SET_PC(0xFC571U);
// dw vTiles2 tile $4f, TowerPillarTile8
    TowerPillarTilePointer9();
    return;
}

void TowerPillarTilePointer9(void){
    SET_PC(0xFC575U);
// dw vTiles2 tile $5d, TowerPillarTile9
    TowerPillarTilePointer10();
    return;
}

void TowerPillarTilePointer10(void){
    SET_PC(0xFC579U);
//dw vTiles2 tile $5f, TowerPillarTile10

    TowerPillarTile1();
    return;
}

void TowerPillarTile1(void){
    SET_PC(0xFC57DU);
// INCBIN "gfx/tilesets/tower-pillar/1.2bpp"
    TowerPillarTile2();
    return;
}

void TowerPillarTile2(void){
    SET_PC(0xFC5CDU);
// INCBIN "gfx/tilesets/tower-pillar/2.2bpp"
    TowerPillarTile3();
    return;
}

void TowerPillarTile3(void){
    SET_PC(0xFC61DU);
// INCBIN "gfx/tilesets/tower-pillar/3.2bpp"
    TowerPillarTile4();
    return;
}

void TowerPillarTile4(void){
    SET_PC(0xFC66DU);
// INCBIN "gfx/tilesets/tower-pillar/4.2bpp"
    TowerPillarTile5();
    return;
}

void TowerPillarTile5(void){
    SET_PC(0xFC6BDU);
// INCBIN "gfx/tilesets/tower-pillar/5.2bpp"
    TowerPillarTile6();
    return;
}

void TowerPillarTile6(void){
    SET_PC(0xFC70DU);
// INCBIN "gfx/tilesets/tower-pillar/6.2bpp"
    TowerPillarTile7();
    return;
}

void TowerPillarTile7(void){
    SET_PC(0xFC75DU);
// INCBIN "gfx/tilesets/tower-pillar/7.2bpp"
    TowerPillarTile8();
    return;
}

void TowerPillarTile8(void){
    SET_PC(0xFC7ADU);
// INCBIN "gfx/tilesets/tower-pillar/8.2bpp"
    TowerPillarTile9();
    return;
}

void TowerPillarTile9(void){
    SET_PC(0xFC7FDU);
// INCBIN "gfx/tilesets/tower-pillar/9.2bpp"
    TowerPillarTile10();
    return;
}

void TowerPillarTile10(void){
    SET_PC(0xFC84DU);
//INCBIN "gfx/tilesets/tower-pillar/10.2bpp"

    WhirlpoolFrames1();
    return;
}

void WhirlpoolFrames1(void){
    SET_PC(0xFC89DU);
//dw vTiles2 tile $32, WhirlpoolTiles1
    WhirlpoolFrames2();
    return;
}

void WhirlpoolFrames2(void){
    SET_PC(0xFC8A1U);
//dw vTiles2 tile $33, WhirlpoolTiles2
    WhirlpoolFrames3();
    return;
}

void WhirlpoolFrames3(void){
    SET_PC(0xFC8A5U);
//dw vTiles2 tile $42, WhirlpoolTiles3
    WhirlpoolFrames4();
    return;
}

void WhirlpoolFrames4(void){
    SET_PC(0xFC8A9U);
//dw vTiles2 tile $43, WhirlpoolTiles4

    WhirlpoolTiles1();
    return;
}

void WhirlpoolTiles1(void){
    SET_PC(0xFC8ADU);
//INCBIN "gfx/tilesets/whirlpool/1.2bpp"
    WhirlpoolTiles2();
    return;
}

void WhirlpoolTiles2(void){
    SET_PC(0xFC8EDU);
//INCBIN "gfx/tilesets/whirlpool/2.2bpp"
    WhirlpoolTiles3();
    return;
}

void WhirlpoolTiles3(void){
    SET_PC(0xFC92DU);
//INCBIN "gfx/tilesets/whirlpool/3.2bpp"
    WhirlpoolTiles4();
    return;
}

void WhirlpoolTiles4(void){
    SET_PC(0xFC96DU);
//INCBIN "gfx/tilesets/whirlpool/4.2bpp"

    TilesetAnims_DummyFunction();
    return;
}

void TilesetAnims_DummyFunction(void){
    SET_PC(0xFC9ADU);
//  //  unreferenced
    RET;  // ret

}
