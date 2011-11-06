/**
	author: cedesmith
	license: GPL
	small utility to fix nb by removing unneeded partitions
*/
#include <stdio.h>
#include <unistd.h>

int main()
{
	char empty[0x200] = {0} ;
	
	FILE *f = fopen("os.nb.payload", "r+");
	if(f==NULL) 
	{
		printf("Failed to open RUU_signed.nbh");
		return 1;
	}
	
	//change from BOOT to XIP
	fseek(f, 0x1C2, SEEK_SET);
	//fwrite( "\0x23", 1, 1, f);
	char xipType = 0x23;
	fwrite( &xipType, 1, 1, f);
	
	//remove other partitions
	fseek(f, 0x1CE, SEEK_SET);
	fwrite( empty, 1, 0x30, f);
	
	//clean MSFLSH50 header
	fseek(f, 0x808, SEEK_SET);
	fwrite( empty, 1, 0x5C, f);
	
	fclose(f);
	
	truncate("os.nb.payload", 0x40000);
	
	
}
