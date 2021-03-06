
#include "SIMON64_96.h"
//#include <jni.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
//#include <cpu-features.h>
#include <arm_neon.h>


void SIMON64_96_NEON_ARM(uint32_t key[2 * 42], uint32_t text[2 * 12]) {

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

			"vldmia %1!,{d0-d11}	\n\t"

			//initialization
			"sub %1,%1,#96	\n\t" // #80 = 16*7(Q register * 7)

			//pre-processing
			"vtrn.32 d0, d1	\n\t"
			"vtrn.32 d2, d3	\n\t"
			"vtrn.32 d4, d5	\n\t"
			"vtrn.32 d6, d7	\n\t"
			"vtrn.32 d8, d9	\n\t"
			"vtrn.32 d10, d11	\n\t"		

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

			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u32 d12,d1,#31	\n\t"
			"vshr.u32 d13,d3,#31	\n\t"
			"vshr.u32 d14,d5,#31	\n\t"
			"vshr.u32 d15,d7,#31	\n\t"
			"vshr.u32 d16,d9,#31	\n\t"
			"vshr.u32 d17,d11,#31	\n\t"
	
			"vsli.u32 d12,d1,#1	\n\t"
			"vsli.u32 d13,d3,#1	\n\t"
			"vsli.u32 d14,d5,#1	\n\t"
			"vsli.u32 d15,d7,#1	\n\t"
			"vsli.u32 d16,d9,#1	\n\t"
			"vsli.u32 d17,d11,#1	\n\t"

			//ROL8(x)
			"vshr.u32 d18,d1,#24	\n\t"
			"vshr.u32 d19,d3,#24	\n\t"
			"vshr.u32 d20,d5,#24	\n\t"
			"vshr.u32 d21,d7,#24	\n\t"
			"vshr.u32 d22,d9,#24	\n\t"
			"vshr.u32 d23,d11,#24	\n\t"
	
			"vsli.u32 d18,d1,#8	\n\t"
			"vsli.u32 d19,d3,#8	\n\t"
			"vsli.u32 d20,d5,#8	\n\t"
			"vsli.u32 d21,d7,#8	\n\t"
			"vsli.u32 d22,d9,#8	\n\t"
			"vsli.u32 d23,d11,#8	\n\t"

			//ROL2(x)
			"vshr.u32 d24,d1,#30	\n\t"
			"vshr.u32 d25,d3,#30	\n\t"
			"vshr.u32 d26,d5,#30	\n\t"
			"vshr.u32 d27,d7,#30	\n\t"
			"vshr.u32 d28,d9,#30	\n\t"
			"vshr.u32 d29,d11,#30	\n\t"
	
			"vsli.u32 d24,d1,#2	\n\t"
			"vsli.u32 d25,d3,#2	\n\t"
			"vsli.u32 d26,d5,#2	\n\t"
			"vsli.u32 d27,d7,#2	\n\t"
			"vsli.u32 d28,d9,#2	\n\t"
			"vsli.u32 d29,d11,#2	\n\t"

			// ROL1(x) & ROL8(x)
			"vand d12,d12,d18	\n\t"
			"vand d13,d13,d19	\n\t"
			"vand d14,d14,d20	\n\t"
			"vand d15,d15,d21	\n\t"
			"vand d16,d16,d22	\n\t"
			"vand d17,d17,d23	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor d12, d12, d0	\n\t"
			"veor d13, d13, d2	\n\t"
			"veor d14, d14, d4	\n\t"
			"veor d15, d15, d6	\n\t"
			"veor d16, d16, d8	\n\t"
			"veor d17, d17, d10	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor d12, d12, d24	\n\t"
			"veor d13, d13, d25	\n\t"
			"veor d14, d14, d26	\n\t"
			"veor d15, d15, d27	\n\t"
			"veor d16, d16, d28	\n\t"
			"veor d17, d17, d29	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{d30}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor d12, d12, d30	\n\t"
			"veor d13, d13, d30	\n\t"
			"veor d14, d14, d30	\n\t"
			"veor d15, d15, d30	\n\t"
			"veor d16, d16, d30	\n\t"
			"veor d17, d17, d30	\n\t"

			//changing location of CT
			"vmov.32 d0,d1		\n\t"
			"vmov.32 d2,d3		\n\t"
			"vmov.32 d4,d5		\n\t"
			"vmov.32 d6,d7		\n\t"
			"vmov.32 d8,d9		\n\t"
			"vmov.32 d10,d11		\n\t"

			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"

			//42-round END


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

			"vtrn.32 d0, d1	\n\t"
			"vtrn.32 d2, d3	\n\t"
			"vtrn.32 d4, d5	\n\t"
			"vtrn.32 d6, d7	\n\t"
			"vtrn.32 d8, d9	\n\t"
			"vtrn.32 d10, d11	\n\t"		

			


/*
			//Debug&Test
			"vmov.32 d1,d12		\n\t"
			"vmov.32 d3,d13		\n\t"
			"vmov.32 d5,d14		\n\t"
			"vmov.32 d7,d15		\n\t"
			"vmov.32 d9,d16		\n\t"
			"vmov.32 d11,d17		\n\t"
*/

			//plain-text store
			//12 plain-text
			//"stmia %1, {r6-r9} 						\n\t"
			//"vstmia %1!, {q8-q11} 					\n\t"
			//"vstmia %1!, {q0-q7} 					\n\t"

			//"vst2.16 {d8,d9},[r2]!			\n\t"
			//"vst4.16 {d4,d5,d6,d7},[r2]!		\n\t"
			//"vst4.16 {d0,d1,d2,d3},[r2]!		\n\t"

			"vstmia %1!, {d0-d11}		\n\t"

			
			/* */
			"vpop         {q4-q7}                 \n\t"
			"pop          {r4-r10}                 \n\t"
			:
			:"r"(&key[0]),"r"(&text[0])
			:
	);
	return;

}

