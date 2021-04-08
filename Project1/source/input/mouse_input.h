#pragma once

#include "../utils/math.h"

namespace mouse_input
{

	// callback for GLUT
	void motionHandler(int x, int y);

	// position of cursor:
	ivec2 &getPosition();

	// difference in position since last frame:
	ivec2 &getDelta();


	// todo: add mouse button support, if needed.

}

