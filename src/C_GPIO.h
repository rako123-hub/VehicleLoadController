/*
 * CGPIO.h
 *
 *  Created on: 23.03.2024
 *      Author: rako
 */

#ifndef SRC_C_GPIO_H_
#define SRC_C_GPIO_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

//#define DEBUG_GPIO 1;
#define DEBUG_GPIO 0;

#define PAGE_SIZE  (4*1024)
#define BLOCK_SIZE (4*1024)

#define MAP_SIZE        (4096*2)
#define MAP_MASK        (MAP_SIZE - 1)

#define SETUP_OK           0
#define SETUP_DEVMEM_FAIL  1
#define SETUP_MALLOC_FAIL  2
#define SETUP_MMAP_FAIL    3
#define SETUP_CPUINFO_FAIL 4
#define SETUP_NOT_RPI_FAIL 5

#define SUNXI_R_GPIO_BASE			0x01F02000
#define SUNXI_R_GPIO_REG_OFFSET   	0xC00
#define SUNXI_GPIO_BASE				0x01C20000
#define SUNXI_GPIO_REG_OFFSET   	0x800
#define SUNXI_CFG_OFFSET			0x00
#define SUNXI_DATA_OFFSET			0x10
#define SUNXI_PUD_OFFSET			0x1C
#define SUNXI_BANK_SIZE				0x24

#define INPUT  1 // is really 0 for control register!
#define OUTPUT 0 // is really 1 for control register!
#define ALT0   4

//#define HIGH 1
//#define LOW  0

#define HIGH 0
#define LOW  1

#define PUD_OFF  0
#define PUD_DOWN 1
#define PUD_UP   2

#define GPIO_BANK(pin)  		((pin) >> 5)
#define GPIO_NUM(pin)   		((pin) & 0x1F)

#define GPIO_CFG_INDEX(pin)     (((pin) & 0x1F) >> 3)
#define GPIO_CFG_OFFSET(pin)    ((((pin) & 0x1F) & 0x7) << 2)

#define GPIO_PUL_INDEX(pin)     (((pin) & 0x1F )>> 4)
#define GPIO_PUL_OFFSET(pin)    (((pin) & 0x0F) << 1)

#define GPIO_0	276    	//Pin 11 Connector
#define GPIO_1  273		//Pin 12 Connector
#define GPIO_2  277		//Pin 13 Connector
#define GPIO_3  249		//Pin 15 Connector
#define GPIO_4  272		//Pin 16 Connector
#define GPIO_5  250		//Pin 18 Connector
#define GPIO_6  251		//Pin 22 Connector
#define GPIO_7   35		//Pin 7  Connector
#define GPIO_21 224		//Pin 29 Connector
#define GPIO_22 225		//Pin 31 Connector
#define GPIO_23 226		//Pin 33 Connector
#define GPIO_24 227		//Pin 35 Connector
#define GPIO_25 228		//Pin 37 Connector
#define GPIO_26 116		//Pin 32 Connector
#define GPIO_27 231		//Pin 36 Connector
#define GPIO_28 230		//Pin 38 Connector
#define GPIO_29 229		//Pin 40 Connector

#define L1 		GPIO_0
#define L2 		GPIO_1
#define L3 		GPIO_2
#define N  		GPIO_3
#define PWM_PIN GPIO_4

typedef struct sunxi_gpio {
    unsigned int CFG[4];
    unsigned int DAT;
    unsigned int DRV[2];
    unsigned int PULL[2];
} sunxi_gpio_t;

/* gpio interrupt control */
typedef struct sunxi_gpio_int {
    unsigned int CFG[3];
    unsigned int CTL;
    unsigned int STA;
    unsigned int DEB;
} sunxi_gpio_int_t;

typedef struct sunxi_gpio_reg {
    struct sunxi_gpio gpio_bank[9];
    unsigned char res[0xbc];
    struct sunxi_gpio_int gpio_int;
} sunxi_gpio_reg_t;

/*
static volatile uint32_t *gpio_map;
static volatile uint32_t *pio_map;
static volatile uint32_t *r_pio_map;
static volatile uint32_t *r_gpio_map;
*/

class C_GPIO {
public:
	C_GPIO();
	virtual ~C_GPIO();

public:
	void sunxi_setup_gpio(int gpio, int direction, int pud);
	int sunxi_gpio_function(int gpio);
	void sunxi_set_pullupdn(int gpio, int pud);
	int sunxi_input_gpio(int gpio);
	void sunxi_output_gpio(int gpio, int value);
	bool sunxi_setup_OK(){ return sunxi_mem_OK; }

private:
	int sunxi_setup(void);
	uint32_t sunxi_readl(volatile uint32_t *addr);
	void sunxi_writel(volatile uint32_t *addr, uint32_t val);
	void short_wait(void);
	void cleanup(void);

	volatile uint32_t *gpio_map;
	volatile uint32_t *pio_map;
	volatile uint32_t *r_pio_map;
	volatile uint32_t *r_gpio_map;

	bool sunxi_mem_OK = false;
};

#endif /* SRC_C_GPIO_H_ */
