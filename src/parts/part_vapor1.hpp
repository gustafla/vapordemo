#pragma once

#include "demo.hpp"
#include "3dapp_program.hpp"
#include "3dapp_texture.hpp"
#include "3dapp_framebuffer.hpp"
#include "3dapp_mvp.hpp"
#include "demo_part.hpp"
#include "3dapp_static_model.hpp"
#include "3dapp_lighting_state.hpp"
#include <vector>

struct Cloud {
    Cloud(float, float, float, float, int);
    float scale;
    float speed;
    int texture;
    float x, y;
    static bool compareScale(Cloud&, Cloud&);
};

#define NUM_CLOUDS 22
#define CLOUD_RANGE 4

class PartVapor1: public DemoPart {
    public:
        PartVapor1(float t);
        ~PartVapor1();
        virtual void draw();
        
    private:
        void genMeshes();
        void genPillarMesh();
        
        //Background
        Program shaderSimple;
        Texture bgTexture;
        
        //Sun
        Texture sunTexture;
        
        //Trees
        Texture treeTexture;
        Framebuffer treeFBO;
        ScalingRectangle shadowRect;
        
        //Clouds
        std::vector<Texture*> cloudTextures;
        std::vector<Cloud> clouds;
        ScalingRectangle cloudRect;
        
        //Pillars
        StaticModel* pillar;
        MVP mvp;
        Program* shader;
        Texture pillarTexture;
        LightingState lights;
        
        //Fractal tunnel
        Program shaderMvp;
		Texture fracTexture;
};
