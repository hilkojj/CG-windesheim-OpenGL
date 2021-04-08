#pragma once

#include "PerspectiveCamera.h"
#include <optional>

class CameraController
{

	bool droneMode = false;

	float yVelocity = 0;

	PerspectiveCamera *walkModeCam = NULL;

public:

	PerspectiveCamera *droneModeDefaultCam = NULL;

	float
		speedMultiplier = 50,
		mouseSensivity = 2,
		gravity = 2,
		minimumEyeHeight = 1.75,
		jumpForce = .5;

	void update(float deltaTime, PerspectiveCamera &);

	~CameraController();

};

