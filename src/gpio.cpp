/*
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "gpio.h"

#define GPIO_BASE        0xFE200000  // Base address for GPIO registers on Raspberry Pi 4/5 (BCM2835)
#define BLOCK_SIZE       (4*1024)    // Size of memory block for mapping

// Offsets for GPIO registers
#define GPIO_FSEL0       0x00  // GPIO Function Select 0
#define GPIO_SET0        0x1C  // GPIO Pin Output Set 0
#define GPIO_CLR0        0x28  // GPIO Pin Output Clear 0
#define GPIO_PUD         0x94  // GPIO Pin Pull-up/down
#define GPIO_PUDCLK0     0x98  // GPIO Pin Pull-up/down Clock 0

// Offsets for PWM registers
#define PWM_BASE         0x3F20C000  // Base address for PWM
#define PWM_CTL          0x00  // PWM Control
#define PWM_RNG1         0x10  // PWM Range 1
#define PWM_DAT1         0x14  // PWM Data 1

volatile uint32_t *gpio;
volatile uint32_t *pwm;

void setup_gpio() {
    // Set GPIO 18 (pin 12) to output mode
    gpio[GPIO_FSEL0] = (gpio[GPIO_FSEL0] & ~(7 << 24)) | (1 << 24);  // Set GPIO18 to output (Function 1)
}

void setup_pwm() {
    // Set PWM1 range (for GPIO18)
    pwm[PWM_RNG1] = 1024;  // Set range (0-1024) for PWM1

    // Enable PWM1, with a basic mode (1) and turn it on
    pwm[PWM_CTL] = (1 << 0) | (1 << 8);
}

void set_pwm_duty_cycle(int duty_cycle) {
    // Set PWM1 duty cycle
    pwm[PWM_DAT1] = (duty_cycle * 1024) / 100;  // Convert percentage to range (0-1024)
}

void start_pwm() {
    // Set PWM signal HIGH (for GPIO18)
    gpio[GPIO_SET0] = (1 << 18);
}

void stop_pwm() {
    // Set PWM signal LOW (for GPIO18)
    gpio[GPIO_CLR0] = (1 << 18);
}

int GPIO_SetupPWMMotor(){
    // Open /dev/mem to access physical memory
    int mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (mem_fd < 0) {
        perror("Error opening /dev/mem");
        return -1;
    }

    // Map GPIO and PWM registers into memory
    gpio = (volatile uint32_t*)mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, GPIO_BASE);
    if (gpio == MAP_FAILED) {
        perror("Error mapping GPIO registers");
        close(mem_fd);
        return -1;
    }

    pwm = (volatile uint32_t*)mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, PWM_BASE);
    if (pwm == MAP_FAILED) {
        perror("Error mapping PWM registers");
        munmap((void*)gpio, BLOCK_SIZE);
        close(mem_fd);
        return -1;
    }

    // Close /dev/mem after mapping
    close(mem_fd);

    // Setup GPIO 18 as output and setup PWM
    setup_gpio();
    setup_pwm();

    return 0;
}

void GPIO_setPWMMotor(int value){

    // Set PWM duty cycle to 25%
    set_pwm_duty_cycle(value);

    // Start PWM signal on GPIO18
    start_pwm();
    printf("PWM signal running on GPIO 18 with %d%% duty cycle\n", value);
}
void GPIO_stopPWMMotor(){
    // Stop PWM signal
    stop_pwm();
}

void GPIO_cleanup(){
    // Cleanup
    munmap((void*)gpio, BLOCK_SIZE);
    munmap((void*)pwm, BLOCK_SIZE);
}
*/






// Access from ARM Running Linux

#define BCM2708_PERI_BASE        0xFE000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
void *gpio_map;

// I/O access
volatile unsigned *gpio;


// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

#define GPIO_PULL *(gpio+37) // Pull up/pull down
#define GPIO_PULLCLK0 *(gpio+38) // Pull up/pull down clock

void setup_io();

int GPIO_SetupPWMMotor()
{
  int g,rep;

  // Set up gpi pointer for direct register access
  setup_io();

  INP_GPIO(18);
  OUT_GPIO(18);

  GPIO_SET = 1 << 18;
    printf("PWM signal running\n");
  return 0;

} // main


//
// Set up a memory regions to access GPIO
//
void setup_io()
{
   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem \n");
      exit(-1);
   }

   /* mmap GPIO */
   gpio_map = mmap(
      NULL,             //Any adddress in our space will do
      BLOCK_SIZE,       //Map length
      PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
      MAP_SHARED,       //Shared with other processes
      mem_fd,           //File to map
      GPIO_BASE         //Offset to GPIO peripheral
   );

   close(mem_fd); //No need to keep mem_fd open after mmap

   if (gpio_map == MAP_FAILED) {
      printf("mmap error\n");//errno also set!
      exit(-1);
   }

   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;


} // setup_io
