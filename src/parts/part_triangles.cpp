#include "part_triangles.hpp"
#include "shader.hpp"
#include "geo_primitives.hpp"
#include "paths.hpp"
#include "vectors.hpp"
#include <iostream>
#include <cmath>
#include "demo_consts.hpp"
#include "tga_file.hpp"
#include "gfx_utils.hpp"
#include "uniforms.hpp"

PartTriangles::PartTriangles(float t):
DemoPart(t),
shaderMvp(shaderPath("generic.vert"), shaderPath("generic.frag")), 
shaderSimple(shaderPath("simple.vert"), shaderPath("generic.frag")), 
frac(loadTGAFile(texturePath("trianglefrac.tga"))),
bg(loadTGAFile(texturePath("vapor.tga"))),
mvp(getPProjMat(45, ((1.0f*DEMO_W)/(1.0f*DEMO_H)), 0.1, 40.0)) {
	setUniforms(shaderMvp);
	setUniforms(shaderSimple);
    
    shaderSimple.use();
    glUniform4f(shaderSimple.getUfmHandle("color"), 0.0, 0.0, 0.0, 0.4);
	mvp.setView();
}

void PartTriangles::draw() {
    Sync& sync = Demo::singleton().getSync();

    shaderSimple.use();
    bg.bindToUnit(0);
    GeoPrimitives::singleton().quad.draw(shaderSimple);
    glClear(GL_DEPTH_BUFFER_BIT);

	vec4 ca = vec4(DEMO_C_DBLUE, 1.0);
	vec4 cb = vec4(DEMO_C_VPINK, 1.0);
	vec4 c;
    shaderMvp.use();
    frac.bindToUnit(0);
    
    float I=18;
    for(int i=0; i<I; i++) {
		c=mix(ca, cb, sin(i/(I)*3.1415*2.0)*0.5+0.5);
		mvp.setModel(sync.getValue(SYNC_PART_TRIANGLES_TUNNEL_DEPTHPAN, DEMO_T()-start)*pow((1.0-i/I), 2.0), 0.0, fmod((i*1.0f)+sync.getValue(SYNC_PART_TRIANGLES_TUNNEL_DEPTH, DEMO_T()-start), I)-I+1, 0.0, sync.getValue(SYNC_PART_TRIANGLES_MAIN_ROTATION, DEMO_T()-start), (2*3.1415)*0.125+sync.getValue(SYNC_PART_TRIANGLES_TUNNEL_DEPTHROT, DEMO_T()-start)*(i/I));
		mvp.setViewTranslation(sync.getValue(SYNC_PART_TRIANGLES_VIEW_X, DEMO_T()-start), sync.getValue(SYNC_PART_TRIANGLES_VIEW_Y, DEMO_T()-start));
        mvp.apply(shaderMvp);
		glUniform4fv(shaderMvp.getUfmHandle("color"), 1, (GLfloat*)&c);
		GeoPrimitives::singleton().quad.draw(shaderMvp);
	}
}
