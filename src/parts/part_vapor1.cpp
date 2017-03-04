#include "part_vapor1.hpp"
#include "3dapp_tga_file.hpp"
#include "3dapp_obj_file.hpp"
#include "3dapp_paths.hpp"
#include "3dapp_gfx_utils.hpp"
#include "3dapp_uniforms.hpp"
#include "3dapp_matrices.hpp"
#include "3dapp_geo_primitives.hpp"
#include "3dapp_util.hpp"
#include <cmath>
#include <cstdlib>
#include <algorithm>

#define PI2 (2*DEMO_PI)

static const Light pointLights[] = {
};

static const Light directionalLights[] = {
    Light(vec3(0.2, -0.2, -1.0), vec3(0.6, 0.6, 0.6))
};

Cloud::Cloud(float _x, float _y, float _scale, float _speed, int _texture):
x(_x),
y(_y),
scale(_scale),
speed(_speed),
texture(_texture) {
    
}

bool Cloud::compareScale(Cloud& a, Cloud& b) {
    return a.scale < b.scale;
}

PartVapor1::PartVapor1(float t):
DemoPart(t),
lights(0.2f, pointLights, sizeof(pointLights)/sizeof(pointLights[0]), directionalLights, sizeof(directionalLights)/sizeof(directionalLights[0])),
pillarTexture(loadTGAFile(texturePath("marble.tga"))),
shaderMvp(Shader::loadFromFile(shaderPath("generic.vert")), Shader::loadFromFile(shaderPath("generic.frag"))), 
shaderSimple(Shader::loadFromFile(shaderPath("simple.vert")), Shader::loadFromFile(shaderPath("generic.frag"))), 
shaderWave(Shader::loadFromFile(shaderPath("simple.vert")), Shader::loadFromFile(shaderPath("wavetex.frag"))), 
fracTexture(loadTGAFile(texturePath("trianglefrac.tga"))),
bgTexture(loadTGAFile(texturePath("seabg.tga"))),
sunTexture(loadTGAFile(texturePath("sun.tga"))),
treeTexture(loadTGAFile(texturePath("palm.tga"))),
shadowRect(vec2(DEMO_W, DEMO_H), vec2(DEMO_W,DEMO_H), 1., vec2(-0.03,-0.02)),
cloudRect(vec2(128, 64), vec2(DEMO_W,DEMO_H)),
treeFBO(DEMO_W, DEMO_H),
mvp(getPProjMat(45, DEMO_W/DEMO_H, 0.1, 10.0)) {
    genMeshes();
    
    cloudTextures.push_back(new Texture(loadTGAFile(texturePath("cloud1.tga"))));
    cloudTextures.push_back(new Texture(loadTGAFile(texturePath("cloud1f.tga"))));
    cloudTextures.push_back(new Texture(loadTGAFile(texturePath("cloud2.tga"))));
    cloudTextures.push_back(new Texture(loadTGAFile(texturePath("cloud2f.tga"))));
    cloudTextures.push_back(new Texture(loadTGAFile(texturePath("cloud3.tga"))));
    cloudTextures.push_back(new Texture(loadTGAFile(texturePath("cloud3f.tga"))));
    
    for(int i=0; i<NUM_CLOUDS; i++) {
        float scale = randf()*0.25+0.35;
        clouds.push_back(
            Cloud(randf()*CLOUD_RANGE, //x
            randf()*0.3+1.1, //y
            scale, //scale
            (scale-0.06)*1.2, //speed
            rand()%cloudTextures.size()) //texture
        );
        std::sort(clouds.begin(), clouds.end(), Cloud::compareScale);
        /*Could use depth testing but scaling is more linear
          and predictable */
    }
    
    shader = new Program(Shader::loadFromFile(shaderPath("generic.vert"), lights), Shader::loadFromFile(shaderPath("generic.frag"), lights));
    shader->use();
    
    setBaseUniforms(*shader, 1, lights);
    setBaseUniforms(shaderMvp);
    setBaseUniforms(shaderSimple);
}

PartVapor1::~PartVapor1() {
    delete pillar;
    delete shader;
    for (std::vector<Texture*>::iterator it = cloudTextures.begin(); it != cloudTextures.end(); ++it) {
        delete (*it);
    }
}

