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

const int WIDTH = 1600, HEIGHT = 900;
unsigned const int DELTA_TIME = 1000 / 60;


//--------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------

Scene *scene = NULL;

CameraController camController;

Model *bolt = NULL; // pointer to the thunderbolt, to animate.
mat4 boltTransform;
std::vector<Model*> trees;  // list of generated trees. Used to update their animations in animateTrees()

// generate a tree at a position
void addTree(const vec2& pos)
{
    scene->models.emplace_back();
    auto& tree = scene->models.back();
    trees.push_back(&tree); // scene.models is a std::list, so no moving will occur, taking a pointer is safe.

    float treeHeight = mu::random(12, 30);

    auto sphere = mesh_generators::createSphere(32, 16, scale(translate(mat4(1), vec3(0, treeHeight, 0)), vec3(4)));
    tree.meshes.push_back(sphere);

    auto cylinder = mesh_generators::createCylinder(16, scale(mat4(1), vec3(1, treeHeight, 1)));
    tree.meshes.push_back(cylinder);

    sphere->material.texture = texture_loader::getOrLoad("assets/models/textures/leaves.png");
    cylinder->material.texture = texture_loader::getOrLoad("assets/models/textures/bark.png");

    auto& buffer = VertexBuffer::with(sphere->attributes)->add(sphere).add(cylinder);

    // add branches:
    for (int i = 0; i < mu::randomInt(2, 5); i++)
    {
        mat4 tf = mat4(1);

        float length = mu::random(1, 10);
        float start = mu::random(0, treeHeight);

        tf = translate(tf, vec3(0, start, 0));
        tf = rotate(tf, mu::random(360) * mu::DEGREES_TO_RAD, mu::Y);
        tf = rotate(tf, mu::random(20, 90) * mu::DEGREES_TO_RAD, mu::X);

        auto sphere = mesh_generators::createSphere(32, 16, scale(tf * translate(mat4(1), vec3(0, length, 0)), vec3(2)));
        tree.meshes.push_back(sphere);

        auto cylinder = mesh_generators::createCylinder(16, tf * scale(mat4(1), vec3(.4, length, .4)));
        tree.meshes.push_back(cylinder);

        buffer.add(sphere).add(cylinder);

        sphere->material.texture = texture_loader::getOrLoad("assets/models/textures/leaves.png");
        cylinder->material.texture = texture_loader::getOrLoad("assets/models/textures/bark.png");
    }

    buffer.upload(true);    // upload vertex data to GPU

    tree.transform = translate(tree.transform, vec3(pos.x, 0, pos.y));
}

// update animations for trees
void animateTrees(float dt)
{
    static float time = mu::PI * .5;
    time += dt;
    for (auto tree : trees)
        tree->transform = rotate(tree->transform, sin(time) * .3f * mu::DEGREES_TO_RAD, mu::X);
}

//--------------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------------

void Render()
{
    float deltaTimeSec = float(DELTA_TIME) / 1000.f;

    if (key_input::justPressed(27)) // ESC
        glutExit();

    camController.update(deltaTimeSec, scene->camera);
    scene->sunDirection = rotate(scene->sunDirection, 1 * mu::DEGREES_TO_RAD, mu::Y);

    animateTrees(deltaTimeSec);

    {   // animate thunderbolt:
        static float time = 0;
        time += deltaTimeSec;

        // sin(time)^300 + sin(time - 0.3)^100 * 0.4
        float s = pow(sin(time), 300) + pow(sin(time - .3), 100) * .4;
        bolt->transform = scale(boltTransform, vec3(s));
    }
    
    scene->render();

    auto &testModel = scene->models.back();

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
    camController.droneModeDefaultCam = new PerspectiveCamera(scene->camera);
    camController.droneModeDefaultCam->position = vec3(100, 200, 0);
    camController.droneModeDefaultCam->lookAt(vec3(-80, 0, 0));

    {   // TOWER:
        scene->models.emplace_back();
        auto& tower = scene->models.back();
        model_loader::loadIntoModel(tower, "assets/models/martini_toren.obj");
        tower.transform = translate(tower.transform, vec3(-80, 0, 0));
    }
    {   // THUNDERBOLT:
        scene->models.emplace_back();
        bolt = &scene->models.back();
        model_loader::loadIntoModel(*bolt, "assets/models/thunderbolt.obj");
        boltTransform = bolt->transform = translate(bolt->transform, vec3(-80, 150, 0));
        bolt->shadeless = true;
    }
    {   // CASTLE:
        scene->models.emplace_back();
        auto& castle = scene->models.back();
        model_loader::loadIntoModel(castle, "assets/models/zamek.obj");
        castle.transform = translate(castle.transform, vec3(40, 0, 0));
    }
    {   // GROUNDPLANE:
        scene->models.emplace_back();
        auto& groundPlane = scene->models.back();
        auto quad = mesh_generators::createQuad(scale(mat4(1), vec3(1000)), vec2(100));
        groundPlane.meshes.push_back(quad);
        VertexBuffer::uploadSingleMesh(quad);
        groundPlane.meshes.front()->material.texture = texture_loader::getOrLoad("assets/models/textures/grass.jpg");
    }
    // TREES:
    addTree(vec2(0, 0));
    addTree(vec2(0, 30));
    addTree(vec2(0, 60));
    addTree(vec2(100, 80));
    addTree(vec2(60, 70));
    addTree(vec2(-60, 50));
    addTree(vec2(-60, -50));
    addTree(vec2(-60, -100));

    onResize(WIDTH, HEIGHT);

    // Main loop
    glutMainLoop();

    delete scene;

    return 0;
}
