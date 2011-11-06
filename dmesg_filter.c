#include <stdio.h>
#include <string.h>

int main() {
	char buff[0x40000];

	char* ptr = buff;
	while(!feof(stdin) && fgetc(stdin)!='\n');

	while(!feof(stdin))
	{
		*ptr++=fgetc(stdin);*ptr=0;

		if(ptr-buff>=5 && memcmp(ptr-5,"\nINFO",5)==0)
		{
			ptr[-5]=0;
			if(ptr-buff>=6 && ptr[-6]=='\r' ) ptr[-6]=0;
			printf("%s", buff);
			ptr=buff;

		}
	}
	printf("%s", buff);
}
