#pragma once

#include "demo.hpp"
#include "program.hpp"
#include "texture.hpp"
#include "mvp.hpp"
#include "demo_part.hpp"
#include "static_model.hpp"
#include "lighting_state.hpp"

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
