#ifndef GPIO_H
#define GPIO_H

int GPIO_SetupPWMMotor();
void GPIO_setPWMMotor(int value);
void GPIO_stopPWMMotor();
void GPIO_cleanup();

#endif //GPIO_H