#pragma once

#include "PerspectiveCamera.h"

class CameraController
{

public:

	float speedMultiplier = 1, mouseSensivity = 2;

	void update(float deltaTime, PerspectiveCamera &);

};

