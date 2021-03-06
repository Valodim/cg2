#ifndef __MESH_OBJ__
#define __MESH_OBJ__

#include <GL/glew.h>
#include <GL/glut.h>

#include <vector>

struct Vertex {
  Vertex(float x = 0, float y = 0, float z = 0, float nx = 1, float ny = 0, float nz = 0, float tx = 0, float ty = 0) {
    position[0] = (GLfloat)x;
    position[1] = (GLfloat)y;
    position[2] = (GLfloat)z;
    normal[0] = (GLfloat)nx;
    normal[1] = (GLfloat)ny;
    normal[2] = (GLfloat)nz;
    texcoord[0] = (GLfloat)tx;
    texcoord[1] = (GLfloat)ty;
    tangent[0] = (GLfloat)0;
    tangent[1] = (GLfloat)0;
    tangent[2] = (GLfloat)0;
    bitangent[0] = (GLfloat)0;
    bitangent[1] = (GLfloat)0;
    bitangent[2] = (GLfloat)0;
  }
  GLfloat position[3];
  GLfloat normal[3];
  GLfloat texcoord[2];
  GLfloat tangent[3];
  GLfloat bitangent[3];

  void addTangent(float a[]) {
      this->tangent[0] += a[0];
      this->tangent[1] += a[1];
      this->tangent[2] += a[2];
  }
  void addBitangent(float a[]) {
      this->bitangent[0] += a[0];
      this->bitangent[1] += a[1];
      this->bitangent[2] += a[2];
  }
};

class MeshObj {
  public:
    MeshObj();
    ~MeshObj();
    
    void setData(const std::vector<Vertex> &vertexData, const std::vector<unsigned int> &indexData);
    void render(GLint att_position, GLint att_normal, GLint att_texcoord, GLint att_tangent, GLint att_bitangent);
    
    float getWidth(void);
    float getHeight(void);
    float getDepth(void);
  private:
    GLuint mVBO;
    GLuint mIBO;
    GLuint mIndexCount;
    float mMinBounds[3];
    float mMaxBounds[3];
};

#endif
