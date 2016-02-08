#pragma once

#include "demo_part.hpp"
#include "program.hpp"

class PartLogo: public DemoPart {
    public:
        PartLogo(float t);
        virtual void draw();
        
    private:
        Program mode7;
};
