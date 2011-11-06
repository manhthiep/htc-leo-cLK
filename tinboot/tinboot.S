@ author: cedesmith
@ license: GPL
@ version 1.0
@ tiny bootloader wrapper for HTC WM6 phones
@ inspired by Martin Johnson Tinboot for htc vogue

.equ	KERNEL_OFFSET,  0x8000
		.org 0
		b boot

		.org 0x40
		.word 0x43454345
		.word romhdr+0x80000000				@ location of wince romhdr 
		.word romhdr

		
		.org 0x00001000

boot:
		b kernel

romhdr:
		.word 0x0		@ dllfirst
		.word 0x0		@ dlllast
		.word 0x80000000	@ physfirst
		.word 0x80000000+fin	@ physlast
		.word 0			@ nummods
		.word 0x80000000+fin	@ ulRAMStart
		.word 0x80000000+fin	@ ulRAMFree
		.word 0x8E600000	@ ulRAMEnd
		.word 0			@ ulCopyEntries
		.word 0x80000000+fin	@ ulCopyOffset
		.word 0			@ ulProfileLen
		.word 0			@ ulProfileOffset
		.word 0			@ numfiles
		.word 0			@ ulKernelFlags
		.word 0x04		@ ulFSRamPercent
		.word 0			@ ulDrivglobStart
		.word 0			@ ulDrivglobLen
		.word 0x201c2		@ usCPUType and usMiscFlags (2 shorts)
		.word 0x80000000	@ pExtensions
		.word 0			@ ulTrackingStart
		.word 0			@ ulTrackingLen

		.org KERNEL_OFFSET
kernel:
		.incbin "../lk/build-htcleo/lk.bin"

fin:
