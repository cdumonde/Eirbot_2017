#include "Pin.hpp"

Pin::Pin(CN connector, uint8_t pin_index, MODE mode, PULL_RESISTOR resistor = NO_PULL, AF function = AF0, SPEED speed = VERY_HIGH_SPEED, OUTPUT_TYPE type = PUSH_PULL)
{
	error = 0;
	convert_to_Gpio(connector, pin_index);
	enableGPIO();
	setAlternate(function);
	setMode(mode);
	setSpeed(speed);
	setType(type);
	setPullResistor(resistor);
}
Pin::Pin(LED debug)
{
	error = 0;
	gpio_address = GPIOB;
	gpio = B;
	switch (debug)
	{
	case GREEN:
		gpio_index = 0;
		break;
	case BLUE:
		gpio_index = 7;
		break;
	case RED:
		gpio_index = 14;
		break;
	default:
		error = 1;
		gpio_index = 0;
		break;
	}
	setAlternate(AF0);
	setMode(OUTPUT);
	setSpeed(VERY_HIGH_SPEED);
	setType(PUSH_PULL);
	setPullResistor(NO_PULL);	
}
void Pin::enableGPIO()
{
	switch (gpio)
	{
	case A:
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
		break;
	case B:
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
		break;
	case C:
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
		break;
	case D:
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
		break;
	case E:
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
		break;
	case F:
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
		break;
	case G:
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
		break;
	case H:
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
		break;
	case I:
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN;
		break;
	case J:
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN;
		break;
	case K:
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOKEN;
		break;
	default:
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
		error = 1;
		break;
	}
}
void Pin::setAlternate(AF function)
{
	if (gpio_index > 7)
	{
		gpio_address->AFR[1] &= ~(0b1111 << (gpio_index - 8) * 4);
		gpio_address->AFR[1] |= ((uint8_t)function) << (gpio_index - 8) * 4;
	}
	else
	{
		gpio_address->AFR[0] &= ~(0b1111 << gpio_index * 4);
		gpio_address->AFR[0] |= ((uint8_t)function) << gpio_index * 4;
	}
	
}
void Pin::setMode(MODE mode)
{
	gpio_address->MODER &= ~(0b11 << 2 * gpio_index);
	switch (mode)
	{
	case INPUT:
		gpio_address->MODER |= 0b00 << 2 * gpio_index;
		break;
	case OUTPUT:
		gpio_address->MODER |= 0b01 << 2 * gpio_index;
		break;
	case ANALOG:
		gpio_address->MODER |= 0b11 << 2 * gpio_index;
		break;
	case ALTERNATE:
		gpio_address->MODER |= 0b10 << 2 * gpio_index;
		break;
	default:
		error = 1;
		gpio_address->MODER |= 0b01 << 2 * gpio_index;
		break;
	}
}
void Pin::setSpeed(SPEED speed)
{
	gpio_address->OSPEEDR &= ~(0b11 << 2 * gpio_index);
	switch (speed)
	{
	case LOW_SPEED:
		gpio_address->OSPEEDR |= 0b00 << 2 * gpio_index;
		break;
	case MEDIUM_SPEED:
		gpio_address->OSPEEDR |= 0b01 << 2 * gpio_index;
		break;
	case HIGH_SPEED:
		gpio_address->OSPEEDR |= 0b10 << 2 * gpio_index;
		break;
	case VERY_HIGH_SPEED:
		gpio_address->OSPEEDR |= 0b11 << 2 * gpio_index;
		break;
	default:
		error = 1;
		gpio_address->OSPEEDR |= 0b11 << 2 * gpio_index;
		break;
	}
}
void Pin::setType(OUTPUT_TYPE type)
{
	gpio_address->OTYPER &= ~(1 << gpio_index);
	switch (type)
	{
	case PUSH_PULL:
		gpio_address->OTYPER |= 0 << gpio_index;
		break;
	case OPEN_DRAIN:
		gpio_address->OTYPER |= 1 << gpio_index;
		break;
	default:
		error = 1;
		gpio_address->OTYPER |= 0 << gpio_index;
		break;
	}
}
void Pin::setPullResistor(PULL_RESISTOR resistor)
{
	gpio_address->PUPDR &= ~(1 << 2 * gpio_index);
	switch (resistor)
	{
	case NO_PULL:
		gpio_address->PUPDR |= 0b00 << 2 * gpio_index;
		break;
	case PULL_UP:
		gpio_address->PUPDR |= 0b01 << 2 * gpio_index;
		break;
	case PULL_DOWN:
		gpio_address->PUPDR |= 0b10 << 2 * gpio_index;
		break;
	default:
		error = 1;
		gpio_address->PUPDR |= 00 << 2 * gpio_index;
		break;
	}
}
void Pin::convert_to_Gpio(CN connector, uint8_t pin_index)
{
	switch (connector)
	{
	case CN7:
		gpio = cn7_port_map[pin_index];
		gpio_index = cn7_pin_map[pin_index];
		break;
	case CN8:
		gpio = cn8_port_map[pin_index];
		gpio_index = cn8_pin_map[pin_index];
		break;
	case CN9:
		gpio = cn9_port_map[pin_index];
		gpio_index = cn9_pin_map[pin_index];
		break;
	case CN10:
		gpio = cn10_port_map[pin_index];
		gpio_index = cn10_pin_map[pin_index];
		break;
	default:
		gpio = B;
		gpio_index = 0;
		break;
	}
	switch (gpio)
	{
	case A:
		gpio_address = GPIOA;
		break;
	case B:
		gpio_address = GPIOB;
		break;
	case C:
		gpio_address = GPIOC;
		break;
	case D:
		gpio_address = GPIOD;
		break;
	case E:
		gpio_address = GPIOE;
		break;
	case F:
		gpio_address = GPIOF;
		break;
	case G:
		gpio_address = GPIOG;
		break;
	case H:
		gpio_address = GPIOH;
		break;
	case I:
		gpio_address = GPIOI;
		break;
	case J:
		gpio_address = GPIOJ;
		break;
	case K:
		gpio_address = GPIOK;
		break;
	default:
		gpio_address = GPIOB;
		break;
	}
}