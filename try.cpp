#include <stdio.h>
int main () {
	FILE *fp = fopen("./Index.txt","r");
	int val = 100;
	while(val--){
		unsigned char ch = fgetc(fp);
		printf("%c", ch);
	}
	puts("");
}
