#include "ObjLoader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

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

MeshObj* ObjLoader::loadObjFile(std::string fileName, std::string ID) {
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
  
  // setup variables used for parsing //
  std::string key;
  float x, y, z;
  int i, j, k;

  // setup local lists //
  std::vector<Vertex> vertexList;
  std::vector<unsigned int> indexList;

  // setup tools for parsing a line correctly //
  std::string line;
  std::stringstream sstr;

  // open file //
  std::ifstream file(fileName.c_str());
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
        vertexList.push_back(Vertex(x, y, z));
      }
      if (!key.compare("f")) {
        // read in vertex indices for a face //
        sstr >> i >> j >> k;
        indexList.push_back(i - 1);
        indexList.push_back(j - 1);
        indexList.push_back(k - 1);
      }
    }
    file.close();
    std::cout << "Imported " << indexList.size() / 3 << " faces from \"" << fileName << "\"" << std::endl;
    
    // reconstruct normals from given vertex data //
    reconstructNormals(vertexList, indexList);
    
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
  // iterate over all faces defined by the indexList vector (index triplets define one face)   //
  // compute a normal for every face
  // accumulate these face normals for every incident vertex to a particular face
  // finally normalize all vertex normals
  std::vector< vec3f_t > faceNormals;
  for(unsigned int i = 0; i < indexList.size(); i += 3) {
    vec3f_t edge0;
    vec3f_t edge1;
    vec3f_t normal;
    // Calculate two edges of triangle:
    edge0.x = vertexList.at(indexList.at(i + 1)).position[0] - vertexList.at(indexList.at(i + 0)).position[0];
    edge0.y = vertexList.at(indexList.at(i + 1)).position[1] - vertexList.at(indexList.at(i + 0)).position[1];
    edge0.z = vertexList.at(indexList.at(i + 1)).position[2] - vertexList.at(indexList.at(i + 0)).position[2];
    edge1.x = vertexList.at(indexList.at(i + 2)).position[0] - vertexList.at(indexList.at(i + 0)).position[0];
    edge1.y = vertexList.at(indexList.at(i + 2)).position[1] - vertexList.at(indexList.at(i + 0)).position[1];
    edge1.z = vertexList.at(indexList.at(i + 2)).position[2] - vertexList.at(indexList.at(i + 0)).position[2];
    // Use these for face normal calculation via the cross product:
    normal.x = edge0.y * edge1.z - edge0.z * edge1.y;
    normal.y = edge0.z * edge1.x - edge0.x * edge1.z;
    normal.z = edge0.x * edge1.y - edge0.y * edge1.x;
    // Normalize normal:
    float length = std::sqrt(std::pow(normal.x, 2) + std::pow(normal.y, 2) + std::pow(normal.z, 2));
    normal.x /= length;
    normal.y /= length;
    normal.z /= length;
    faceNormals.push_back(normal);
  }
  for(std::vector< unsigned int >::size_type i = 0; i < indexList.size(); i++) {
    Vertex &v = vertexList.at(indexList.at(i));
    if(v.normal[0] == 0 && v.normal[1] == 0 && v.normal[2] == 0) {
    	v.normal[0] = faceNormals.at(i / 3).x;
    	v.normal[1] = faceNormals.at(i / 3).y;
    	v.normal[2] = faceNormals.at(i / 3).z;
    }
    else {
    	v.normal[0] = (v.normal[0] + faceNormals.at(i / 3).x) / 2;
    	v.normal[1] = (v.normal[1] + faceNormals.at(i / 3).y) / 2;
    	v.normal[2] = (v.normal[2] + faceNormals.at(i / 3).z) / 2;
    }
  }
  for(std::vector< Vertex >::size_type i = 0; i < vertexList.size(); i++) {
    // Normalize normal:
    Vertex &v = vertexList.at(i);
    float length = std::sqrt(std::pow(v.normal[0], 2) + std::pow(v.normal[1], 2) + std::pow(v.normal[2], 2));
    v.normal[0] /= length;
    v.normal[1] /= length;
    v.normal[2] /= length;
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
