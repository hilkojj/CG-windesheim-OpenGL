#include "mesh_generators.h"

vec3 tf(const vec3& v, const mat4& transform, bool dir)
{
    return vec3(transform * vec4(v, dir ? 0 : 1));
}

SharedMesh mesh_generators::createQuad(const mat4 &transform, float uvScaling)
{
    
    SharedMesh quad(new Mesh);
    quad->name = "quad";
    int pos = quad->attributes.add({ "position", 3 });
    int nor = quad->attributes.add({ "normal", 3 });
    int uv = quad->attributes.add({ "uv", 2 });

    quad->addVertices(4);
    
    quad->get<vec3>(0, pos) = tf(vec3(1, 0, 1), transform, false);
    quad->get<vec3>(1, pos) = tf(vec3(1, 0, -1), transform, false);
    quad->get<vec3>(2, pos) = tf(vec3(-1, 0, -1), transform, false);
    quad->get<vec3>(3, pos) = tf(vec3(-1, 0, 1), transform, false);

    quad->get<vec3>(0, nor) =
        quad->get<vec3>(1, nor) =
        quad->get<vec3>(2, nor) =
        quad->get<vec3>(3, nor) = tf(mu::Y, transform, true);

    quad->get<vec2>(0, uv) = uvScaling * vec2(1, 1);
    quad->get<vec2>(1, uv) = uvScaling * vec2(1, -1);
    quad->get<vec2>(2, uv) = uvScaling * vec2(-1, -1);
    quad->get<vec2>(3, uv) = uvScaling * vec2(-1, 1);

    quad->indices = { 0, 1, 2, 2, 3, 0 };

    return quad;
}
