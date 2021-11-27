/**
 * MIT License
 *
 * Copyright (c) 2018 Mahyar Koshkouei
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Please note that at least three parts of source code within this project was
 * taken from the SameBoy project at https://github.com/LIJI32/SameBoy/ which at
 * the time of this writing is released under the MIT License. Occurrences of
 * this code is marked as being taken from SameBoy with a comment.
 * SameBoy, and code marked as being taken from SameBoy,
 * is Copyright (c) 2015-2019 Lior Halphon.
 */

#pragma once

#include <stdint.h>	/* Required for int types */
#include <time.h>	/* Required for tm struct */

/**
 * Sound support must be provided by an external library. When audio_read() and
 * audio_write() functions are provided, define ENABLE_SOUND to a non-zero value
 * before including peanut_gb.h in order for these functions to be used.
 */
#ifndef ENABLE_SOUND
#	define ENABLE_SOUND 0
#endif

/* Enable LCD drawing. On by default. May be turned off for testing purposes. */
#ifndef ENABLE_LCD
#	define ENABLE_LCD 1
#endif

/* Interrupt masks */
#define VBLANK_INTR	0x01
#define LCDC_INTR	0x02
#define TIMER_INTR	0x04
#define SERIAL_INTR	0x08
#define CONTROL_INTR	0x10
#define ANY_INTR	0x1F

/* Memory section sizes for DMG */
#define WRAM_SIZE	0x2000
#define VRAM_SIZE	0x2000
#define HRAM_SIZE	0x0100
#define OAM_SIZE	0x00A0

/* Memory addresses */
#define ROM_0_ADDR      0x0000
#define ROM_N_ADDR      0x4000
#define VRAM_ADDR       0x8000
#define CART_RAM_ADDR   0xA000
#define WRAM_0_ADDR     0xC000
#define WRAM_1_ADDR     0xD000
#define ECHO_ADDR       0xE000
#define OAM_ADDR        0xFE00
#define UNUSED_ADDR     0xFEA0
#define IO_ADDR         0xFF00
#define HRAM_ADDR       0xFF80
#define INTR_EN_ADDR    0xFFFF

/* Cart section sizes */
#define ROM_BANK_SIZE   0x4000
#define WRAM_BANK_SIZE  0x1000
#define CRAM_BANK_SIZE  0x2000
#define VRAM_BANK_SIZE  0x2000

/* DIV Register is incremented at rate of 16384Hz.
 * 4194304 / 16384 = 256 clock cycles for one increment. */
#define DIV_CYCLES          256

/* Serial clock locked to 8192Hz on DMG.
 * 4194304 / (8192 / 8) = 4096 clock cycles for sending 1 byte. */
#define SERIAL_CYCLES		4096

/* Calculating VSYNC. */
#define DMG_CLOCK_FREQ		4194304.0
#define SCREEN_REFRESH_CYCLES	70224.0
#define VERTICAL_SYNC		(DMG_CLOCK_FREQ/SCREEN_REFRESH_CYCLES)

/* SERIAL SC register masks. */
#define SERIAL_SC_TX_START	0x80
#define SERIAL_SC_CLOCK_SRC	0x01

/* STAT register masks */
#define STAT_LYC_INTR       0x40
#define STAT_MODE_2_INTR    0x20
#define STAT_MODE_1_INTR    0x10
#define STAT_MODE_0_INTR    0x08
#define STAT_LYC_COINC      0x04
#define STAT_MODE           0x03
#define STAT_USER_BITS      0xF8

/* LCDC control masks */
#define LCDC_ENABLE         0x80
#define LCDC_WINDOW_MAP     0x40
#define LCDC_WINDOW_ENABLE  0x20
#define LCDC_TILE_SELECT    0x10
#define LCDC_BG_MAP         0x08
#define LCDC_OBJ_SIZE       0x04
#define LCDC_OBJ_ENABLE     0x02
#define LCDC_BG_ENABLE      0x01

/* LCD characteristics */
#define LCD_LINE_CYCLES     456
#define LCD_MODE_0_CYCLES   0
#define LCD_MODE_2_CYCLES   204
#define LCD_MODE_3_CYCLES   284
#define LCD_VERT_LINES      154
#define LCD_WIDTH           160
#define LCD_HEIGHT          144

/* VRAM Locations */
#define VRAM_TILES_1        (0x8000 - VRAM_ADDR)
#define VRAM_TILES_2        (0x8800 - VRAM_ADDR)
#define VRAM_BMAP_1         (0x9800 - VRAM_ADDR)
#define VRAM_BMAP_2         (0x9C00 - VRAM_ADDR)
#define VRAM_TILES_3        (0x8000 - VRAM_ADDR + VRAM_BANK_SIZE)
#define VRAM_TILES_4        (0x8800 - VRAM_ADDR + VRAM_BANK_SIZE)

/* Interrupt jump addresses */
#define VBLANK_INTR_ADDR    0x0040
#define LCDC_INTR_ADDR      0x0048
#define TIMER_INTR_ADDR     0x0050
#define SERIAL_INTR_ADDR    0x0058
#define CONTROL_INTR_ADDR   0x0060

/* SPRITE controls */
#define NUM_SPRITES         0x28
#define MAX_SPRITES_LINE    0x0A
#define OBJ_PRIORITY        0x80
#define OBJ_FLIP_Y          0x40
#define OBJ_FLIP_X          0x20
#define OBJ_PALETTE         0x10

#define ROM_HEADER_CHECKSUM_LOC	0x014D

#ifndef MIN
	#define MIN(a, b)   ((a) < (b) ? (a) : (b))
#endif

struct cpu_registers_s
{
	/* Combine A and F registers. */
	union
	{
		struct
		{
			/* Define specific bits of Flag register. */
			union
			{
				struct
				{
					unsigned unused : 4;
					unsigned c : 1; /* Carry flag. */
					unsigned h : 1; /* Half carry flag. */
					unsigned n : 1; /* Add/sub flag. */
					unsigned z : 1; /* Zero flag. */
				} f_bits;
				uint8_t f;
			};
			uint8_t a;
		};
		uint16_t af;
	};

	union
	{
		struct
		{
			uint8_t c;
			uint8_t b;
		};
		uint16_t bc;
	};

	union
	{
		struct
		{
			uint8_t e;
			uint8_t d;
		};
		uint16_t de;
	};

	union
	{
		struct
		{
			uint8_t l;
			uint8_t h;
		};
		uint16_t hl;
	};

	uint16_t sp; /* Stack pointer */
	uint16_t pc; /* Program counter */
};

struct count_s
{
	uint_fast16_t lcd_count;	/* LCD Timing */
	uint_fast16_t div_count;	/* Divider Register Counter */
	uint_fast16_t tima_count;	/* Timer Counter */
	uint_fast16_t serial_count;	/* Serial Counter */
};

struct gb_registers_s
{
	/* TODO: Sort variables in address order. */
	/* Timing */
	uint8_t TIMA, TMA, DIV;
	union
	{
		struct
		{
			unsigned tac_rate : 2;	/* Input clock select */
			unsigned tac_enable : 1;	/* Timer enable */
			unsigned unused : 5;
		};
		uint8_t TAC;
	};

	/* LCD */
	uint8_t LCDC;
	uint8_t STAT;
	uint8_t SCY;
	uint8_t SCX;
	uint8_t LY;
	uint8_t LYC;
	uint8_t DMA;
	uint8_t BGP;
	uint8_t OBP0;
	uint8_t OBP1;
	uint8_t WY;
	uint8_t WX;

	/* Joypad info. */
	uint8_t P1;

	/* Serial data. */
	uint8_t SB;
	uint8_t SC;

	/* Interrupt flag. */
	uint8_t IF;

	/* Interrupt enable. */
	uint8_t IE;
};

#if ENABLE_LCD
	/* Bit mask for the shade of pixel to display */
	#define LCD_COLOUR	0x03
	/**
	* Bit mask for whether a pixel is OBJ0, OBJ1, or BG. Each may have a different
	* palette when playing a DMG game on CGB.
	*/
	#define LCD_PALETTE_OBJ	0x10
	#define LCD_PALETTE_BG	0x20
	/**
	* Bit mask for the two bits listed above.
	* LCD_PALETTE_ALL == 0b00 --> OBJ0
	* LCD_PALETTE_ALL == 0b01 --> OBJ1
	* LCD_PALETTE_ALL == 0b10 --> BG
	* LCD_PALETTE_ALL == 0b11 --> NOT POSSIBLE
	*/
	#define LCD_PALETTE_ALL 0x30
#endif

/**
 * Errors that may occur during emulation.
 */
enum gb_error_e
{
	GB_UNKNOWN_ERROR,
	GB_INVALID_OPCODE,
	GB_INVALID_READ,
	GB_INVALID_WRITE,

	GB_INVALID_MAX
};

/**
 * Errors that may occur during library initialisation.
 */
enum gb_init_error_e
{
	GB_INIT_NO_ERROR,
	GB_INIT_CARTRIDGE_UNSUPPORTED,
	GB_INIT_INVALID_CHECKSUM
};

/**
 * Return codes for serial receive function, mainly for clarity.
 */
enum gb_serial_rx_ret_e
{
	GB_SERIAL_RX_SUCCESS = 0,
	GB_SERIAL_RX_NO_CONNECTION = 1
};

/**
 * Emulator context.
 *
 * Only values within the `direct` struct may be modified directly by the
 * front-end implementation. Other variables must not be modified.
 */
struct gb_s
{
	/**
	 * Return byte from ROM at given address.
	 *
	 * \param gb_s	emulator context
	 * \param addr	address
	 * \return		byte at address in ROM
	 */
	uint8_t (*gb_rom_read)(struct gb_s*, const uint_fast32_t addr);

	/**
	 * Return byte from cart RAM at given address.
	 *
	 * \param gb_s	emulator context
	 * \param addr	address
	 * \return		byte at address in RAM
	 */
	uint8_t (*gb_cart_ram_read)(struct gb_s*, const uint_fast32_t addr);

	/**
	 * Write byte to cart RAM at given address.
	 *
	 * \param gb_s	emulator context
	 * \param addr	address
	 * \param val	value to write to address in RAM
	 */
	void (*gb_cart_ram_write)(struct gb_s*, const uint_fast32_t addr,
				  const uint8_t val);

	/**
	 * Notify front-end of error.
	 *
	 * \param gb_s			emulator context
	 * \param gb_error_e	error code
	 * \param val			arbitrary value related to error
	 */
	void (*gb_error)(struct gb_s*, const enum gb_error_e, const uint16_t val);

