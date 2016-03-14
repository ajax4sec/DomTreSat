#include<stdio.h>
#include<string.h>
#include<math.h>

int main(int argc, char* argv[]){
	printf("HELLO WORLD\n");
	char *b = argv[0];
	char f = (b[0]-'A');
	if(f==0x20){
		char l = (b[1]-'B');
		if(l==0x20){
			char a = (b[2]-'C');
			if(a==0x20){
				char g = (b[3]-'D');
				if(g==0x20){
					//target code block
					printf("it was just..\n");
					return 1;
				}
			}
		}
	
	}
	return 0;
}
