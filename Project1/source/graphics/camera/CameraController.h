#pragma once

#include "PerspectiveCamera.h"

class CameraController
{

public:

	float speedMultiplier = 1, mouseSensivity = 1;

	void update(float deltaTime, PerspectiveCamera &);

};
