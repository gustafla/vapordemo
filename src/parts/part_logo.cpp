#include "part_logo.hpp"
#include "paths.hpp"
#include "geo_primitives.hpp"
#include "demo_consts.hpp"
#include "demo.hpp"
#include "define.hpp"
#include "tga_file.hpp"
#include "gfx_utils.hpp"
#include "uniforms.hpp"
#include <cmath>

PartLogo::PartLogo(float t):
DemoPart(t),
shader(Shader(shaderPath("generic.vert")), Shader(shaderPath("generic.frag"))),
shader2(Shader(shaderPath("simple.vert")), Shader(shaderPath("generic.frag"))),
mvp(getPProjMat(45, (DEMO_W/DEMO_H), 0.1, 50.0)),
logo(loadTGAFile(texturePath("logo.tga"))),
logoRect(vec2(0), vec2(0)) {
    setBaseUniforms(shader);
    setBaseUniforms(shader2);
    
    logoRect.resize(vec2(logo.getWidth(), logo.getHeight()), vec2(DEMO_W, DEMO_H));
    
    for (int i = 0; i < N_LINES; i++) {
        vertices[i*2].x   = -(N_LINES/2);
        vertices[i*2].y   = 0.0;
        vertices[i*2].z   = (((float)i)-(N_LINES/2));
        vertices[i*2+1].x = (N_LINES/2);
        vertices[i*2+1].y = 0.0;
        vertices[i*2+1].z = vertices[i*2].z;
    }
    for (int i = 0; i < N_LINES; i++) {
        vertices[(2*N_LINES)+i*2].x   = (((float)i)-(N_LINES/2));
        vertices[(2*N_LINES)+i*2].y   = 0.0;
        vertices[(2*N_LINES)+i*2].z   = -(N_LINES/2);
        vertices[(2*N_LINES)+i*2+1].x = vertices[(2*N_LINES)+i*2].x;
        vertices[(2*N_LINES)+i*2+1].y = 0.0;
        vertices[(2*N_LINES)+i*2+1].z = (N_LINES/2);
    }
}

void PartLogo::draw() {
    Sync& sync = Demo::singleton().getSync();
    shader.use();
    
    vec4 c = vec4(DEMO_C_DBLUE, 1.0);
    glUniform4fv(shader.getUfmHandle("color"), 1, (GLfloat*)&c);
    
    mvp.setView(
        sync.getValue(SYNC_PART_LOGO_VIEW_X,  DEMO_T()-start),
        sync.getValue(SYNC_PART_LOGO_VIEW_Y,  DEMO_T()-start),
        sync.getValue(SYNC_PART_LOGO_VIEW_Z,  DEMO_T()-start),
        sync.getValue(SYNC_PART_LOGO_VIEW_RX, DEMO_T()-start),
        sync.getValue(SYNC_PART_LOGO_VIEW_RY, DEMO_T()-start),
        sync.getValue(SYNC_PART_LOGO_VIEW_RZ, DEMO_T()-start)
    );
    mvp.setModel(fmod(DEMO_T()*5.0, 1.0)); //Move the grid and jump to start after one unit to fake infinity
    mvp.apply(shader);
    
    glEnableVertexAttribArray(shader.getAtrHandle(NAME_A_POSITION));
    glVertexAttribPointer(shader.getAtrHandle(NAME_A_POSITION), SIZE_POS, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*SIZE_POS, (GLfloat*)vertices);
    glDrawArrays(GL_LINES, 0, N_LINES*2*2);
    
    glClear(GL_DEPTH_BUFFER_BIT);
    shader2.use();
    glUniform4f(shader2.getUfmHandle("color"), 0.0, 0.0, 0.0, 0.0);
    logo.bindToUnit(0);
    for (int i=12; i > 0; i--) {
        logoRect.move(vec2(sync.getValue(SYNC_PART_LOGO_X, DEMO_T()-start)+sync.getValue(SYNC_PART_LOGO_X_SEPARATION, DEMO_T()-start)*i, 0), max(10.0-DEMO_T()*6.0-start-(0.3*i), 0.6));
        logoRect.draw(shader2);
    }
}
