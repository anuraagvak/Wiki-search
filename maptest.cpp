#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <map>
using namespace std;
typedef unsigned long long int UINT_64;
typedef unsigned char UINT_8;
#define max_size 1024

map <UINT_64, UINT_64> WordPos;

UINT_8 buff[max_size];
int main_index = open("/home/dilipvamsi/mainindex.txt", O_RDONLY|O_LARGEFILE);
size_t maxy = max_size;

UINT_64 uncopressit(UINT_64 *curr){
	UINT_64 ret = 0;
	UINT_64 iter = *curr,pos = 0;
	do {
		if (iter == maxy) {
			maxy = read(main_index,buff,max_size);
			*curr = 0,iter = 0;
			if (maxy == 0) return 0;
		}   
		UINT_64 pu = (buff[iter] >> 1); 
		ret += pu << pos;
		pos += 7;
	}while(buff[iter++]&1);
	*curr = iter;
	return ret;
}

int main(int argc, const char *argv[])
{
	FILE *fp = fopen("/home/dilipvamsi/wordindex.txt","rb");
	unsigned long long int pos, count = 10;
	if (argc > 1) count = strtoull(argv[1], NULL, 10);
	while(fread(&pos, 8, 1, fp)) {
		lseek(main_index, pos, SEEK_SET);
		read(main_index, buff, max_size);
		UINT_64 curr = 0;
		WordPos[uncopressit(&curr)] = pos;
	}
	puts("hi");
	return 0;
}
