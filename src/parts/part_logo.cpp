#include "part_logo.hpp"
#include "paths.hpp"
#include "geo_primitives.hpp"
#include "demo_consts.hpp"
#include "demo.hpp"

PartLogo::PartLogo(float t):
DemoPart(t),
mode7(Shader(shaderPath("simple.vert")), Shader(shaderPath("m7.frag"))) {
    mode7.use();
    glUniform2f(mode7.getUfmHandle("iResolution"), DEMO_W, DEMO_H);
    vec3 c = DEMO_C_DBLUE;
    glUniform3fv(mode7.getUfmHandle("color"), 1, (GLfloat*)&c);
}

void PartLogo::draw() {
    mode7.use();
    glUniform1f(mode7.getUfmHandle("iGlobalTime"), DEMO_T());
    GeoPrimitives::singleton().quad.draw(mode7);
}
