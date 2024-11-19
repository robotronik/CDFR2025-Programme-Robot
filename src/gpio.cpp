/*
    GPIO access on RP1 through PCI BAR1
    2024 Feb
    Praktronics
    GPL3
    

    run with sudo or as root for permissions to access to /dev/mem
    this file is self-contained

*/


#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "logger.hpp"

void delay_ms(int milliseconds)
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}

// pci bar info
// from: https://github.com/G33KatWork/RP1-Reverse-Engineering/blob/master/pcie/hacks.py
#define RP1_BAR1 0x1f00000000
#define RP1_BAR1_LEN 0x400000

// offsets from include/dt-bindings/mfd/rp1.h
// https://github.com/raspberrypi/linux/blob/rpi-6.1.y/include/dt-bindings/mfd/rp1.h
#define RP1_IO_BANK0_BASE 0x0d0000
#define RP1_RIO0_BASE 0x0e0000
#define RP1_PADS_BANK0_BASE 0x0f0000

// the following info is from the RP1 datasheet (draft & incomplete as of 2024-02-18)
// https://datasheets.raspberrypi.com/rp1/rp1-peripherals.pdf
#define RP1_ATOM_XOR_OFFSET 0x1000
#define RP1_ATOM_SET_OFFSET 0x2000
#define RP1_ATOM_CLR_OFFSET 0x3000

#define PADS_BANK0_VOLTAGE_SELECT_OFFSET 0
#define PADS_BANK0_GPIO_OFFSET 0x4

#define RIO_OUT_OFFSET 0x00
#define RIO_OE_OFFSET 0x04
#define RIO_NOSYNC_IN_OFFSET 0x08
#define RIO_SYNC_IN_OFFSET 0x0C
//                           3         2         1
//                          10987654321098765432109876543210
#define CTRL_MASK_FUNCSEL 0b00000000000000000000000000011111
#define PADS_MASK_OUTPUT  0b00000000000000000000000011000000

#define CTRL_FUNCSEL_RIO 0x05


void *mapgpio(off_t dev_base, off_t dev_size)
{
    int fd;
    void *mapped;
    
    printf("sizeof(off_t) %d\n", sizeof(off_t));

    if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1)
    {
        printf("Can't open /dev/mem\n");
        return (void *)0;
    }

    mapped = mmap(0, dev_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, dev_base);
    // close(fd);

    printf("base address: %llx, size: %x, mapped: %p\n", dev_base, dev_size, mapped);

    if (mapped == (void *)-1)
    {
        printf("Can't map the memory to user space.\n");
        return (void *)0;
    }

    return mapped;
}


typedef struct
{
    uint8_t number;
    volatile uint32_t *status;
    volatile uint32_t *ctrl;
    volatile uint32_t *pad;
} gpio_pin_t;

typedef struct
{
    volatile void *rp1_peripherial_base;
    volatile void *gpio_base;
    volatile void *pads_base;
    volatile uint32_t *rio_out;
    volatile uint32_t *rio_output_enable;
    volatile uint32_t *rio_nosync_in;

    gpio_pin_t *pins[27];

} rp1_t;

static rp1_t *rp1;


bool create_rp1()
{
    rp1_t *r = (rp1_t *)calloc(1, sizeof(rp1_t));
    if (r == NULL)
        return false;

    void *base = mapgpio(RP1_BAR1, RP1_BAR1_LEN);
    if (base == NULL)
        return false;

    r->rp1_peripherial_base = base;
    r->gpio_base = base + RP1_IO_BANK0_BASE;
    r->pads_base = base + RP1_PADS_BANK0_BASE;
    r->rio_out = (volatile uint32_t *)(base + RP1_RIO0_BASE + RIO_OUT_OFFSET);
    r->rio_output_enable = (volatile uint32_t *)(base + RP1_RIO0_BASE + RIO_OE_OFFSET);
    r->rio_nosync_in = (volatile uint32_t *)(base + RP1_RIO0_BASE + RIO_NOSYNC_IN_OFFSET);

    rp1 = r;

    return true;
}

