#pragma once

#include "demo_part.hpp"
#include "program.hpp"
#include "mvp.hpp"
#include "vectors.hpp"
#include "texture.hpp"
#include "scaling_rectangle.hpp"

#define N_LINES 110

class PartLogo: public DemoPart {
    public:
        PartLogo(float t);
        ~PartLogo();
        virtual void draw();
        
    private:
        Program shader;
        Program shader2;
        MVP mvp;
        StaticModel* grid;
        ScalingRectangle logoRect;
        Texture logo;
};
