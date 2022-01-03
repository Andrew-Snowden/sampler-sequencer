#include <stdint.h>

static volatile uint32_t * GPIOA_MODER_D 	= (uint32_t *) 0x50002000;
static volatile uint32_t * GPIOA_ODR_D		= (uint32_t *) 0x50002014;
static volatile uint32_t * GPIOA_IDR_D		= (uint32_t *) 0x50002010;
static volatile uint32_t * GPIOA_PUPD_D		= (uint32_t *) 0x5000200C;
static volatile uint32_t * GPIOA_OSPEED_D 	= (uint32_t *) 0x50002008;
static volatile uint32_t * GPIOA_OTYPER_D	= (uint32_t *) 0x50002004;
static volatile uint32_t * GPIOA_AFRL_D		= (uint32_t *) 0x50002020;
static volatile uint32_t * GPIOA_AFRH_D		= (uint32_t *) 0x50002024;
static volatile uint32_t * GPIOA_BSRR_D		= (uint32_t *) 0x50002018;
static volatile uint32_t * GPIOA_BRR_D		= (uint32_t *) 0x50002028;
static volatile uint32_t * GPIOA_LCKR_D		= (uint32_t *) 0x5000201C;

#define 	GPIOA_MODER		*GPIOA_MODER_D
#define		GPIOA_ODR		*GPIOA_ODR_D
#define		GPIOA_IDR		*GPIOA_IDR_D
#define		GPIOA_PUPDR		*GPIOA_PUPD_D
#define		GPIOA_OSPEEDR	*GPIOA_OSPEED_D
#define		GPIOA_OTYPER	*GPIOA_OTYPER_D
#define		GPIOA_AFRL		*GPIOA_AFRL_D
#define		GPIOA_AFRH		*GPIOA_AFRH_D
#define		GPIOA_BSRR		*GPIOA_BSRR_D
#define 	GPIOA_BRR		*GPIOA_BRR_D
#define		GPIOA_LCKR		*GPIOA_LCKR_D

#define 	SET(REG, VALUE) REG |= (VALUE)
#define 	CLR(REG, VALUE)	REG &= ~(VALUE)

#define 	    MODER0_MASK		0x00000003
#define		    MODER1_MASK		0x0000000C
#define 	    MODER2_MASK		0x00000030
#define		    MODER3_MASK		0x000000C0
#define		    MODER4_MASK		0x00000300
#define		    MODER5_MASK		0x00000C00
#define		    MODER6_MASK		0x00003000
#define		    MODER7_MASK		0x0000C000
#define		    MODER8_MASK		0x00030000
#define		    MODER9_MASK		0x000C0000
#define		    MODER10_MASK		0x00300000
#define		    MODER11_MASK		0x00C00000
#define		    MODER12_MASK		0x03000000
#define		    MODER13_MASK		0x0C000000
#define		    MODER14_MASk		0x30000000
#define		    MODER15_MASK		0xC0000000

#define 	    MODER0_POS		0
#define		    MODER1_POS		2
#define 	    MODER2_POS		4
#define		    MODER3_POS		6
#define		    MODER4_POS		8
#define		    MODER5_POS		10
#define		    MODER6_POS		12
#define		    MODER7_POS		14
#define		    MODER8_POS		16
#define		    MODER9_POS		18
#define		    MODER10_POS		20
#define		    MODER11_POS		22
#define		    MODER12_POS		24
#define		    MODER13_POS		26
#define		    MODER14_POS		28
#define		    MODER15_POS		30


#define 	    OTYPER0_MASK		0x00000001
#define		    OTYPER1_MASK		0x00000002
#define		    OTYPER2_MASK		0x00000004
#define		    OTYPER3_MASK		0x00000008
#define		    OTYPER4_MASK		0x00000010
#define		    OTYPER5_MASK		0x00000020
#define		    OTYPER6_MASK		0x00000040
#define		    OTYPER7_MASK		0x00000080
#define		    OTYPER8_MASK		0x00000100
#define		    OTYPER9_MASK		0x00000200
#define		    OTYPER10_MASK		0x00000400
#define		    OTYPER11_MASK		0x00000800
#define		    OTYPER12_MASK		0x00001000
#define		    OTYPER13_MASK		0x00002000
#define		    OTYPER14_MASK		0x00004000
#define		    OTYPER15_MASK		0x00008000

