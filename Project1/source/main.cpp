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
#include "graphics/geometry/Mesh.h"
#include "graphics/geometry/VertexBuffer.h"

using namespace std;


//--------------------------------------------------------------------------------
// Consts
//--------------------------------------------------------------------------------

const int WIDTH = 800, HEIGHT = 600;
unsigned const int DELTA_TIME = 1000 / 60;


//--------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------

SharedMesh testMesh;

// Matrices
mat4 model;
mat4 mvp;

PerspectiveCamera *camera = NULL;   // initialization in main()
CameraController camController;

ShaderProgram* shader = NULL; // initialization in main()


//--------------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------------

void Render()
{
    float deltaTimeSec = float(DELTA_TIME) / 1000.f;

    if (key_input::justPressed(27)) // ESC
        glutExit();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();

    // Do transformation
    camController.update(deltaTimeSec, *camera);
    model = rotate(model, 0.01f, vec3(0.0f, 1.0f, 0.0f));
    mvp = camera->combined * model;

    // Send mvp
    glUniformMatrix4fv(shader->location("mvp"), 1, GL_FALSE, value_ptr(mvp));

    testMesh->render();

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
    camera->viewportWidth = w;
    camera->viewportHeight = h;
    camera->update();
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


//------------------------------------------------------------
// void InitShaders()
// Initializes the fragmentshader and vertexshader
//------------------------------------------------------------

void InitShaders()
{
    shader = new ShaderProgram(
        "default shader",
        files::readString("assets/shaders/default.vert").c_str(),
        files::readString("assets/shaders/default.frag").c_str());
}


int main(int argc, char** argv)
{
    camera = new PerspectiveCamera(.1, 1000, WIDTH, HEIGHT, 90);
    InitGlutGlew(argc, argv);
    InitShaders();

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);


    testMesh = SharedMesh(new Mesh);
    testMesh->attributes.add({ "position", 3 });

    testMesh->addVertices(4);

    testMesh->get<vec3>(0, 0) = vec3(-1, -1, 0);
    testMesh->get<vec3>(1, 0) = vec3(-1, 1, 0);
    testMesh->get<vec3>(2, 0) = vec3(1, 1, 0);
    testMesh->get<vec3>(3, 0) = vec3(1, -1, 0);

    testMesh->parts.emplace_back();
    testMesh->parts.back().indices = { 0, 1, 2, 2, 3, 0 };

    VertexBuffer::uploadSingleMesh(testMesh);


    // Main loop
    glutMainLoop();

    return 0;
}
