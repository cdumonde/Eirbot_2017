#include "DigitalIO.hpp"

DigitalIO::DigitalIO(LED led)
{
	pin = new Pin(led);
	pin_mode = OUT;
	pin_state = LOW;
}
DigitalIO::DigitalIO(CN connector, uint8_t pin_index, TYPE type)
{
	pin = new Pin(connector, pin_index, (MODE)type);
	pin_mode = type;
	pin_state = LOW;
}
void DigitalIO::set(STATE state)
{
	if (pin_mode == OUT)
	{
		switch (state)
		{
		case LOW:
			pin->gpio_address->ODR &= ~(0x0001 << pin->gpio_index);
			break;
		default:
			pin->gpio_address->ODR |= 0x0001 << pin->gpio_index;
			break;
		}
		pin_state = state;
	}

}
STATE DigitalIO::get()
{
	if (pin_mode == IN)
	{
		return (STATE)((pin->gpio_address->IDR & (1 << pin->gpio_index)) != 0);
	}
	else
	{
		return pin_state;
	}
}

DigitalIO::~DigitalIO()
{
}
