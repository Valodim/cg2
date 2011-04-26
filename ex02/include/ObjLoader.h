#ifndef __OBJ_LOADER__
#define __OBJ_LOADER__

#include <map>
#include <string>

#include "MeshObj.h"

class ObjLoader {
    public:
        ObjLoader();
        ~ObjLoader();
        MeshObj* loadObjFile(std::string fileName, std::string ID = "");
        MeshObj* getMeshObj(std::string ID);
    protected:
        static bool loadObjFile(std::string fileName, std::vector<Vertex> &vertexList, std::vector<unsigned int> &indexList);
    private:
        std::map<std::string, MeshObj*> mMeshMap;
};

#endif
