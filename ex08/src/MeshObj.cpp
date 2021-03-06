#include "MeshObj.h"
#include "ObjLoader.h"
#include <iostream>
#include <limits>
#include <cmath>
#include <cstring>

MeshObj::MeshObj() {
  mVBO = 0;
  mIBO = 0;
  mIndexCount = 0;
  for (int i = 0; i < 3; ++i) {
    mMinBounds[i] = std::numeric_limits<float>::max();
    mMaxBounds[i] = std::numeric_limits<float>::min();
  }
}

MeshObj::~MeshObj() {
  glDeleteBuffers(1, &mIBO);
  glDeleteBuffers(1, &mVBO);
}

void MeshObj::setData(const std::vector<Vertex> &vertexData, const std::vector<unsigned int> &indexData) {
  // compute bounds //
  for (int i = 0; i < 3; ++i) {
    mMinBounds[i] = std::numeric_limits<float>::max();
    mMaxBounds[i] = std::numeric_limits<float>::min();
  }
  for (unsigned int i = 0; i < vertexData.size(); ++i) {
    for (int j = 0; j < 3; ++j) {
      if (vertexData[i].position[j] < mMinBounds[j]) mMinBounds[j] = vertexData[i].position[j];
      if (vertexData[i].position[j] > mMaxBounds[j]) mMaxBounds[j] = vertexData[i].position[j];
    }
  }
  
  // copy given vertex data to local storage //
  mVertexData.assign(vertexData.begin(), vertexData.end());
  mIndexData.assign(indexData.begin(), indexData.end());
  mIndexCount = indexData.size();
  
  // init and bind a VBO (vertex buffer object) //
  if (mVBO == 0) {
    glGenBuffers(1, &mVBO);
  }
  glBindBuffer(GL_ARRAY_BUFFER, mVBO);
  // copy data into the VBO //
  glBufferData(GL_ARRAY_BUFFER, mVertexData.size() * sizeof(Vertex), &mVertexData[0], GL_STATIC_DRAW);
  
  // init and bind a IBO (index buffer object) //
  if (mIBO == 0) {
    glGenBuffers(1, &mIBO);
  }
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
  // copy data into the IBO //
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexCount * sizeof(GLint), &mIndexData[0], GL_STATIC_DRAW);
  
  // unbind buffers //
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshObj::render(void) {
  if (mVBO != 0) {
    // init vertex attribute arrays //
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));
    
    // bind the index buffer object mIBO here //
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    
    // render VBO as triangles //
    glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, (void*)0);
    
    // unbind the element render buffer //
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // unbind the vertex array buffer //
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
}

void MeshObj::initShadowVolume(GLfloat lightPos[4]) {
  // XXX: init the shadow volume for a given light source position 'lightPos' //
  Point3D light = Point3D(lightPos);
  
  // XXX: clone all vertices and compute their projections //
  //       -> store as NEW local array of 'vertex'          //
  std::vector<Vertex> projectionVertices = std::vector<Vertex>(mVertexData);
  unsigned int pSize = projectionVertices.size();
  // Make room for twice as many
  projectionVertices.resize(pSize * 2);
  std::copy(mVertexData.begin(), mVertexData.end(), &projectionVertices[pSize]);
  for(unsigned int i = 0; i < pSize; i++) {
    Vertex& v = projectionVertices[i+pSize];
    Point3D pos = Point3D(v.position);
    pos = pos + (pos - light).normalized() * 1000;
    memcpy(v.position, pos.data, 3*sizeof(float));
  }

  // XXX: compute shadow volume faces (6 resp. 8 triangles depending on technique) //
  //       -> check correct face orientation                                        //
  //       -> create new faces by adding indices to a NEW index array               //
  std::vector<unsigned int> projectionIndices;
  projectionIndices.reserve(mIndexCount * 6);
  for(unsigned int i = 0, j = 0; i < mIndexCount; i += 3, j += 18) {
    Point3D v01 = Point3D(projectionVertices[mIndexData[i+1]].position) -Point3D(projectionVertices[mIndexData[i+0]].position);
    Point3D v02 = Point3D(projectionVertices[mIndexData[i+2]].position) -Point3D(projectionVertices[mIndexData[i+0]].position);

    Point3D trinorm = v01.cross(v02);
    Point3D trilight = Point3D(projectionVertices[mIndexData[i+0]].position) -light;

    int off = (trinorm * trilight) < 0.0f ? +2 : 0;

    projectionIndices[j +0 +off] = mIndexData[i+0];
    projectionIndices[j +1] = mIndexData[i+0] + pSize;
    projectionIndices[j +2 -off] = mIndexData[i+2] + pSize;

    projectionIndices[j +3 +off] = mIndexData[i+2] + pSize;
    projectionIndices[j +4] = mIndexData[i+2];
    projectionIndices[j +5 -off] = mIndexData[i+0];

    projectionIndices[j +6 +off] = mIndexData[i+1];
    projectionIndices[j +7] = mIndexData[i+1] + pSize;
    projectionIndices[j +8 -off] = mIndexData[i+0] + pSize;

    projectionIndices[j +9 +off] = mIndexData[i+0] + pSize;
    projectionIndices[j+10] = mIndexData[i+0];
    projectionIndices[j+11 -off] = mIndexData[i+1];

    projectionIndices[j+12 +off] = mIndexData[i+2];
    projectionIndices[j+13] = mIndexData[i+2] + pSize;
    projectionIndices[j+14 -off] = mIndexData[i+1] + pSize;

    projectionIndices[j+15 +off] = mIndexData[i+1] + pSize;
    projectionIndices[j+16] = mIndexData[i+1];
    projectionIndices[j+17 -off] = mIndexData[i+2];
  }
  
  // XXX: store the index count for indexed vertex buffer rendering to 'mShadowIndexCount' //
  mShadowIndexCount = 6 * mIndexCount;
  
  // XXX: setup VBO ('mShadowVBO') and IBO ('mShadowIBO') for the computed data //
  if (mShadowVBO == 0) {
    glGenBuffers(1, &mShadowVBO);
  }
  glBindBuffer(GL_ARRAY_BUFFER, mShadowVBO);
  // copy data into the VBO //
  glBufferData(GL_ARRAY_BUFFER, projectionVertices.size() * sizeof(Vertex), &projectionVertices[0], GL_STATIC_DRAW);
  
  // init and bind a IBO (index buffer object) //
  if (mShadowIBO == 0) {
    glGenBuffers(1, &mShadowIBO);
  }
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mShadowIBO);
  // copy data into the IBO //
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mShadowIndexCount * sizeof(GLint), &projectionIndices[0], GL_STATIC_DRAW);
  
  // unbind buffers //
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshObj::renderShadowVolume() {
  // XXX: render the shadow volume //
  if (mShadowVBO != 0) {
    // init vertex attribute arrays //
    glBindBuffer(GL_ARRAY_BUFFER, mShadowVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    // glEnableVertexAttribArray(2);
    // glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    
    // bind the index buffer object mIBO here //
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mShadowIBO);
    
    // render VBO as triangles //
    glDrawElements(GL_TRIANGLES, mShadowIndexCount, GL_UNSIGNED_INT, (void*)0);
    
    // unbind the element render buffer //
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // unbind the vertex array buffer //
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
}

float MeshObj::getWidth(void) {
  return mMaxBounds[0] - mMinBounds[0];
}

float MeshObj::getHeight(void) {
  return mMaxBounds[1] - mMinBounds[1]; 
}

float MeshObj::getDepth(void) {
  return mMaxBounds[2] - mMinBounds[2]; 
}
