#ifndef SPECK128_256
#define SPECK128_256
//#include <jni.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
//#include <cpu-features.h>
#include <stdint.h>



 extern void SPECK128_192_NEON_ARM(uint64_t key[2 * 34], uint64_t text[2 * 8]);

#endif
