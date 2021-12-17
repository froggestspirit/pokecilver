#include "peanut_gb.h"
#include "macros.h"
#include "global.h"

#define ROM_SIZE 0x200000


extern struct gb_s;

int (*func[ROM_SIZE])(struct gb_s *gb);

int _Start(struct gb_s *gb){
	CP_A(0x11);
	IF_Z goto _cgb;
	XOR_A_A;
	goto _load;

_cgb:
	LD_A(TRUE);

_load:
	LD_addr_A(hCGB);
    return 0x05D1;  // Function ends without a return, give it the address PC should be set to
}

void init_function_pointers(){
    for(int i = 0; i < ROM_SIZE; i++) func[i] = NULL;
    func[0x05C6] = _Start;
}
