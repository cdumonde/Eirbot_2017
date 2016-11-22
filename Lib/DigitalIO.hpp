#include "Pin.hpp"

enum TYPE
{
	IN,
	OUT
};
enum STATE
{
	LOW,
	HIGH
};

class DigitalIO
{
public:
	DigitalIO(LED led);
	DigitalIO(CN connector, uint8_t pin_index, TYPE type);
	~DigitalIO();
	void set(STATE state);
	STATE get();

private:
	Pin *pin;
	TYPE pin_mode;
	STATE pin_state;
};
