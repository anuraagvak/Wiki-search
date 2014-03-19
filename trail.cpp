#include <stdio.h>
#include <stdlib.h>
typedef unsigned long long int UINT_64;
typedef unsigned char UINT_8;
#define max_size 1048576

FILE *fp;
int fill = 0;
unsigned char buff[max_size];
unsigned int pos = 0;
UINT_64 maxy = max_size;
UINT_64 uncopressit(UINT_64 *curr){
	UINT_64 ret = 0;
	UINT_64 iter = *curr,pos = 0;
//	if (fill == 6 &&(*curr == 736505 || *curr == 736496)) fputs("hello\n",stderr);
	do {
		if (iter == maxy) {
			fill++;
			maxy = fread(buff,1,max_size,fp);
			*curr = 0,iter = 0;
			if (maxy == 0) return 0;
		} 
//		printf("i am = %llu\n",(buff[iter] >> 1) << pos);
		UINT_64 pu = (buff[iter] >> 1);
		ret += pu << pos;
		//fprintf(stdout,"val = %llu\n", ret);
		pos += 7;

	}while(buff[iter++]&1);
	*curr = iter;
	//if (fill == 6 &&(*curr == 736505 || *curr == 736496)) 
	//fprintf(stdout,"val = %llu\n", ret);
	//printf("%llu\n", *curr);
	return ret;
}

/*
UINT_64 uncopressit(UINT_64 *curr){
	UINT_64 ret = 0;
	UINT_8 iter = *curr,pos = 0;
	do {
		if (iter == maxy) {
			maxy = fread(buff,1,max_size,fp);
			*curr = 0; iter = 0;
			//if (maxy == 0) return 0;
			puts("hello");
		}
		ret += (buff[iter] >> 1) << pos;
	//	printf("buff %d = %d , ret = %llu ", iter, buff[iter],ret);
		pos += 7;

	} while(buff[iter++]&1);
	*curr = iter;
//	printf("\n%llu ", ret);
	return ret;
}
*/
int main (int argc, const char** argv) {
	fp = fopen(argv[1], "r");
	bool checkit = true;
	if (argc == 3) checkit = false;
	UINT_64 pos = 0, val;
			maxy = fread(buff,1,max_size,fp);
//	do {
	while(maxy) {
			val = uncopressit(&pos);
			if (checkit) printf("%llu$",val);	
			else printf("%llx$",val);	
		while(true) {
			if (!val) break;
			val = uncopressit(&pos);
			if (checkit) printf("%llu ",val);	
			else printf("%llx ",val);	
		} puts("");
//		exit(0);
	}
//	} while (pos);
}
