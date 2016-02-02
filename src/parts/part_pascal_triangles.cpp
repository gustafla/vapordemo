#include "part_pascal_triangles.hpp"
#include "shader.hpp"
#include "geo_primitives.hpp"
#include "paths.hpp"
#include "tga_file.hpp"
#include <iostream>

PartPascalTriangles::PartPascalTriangles():
shader(Shader(shaderPath("simple.vert")), Shader(shaderPath("generic.frag"))),
testImage(loadTGAFile(texturePath("vapor.tga"))) {
    shader.use();
    setTextureUniforms(shader);
    glUniform4f(shader.getUfmHandle("color"), 1, 0, 0, 0.0f);
}

void PartPascalTriangles::draw() {
    shader.use();
    testImage.bindToUnit(0);
    GeoPrimitives::singleton().quad.draw(shader);
}
