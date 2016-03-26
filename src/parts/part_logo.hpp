#pragma once

#include "demo_part.hpp"
#include "program.hpp"
#include "mvp.hpp"
#include "vectors.hpp"
#include "texture.hpp"
#include "scaling_rectangle.hpp"

#define N_LINES 110
#define N_STARS 200

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
