#pragma once

#include "demo.hpp"
#include "program.hpp"
#include "texture.hpp"
#include "mvp.hpp"
#include "demo_part.hpp"
#include "static_model.hpp"
#include "lighting_state.hpp"

class PartVapor1: public DemoPart {
    public:
        PartVapor1(float t);
        ~PartVapor1();
        virtual void draw();
        
    private:
        void genPillarMesh();
        StaticModel* pillar;
        
        MVP mvp;
        Program* shader;
        Texture pillarTexture;
        LightingState lights;
};
