#pragma once

#include "demo_part.hpp"
#include "3dapp_program.hpp"
#include "3dapp_mvp.hpp"
#include "3dapp_vectors.hpp"
#include "3dapp_texture.hpp"
#include "3dapp_scaling_rectangle.hpp"

#define N_LINES 110
#define N_STARS 100

class PartLogo: public DemoPart {
    public:
        PartLogo(float t);
        ~PartLogo();
        virtual void draw();
        
    private:
        void genMeshes();
        Program shader;
        Program shaderFlare;
        Program shaderPyramid;
        Program shader2;
        Program shaderStarfield;
        MVP mvp;
        StaticModel* grid;
        float starfield[N_STARS*3];
        ScalingRectangle logoRect;
        Texture logo;
        Texture flare;
        StaticModel* pyramid;
};
