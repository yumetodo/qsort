#ifndef INC_MM88_H_
#define INC_MM88_H_
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

extern size_t g_QS_MVR;

void mmswap(char *a, char *b);
void mmrot3( char *a, char *b, char *c );
void mmswapblock( char *a, char *b, size_t size );
void mmprepare( void *base, size_t size );
void mmmove(char *a, const char *b);

#ifdef __cplusplus
}
#endif

#endif //INC_MM88_H_
