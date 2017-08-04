
#include "SPECK32_64.h"
//#include <jni.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
//#include <cpu-features.h>
#include <arm_neon.h>


void SPECK32_64_NEON_ARM(uint16_t key[4 * 22], uint16_t text[4 * 14]) { //uint32_t text[13 * 4]) {

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
			"vtrn.16 d0, d1	\n\t"
			"vtrn.16 d2, d3	\n\t"
			"vtrn.16 d4, d5	\n\t"
			"vtrn.16 d6, d7	\n\t"
			"vtrn.16 d8, d9	\n\t"
			"vtrn.16 d10, d11	\n\t"
			"vtrn.16 d12, d13	\n\t"		

			"vtrn.16 d0, d0	\n\t"
			"vtrn.16 d1, d1	\n\t"
			"vtrn.16 d2, d2	\n\t"
			"vtrn.16 d3, d3	\n\t"
			"vtrn.16 d4, d4	\n\t"
			"vtrn.16 d5, d5	\n\t"
			"vtrn.16 d6, d6	\n\t"
			"vtrn.16 d7, d7	\n\t"
			"vtrn.16 d8, d8	\n\t"
			"vtrn.16 d9, d9	\n\t"
			"vtrn.16 d10, d10	\n\t"
			"vtrn.16 d11, d11	\n\t"
			"vtrn.16 d12, d12	\n\t"
			"vtrn.16 d13, d13	\n\t"

			//Round#1
			//ROR#7(x)
			"vshl.u16 d14,d1,#9	\n\t"
			"vshl.u16 d15,d3,#9	\n\t"
			"vshl.u16 d16,d5,#9	\n\t"
			"vshl.u16 d17,d7,#9	\n\t"
			"vshl.u16 d18,d9,#9	\n\t"
			"vshl.u16 d19,d11,#9	\n\t"
			"vshl.u16 d20,d13,#9	\n\t"
	
			"vsri.u16 d14,d1,#7	\n\t"
			"vsri.u16 d15,d3,#7	\n\t"
			"vsri.u16 d16,d5,#7	\n\t"
			"vsri.u16 d17,d7,#7	\n\t"
			"vsri.u16 d18,d9,#7	\n\t"
			"vsri.u16 d19,d11,#7	\n\t"
			"vsri.u16 d20,d13,#7	\n\t"

			// ROR#7(x) + y
			"vadd.i16 d14,d14,d0	\n\t"
			"vadd.i16 d15,d15,d2	\n\t"
			"vadd.i16 d16,d16,d4	\n\t"
			"vadd.i16 d17,d17,d6	\n\t"
			"vadd.i16 d18,d18,d8	\n\t"
			"vadd.i16 d19,d19,d10	\n\t"
			"vadd.i16 d20,d20,d12	\n\t"

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
			"vshr.u16 d21,d0,#14	\n\t"
			"vshr.u16 d22,d2,#14	\n\t"
			"vshr.u16 d23,d4,#14	\n\t"
			"vshr.u16 d24,d6,#14	\n\t"
			"vshr.u16 d25,d8,#14	\n\t"
			"vshr.u16 d26,d10,#14	\n\t"
			"vshr.u16 d27,d12,#14	\n\t"
	
			"vsli.u16 d21,d0,#2	\n\t"
			"vsli.u16 d22,d2,#2	\n\t"
			"vsli.u16 d23,d4,#2	\n\t"
			"vsli.u16 d24,d6,#2	\n\t"
			"vsli.u16 d25,d8,#2	\n\t"
			"vsli.u16 d26,d10,#2	\n\t"
			"vsli.u16 d27,d12,#2	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"

			//Round#1
			//ROR#7(x)
			"vshl.u16 d14,d1,#9	\n\t"
			"vshl.u16 d15,d3,#9	\n\t"
			"vshl.u16 d16,d5,#9	\n\t"
			"vshl.u16 d17,d7,#9	\n\t"
			"vshl.u16 d18,d9,#9	\n\t"
			"vshl.u16 d19,d11,#9	\n\t"
			"vshl.u16 d20,d13,#9	\n\t"
	
			"vsri.u16 d14,d1,#7	\n\t"
			"vsri.u16 d15,d3,#7	\n\t"
			"vsri.u16 d16,d5,#7	\n\t"
			"vsri.u16 d17,d7,#7	\n\t"
			"vsri.u16 d18,d9,#7	\n\t"
			"vsri.u16 d19,d11,#7	\n\t"
			"vsri.u16 d20,d13,#7	\n\t"

			// ROR#7(x) + y
			"vadd.i16 d14,d14,d0	\n\t"
			"vadd.i16 d15,d15,d2	\n\t"
			"vadd.i16 d16,d16,d4	\n\t"
			"vadd.i16 d17,d17,d6	\n\t"
			"vadd.i16 d18,d18,d8	\n\t"
			"vadd.i16 d19,d19,d10	\n\t"
			"vadd.i16 d20,d20,d12	\n\t"

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
			"vshr.u16 d21,d0,#14	\n\t"
			"vshr.u16 d22,d2,#14	\n\t"
			"vshr.u16 d23,d4,#14	\n\t"
			"vshr.u16 d24,d6,#14	\n\t"
			"vshr.u16 d25,d8,#14	\n\t"
			"vshr.u16 d26,d10,#14	\n\t"
			"vshr.u16 d27,d12,#14	\n\t"
	
			"vsli.u16 d21,d0,#2	\n\t"
			"vsli.u16 d22,d2,#2	\n\t"
			"vsli.u16 d23,d4,#2	\n\t"
			"vsli.u16 d24,d6,#2	\n\t"
			"vsli.u16 d25,d8,#2	\n\t"
			"vsli.u16 d26,d10,#2	\n\t"
			"vsli.u16 d27,d12,#2	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"

			//Round#1
			//ROR#7(x)
			"vshl.u16 d14,d1,#9	\n\t"
			"vshl.u16 d15,d3,#9	\n\t"
			"vshl.u16 d16,d5,#9	\n\t"
			"vshl.u16 d17,d7,#9	\n\t"
			"vshl.u16 d18,d9,#9	\n\t"
			"vshl.u16 d19,d11,#9	\n\t"
			"vshl.u16 d20,d13,#9	\n\t"
	
			"vsri.u16 d14,d1,#7	\n\t"
			"vsri.u16 d15,d3,#7	\n\t"
			"vsri.u16 d16,d5,#7	\n\t"
			"vsri.u16 d17,d7,#7	\n\t"
			"vsri.u16 d18,d9,#7	\n\t"
			"vsri.u16 d19,d11,#7	\n\t"
			"vsri.u16 d20,d13,#7	\n\t"

			// ROR#7(x) + y
			"vadd.i16 d14,d14,d0	\n\t"
			"vadd.i16 d15,d15,d2	\n\t"
			"vadd.i16 d16,d16,d4	\n\t"
			"vadd.i16 d17,d17,d6	\n\t"
			"vadd.i16 d18,d18,d8	\n\t"
			"vadd.i16 d19,d19,d10	\n\t"
			"vadd.i16 d20,d20,d12	\n\t"

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
			"vshr.u16 d21,d0,#14	\n\t"
			"vshr.u16 d22,d2,#14	\n\t"
			"vshr.u16 d23,d4,#14	\n\t"
			"vshr.u16 d24,d6,#14	\n\t"
			"vshr.u16 d25,d8,#14	\n\t"
			"vshr.u16 d26,d10,#14	\n\t"
			"vshr.u16 d27,d12,#14	\n\t"
	
			"vsli.u16 d21,d0,#2	\n\t"
			"vsli.u16 d22,d2,#2	\n\t"
			"vsli.u16 d23,d4,#2	\n\t"
			"vsli.u16 d24,d6,#2	\n\t"
			"vsli.u16 d25,d8,#2	\n\t"
			"vsli.u16 d26,d10,#2	\n\t"
			"vsli.u16 d27,d12,#2	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"

			//Round#1
			//ROR#7(x)
			"vshl.u16 d14,d1,#9	\n\t"
			"vshl.u16 d15,d3,#9	\n\t"
			"vshl.u16 d16,d5,#9	\n\t"
			"vshl.u16 d17,d7,#9	\n\t"
			"vshl.u16 d18,d9,#9	\n\t"
			"vshl.u16 d19,d11,#9	\n\t"
			"vshl.u16 d20,d13,#9	\n\t"
	
			"vsri.u16 d14,d1,#7	\n\t"
			"vsri.u16 d15,d3,#7	\n\t"
			"vsri.u16 d16,d5,#7	\n\t"
			"vsri.u16 d17,d7,#7	\n\t"
			"vsri.u16 d18,d9,#7	\n\t"
			"vsri.u16 d19,d11,#7	\n\t"
			"vsri.u16 d20,d13,#7	\n\t"

			// ROR#7(x) + y
			"vadd.i16 d14,d14,d0	\n\t"
			"vadd.i16 d15,d15,d2	\n\t"
			"vadd.i16 d16,d16,d4	\n\t"
			"vadd.i16 d17,d17,d6	\n\t"
			"vadd.i16 d18,d18,d8	\n\t"
			"vadd.i16 d19,d19,d10	\n\t"
			"vadd.i16 d20,d20,d12	\n\t"

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
			"vshr.u16 d21,d0,#14	\n\t"
			"vshr.u16 d22,d2,#14	\n\t"
			"vshr.u16 d23,d4,#14	\n\t"
			"vshr.u16 d24,d6,#14	\n\t"
			"vshr.u16 d25,d8,#14	\n\t"
			"vshr.u16 d26,d10,#14	\n\t"
			"vshr.u16 d27,d12,#14	\n\t"
	
			"vsli.u16 d21,d0,#2	\n\t"
			"vsli.u16 d22,d2,#2	\n\t"
			"vsli.u16 d23,d4,#2	\n\t"
			"vsli.u16 d24,d6,#2	\n\t"
			"vsli.u16 d25,d8,#2	\n\t"
			"vsli.u16 d26,d10,#2	\n\t"
			"vsli.u16 d27,d12,#2	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"

			//Round#1
			//ROR#7(x)
			"vshl.u16 d14,d1,#9	\n\t"
			"vshl.u16 d15,d3,#9	\n\t"
			"vshl.u16 d16,d5,#9	\n\t"
			"vshl.u16 d17,d7,#9	\n\t"
			"vshl.u16 d18,d9,#9	\n\t"
			"vshl.u16 d19,d11,#9	\n\t"
			"vshl.u16 d20,d13,#9	\n\t"
	
			"vsri.u16 d14,d1,#7	\n\t"
			"vsri.u16 d15,d3,#7	\n\t"
			"vsri.u16 d16,d5,#7	\n\t"
			"vsri.u16 d17,d7,#7	\n\t"
			"vsri.u16 d18,d9,#7	\n\t"
			"vsri.u16 d19,d11,#7	\n\t"
			"vsri.u16 d20,d13,#7	\n\t"

			// ROR#7(x) + y
			"vadd.i16 d14,d14,d0	\n\t"
			"vadd.i16 d15,d15,d2	\n\t"
			"vadd.i16 d16,d16,d4	\n\t"
			"vadd.i16 d17,d17,d6	\n\t"
			"vadd.i16 d18,d18,d8	\n\t"
			"vadd.i16 d19,d19,d10	\n\t"
			"vadd.i16 d20,d20,d12	\n\t"

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
			"vshr.u16 d21,d0,#14	\n\t"
			"vshr.u16 d22,d2,#14	\n\t"
			"vshr.u16 d23,d4,#14	\n\t"
			"vshr.u16 d24,d6,#14	\n\t"
			"vshr.u16 d25,d8,#14	\n\t"
			"vshr.u16 d26,d10,#14	\n\t"
			"vshr.u16 d27,d12,#14	\n\t"
	
			"vsli.u16 d21,d0,#2	\n\t"
			"vsli.u16 d22,d2,#2	\n\t"
			"vsli.u16 d23,d4,#2	\n\t"
			"vsli.u16 d24,d6,#2	\n\t"
			"vsli.u16 d25,d8,#2	\n\t"
			"vsli.u16 d26,d10,#2	\n\t"
			"vsli.u16 d27,d12,#2	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"

			//Round#1
			//ROR#7(x)
			"vshl.u16 d14,d1,#9	\n\t"
			"vshl.u16 d15,d3,#9	\n\t"
			"vshl.u16 d16,d5,#9	\n\t"
			"vshl.u16 d17,d7,#9	\n\t"
			"vshl.u16 d18,d9,#9	\n\t"
			"vshl.u16 d19,d11,#9	\n\t"
			"vshl.u16 d20,d13,#9	\n\t"
	
			"vsri.u16 d14,d1,#7	\n\t"
			"vsri.u16 d15,d3,#7	\n\t"
			"vsri.u16 d16,d5,#7	\n\t"
			"vsri.u16 d17,d7,#7	\n\t"
			"vsri.u16 d18,d9,#7	\n\t"
			"vsri.u16 d19,d11,#7	\n\t"
			"vsri.u16 d20,d13,#7	\n\t"

			// ROR#7(x) + y
			"vadd.i16 d14,d14,d0	\n\t"
			"vadd.i16 d15,d15,d2	\n\t"
			"vadd.i16 d16,d16,d4	\n\t"
			"vadd.i16 d17,d17,d6	\n\t"
			"vadd.i16 d18,d18,d8	\n\t"
			"vadd.i16 d19,d19,d10	\n\t"
			"vadd.i16 d20,d20,d12	\n\t"

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
			"vshr.u16 d21,d0,#14	\n\t"
			"vshr.u16 d22,d2,#14	\n\t"
			"vshr.u16 d23,d4,#14	\n\t"
			"vshr.u16 d24,d6,#14	\n\t"
			"vshr.u16 d25,d8,#14	\n\t"
			"vshr.u16 d26,d10,#14	\n\t"
			"vshr.u16 d27,d12,#14	\n\t"
	
			"vsli.u16 d21,d0,#2	\n\t"
			"vsli.u16 d22,d2,#2	\n\t"
			"vsli.u16 d23,d4,#2	\n\t"
			"vsli.u16 d24,d6,#2	\n\t"
			"vsli.u16 d25,d8,#2	\n\t"
			"vsli.u16 d26,d10,#2	\n\t"
			"vsli.u16 d27,d12,#2	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"

			//Round#1
			//ROR#7(x)
			"vshl.u16 d14,d1,#9	\n\t"
			"vshl.u16 d15,d3,#9	\n\t"
			"vshl.u16 d16,d5,#9	\n\t"
			"vshl.u16 d17,d7,#9	\n\t"
			"vshl.u16 d18,d9,#9	\n\t"
			"vshl.u16 d19,d11,#9	\n\t"
			"vshl.u16 d20,d13,#9	\n\t"
	
			"vsri.u16 d14,d1,#7	\n\t"
			"vsri.u16 d15,d3,#7	\n\t"
			"vsri.u16 d16,d5,#7	\n\t"
			"vsri.u16 d17,d7,#7	\n\t"
			"vsri.u16 d18,d9,#7	\n\t"
			"vsri.u16 d19,d11,#7	\n\t"
			"vsri.u16 d20,d13,#7	\n\t"

			// ROR#7(x) + y
			"vadd.i16 d14,d14,d0	\n\t"
			"vadd.i16 d15,d15,d2	\n\t"
			"vadd.i16 d16,d16,d4	\n\t"
			"vadd.i16 d17,d17,d6	\n\t"
			"vadd.i16 d18,d18,d8	\n\t"
			"vadd.i16 d19,d19,d10	\n\t"
			"vadd.i16 d20,d20,d12	\n\t"

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
			"vshr.u16 d21,d0,#14	\n\t"
			"vshr.u16 d22,d2,#14	\n\t"
			"vshr.u16 d23,d4,#14	\n\t"
			"vshr.u16 d24,d6,#14	\n\t"
			"vshr.u16 d25,d8,#14	\n\t"
			"vshr.u16 d26,d10,#14	\n\t"
			"vshr.u16 d27,d12,#14	\n\t"
	
			"vsli.u16 d21,d0,#2	\n\t"
			"vsli.u16 d22,d2,#2	\n\t"
			"vsli.u16 d23,d4,#2	\n\t"
			"vsli.u16 d24,d6,#2	\n\t"
			"vsli.u16 d25,d8,#2	\n\t"
			"vsli.u16 d26,d10,#2	\n\t"
			"vsli.u16 d27,d12,#2	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"

			//Round#1
			//ROR#7(x)
			"vshl.u16 d14,d1,#9	\n\t"
			"vshl.u16 d15,d3,#9	\n\t"
			"vshl.u16 d16,d5,#9	\n\t"
			"vshl.u16 d17,d7,#9	\n\t"
			"vshl.u16 d18,d9,#9	\n\t"
			"vshl.u16 d19,d11,#9	\n\t"
			"vshl.u16 d20,d13,#9	\n\t"
	
			"vsri.u16 d14,d1,#7	\n\t"
			"vsri.u16 d15,d3,#7	\n\t"
			"vsri.u16 d16,d5,#7	\n\t"
			"vsri.u16 d17,d7,#7	\n\t"
			"vsri.u16 d18,d9,#7	\n\t"
			"vsri.u16 d19,d11,#7	\n\t"
			"vsri.u16 d20,d13,#7	\n\t"

			// ROR#7(x) + y
			"vadd.i16 d14,d14,d0	\n\t"
			"vadd.i16 d15,d15,d2	\n\t"
			"vadd.i16 d16,d16,d4	\n\t"
			"vadd.i16 d17,d17,d6	\n\t"
			"vadd.i16 d18,d18,d8	\n\t"
			"vadd.i16 d19,d19,d10	\n\t"
			"vadd.i16 d20,d20,d12	\n\t"

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
			"vshr.u16 d21,d0,#14	\n\t"
			"vshr.u16 d22,d2,#14	\n\t"
			"vshr.u16 d23,d4,#14	\n\t"
			"vshr.u16 d24,d6,#14	\n\t"
			"vshr.u16 d25,d8,#14	\n\t"
			"vshr.u16 d26,d10,#14	\n\t"
			"vshr.u16 d27,d12,#14	\n\t"
	
			"vsli.u16 d21,d0,#2	\n\t"
			"vsli.u16 d22,d2,#2	\n\t"
			"vsli.u16 d23,d4,#2	\n\t"
			"vsli.u16 d24,d6,#2	\n\t"
			"vsli.u16 d25,d8,#2	\n\t"
			"vsli.u16 d26,d10,#2	\n\t"
			"vsli.u16 d27,d12,#2	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"

			//Round#1
			//ROR#7(x)
			"vshl.u16 d14,d1,#9	\n\t"
			"vshl.u16 d15,d3,#9	\n\t"
			"vshl.u16 d16,d5,#9	\n\t"
			"vshl.u16 d17,d7,#9	\n\t"
			"vshl.u16 d18,d9,#9	\n\t"
			"vshl.u16 d19,d11,#9	\n\t"
			"vshl.u16 d20,d13,#9	\n\t"
	
			"vsri.u16 d14,d1,#7	\n\t"
			"vsri.u16 d15,d3,#7	\n\t"
			"vsri.u16 d16,d5,#7	\n\t"
			"vsri.u16 d17,d7,#7	\n\t"
			"vsri.u16 d18,d9,#7	\n\t"
			"vsri.u16 d19,d11,#7	\n\t"
			"vsri.u16 d20,d13,#7	\n\t"

			// ROR#7(x) + y
			"vadd.i16 d14,d14,d0	\n\t"
			"vadd.i16 d15,d15,d2	\n\t"
			"vadd.i16 d16,d16,d4	\n\t"
			"vadd.i16 d17,d17,d6	\n\t"
			"vadd.i16 d18,d18,d8	\n\t"
			"vadd.i16 d19,d19,d10	\n\t"
			"vadd.i16 d20,d20,d12	\n\t"

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
			"vshr.u16 d21,d0,#14	\n\t"
			"vshr.u16 d22,d2,#14	\n\t"
			"vshr.u16 d23,d4,#14	\n\t"
			"vshr.u16 d24,d6,#14	\n\t"
			"vshr.u16 d25,d8,#14	\n\t"
			"vshr.u16 d26,d10,#14	\n\t"
			"vshr.u16 d27,d12,#14	\n\t"
	
			"vsli.u16 d21,d0,#2	\n\t"
			"vsli.u16 d22,d2,#2	\n\t"
			"vsli.u16 d23,d4,#2	\n\t"
			"vsli.u16 d24,d6,#2	\n\t"
			"vsli.u16 d25,d8,#2	\n\t"
			"vsli.u16 d26,d10,#2	\n\t"
			"vsli.u16 d27,d12,#2	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"

			//Round#1
			//ROR#7(x)
			"vshl.u16 d14,d1,#9	\n\t"
			"vshl.u16 d15,d3,#9	\n\t"
			"vshl.u16 d16,d5,#9	\n\t"
			"vshl.u16 d17,d7,#9	\n\t"
			"vshl.u16 d18,d9,#9	\n\t"
			"vshl.u16 d19,d11,#9	\n\t"
			"vshl.u16 d20,d13,#9	\n\t"
	
			"vsri.u16 d14,d1,#7	\n\t"
			"vsri.u16 d15,d3,#7	\n\t"
			"vsri.u16 d16,d5,#7	\n\t"
			"vsri.u16 d17,d7,#7	\n\t"
			"vsri.u16 d18,d9,#7	\n\t"
			"vsri.u16 d19,d11,#7	\n\t"
			"vsri.u16 d20,d13,#7	\n\t"

			// ROR#7(x) + y
			"vadd.i16 d14,d14,d0	\n\t"
			"vadd.i16 d15,d15,d2	\n\t"
			"vadd.i16 d16,d16,d4	\n\t"
			"vadd.i16 d17,d17,d6	\n\t"
			"vadd.i16 d18,d18,d8	\n\t"
			"vadd.i16 d19,d19,d10	\n\t"
			"vadd.i16 d20,d20,d12	\n\t"

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
			"vshr.u16 d21,d0,#14	\n\t"
			"vshr.u16 d22,d2,#14	\n\t"
			"vshr.u16 d23,d4,#14	\n\t"
			"vshr.u16 d24,d6,#14	\n\t"
			"vshr.u16 d25,d8,#14	\n\t"
			"vshr.u16 d26,d10,#14	\n\t"
			"vshr.u16 d27,d12,#14	\n\t"
	
			"vsli.u16 d21,d0,#2	\n\t"
			"vsli.u16 d22,d2,#2	\n\t"
			"vsli.u16 d23,d4,#2	\n\t"
			"vsli.u16 d24,d6,#2	\n\t"
			"vsli.u16 d25,d8,#2	\n\t"
			"vsli.u16 d26,d10,#2	\n\t"
			"vsli.u16 d27,d12,#2	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"

			//Round#1
			//ROR#7(x)
			"vshl.u16 d14,d1,#9	\n\t"
			"vshl.u16 d15,d3,#9	\n\t"
			"vshl.u16 d16,d5,#9	\n\t"
			"vshl.u16 d17,d7,#9	\n\t"
			"vshl.u16 d18,d9,#9	\n\t"
			"vshl.u16 d19,d11,#9	\n\t"
			"vshl.u16 d20,d13,#9	\n\t"
	
			"vsri.u16 d14,d1,#7	\n\t"
			"vsri.u16 d15,d3,#7	\n\t"
			"vsri.u16 d16,d5,#7	\n\t"
			"vsri.u16 d17,d7,#7	\n\t"
			"vsri.u16 d18,d9,#7	\n\t"
			"vsri.u16 d19,d11,#7	\n\t"
			"vsri.u16 d20,d13,#7	\n\t"

			// ROR#7(x) + y
			"vadd.i16 d14,d14,d0	\n\t"
			"vadd.i16 d15,d15,d2	\n\t"
			"vadd.i16 d16,d16,d4	\n\t"
			"vadd.i16 d17,d17,d6	\n\t"
			"vadd.i16 d18,d18,d8	\n\t"
			"vadd.i16 d19,d19,d10	\n\t"
			"vadd.i16 d20,d20,d12	\n\t"

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
			"vshr.u16 d21,d0,#14	\n\t"
			"vshr.u16 d22,d2,#14	\n\t"
			"vshr.u16 d23,d4,#14	\n\t"
			"vshr.u16 d24,d6,#14	\n\t"
			"vshr.u16 d25,d8,#14	\n\t"
			"vshr.u16 d26,d10,#14	\n\t"
			"vshr.u16 d27,d12,#14	\n\t"
	
			"vsli.u16 d21,d0,#2	\n\t"
			"vsli.u16 d22,d2,#2	\n\t"
			"vsli.u16 d23,d4,#2	\n\t"
			"vsli.u16 d24,d6,#2	\n\t"
			"vsli.u16 d25,d8,#2	\n\t"
			"vsli.u16 d26,d10,#2	\n\t"
			"vsli.u16 d27,d12,#2	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"

			//Round#1
			//ROR#7(x)
			"vshl.u16 d14,d1,#9	\n\t"
			"vshl.u16 d15,d3,#9	\n\t"
			"vshl.u16 d16,d5,#9	\n\t"
			"vshl.u16 d17,d7,#9	\n\t"
			"vshl.u16 d18,d9,#9	\n\t"
			"vshl.u16 d19,d11,#9	\n\t"
			"vshl.u16 d20,d13,#9	\n\t"
	
			"vsri.u16 d14,d1,#7	\n\t"
			"vsri.u16 d15,d3,#7	\n\t"
			"vsri.u16 d16,d5,#7	\n\t"
			"vsri.u16 d17,d7,#7	\n\t"
			"vsri.u16 d18,d9,#7	\n\t"
			"vsri.u16 d19,d11,#7	\n\t"
			"vsri.u16 d20,d13,#7	\n\t"

			// ROR#7(x) + y
			"vadd.i16 d14,d14,d0	\n\t"
			"vadd.i16 d15,d15,d2	\n\t"
			"vadd.i16 d16,d16,d4	\n\t"
			"vadd.i16 d17,d17,d6	\n\t"
			"vadd.i16 d18,d18,d8	\n\t"
			"vadd.i16 d19,d19,d10	\n\t"
			"vadd.i16 d20,d20,d12	\n\t"

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
			"vshr.u16 d21,d0,#14	\n\t"
			"vshr.u16 d22,d2,#14	\n\t"
			"vshr.u16 d23,d4,#14	\n\t"
			"vshr.u16 d24,d6,#14	\n\t"
			"vshr.u16 d25,d8,#14	\n\t"
			"vshr.u16 d26,d10,#14	\n\t"
			"vshr.u16 d27,d12,#14	\n\t"
	
			"vsli.u16 d21,d0,#2	\n\t"
			"vsli.u16 d22,d2,#2	\n\t"
			"vsli.u16 d23,d4,#2	\n\t"
			"vsli.u16 d24,d6,#2	\n\t"
			"vsli.u16 d25,d8,#2	\n\t"
			"vsli.u16 d26,d10,#2	\n\t"
			"vsli.u16 d27,d12,#2	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"

			//Round#1
			//ROR#7(x)
			"vshl.u16 d14,d1,#9	\n\t"
			"vshl.u16 d15,d3,#9	\n\t"
			"vshl.u16 d16,d5,#9	\n\t"
			"vshl.u16 d17,d7,#9	\n\t"
			"vshl.u16 d18,d9,#9	\n\t"
			"vshl.u16 d19,d11,#9	\n\t"
			"vshl.u16 d20,d13,#9	\n\t"
	
			"vsri.u16 d14,d1,#7	\n\t"
			"vsri.u16 d15,d3,#7	\n\t"
			"vsri.u16 d16,d5,#7	\n\t"
			"vsri.u16 d17,d7,#7	\n\t"
			"vsri.u16 d18,d9,#7	\n\t"
			"vsri.u16 d19,d11,#7	\n\t"
			"vsri.u16 d20,d13,#7	\n\t"

			// ROR#7(x) + y
			"vadd.i16 d14,d14,d0	\n\t"
			"vadd.i16 d15,d15,d2	\n\t"
			"vadd.i16 d16,d16,d4	\n\t"
			"vadd.i16 d17,d17,d6	\n\t"
			"vadd.i16 d18,d18,d8	\n\t"
			"vadd.i16 d19,d19,d10	\n\t"
			"vadd.i16 d20,d20,d12	\n\t"

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
			"vshr.u16 d21,d0,#14	\n\t"
			"vshr.u16 d22,d2,#14	\n\t"
			"vshr.u16 d23,d4,#14	\n\t"
			"vshr.u16 d24,d6,#14	\n\t"
			"vshr.u16 d25,d8,#14	\n\t"
			"vshr.u16 d26,d10,#14	\n\t"
			"vshr.u16 d27,d12,#14	\n\t"
	
			"vsli.u16 d21,d0,#2	\n\t"
			"vsli.u16 d22,d2,#2	\n\t"
			"vsli.u16 d23,d4,#2	\n\t"
			"vsli.u16 d24,d6,#2	\n\t"
			"vsli.u16 d25,d8,#2	\n\t"
			"vsli.u16 d26,d10,#2	\n\t"
			"vsli.u16 d27,d12,#2	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"

			//Round#1
			//ROR#7(x)
			"vshl.u16 d14,d1,#9	\n\t"
			"vshl.u16 d15,d3,#9	\n\t"
			"vshl.u16 d16,d5,#9	\n\t"
			"vshl.u16 d17,d7,#9	\n\t"
			"vshl.u16 d18,d9,#9	\n\t"
			"vshl.u16 d19,d11,#9	\n\t"
			"vshl.u16 d20,d13,#9	\n\t"
	
			"vsri.u16 d14,d1,#7	\n\t"
			"vsri.u16 d15,d3,#7	\n\t"
			"vsri.u16 d16,d5,#7	\n\t"
			"vsri.u16 d17,d7,#7	\n\t"
			"vsri.u16 d18,d9,#7	\n\t"
			"vsri.u16 d19,d11,#7	\n\t"
			"vsri.u16 d20,d13,#7	\n\t"

			// ROR#7(x) + y
			"vadd.i16 d14,d14,d0	\n\t"
			"vadd.i16 d15,d15,d2	\n\t"
			"vadd.i16 d16,d16,d4	\n\t"
			"vadd.i16 d17,d17,d6	\n\t"
			"vadd.i16 d18,d18,d8	\n\t"
			"vadd.i16 d19,d19,d10	\n\t"
			"vadd.i16 d20,d20,d12	\n\t"

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
			"vshr.u16 d21,d0,#14	\n\t"
			"vshr.u16 d22,d2,#14	\n\t"
			"vshr.u16 d23,d4,#14	\n\t"
			"vshr.u16 d24,d6,#14	\n\t"
			"vshr.u16 d25,d8,#14	\n\t"
			"vshr.u16 d26,d10,#14	\n\t"
			"vshr.u16 d27,d12,#14	\n\t"
	
			"vsli.u16 d21,d0,#2	\n\t"
			"vsli.u16 d22,d2,#2	\n\t"
			"vsli.u16 d23,d4,#2	\n\t"
			"vsli.u16 d24,d6,#2	\n\t"
			"vsli.u16 d25,d8,#2	\n\t"
			"vsli.u16 d26,d10,#2	\n\t"
			"vsli.u16 d27,d12,#2	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"

			//Round#1
			//ROR#7(x)
			"vshl.u16 d14,d1,#9	\n\t"
			"vshl.u16 d15,d3,#9	\n\t"
			"vshl.u16 d16,d5,#9	\n\t"
			"vshl.u16 d17,d7,#9	\n\t"
			"vshl.u16 d18,d9,#9	\n\t"
			"vshl.u16 d19,d11,#9	\n\t"
			"vshl.u16 d20,d13,#9	\n\t"
	
			"vsri.u16 d14,d1,#7	\n\t"
			"vsri.u16 d15,d3,#7	\n\t"
			"vsri.u16 d16,d5,#7	\n\t"
			"vsri.u16 d17,d7,#7	\n\t"
			"vsri.u16 d18,d9,#7	\n\t"
			"vsri.u16 d19,d11,#7	\n\t"
			"vsri.u16 d20,d13,#7	\n\t"

			// ROR#7(x) + y
			"vadd.i16 d14,d14,d0	\n\t"
			"vadd.i16 d15,d15,d2	\n\t"
			"vadd.i16 d16,d16,d4	\n\t"
			"vadd.i16 d17,d17,d6	\n\t"
			"vadd.i16 d18,d18,d8	\n\t"
			"vadd.i16 d19,d19,d10	\n\t"
			"vadd.i16 d20,d20,d12	\n\t"

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
			"vshr.u16 d21,d0,#14	\n\t"
			"vshr.u16 d22,d2,#14	\n\t"
			"vshr.u16 d23,d4,#14	\n\t"
			"vshr.u16 d24,d6,#14	\n\t"
			"vshr.u16 d25,d8,#14	\n\t"
			"vshr.u16 d26,d10,#14	\n\t"
			"vshr.u16 d27,d12,#14	\n\t"
	
			"vsli.u16 d21,d0,#2	\n\t"
			"vsli.u16 d22,d2,#2	\n\t"
			"vsli.u16 d23,d4,#2	\n\t"
			"vsli.u16 d24,d6,#2	\n\t"
			"vsli.u16 d25,d8,#2	\n\t"
			"vsli.u16 d26,d10,#2	\n\t"
			"vsli.u16 d27,d12,#2	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"

			//Round#1
			//ROR#7(x)
			"vshl.u16 d14,d1,#9	\n\t"
			"vshl.u16 d15,d3,#9	\n\t"
			"vshl.u16 d16,d5,#9	\n\t"
			"vshl.u16 d17,d7,#9	\n\t"
			"vshl.u16 d18,d9,#9	\n\t"
			"vshl.u16 d19,d11,#9	\n\t"
			"vshl.u16 d20,d13,#9	\n\t"
	
			"vsri.u16 d14,d1,#7	\n\t"
			"vsri.u16 d15,d3,#7	\n\t"
			"vsri.u16 d16,d5,#7	\n\t"
			"vsri.u16 d17,d7,#7	\n\t"
			"vsri.u16 d18,d9,#7	\n\t"
			"vsri.u16 d19,d11,#7	\n\t"
			"vsri.u16 d20,d13,#7	\n\t"

			// ROR#7(x) + y
			"vadd.i16 d14,d14,d0	\n\t"
			"vadd.i16 d15,d15,d2	\n\t"
			"vadd.i16 d16,d16,d4	\n\t"
			"vadd.i16 d17,d17,d6	\n\t"
			"vadd.i16 d18,d18,d8	\n\t"
			"vadd.i16 d19,d19,d10	\n\t"
			"vadd.i16 d20,d20,d12	\n\t"

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
			"vshr.u16 d21,d0,#14	\n\t"
			"vshr.u16 d22,d2,#14	\n\t"
			"vshr.u16 d23,d4,#14	\n\t"
			"vshr.u16 d24,d6,#14	\n\t"
			"vshr.u16 d25,d8,#14	\n\t"
			"vshr.u16 d26,d10,#14	\n\t"
			"vshr.u16 d27,d12,#14	\n\t"
	
			"vsli.u16 d21,d0,#2	\n\t"
			"vsli.u16 d22,d2,#2	\n\t"
			"vsli.u16 d23,d4,#2	\n\t"
			"vsli.u16 d24,d6,#2	\n\t"
			"vsli.u16 d25,d8,#2	\n\t"
			"vsli.u16 d26,d10,#2	\n\t"
			"vsli.u16 d27,d12,#2	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"

			//Round#1
			//ROR#7(x)
			"vshl.u16 d14,d1,#9	\n\t"
			"vshl.u16 d15,d3,#9	\n\t"
			"vshl.u16 d16,d5,#9	\n\t"
			"vshl.u16 d17,d7,#9	\n\t"
			"vshl.u16 d18,d9,#9	\n\t"
			"vshl.u16 d19,d11,#9	\n\t"
			"vshl.u16 d20,d13,#9	\n\t"
	
			"vsri.u16 d14,d1,#7	\n\t"
			"vsri.u16 d15,d3,#7	\n\t"
			"vsri.u16 d16,d5,#7	\n\t"
			"vsri.u16 d17,d7,#7	\n\t"
			"vsri.u16 d18,d9,#7	\n\t"
			"vsri.u16 d19,d11,#7	\n\t"
			"vsri.u16 d20,d13,#7	\n\t"

			// ROR#7(x) + y
			"vadd.i16 d14,d14,d0	\n\t"
			"vadd.i16 d15,d15,d2	\n\t"
			"vadd.i16 d16,d16,d4	\n\t"
			"vadd.i16 d17,d17,d6	\n\t"
			"vadd.i16 d18,d18,d8	\n\t"
			"vadd.i16 d19,d19,d10	\n\t"
			"vadd.i16 d20,d20,d12	\n\t"

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
			"vshr.u16 d21,d0,#14	\n\t"
			"vshr.u16 d22,d2,#14	\n\t"
			"vshr.u16 d23,d4,#14	\n\t"
			"vshr.u16 d24,d6,#14	\n\t"
			"vshr.u16 d25,d8,#14	\n\t"
			"vshr.u16 d26,d10,#14	\n\t"
			"vshr.u16 d27,d12,#14	\n\t"
	
			"vsli.u16 d21,d0,#2	\n\t"
			"vsli.u16 d22,d2,#2	\n\t"
			"vsli.u16 d23,d4,#2	\n\t"
			"vsli.u16 d24,d6,#2	\n\t"
			"vsli.u16 d25,d8,#2	\n\t"
			"vsli.u16 d26,d10,#2	\n\t"
			"vsli.u16 d27,d12,#2	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"

			//Round#1
			//ROR#7(x)
			"vshl.u16 d14,d1,#9	\n\t"
			"vshl.u16 d15,d3,#9	\n\t"
			"vshl.u16 d16,d5,#9	\n\t"
			"vshl.u16 d17,d7,#9	\n\t"
			"vshl.u16 d18,d9,#9	\n\t"
			"vshl.u16 d19,d11,#9	\n\t"
			"vshl.u16 d20,d13,#9	\n\t"
	
			"vsri.u16 d14,d1,#7	\n\t"
			"vsri.u16 d15,d3,#7	\n\t"
			"vsri.u16 d16,d5,#7	\n\t"
			"vsri.u16 d17,d7,#7	\n\t"
			"vsri.u16 d18,d9,#7	\n\t"
			"vsri.u16 d19,d11,#7	\n\t"
			"vsri.u16 d20,d13,#7	\n\t"

			// ROR#7(x) + y
			"vadd.i16 d14,d14,d0	\n\t"
			"vadd.i16 d15,d15,d2	\n\t"
			"vadd.i16 d16,d16,d4	\n\t"
			"vadd.i16 d17,d17,d6	\n\t"
			"vadd.i16 d18,d18,d8	\n\t"
			"vadd.i16 d19,d19,d10	\n\t"
			"vadd.i16 d20,d20,d12	\n\t"

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
			"vshr.u16 d21,d0,#14	\n\t"
			"vshr.u16 d22,d2,#14	\n\t"
			"vshr.u16 d23,d4,#14	\n\t"
			"vshr.u16 d24,d6,#14	\n\t"
			"vshr.u16 d25,d8,#14	\n\t"
			"vshr.u16 d26,d10,#14	\n\t"
			"vshr.u16 d27,d12,#14	\n\t"
	
			"vsli.u16 d21,d0,#2	\n\t"
			"vsli.u16 d22,d2,#2	\n\t"
			"vsli.u16 d23,d4,#2	\n\t"
			"vsli.u16 d24,d6,#2	\n\t"
			"vsli.u16 d25,d8,#2	\n\t"
			"vsli.u16 d26,d10,#2	\n\t"
			"vsli.u16 d27,d12,#2	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"

			//Round#1
			//ROR#7(x)
			"vshl.u16 d14,d1,#9	\n\t"
			"vshl.u16 d15,d3,#9	\n\t"
			"vshl.u16 d16,d5,#9	\n\t"
			"vshl.u16 d17,d7,#9	\n\t"
			"vshl.u16 d18,d9,#9	\n\t"
			"vshl.u16 d19,d11,#9	\n\t"
			"vshl.u16 d20,d13,#9	\n\t"
	
			"vsri.u16 d14,d1,#7	\n\t"
			"vsri.u16 d15,d3,#7	\n\t"
			"vsri.u16 d16,d5,#7	\n\t"
			"vsri.u16 d17,d7,#7	\n\t"
			"vsri.u16 d18,d9,#7	\n\t"
			"vsri.u16 d19,d11,#7	\n\t"
			"vsri.u16 d20,d13,#7	\n\t"

			// ROR#7(x) + y
			"vadd.i16 d14,d14,d0	\n\t"
			"vadd.i16 d15,d15,d2	\n\t"
			"vadd.i16 d16,d16,d4	\n\t"
			"vadd.i16 d17,d17,d6	\n\t"
			"vadd.i16 d18,d18,d8	\n\t"
			"vadd.i16 d19,d19,d10	\n\t"
			"vadd.i16 d20,d20,d12	\n\t"

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
			"vshr.u16 d21,d0,#14	\n\t"
			"vshr.u16 d22,d2,#14	\n\t"
			"vshr.u16 d23,d4,#14	\n\t"
			"vshr.u16 d24,d6,#14	\n\t"
			"vshr.u16 d25,d8,#14	\n\t"
			"vshr.u16 d26,d10,#14	\n\t"
			"vshr.u16 d27,d12,#14	\n\t"
	
			"vsli.u16 d21,d0,#2	\n\t"
			"vsli.u16 d22,d2,#2	\n\t"
			"vsli.u16 d23,d4,#2	\n\t"
			"vsli.u16 d24,d6,#2	\n\t"
			"vsli.u16 d25,d8,#2	\n\t"
			"vsli.u16 d26,d10,#2	\n\t"
			"vsli.u16 d27,d12,#2	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"

			//Round#1
			//ROR#7(x)
			"vshl.u16 d14,d1,#9	\n\t"
			"vshl.u16 d15,d3,#9	\n\t"
			"vshl.u16 d16,d5,#9	\n\t"
			"vshl.u16 d17,d7,#9	\n\t"
			"vshl.u16 d18,d9,#9	\n\t"
			"vshl.u16 d19,d11,#9	\n\t"
			"vshl.u16 d20,d13,#9	\n\t"
	
			"vsri.u16 d14,d1,#7	\n\t"
			"vsri.u16 d15,d3,#7	\n\t"
			"vsri.u16 d16,d5,#7	\n\t"
			"vsri.u16 d17,d7,#7	\n\t"
			"vsri.u16 d18,d9,#7	\n\t"
			"vsri.u16 d19,d11,#7	\n\t"
			"vsri.u16 d20,d13,#7	\n\t"

			// ROR#7(x) + y
			"vadd.i16 d14,d14,d0	\n\t"
			"vadd.i16 d15,d15,d2	\n\t"
			"vadd.i16 d16,d16,d4	\n\t"
			"vadd.i16 d17,d17,d6	\n\t"
			"vadd.i16 d18,d18,d8	\n\t"
			"vadd.i16 d19,d19,d10	\n\t"
			"vadd.i16 d20,d20,d12	\n\t"

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
			"vshr.u16 d21,d0,#14	\n\t"
			"vshr.u16 d22,d2,#14	\n\t"
			"vshr.u16 d23,d4,#14	\n\t"
			"vshr.u16 d24,d6,#14	\n\t"
			"vshr.u16 d25,d8,#14	\n\t"
			"vshr.u16 d26,d10,#14	\n\t"
			"vshr.u16 d27,d12,#14	\n\t"
	
			"vsli.u16 d21,d0,#2	\n\t"
			"vsli.u16 d22,d2,#2	\n\t"
			"vsli.u16 d23,d4,#2	\n\t"
			"vsli.u16 d24,d6,#2	\n\t"
			"vsli.u16 d25,d8,#2	\n\t"
			"vsli.u16 d26,d10,#2	\n\t"
			"vsli.u16 d27,d12,#2	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"

			//Round#1
			//ROR#7(x)
			"vshl.u16 d14,d1,#9	\n\t"
			"vshl.u16 d15,d3,#9	\n\t"
			"vshl.u16 d16,d5,#9	\n\t"
			"vshl.u16 d17,d7,#9	\n\t"
			"vshl.u16 d18,d9,#9	\n\t"
			"vshl.u16 d19,d11,#9	\n\t"
			"vshl.u16 d20,d13,#9	\n\t"
	
			"vsri.u16 d14,d1,#7	\n\t"
			"vsri.u16 d15,d3,#7	\n\t"
			"vsri.u16 d16,d5,#7	\n\t"
			"vsri.u16 d17,d7,#7	\n\t"
			"vsri.u16 d18,d9,#7	\n\t"
			"vsri.u16 d19,d11,#7	\n\t"
			"vsri.u16 d20,d13,#7	\n\t"

			// ROR#7(x) + y
			"vadd.i16 d14,d14,d0	\n\t"
			"vadd.i16 d15,d15,d2	\n\t"
			"vadd.i16 d16,d16,d4	\n\t"
			"vadd.i16 d17,d17,d6	\n\t"
			"vadd.i16 d18,d18,d8	\n\t"
			"vadd.i16 d19,d19,d10	\n\t"
			"vadd.i16 d20,d20,d12	\n\t"

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
			"vshr.u16 d21,d0,#14	\n\t"
			"vshr.u16 d22,d2,#14	\n\t"
			"vshr.u16 d23,d4,#14	\n\t"
			"vshr.u16 d24,d6,#14	\n\t"
			"vshr.u16 d25,d8,#14	\n\t"
			"vshr.u16 d26,d10,#14	\n\t"
			"vshr.u16 d27,d12,#14	\n\t"
	
			"vsli.u16 d21,d0,#2	\n\t"
			"vsli.u16 d22,d2,#2	\n\t"
			"vsli.u16 d23,d4,#2	\n\t"
			"vsli.u16 d24,d6,#2	\n\t"
			"vsli.u16 d25,d8,#2	\n\t"
			"vsli.u16 d26,d10,#2	\n\t"
			"vsli.u16 d27,d12,#2	\n\t"



			//((ROR#7(x) + y) ^ Roundkey) ^ ROL_beta(3)(y)
			"veor d0, d1, d21	\n\t"
			"veor d2, d3, d22	\n\t"
			"veor d4, d5, d23	\n\t"
			"veor d6, d7, d24	\n\t"
			"veor d8, d9, d25	\n\t"
			"veor d10, d11, d26	\n\t"
			"veor d12, d13, d27	\n\t"

			//Round#1
			//ROR#7(x)
			"vshl.u16 d14,d1,#9	\n\t"
			"vshl.u16 d15,d3,#9	\n\t"
			"vshl.u16 d16,d5,#9	\n\t"
			"vshl.u16 d17,d7,#9	\n\t"
			"vshl.u16 d18,d9,#9	\n\t"
			"vshl.u16 d19,d11,#9	\n\t"
			"vshl.u16 d20,d13,#9	\n\t"
	
			"vsri.u16 d14,d1,#7	\n\t"
			"vsri.u16 d15,d3,#7	\n\t"
			"vsri.u16 d16,d5,#7	\n\t"
			"vsri.u16 d17,d7,#7	\n\t"
			"vsri.u16 d18,d9,#7	\n\t"
			"vsri.u16 d19,d11,#7	\n\t"
			"vsri.u16 d20,d13,#7	\n\t"

			// ROR#7(x) + y
			"vadd.i16 d14,d14,d0	\n\t"
			"vadd.i16 d15,d15,d2	\n\t"
			"vadd.i16 d16,d16,d4	\n\t"
			"vadd.i16 d17,d17,d6	\n\t"
			"vadd.i16 d18,d18,d8	\n\t"
			"vadd.i16 d19,d19,d10	\n\t"
			"vadd.i16 d20,d20,d12	\n\t"

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
			"vshr.u16 d21,d0,#14	\n\t"
			"vshr.u16 d22,d2,#14	\n\t"
			"vshr.u16 d23,d4,#14	\n\t"
			"vshr.u16 d24,d6,#14	\n\t"
			"vshr.u16 d25,d8,#14	\n\t"
			"vshr.u16 d26,d10,#14	\n\t"
			"vshr.u16 d27,d12,#14	\n\t"
	
			"vsli.u16 d21,d0,#2	\n\t"
			"vsli.u16 d22,d2,#2	\n\t"
			"vsli.u16 d23,d4,#2	\n\t"
			"vsli.u16 d24,d6,#2	\n\t"
			"vsli.u16 d25,d8,#2	\n\t"
			"vsli.u16 d26,d10,#2	\n\t"
			"vsli.u16 d27,d12,#2	\n\t"



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
			"vtrn.16 d0, d0	\n\t"
			"vtrn.16 d1, d1	\n\t"
			"vtrn.16 d2, d2	\n\t"
			"vtrn.16 d3, d3	\n\t"
			"vtrn.16 d4, d4	\n\t"
			"vtrn.16 d5, d5	\n\t"
			"vtrn.16 d6, d6	\n\t"
			"vtrn.16 d7, d7	\n\t"
			"vtrn.16 d8, d8	\n\t"
			"vtrn.16 d9, d9	\n\t"
			"vtrn.16 d10, d10	\n\t"
			"vtrn.16 d11, d11	\n\t"
			"vtrn.16 d12, d12	\n\t"
			"vtrn.16 d13, d13	\n\t"

			"vtrn.16 d0, d1	\n\t"
			"vtrn.16 d2, d3	\n\t"
			"vtrn.16 d4, d5	\n\t"
			"vtrn.16 d6, d7	\n\t"
			"vtrn.16 d8, d9	\n\t"
			"vtrn.16 d10, d11	\n\t"
			"vtrn.16 d12, d13	\n\t"		
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

