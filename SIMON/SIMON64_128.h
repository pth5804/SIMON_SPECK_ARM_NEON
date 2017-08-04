#ifndef SIMON64_128
#define SIMON64_128
//#include <jni.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
//#include <cpu-features.h>
#include <stdint.h>



 extern void SIMON64_128_NEON_ARM(uint32_t key[2 * 44], uint32_t text[2 * 12]);

#endif
