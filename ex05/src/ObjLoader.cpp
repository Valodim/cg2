#include "ObjLoader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cassert>
#include <cstring>

ObjLoader::ObjLoader() {
}

ObjLoader::~ObjLoader() {
  // thanks to Vincent for pointing out the broken destructor ... erasing pointers without deleting the referenced object is baaaaad //
  for (std::map<std::string, MeshObj*>::iterator iter = mMeshMap.begin(); iter != mMeshMap.end(); ++iter) {
    delete iter->second;
    iter->second = NULL;
  }
  mMeshMap.clear();
}

// Splits a string by slashes
int slashSplit(char* string, char** ret, int max) {
    // m = ret index
    // n = string mark
    // i = running variable
    int m = 0, n = 0, i = 0;
    // while there is string
    while(string[i] != 0 && m < max) {
        if(string[i] == '/') {
            ret[m] = new char[i-n+1];
            // copy string from mark to counter (this may be zero!)
            strncpy(ret[m], string+n, i-n);
            ret[m++][i-n] = 0;
            // set new mark
            n = i+1;
        }
        i++;
    }
    if(i > n) {
        ret[m] = new char[i-n+1];
        strncpy(ret[m], string+n, i-n);
        ret[m++][i-n] = 0;
    }
    return m;
}

int mainn() {
    char x[] = "a//c/dddd"; // second element is empty!
    char** ret = new char*[4];
    int n;
    n = slashSplit(x, ret, 4);
    for(int i = 0; i < n; i++)
        std::cout << i << ": " << ret[i] << std::endl;
}

