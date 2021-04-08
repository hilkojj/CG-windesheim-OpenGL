#include "mesh_generators.h"

vec3 tf(const vec3& v, const mat4& transform, bool dir)
{
    return vec3(transform * vec4(v, dir ? 0 : 1));
}

SharedMesh mesh_generators::createQuad(const mat4 &transform, const vec2& uvScaling)
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
        quad->get<vec3>(3, nor) = normalize(tf(mu::Y, transform, true));

    quad->get<vec2>(0, uv) = uvScaling * vec2(1, 1);
    quad->get<vec2>(1, uv) = uvScaling * vec2(1, -1);
    quad->get<vec2>(2, uv) = uvScaling * vec2(-1, -1);
    quad->get<vec2>(3, uv) = uvScaling * vec2(-1, 1);

    quad->indices = { 0, 1, 2, 2, 3, 0 };

    return quad;
}

SharedMesh mesh_generators::createSphere(int segments, int rings, const mat4& transform, const vec2& uvScaling)
{
    
    SharedMesh sphere(new Mesh);
    sphere->name = "sphere";
    int posOffset = sphere->attributes.add({ "position", 3 });
    int norOffset = sphere->attributes.add({ "normal", 3 });
    int texOffset = sphere->attributes.add({ "uv", 2 });

    sphere->indices.resize((segments * (rings - 1)) * 6);

    int nrOfVerts = (segments + 1) * rings;
    sphere->addVertices(nrOfVerts);

    int vertI = 0;
    for (int x = 0; x <= segments; x++)
    {
        for (int y = 0; y < rings; y++)
        {
            float lon = 360.0 * ((float) x / segments);
            float lat = (y / (rings - 1.0)) * 180.0 - 90.0;
            vec3 pos = vec3(cos(lon * mu::DEGREES_TO_RAD), sin(lat * mu::DEGREES_TO_RAD), sin(lon * mu::DEGREES_TO_RAD));
            float m = cos(lat * mu::DEGREES_TO_RAD);
            pos.x *= m;
            pos.z *= m;
            sphere->get<vec3>(vertI, posOffset) = tf(pos, transform, false);
            sphere->get<vec3>(vertI, norOffset) = normalize(tf(pos, transform, true));
            sphere->get<vec2>(vertI, texOffset) = vec2(lon / 360, lat / 180 + .5) * uvScaling;
            vertI++;
        }
    }
    assert(nrOfVerts == vertI);
    int i = 0;
    for (int x = 0; x < segments; x++)
    {
        for (int y = 0; y < rings - 1; y++)
        {
            sphere->indices[i++] = y + (x * rings);
            sphere->indices[i++] = y + 1 + (x * rings);
            sphere->indices[i++] = y + ((x + 1) * rings);

            sphere->indices[i++] = y + ((x + 1) * rings);
            sphere->indices[i++] = y + (x * rings) + 1;
            sphere->indices[i++] = y + ((x + 1) * rings) + 1;
        }
    }
    assert(sphere->indices.size() == i);

    return sphere;
}

SharedMesh mesh_generators::createCylinder(int segments, const mat4& transform, const vec2& uvScaling)
{
    SharedMesh cylinder(new Mesh);
    cylinder->name = "cylinder";
    int posOffset = cylinder->attributes.add({ "position", 3 });
    int norOffset = cylinder->attributes.add({ "normal", 3 });
    int texOffset = cylinder->attributes.add({ "uv", 2 });

    cylinder->indices.resize(segments * 6);

    int nrOfVerts = (segments + 1) * 2;
    cylinder->addVertices(nrOfVerts);

    int vertI = 0;
    for (int x = 0; x <= segments; x++)
    {
        float lon = 360.0 * ((float)x / segments);

        vec3 pos0 = vec3(cos(lon * mu::DEGREES_TO_RAD), 0, sin(lon * mu::DEGREES_TO_RAD));
        vec3 pos1 = vec3(pos0.x, 1, pos0.z);

        {
            cylinder->get<vec3>(vertI, posOffset) = tf(pos0, transform, false);
            cylinder->get<vec3>(vertI, norOffset) = normalize(tf(pos0, transform, true));
            cylinder->get<vec2>(vertI, texOffset) = vec2(lon / 360, 0) * uvScaling;
            vertI++;
        }
        {
            cylinder->get<vec3>(vertI, posOffset) = tf(pos1, transform, false);
            cylinder->get<vec3>(vertI, norOffset) = normalize(tf(pos0, transform, true));
            cylinder->get<vec2>(vertI, texOffset) = vec2(lon / 360, 1) * uvScaling;
            vertI++;
        }
    }
    assert(nrOfVerts == vertI);

    int i = 0;
    for (int x = 0; x < segments * 2; x += 2)
    {
        cylinder->indices[i++] = x + 0;
        cylinder->indices[i++] = x + 1;
        cylinder->indices[i++] = x + 2;
        cylinder->indices[i++] = x + 2;
        cylinder->indices[i++] = x + 1;
        cylinder->indices[i++] = x + 3;
    }
    assert(cylinder->indices.size() == i);

    return cylinder;
}
