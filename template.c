/*
 * STM32F107 - C template
 * Turns on GPIOD Pin 1
 */

/*************************************************
* Definitions
*************************************************/
// Define some types for readibility
#define int32_t         int
#define int16_t         short
#define int8_t          char
#define uint32_t        unsigned int
#define uint16_t        unsigned short
#define uint8_t         unsigned char

// Define the base addresses for RCC and GPIOD peripherals.
// You can calculate the addresses relative to periph_base,
// or directly assign them
// Base addresses can be found on page 49 in RM0008
#define PERIPH_BASE     ((uint32_t) 0x40000000)

#define GPIOD_BASE      (PERIPH_BASE + 0x11400) // GPIOD base address is 0x40011400
#define RCC_BASE        (PERIPH_BASE + 0x21000) //   RCC base address is 0x40021000

#define STACKINIT       0x20008000
#define LEDDELAY        800000

/*
 * Register Addresses
 * We will use the registers from RCC and GPIO, thus we created a struct to
 * bring together all the registers in a module. The list is in order.
 */
typedef struct {
	uint32_t CRL;      /* GPIO port configuration register low,      Address offset: 0x00 */
	uint32_t CRH;      /* GPIO port configuration register high,     Address offset: 0x04 */
	uint32_t IDR;      /* GPIO port input data register,             Address offset: 0x08 */
	uint32_t ODR;      /* GPIO port output data register,            Address offset: 0x0C */
	uint32_t BSRR;     /* GPIO port bit set/reset register,          Address offset: 0x10 */
	uint32_t BRR;      /* GPIO port bit reset register,              Address offset: 0x14 */
	uint32_t LCKR;     /* GPIO port configuration lock register,     Address offset: 0x18 */
} GPIO_type;

typedef struct {
	uint32_t CR;       /* RCC clock control register,                Address offset: 0x00 */
	uint32_t CFGR;     /* RCC clock configuration register,          Address offset: 0x04 */
	uint32_t CIR;      /* RCC clock interrupt register,              Address offset: 0x08 */
	uint32_t APB2RSTR; /* RCC APB2 peripheral reset register,        Address offset: 0x0C */
	uint32_t APB1RSTR; /* RCC APB1 peripheral reset register,        Address offset: 0x10 */
	uint32_t AHBENR;   /* RCC AHB peripheral clock enable register,  Address offset: 0x14 */
	uint32_t APB2ENR;  /* RCC APB2 peripheral clock enable register, Address offset: 0x18 */
	uint32_t APB1ENR;  /* RCC APB1 peripheral clock enable register, Address offset: 0x1C */
	uint32_t BDCR;     /* RCC backup domain control register,        Address offset: 0x20 */
	uint32_t CSR;      /* RCC control/status register,               Address offset: 0x24 */
	uint32_t AHBRSTR;  /* RCC AHB peripheral clock reset register,   Address offset: 0x28 */
	uint32_t CFGR2;    /* RCC clock configuration register2,         Address offset: 0x2C */
} RCC_type;

// Function declarations. Add your functions here
void copy_data(void);
void nmi_handler(void);
int32_t main(void);
void delay(volatile uint32_t s);

// This is what we will use to write to ports
// You can access to each register by using ->
// e.g. GPIOD->ODR
// Define one for each of the registers you want to use
#define GPIOD  ((GPIO_type  *) GPIOD_BASE)
#define RCC    ((RCC_type   *)   RCC_BASE)

/*************************************************
* Vector Table
*************************************************/
// Attribute puts table in beginning of .vector section
//   which is the beginning of .text section in the linker script
// Add other vectors in order here
// Vector table can be found on page 197 in RM0008
uint32_t (* const vector_table[])
__attribute__ ((section(".vectors"))) = {
	((uint32_t *) STACKINIT),          /* 0x00 Stack Pointer */
	((uint32_t *) main),               /* 0x04 Reset         */
	((uint32_t *) nmi_handler),        /* 0x08 NMI           */
	0,                                 /* 0x0C Hardfault     */
	0,                                 /* 0x10 MemManage     */
};

/*************************************************
* Copy the data contents from LMA to VMA
*************************************************/
void copy_data(void)
{
	extern char _etext, _sdata, _edata, _sbss, _ebss;
	char *src = &_etext;
	char *dst = &_sdata;

	/* ROM has data at end of text; copy it.  */
	while (dst < &_edata)
		*dst++ = *src++;

	/* Zero bss.  */
	for (dst = &_sbss; dst< &_ebss; dst++)
		*dst = 0;
}

// Non-maskable interrupt handler function
void nmi_handler(void)
{
	for (;;);  // Wait forever
}

/*************************************************
* Main code starts from here
*************************************************/
int32_t main(void)
{
	// Copy LMA to VMA for data section
	copy_data();
	// Set Bit 5 to enable GPIOD clock
	// APB2ENR: XXXX XXXX XXXX XXXX XXXX XXXX XX1X XXXX
	RCC->APB2ENR |= 0x00000020;

	// Another way to write a 1 to a bit location is to shift it that much
	// Meaning shift number 1, 5 times to the left. Which would result in
	// 0b100000 or 0x20
	// RCC->APB2ENR |= (1 << 5);

	// Since we need to modify 4 bits to make Pin 1 output (Pins 7-4)
	// We first need to AND it to reset them, then OR it to set them.
	// CRL should look like this: XXXX XXXX XXXX XXXX XXXX XXXX 0010 XXXX
	//                            pin7 pin6 pin5 pin4 pin3 pin2 pin1 pin0

	GPIOD->CRL &= 0xFFFFFF0F;   // Reset bits 4-7 to clear out previous values
	GPIOD->CRL |= 0x00000020;   // Set CNF bits to 00 and MODE bits to 10

	// You can do the same setup with shifting
	// GPIOD->CRL &= ~(0xF << 4); or GPIOD->CRL &= ~(0b1111 << 4);
	// GPIOD->CRL |=  (0x2 << 4); or GPIOD->CRL |=  (0b0010 << 4);

	// Set Pin 1 to 1 to turn on LED 1
	// ODR: XXXX XXXX XXXX XX1X
	GPIOD->ODR |= 0x0002;

	// You can do the same setup with shifting
	// GPIOD->ODR |= (1 << 1);

	while(1){
		delay(LEDDELAY);
	}

	__asm__("NOP"); // Assembly inline can be used if needed

	// Should never reach here
	return 0;
}

// A simple and not accurate delay function
void delay(volatile uint32_t s)
{
	for(s; s>0; s--){
		// Do nothing
	}
}
