
#ifndef SIMON32_64
#define SIMON32_64
//#include <jni.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
//#include <cpu-features.h>
#include <stdint.h>



 extern void SIMON32_64_NEON_ARM(uint16_t key[4 * 32], uint16_t text[4 * 12]);

#endif
