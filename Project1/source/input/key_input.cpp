#include "key_input.h"

#include <unordered_map>

enum status
{
	NOT_PRESSED,
	JUST_PRESSED,
	PRESSED,
	JUST_RELEASED
};

std::unordered_map<unsigned char, status> &getMap()
{
	static std::unordered_map<unsigned char, status> map;
	return map;
}

void key_input::keyPressedHandler(unsigned char key, int, int)
{
	getMap()[key] = JUST_PRESSED;
}

void key_input::keyUpHandler(unsigned char key, int, int)
{
	getMap()[key] = JUST_RELEASED;
}

void key_input::update()
{
	for (auto &entry : getMap())
	{
		if (entry.second == JUST_PRESSED) entry.second = PRESSED;
		else if (entry.second == JUST_RELEASED) entry.second = NOT_PRESSED;
	}
}

bool key_input::justPressed(unsigned char key)
{
	return getMap()[key] == JUST_PRESSED;
}

bool key_input::pressed(unsigned char key)
{
	auto &status = getMap()[key];
	return status == PRESSED || status == JUST_PRESSED;
}

bool key_input::justReleased(unsigned char key)
{
	return getMap()[key] == JUST_RELEASED;
}
