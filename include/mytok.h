#ifndef __MYTOK_H
#define __MYTOK_H 1

typedef unsigned int UINT_32;
char * mytok(char * str, UINT_32 * pos) {
	UINT_32 i;
	for (i = *pos;(str[i] < 48 || (str[i] > 57 && str[i] < 65) || (str[i] > 90 && str[i] < 97) || str[i] > 122) && str[i] != '\0'; i++) {}
	if (str[i] == '\0') return NULL;
	char *ret = &str[i];
	for (; (str[i] > 96 && str[i] < 123) || (str[i] > 64 && str[i] < 91) || (str[i] > 47 && str[i] < 58); i++) {}
	str[i] = '\0';
	*pos = i+1;
	return ret;
}

#endif
