#include "CameraController.h"
#include "../../input/key_input.h"
#include "../../input/mouse_input.h"

#include <GL/freeglut.h>

void CameraController::update(float deltaTime, PerspectiveCamera &cam)
{
    // move using keyboard:
    {
        vec3 forward = cam.direction;
        forward.y = 0;
        forward = normalize(forward);

        // forward, backward
        if (key_input::pressed('w'))
            cam.position += forward * deltaTime * speedMultiplier;
        if (key_input::pressed('s'))
            cam.position += -forward * deltaTime * speedMultiplier;

        // right, left
        if (key_input::pressed('d'))
            cam.position += cam.right * deltaTime * speedMultiplier;
        if (key_input::pressed('a'))
            cam.position += -cam.right * deltaTime * speedMultiplier;
    }

    // rotate using mouse, instead of IJKL:
    {
        cam.rotate(mouse_input::getDelta().x / cam.viewportWidth * -100 * mouseSensivity, mu::Y);
        cam.rotate(mouse_input::getDelta().y / cam.viewportHeight * -100 * mouseSensivity, cam.right);

        // set cursor back to center of screen:
        auto mousePos = mouse_input::getPosition();

        if (mousePos.x <= 100 || mousePos.y <= 100
            || mousePos.x >= cam.viewportWidth - 100 || mousePos.y >= cam.viewportHeight - 100)
        {
            mouse_input::getPosition() = ivec2(cam.viewportWidth / 2, cam.viewportHeight / 2);
            glutWarpPointer(cam.viewportWidth / 2, cam.viewportHeight / 2);
        }        
    }

    if (key_input::justPressed('v'))    // switch view modes
    {
        droneMode = !droneMode;
        int w = cam.viewportWidth;
        int h = cam.viewportHeight;

        if (droneMode)
        {
            delete walkModeCam;
            walkModeCam = new PerspectiveCamera(cam);
            if (droneModeDefaultCam)
                cam = *droneModeDefaultCam;
        }
        else if (walkModeCam)
            cam = *walkModeCam;

        cam.viewportWidth = w;
        cam.viewportHeight = h;
    }
        

    if (!droneMode) // walk mode:
    {
        yVelocity -= gravity * deltaTime;
        cam.position.y += yVelocity;

        if (cam.position.y <= minimumEyeHeight)
        {
            yVelocity = 0;
            cam.position.y = minimumEyeHeight;
        }

        if (key_input::pressed(' ') && cam.position.y == minimumEyeHeight)
            yVelocity = jumpForce;
    }
    else // drone mode:
    {
        // down, up
        if (key_input::pressed('q'))
            cam.position.y -= deltaTime * speedMultiplier;
        if (key_input::pressed('e'))
            cam.position.y += deltaTime * speedMultiplier;
    }
    cam.update(); // update matrices.
}

CameraController::~CameraController()
{
    delete droneModeDefaultCam;
    delete walkModeCam;
}