#define 	    OSPEEDR0_MASK		0x00000003
#define		    OSPEEDR1_MASK		0x0000000C
#define 	    OSPEEDR2_MASK		0x00000030
#define		    OSPEEDR3_MASK		0x000000C0
#define		    OSPEEDR4_MASK		0x00000300
#define		    OSPEEDR5_MASK		0x00000C00
#define		    OSPEEDR6_MASK		0x00003000
#define		    OSPEEDR7_MASK		0x0000C000
#define		    OSPEEDR8_MASK		0x00030000
#define		    OSPEEDR9_MASK		0x000C0000
#define		    OSPEEDR10_MASK	0x00300000
#define		    OSPEEDR11_MASK	0x00C00000
#define		    OSPEEDR12_MASK	0x03000000
#define		    OSPEEDR13_MASK	0x0C000000
#define		    OSPEEDR14_MASk	0x30000000
#define		    OSPEEDR15_MASK	0xC0000000

#define 	    OSPEEDR0_POS		0
#define		    OSPEEDR1_POS		2
#define 	    OSPEEDR2_POS		4
#define		    OSPEEDR3_POS		6
#define		    OSPEEDR4_POS		8
#define		    OSPEEDR5_POS		10
#define		    OSPEEDR6_POS		12
#define		    OSPEEDR7_POS		14
#define		    OSPEEDR8_POS		16
#define		    OSPEEDR9_POS		18
#define		    OSPEEDR10_POS		20
#define		    OSPEEDR11_POS		22
#define		    OSPEEDR12_POS		24
#define		    OSPEEDR13_POS		26
#define		    OSPEEDR14_POS		28
#define		    OSPEEDR15_POS		30

#define 	    PUPDR0_MASK		0x00000003
#define		    PUPDR1_MASK		0x0000000C
#define 	    PUPDR2_MASK		0x00000030
#define		    PUPDR3_MASK		0x000000C0
#define		    PUPDR4_MASK		0x00000300
#define		    PUPDR5_MASK		0x00000C00
#define		    PUPDR6_MASK		0x00003000
#define		    PUPDR7_MASK		0x0000C000
#define		    PUPDR8_MASK		0x00030000
#define		    PUPDR9_MASK		0x000C0000
#define		    PUPDR10_MASK		0x00300000
#define		    PUPDR11_MASK		0x00C00000
#define		    PUPDR12_MASK		0x03000000
#define		    PUPDR13_MASK		0x0C000000
#define		    PUPDR14_MASk		0x30000000
#define		    PUPDR15_MASK		0xC0000000

#define 	    PUPDR0_POS		0
#define		    PUPDR1_POS		2
#define 	    PUPDR2_POS		4
#define		    PUPDR3_POS		6
#define		    PUPDR4_POS		8
#define		    PUPDR5_POS		10
#define		    PUPDR6_POS		12
#define		    PUPDR7_POS		14
#define		    PUPDR8_POS		16
#define		    PUPDR9_POS		18
#define		    PUPDR10_POS		20
#define		    PUPDR11_POS		22
#define		    PUPDR12_POS		24
#define		    PUPDR13_POS		26
#define		    PUPDR14_POS		28
#define		    PUPDR15_POS		30

#define 	    IDR0_MASK		0x00000001
#define		    IDR1_MASK		0x00000002
#define		    IDR2_MASK		0x00000004
#define		    IDR3_MASK		0x00000008
#define		    IDR4_MASK		0x00000010
#define		    IDR5_MASK		0x00000020
#define		    IDR6_MASK		0x00000040
#define		    IDR7_MASK		0x00000080
#define		    IDR8_MASK		0x00000100
#define		    IDR9_MASK		0x00000200
#define		    IDR10_MASK	0x00000400
#define		    IDR11_MASK	0x00000800
#define		    IDR12_MASK	0x00001000
#define		    IDR13_MASK	0x00002000
#define		    IDR14_MASK	0x00004000
#define		    IDR15_MASK	0x00008000

