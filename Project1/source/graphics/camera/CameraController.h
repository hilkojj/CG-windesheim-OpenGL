#pragma once

#include "PerspectiveCamera.h"

class CameraController
{

	bool droneMode = false;

	float yVelocity = 0;

public:

	float
		speedMultiplier = 50,
		mouseSensivity = 2,
		gravity = 2,
		minimumEyeHeight = 1.75,
		jumpForce = .5;

	void update(float deltaTime, PerspectiveCamera &);

};

