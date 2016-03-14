#include<stdio.h>
#include<string.h>
#include<math.h>

int main(int argc, char* argv[]){
	printf("HELLO WORLD\n");
	char *b = argv[0];

	// move variables away from their definitions
	char f = b[0];
	char l = b[1];
	char a = b[2];
	char g = b[3];
	
	// check variables for satisfiable conditions
	if(f==0x45){
		if(l==0x44){
			if(a==0x43){
				if(g==0x42){
					//target code block
					printf("it was just..\n");
					return 1;
				}
			}
		}
	
	}
	return 0;
}
