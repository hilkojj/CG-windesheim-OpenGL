#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "utils/math.h"
#include "graphics/camera/PerspectiveCamera.h"
#include "graphics/camera/CameraController.h"

#include "input/key_input.h"
#include "input/mouse_input.h"
#include "graphics/ShaderProgram.h"
#include "utils/files.h"
#include "graphics/geometry/VertexBuffer.h"
#include "graphics/Model.h"
#include "loaders/model_loader.h"
#include "Scene.h"
#include "graphics/geometry/mesh_generators.h"
#include "loaders/texture_loader.h"

using namespace std;


//--------------------------------------------------------------------------------
// Consts
//--------------------------------------------------------------------------------

const int WIDTH = 800, HEIGHT = 600;
unsigned const int DELTA_TIME = 1000 / 60;


//--------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------

Scene *scene = NULL;

CameraController camController;


//--------------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------------

void Render()
{
    float deltaTimeSec = float(DELTA_TIME) / 1000.f;

    if (key_input::justPressed(27)) // ESC
        glutExit();

    camController.update(deltaTimeSec, scene->camera);
    
    scene->render();

    auto &testModel = scene->models.back();

    //testModel.transform = rotate(testModel.transform, .1f, mu::Y);

    glutSwapBuffers();
    key_input::update();
    mouse_input::getDelta() = ivec2(0);
}


//------------------------------------------------------------
// void Render(int n)
// Render method that is called by the timer function
//------------------------------------------------------------

void Render(int n)
{
    Render();
    glutTimerFunc(DELTA_TIME, Render, 0);
}

void onResize(int w, int h)
{
    scene->camera.viewportWidth = w;
    scene->camera.viewportHeight = h;
    scene->camera.update();
    glViewport(0, 0, w, h);
}

//------------------------------------------------------------
// void InitGlutGlew(int argc, char **argv)
// Initializes Glut and Glew
//------------------------------------------------------------

void InitGlutGlew(int argc, char** argv)
{
    glewExperimental = true;        // https://stackoverflow.com/questions/30061443/opengl-glgenvertexarrays-access-violation-executing-location-0x00000000
    glutInit(&argc, argv);
    glutInitContextVersion(4, 3);   // request OpenGL Core 4.3
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("CG Hilko Janssen");
    glutDisplayFunc(Render);
    glutKeyboardFunc(key_input::keyPressedHandler);
    glutKeyboardUpFunc(key_input::keyUpHandler);
    glutPassiveMotionFunc(mouse_input::motionHandler);
    glutReshapeFunc(onResize);
    glutTimerFunc(DELTA_TIME, Render, 0);

    glutSetCursor(GLUT_CURSOR_NONE);    // hide cursor

    glewInit();

    // print OpenGL version:
    std::cout << "OpenGL renderer & version: " << glGetString(GL_RENDERER) << " - " << glGetString(GL_VERSION) << std::endl;
}


int main(int argc, char** argv)
{
    InitGlutGlew(argc, argv);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    scene = new Scene;

    scene->models.emplace_back();
    auto &tower = scene->models.back();
    model_loader::loadIntoModel(tower, "assets/models/martini_toren.obj");
    tower.transform = translate(tower.transform, vec3(-80, 0, 0));

    scene->models.emplace_back();
    auto &castle = scene->models.back();
    model_loader::loadIntoModel(castle, "assets/models/zamek.obj");
    castle.transform = translate(castle.transform, vec3(40, 0, 0));

    scene->models.emplace_back();
    auto &groundPlane = scene->models.back();
    auto quad = mesh_generators::createQuad(scale(mat4(1), vec3(1000)), 100);
    groundPlane.meshes.push_back(quad);
    VertexBuffer::uploadSingleMesh(quad);
    groundPlane.meshes.front()->material.texture = texture_loader::getOrLoad("assets/models/textures/grass.jpg");


    scene->models.emplace_back();
    auto& tree = scene->models.back();
    auto sphere = mesh_generators::createSphere(32, 16, scale(mat4(1), vec3(10, 2, 1)));
    tree.meshes.push_back(sphere);
    VertexBuffer::uploadSingleMesh(sphere);
    tree.meshes.front()->material.texture = texture_loader::getOrLoad("assets/models/textures/grass.jpg");


    onResize(WIDTH, HEIGHT);

    // Main loop
    glutMainLoop();

    delete scene;

    return 0;
}
