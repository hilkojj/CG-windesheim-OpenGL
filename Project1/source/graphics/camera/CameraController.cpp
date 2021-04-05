#include "CameraController.h"
#include "../../input/key_input.h"
#include "../../input/mouse_input.h"

#include <GL/freeglut.h>

void CameraController::update(float deltaTime, PerspectiveCamera &cam)
{
    if (key_input::pressed('w'))
        cam.position += cam.direction * deltaTime * speedMultiplier;

    if (key_input::pressed('s'))
        cam.position += -cam.direction * deltaTime * speedMultiplier;

    if (key_input::pressed('d'))
        cam.position += cam.right * deltaTime * speedMultiplier;

    if (key_input::pressed('a'))
        cam.position += -cam.right * deltaTime * speedMultiplier;

    if (key_input::pressed('q'))
        cam.position.y -= deltaTime * speedMultiplier;

    if (key_input::pressed('e'))
        cam.position.y += deltaTime * speedMultiplier;

    cam.rotate(mouse_input::getDelta().x / cam.viewportWidth * -100 * mouseSensivity, mu::Y);

    cam.rotate(mouse_input::getDelta().y / cam.viewportHeight * -100 * mouseSensivity, cam.right);

    cam.update();

    // set cursor back to center of screen:
    glutWarpPointer(cam.viewportWidth / 2, cam.viewportHeight / 2);
    mouse_input::getPosition() = ivec2(cam.viewportWidth / 2, cam.viewportHeight / 2);
}
