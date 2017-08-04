#ifndef SIMON64_96
#define SIMON64_96
//#include <jni.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
//#include <cpu-features.h>
#include <stdint.h>



 extern void SIMON64_96_NEON_ARM(uint32_t key[2 * 42], uint32_t text[2 * 12]);

#endif