MeshObj* ObjLoader::loadObjFile(std::string fileName, std::string ID, float scale) {
  // sanity check for identfier -> must not be empty //
  if (ID.length() == 0) {
    return NULL;
  }
  // try to load the MeshObj for ID //
  MeshObj* obj = getMeshObj(ID);
  if (obj != NULL) {
    // if found, return it instead of loading a new one from file //
    return obj;
  }
  // ID is not known yet -> try to load mesh from file //
  
  // XXX: extend your objLoader to load vertices, vertex normals and texture coordinates
  //       note: faces using normals and texture coordinates are defines as "f vi0/ti0/ni0 ... viN/tiN/niN"
  //       vi0 .. viN : vertex index of vertex 0..N
  //       ti0 .. tiN : texture coordinate index of vertex 0..N
  //       ni0 .. niN : vertex normal index of vertex 0..N
  //       faces without normals and texCoords are defined as  "f vi0// ... viN//"
  //       faces without texCoords are defined as              "f vi0//ni0 ... viN//niN"
  //       make your parser robust against ALL possible combinations
  //       also allow to import QUADS as faces. directly split them up into two triangles!
  
  // setup variables used for parsing //
  std::string key;
  char _;
  char word[256];
  float x, y, z;
  unsigned int vi[4];
  unsigned int ni[4];
  unsigned int ti[4];

  // setup local lists                        //
  // import all data in temporary lists first //
  std::vector<Point3D> localVertexList;
  std::vector<Point3D> localNormalList;
  std::vector<Point3D> localTexCoordList;
  std::vector<Face> localFaceList;

  // setup tools for parsing a line correctly //
  std::string line;
  std::stringstream sstr;

  // open file //
  std::ifstream file(fileName.c_str());
  unsigned int lineNumber = 0;
  if (file.is_open()) {
    while (file.good()) {
      key = "";
      getline(file, line);
      sstr.clear();
      sstr.str(line);
      sstr >> key;
      if (!key.compare("v")) {
        // read in vertex //
        sstr >> x >> y >> z;
        localVertexList.push_back(Point3D(x * scale, y * scale, z * scale));
      }
      // XXX: implement parsing of vertex normals and texture coordinates //
      if (!key.compare("vn")) {
        // read in vertex //
        sstr >> x >> y >> z;
        localNormalList.push_back(Point3D(x * scale, y * scale, z * scale));
      }
      if (!key.compare("vt")) {
        // read in vertex //
        sstr >> x >> y;
        localTexCoordList.push_back(Point3D(x * scale, y * scale, 0.0f));
      }

      if (!key.compare("f")) {
        // XXX: implement a robust method to load a face definition               //
        //       allowing vertex normals, tecture coordinates, triangles and quads //

        // read in vertex indices for a face //
        int i = 0;
        for(i = 0; i < 4; i++) {
            if(sstr.eof())
                break;

            // Step 1: get one word. this may be either v1, v1/t1/, v1/t1/n1 or v1//n1
            sstr >> word;
            char* buf[3];
            // Step 2: Split up word by /
            int n = slashSplit(word, buf, 3);
            // std::cout << "dbg: " << buf[0] << ", " << buf[1] << ", " << buf[2] << std::endl;
            // Step 3: for each element between the /, try to parse the int
            if(!sscanf(buf[0], "%d", &vi[i]))
                vi[i] = 0;
            if(!sscanf(buf[1], "%d", &ti[i]))
                ti[i] = 0;
            if(!sscanf(buf[2], "%d", &ni[i]))
                ni[i] = 0;
            std::cout << "got " << n << " results: " << vi[i] << ", " << ti[i] << ", " << ni[i] << std::endl;
        }

        // XXX: directly split up polygons using 4 vertices into two triangles //
        // Hardcore splitting action going on here
        if(i == 4) {
            // Two faces: one with x1, x2, x3 - the other with x2, x3, x4

            std::cout << "splitting up quad" << std::endl;

            // So that means we add the x2, x3, x4 here, the other one as usual
            Face f;
            memcpy(f.vIndex, vi+1, sizeof(unsigned int) * 3);
            memcpy(f.nIndex, ni+1, sizeof(unsigned int) * 3);
            memcpy(f.tIndex, ti+1, sizeof(unsigned int) * 3);

            //       add all imported faces to 'localFaceList'                      //
            localFaceList.push_back(f);
        }

        // Otherwise, it's just a plain old triangle
        Face f;
        memcpy(f.vIndex, vi, sizeof(unsigned int) * 3);
        memcpy(f.nIndex, ni, sizeof(unsigned int) * 3);
        memcpy(f.tIndex, ti, sizeof(unsigned int) * 3);

        //       add all imported faces to 'localFaceList'                      //
        localFaceList.push_back(f);

      }
    }
    file.close();
    std::cout << "Imported " << localFaceList.size() << " faces from \"" << fileName << "\"" << std::endl;
    
    // XXX: every face is able to use a different set of vertex normals and texture coordinates
    //       when using a single vertex for multiple faces, however, this conflicts multiple normals
    //       rearrange and complete the imported data in the following way:
    //        - if a vertex uses multiple normals and/or texture coordinates, create copies of that vertex
    //        - every triplet (vertex-index, textureCoord-index, normal-index) is unique and indexed by indexList
    // create vertex list for vertex buffer object //
    // one vertex definition per index-triplet (vertex index, texture index, normal index) //
    
    std::vector<Vertex> vertexList;
    std::vector<unsigned int> indexList;
    
    for (std::vector<Face>::iterator faceIter = localFaceList.begin(); faceIter != localFaceList.end(); ++faceIter) {
      // XXX: rearrange and complete data, when conflicting combinations of vertex and vertex attributes occur //
      for(unsigned int i = 0; i < 3; i++) {
        Vertex v;
        v.position[0] = localVertexList.at(faceIter->vIndex[i] - 1).data[0];
        v.position[1] = localVertexList.at(faceIter->vIndex[i] - 1).data[1];
        v.position[2] = localVertexList.at(faceIter->vIndex[i] - 1).data[2];
        v.normal[0] = localNormalList.at(faceIter->nIndex[i] - 1).data[0];
        v.normal[1] = localNormalList.at(faceIter->nIndex[i] - 1).data[1];
        v.normal[2] = localNormalList.at(faceIter->nIndex[i] - 1).data[2];
        v.texcoord[0] = localTexCoordList.at(faceIter->tIndex[i] - 1).data[0];
        v.texcoord[1] = localTexCoordList.at(faceIter->tIndex[i] - 1).data[1];
        std::vector< Vertex >::size_type vertexIndex = 0;
        for(vertexIndex = 0; vertexIndex < vertexList.size(); vertexIndex++) {
          if(v == vertexList.at(vertexIndex))
            break;
        }
        if(vertexIndex < vertexList.size()) {
          indexList.push_back(vertexIndex);
        }
        else {
          vertexList.push_back(v);
          indexList.push_back(vertexList.size() - 1);
        }
      }
    }
    
    // reconstruct normals from given vertex data (only if no normals have been imported) //
    if (localNormalList.size() == 0) reconstructNormals(vertexList, indexList);
    
    // create new MeshObj and set imported geoemtry data //
    obj = new MeshObj();
    obj->setData(vertexList, indexList);
    
    // insert MeshObj into map //
    mMeshMap.insert(std::make_pair(ID, obj));
    
    // return newly created MeshObj //
    return obj;
  } else {
    std::cout << "(ObjLoader::loadObjFile) : Could not open file: \"" << fileName << "\"" << std::endl;
    return NULL;
  }
}

