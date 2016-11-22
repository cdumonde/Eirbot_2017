#ifndef PIN_HPP
#define PIN_HPP

#include <stm32f4xx.h>

enum GPIO
{
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	ERR
};
enum CN
{
	CN7,
	CN8,
	CN9,
	CN10
};
enum AF
{
	AF0,
	AF1,
	AF2,
	AF3,
	AF4,
	AF5,
	AF6,
	AF7,
	AF8,
	AF9,
	AF10,
	AF11,
	AF12,
	AF13,
	AF14,
	Af15
};
enum MODE
{
	INPUT,
	OUTPUT,
	ANALOG,
	ALTERNATE
};
enum SPEED
{
	LOW_SPEED,
	MEDIUM_SPEED,
	HIGH_SPEED,
	VERY_HIGH_SPEED
};
enum OUTPUT_TYPE
{
	PUSH_PULL,
	OPEN_DRAIN
};
enum PULL_RESISTOR
{
	NO_PULL,
	PULL_UP,
	PULL_DOWN
};
enum LED
{
	GREEN,
	BLUE,
	RED
};

class Pin
{
public:
	Pin(CN connector, uint8_t pin_index, MODE mode, PULL_RESISTOR resistor = NO_PULL, AF function = AF0, SPEED speed = HIGH_SPEED, OUTPUT_TYPE type = OPEN_DRAIN);
	Pin(LED debug);
	~Pin();
	GPIO_TypeDef* gpio_address;
	uint8_t gpio_index;
	uint8_t error;


private:

	void enableGPIO();
	void setAlternate(AF function);
	void setMode(MODE mode);
	void setSpeed(SPEED speed);
	void setType(OUTPUT_TYPE type);
	void setPullResistor(PULL_RESISTOR resistor);
	void convert_to_Gpio(CN connector, uint8_t pin_index);
	GPIO gpio;
	uint8_t pin_index;


	GPIO cn7_port_map[20] = { C, B, B, B, B, ERR, B, ERR, A, A, C, A, B, A, B, D, A, D, B, F };
	GPIO cn8_port_map[16] = { ERR, C, ERR, C, ERR, C, ERR, C, ERR, C, ERR, D, ERR, G, ERR, G };
	GPIO cn9_port_map[30] = { A, D, C, D, C, D, F, D, F, D, F, ERR, ERR, E, A, E, F, E, F, E, F, E, ERR, F, D, F, D, F, G, G };
	GPIO cn10_port_map[34] = { ERR, F, ERR, E, ERR, E, B, F, C, E, F, F, B, G, B, G, ERR, E, D, E, D, ERR, D, E, E, E, ERR, E, A, E, B, B, E, B };

	uint8_t cn7_pin_map[20] = { 6, 8, 15, 9, 13, ERR, 12, ERR, 15, 5, 7, 6, 5, 7, 3, 14, 4, 15, 4, 12 };
	uint8_t cn8_pin_map[16] = { ERR, 8, ERR, 9, ERR, 10, ERR, 11, ERR, 12, ERR, 2, ERR, 2, ERR, 3 };
	uint8_t cn9_pin_map[30] = { 3, 7, 0, 6, 3, 5, 3, 4, 5, 3, 10, ERR, ERR, 2, 7, 4, 2, 5, 1, 6, 0, 3, ERR, 8, 0, 7, 1, 9, 0, 1 };
	uint8_t cn10_pin_map[34] = { ERR, 13, ERR, 9, ERR, 11, 1, 14, 2, 13, 4, 15, 6, 14, 2, 9, ERR, 8, 13, 7, 12, ERR, 11, 10, 2, 12, ERR, 14, 0, 15, 0, 10, 0, 11 };
};

#endif // !PIN_HPP
