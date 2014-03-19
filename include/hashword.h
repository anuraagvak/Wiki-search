#include <stdio.h>    
#include <stdlib.h>
#include <string.h>
#include <unordered_map>

#ifndef __MYHASHMAP_H
#define __MYHASHMAP_H 1
typedef unsigned long long int UINT_64;
typedef unsigned char UINT_8;

std::unordered_map<char, UINT_8> hashval = {{'a', 1},{'b', 2},{'c', 3},
	{'d', 4},{'e', 5},{'f', 6},{'g', 7},{'h', 8},{'i', 9},{'j', 10},
	{'k', 11},{'l', 12},{'m', 13},{'n', 14},{'o', 15},{'p', 16},{'q', 17},
	{'r', 18},{'s', 19},{'t', 20},{'u', 21},{'v', 22},{'w', 23},{'x', 24},
	{'y', 25},{'z', 26},{'0', 27},{'1', 28},{'2', 29},{'3', 30},{'4', 31},
	{'5', 32},{'6', 33},{'7', 34},{'8', 35},{'9', 36}};

UINT_64 HashMapWord(const char *word) {
	UINT_64 ret64=0, p = 1;
	for (int i = 0; word[i] != '\0'; i++) {
		ret64 += hashval[word[i]] * p;
		p *= 37;
	}
//	printf("%lu ", strlen(word));
	return ret64;
}

UINT_64 HashMapWordRev(const char *word) {
	UINT_64 ret64=0, p = 1;
	for (int i = strlen(word)-1; i > -1; i--) {
		ret64 += hashval[word[i]] * p;
		p *= 37;
	}
//	printf("%lu ", strlen(word));
	return ret64;
}

char revhashval[40] = "-abcdefghijklmnopqrstuvwxyz0123456789";

void revPrint(UINT_64 hval) {
	UINT_64 val, p = 37;
	while (hval) {
		val = hval % p;
		printf("%c", revhashval[val]);
		hval /= p;
	}
	puts("");
}

#endif
