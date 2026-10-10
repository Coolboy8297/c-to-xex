/*
 * Minimal Xbox 360 homebrew "Hello World"
 * Built with libxenon → produces .elf32 (and optionally .xex)
 *
 * Requires:
 *   - DEVKITXENON environment variable set
 *   - libxenon installed
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <console/console.h>
#include <input/input.h>
#include <usb/usbmain.h>
#include <xenon_soc/xenon_power.h>
#include <xenos/xenos.h>

int main(void)
{
	/* Initialise GPU (auto-detect video mode from cable / region) */
	xenos_init(VIDEO_MODE_AUTO);

	/* Text console on the framebuffer */
	console_init();

	/* Run CPU at full 3.2 GHz */
	xenon_make_it_faster(XENON_SPEED_FULL);

	/* USB + controllers */
	usb_init();
	usb_do_poll();

	/* Clear the noisy init text */
	console_clrscr();

	printf("Hello from C -> XEX (libxenon)!\n");
	printf("Xbox 360 homebrew template\n\n");
	printf("Press Guide or Y to exit.\n");

	struct controller_data_s oldc;
	memset(&oldc, 0, sizeof(oldc));

	while (1) {
		struct controller_data_s c;
		if (get_controller_data(&c, 0)) {
			/* Exit on Guide or Y */
			if ((c.guide && !oldc.guide) || (c.y && !oldc.y)) {
				break;
			}
			oldc = c;
		}
		usb_do_poll();
	}

	return 0;
}