#define 	    ODR0_MASK		0x00000001
#define		    ODR1_MASK		0x00000002
#define		    ODR2_MASK		0x00000004
#define		    ODR3_MASK		0x00000008
#define		    ODR4_MASK		0x00000010
#define		    ODR5_MASK		0x00000020
#define		    ODR6_MASK		0x00000040
#define		    ODR7_MASK		0x00000080
#define		    ODR8_MASK		0x00000100
#define		    ODR9_MASK		0x00000200
#define		    ODR10_MASK	0x00000400
#define		    ODR11_MASK	0x00000800
#define		    ODR12_MASK	0x00001000
#define		    ODR13_MASK	0x00002000
#define		    ODR14_MASK	0x00004000
#define		    ODR15_MASK	0x00008000

#define 	    BSRR0_MASK	0x00000001
#define		    BSRR1_MASK	0x00000002
#define		    BSRR2_MASK	0x00000004
#define		    BSRR3_MASK	0x00000008
#define		    BSRR4_MASK	0x00000010
#define		    BSRR5_MASK	0x00000020
#define		    BSRR6_MASK	0x00000040
#define		    BSRR7_MASK	0x00000080
#define		    BSRR8_MASK	0x00000100
#define		    BSRR9_MASK	0x00000200
#define		    BSRR10_MASK	0x00000400
#define		    BSRR11_MASK	0x00000800
#define		    BSRR12_MASK	0x00001000
#define		    BSRR13_MASK	0x00002000
#define		    BSRR14_MASK	0x00004000
#define		    BSRR15_MASK	0x00008000

#define 	    BRR0_MASK		0x00000001
#define		    BRR1_MASK		0x00000002
#define		    BRR2_MASK		0x00000004
#define		    BRR3_MASK		0x00000008
#define		    BRR4_MASK		0x00000010
#define		    BRR5_MASK		0x00000020
#define		    BRR6_MASK		0x00000040
#define		    BRR7_MASK		0x00000080
#define		    BRR8_MASK		0x00000100
#define		    BRR9_MASK		0x00000200
#define		    BRR10_MASK	0x00000400
#define		    BRR11_MASK	0x00000800
#define		    BRR12_MASK	0x00001000
#define		    BRR13_MASK	0x00002000
#define		    BRR14_MASK	0x00004000
#define		    BRR15_MASK	0x00008000

#define 	    LCKR0_MASK	0x00000001
#define		    LCKR1_MASK	0x00000002
#define		    LCKR2_MASK	0x00000004
#define		    LCKR3_MASK	0x00000008
#define		    LCKR4_MASK	0x00000010
#define		    LCKR5_MASK	0x00000020
#define		    LCKR6_MASK	0x00000040
#define		    LCKR7_MASK	0x00000080
#define		    LCKR8_MASK	0x00000100
#define		    LCKR9_MASK	0x00000200
#define		    LCKR10_MASK	0x00000400
#define		    LCKR11_MASK	0x00000800
#define		    LCKR12_MASK	0x00001000
#define		    LCKR13_MASK	0x00002000
#define		    LCKR14_MASK	0x00004000
#define		    LCKR15_MASK	0x00008000
#define		    LCKR16_MASK	0x00010000

#define		    AFRL0_MASK	0x0000000F
#define		    AFRL1_MASK	0x000000F0
#define		    AFRL2_MASK	0x00000F00
#define		    AFRL3_MASK	0x0000F000
#define		    AFRL4_MASK	0x000F0000
#define		    AFRL5_MASK	0x00F00000
#define		    AFRL6_MASK	0x0F000000
#define		    AFRL7_MASK	0xF0000000
#define		    AFRH8_MASK	0x0000000F
#define		    AFRH9_MASK	0x000000F0
#define		    AFRH10_MASK	0x00000F00
#define		    AFRH11_MASK	0x0000F000
#define		    AFRH12_MASK	0x000F0000
#define		    AFRH13_MASK	0x00F00000
#define		    AFRH14_MASK	0x0F000000
#define		    AFRH15_MASK	0xF0000000

#define		    AFRL0_POS		0
#define		    AFRL1_POS		4
#define		    AFRL2_POS		8
#define		    AFRL3_POS		12
#define		    AFRL4_POS		16
#define		    AFRL5_POS		20
#define		    AFRL6_POS		24
#define		    AFRL7_POS		28
#define		    AFRH8_POS		0
#define		    AFRH9_POS		4
#define		    AFRH10_POS	8
#define		    AFRH11_POS	12
#define		    AFRH12_POS	16
#define		    AFRH13_POS	20
#define		    AFRH14_POS	24
#define		    AFRH15_POS	28