	/* Transmit one byte and return the received byte. */
	void (*gb_serial_tx)(struct gb_s*, const uint8_t tx);
	enum gb_serial_rx_ret_e (*gb_serial_rx)(struct gb_s*, uint8_t* rx);

	struct
	{
		unsigned gb_halt	: 1;
		unsigned gb_ime		: 1;
		unsigned gb_bios_enable : 1;
		unsigned gb_frame	: 1; /* New frame drawn. */

#		define LCD_HBLANK	0
#		define LCD_VBLANK	1
#		define LCD_SEARCH_OAM	2
#		define LCD_TRANSFER	3
		unsigned lcd_mode	: 2;
	};

	/* Cartridge information:
	 * Memory Bank Controller (MBC) type. */
	uint8_t mbc;
	/* Whether the MBC has internal RAM. */
	uint8_t cart_ram;
	/* Number of ROM banks in cartridge. */
	uint16_t num_rom_banks_mask;
	/* Number of RAM banks in cartridge. */
	uint8_t num_ram_banks;

	uint16_t selected_rom_bank;
	/* WRAM and VRAM bank selection not available. */
	uint8_t cart_ram_bank;
	uint8_t enable_cart_ram;
	/* Cartridge ROM/RAM mode select. */
	uint8_t cart_mode_select;
	union
	{
		struct
		{
			uint8_t sec;
			uint8_t min;
			uint8_t hour;
			uint8_t yday;
			uint8_t high;
		} rtc_bits;
		uint8_t cart_rtc[5];
	};

	struct cpu_registers_s cpu_reg;
	struct gb_registers_s gb_reg;
	struct count_s counter;

	/* TODO: Allow implementation to allocate WRAM, VRAM and Frame Buffer. */
	uint8_t wram[WRAM_SIZE];
	uint8_t vram[VRAM_SIZE];
	uint8_t hram[HRAM_SIZE];
	uint8_t oam[OAM_SIZE];

	struct
	{
		/**
		 * Draw line on screen.
		 *
		 * \param gb_s		emulator context
		 * \param pixels	The 160 pixels to draw.
		 * 			Bits 1-0 are the colour to draw.
		 * 			Bits 5-4 are the palette, where:
		 * 				OBJ0 = 0b00,
		 * 				OBJ1 = 0b01,
		 * 				BG = 0b10
		 * 			Other bits are undefined.
		 * 			Bits 5-4 are only required by front-ends
		 * 			which want to use a different colour for
		 * 			different object palettes. This is what
		 * 			the Game Boy Color (CGB) does to DMG
		 * 			games.
		 * \param line		Line to draw pixels on. This is
		 * guaranteed to be between 0-144 inclusive.
		 */
		void (*lcd_draw_line)(struct gb_s *gb,
				const uint8_t *pixels,
				const uint_fast8_t line);

		/* Palettes */
		uint8_t bg_palette[4];
		uint8_t sp_palette[8];

		uint8_t window_clear;
		uint8_t WY;

		/* Only support 30fps frame skip. */
		unsigned frame_skip_count : 1;
		unsigned interlace_count : 1;
	} display;

	/**
	 * Variables that may be modified directly by the front-end.
	 * This method seems to be easier and possibly less overhead than
	 * calling a function to modify these variables each time.
	 *
	 * None of this is thread-safe.
	 */
	struct
	{
		/* Set to enable interlacing. Interlacing will start immediately
		 * (at the next line drawing).
		 */
		unsigned interlace : 1;
		unsigned frame_skip : 1;

		union
		{
			struct
			{
				unsigned a		: 1;
				unsigned b		: 1;
				unsigned select	: 1;
				unsigned start	: 1;
				unsigned right	: 1;
				unsigned left	: 1;
				unsigned up		: 1;
				unsigned down	: 1;
			} joypad_bits;
			uint8_t joypad;
		};

		/* Implementation defined data. Set to NULL if not required. */
		void *priv;
	} direct;
};

/**
 * Tick the internal RTC by one second.
 * This was taken from SameBoy, which is released under MIT Licence.
 */
void gb_tick_rtc(struct gb_s *gb)
{
	/* is timer running? */
	if((gb->cart_rtc[4] & 0x40) == 0)
	{
		if(++gb->rtc_bits.sec == 60)
		{
			gb->rtc_bits.sec = 0;

			if(++gb->rtc_bits.min == 60)
			{
				gb->rtc_bits.min = 0;

				if(++gb->rtc_bits.hour == 24)
				{
					gb->rtc_bits.hour = 0;

					if(++gb->rtc_bits.yday == 0)
					{
						if(gb->rtc_bits.high & 1)  /* Bit 8 of days*/
						{
							gb->rtc_bits.high |= 0x80; /* Overflow bit */
						}

						gb->rtc_bits.high ^= 1;
					}
				}
			}
		}
	}
}

/**
 * Set initial values in RTC.
 * Should be called after gb_init().
 */
void gb_set_rtc(struct gb_s *gb, const struct tm * const time)
{
	gb->cart_rtc[0] = time->tm_sec;
	gb->cart_rtc[1] = time->tm_min;
	gb->cart_rtc[2] = time->tm_hour;
	gb->cart_rtc[3] = time->tm_yday & 0xFF; /* Low 8 bits of day counter. */
	gb->cart_rtc[4] = time->tm_yday >> 8; /* High 1 bit of day counter. */
}

/**
 * Internal function used to read bytes.
 */
uint8_t __gb_read(struct gb_s *gb, const uint_fast16_t addr)
{
	switch(addr >> 12)
	{
	case 0x0:

	/* TODO: BIOS support. */
	case 0x1:
	case 0x2:
	case 0x3:
		return gb->gb_rom_read(gb, addr);

	case 0x4:
	case 0x5:
	case 0x6:
	case 0x7:
		if(gb->mbc == 1 && gb->cart_mode_select)
			return gb->gb_rom_read(gb,
					       addr + ((gb->selected_rom_bank & 0x1F) - 1) * ROM_BANK_SIZE);
		else
			return gb->gb_rom_read(gb, addr + (gb->selected_rom_bank - 1) * ROM_BANK_SIZE);

	case 0x8:
	case 0x9:
		return gb->vram[addr - VRAM_ADDR];

	case 0xA:
	case 0xB:
		if(gb->cart_ram && gb->enable_cart_ram)
		{
			if(gb->mbc == 3 && gb->cart_ram_bank >= 0x08)
				return gb->cart_rtc[gb->cart_ram_bank - 0x08];
			else if((gb->cart_mode_select || gb->mbc != 1) &&
					gb->cart_ram_bank < gb->num_ram_banks)
			{
				return gb->gb_cart_ram_read(gb, addr - CART_RAM_ADDR +
							    (gb->cart_ram_bank * CRAM_BANK_SIZE));
			}
			else
				return gb->gb_cart_ram_read(gb, addr - CART_RAM_ADDR);
		}

		return 0xFF;

	case 0xC:
		return gb->wram[addr - WRAM_0_ADDR];

	case 0xD:
		return gb->wram[addr - WRAM_1_ADDR + WRAM_BANK_SIZE];

	case 0xE:
		return gb->wram[addr - ECHO_ADDR];

	case 0xF:
		if(addr < OAM_ADDR)
			return gb->wram[addr - ECHO_ADDR];

		if(addr < UNUSED_ADDR)
			return gb->oam[addr - OAM_ADDR];

		/* Unusable memory area. Reading from this area returns 0.*/
		if(addr < IO_ADDR)
			return 0xFF;

		/* HRAM */
		if(HRAM_ADDR <= addr && addr < INTR_EN_ADDR)
			return gb->hram[addr - HRAM_ADDR];

		if((addr >= 0xFF10) && (addr <= 0xFF3F))
		{
#if ENABLE_SOUND
			return audio_read(addr);
#else
			return 1;
#endif
		}

		/* IO and Interrupts. */
		switch(addr & 0xFF)
		{
		/* IO Registers */
		case 0x00:
			return 0xC0 | gb->gb_reg.P1;

		case 0x01:
			return gb->gb_reg.SB;

		case 0x02:
			return gb->gb_reg.SC;

		/* Timer Registers */
		case 0x04:
			return gb->gb_reg.DIV;

		case 0x05:
			return gb->gb_reg.TIMA;

		case 0x06:
			return gb->gb_reg.TMA;

		case 0x07:
			return gb->gb_reg.TAC;

		/* Interrupt Flag Register */
		case 0x0F:
			return gb->gb_reg.IF;

		/* LCD Registers */
		case 0x40:
			return gb->gb_reg.LCDC;

		case 0x41:
			return (gb->gb_reg.STAT & STAT_USER_BITS) |
			       (gb->gb_reg.LCDC & LCDC_ENABLE ? gb->lcd_mode : LCD_VBLANK);

		case 0x42:
			return gb->gb_reg.SCY;

		case 0x43:
			return gb->gb_reg.SCX;

		case 0x44:
			return gb->gb_reg.LY;

		case 0x45:
			return gb->gb_reg.LYC;

		/* DMA Register */
		case 0x46:
			return gb->gb_reg.DMA;

		/* DMG Palette Registers */
		case 0x47:
			return gb->gb_reg.BGP;

		case 0x48:
			return gb->gb_reg.OBP0;

		case 0x49:
			return gb->gb_reg.OBP1;

		/* Window Position Registers */
		case 0x4A:
			return gb->gb_reg.WY;

		case 0x4B:
			return gb->gb_reg.WX;

		/* Interrupt Enable Register */
		case 0xFF:
			return gb->gb_reg.IE;

		/* Unused registers return 1 */
		default:
			return 0xFF;
		}
	}

	(gb->gb_error)(gb, GB_INVALID_READ, addr);
	return 0xFF;
}

/**
 * Internal function used to write bytes.
 */
