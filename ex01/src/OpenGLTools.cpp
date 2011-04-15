#include "ObjLoader.h"
#include "OpenGLTools.h"

void renderVertexArray(const std::vector<Vertex> &vertexArray, const std::vector<int> &indexList) {
    glBegin(GL_TRIANGLES);
    for(unsigned int i = 0; i < indexList.size(); i += 3) {
        Vertex x;

        x = vertexArray[indexList[i]];
        glVertex3d(x.position[0], x.position[1], x.position[2]);
        x = vertexArray[indexList[i+1]];
        glVertex3d(x.position[0], x.position[1], x.position[2]);
        x = vertexArray[indexList[i+2]];
        glVertex3d(x.position[0], x.position[1], x.position[2]);

    }
    glEnd();
}
