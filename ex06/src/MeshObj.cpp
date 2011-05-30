#include "MeshObj.h"
#include <iostream>
#include <limits>

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
  
  mIndexCount = indexData.size();
  
  // XXX: init and bind a VBO (vertex buffer object) //
  glGenBuffers(1, &mVBO);
  glBindBuffer(GL_ARRAY_BUFFER, mVBO);
  
  // XXX: copy data into the VBO //
  glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex), &vertexData[0], GL_STATIC_DRAW);
  
  // XXX: init and bind a IBO (index buffer object) //
  glGenBuffers(1, &mIBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
  
  // XXX: copy data into the IBO //
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(unsigned int), &indexData[0], GL_STATIC_DRAW);
  
  // unbind buffers //
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshObj::render(void) {
  if (mVBO != 0) {
    // XXX: init vertex attribute arrays for vertex position, normal vector and texture coordinate //
    // enable if needed //
    glClientActiveTexture(GL_TEXTURE0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    // bind the buffer for rendering //
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    // XXX: bind the index buffer object mIBO here to tell OpenGL to use the indices in that array for indexing elements in our vertex buffer //
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);

    // give pointers for vertices, normals, colors, texture coordinates, if necessary //
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), NULL);
    glNormalPointer(GL_FLOAT, sizeof(Vertex), (GLvoid*)(sizeof(GLfloat) * 3));
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid*)(sizeof(GLfloat) * 6));
    
    // XXX: render VBO as triangles //
    glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, NULL);

    // unbind the buffers //
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
}

float MeshObj::getWidth(void) {
  return mMaxBounds[0];
}

float MeshObj::getHeight(void) {
  return mMaxBounds[1] - mMinBounds[1]; 
}

float MeshObj::getDepth(void) {
  return mMaxBounds[2] - mMinBounds[2]; 
}
