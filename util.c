#include <util.h>

void delay(uint32_t w) {
	for(uint32_t i = 0; i < w; i++);
}

void setupPLL() {
	FLASH->ACR = 4;//More Flash wait state for faster clock
	//In = 8MHz, Out=144MHz, APB2=72MHz, APB1=36MHz, PLLQ=48MHz
	RCC->CFGR = (4 << 13) + (5 << 10); //PPRE2=4(/2), PPRE1=5(/4)
	//PLLN=144, PLLM=4, PLLSRC=1, PLLQ=3, PLLP=0(/2)
	RCC->PLLCFGR = (3 << 24) + (1 << 22) + (144 << 6) + 4;
	RCC->CR |= (1 << 24) + (1 << 16); //PLL + HSE En
	while((RCC->CR & (1 << 25)) == 0); //Wait for PLL
	RCC->CFGR |= 2; //Switch to PLL
}

//Set GPIO b's pin to mode m
void setGPIOMode(GPIO_TypeDef *b, uint8_t pin, uint8_t m) {
	uint8_t shifter = pin * 2;
	b->MODER = b->MODER & ~(3 << shifter);
	b->MODER |= b->MODER + (m << shifter);
}

//Set GPIO b's pin to AFIO num
void setAFIO(GPIO_TypeDef *b, uint8_t pin, uint8_t num) {
	setGPIOMode(b, pin, GPIO_ALT);
	uint8_t shifter = (pin % 8) * 4;
	if(pin < 8) {
		b->AFR[0] = b->AFR[0] & ~(0xf << shifter);
		b->AFR[0] = b->AFR[0] + (num << shifter);
	} else {
		b->AFR[1] = b->AFR[1] & ~(0xf << shifter);
		b->AFR[1] = b->AFR[1] + (num << shifter);
	}
}

//Set GPIO b's pin
void setGPIO(GPIO_TypeDef *b, uint8_t pin) {
	b->BSRRL = 1 << pin;
}

//Reset GPIO b's pin
void resetGPIO(GPIO_TypeDef *b, uint8_t pin) {
	b->BSRRH = 1 << pin;
}

//Toggle GPIO b's pin
void toggleGPIO(GPIO_TypeDef *b, uint8_t pin) {
	b->ODR = b->ODR ^ (1 << pin);
}

//Set GPIO b's pin to state
void writeGPIO(GPIO_TypeDef *b, uint8_t pin, uint8_t state) {
	if(state == 1) {
		b->BSRRL = 1 << pin;
	} else {
		b->BSRRH = 1 << pin;
	}
}

//Read GPIO b's pin state
uint8_t readGPIO(GPIO_TypeDef *b, uint8_t pin) {
	if((b->IDR & (1 << pin)) == 0) { return 0; }
	return 1;
}

//Set GPIO b's pin pull mode to mode m
void setGPIOPullMode(GPIO_TypeDef *b, uint8_t pin, uint8_t m) {
	uint8_t shifter = pin * 2;
	b->PUPDR &= ~(1 << shifter);
	b->PUPDR |= (m << shifter);
} 

//Init USART1 for 2Mbaud at 72MHz
void U1init() {
	RCC->APB2ENR |= 0x10; //UART1 On
	USART1->BRR = 0x24; //2.25 (2MBaud @ 72MHz)
	USART1->CR1 = 0x200c; //USART1 Enable, TX and RX Enable
}

//Send string d via USART1
void U1print(char *d) {
	uint32_t i = 0;
	while(d[i]) {
		U1putch(d[i]);
		i++;
	}
}

//Send 1 letter via USART1
void U1putch(char d) {
	USART1->DR = d; 
	while((USART1->SR & 0x80) == 0); //wait for TXE=1 (transfer complete)
}