void __gb_write(struct gb_s *gb, const uint_fast16_t addr, const uint8_t val)
{
	switch(addr >> 12)
	{
	case 0x0:
	case 0x1:
		if(gb->mbc == 2 && addr & 0x10)
			return;
		else if(gb->mbc > 0 && gb->cart_ram)
			gb->enable_cart_ram = ((val & 0x0F) == 0x0A);

		return;

	case 0x2:
		if(gb->mbc == 5)
		{
			gb->selected_rom_bank = (gb->selected_rom_bank & 0x100) | val;
			gb->selected_rom_bank =
				gb->selected_rom_bank & gb->num_rom_banks_mask;
			return;
		}

	/* Intentional fall through. */

	case 0x3:
		if(gb->mbc == 1)
		{
			//selected_rom_bank = val & 0x7;
			gb->selected_rom_bank = (val & 0x1F) | (gb->selected_rom_bank & 0x60);

			if((gb->selected_rom_bank & 0x1F) == 0x00)
				gb->selected_rom_bank++;
		}
		else if(gb->mbc == 2 && addr & 0x10)
		{
			gb->selected_rom_bank = val & 0x0F;

			if(!gb->selected_rom_bank)
				gb->selected_rom_bank++;
		}
		else if(gb->mbc == 3)
		{
			gb->selected_rom_bank = val;

			if(!gb->selected_rom_bank)
				gb->selected_rom_bank++;
		}
		else if(gb->mbc == 5)
			gb->selected_rom_bank = (val & 0x01) << 8 | (gb->selected_rom_bank & 0xFF);

		gb->selected_rom_bank = gb->selected_rom_bank & gb->num_rom_banks_mask;
		return;

	case 0x4:
	case 0x5:
		if(gb->mbc == 1)
		{
			gb->cart_ram_bank = (val & 3);
			gb->selected_rom_bank = ((val & 3) << 5) | (gb->selected_rom_bank & 0x1F);
			gb->selected_rom_bank = gb->selected_rom_bank & gb->num_rom_banks_mask;
		}
		else if(gb->mbc == 3)
			gb->cart_ram_bank = val;
		else if(gb->mbc == 5)
			gb->cart_ram_bank = (val & 0x0F);

		return;

	case 0x6:
	case 0x7:
		gb->cart_mode_select = (val & 1);
		return;

	case 0x8:
	case 0x9:
		gb->vram[addr - VRAM_ADDR] = val;
		return;

	case 0xA:
	case 0xB:
		if(gb->cart_ram && gb->enable_cart_ram)
		{
			if(gb->mbc == 3 && gb->cart_ram_bank >= 0x08)
				gb->cart_rtc[gb->cart_ram_bank - 0x08] = val;
			else if(gb->cart_mode_select &&
					gb->cart_ram_bank < gb->num_ram_banks)
			{
				gb->gb_cart_ram_write(gb,
						      addr - CART_RAM_ADDR + (gb->cart_ram_bank * CRAM_BANK_SIZE), val);
			}
			else if(gb->num_ram_banks)
				gb->gb_cart_ram_write(gb, addr - CART_RAM_ADDR, val);
		}

		return;

	case 0xC:
		gb->wram[addr - WRAM_0_ADDR] = val;
		return;

	case 0xD:
		gb->wram[addr - WRAM_1_ADDR + WRAM_BANK_SIZE] = val;
		return;

	case 0xE:
		gb->wram[addr - ECHO_ADDR] = val;
		return;

	case 0xF:
		if(addr < OAM_ADDR)
		{
			gb->wram[addr - ECHO_ADDR] = val;
			return;
		}

		if(addr < UNUSED_ADDR)
		{
			gb->oam[addr - OAM_ADDR] = val;
			return;
		}

		/* Unusable memory area. */
		if(addr < IO_ADDR)
			return;

		if(HRAM_ADDR <= addr && addr < INTR_EN_ADDR)
		{
			gb->hram[addr - HRAM_ADDR] = val;
			return;
		}

		if((addr >= 0xFF10) && (addr <= 0xFF3F))
		{
#if ENABLE_SOUND
			audio_write(addr, val);
#endif
			return;
		}

		/* IO and Interrupts. */
		switch(addr & 0xFF)
		{
		/* Joypad */
		case 0x00:
			/* Only bits 5 and 4 are R/W.
			 * The lower bits are overwritten later, and the two most
			 * significant bits are unused. */
			gb->gb_reg.P1 = val;

			/* Direction keys selected */
			if((gb->gb_reg.P1 & 0b010000) == 0)
				gb->gb_reg.P1 |= (gb->direct.joypad >> 4);
			/* Button keys selected */
			else
				gb->gb_reg.P1 |= (gb->direct.joypad & 0x0F);

			return;

		/* Serial */
		case 0x01:
			gb->gb_reg.SB = val;
			return;

		case 0x02:
			gb->gb_reg.SC = val;
			return;

		/* Timer Registers */
		case 0x04:
			gb->gb_reg.DIV = 0x00;
			return;

		case 0x05:
			gb->gb_reg.TIMA = val;
			return;

		case 0x06:
			gb->gb_reg.TMA = val;
			return;

		case 0x07:
			gb->gb_reg.TAC = val;
			return;

		/* Interrupt Flag Register */
		case 0x0F:
			gb->gb_reg.IF = (val | 0b11100000);
			return;

		/* LCD Registers */
		case 0x40:
			gb->gb_reg.LCDC = val;

			/* LY fixed to 0 when LCD turned off. */
			if((gb->gb_reg.LCDC & LCDC_ENABLE) == 0)
			{
				/* Do not turn off LCD outside of VBLANK. This may
				 * happen due to poor timing in this emulator. */
				if(gb->lcd_mode != LCD_VBLANK)
				{
					gb->gb_reg.LCDC |= LCDC_ENABLE;
					return;
				}

				gb->gb_reg.STAT = (gb->gb_reg.STAT & ~0x03) | LCD_VBLANK;
				gb->gb_reg.LY = 0;
				gb->counter.lcd_count = 0;
			}

			return;

		case 0x41:
			gb->gb_reg.STAT = (val & 0b01111000);
			return;

		case 0x42:
			gb->gb_reg.SCY = val;
			return;

		case 0x43:
			gb->gb_reg.SCX = val;
			return;

		/* LY (0xFF44) is read only. */
		case 0x45:
			gb->gb_reg.LYC = val;
			return;

		/* DMA Register */
		case 0x46:
			gb->gb_reg.DMA = (val % 0xF1);

			for(uint8_t i = 0; i < OAM_SIZE; i++)
				gb->oam[i] = __gb_read(gb, (gb->gb_reg.DMA << 8) + i);

			return;

		/* DMG Palette Registers */
		case 0x47:
			gb->gb_reg.BGP = val;
			gb->display.bg_palette[0] = (gb->gb_reg.BGP & 0x03);
			gb->display.bg_palette[1] = (gb->gb_reg.BGP >> 2) & 0x03;
			gb->display.bg_palette[2] = (gb->gb_reg.BGP >> 4) & 0x03;
			gb->display.bg_palette[3] = (gb->gb_reg.BGP >> 6) & 0x03;
			return;

		case 0x48:
			gb->gb_reg.OBP0 = val;
			gb->display.sp_palette[0] = (gb->gb_reg.OBP0 & 0x03);
			gb->display.sp_palette[1] = (gb->gb_reg.OBP0 >> 2) & 0x03;
			gb->display.sp_palette[2] = (gb->gb_reg.OBP0 >> 4) & 0x03;
			gb->display.sp_palette[3] = (gb->gb_reg.OBP0 >> 6) & 0x03;
			return;

		case 0x49:
			gb->gb_reg.OBP1 = val;
			gb->display.sp_palette[4] = (gb->gb_reg.OBP1 & 0x03);
			gb->display.sp_palette[5] = (gb->gb_reg.OBP1 >> 2) & 0x03;
			gb->display.sp_palette[6] = (gb->gb_reg.OBP1 >> 4) & 0x03;
			gb->display.sp_palette[7] = (gb->gb_reg.OBP1 >> 6) & 0x03;
			return;

		/* Window Position Registers */
		case 0x4A:
			gb->gb_reg.WY = val;
			return;

		case 0x4B:
			gb->gb_reg.WX = val;
			return;

		/* Turn off boot ROM */
		case 0x50:
			gb->gb_bios_enable = 0;
			return;

		/* Interrupt Enable Register */
		case 0xFF:
			gb->gb_reg.IE = val;
			return;
		}
	}

	(gb->gb_error)(gb, GB_INVALID_WRITE, addr);
}

uint8_t __gb_execute_cb(struct gb_s *gb)
{
	uint8_t inst_cycles;
	uint8_t cbop = __gb_read(gb, gb->cpu_reg.pc++);
	uint8_t r = (cbop & 0x7);
	uint8_t b = (cbop >> 3) & 0x7;
	uint8_t d = (cbop >> 3) & 0x1;
	uint8_t val;
	uint8_t writeback = 1;

	inst_cycles = 8;
	switch(r)
	{
	case 0:
		val = gb->cpu_reg.b;
		break;

	case 1:
		val = gb->cpu_reg.c;
		break;

	case 2:
		val = gb->cpu_reg.d;
		break;

	case 3:
		val = gb->cpu_reg.e;
		break;

	case 4:
		val = gb->cpu_reg.h;
		break;

	case 5:
		val = gb->cpu_reg.l;
		break;

	case 6:
		val = __gb_read(gb, gb->cpu_reg.hl);
		break;

	/* Only values 0-7 are possible here, so we make the final case
	 * default to satisfy -Wmaybe-uninitialized warning. */
	default:
		val = gb->cpu_reg.a;
		break;
	}

	/* TODO: Find out WTF this is doing. */
	switch(cbop >> 6)
	{
	case 0x0:
		cbop = (cbop >> 4) & 0x3;

		switch(cbop)
		{
		case 0x0: /* RdC R */
		case 0x1: /* Rd R */
			if(d) /* RRC R / RR R */
			{
				uint8_t temp = val;
				val = (val >> 1);
				val |= cbop ? (gb->cpu_reg.f_bits.c << 7) : (temp << 7);
				gb->cpu_reg.f_bits.z = (val == 0x00);
				gb->cpu_reg.f_bits.n = 0;
				gb->cpu_reg.f_bits.h = 0;
				gb->cpu_reg.f_bits.c = (temp & 0x01);
			}
			else /* RLC R / RL R */
			{
				uint8_t temp = val;
				val = (val << 1);
				val |= cbop ? gb->cpu_reg.f_bits.c : (temp >> 7);
				gb->cpu_reg.f_bits.z = (val == 0x00);
				gb->cpu_reg.f_bits.n = 0;
				gb->cpu_reg.f_bits.h = 0;
				gb->cpu_reg.f_bits.c = (temp >> 7);
			}

			break;

		case 0x2:
			if(d) /* SRA R */
			{
				gb->cpu_reg.f_bits.c = val & 0x01;
				val = (val >> 1) | (val & 0x80);
				gb->cpu_reg.f_bits.z = (val == 0x00);
				gb->cpu_reg.f_bits.n = 0;
				gb->cpu_reg.f_bits.h = 0;
			}
			else /* SLA R */
			{
				gb->cpu_reg.f_bits.c = (val >> 7);
				val = val << 1;
				gb->cpu_reg.f_bits.z = (val == 0x00);
				gb->cpu_reg.f_bits.n = 0;
				gb->cpu_reg.f_bits.h = 0;
			}

			break;

		case 0x3:
			if(d) /* SRL R */
			{
				gb->cpu_reg.f_bits.c = val & 0x01;
				val = val >> 1;
				gb->cpu_reg.f_bits.z = (val == 0x00);
				gb->cpu_reg.f_bits.n = 0;
				gb->cpu_reg.f_bits.h = 0;
			}
			else /* SWAP R */
			{
				uint8_t temp = (val >> 4) & 0x0F;
				temp |= (val << 4) & 0xF0;
				val = temp;
				gb->cpu_reg.f_bits.z = (val == 0x00);
				gb->cpu_reg.f_bits.n = 0;
				gb->cpu_reg.f_bits.h = 0;
				gb->cpu_reg.f_bits.c = 0;
			}

			break;
		}

		break;

	case 0x1: /* BIT B, R */
		gb->cpu_reg.f_bits.z = !((val >> b) & 0x1);
		gb->cpu_reg.f_bits.n = 0;
		gb->cpu_reg.f_bits.h = 1;
		writeback = 0;
		break;

	case 0x2: /* RES B, R */
		val &= (0xFE << b) | (0xFF >> (8 - b));
		break;

	case 0x3: /* SET B, R */
		val |= (0x1 << b);
		break;
	}

	if(writeback)
	{
		switch(r)
		{
		case 0:
			gb->cpu_reg.b = val;
			break;

		case 1:
			gb->cpu_reg.c = val;
			break;

		case 2:
			gb->cpu_reg.d = val;
			break;

		case 3:
			gb->cpu_reg.e = val;
			break;

		case 4:
			gb->cpu_reg.h = val;
			break;

		case 5:
			gb->cpu_reg.l = val;
			break;

		case 6:
			__gb_write(gb, gb->cpu_reg.hl, val);
			break;

		case 7:
			gb->cpu_reg.a = val;
			break;
		}
	}
	return inst_cycles;
}

