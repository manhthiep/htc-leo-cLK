
//cedesmith: from kernel board-htcleo.
#define MSM_EBI1_BANK0_BASE     0x11800000
//#define MSM_EBI1_BANK0_SIZE     0x1E800000 /* 488MB */
//CONFIG_USING_BRAVOS_DSP
//#define MSM_EBI1_BANK0_SIZE     0x1CFC0000 /* 488MB - DESIRE DSP - 0x00040000 RAM CONSOLE*/
#define MSM_EBI1_BANK0_SIZE     0x1E7C0000 /* 488MB - 0x00040000 RAM CONSOLE*/

unsigned* target_atag_mem(unsigned* ptr)
{
	*ptr++ = 4;
	*ptr++ = 0x54410002;
	*ptr++ = MSM_EBI1_BANK0_SIZE;
	*ptr++ = MSM_EBI1_BANK0_BASE;

	return ptr;
}
