#include<stdio.h>
typedef unsigned long long int UINT_64;
typedef unsigned char UINT_8;
//#define max_size  104857600
//#define max_size  100

UINT_64 compressit(UINT_64 val, UINT_8 * str, UINT_64 pos) {
	while(true) {
		str[pos] = (val&127)<<1;
//		fprintf(stderr,"%d ", str[pos]);
		if (val >> 7) {
			val = val >> 7;
			str[pos++] += 1;
	//		fprintf(stderr,"%d ", str[pos]);
		} else {
			pos++;
	//		fprintf(stderr,"%d\n", str[pos]);
			break;
		}
	}
	return pos;
}

UINT_64 uncopressit(UINT_8 * val, UINT_64 *curr, UINT_64 maxy){
	UINT_64 ret = 0;
	UINT_8 iter = *curr,pos = 0;
	do {
		if (iter == maxy) return 0;
		ret += (val[iter] >> 1) << pos;
		pos += 7;
		
	} while(val[iter++]&1);
	*curr = iter;
	printf("\n%llu ", ret);
	return ret;
}
