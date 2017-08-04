
#include "SPECK64_96.h"
//#include <jni.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
//#include <cpu-features.h>
#include <arm_neon.h>


void  SPECK64_96_NEON_ARM(uint32_t key[2 * 26], uint32_t text[2 * 14]){ 

	asm(

			"push         {r4-r10}                 	\n\t"
			"vpush        {q4-q7}                 	\n\t"

			//plain-text load
			//12 plain-text
			//"vldmia %1!, {q0-q7} 					\n\t"
			//"vldmia %1!, {q8-q11} 					\n\t"
			//"ldmia %1!, {r6-r9} 					\n\t"

			//"vld4.16 {d0,d1,d2,d3},  [r2]!		\n\t"
			//"vld4.16 {d4,d5,d6,d7},  [r2]!		\n\t"
			//"vld2.16 {d8,d9},  [r2]!		\n\t"

			"vldmia %1!,{d0-d13}	\n\t"

			//initialization
			"sub %1,%1,#112	\n\t" // #112 = 16*7(Q register * 7)

			//pre-processing
			"vtrn.32 d0, d1	\n\t"
			"vtrn.32 d2, d3	\n\t"
			"vtrn.32 d4, d5	\n\t"
			"vtrn.32 d6, d7	\n\t"
			"vtrn.32 d8, d9	\n\t"
			"vtrn.32 d10, d11	\n\t"
			"vtrn.32 d12, d13	\n\t"		

			"vtrn.32 d0, d0	\n\t"
			"vtrn.32 d1, d1	\n\t"
			"vtrn.32 d2, d2	\n\t"
			"vtrn.32 d3, d3	\n\t"
			"vtrn.32 d4, d4	\n\t"
			"vtrn.32 d5, d5	\n\t"
			"vtrn.32 d6, d6	\n\t"
			"vtrn.32 d7, d7	\n\t"
			"vtrn.32 d8, d8	\n\t"
			"vtrn.32 d9, d9	\n\t"
			"vtrn.32 d10, d10	\n\t"
			"vtrn.32 d11, d11	\n\t"
			"vtrn.32 d12, d12	\n\t"
			"vtrn.32 d13, d13	\n\t"

			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"



			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"


			//Round#1
			//ROR#8(x)
			"vshl.u32 d14,d1,#24	\n\t"
			"vshl.u32 d15,d3,#24	\n\t"
			"vshl.u32 d16,d5,#24	\n\t"
			"vshl.u32 d17,d7,#24	\n\t"
			"vshl.u32 d18,d9,#24	\n\t"
			"vshl.u32 d19,d11,#24	\n\t"
			"vshl.u32 d20,d13,#24	\n\t"
	
			"vsri.u32 d14,d1,#8	\n\t"
			"vsri.u32 d15,d3,#8	\n\t"
			"vsri.u32 d16,d5,#8	\n\t"
			"vsri.u32 d17,d7,#8	\n\t"
			"vsri.u32 d18,d9,#8	\n\t"
			"vsri.u32 d19,d11,#8	\n\t"
			"vsri.u32 d20,d13,#8	\n\t"

			// ROR#7(x) + y
			"vadd.i32 d14,d14,d0	\n\t"
			"vadd.i32 d15,d15,d2	\n\t"
			"vadd.i32 d16,d16,d4	\n\t"
			"vadd.i32 d17,d17,d6	\n\t"
			"vadd.i32 d18,d18,d8	\n\t"
			"vadd.i32 d19,d19,d10	\n\t"
			"vadd.i32 d20,d20,d12	\n\t"

			// Load Roundkey
			"vldmia %0!,{d28}	\n\t"

			//(ROR#7(x) + y) ^ Roundkey
			"veor d1, d14, d28	\n\t"
			"veor d3, d15, d28	\n\t"
			"veor d5, d16, d28	\n\t"
			"veor d7, d17, d28	\n\t"
			"veor d9, d18, d28	\n\t"
			"veor d11, d19, d28	\n\t"
			"veor d13, d20, d28	\n\t"

			//ROL_beta(3)(y)
			"vshr.u32 d21,d0,#29	\n\t"
			"vshr.u32 d22,d2,#29	\n\t"
			"vshr.u32 d23,d4,#29	\n\t"
			"vshr.u32 d24,d6,#29	\n\t"
			"vshr.u32 d25,d8,#29	\n\t"
			"vshr.u32 d26,d10,#29	\n\t"
			"vshr.u32 d27,d12,#29	\n\t"
	
			"vsli.u32 d21,d0,#3	\n\t"
			"vsli.u32 d22,d2,#3	\n\t"
			"vsli.u32 d23,d4,#3	\n\t"
			"vsli.u32 d24,d6,#3	\n\t"
			"vsli.u32 d25,d8,#3	\n\t"
			"vsli.u32 d26,d10,#3	\n\t"
			"vsli.u32 d27,d12,#3	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"




///*
			//post-processing
			"vtrn.32 d0, d0	\n\t"
			"vtrn.32 d1, d1	\n\t"
			"vtrn.32 d2, d2	\n\t"
			"vtrn.32 d3, d3	\n\t"
			"vtrn.32 d4, d4	\n\t"
			"vtrn.32 d5, d5	\n\t"
			"vtrn.32 d6, d6	\n\t"
			"vtrn.32 d7, d7	\n\t"
			"vtrn.32 d8, d8	\n\t"
			"vtrn.32 d9, d9	\n\t"
			"vtrn.32 d10, d10	\n\t"
			"vtrn.32 d11, d11	\n\t"
			"vtrn.32 d12, d12	\n\t"
			"vtrn.32 d13, d13	\n\t"

			"vtrn.32 d0, d1	\n\t"
			"vtrn.32 d2, d3	\n\t"
			"vtrn.32 d4, d5	\n\t"
			"vtrn.32 d6, d7	\n\t"
			"vtrn.32 d8, d9	\n\t"
			"vtrn.32 d10, d11	\n\t"
			"vtrn.32 d12, d13	\n\t"		
//*/
			


/*
			//Debug&Test
			"vmov.16 d1,d14		\n\t"
			"vmov.16 d3,d15		\n\t"
			"vmov.16 d5,d16		\n\t"
			"vmov.16 d7,d17		\n\t"
			"vmov.16 d9,d18		\n\t"
			"vmov.16 d11,d19		\n\t"
			"vmov.16 d13,d20		\n\t"
*/

			//plain-text store
			//12 plain-text
			//"stmia %1, {r6-r9} 						\n\t"
			//"vstmia %1!, {q8-q11} 					\n\t"
			//"vstmia %1!, {q0-q7} 					\n\t"

			//"vst2.16 {d8,d9},[r2]!			\n\t"
			//"vst4.16 {d4,d5,d6,d7},[r2]!		\n\t"
			//"vst4.16 {d0,d1,d2,d3},[r2]!		\n\t"

			"vstmia %1!, {d0-d13}		\n\t"

			
			/* */
			"vpop         {q4-q7}                 \n\t"
			"pop          {r4-r10}                 \n\t"
			:
			:"r"(&key[0]),"r"(&text[0])
			:
	);
	return;

}

