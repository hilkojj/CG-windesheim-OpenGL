#pragma once

#include "../../math.h"

class PerspectiveCamera
{
public:
    PerspectiveCamera(float nearClipping, float farClipping, float viewportWidth, float viewportHeight, float fieldOfView);

    vec3 position, direction, up, right;

    mat4 projection, view, combined;

    float
        nearClipping, farClipping,
        viewportWidth, viewportHeight,
        fieldOfView;

    /**
     * sets 'direction', 'up' and 'right' so that the Camera looks at the 'target'.
     * 'up' will be uppish in relation to the Y axis.
     */
    void lookAt(vec3 target);

    /**
     * sets 'direction', 'up' and 'right' so that the Camera looks at the 'target'.
     * 'up' will be uppish in relation to 'localYAxis'.
     */
    void lookAt(vec3 target, vec3 localYAxis);

    void rotate(float degrees, vec3 axis);

    void update();

    vec3 getRayDirection(float viewportX, float viewportY) const;

    /**
     * returns the point in 'normalized device space'.
     * IF inViewport is originally set to false then inViewport will be set to true if the point is inside the viewport.
     */
    vec3 project(const vec3& p, bool& inViewport) const;
    vec3 project(const vec3& p) const;

    /**
     * returns the point in 'viewport space'.
     * IF inViewport is originally set to false then inViewport will be set to true if the point is inside the viewport.
     */
    vec3 projectPixels(const vec3& p, bool& inViewport) const;
    vec3 projectPixels(const vec3& p) const;


};

