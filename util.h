#define STM32F40XX
#include <stm32f4xx.h>

#define GPIO_INPUT 0
#define GPIO_OUTPUT 1
#define GPIO_ALT 2
#define GPIO_ANALOG 3
#define GPIO_NORMAL 0
#define GPIO_PUP 1
#define GPIO_PDN 2

void delay(uint32_t);
void setupPLL();
void setGPIOMode(GPIO_TypeDef*, uint8_t, uint8_t);
void setAFIO(GPIO_TypeDef*, uint8_t, uint8_t);
void setGPIO(GPIO_TypeDef*, uint8_t);
void resetGPIO(GPIO_TypeDef*, uint8_t);
void toggleGPIO(GPIO_TypeDef*, uint8_t);
void writeGPIO(GPIO_TypeDef*, uint8_t, uint8_t);
uint8_t readGPIO(GPIO_TypeDef*, uint8_t);
void setGPIOPullMode(GPIO_TypeDef*, uint8_t, uint8_t);
