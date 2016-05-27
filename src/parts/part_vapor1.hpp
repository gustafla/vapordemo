#pragma once

#include "demo.hpp"
#include "3dapp_program.hpp"
#include "3dapp_texture.hpp"
#include "3dapp_mvp.hpp"
#include "demo_part.hpp"
#include "3dapp_static_model.hpp"
#include "3dapp_lighting_state.hpp"

#define NUM_CLOUDS 100
#define STR_CLOUDS 5

class PartVapor1: public DemoPart {
    public:
        PartVapor1(float t);
        ~PartVapor1();
        virtual void draw();
        
    private:
        void genMeshes();
        void genPillarMesh();
        StaticModel* pillar;
        
        MVP mvp;
        Program* shader;
        Texture pillarTexture;
        LightingState lights;
        
        Program shaderMvp;
		Texture frac;
        
        float clouds[NUM_CLOUDS*5];
        Texture cloud;
};
