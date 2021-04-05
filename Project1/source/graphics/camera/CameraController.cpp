#include "CameraController.h"
#include "../../input/key_input.h"

void CameraController::update(float deltaTime, PerspectiveCamera &cam)
{
    /*
    speedMultiplier += MouseInput::yScroll * .5;
    if (speedMultiplier < 0) speedMultiplier = 0;

    if (KeyInput::pressed(GLFW_KEY_W))
        cam->position += cam->direction * glm::vec3(deltaTime * speedMultiplier);

    if (KeyInput::pressed(GLFW_KEY_S))
        cam->position += cam->direction * glm::vec3(-deltaTime * speedMultiplier);

    if (KeyInput::pressed(GLFW_KEY_D))
        cam->position += cam->right * glm::vec3(deltaTime * speedMultiplier);

    if (KeyInput::pressed(GLFW_KEY_A))
        cam->position += cam->right * glm::vec3(-deltaTime * speedMultiplier);

    if (KeyInput::pressed(GLFW_KEY_LEFT_SHIFT))
        cam->position.y -= deltaTime * speedMultiplier;

    if (KeyInput::pressed(GLFW_KEY_SPACE))
        cam->position.y += deltaTime * speedMultiplier;

    if (MouseInput::deltaMouseX != 0)
        cam->rotate(MouseInput::deltaMouseX / gu::width * -100 * mouseSensivity, mu::Y);

    if (MouseInput::deltaMouseY != 0)
        cam->rotate(MouseInput::deltaMouseY / gu::height * -100 * mouseSensivity, cam->right);


    */

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

    cam.update();
}
