/*
 * Copyright (c) 2009, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of Google, Inc. nor the names of its contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <dev/keys.h>
#include <dev/gpio_keypad.h>

#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))


/* Keypad */
#define HTCLEO_GPIO_KP_MKOUT0    		33
#define HTCLEO_GPIO_KP_MKOUT1    		32
#define HTCLEO_GPIO_KP_MKOUT2    		31
#define HTCLEO_GPIO_KP_MPIN0     		42
#define HTCLEO_GPIO_KP_MPIN1     		41
#define HTCLEO_GPIO_KP_MPIN2     		40 

/*
 * cedesmith
 * NOTE: htcleo kernel differs by havin row swaped to col
 */
static unsigned int halibut_row_gpios[] = { 33, 32, 31 };
static unsigned int halibut_col_gpios[] = { 42, 41, 40 };

#define KEYMAP_INDEX(row, col) ((row)*ARRAY_SIZE(halibut_col_gpios) + (col))

static const unsigned short halibut_keymap[ARRAY_SIZE(halibut_col_gpios) * ARRAY_SIZE(halibut_row_gpios)] = {
	[KEYMAP_INDEX(0, 0)] = KEY_VOLUMEUP,	// Volume Up
	[KEYMAP_INDEX(0, 1)] = KEY_VOLUMEDOWN,	// Volume Down
	[KEYMAP_INDEX(1, 0)] = KEY_SOFT1,	// Windows Button
	[KEYMAP_INDEX(1, 1)] = KEY_SEND,	// Dial Button
	[KEYMAP_INDEX(1, 2)] = KEY_CLEAR,	// Hangup Button
	[KEYMAP_INDEX(2, 0)] = KEY_BACK,	// Back Button
	[KEYMAP_INDEX(2, 1)] = KEY_HOME,	// Home Button 
};

static struct gpio_keypad_info halibut_keypad_info = {
	.keymap		= halibut_keymap,
	.output_gpios	= halibut_row_gpios,
	.input_gpios	= halibut_col_gpios,
	.noutputs	= ARRAY_SIZE(halibut_row_gpios),
	.ninputs	= ARRAY_SIZE(halibut_col_gpios),
	.settle_time	= 40 /* msec */,
	.poll_time	= 20 /* msec */,
	.flags		= GPIOKPF_DRIVE_INACTIVE,
};

void keypad_init(void)
{
	gpio_keypad_init(&halibut_keypad_info);
}
