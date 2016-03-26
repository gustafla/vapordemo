#pragma once

#include "demo.hpp"
#include "program.hpp"
#include "texture.hpp"
#include "mvp.hpp"

class PartTriangles: public DemoPart {
    public:
        PartTriangles(float t);
        virtual void draw();
        
    private:

		MVP mvp;
};
