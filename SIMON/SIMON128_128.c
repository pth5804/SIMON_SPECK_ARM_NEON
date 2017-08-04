
#include "SIMON128_128.h"
//#include <jni.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
//#include <cpu-features.h>
#include <arm_neon.h>


void SIMON128_128_NEON_ARM(uint64_t key[2 * 68], uint64_t text[2 * 6]) {

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
			"vmov.64 d12, d2	\n\t"
			"vmov.64 d13, d1	\n\t"
			"vmov.64 d14, d6	\n\t"
			"vmov.64 d15, d5	\n\t"
			"vmov.64 d16, d10	\n\t"
			"vmov.64 d17, d9	\n\t"

			"vmov.64 d1,d12	\n\t"
			"vmov.64 d2,d13	\n\t"
			"vmov.64 d5,d14	\n\t"
			"vmov.64 d6,d15	\n\t"
			"vmov.64 d9,d16	\n\t"
			"vmov.64 d10,d17	\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"
	

			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"


			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"



			//Round#1
			//ROL#1(x)
			"vshr.u64 q6,q1,#63	\n\t"
			"vshr.u64 q7,q3,#63	\n\t"
			"vshr.u64 q8,q5,#63	\n\t"
	
			"vsli.u64 q6,q1,#1	\n\t"
			"vsli.u64 q7,q3,#1	\n\t"
			"vsli.u64 q8,q5,#1	\n\t"

			//ROL8(x)
			"vshr.u64 q9,q1,#56	\n\t"
			"vshr.u64 q10,q3,#56	\n\t"
			"vshr.u64 q11,q5,#56	\n\t"
	
			"vsli.u64 q9,q1,#8	\n\t"
			"vsli.u64 q10,q3,#8	\n\t"
			"vsli.u64 q11,q5,#8	\n\t"

			// ROL1(x) & ROL8(x)
			"vand q6,q6,q9	\n\t"
			"vand q7,q7,q10	\n\t"
			"vand q8,q8,q11	\n\t"

			// y ^(ROL1(x) & ROL8(x))
			"veor q6, q6, q0	\n\t"
			"veor q7, q7, q2	\n\t"
			"veor q8, q8, q4	\n\t"

			//ROL2(x)
			"vshr.u64 q12,q1,#62	\n\t"
			"vshr.u64 q13,q3,#62	\n\t"
			"vshr.u64 q14,q5,#62	\n\t"
	
			"vsli.u64 q12,q1,#2	\n\t"
			"vsli.u64 q13,q3,#2	\n\t"
			"vsli.u64 q14,q5,#2	\n\t"

			//( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)
			"veor q6, q6, q12	\n\t"
			"veor q7, q7, q13	\n\t"
			"veor q8, q8, q14	\n\t"
			
			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//(( y ^(ROL1(x) & ROL8(x))) ^ ROL2(x)) ^ RK
			"veor q6, q6, q15	\n\t"
			"veor q7, q7, q15	\n\t"
			"veor q8, q8, q15	\n\t"


			//changing location of CT
			"vmov.64 q0,q1		\n\t"
			"vmov.64 q2,q3		\n\t"
			"vmov.64 q4,q5		\n\t"

			"vmov.64 q1,q6		\n\t"
			"vmov.64 q3,q7		\n\t"
			"vmov.64 q5,q8		\n\t"
		
			// End of Round#68





			//post-processing
			"vmov.64 d12, d2	\n\t"
			"vmov.64 d13, d5	\n\t"
			"vmov.64 d14, d9	\n\t"

			"vmov.64 d2, d1	\n\t"
			"vmov.64 d5, d6	\n\t"
			"vmov.64 d9, d10	\n\t"

			"vmov.64 d1,d12	\n\t"
			"vmov.64 d6,d13	\n\t"
			"vmov.64 d10,d14	\n\t"
//*/


/*
			//Debug&Test
			"vmov.64 q0,q6		\n\t"
			"vmov.64 q2,q7		\n\t"
			"vmov.64 q4,q8		\n\t"
//*/

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