void PartVapor1::draw() {
    Sync& sync = Demo::singleton().getSync();
    float speed = sync.getValue(SYNC_PART_VAPOR1_SPEED, DEMO_T());
    
    //Draw BG
    shaderWave.use();
    setTimeUniform(shaderWave, DEMO_T());
    setColorUniform(shaderWave, vec4(0,0,0,0));
    bgTexture.bindToUnit(0);
    GeoPrimitives::singleton().quad.draw(shaderWave);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    //Draw sun
    shaderMvp.use();
    setColorUniform(shaderMvp, vec4(0,0,0,0));
    mvp.reset();
    mvp.setView(0,0,-4,0,0,0);
    mvp.setModelTranslation(1.5,1.6,0);
    mvp.setModelRotation(0,0,DEMO_T()*0.2*speed);
    mvp.apply(shaderMvp);
    sunTexture.bindToUnit(0);
    GeoPrimitives::singleton().quad.draw(shaderMvp);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    //Draw clouds as well
    setColorUniform(shaderMvp, vec4(0));
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    //glBlendFunc(GL_SRC_COLOR, GL_ONE);
    for(int i=0; i<NUM_CLOUDS; i++) {
        cloudTextures[clouds[i].texture]->bindToUnit(0);
        setOpacityUniform(shaderMvp, 1.0-clouds[i].scale);
        float x = fmod(DEMO_T()*clouds[i].speed*speed+clouds[i].x, CLOUD_RANGE*2)-CLOUD_RANGE;
        mvp.setModel(x,clouds[i].y,-2,0,0,0,clouds[i].scale);
        mvp.apply(shaderMvp);
        cloudRect.draw(shaderMvp);
        glClear(GL_DEPTH_BUFFER_BIT);
    }
    setOpacityUniform(shaderMvp, 1);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
        
    
    //Draw trees to FBO
    treeFBO.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderMvp.use();
    treeTexture.bindToUnit(0);
    mvp.reset();
    mvp.setView(0,0,-4,0,0,0);
    float xrange=4.0;
    for (int i=0; i<8; i++) {
        float x = fmod(-10.4+i*0.6+DEMO_T()*speed, xrange*2)-xrange;
        mvp.setModelTranslation(x, -0.8-x*x*0.1, 0+x*x*0.2);
        mvp.setModelRotation(0,0, sin(x*2.0)*0.1);
        mvp.apply(shaderMvp);
        vec3 color1 = vec3(0.8, 0.5, 0.1);
        vec3 color2 = vec3(0.3, 0.7, 0.3);
        setColorUniform(shaderMvp, vec4(mix(color1,color2,cos(i*DEMO_PI*0.5)*0.5+0.5),1));
        GeoPrimitives::singleton().quad.draw(shaderMvp);
        glClear(GL_DEPTH_BUFFER_BIT);
    }
    
    //Draw trees and shadow to screen
    Demo::singleton().getLastFBO().bind();
    shaderSimple.use();
    treeFBO.getTexture().bindToUnit(0);
    setColorUniform(shaderSimple, vec4(vec3(0),1));
    shadowRect.draw(shaderSimple);
    glClear(GL_DEPTH_BUFFER_BIT);
    setColorUniform(shaderSimple, vec4(0));
    GeoPrimitives::singleton().quad.draw(shaderSimple);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    //Draw tunnel
    /*vec4 ca = vec4(vec3(0,.1,.6), 1.0);
	vec4 cb = vec4(vec3(.5,0,.2), 1.0);
    shaderMvp.use();
    fracTexture.bindToUnit(0);
    
    mvp.reset();
    float I=18;
    for(int i=0; i<I; i++) {
		mvp.setModel(0, 0.0, fmod((i*1.0f)+sync.getValue(SYNC_PART_TRIANGLES_TUNNEL_DEPTH, DEMO_T()), I)-I+1, 0.0, sync.getValue(SYNC_PART_TRIANGLES_MAIN_ROTATION, DEMO_T()), (2*3.1415)*0.125+sync.getValue(SYNC_PART_TRIANGLES_TUNNEL_DEPTHROT, DEMO_T())*(i/I));
		mvp.setViewTranslation(sync.getValue(SYNC_PART_TRIANGLES_VIEW_X, DEMO_T()), sync.getValue(SYNC_PART_TRIANGLES_VIEW_Y, DEMO_T()));
        mvp.apply(shaderMvp);
        setColorUniform(shaderMvp, mix(ca, cb, sin(i/(I)*3.1415*2.0)*0.5+0.5));
		GeoPrimitives::singleton().quad.draw(shaderMvp);
	}*/
}

