#ifndef SIMON128_128
#define SIMON128_128
//#include <jni.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
//#include <cpu-features.h>
#include <stdint.h>



 extern void SIMON128_128_NEON_ARM(uint64_t key[2 * 68], uint64_t text[2 * 6]);

#endif
