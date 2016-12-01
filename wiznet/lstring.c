/////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2001-2002 Hybus Co,.ltd. All rights reserved.
//
// Module name:
//      lstring.c
//
// Description:
//
//
// Author:
//      bedguy
//
// Created:
//      2002.10
//
///////////////////////////////////////////////////////////////////////////////

#include "lstring.h"

void MemCpy32(void *dest, void *src, int numWords){
	volatile long *s1 = (volatile long *)dest, *s2 = (volatile long *)src;
	while(numWords--) {
		*s1++ = *s2++;
	}
	return;
}	// MemCpy32.


void MemCpy(void *dest, void *src, int cnt){
	char *s1 = (char *)dest, *s2 = (char *)src, *endptr = (char *)dest+cnt;
	
	while (s1<endptr)
		*s1++ = *s2++;
	return;
}	// MemCpy.


void MemSet(void *dest, char c, int len){
	char *s = (char *)dest;
	char *limit = (char *)dest+len;

	while (s < limit) *s++ = c;
}	// MemSet.

void StrCpy(char *dest, char *src){
	volatile char *s1=dest, *s2=src;
	
	while (*s2!=0) *s1++ = *s2++;
	*s1 = 0;
	return;
}	// StrCpy.


int StrLen(char *dest){
	volatile char *tmp = dest;

	if (!tmp) return -1;
	while (*tmp!=0) tmp++;
	return (tmp - dest);
}	// StrLen.


int StrNCmp(char *s1, char *s2, int len){
	int i;

	for(i = 0; i < len; i++){
		if(s1[i] != s2[i])	return ((int)s1[i]) - ((int)s2[i]);
		if(s1[i] == 0)		return 0;
	}
	return 0;
} // StrNCmp.


int StrCmp(char *s1, char *s2){
	for (; *s1 && *s2; s1++, s2++){
		if (*s1 != *s2) return ((int)(*s1) - (int)(*s2));
	}
	if (*s1 || *s2) return ((int)(*s1) - (int)(*s2));
	return 0;
}	// StrCmp.


int DecToLong(char *s, long *retval)
{
	long remainder;
	if (!s || !s[0]) return false;

	for (*retval=0; *s; s++)
	{
		if (*s < '0' || *s > '9') return false;
		remainder = *s - '0';
		*retval = *retval * 10 + remainder;
	}

	return true;
}	// DecToLong.