bool create_pin(uint8_t pinnumber)
{
    gpio_pin_t *newpin = (gpio_pin_t*)calloc(1, sizeof(gpio_pin_t));
    if(newpin == NULL) return false;

    newpin->number = pinnumber;

    // each gpio has a status and control register
    // adjacent to each other. control = status + 4 (uint8_t)
    newpin->status = (uint32_t *)(rp1->gpio_base + 8 * pinnumber);
    newpin->ctrl = (uint32_t *)(rp1->gpio_base + 8 * pinnumber + 4);
    newpin->pad = (uint32_t *)(rp1->pads_base + PADS_BANK0_GPIO_OFFSET + pinnumber * 4);

    // set the function
    *(newpin->ctrl + RP1_ATOM_CLR_OFFSET / 4) = CTRL_MASK_FUNCSEL; // first clear the bits
    *(newpin->ctrl + RP1_ATOM_SET_OFFSET / 4) = CTRL_FUNCSEL_RIO;  // now set the value we need

    rp1->pins[pinnumber] = newpin;
    LOG_DEBUG("pin ", pinnumber, " stored in pins array ", rp1->pins[pinnumber]);

    return true;
}

int pin_enable_output(uint8_t pinnumber)
{

    LOG_DEBUG("GPIO - Attempting to enable output");
    //LOG_DEBUG("rp1 @ %p\n", rp1);
    //LOG_DEBUG("pin @ %p\n", rp1->pins[pinnumber]);

    //gpio_pin_t *pin = NULL;
    //LOG_DEBUG("pin: %p\n", pin);

    //pin = rp1->pins[pinnumber];

    //LOG_DEBUG("pin: %p\n", pin);
    //LOG_DEBUG("pin pads %p\n", pin->pad);
    //LOG_DEBUG("oe: %p\n", rp1->rio_output_enable);

    // first enable the pad to output
    // pads needs to have OD[7] -> 0 (don't disable output)
    // and                IE[6] -> 0 (don't enable input)
    // we use atomic access to the bit clearing alias with a mask
    // divide the offset by 4 since we're doing uint32* math

    volatile uint32_t *writeadd = rp1->pins[pinnumber]->pad + RP1_ATOM_CLR_OFFSET / 4;

    LOG_DEBUG("GPIO - Attempting write for ", rp1->pins[pinnumber]->pad, " at ", writeadd);

    *writeadd = PADS_MASK_OUTPUT;

    // now set the RIO output enable using the atomic set alias
    *(rp1->rio_output_enable + RP1_ATOM_SET_OFFSET / 4) = 1 << rp1->pins[pinnumber]->number;

    return 0;
}

void pin_on(uint8_t pin)
{
    *(rp1->rio_out + RP1_ATOM_SET_OFFSET / 4) = 1 << pin;
}
void pin_off(uint8_t pin)
{
    *(rp1->rio_out + RP1_ATOM_CLR_OFFSET / 4) = 1 << pin;
}
void print_gpio_status(){
    // let's see if we can dump the iobank registers
    uint32_t *p;

    int i;
    for (i = 0; i < 27; i++)
    {
        p = (uint32_t *)(rp1->gpio_base + i * 8);
        LOG_DEBUG("gpio ", i, " status @ p = ", *p, ", ctrl @ p = ", *(p + 1));
    }
}



#define GPIO_MOTOR_PIN 18

// PWM frequency and duty cycle (percentage)
#define PWM_FREQUENCY_HZ 20000   // 20 kHz PWM frequency
#define PWM_DUTY_CYCLE 25       // 20% duty cycle

// Variables to track PWM state
static int pwm_state = 0; // 0 for OFF, 1 for ON

