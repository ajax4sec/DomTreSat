#include<stdio.h>
#include<string.h>
#include<math.h>

int main(int argc, char* argv[]){
	printf("HELLO WORLD\n");
	char *b = argv[0];
	char f = (b[0]+'B');
	if(f==0x70){
		char l = (b[1]+'E');
		if(l==0x7d){
			char a = (b[2]+'E');
			if(a==0x72){
				char g = (b[3]+'P');
				if(g==0x74){
					//target code block
					printf("it was just..\n");
					return 1;
				}
			}
		}
	
	}
	return 0;
}
