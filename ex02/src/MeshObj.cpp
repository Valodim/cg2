#include "MeshObj.h"
#include <iostream>
#include <limits>
#include <algorithm>

MeshObj::MeshObj() {
  mVertexData.clear();
  mIndexData.clear();
  for (int i = 0; i < 3; ++i) {
    mMinBounds[i] = std::numeric_limits<float>::max();
    mMaxBounds[i] = std::numeric_limits<float>::min();
  }
}

MeshObj::~MeshObj() {
  mVertexData.clear();
  mIndexData.clear();
}

void MeshObj::setData(const std::vector<Vertex> &vertexData, const std::vector<unsigned int> &indexData) {
    // compute the loaded meshes bounds that may be used later on and store them in mMinBounds, mMaxBounds resp. //
    for(std::vector<unsigned int>::const_iterator it = indexData.begin(); it != indexData.end(); it++) {
        if(vertexData[*it].position[0] < mMinBounds[0])
            mMinBounds[0] = vertexData[*it].position[0];
        if(vertexData[*it].position[1] < mMinBounds[1])
            mMinBounds[1] = vertexData[*it].position[1];
        if(vertexData[*it].position[2] < mMinBounds[2])
            mMinBounds[2] = vertexData[*it].position[2];

        if(vertexData[*it].position[0] > mMaxBounds[0])
            mMaxBounds[0] = vertexData[*it].position[0];
        if(vertexData[*it].position[1] > mMinBounds[1])
            mMaxBounds[1] = vertexData[*it].position[1];
        if(vertexData[*it].position[2] > mMinBounds[2])
            mMaxBounds[2] = vertexData[*it].position[2];
    }

    // COPY the data from vertexData and indexData in own data vectors mVertexData and mIndexData //
    this->mVertexData = std::vector<Vertex>(vertexData);
    this->mIndexData = std::vector<unsigned int>(indexData);
}

void MeshObj::render(void) {
    // render the data stored in this object //
    // - use glBegin(GL_TRIANGLES) ... glEnd() to render every triangle indexed by the mIndexData list //

    glBegin(GL_TRIANGLES);
    for(unsigned int i = 0; i < mIndexData.size(); i += 3) {
        Vertex x;

        x = mVertexData[mIndexData[i]];
        glVertex3d(x.position[0], x.position[1], x.position[2]);
        x = mVertexData[mIndexData[i+1]];
        glVertex3d(x.position[0], x.position[1], x.position[2]);
        x = mVertexData[mIndexData[i+2]];
        glVertex3d(x.position[0], x.position[1], x.position[2]);

    }
    glEnd();

}

float MeshObj::getWidth(void) {
    return mMaxBounds[0] -mMinBounds[0];
}

float MeshObj::getHeight(void) {
    return mMaxBounds[1] -mMinBounds[1];
}

float MeshObj::getDepth(void) {
    return mMaxBounds[2] -mMinBounds[2];
}