// Timer interval in microseconds for PWM frequency
unsigned long pwm_period_us = 1000000 / PWM_FREQUENCY_HZ;  // in microseconds
unsigned long pwm_high_time_us = pwm_period_us * PWM_DUTY_CYCLE / 100; // ON time in microseconds
unsigned long pwm_low_time_us = pwm_period_us - pwm_high_time_us;  // OFF time in microseconds

static timer_t timerid;

static struct itimerspec ts_high = {
    .it_interval = { .tv_sec = 0, .tv_nsec = 0 } // No periodic interval
};
static struct itimerspec ts_low = {
    .it_interval = { .tv_sec = 0, .tv_nsec = 0 } // No periodic interval
};

// Timer callback function to toggle PWM
void timer_thread_callback(union sigval sv) {
    // Toggle the pin state based on the current PWM state
    if (pwm_state == 0) {
        ts_high.it_value.tv_nsec = pwm_high_time_us * 1000;
        ts_high.it_value.tv_sec = 0;
        timer_settime(timerid, 0, &ts_high, NULL);
        // Pin ON: Use high duration
        pin_on(GPIO_MOTOR_PIN);
        pwm_state = 1;
    } else {
        ts_low.it_value.tv_nsec = pwm_low_time_us * 1000;
        ts_low.it_value.tv_sec = 0;
        timer_settime(timerid, 0, &ts_low, NULL);
        // Pin OFF: Use low duration
        pin_off(GPIO_MOTOR_PIN);
        pwm_state = 0;
    }
}


int setup_pwm_timer() {
    struct sigevent sev;
    struct itimerspec ts;

    // Configure the timer to use a thread for notifications
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_value.sival_ptr = &timerid;
    sev.sigev_notify_function = timer_thread_callback;
    sev.sigev_notify_attributes = NULL;  // Use default thread attributes

    // Create a timer using CLOCK_MONOTONIC for high precision
    if (timer_create(CLOCK_MONOTONIC, &sev, &timerid) == -1) {
        perror("timer_create failed");
        return -1;
    }

    // Set the initial timer to trigger 
    ts_high.it_value.tv_nsec = pwm_high_time_us * 1000;
    ts_high.it_value.tv_sec = 0;
    ts_low.it_value.tv_nsec = pwm_low_time_us * 1000;
    ts_low.it_value.tv_sec = 0;
    ts_high.it_interval.tv_sec = 0;  // No periodic interval
    ts_high.it_interval.tv_nsec = 0;
    ts_low.it_interval.tv_sec = 0;  // No periodic interval
    ts_low.it_interval.tv_nsec = 0;
    if (timer_settime(timerid, 0, &ts_low, NULL) == -1) {
        LOG_ERROR("GPIO PWM - timer_settime failed");
        return 3;
    }

    LOG_DEBUG("GPIO PWM - High ", pwm_high_time_us, "us, Low ", pwm_low_time_us, "us");
    return 0;
}



int GPIO_SetupPWMMotor(){

    // create a rp1 device
    LOG_DEBUG("GPIO - Creating rp1");
    if (!create_rp1())
    {
        LOG_ERROR("GPIO - Unable to create rp1");
        return 2;
    }
    // Optionnal
    // print_gpio_status();


    LOG_DEBUG("GPIO - Creating pins");

    if(!create_pin(GPIO_MOTOR_PIN)) {
        LOG_ERROR("GPIO - Unable to create pin ", GPIO_MOTOR_PIN);
        return 3;
    };
    pin_enable_output(GPIO_MOTOR_PIN);

    // pin_on(GPIO_MOTOR_PIN);
    // pin_off(GPIO_MOTOR_PIN);

    return setup_pwm_timer();
}


void GPIO_stopPWMMotor(){

    // Disable the timer (stop PWM generation)
    if (timer_delete(timerid) == -1) {
        LOG_ERROR("GPIO PWM - timer_delete failed");
        return;
    }

    pin_off(GPIO_MOTOR_PIN);

    // TODO : SEE DOCS ABOUT RP1
    // Stop PWM signal
    // stop_pwm();
}