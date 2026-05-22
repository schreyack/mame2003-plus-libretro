/*
 * namco_sound_enable_patch.c
 *
 * Pac-Man-family games (Pac-Man, Ms. Pac-Man, Rally-X, Mappy, Pengo, etc.)
 * produce silent audio in mame2003-plus and current upstream MAME alike.
 * The Z80 boot routine writes 0 once to the LS259 latch at 0x5001, which
 * drives pengo_sound_enable_w(0) and gates the Namco WSG chip output to
 * silence. The boot code never writes a 1 back to re-enable, so audio
 * stays muted forever. Real arcade hardware produces audio, so the LS259
 * Q1 line is either not actually the sound gate or initializes high on
 * real hardware.
 *
 * Rather than modify the vendored namco.c, intercept via the linker:
 * the Makefile passes -Wl,--wrap=pengo_sound_enable_w which redirects all
 * callers to __wrap_pengo_sound_enable_w. The wrapper forwards every
 * value except a zero write, keeping sound_enable at its initial 1.
 */

#include "driver.h"

extern WRITE_HANDLER( __real_pengo_sound_enable_w );

WRITE_HANDLER( __wrap_pengo_sound_enable_w )
{
	if (data != 0)
		__real_pengo_sound_enable_w(offset, data);
}
