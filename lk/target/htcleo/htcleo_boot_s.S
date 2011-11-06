.text		
.code 32

.globl htcleo_boot_s
htcleo_boot_s:
		// save registers
		MOV r9, r0

		//cedesmith: cotulla's code so kernel will not crash. aux control register
		MRC p15, 0, r0, c1, c0, 1
		BIC r0, r0, #0x40					// (1<<6)  Instruction cache reload on a parity error disabled
		BIC r0, r0, #0x200000				// (1<<21) undocumented bit ?
		MCR p15, 0, r0, c1, c0, 1

		//flashlight to see we get here
		//ldr     r4, =0xa9000864        @ bank6_in (phys)
		//ldr     r5, =0xa9000814        @ bank6_out (phys)
		//orr     r6, r4, #0x200000      @ 22nd bit for flash
		//str     r6, [r5, #0]           @ store in out (enables bright for 500ms, limited by hardware)

		MOV R0, #0
        BLX R9

.ltorg
