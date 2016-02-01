#include "part_pascal_triangles.hpp"
#include "shader.hpp"
#include "geo_primitives.hpp"
#include "paths.hpp"
#include <iostream>

PartPascalTriangles::PartPascalTriangles():
shader(Shader(shaderPath("simple.vert")), Shader(shaderPath("generic.frag"))) {
    shader.use();
    glUniform1i(shader.getUfmHandle("iChannel0"), 0);
    glUniform4f(shader.getUfmHandle("color"), 1, 0, 0, 1);
}

void PartPascalTriangles::draw() {
    shader.use();
    GeoPrimitives::singleton().triangleEquilateral.draw(shader);
}
