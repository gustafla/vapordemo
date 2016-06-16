#ifndef DRAW_LINE_HPP
#define DRAW_LINE_HPP

#include "3dapp_graphics.hpp"
#include "3dapp_program.hpp"

void drawLine(Program& shader, float x1, float y1, float x2, float y2);
void drawLine3D(Program& shader, float x1, float y1, float z1, float x2, float y2, float z2);

#endif
