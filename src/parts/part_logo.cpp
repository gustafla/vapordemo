#include "part_logo.hpp"
#include "paths.hpp"
#include "geo_primitives.hpp"
#include "demo_consts.hpp"
#include "demo.hpp"
#include "define.hpp"
#include "tga_file.hpp"
#include "gfx_utils.hpp"
#include "uniforms.hpp"
#include "util.hpp"
#include <cmath>

static const float PI2=(2.0*DEMO_PI);

PartLogo::PartLogo(float t):
DemoPart(t),
shader(Shader(shaderPath("generic.vert")), Shader(shaderPath("generic.frag"))),
shaderFlare(Shader(shaderPath("generic.vert")), Shader(shaderPath("generic.frag"))),
shaderPyramid(Shader(shaderPath("generic.vert")), Shader(shaderPath("solid.frag"))),
shader2(Shader(shaderPath("simple.vert")), Shader(shaderPath("generic.frag"))),
shaderStarfield(Shader(shaderPath("generic.vert")), Shader(shaderPath("generic.frag"))),
mvp(getPProjMat(45, DEMO_W/DEMO_H, 0.1, 50.0)),
logo(loadTGAFile(texturePath("logo.tga"))),
flare(loadTGAFile(texturePath("flare.tga"))),
logoRect(vec2(0), vec2(0)) {
    setBaseUniforms(shader, 1, LightingState(), vec4(DEMO_C_DBLUE, 1.0));
    setBaseUniforms(shaderFlare);
    setBaseUniforms(shaderPyramid);
    setBaseUniforms(shader2);
    setBaseUniforms(shaderStarfield, 1, LightingState(), vec4(vec3(0.4), 1.0));
    
    logoRect.resize(vec2(logo.getWidth(), logo.getHeight()), vec2(DEMO_W, DEMO_H));
    
    genMeshes();
}

PartLogo::~PartLogo() {
    delete grid;
    delete pyramid;
}

