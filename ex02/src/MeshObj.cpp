#include "MeshObj.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>

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
  // TODO: compute the loaded meshes bounds that may be used later on and store them in mMinBounds, mMaxBounds resp. //
  
  // TODO: COPY the data from vertexData and indexData in own data vectors mVertexData and mIndexData //
  mVertexData = std::vector< Vertex >(vertexData);
  mIndexData = std::vector< unsigned int >(indexData);

  // Calculate extents of this mesh object:
  for(unsigned int i = 0; i < 3; i++) {
    GLfloat minValue = mVertexData.begin()->position[i];
    GLfloat maxValue = mVertexData.begin()->position[i];
    for(std::vector< Vertex >::const_iterator itVertex = mVertexData.begin(); itVertex != mVertexData.end(); itVertex++) {
      minValue = std::min(minValue, itVertex->position[i]);
      maxValue = std::max(maxValue, itVertex->position[i]);
    }
    mExtents[i] = std::abs(maxValue - minValue);
  }
}

void MeshObj::render(void) {
  // render the data stored in this object //
  // - use glBegin(GL_TRIANGLES) ... glEnd() to render every triangle indexed by the mIndexData list //
  /*
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, mVertexData.data());
  glBegin(GL_TRIANGLES);
    for(std::vector< unsigned int >::size_type i = 0; i < mIndexData.size(); i += 3) {
      glArrayElement(mIndexData.at(i));
    }
  glEnd();
  */
  glBegin(GL_TRIANGLES);
    for(std::vector< unsigned int >::size_type i = 0; i < mIndexData.size(); i += 3) {
      glVertex3f( mVertexData.at(mIndexData.at(i + 0)).position[0]
                , mVertexData.at(mIndexData.at(i + 0)).position[1]
                , mVertexData.at(mIndexData.at(i + 0)).position[2]
                );
      glVertex3f( mVertexData.at(mIndexData.at(i + 1)).position[0]
                , mVertexData.at(mIndexData.at(i + 1)).position[1]
                , mVertexData.at(mIndexData.at(i + 1)).position[2]
                );
      glVertex3f( mVertexData.at(mIndexData.at(i + 2)).position[0]
                , mVertexData.at(mIndexData.at(i + 2)).position[1]
                , mVertexData.at(mIndexData.at(i + 2)).position[2]
                );
    }
  glEnd();
}

float MeshObj::getWidth(void) {
  return mExtents[0];
}

float MeshObj::getHeight(void) {
  return mExtents[1];
}

float MeshObj::getDepth(void) {
  return mExtents[2];
}
