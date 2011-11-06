
//cedesmith: from kernel board-htcleo.
#define MSM_EBI1_BANK0_BASE     0x11800000
//#define MSM_EBI1_BANK0_SIZE     0x1E800000 /* 488MB */
//CONFIG_USING_BRAVOS_DSP
//#define MSM_EBI1_BANK0_SIZE     0x1CFC0000 /* 488MB - DESIRE DSP - 0x00040000 RAM CONSOLE*/
#define MSM_EBI1_BANK0_SIZE     0x1E7C0000 /* 488MB - 0x00040000 RAM CONSOLE*/

unsigned* target_atag_mem(unsigned* ptr)
{

	//MEM TAG
	*ptr++ = 4;
	*ptr++ = 0x54410002;
	//*ptr++ = 0x1e400000; //mem size from haret
	//*ptr++ = 0x1E7C0000; //mem size from kernel config
	*ptr++ = 0x1CFC0000; //mem size from kernel config with bravo dsp
	*ptr++ = 0x11800000; //mem base


	//add atag to notify nand boot
	*ptr++ = 4;
	*ptr++ = 0x4C47414D;
	*ptr++ = 0;
	*ptr++ = 0;

	return ptr;
}
