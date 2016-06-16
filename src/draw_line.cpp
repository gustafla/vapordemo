#include "draw_line.hpp"

void drawLine(Program& shader, float x1, float y1, float x2, float y2) {
    float vec[6];
    vec[0] = x1;
    vec[1] = y1;
    vec[2] = 0.0f;
    vec[3] = x2;
    vec[4] = y2;
    vec[5] = 0.0f;

    glEnableVertexAttribArray(shader.getAtrHandle("a_vertex"));
    glVertexAttribPointer(shader.getAtrHandle("a_vertex"), 3, GL_FLOAT, GL_TRUE, sizeof(GLfloat)*3, vec);
    glDrawArrays(GL_LINES, 0, 2);

}

void drawLine3D(Program& shader, float x1, float y1, float z1, float x2, float y2, float z2) {
    float vec[6];
    vec[0] = x1;
    vec[1] = y1;
    vec[2] = z1;
    vec[3] = x2;
    vec[4] = y2;
    vec[5] = z2;

    glEnableVertexAttribArray(shader.getAtrHandle("a_vertex"));
    glVertexAttribPointer(shader.getAtrHandle("a_vertex"), 3, GL_FLOAT, GL_TRUE, sizeof(GLfloat)*3, vec);
    glDrawArrays(GL_LINES, 0, 2);

}