#if ENABLE_LCD
void __gb_draw_line(struct gb_s *gb)
{
	uint8_t pixels[160] = {0};

	/* If LCD not initialised by front-end, don't render anything. */
	if(gb->display.lcd_draw_line == NULL)
		return;

	if(gb->direct.frame_skip && !gb->display.frame_skip_count)
		return;

	/* If interlaced mode is activated, check if we need to draw the current
	 * line. */
	if(gb->direct.interlace)
	{
		if((gb->display.interlace_count == 0
				&& (gb->gb_reg.LY & 1) == 0)
				|| (gb->display.interlace_count == 1
				    && (gb->gb_reg.LY & 1) == 1))
		{
			/* Compensate for missing window draw if required. */
			if(gb->gb_reg.LCDC & LCDC_WINDOW_ENABLE
					&& gb->gb_reg.LY >= gb->display.WY
					&& gb->gb_reg.WX <= 166)
				gb->display.window_clear++;

			return;
		}
	}

	/* If background is enabled, draw it. */
	if(gb->gb_reg.LCDC & LCDC_BG_ENABLE)
	{
		/* Calculate current background line to draw. Constant because
		 * this function draws only this one line each time it is
		 * called. */
		const uint8_t bg_y = gb->gb_reg.LY + gb->gb_reg.SCY;

		/* Get selected background map address for first tile
		 * corresponding to current line.
		 * 0x20 (32) is the width of a background tile, and the bit
		 * shift is to calculate the address. */
		const uint16_t bg_map =
			((gb->gb_reg.LCDC & LCDC_BG_MAP) ?
			 VRAM_BMAP_2 : VRAM_BMAP_1)
			+ (bg_y >> 3) * 0x20;

		/* The displays (what the player sees) X coordinate, drawn right
		 * to left. */
		uint8_t disp_x = LCD_WIDTH - 1;

		/* The X coordinate to begin drawing the background at. */
		uint8_t bg_x = disp_x + gb->gb_reg.SCX;

		/* Get tile index for current background tile. */
		uint8_t idx = gb->vram[bg_map + (bg_x >> 3)];
		/* Y coordinate of tile pixel to draw. */
		const uint8_t py = (bg_y & 0x07);
		/* X coordinate of tile pixel to draw. */
		uint8_t px = 7 - (bg_x & 0x07);

		uint16_t tile;

		/* Select addressing mode. */
		if(gb->gb_reg.LCDC & LCDC_TILE_SELECT)
			tile = VRAM_TILES_1 + idx * 0x10;
		else
			tile = VRAM_TILES_2 + ((idx + 0x80) % 0x100) * 0x10;

		tile += 2 * py;

		/* fetch first tile */
		uint8_t t1 = gb->vram[tile] >> px;
		uint8_t t2 = gb->vram[tile + 1] >> px;

		for(; disp_x != 0xFF; disp_x--)
		{
			if(px == 8)
			{
				/* fetch next tile */
				px = 0;
				bg_x = disp_x + gb->gb_reg.SCX;
				idx = gb->vram[bg_map + (bg_x >> 3)];

				if(gb->gb_reg.LCDC & LCDC_TILE_SELECT)
					tile = VRAM_TILES_1 + idx * 0x10;
				else
					tile = VRAM_TILES_2 + ((idx + 0x80) % 0x100) * 0x10;

				tile += 2 * py;
				t1 = gb->vram[tile];
				t2 = gb->vram[tile + 1];
			}

			/* copy background */
			uint8_t c = (t1 & 0x1) | ((t2 & 0x1) << 1);
			pixels[disp_x] = gb->display.bg_palette[c];
			pixels[disp_x] |= LCD_PALETTE_BG;
			t1 = t1 >> 1;
			t2 = t2 >> 1;
			px++;
		}
	}

	/* draw window */
	if(gb->gb_reg.LCDC & LCDC_WINDOW_ENABLE
			&& gb->gb_reg.LY >= gb->display.WY
			&& gb->gb_reg.WX <= 166)
	{
		/* Calculate Window Map Address. */
		uint16_t win_line = (gb->gb_reg.LCDC & LCDC_WINDOW_MAP) ?
				    VRAM_BMAP_2 : VRAM_BMAP_1;
		win_line += (gb->display.window_clear >> 3) * 0x20;

		uint8_t disp_x = LCD_WIDTH - 1;
		uint8_t win_x = disp_x - gb->gb_reg.WX + 7;

		// look up tile
		uint8_t py = gb->display.window_clear & 0x07;
		uint8_t px = 7 - (win_x & 0x07);
		uint8_t idx = gb->vram[win_line + (win_x >> 3)];

		uint16_t tile;

		if(gb->gb_reg.LCDC & LCDC_TILE_SELECT)
			tile = VRAM_TILES_1 + idx * 0x10;
		else
			tile = VRAM_TILES_2 + ((idx + 0x80) % 0x100) * 0x10;

		tile += 2 * py;

		// fetch first tile
		uint8_t t1 = gb->vram[tile] >> px;
		uint8_t t2 = gb->vram[tile + 1] >> px;

		// loop & copy window
		uint8_t end = (gb->gb_reg.WX < 7 ? 0 : gb->gb_reg.WX - 7) - 1;

		for(; disp_x != end; disp_x--)
		{
			if(px == 8)
			{
				// fetch next tile
				px = 0;
				win_x = disp_x - gb->gb_reg.WX + 7;
				idx = gb->vram[win_line + (win_x >> 3)];

				if(gb->gb_reg.LCDC & LCDC_TILE_SELECT)
					tile = VRAM_TILES_1 + idx * 0x10;
				else
					tile = VRAM_TILES_2 + ((idx + 0x80) % 0x100) * 0x10;

				tile += 2 * py;
				t1 = gb->vram[tile];
				t2 = gb->vram[tile + 1];
			}

			// copy window
			uint8_t c = (t1 & 0x1) | ((t2 & 0x1) << 1);
			pixels[disp_x] = gb->display.bg_palette[c];
			pixels[disp_x] |= LCD_PALETTE_BG;
			t1 = t1 >> 1;
			t2 = t2 >> 1;
			px++;
		}

		gb->display.window_clear++; // advance window line
	}

	// draw sprites
	if(gb->gb_reg.LCDC & LCDC_OBJ_ENABLE)
	{
		uint8_t count = 0;

		for(uint8_t s = NUM_SPRITES - 1;
				s != 0xFF /* && count < MAX_SPRITES_LINE */ ;
				s--)
		{
			/* Sprite Y position. */
			uint8_t OY = gb->oam[4 * s + 0];
			/* Sprite X position. */
			uint8_t OX = gb->oam[4 * s + 1];
			/* Sprite Tile/Pattern Number. */
			uint8_t OT = gb->oam[4 * s + 2]
				     & (gb->gb_reg.LCDC & LCDC_OBJ_SIZE ? 0xFE : 0xFF);
			/* Additional attributes. */
			uint8_t OF = gb->oam[4 * s + 3];

			/* If sprite isn't on this line, continue. */
			if(gb->gb_reg.LY +
					(gb->gb_reg.LCDC & LCDC_OBJ_SIZE ?
					 0 : 8) >= OY
					|| gb->gb_reg.LY + 16 < OY)
				continue;

			count++;

			/* Continue if sprite not visible. */
			if(OX == 0 || OX >= 168)
				continue;

			// y flip
			uint8_t py = gb->gb_reg.LY - OY + 16;

			if(OF & OBJ_FLIP_Y)
				py = (gb->gb_reg.LCDC & LCDC_OBJ_SIZE ? 15 : 7) - py;

			// fetch the tile
			uint8_t t1 = gb->vram[VRAM_TILES_1 + OT * 0x10 + 2 * py];
			uint8_t t2 = gb->vram[VRAM_TILES_1 + OT * 0x10 + 2 * py + 1];

			// handle x flip
			uint8_t dir, start, end, shift;

			if(OF & OBJ_FLIP_X)
			{
				dir = 1;
				start = (OX < 8 ? 0 : OX - 8);
				end = MIN(OX, LCD_WIDTH);
				shift = 8 - OX + start;
			}
			else
			{
				dir = -1;
				start = MIN(OX, LCD_WIDTH) - 1;
				end = (OX < 8 ? 0 : OX - 8) - 1;
				shift = OX - (start + 1);
			}

			// copy tile
			t1 >>= shift;
			t2 >>= shift;

			for(uint8_t disp_x = start; disp_x != end; disp_x += dir)
			{
				uint8_t c = (t1 & 0x1) | ((t2 & 0x1) << 1);
				// check transparency / sprite overlap / background overlap
#if 0

				if(c
						//	&& OX <= fx[disp_x]
						&& !((OF & OBJ_PRIORITY)
						     && ((pixels[disp_x] & 0x3)
							 && fx[disp_x] == 0xFE)))
#else
				if(c && !(OF & OBJ_PRIORITY
						&& pixels[disp_x] & 0x3))
#endif
				{
					/* Set pixel colour. */
					pixels[disp_x] = (OF & OBJ_PALETTE)
							 ? gb->display.sp_palette[c + 4]
							 : gb->display.sp_palette[c];
					/* Set pixel palette (OBJ0 or OBJ1). */
					pixels[disp_x] |= (OF & OBJ_PALETTE);
					/* Deselect BG palette. */
					pixels[disp_x] &= ~LCD_PALETTE_BG;
				}

				t1 = t1 >> 1;
				t2 = t2 >> 1;
			}
		}
	}

	gb->display.lcd_draw_line(gb, pixels, gb->gb_reg.LY);
}
#endif

