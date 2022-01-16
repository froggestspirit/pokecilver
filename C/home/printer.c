#include "../constants.h"
#include "printer.h"

void PrinterReceive(void){
    SET_PC(0x1EB1U);
    HOMECALL(av_PrinterReceive);  // homecall _PrinterReceive
    RET;  // ret

}

void AskSerial(void){
//  send out a handshake while serial int is off
    LD_A_addr(wPrinterConnectionOpen);  // ld a, [wPrinterConnectionOpen]
    BIT_A(0);  // bit 0, a
    RET_Z ;  // ret z

//  if we're still interpreting data, don't try to receive
    LD_A_addr(wPrinterOpcode);  // ld a, [wPrinterOpcode]
    AND_A_A;  // and a
    RET_NZ ;  // ret nz

//  once every 6 frames
    LD_HL(wHandshakeFrameDelay);  // ld hl, wHandshakeFrameDelay
    INC_hl;  // inc [hl]
    LD_A_hl;  // ld a, [hl]
    CP_A(6);  // cp 6
    RET_C ;  // ret c

    XOR_A_A;  // xor a
    LD_hl_A;  // ld [hl], a

    LD_A(0x0c);  // ld a, $0c
    LD_addr_A(wPrinterOpcode);  // ld [wPrinterOpcode], a

//  handshake
    LD_A(0x88);  // ld a, $88
    LDH_addr_A(rSB);  // ldh [rSB], a

//  switch to internal clock
    LD_A((0 << rSC_ON) | (1 << rSC_CLOCK));  // ld a, (0 << rSC_ON) | (1 << rSC_CLOCK)
    LDH_addr_A(rSC);  // ldh [rSC], a

//  start transfer
    LD_A((1 << rSC_ON) | (1 << rSC_CLOCK));  // ld a, (1 << rSC_ON) | (1 << rSC_CLOCK)
    LDH_addr_A(rSC);  // ldh [rSC], a

    RET;  // ret

}