void ObjLoader::reconstructNormals(std::vector<Vertex> &vertexList, const std::vector<unsigned int> &indexList) {
  // iterator over faces (given by index triplets) and calculate normals for each incident vertex //
  for (unsigned int i = 0; i < (indexList.size() - 3); i += 3) {
    // face edges incident with vertex 0 //
    GLfloat edge0[3] = {vertexList[indexList[i+1]].position[0] - vertexList[indexList[i]].position[0],
                        vertexList[indexList[i+1]].position[1] - vertexList[indexList[i]].position[1],
                        vertexList[indexList[i+1]].position[2] - vertexList[indexList[i]].position[2]};
    GLfloat edge1[3] = {vertexList[indexList[i+2]].position[0] - vertexList[indexList[i]].position[0],
                        vertexList[indexList[i+2]].position[1] - vertexList[indexList[i]].position[1],
                        vertexList[indexList[i+2]].position[2] - vertexList[indexList[i]].position[2]};
    normalizeVector(edge0);
    normalizeVector(edge1);
    // compute normal using cross product //
    GLfloat normal[3] = {edge0[1] * edge1[2] - edge0[2] * edge1[1],
                         edge0[2] * edge1[0] - edge0[0] * edge1[2],
                         edge0[0] * edge1[1] - edge0[1] * edge1[0]};
    normalizeVector(normal);
    
    // add this normal to all face-vertices //
    for (int j = 0; j < 3; ++j) {
      vertexList[indexList[i]].normal[j] += normal[j];
      vertexList[indexList[i+1]].normal[j] += normal[j];
      vertexList[indexList[i+2]].normal[j] += normal[j];
    }
  }

  // normalize all normals //
  for (unsigned int i = 0; i < indexList.size(); ++i) {
    normalizeVector(vertexList[indexList[i]].normal);
  }
}

MeshObj* ObjLoader::getMeshObj(std::string ID) {
  // sanity check for ID //
  if (ID.length() > 0) {
    std::map<std::string, MeshObj*>::iterator mapLocation = mMeshMap.find(ID);
    if (mapLocation != mMeshMap.end()) {
      // mesh with given ID already exists in meshMap -> return this mesh //
      return mapLocation->second;
    }
  }
  // no MeshObj found for ID -> return NULL //
  return NULL;
}

void ObjLoader::normalizeVector(float *vector, int dim) {
  float length = 0.0f;
  for (int i = 0; i < dim; ++i) {
    length += powf(vector[i],2);
  }
  length = sqrt(length);
  
  if (length != 0) {
    for (int i = 0; i < dim; ++i) {
      vector[i] /= length;
    }
  }
}