/**
 * Internal function used to step the CPU.
 */

#include "macros.h"
#include "functions.h"
extern int (*func[ROM_SIZE])(struct gb_s *gb);

void __gb_step_cpu(struct gb_s *gb)
{
	uint8_t opcode, inst_cycles;

	if(gb->cpu_reg.pc == 0x18) gb->gb_frame = 1;
	if(gb->cpu_reg.pc < 0x8000){
		if(func[gb->cpu_reg.pc + ((gb->selected_rom_bank - 1) * ROM_BANK_SIZE)] != NULL){
			gb->cpu_reg.pc = func[gb->cpu_reg.pc + ((gb->selected_rom_bank - 1) * ROM_BANK_SIZE)](gb);
			if(gb->cpu_reg.pc == 0x18) gb->gb_frame = 1;
		}
	}
	/* Obtain opcode */
	opcode = (gb->gb_halt ? 0x00 : __gb_read(gb, gb->cpu_reg.pc++));


	/* Execute opcode */
	switch(opcode)
	{
	case 0x00:	/* NOP */	break;
	case 0x01:	LD_BC(imm16);	break;
	case 0x02:	LD_bc_A;	break;
	case 0x03:	INC_BC;	break;
	case 0x04:	INC_B;	break;
	case 0x05:	DEC_B;	break;
	case 0x06:	LD_B(imm8);	break;
	case 0x07:	RLCA;	break;
	case 0x08:	LD_addr_SP(imm16);	break;
	case 0x09:	ADD_HL_BC;	break;
	case 0x0A:	LD_A_bc;	break;
	case 0x0B:	DEC_BC;	break;
	case 0x0C:	INC_C;	break;
	case 0x0D:	DEC_C;	break;
	case 0x0E:	LD_C(imm8);	break;
	case 0x0F:	RRCA;	break;
	case 0x10: /* STOP */	break;
	case 0x11:	LD_DE(imm16);	break;
	case 0x12:	LD_de_A;	break;
	case 0x13:	INC_DE;	break;
	case 0x14:	INC_D;	break;
	case 0x15:	DEC_D;	break;
	case 0x16:	LD_D(imm8);	break;
	case 0x17:	RLA;	break;
	case 0x18:	JR(imm8);	break;
	case 0x19:	ADD_HL_DE;	break;
	case 0x1A:	LD_A_de;	break;
	case 0x1B:	DEC_DE;	break;
	case 0x1C:	INC_E;	break;
	case 0x1D:	DEC_E;	break;
	case 0x1E:	LD_E(imm8);	break;
	case 0x1F:	RRA;	break;
	case 0x20:	JR_NZ(imm8);	break;
	case 0x21:	LD_HL(imm16);	break;
	case 0x22:	LD_hli_A;	break;
	case 0x23:	INC_HL;	break;
	case 0x24:	INC_H;	break;
	case 0x25:	DEC_H;	break;
	case 0x26:	LD_H(imm8);	break;
	case 0x27:	DAA;	break;
	case 0x28:	JR_Z(imm8);	break;
	case 0x29:	ADD_HL_HL;	break;
	case 0x2A:	LD_A_hli;	break;
	case 0x2B:	DEC_HL;	break;
	case 0x2C:	INC_L;	break;
	case 0x2D:	DEC_L;	break;
	case 0x2E:	LD_L(imm8);	break;
	case 0x2F:	CPL;	break;
	case 0x30:	JR_NC(imm8);	break;
	case 0x31:	LD_SP(imm16);	break;
	case 0x32:	LD_hld_A;	break;
	case 0x33:	INC_SP;	break;
	case 0x34:	INC_hl;	break;
	case 0x35:	DEC_hl;	break;
	case 0x36:	LD_hl(imm8);	break;
	case 0x37:	SCF;	break;
	case 0x38:	JR_C(imm8);	break;
	case 0x39:	ADD_HL_SP;	break;
	case 0x3A:	LD_A_hld;	break;
	case 0x3B:	DEC_SP;	break;
	case 0x3C:	INC_A;	break;
	case 0x3D:	DEC_A;	break;
	case 0x3E:	LD_A(imm8);	break;
	case 0x3F:	CCF;	break;
	case 0x40:	LD_B_B;	break;
	case 0x41:	LD_B_C;	break;
	case 0x42:	LD_B_D;	break;
	case 0x43:	LD_B_E;	break;
	case 0x44:	LD_B_H;	break;
	case 0x45:	LD_B_L;	break;
	case 0x46:	LD_B_hl;	break;
	case 0x47:	LD_B_A;	break;
	case 0x48:	LD_C_B;	break;
	case 0x49:	LD_C_C;	break;
	case 0x4A:	LD_C_D;	break;
	case 0x4B:	LD_C_E;	break;
	case 0x4C:	LD_C_H;	break;
	case 0x4D:	LD_C_L;	break;
	case 0x4E:	LD_C_hl;	break;
	case 0x4F:	LD_C_A;	break;
	case 0x50:	LD_D_B;	break;
	case 0x51:	LD_D_C;	break;
	case 0x52:	LD_D_D;	break;
	case 0x53:	LD_D_E;	break;
	case 0x54:	LD_D_H;	break;
	case 0x55:	LD_D_L;	break;
	case 0x56:	LD_D_hl;	break;
	case 0x57:	LD_D_A;	break;
	case 0x58:	LD_E_B;	break;
	case 0x59:	LD_E_C;	break;
	case 0x5A:	LD_E_D;	break;
	case 0x5B:	LD_E_E;	break;
	case 0x5C:	LD_E_H;	break;
	case 0x5D:	LD_E_L;	break;
	case 0x5E:	LD_E_hl;	break;
	case 0x5F:	LD_E_A;	break;
	case 0x60:	LD_H_B;	break;
	case 0x61:	LD_H_C;	break;
	case 0x62:	LD_H_D;	break;
	case 0x63:	LD_H_E;	break;
	case 0x64:	LD_H_H;	break;
	case 0x65:	LD_H_L;	break;
	case 0x66:	LD_H_hl;	break;
	case 0x67:	LD_H_A;	break;
	case 0x68:	LD_L_B;	break;
	case 0x69:	LD_L_C;	break;
	case 0x6A:	LD_L_D;	break;
	case 0x6B:	LD_L_E;	break;
	case 0x6C:	LD_L_H;	break;
	case 0x6D:	LD_L_L;	break;
	case 0x6E:	LD_L_hl;	break;
	case 0x6F:	LD_L_A;	break;
	case 0x70:	LD_hl_B;	break;
	case 0x71:	LD_hl_C;	break;
	case 0x72:	LD_hl_D;	break;
	case 0x73:	LD_hl_E;	break;
	case 0x74:	LD_hl_H;	break;
	case 0x75:	LD_hl_L;	break;
	case 0x76:	LD_hl_hl;	break;
	case 0x77:	LD_hl_A;	break;
	case 0x78:	LD_A_B;	break;
	case 0x79:	LD_A_C;	break;
	case 0x7A:	LD_A_D;	break;
	case 0x7B:	LD_A_E;	break;
	case 0x7C:	LD_A_H;	break;
	case 0x7D:	LD_A_L;	break;
	case 0x7E:	LD_A_hl;	break;
	case 0x7F:	LD_A_A;	break;
	case 0x80:	ADD_A_B;	break;
	case 0x81:	ADD_A_C;	break;
	case 0x82:	ADD_A_D;	break;
	case 0x83:	ADD_A_E;	break;
	case 0x84:	ADD_A_H;	break;
	case 0x85:	ADD_A_L;	break;
	case 0x86:	ADD_A_hl;	break;
	case 0x87:	ADD_A_A;	break;
	case 0x88:	ADC_A_B;	break;
	case 0x89:	ADC_A_C;	break;
	case 0x8A:	ADC_A_D;	break;
	case 0x8B:	ADC_A_E;	break;
	case 0x8C:	ADC_A_H;	break;
	case 0x8D:	ADC_A_L;	break;
	case 0x8E:	ADC_A_hl;	break;
	case 0x8F:	ADC_A_A;	break;
	case 0x90:	SUB_A_B;	break;
	case 0x91:	SUB_A_C;	break;
	case 0x92:	SUB_A_D;	break;
	case 0x93:	SUB_A_E;	break;
	case 0x94:	SUB_A_H;	break;
	case 0x95:	SUB_A_L;	break;
	case 0x96:	SUB_A_hl;	break;
	case 0x97:	SUB_A_A;	break;
	case 0x98:	SBC_A_B;	break;
	case 0x99:	SBC_A_C;	break;
	case 0x9A:	SBC_A_D;	break;
	case 0x9B:	SBC_A_E;	break;
	case 0x9C:	SBC_A_H;	break;
	case 0x9D:	SBC_A_L;	break;
	case 0x9E:	SBC_A_hl;	break;
	case 0x9F:	SBC_A_A;	break;
	case 0xA0:	AND_A_B;	break;
	case 0xA1:	AND_A_C;	break;
	case 0xA2:	AND_A_D;	break;
	case 0xA3:	AND_A_E;	break;
	case 0xA4:	AND_A_H;	break;
	case 0xA5:	AND_A_L;	break;
	case 0xA6:	AND_A_hl;	break;
	case 0xA7:	AND_A_A;	break;
	case 0xA8:	XOR_A_B;	break;
	case 0xA9:	XOR_A_C;	break;
	case 0xAA:	XOR_A_D;	break;
	case 0xAB:	XOR_A_E;	break;
	case 0xAC:	XOR_A_H;	break;
	case 0xAD:	XOR_A_L;	break;
	case 0xAE:	XOR_A_hl;	break;
	case 0xAF:	XOR_A_A;	break;
	case 0xB0:	OR_A_B;	break;
	case 0xB1:	OR_A_C;	break;
	case 0xB2:	OR_A_D;	break;
	case 0xB3:	OR_A_E;	break;
	case 0xB4:	OR_A_H;	break;
	case 0xB5:	OR_A_L;	break;
	case 0xB6:	OR_A_hl;	break;
	case 0xB7:	OR_A_A;	break;
	case 0xB8:	CP_A_B;	break;
	case 0xB9:	CP_A_C;	break;
	case 0xBA:	CP_A_D;	break;
	case 0xBB:	CP_A_E;	break;
	case 0xBC:	CP_A_H;	break;
	case 0xBD:	CP_A_L;	break;
	case 0xBE:	CP_A_hl;	break;
	case 0xBF:	CP_A_A;	break;
	case 0xC0:	RET_NZ;	break;
	case 0xC1:	POP_BC;	break;
	case 0xC2:	JP_NZ(imm16);	break;
	case 0xC3:	JP(imm16);	break;
	case 0xC4:	CALL_NZ(imm16);	break;
	case 0xC5:	PUSH_BC;	break;
	case 0xC6:	ADD_A(imm8);	break;
	case 0xC7:	RST(0x00);	break;  // RST
	case 0xC8:	RET_Z;	break;
	case 0xC9:	RET;	break;
	case 0xCA:	JP_Z(imm16);	break;
	case 0xCC:	CALL_Z(imm16);	break;
	case 0xCD:	CALL(imm16);	break;
	case 0xCE:	ADC_A(imm8);	break;
	case 0xCF:	RST(0x08);	break;  // RST
	case 0xD0:	RET_NC;	break;
	case 0xD1:	POP_DE;	break;
	case 0xD2:	JP_NC(imm16);	break;
	case 0xD4:	CALL_NC(imm16);	break;
	case 0xD5:	PUSH_DE;	break;
	case 0xD6:	SUB_A(imm8);	break;
	case 0xD7:	RST(0x10);	break;  // RST
	case 0xD8:	RET_C;	break;
	case 0xD9:	RETI;	break;
	case 0xDA:	JP_C(imm16);	break;
	case 0xDC:	CALL_C(imm16);	break;
	case 0xDE:	SBC_A(imm8);	break;
	case 0xDF:	RST(0x18);	break;  // RST
	case 0xE0:	LD_addr_A(0xFF00 + imm8);	break;
	case 0xE1:	POP_HL;	break;
	case 0xE2:	LD_c_A;	break;
	case 0xE5:	PUSH_HL;	break;
	case 0xE6:	AND_A(imm8);	break;
	case 0xE7:	RST(0x20);	break;  // RST
	case 0xE8:	ADD_SP(imm16);	break;
	case 0xE9:	JP_hl;	break;
	case 0xEA:	LD_addr_A(imm16);	break;
	case 0xEE:	XOR_A(imm8);	break;
	case 0xEF:	RST(0x28);	break;  // RST
	case 0xF0:	LD_A_addr(0xFF00 + imm8);	break;
	case 0xF1:	POP_AF;	break;
	case 0xF2:	LD_A_c;	break;
	case 0xF3:	/* DI */	break;
	case 0xF5:	PUSH_AF;	break;
	case 0xF6:	OR_A(imm8);	break;
	case 0xF7:	RST(0x30);	break;  // RST
	case 0xF8:	LD_HL_SP(imm8);	break;
	case 0xF9:	LD_SP_HL;	break;
	case 0xFA:	LD_A_addr(imm16);	break;
	case 0xFB:	/* EI */	break;
	case 0xFE:	CP_A(imm8);	break;
	case 0xFF:	RST(0x38);	break;  // RST
	case 0xCB: /* CB INST */
		uint8_t op = __gb_read(gb, gb->cpu_reg.pc++);
		switch(op){
		case 0x00:	RLC_B;	break;
		case 0x01:	RLC_C;	break;
		case 0x02:	RLC_D;	break;
		case 0x03:	RLC_E;	break;
		case 0x04:	RLC_H;	break;
		case 0x05:	RLC_L;	break;
		case 0x06:	RLC_hl;	break;
		case 0x07:	RLC_A;	break;
		case 0x08:	RRC_B;	break;
		case 0x09:	RRC_C;	break;
		case 0x0A:	RRC_D;	break;
		case 0x0B:	RRC_E;	break;
		case 0x0C:	RRC_H;	break;
		case 0x0D:	RRC_L;	break;
		case 0x0E:	RRC_hl;	break;
		case 0x0F:	RRC_A;	break;
		case 0x10:	RL_B;	break;
		case 0x11:	RL_C;	break;
		case 0x12:	RL_D;	break;
		case 0x13:	RL_E;	break;
		case 0x14:	RL_H;	break;
		case 0x15:	RL_L;	break;
		case 0x16:	RL_hl;	break;
		case 0x17:	RL_A;	break;
		case 0x18:	RR_B;	break;
		case 0x19:	RR_C;	break;
		case 0x1A:	RR_D;	break;
		case 0x1B:	RR_E;	break;
		case 0x1C:	RR_H;	break;
		case 0x1D:	RR_L;	break;
		case 0x1E:	RR_hl;	break;
		case 0x1F:	RR_A;	break;
		case 0x20:	SLA_B;	break;
		case 0x21:	SLA_C;	break;
		case 0x22:	SLA_D;	break;
		case 0x23:	SLA_E;	break;
		case 0x24:	SLA_H;	break;
		case 0x25:	SLA_L;	break;
		case 0x26:	SLA_hl;	break;
		case 0x27:	SLA_A;	break;
		case 0x28:	SRA_B;	break;
		case 0x29:	SRA_C;	break;
		case 0x2A:	SRA_D;	break;
		case 0x2B:	SRA_E;	break;
		case 0x2C:	SRA_H;	break;
		case 0x2D:	SRA_L;	break;
		case 0x2E:	SRA_hl;	break;
		case 0x2F:	SRA_A;	break;
		case 0x30:	SWAP_B;	break;
		case 0x31:	SWAP_C;	break;
		case 0x32:	SWAP_D;	break;
		case 0x33:	SWAP_E;	break;
		case 0x34:	SWAP_H;	break;
		case 0x35:	SWAP_L;	break;
		case 0x36:	SWAP_hl;	break;
		case 0x37:	SWAP_A;	break;
		case 0x38:	SRL_B;	break;
		case 0x39:	SRL_C;	break;
		case 0x3A:	SRL_D;	break;
		case 0x3B:	SRL_E;	break;
		case 0x3C:	SRL_H;	break;
		case 0x3D:	SRL_L;	break;
		case 0x3E:	SRL_hl;	break;
		case 0x3F:	SRL_A;	break;
		case 0x40:	BIT_B(0);	break;
		case 0x41:	BIT_C(0);	break;
		case 0x42:	BIT_D(0);	break;
		case 0x43:	BIT_E(0);	break;
		case 0x44:	BIT_H(0);	break;
		case 0x45:	BIT_L(0);	break;
		case 0x46:	BIT_hl(0);	break;
		case 0x47:	BIT_A(0);	break;
		case 0x48:	BIT_B(1);	break;
		case 0x49:	BIT_C(1);	break;
		case 0x4A:	BIT_D(1);	break;
		case 0x4B:	BIT_E(1);	break;
		case 0x4C:	BIT_H(1);	break;
		case 0x4D:	BIT_L(1);	break;
		case 0x4E:	BIT_hl(1);	break;
		case 0x4F:	BIT_A(1);	break;
		case 0x50:	BIT_B(2);	break;
		case 0x51:	BIT_C(2);	break;
		case 0x52:	BIT_D(2);	break;
		case 0x53:	BIT_E(2);	break;
		case 0x54:	BIT_H(2);	break;
		case 0x55:	BIT_L(2);	break;
		case 0x56:	BIT_hl(2);	break;
		case 0x57:	BIT_A(2);	break;
		case 0x58:	BIT_B(3);	break;
		case 0x59:	BIT_C(3);	break;
		case 0x5A:	BIT_D(3);	break;
		case 0x5B:	BIT_E(3);	break;
		case 0x5C:	BIT_H(3);	break;
		case 0x5D:	BIT_L(3);	break;
		case 0x5E:	BIT_hl(3);	break;
		case 0x5F:	BIT_A(3);	break;
		case 0x60:	BIT_B(4);	break;
		case 0x61:	BIT_C(4);	break;
		case 0x62:	BIT_D(4);	break;
		case 0x63:	BIT_E(4);	break;
		case 0x64:	BIT_H(4);	break;
		case 0x65:	BIT_L(4);	break;
		case 0x66:	BIT_hl(4);	break;
		case 0x67:	BIT_A(4);	break;
		case 0x68:	BIT_B(5);	break;
		case 0x69:	BIT_C(5);	break;
		case 0x6A:	BIT_D(5);	break;
		case 0x6B:	BIT_E(5);	break;
		case 0x6C:	BIT_H(5);	break;
		case 0x6D:	BIT_L(5);	break;
		case 0x6E:	BIT_hl(5);	break;
		case 0x6F:	BIT_A(5);	break;
		case 0x70:	BIT_B(6);	break;
		case 0x71:	BIT_C(6);	break;
		case 0x72:	BIT_D(6);	break;
		case 0x73:	BIT_E(6);	break;
		case 0x74:	BIT_H(6);	break;
		case 0x75:	BIT_L(6);	break;
		case 0x76:	BIT_hl(6);	break;
		case 0x77:	BIT_A(6);	break;
		case 0x78:	BIT_B(7);	break;
		case 0x79:	BIT_C(7);	break;
		case 0x7A:	BIT_D(7);	break;
		case 0x7B:	BIT_E(7);	break;
		case 0x7C:	BIT_H(7);	break;
		case 0x7D:	BIT_L(7);	break;
		case 0x7E:	BIT_hl(7);	break;
		case 0x7F:	BIT_A(7);	break;
		case 0x80:	RES_B(0);	break;
		case 0x81:	RES_C(0);	break;
		case 0x82:	RES_D(0);	break;
		case 0x83:	RES_E(0);	break;
		case 0x84:	RES_H(0);	break;
		case 0x85:	RES_L(0);	break;
		case 0x86:	RES_hl(0);	break;
		case 0x87:	RES_A(0);	break;
		case 0x88:	RES_B(1);	break;
		case 0x89:	RES_C(1);	break;
		case 0x8A:	RES_D(1);	break;
		case 0x8B:	RES_E(1);	break;
		case 0x8C:	RES_H(1);	break;
		case 0x8D:	RES_L(1);	break;
		case 0x8E:	RES_hl(1);	break;
		case 0x8F:	RES_A(1);	break;
		case 0x90:	RES_B(2);	break;
		case 0x91:	RES_C(2);	break;
		case 0x92:	RES_D(2);	break;
		case 0x93:	RES_E(2);	break;
		case 0x94:	RES_H(2);	break;
		case 0x95:	RES_L(2);	break;
		case 0x96:	RES_hl(2);	break;
		case 0x97:	RES_A(2);	break;
		case 0x98:	RES_B(3);	break;
		case 0x99:	RES_C(3);	break;
		case 0x9A:	RES_D(3);	break;
		case 0x9B:	RES_E(3);	break;
		case 0x9C:	RES_H(3);	break;
		case 0x9D:	RES_L(3);	break;
		case 0x9E:	RES_hl(3);	break;
		case 0x9F:	RES_A(3);	break;
		case 0xA0:	RES_B(4);	break;
		case 0xA1:	RES_C(4);	break;
		case 0xA2:	RES_D(4);	break;
		case 0xA3:	RES_E(4);	break;
		case 0xA4:	RES_H(4);	break;
		case 0xA5:	RES_L(4);	break;
		case 0xA6:	RES_hl(4);	break;
		case 0xA7:	RES_A(4);	break;
		case 0xA8:	RES_B(5);	break;
		case 0xA9:	RES_C(5);	break;
		case 0xAA:	RES_D(5);	break;
		case 0xAB:	RES_E(5);	break;
		case 0xAC:	RES_H(5);	break;
		case 0xAD:	RES_L(5);	break;
		case 0xAE:	RES_hl(5);	break;
		case 0xAF:	RES_A(5);	break;
		case 0xB0:	RES_B(6);	break;
		case 0xB1:	RES_C(6);	break;
		case 0xB2:	RES_D(6);	break;
		case 0xB3:	RES_E(6);	break;
		case 0xB4:	RES_H(6);	break;
		case 0xB5:	RES_L(6);	break;
		case 0xB6:	RES_hl(6);	break;
		case 0xB7:	RES_A(6);	break;
		case 0xB8:	RES_B(7);	break;
		case 0xB9:	RES_C(7);	break;
		case 0xBA:	RES_D(7);	break;
		case 0xBB:	RES_E(7);	break;
		case 0xBC:	RES_H(7);	break;
		case 0xBD:	RES_L(7);	break;
		case 0xBE:	RES_hl(7);	break;
		case 0xBF:	RES_A(7);	break;
		case 0xC0:	SET_B(0);	break;
		case 0xC1:	SET_C(0);	break;
		case 0xC2:	SET_D(0);	break;
		case 0xC3:	SET_E(0);	break;
		case 0xC4:	SET_H(0);	break;
		case 0xC5:	SET_L(0);	break;
		case 0xC6:	SET_hl(0);	break;
		case 0xC7:	SET_A(0);	break;
		case 0xC8:	SET_B(1);	break;
		case 0xC9:	SET_C(1);	break;
		case 0xCA:	SET_D(1);	break;
		case 0xCB:	SET_E(1);	break;
		case 0xCC:	SET_H(1);	break;
		case 0xCD:	SET_L(1);	break;
		case 0xCE:	SET_hl(1);	break;
		case 0xCF:	SET_A(1);	break;
		case 0xD0:	SET_B(2);	break;
		case 0xD1:	SET_C(2);	break;
		case 0xD2:	SET_D(2);	break;
		case 0xD3:	SET_E(2);	break;
		case 0xD4:	SET_H(2);	break;
		case 0xD5:	SET_L(2);	break;
		case 0xD6:	SET_hl(2);	break;
		case 0xD7:	SET_A(2);	break;
		case 0xD8:	SET_B(3);	break;
		case 0xD9:	SET_C(3);	break;
		case 0xDA:	SET_D(3);	break;
		case 0xDB:	SET_E(3);	break;
		case 0xDC:	SET_H(3);	break;
		case 0xDD:	SET_L(3);	break;
		case 0xDE:	SET_hl(3);	break;
		case 0xDF:	SET_A(3);	break;
		case 0xE0:	SET_B(4);	break;
		case 0xE1:	SET_C(4);	break;
		case 0xE2:	SET_D(4);	break;
		case 0xE3:	SET_E(4);	break;
		case 0xE4:	SET_H(4);	break;
		case 0xE5:	SET_L(4);	break;
		case 0xE6:	SET_hl(4);	break;
		case 0xE7:	SET_A(4);	break;
		case 0xE8:	SET_B(5);	break;
		case 0xE9:	SET_C(5);	break;
		case 0xEA:	SET_D(5);	break;
		case 0xEB:	SET_E(5);	break;
		case 0xEC:	SET_H(5);	break;
		case 0xED:	SET_L(5);	break;
		case 0xEE:	SET_hl(5);	break;
		case 0xEF:	SET_A(5);	break;
		case 0xF0:	SET_B(6);	break;
		case 0xF1:	SET_C(6);	break;
		case 0xF2:	SET_D(6);	break;
		case 0xF3:	SET_E(6);	break;
		case 0xF4:	SET_H(6);	break;
		case 0xF5:	SET_L(6);	break;
		case 0xF6:	SET_hl(6);	break;
		case 0xF7:	SET_A(6);	break;
		case 0xF8:	SET_B(7);	break;
		case 0xF9:	SET_C(7);	break;
		case 0xFA:	SET_D(7);	break;
		case 0xFB:	SET_E(7);	break;
		case 0xFC:	SET_H(7);	break;
		case 0xFD:	SET_L(7);	break;
		case 0xFE:	SET_hl(7);	break;
		case 0xFF:	SET_A(7);	break;
		}
		break;

	default:
		(gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
	}

	inst_cycles = 4; // make this static to remove a dependancy on it

	/* DIV register timing */
	gb->counter.div_count += inst_cycles;

	if(gb->counter.div_count >= DIV_CYCLES)
	{
		gb->gb_reg.DIV++;
		gb->counter.div_count -= DIV_CYCLES;
	}

	/* Check serial transmission. */
	if(gb->gb_reg.SC & SERIAL_SC_TX_START)
	{
		/* If new transfer, call TX function. */
		if(gb->counter.serial_count == 0 && gb->gb_serial_tx != NULL)
			(gb->gb_serial_tx)(gb, gb->gb_reg.SB);

		gb->counter.serial_count += inst_cycles;

		/* If it's time to receive byte, call RX function. */
		if(gb->counter.serial_count >= SERIAL_CYCLES)
		{
			/* If RX can be done, do it. */
			/* If RX failed, do not change SB if using external
			 * clock, or set to 0xFF if using internal clock. */
			uint8_t rx;

			if(gb->gb_serial_rx != NULL &&
				(gb->gb_serial_rx(gb, &rx) ==
					 GB_SERIAL_RX_SUCCESS))
			{
				gb->gb_reg.SB = rx;

				/* Inform game of serial TX/RX completion. */
				gb->gb_reg.SC &= 0x01;
				gb->gb_reg.IF |= SERIAL_INTR;
			}
			else if(gb->gb_reg.SC & SERIAL_SC_CLOCK_SRC)
			{
				/* If using internal clock, and console is not
				 * attached to any external peripheral, shifted
				 * bits are replaced with logic 1. */
				gb->gb_reg.SB = 0xFF;

				/* Inform game of serial TX/RX completion. */
				gb->gb_reg.SC &= 0x01;
				gb->gb_reg.IF |= SERIAL_INTR;
			}
			else
			{
				/* If using external clock, and console is not
				 * attached to any external peripheral, bits are
				 * not shifted, so SB is not modified. */
			}

			gb->counter.serial_count = 0;
		}
	}

	/* TIMA register timing */
	/* TODO: Change tac_enable to struct of TAC timer control bits. */
	if(gb->gb_reg.tac_enable)
	{
		static const uint_fast16_t TAC_CYCLES[4] = {1024, 16, 64, 256};

		gb->counter.tima_count += inst_cycles;

		while(gb->counter.tima_count >= TAC_CYCLES[gb->gb_reg.tac_rate])
		{
			gb->counter.tima_count -= TAC_CYCLES[gb->gb_reg.tac_rate];

			if(++gb->gb_reg.TIMA == 0)
			{
				gb->gb_reg.IF |= TIMER_INTR;
				/* On overflow, set TMA to TIMA. */
				gb->gb_reg.TIMA = gb->gb_reg.TMA;
			}
		}
	}

	/* TODO Check behaviour of LCD during LCD power off state. */
	/* If LCD is off, don't update LCD state. */
	if((gb->gb_reg.LCDC & LCDC_ENABLE) == 0)
		return;

	/* LCD Timing */
	gb->counter.lcd_count += inst_cycles;

	/* New Scanline */
	if(gb->counter.lcd_count > LCD_LINE_CYCLES)
	{
		gb->counter.lcd_count -= LCD_LINE_CYCLES;

		/* LYC Update */
		if(gb->gb_reg.LY == gb->gb_reg.LYC)
		{
			gb->gb_reg.STAT |= STAT_LYC_COINC;

			if(gb->gb_reg.STAT & STAT_LYC_INTR)
				gb->gb_reg.IF |= LCDC_INTR;
		}
		else
			gb->gb_reg.STAT &= 0xFB;

		/* Next line */
		gb->gb_reg.LY = (gb->gb_reg.LY + 1) % LCD_VERT_LINES;

		/* VBLANK Start */
		if(gb->gb_reg.LY == LCD_HEIGHT)
		{
			gb->lcd_mode = LCD_VBLANK;
			//gb->gb_frame = 1;
			gb->gb_reg.IF |= VBLANK_INTR;

			if(gb->gb_reg.STAT & STAT_MODE_1_INTR)
				gb->gb_reg.IF |= LCDC_INTR;

#if ENABLE_LCD

			/* If frame skip is activated, check if we need to draw
			 * the frame or skip it. */
			if(gb->direct.frame_skip)
			{
				gb->display.frame_skip_count =
					!gb->display.frame_skip_count;
			}

			/* If interlaced is activated, change which lines get
			 * updated. Also, only update lines on frames that are
			 * actually drawn when frame skip is enabled. */
			if(gb->direct.interlace &&
					(!gb->direct.frame_skip ||
					 gb->display.frame_skip_count))
			{
				gb->display.interlace_count =
					!gb->display.interlace_count;
			}

#endif
		}
		/* Normal Line */
		else if(gb->gb_reg.LY < LCD_HEIGHT)
		{
			if(gb->gb_reg.LY == 0)
			{
				/* Clear Screen */
				gb->display.WY = gb->gb_reg.WY;
				gb->display.window_clear = 0;
			}

			gb->lcd_mode = LCD_HBLANK;

			if(gb->gb_reg.STAT & STAT_MODE_0_INTR)
				gb->gb_reg.IF |= LCDC_INTR;
		}
	}
	/* OAM access */
	else if(gb->lcd_mode == LCD_HBLANK
			&& gb->counter.lcd_count >= LCD_MODE_2_CYCLES)
	{
		gb->lcd_mode = LCD_SEARCH_OAM;

		if(gb->gb_reg.STAT & STAT_MODE_2_INTR)
			gb->gb_reg.IF |= LCDC_INTR;
	}
	/* Update LCD */
	else if(gb->lcd_mode == LCD_SEARCH_OAM
			&& gb->counter.lcd_count >= LCD_MODE_3_CYCLES)
	{
		gb->lcd_mode = LCD_TRANSFER;
#if ENABLE_LCD
		__gb_draw_line(gb);
#endif
	}
}

void gb_run_frame(struct gb_s *gb)
{
	gb->gb_frame = 0;

	while(!gb->gb_frame)
		__gb_step_cpu(gb);
}

/**
 * Gets the size of the save file required for the ROM.
 */
uint_fast32_t gb_get_save_size(struct gb_s *gb)
{
	const uint_fast16_t ram_size_location = 0x0149;
	const uint_fast32_t ram_sizes[] =
	{
		0x00, 0x800, 0x2000, 0x8000, 0x20000
	};
	uint8_t ram_size = gb->gb_rom_read(gb, ram_size_location);
	return ram_sizes[ram_size];
}

/**
 * Set the function used to handle serial transfer in the front-end. This is
 * optional.
 * gb_serial_transfer takes a byte to transmit and returns the received byte. If
 * no cable is connected to the console, return 0xFF.
 */
void gb_init_serial(struct gb_s *gb,
		    void (*gb_serial_tx)(struct gb_s*, const uint8_t),
		    enum gb_serial_rx_ret_e (*gb_serial_rx)(struct gb_s*,
			    uint8_t*))
{
	gb->gb_serial_tx = gb_serial_tx;
	gb->gb_serial_rx = gb_serial_rx;
}

uint8_t gb_colour_hash(struct gb_s *gb)
{
#define ROM_TITLE_START_ADDR	0x0134
#define ROM_TITLE_END_ADDR	0x0143

	uint8_t x = 0;

	for(uint16_t i = ROM_TITLE_START_ADDR; i <= ROM_TITLE_END_ADDR; i++)
		x += gb->gb_rom_read(gb, i);

	return x;
}

/**
 * Resets the context, and initialises startup values.
 */
void gb_reset(struct gb_s *gb)
{
	init_function_pointers();
	gb->gb_halt = 0;
	gb->gb_ime = 1;
	gb->gb_bios_enable = 0;
	gb->lcd_mode = LCD_HBLANK;

	/* Initialise MBC values. */
	gb->selected_rom_bank = 1;
	gb->cart_ram_bank = 0;
	gb->enable_cart_ram = 0;
	gb->cart_mode_select = 0;

	/* Initialise CPU registers as though a DMG. */
	gb->cpu_reg.af = 0x01B0;
	gb->cpu_reg.bc = 0x0013;
	gb->cpu_reg.de = 0x00D8;
	gb->cpu_reg.hl = 0x014D;
	gb->cpu_reg.sp = 0xFFFE;
	/* TODO: Add BIOS support. */
	gb->cpu_reg.pc = 0x0100;

	gb->counter.lcd_count = 0;
	gb->counter.div_count = 0;
	gb->counter.tima_count = 0;
	gb->counter.serial_count = 0;

	gb->gb_reg.TIMA      = 0x00;
	gb->gb_reg.TMA       = 0x00;
	gb->gb_reg.TAC       = 0xF8;
	gb->gb_reg.DIV       = 0xAC;

	gb->gb_reg.IF        = 0xE1;

	gb->gb_reg.LCDC      = 0x91;
	gb->gb_reg.SCY       = 0x00;
	gb->gb_reg.SCX       = 0x00;
	gb->gb_reg.LYC       = 0x00;

	/* Appease valgrind for invalid reads and unconditional jumps. */
	gb->gb_reg.SC = 0x7E;
	gb->gb_reg.STAT = 0;
	gb->gb_reg.LY = 0;

	__gb_write(gb, 0xFF47, 0xFC);    // BGP
	__gb_write(gb, 0xFF48, 0xFF);    // OBJP0
	__gb_write(gb, 0xFF49, 0x0F);    // OBJP1
	gb->gb_reg.WY        = 0x00;
	gb->gb_reg.WX        = 0x00;
	gb->gb_reg.IE        = 0x00;

	gb->direct.joypad = 0xFF;
	gb->gb_reg.P1 = 0xCF;
}

/**
 * Initialise the emulator context. gb_reset() is also called to initialise
 * the CPU.
 */
enum gb_init_error_e gb_init(struct gb_s *gb,
			     uint8_t (*gb_rom_read)(struct gb_s*, const uint_fast32_t),
			     uint8_t (*gb_cart_ram_read)(struct gb_s*, const uint_fast32_t),
			     void (*gb_cart_ram_write)(struct gb_s*, const uint_fast32_t, const uint8_t),
			     void (*gb_error)(struct gb_s*, const enum gb_error_e, const uint16_t),
			     void *priv)
{
	const uint16_t mbc_location = 0x0147;
	const uint16_t bank_count_location = 0x0148;
	const uint16_t ram_size_location = 0x0149;
	/**
	 * Table for cartridge type (MBC). -1 if invalid.
	 * TODO: MMM01 is untested.
	 * TODO: MBC6 is untested.
	 * TODO: MBC7 is unsupported.
	 * TODO: POCKET CAMERA is unsupported.
	 * TODO: BANDAI TAMA5 is unsupported.
	 * TODO: HuC3 is unsupported.
	 * TODO: HuC1 is unsupported.
	 **/
	const uint8_t cart_mbc[] =
	{
		0, 1, 1, 1, -1, 2, 2, -1, 0, 0, -1, 0, 0, 0, -1, 3,
		3, 3, 3, 3, -1, -1, -1, -1, -1, 5, 5, 5, 5, 5, 5, -1
	};
	const uint8_t cart_ram[] =
	{
		0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
		1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0
	};
	const uint16_t num_rom_banks_mask[] =
	{
		2, 4, 8, 16, 32, 64, 128, 256, 512
	};
	const uint8_t num_ram_banks[] = { 0, 1, 1, 4, 16, 8 };

	gb->gb_rom_read = gb_rom_read;
	gb->gb_cart_ram_read = gb_cart_ram_read;
	gb->gb_cart_ram_write = gb_cart_ram_write;
	gb->gb_error = gb_error;
	gb->direct.priv = priv;

	/* Initialise serial transfer function to NULL. If the front-end does
	 * not provide serial support, Peanut-GB will emulate no cable connected
	 * automatically. */
	gb->gb_serial_tx = NULL;
	gb->gb_serial_rx = NULL;

	/* Check valid ROM using checksum value. */
	{
		uint8_t x = 0;

		for(uint16_t i = 0x0134; i <= 0x014C; i++)
			x = x - gb->gb_rom_read(gb, i) - 1;

		if(x != gb->gb_rom_read(gb, ROM_HEADER_CHECKSUM_LOC))
			return GB_INIT_INVALID_CHECKSUM;
	}

	/* Check if cartridge type is supported, and set MBC type. */
	{
		const uint8_t mbc_value = gb->gb_rom_read(gb, mbc_location);

		if(mbc_value > sizeof(cart_mbc) - 1 ||
				(gb->mbc = cart_mbc[mbc_value]) == 255u)
			return GB_INIT_CARTRIDGE_UNSUPPORTED;
	}

	gb->cart_ram = cart_ram[gb->gb_rom_read(gb, mbc_location)];
	gb->num_rom_banks_mask = num_rom_banks_mask[gb->gb_rom_read(gb, bank_count_location)] - 1;
	gb->num_ram_banks = num_ram_banks[gb->gb_rom_read(gb, ram_size_location)];

	gb->display.lcd_draw_line = NULL;

	gb_reset(gb);

	return GB_INIT_NO_ERROR;
}

/**
 * Returns the title of ROM.
 *
 * \param gb		Initialised context.
 * \param title_str	Allocated string at least 16 characters.
 * \returns		Pointer to start of string, null terminated.
 */
const char* gb_get_rom_name(struct gb_s* gb, char *title_str)
{
	uint_fast16_t title_loc = 0x134;
	/* End of title may be 0x13E for newer games. */
	const uint_fast16_t title_end = 0x143;
	const char* title_start = title_str;

	for(; title_loc <= title_end; title_loc++)
	{
		const char title_char = gb->gb_rom_read(gb, title_loc);

		if(title_char >= ' ' && title_char <= '_')
		{
			*title_str = title_char;
			title_str++;
		}
		else
			break;
	}

	*title_str = '\0';
	return title_start;
}

#if ENABLE_LCD
void gb_init_lcd(struct gb_s *gb,
		void (*lcd_draw_line)(struct gb_s *gb,
			const uint8_t *pixels,
			const uint_fast8_t line))
{
	gb->display.lcd_draw_line = lcd_draw_line;

	gb->direct.interlace = 0;
	gb->display.interlace_count = 0;
	gb->direct.frame_skip = 0;
	gb->display.frame_skip_count = 0;

	gb->display.window_clear = 0;
	gb->display.WY = 0;

	return;
}
#endif
