# Pokémon Gold and Silver [![Build Status][ci-badge]][ci]

This is an attempt at a PC port of Pokemon Gold and Silver. It currently runs, but is not recommended to be used until completed.

The goal is to get assets to load from outside files, and to be able to rewrite parts of the game in C. The game currently relies on a modified rom (Not provided, but can be compiled). Eventually, this requirement should be phased out.

**Overview**

The project starts off with a modified version of pokegold, with no reliance on some GameBoy specific features, like interrupts. The game is emulated, and parts of the ASM are converted to C macros. As these are converted, they are set up to re-direct the emulation code, to these macros. Since this is technically C, the workflow works as follows:
- Convert an ASM script to C macros
- Check that it works, make adjustments if needed
- After it's working, it's technically C, so the functions should be able to be re-written in actual C, and recompiled for testing

**Setup**

The root folder contains the modified pokegold project. This should be compiled (like normal pokegold), and pokesilver.gbc should be renamed to rom.gbc, and copied to the peanut-gb folder

in the peanut-gb folder, run

    make clean && make

Then run peanut-sdl to launch

**Process (Beginning)**

The C folder in the root directory is a copy of the main directory, but with the C code

Converting begins with running convASM.py (with python 3) with an ASM file

    ./convASM.py home/init.asm

The above will attempt to convert init.asm, and create home/init.c and home/init.h

The output will also list any lines that it may have not known how to convert, followed by a list of redirects

The redirects should be copied from the terminal window, and pasted into C/functions.h

Additionally, the .h file should be included (In this case, it is in home.h, which is included in functions.h)

After linking a converted ASM script, try building and running. If everything runs correctly, awesome! Otherwise some constants, macros, etc may need to be converted by hand

Occasionally, a modification to convASM.py may need to be made to handle future conversions

**Things to note**

- The emulator will run a frame of the game, until address 0x0018 is hit (considered the game's main loop/vblank)
- When looking to fetch an opcode, the game will check for redirects, if one is found on the current address, it will jump to that script instead
- Any return from this script will jump back to emulation, until a new redirect is found
    - This means things like "CALL, JUMP, RET" will actually return to emulation
- Any data reads are initially read from ROM (But should be factored out to not need reliance to ROM when being re-written in actual C)
- Functions are prefixed automatically with convASM.py as follows:
    - Func - The original C function
    - mFunc - The address of the function if it were currently loaded in ROM (0x0000 - 0x7FFF)
    - bFunc - The bank of the function in ROM
    - aFunc - The absolute address of the function in the ROM
- After called functions are converted, the "CALL(mFunc)" commands should be able to be changed to "Func()" for example. This should also be tested
- When first converted, if the original asm file had any includes, they will be commented out in the C file, and copied to the header file (also commented out). These will need to be manually uncommented (and the path corrected)

**Misc**

- The game is currently ran in GB mode, as this has less reliance on palettes, and no GBC specifics to deal with like RAM banks
- Due to the removal of reliance on interrupts, the following have been diabled and should be re-implemented once in C:
    - Scanline effects
    - Some delays that would wait for scanlines
        - This includes a lot of things, especially noticable in battles (like battle animations, and waiting for SFX/fanfares to finish)
    - Probably some more things I missed

Milestones

    -1 line macros of all opcodes

## See also

Other disassembly projects:

- [**Pokémon Red/Blue**][pokered]
- [**Pokémon Yellow**][pokeyellow]
- [**Pokémon Crystal**][pokecrystal]
- [**Pokémon Pinball**][pokepinball]
- [**Pokémon TCG**][poketcg]
- [**Pokémon Ruby**][pokeruby]
- [**Pokémon FireRed**][pokefirered]
- [**Pokémon Emerald**][pokeemerald]

[pokered]: https://github.com/pret/pokered
[pokeyellow]: https://github.com/pret/pokeyellow
[pokecrystal]: https://github.com/pret/pokecrystal
[pokepinball]: https://github.com/pret/pokepinball
[poketcg]: https://github.com/pret/poketcg
[pokeruby]: https://github.com/pret/pokeruby
[pokefirered]: https://github.com/pret/pokefirered
[pokeemerald]: https://github.com/pret/pokeemerald
[discord]: https://discord.gg/d5dubZ3
[irc]: https://web.libera.chat/?#pret
[ci]: https://github.com/pret/pokegold/actions
[ci-badge]: https://github.com/pret/pokegold/actions/workflows/main.yml/badge.svg
