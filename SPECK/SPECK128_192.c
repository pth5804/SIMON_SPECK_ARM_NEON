
#include "SPECK128_192.h"
//#include <jni.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
//#include <cpu-features.h>
#include <arm_neon.h>


void SPECK128_192_NEON_ARM(uint64_t key[2 * 34], uint64_t text[2 * 8]) {

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

			"vldmia %1!,{d0-d15}	\n\t"

			//initialization
			"sub %1,%1,#128	\n\t" // #80 = 16*8(Q register * 7)

			//pre-processing
			"vmov.64 d20, d1	\n\t"
			"vmov.64 d21, d2	\n\t"
			"vmov.64 d22, d5	\n\t"
			"vmov.64 d23, d6	\n\t"
			"vmov.64 d24, d9	\n\t"
			"vmov.64 d25, d10	\n\t"
			"vmov.64 d26, d13	\n\t"
			"vmov.64 d27, d14	\n\t"

			"vmov.64 d1,d21	\n\t"
			"vmov.64 d2,d20	\n\t"
			"vmov.64 d5,d23	\n\t"
			"vmov.64 d6,d22	\n\t"
			"vmov.64 d9,d25	\n\t"
			"vmov.64 d10,d24	\n\t"
			"vmov.64 d13,d27	\n\t"
			"vmov.64 d14,d26	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"

			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"


			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"

			//End of Round#32
			
			//Round#1
			//ROR#alpha(8)(x)
			"vshl.u64 q10,q1,#56	\n\t"
			"vshl.u64 q11,q3,#56	\n\t"
			"vshl.u64 q12,q5,#56	\n\t"
			"vshl.u64 q13,q7,#56	\n\t"
	
			"vsri.u64 q10,q1,#8	\n\t"
			"vsri.u64 q11,q3,#8	\n\t"
			"vsri.u64 q12,q5,#8	\n\t"
			"vsri.u64 q13,q7,#8	\n\t"

			// Load Roundkey
			"vldmia %0!,{q15}	\n\t"

			//ROR#alpha(8)(x) + y
			"vadd.i64 q10,q10,q0	\n\t"
			"vadd.i64 q11,q11,q2	\n\t"
			"vadd.i64 q12,q12,q4	\n\t"
			"vadd.i64 q13,q13,q6	\n\t"


			//(ROR#alpha(8)(x) + y) ^ RK
			"veor q1, q10, q15	\n\t"
			"veor q3, q11, q15	\n\t"
			"veor q5, q12, q15	\n\t"
			"veor q7, q13, q15	\n\t"

			//ROL_beta(3)(y)
			"vshr.u64 q10,q0,#61	\n\t"
			"vshr.u64 q11,q2,#61	\n\t"
			"vshr.u64 q12,q4,#61	\n\t"
			"vshr.u64 q13,q6,#61	\n\t"
	
			"vsli.u64 q10,q0,#3	\n\t"
			"vsli.u64 q11,q2,#3	\n\t"
			"vsli.u64 q12,q4,#3	\n\t"
			"vsli.u64 q13,q6,#3	\n\t"

			//ROL_beta(3)(y) ^ ((ROR#alpha(8)(x) + y) ^ RK)
			"veor q0, q1, q10	\n\t"
			"veor q2, q3, q11	\n\t"
			"veor q4, q5, q12	\n\t"
			"veor q6, q7, q13	\n\t"

			//End of Round#33
			




			//post-processing
			"vmov.64 d20, d1	\n\t"
			"vmov.64 d21, d2	\n\t"
			"vmov.64 d22, d5	\n\t"
			"vmov.64 d23, d6	\n\t"
			"vmov.64 d24, d9	\n\t"
			"vmov.64 d25, d10	\n\t"
			"vmov.64 d26, d13	\n\t"
			"vmov.64 d27, d14	\n\t"

			"vmov.64 d1,d21	\n\t"
			"vmov.64 d2,d20	\n\t"
			"vmov.64 d5,d23	\n\t"
			"vmov.64 d6,d22	\n\t"
			"vmov.64 d9,d25	\n\t"
			"vmov.64 d10,d24	\n\t"
			"vmov.64 d13,d27	\n\t"
			"vmov.64 d14,d26	\n\t"



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

			"vstmia %1!, {d0-d15}		\n\t"

			
			/* */
			"vpop         {q4-q7}                 \n\t"
			"pop          {r4-r10}                 \n\t"
			:
			:"r"(&key[0]),"r"(&text[0])
			:
	);
	return;

}

