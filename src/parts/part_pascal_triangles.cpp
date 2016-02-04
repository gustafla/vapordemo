#include "part_pascal_triangles.hpp"
#include "shader.hpp"
#include "geo_primitives.hpp"
#include "paths.hpp"
#include "tga_file.hpp"
#include <iostream>
#include "demo_consts.hpp"

PartPascalTriangles::PartPascalTriangles():
shader(Shader(shaderPath("simple.vert")), Shader(shaderPath("tunnel.frag"))) {
    shader.use();
    //setTextureUniforms(shader);
    //glUniform4f(shader.getUfmHandle("color"), 1, 0, 0, 0.0f);
    glUniform2f(shader.getUfmHandle("iResolution"), DEMO_W, DEMO_H);
}

void PartPascalTriangles::draw() {
    shader.use();
    glUniform1f(shader.getUfmHandle("iGlobalTime"), Demo::singleton().getWindow().getTime());
    //testImage.bindToUnit(0);
    GeoPrimitives::singleton().quad.draw(shader);
}
