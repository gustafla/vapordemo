#pragma once

class DemoPart {
    public:
        DemoPart(float t);
        virtual void draw();
    
    protected:
        float start;
};