void PartLogo::draw() {
    Sync& sync = Demo::singleton().getSync();
    
    shaderStarfield.use();
    mvp.reset();
    mvp.setViewRotation(0.0, cos(DEMO_T()*0.5)*0.4);
    //mvp.setViewRotation(DEMO_T(), DEMO_T()*0.2, 0.0);
    for (int i=0; i<N_STARS; i++) {
        glClear(GL_DEPTH_BUFFER_BIT);
        mvp.setModel(starfield[i*3], starfield[i*3+1], fmod(starfield[i*3+2]+4+DEMO_T()*2.0, 8)-8, 0, 0, 0, 0.02);
        mvp.apply(shaderStarfield);
        setColorUniform(shaderStarfield, vec4(vec3(sync.getValue(SYNC_PART_LOGO_STARFIELD, DEMO_T())), 1.0));
        GeoPrimitives::singleton().quad.draw(shaderStarfield);
    }
    
    glClear(GL_DEPTH_BUFFER_BIT);
    
    shader.use();
    mvp.setView(
        sync.getValue(SYNC_PART_LOGO_VIEW_X,  DEMO_T()),
        sync.getValue(SYNC_PART_LOGO_VIEW_Y,  DEMO_T()),
        sync.getValue(SYNC_PART_LOGO_VIEW_Z,  DEMO_T()),
        sync.getValue(SYNC_PART_LOGO_VIEW_RX, DEMO_T()),
        sync.getValue(SYNC_PART_LOGO_VIEW_RY, DEMO_T()),
        sync.getValue(SYNC_PART_LOGO_VIEW_RZ, DEMO_T())
    );
    mvp.setModel(fmod(DEMO_T()*3.0, 1.0)); //Move the grid and jump to start after one unit to fake infinity
    mvp.apply(shader);
    grid->draw(shader, GL_LINES);
    
    glClear(GL_DEPTH_BUFFER_BIT);
    shader2.use();
    logo.bindToUnit(0);
    for (int i=12; i > 0; i--) {
        logoRect.move(vec2(sync.getValue(SYNC_PART_LOGO_X, DEMO_T())+sync.getValue(SYNC_PART_LOGO_X_SEPARATION, DEMO_T())*i, 0), max(10.0-DEMO_T()*4.0-(0.3*i), 0.6));
        logoRect.draw(shader2);
    }
    
    glClear(GL_DEPTH_BUFFER);
    shaderFlare.use();
    flare.bindToUnit(0);
    mvp.reset();
    
    mvp.setModel(-2.6, 1, -10, 0, 0, DEMO_T(), 1.0);
    mvp.apply(shaderFlare);
    setOpacityUniform(shaderFlare, sync.getValue(SYNC_PART_LOGO_FLARE1_OPACITY, DEMO_T()));
    GeoPrimitives::singleton().quad.draw(shaderFlare);
    
    mvp.setModel(-0.1, 0.6, -10, 0, 0, DEMO_T()*3.0, 0.5);
    mvp.apply(shaderFlare);
    setOpacityUniform(shaderFlare, sync.getValue(SYNC_PART_LOGO_FLARE2_OPACITY, DEMO_T()));
    GeoPrimitives::singleton().quad.draw(shaderFlare);
    
    mvp.setModel(2, 0.5, -10, 0, 0, -DEMO_T(), 0.7);
    mvp.apply(shaderFlare);
    setOpacityUniform(shaderFlare, sync.getValue(SYNC_PART_LOGO_FLARE3_OPACITY, DEMO_T()));
    GeoPrimitives::singleton().quad.draw(shaderFlare);
    
    glClear(GL_DEPTH_BUFFER_BIT);
    shaderPyramid.use();
    float nPyramids=sync.getValue(SYNC_PART_LOGO_PYRAMID_RECURSION, DEMO_T());
    for (float i=0.0; i<nPyramids; i+=1) {
        mvp.reset();
        mvp.setModel(
            0.0,
            sync.getValue(SYNC_PART_LOGO_PYRAMID_Y, DEMO_T()),
            -3.0,
            sync.getValue(SYNC_PART_LOGO_VIEW_RX, DEMO_T()),
            sync.getValue(SYNC_PART_LOGO_VIEW_RY, DEMO_T()),
            sync.getValue(SYNC_PART_LOGO_VIEW_RZ, DEMO_T()),
            1.0-(i/10.0)+sync.getValue(SYNC_PART_LOGO_PYRAMID_EXPLOSION, DEMO_T())*(i+1.0)
        );
        mvp.apply(shaderPyramid);
        setColorUniform(shaderPyramid, vec4(mix(vec3(0.4), DEMO_C_VPINK, i/nPyramids), 1.0));
        pyramid->draw(shaderPyramid, GL_LINES);
    }
}

void PartLogo::genMeshes() {
    Mesh gridMesh;
    for (int i = 0; i < N_LINES; i++) {
        gridMesh.pushPosition(vec3(-(N_LINES/2), 0.0, (((float)i)-(N_LINES/2))));
        gridMesh.pushPosition(vec3((N_LINES/2), 0.0, (((float)i)-(N_LINES/2))));
    }
    for (int i = 0; i < N_LINES; i++) {
        gridMesh.pushPosition(vec3((((float)i)-(N_LINES/2)), 0.0, -(N_LINES/2)));
        gridMesh.pushPosition(vec3((((float)i)-(N_LINES/2)), 0.0, (N_LINES/2)));
    }
    grid = new StaticModel(gridMesh);
    
    for(int i=0; i<N_STARS*3; i++) {
        starfield[i] = randf()*4.0;
    }
    
    Mesh pyramidMesh;
    
    const float H = 1.0;
    const float W = 1.0;
    const float hdelta = 0.2;
    const float wdelta = (DEMO_PI/2.0);
    
    for (float h=0.0; h<H; h+=hdelta) {
        for (float w=0.0; w<PI2; w+=wdelta) {
            pyramidMesh.pushPosition(vec3(sin(w)*mix(W, 0.0, h/H), h, cos(w)*mix(W, 0.0, h/H)));
            pyramidMesh.pushPosition(vec3(sin(w+wdelta)*mix(W, 0.0, h/H), h, cos(w+wdelta)*mix(W, 0.0, h/H)));
        }
    }
    for (float w=0.0; w<PI2; w+=wdelta) {
            pyramidMesh.pushPosition(vec3(sin(w)*W, 0.0, cos(w)*W));
            pyramidMesh.pushPosition(vec3(0.0, H, 0.0));
    }
    
    pyramid = new StaticModel(pyramidMesh);
}