float getW(float y, float r) {
    return 0.3*std::cos(y*0.2*DEMO_PI)+std::sin(r*16.0)*0.01;
}

float getWEnd(float y, float r) {
    return 0.45;
}

void pushTrianglePositionNormal(Mesh& mesh, vec3 a, vec3 b, vec3 c) {
    mesh.pushPosition(a);
    mesh.pushPosition(b);
    mesh.pushPosition(c);
    vec3 normal = triangleNormal(a, b, c);
    for (int i=0; i<3; i++)
        mesh.pushNormal(normal);
}

void fillCylindricalMesh(Mesh& mesh, const float STRIP_H, const float STRIP_W, float start, float stop, float radiusFunc(float,float)) {
    for (float y=start; y<stop; y+=STRIP_H) {
        for (float r=0.0; r<PI2; r+=STRIP_W) {
            pushTrianglePositionNormal( mesh,
                vec3(std::sin(r)*radiusFunc(y,r), y, std::cos(r)*radiusFunc(y,r)),
                vec3(std::sin(r+STRIP_W)*radiusFunc(y,r+STRIP_W), y, std::cos(r+STRIP_W)*radiusFunc(y,r+STRIP_W)),
                vec3(std::sin(r+STRIP_W)*radiusFunc(y+STRIP_H,r+STRIP_W), y+STRIP_H, std::cos(r+STRIP_W)*radiusFunc(y+STRIP_H,r+STRIP_W))
            );
            mesh.pushTexcoord(vec2(((r)/PI2), (y+1)/2));
            mesh.pushTexcoord(vec2(((r+STRIP_W)/PI2), (y+1)/2));
            mesh.pushTexcoord(vec2(((r+STRIP_W)/PI2), (y+STRIP_H+1)/2));
                
            pushTrianglePositionNormal( mesh,
                vec3(std::sin(r+STRIP_W)*radiusFunc(y+STRIP_H,r+STRIP_W), y+STRIP_H, std::cos(r+STRIP_W)*radiusFunc(y+STRIP_H,r+STRIP_W)),
                vec3(std::sin(r)*radiusFunc(y+STRIP_H,r), y+STRIP_H, std::cos(r)*radiusFunc(y+STRIP_H,r)),
                vec3(std::sin(r)*radiusFunc(y,r), y, std::cos(r)*radiusFunc(y,r))
            );
            mesh.pushTexcoord(vec2(((r+STRIP_W)/PI2), (y+STRIP_H+1)/2));
            mesh.pushTexcoord(vec2(((r)/PI2), (y+STRIP_H+1)/2));
            mesh.pushTexcoord(vec2(((r)/PI2), (y+1)/2));
        }
    }
}

void fillCircularMesh(Mesh& mesh, bool up, const float STRIP_W, float y, float radius) {
    vec3 a, b, c;
    for (float r=0.0; r<PI2; r+=STRIP_W) {
        if (up) {
            a = vec3(0.0f, y, 0.0f);
            b = vec3(std::cos(r+STRIP_W)*radius, y, std::sin(r+STRIP_W)*radius);
            c = vec3(std::cos(r)*radius, y, std::sin(r)*radius);
            mesh.pushTexcoord(vec2(0.5, 0.5));
            mesh.pushTexcoord(vec2(std::cos(r+STRIP_W)/2+1, std::sin(r+STRIP_W)/2+1));
            mesh.pushTexcoord(vec2(std::cos(r)/2+1, std::sin(r)/2+1));
        } else {
            a = vec3(0.0f, y, 0.0f);
            c = vec3(std::sin(r)*radius, y, std::cos(r)*radius);
            b = vec3(std::sin(r+STRIP_W)*radius, y, std::cos(r+STRIP_W)*radius);
            mesh.pushTexcoord(vec2(0.5, 0.5));
            mesh.pushTexcoord(vec2(std::cos(r)/2+1, std::sin(r)/2+1));
            mesh.pushTexcoord(vec2(std::cos(r+STRIP_W)/2+1, std::sin(r+STRIP_W)/2+1));
        }
        pushTrianglePositionNormal(mesh, a, b, c);
    }
}

