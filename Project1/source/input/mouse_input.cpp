#include "mouse_input.h"

bool first = true;

void mouse_input::motionHandler(int x, int y)
{
	ivec2 prev = getPosition();
	getPosition().x = x;
	getPosition().y = y;

	if (!first)
		getDelta() = getPosition() - prev;
	else first = false;
}

ivec2 &mouse_input::getPosition()
{
	static ivec2 pos(0);
	return pos;
}

ivec2 &mouse_input::getDelta()
{
	static ivec2 delta(0);
	return delta;
}
