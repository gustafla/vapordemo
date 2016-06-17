#pragma once

#include "demo.hpp"
#include "3dapp_program.hpp"
#include "3dapp_texture.hpp"
#include "3dapp_framebuffer.hpp"
#include "3dapp_mvp.hpp"
#include "demo_part.hpp"
#include "3dapp_static_model.hpp"
#include "3dapp_lighting_state.hpp"

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
        ScalingRectangle rectTfm;
        
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