void fillPlanarMesh(Mesh& mesh, bool up, const float STRIP_W, const float STRIP_H, float height, float w, float h) {
    vec3 a, b, c;
    if (up) {
        for (float x=-w/2; x<w/2; x+=STRIP_W) {
            for (float y=-h/2; y<h/2; y+=STRIP_H) {
                a = vec3(x, height, y);
                b = vec3(x+STRIP_W, height, y+STRIP_H);
                c = vec3(x+STRIP_W, height, y);
                mesh.pushTexcoord(vec2(x/w+0.5, y/w+0.5));
                mesh.pushTexcoord(vec2((x+STRIP_W)/w+0.5, (y+STRIP_H)/w+0.5));
                mesh.pushTexcoord(vec2((x+STRIP_W)/w+0.5, y/w+0.5));
                pushTrianglePositionNormal(mesh, a, b, c);
                
                a = vec3(x, height, y);
                b = vec3(x, height, y+STRIP_H);
                c = vec3(x+STRIP_W, height, y+STRIP_H);
                mesh.pushTexcoord(vec2(x/w+0.5, y/w+0.5));
                mesh.pushTexcoord(vec2(x/w+0.5, (y+STRIP_H)/w+0.5));
                mesh.pushTexcoord(vec2((x+STRIP_W)/w+0.5, (y+STRIP_H)/w+0.5));
                pushTrianglePositionNormal(mesh, a, b, c);
            }
        }
    } else {
        for (float x=-w/2; x<w/2; x+=STRIP_W) {
            for (float y=-h/2; y<h/2; y+=STRIP_H) {
                a = vec3(x, height, y);
                b = vec3(x+STRIP_W, height, y);
                c = vec3(x+STRIP_W, height, y+STRIP_H);
                mesh.pushTexcoord(vec2(x/w+0.5, y/w+0.5));
                mesh.pushTexcoord(vec2((x+STRIP_W)/w+0.5, y/w+0.5));
                mesh.pushTexcoord(vec2((x+STRIP_W)/w+0.5, (y+STRIP_H)/w+0.5));
                pushTrianglePositionNormal(mesh, a, b, c);
                
                a = vec3(x, height, y);
                b = vec3(x+STRIP_W, height, y+STRIP_H);
                c = vec3(x, height, y+STRIP_H);
                mesh.pushTexcoord(vec2(x/w+0.5, y/w+0.5));
                mesh.pushTexcoord(vec2((x+STRIP_W)/w+0.5, (y+STRIP_H)/w+0.5));
                mesh.pushTexcoord(vec2(x/w+0.5, (y+STRIP_H)/w+0.5));
                pushTrianglePositionNormal(mesh, a, b, c);
            }
        }
    }
}

void PartVapor1::genPillarMesh() {
    Mesh mesh;
    
    const float STRIP_H = 0.5;
    const float STRIP_W = 0.1;
    const float HALF_PI = DEMO_PI/2.0;
    
    fillCylindricalMesh(mesh, STRIP_H, STRIP_W, -1.0, 1.0, getW);
    fillCylindricalMesh(mesh, 0.1, HALF_PI, -1.1, -0.9, getWEnd);
    fillCylindricalMesh(mesh, 0.1, HALF_PI, 0.9, 1.1, getWEnd);
    fillCircularMesh(mesh, true, HALF_PI, 1.1, 0.45);
    fillCircularMesh(mesh, false, HALF_PI, 0.9, 0.45);
    fillCircularMesh(mesh, true, HALF_PI, -0.9,0.45);
    fillCircularMesh(mesh, false, HALF_PI, -1.1, 0.45);
    
    pillar = new StaticModel(mesh);
}

void PartVapor1::genMeshes() {
    genPillarMesh();
}
