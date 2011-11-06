@ author: cedesmith
@ license: GPL
@ version 1.0
@ tiny bootloader wrapper for HTC WM6 phones
@ inspired by Martin Johnson Tinboot for htc vogue

		.org 0
		b boot

		.org 0x40
		.word 0x43454345
		.word romhdr+0x80000000				@ location of wince romhdr 
		.word romhdr


		.org 0x00000900
		romhdr: @ 23E6F0 in original rom file
		.word 0x0		@ dllfirst					FD 01 F9 01
		.word 0x0		@ dlllast					00 00 00 02
		.word 0x80000000	@ physfirst				00 00 00 80
		.word 0x80000000+fin+0x100 @ physlast		48 B4 4D 80
		.word 0			@ nummods					29 00 00 00
		.word 0x80000000+fin+0x200 @ ulRAMStart		00 C0 4D 80
		.word 0x80000000+fin+0x300 @ ulRAMFree		00 B0 5C 80
		.word 0x8E600000	@ ulRAMEnd				00 00 60 8E
		.word 0			@ ulCopyEntries				07 00 00 00
		.word 0x80000000+fin+0x400 @ ulCopyOffset	DC 24 24 80
		.word 0			@ ulProfileLen				00 00 00 00
		.word 0			@ ulProfileOffset			00 00 00 00
		.word 0			@ numfiles					0E 00 00 00
		.word 0			@ ulKernelFlags				02 00 00 00
		.word 0x04		@ ulFSRamPercent			04 00 00 00
		.word 0			@ ulDrivglobStart			00 00 00 00
		.word 0			@ ulDrivglobLen				00 00 00 00
		.word 0x201c2		@ usCPUType and usMiscFlags (2 shorts)
		.word 0x80000000+fin+0x500 @ pExtensions	EC 2A 00 80
		.word 0			@ ulTrackingStart			00 00 00 00
		.word 0			@ ulTrackingLen				00 00 00 00

		.org 0x00001000
boot:
		.incbin "lk.bin"

fin:
