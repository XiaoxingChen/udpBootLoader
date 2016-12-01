#ifndef _FW_LSTRING_H_
#define _FW_LSTRING_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
typedef int				bool;
#define	true			1
#define false			0
#endif

void	MemCpy32(void *dest, void *src, int numWords);
void	HexDump(char *addr, int len);

int		StrCmp(char *s1, char *s2);
int		StrNCmp(char *s1, char *s2, int len);
void	MemCpy(void *dest, void *src, int len);
void	MemSet(void *dest, const char c, int len);
int		MemCmp(void *addr1, void *addr2, int len);
void	StrCpy(char *dest, char *src);
int		StrLen(char *str);

int		DecToLong(char *s, long *retval);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif		// end _FW_LSTRING_H_.
