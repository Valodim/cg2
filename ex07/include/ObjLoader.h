#ifndef __OBJ_LOADER__
#define __OBJ_LOADER__

#include <map>
#include <vector>
#include <string>

#include "MeshObj.h"

struct Point3D {
  Point3D(float x = 0, float y = 0, float z = 0) {
    data[0] = x;
    data[1] = y;
    data[2] = z;
  }
  Point3D(float const* const d) {
    data[0] = d[0];
    data[1] = d[1];
    data[2] = d[2];
  }
  float data[3];

  Point3D operator+(Point3D const &a) {
      Point3D p(this->data);
      p.data[0] += a.data[0];
      p.data[1] += a.data[1];
      p.data[2] += a.data[2];
      return p;
  }

  Point3D operator-(Point3D const &a) {
      Point3D p(this->data);
      p.data[0] -= a.data[0];
      p.data[1] -= a.data[1];
      p.data[2] -= a.data[2];
      return p;
  }

};

struct Face {
  unsigned int vIndex[3];
  unsigned int nIndex[3];
  unsigned int tIndex[3];
};

class ObjLoader {
  public:
    ObjLoader();
    ~ObjLoader();
    MeshObj* loadObjFile(std::string fileName, std::string ID = "", float scale = 1.0f);
    MeshObj* getMeshObj(std::string ID);
  private:
    void reconstructNormals(std::vector<Vertex> &vertexList, const std::vector<unsigned int> &indexList);
    void computeTangentSpace(std::vector<Vertex> &vertexList, const std::vector<unsigned int> &indexList);
    void normalizeVector(float *vector, int dim = 3);
    
    std::map<std::string, MeshObj*> mMeshMap;
};

#endif
