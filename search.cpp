#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "./include/hashword.h"
#include "./include/porter.h"
typedef unsigned long long int UINT_64;
typedef unsigned int UINT_32;
typedef unsigned char UINT_8;
#define max_size 104858
#define doc_end 699392
#define word_end 18392417

UINT_8 buff[max_size];
int main_index = open("/home/dilipvamsi/mainindex.txt.bak", O_RDONLY|O_LARGEFILE);
FILE *word_index = fopen("/home/dilipvamsi/wordindex.txt.bak", "rb");
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
FILE *doc_index = fopen("/home/dilipvamsi/docindex.txt.bak", "rb");
FILE *title_index = fopen("/home/dilipvamsi/titleindex.txt.bak", "r");

void PrintTitle(UINT_32 start, UINT_32 end, UINT_32 docID) {
	UINT_32 mid = (start + end)/2;
	fseek(doc_index, mid*8, SEEK_SET);
	UINT_32 DocPos[2];
	fread(DocPos, 4, 2, doc_index);
	printf("start = %llu end = %llu midWord = %llu pos = %llu\n", start, end, DocPos[0], DocPos[1]);
	if (start == end) {
		if (docID == DocPos[0]) {
			fseek(title_index, DocPos[1], SEEK_SET);
			char tit[1000];
			fscanf(title_index,"%[^\n]",tit);
			puts(tit);
		} 
		else {
			puts("not found");
		}
	} 
	else if (docID > DocPos[0]) return PrintTitle(mid+1,end,docID);
	else if (docID < DocPos[0]) return PrintTitle(start, mid-1,docID);
	else {
		fseek(title_index, DocPos[1], SEEK_SET);
		char tit[1000];
		fscanf(title_index,"%[^\n]",tit);
		puts(tit);
	}
}

const UINT_8 sz = sizeof(UINT_64);
long long int B_search_word(UINT_64 start, UINT_64 end, UINT_64 word) {
	UINT_64 mid = (start + end)/2;
	fseek(word_index, mid*16, SEEK_SET);
	UINT_64 WordPos[2];
	fread(WordPos, sz, 2, word_index);
	//printf("start = %llu end = %llu midWord = %llu pos = %llu\n", start, end, WordPos[0], WordPos[1]);
	if (start == end) {
		if (word == WordPos[0]) {
			puts("done");
			UINT_64 pos = 0;
			lseek(main_index, WordPos[1], SEEK_SET);
			read(main_index, buff, max_size);
			//uncopressit(&pos);// >> 5;
			for (int i = 0; i < 5; i++) {
			UINT_64 id = uncopressit(&pos) >> 5;
			printf("id = %llu\n", id);
			PrintTitle(0, doc_end, (UINT_32) id);
			}
			return word;
		} 
		else {
			puts("not found");
			return -1;
		}
	} 
	else if (word > WordPos[0]) {
		return B_search_word(mid+1, end, word); 
	} 
	else if (word < WordPos[0]) {
		return B_search_word(start, mid-1,word);
	} 
	else {
		puts("done");
		return word;
	}
}

int main () {
	char s[1000];
	while(scanf("%s", s) != EOF) {
		UINT_64 word =  HashMapWord(stemword(s));
		printf("stemword = %llu\n", word);
		B_search_word(0, word_end, word);
	}
	return 0;
}
