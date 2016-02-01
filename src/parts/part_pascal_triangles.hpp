#pragma once

#include "demo.hpp"
#include "program.hpp"

class PartPascalTriangles: public DemoDrawable {
    public:
        PartPascalTriangles();
        virtual void draw();
    private:
        Program shader;
};
