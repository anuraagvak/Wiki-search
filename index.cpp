#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef unsigned long long int UINT_64;
typedef unsigned char UINT_8;

#define max_size 20971520
UINT_8 buff[max_size];
UINT_64 maxy = max_size;

FILE *main_index = fopen("/home/dilipvamsi/mainindex.txt", "r");
UINT_64 file_pos =  0;

UINT_64 uncopressit(UINT_64 *curr){
	UINT_64 ret = 0;
	UINT_64 iter = *curr,pos = 0;
	do {
		if (iter == maxy) {
			maxy = fread(buff,1,max_size,main_index);
			*curr = 0,iter = 0;
			if (maxy == 0) return 0;
		}   
		UINT_64 pu = (buff[iter] >> 1); 
		ret += pu << pos;
		pos += 7;
	//	file_pos++;
	}while(buff[iter++]&1);
	*curr = iter;
	return ret;
}

int main(int argc, const char *argv[])
{
	UINT_8 sz = sizeof(UINT_64);
	FILE *word_index = fopen("/home/dilipvamsi/wordindex.txt", "wb");
	UINT_64 pos = 0;
	//UINT_8 check = 10;
	maxy = fread(buff,1,max_size,main_index);
	while (maxy) { // && check--){
		UINT_64 word = uncopressit(&pos);
		printf("word = %llu pos = %llu\n", word, file_pos);
		fwrite(&word, sz, 1, word_index);
		file_pos=ftell(main_index);
		fwrite(&file_pos, sz, 1, word_index);
		while(uncopressit(&pos)){}
	}
	fclose(word_index);
	return 0;
}
