#include "ObjLoader.h"

#include <iostream>
#include <fstream>
#include <sstream>

// see 'http://en.wikipedia.org/wiki/Wavefront_.obj_file' for specifications
bool loadObjFile(const char *fileName, std::vector<Vertex> &vertexList, std::vector<int> &indexList) {

    // Clear those
    vertexList.clear();
    indexList.clear();

    std::ifstream ifs(fileName, std::ifstream::in);

    int lineno = 0;
    // We parse this line-wise
    while (ifs.good() && !ifs.eof()) {
        char buf[512];
        char typechar;

        // Buffer a single line
        ifs.getline(buf, 512);
        lineno += 1;

        // Parseable line
        std::stringstream line(buf, std::stringstream::in);

        line >> typechar;
        if(line.fail()) {
            // End of file? Break out of loop.
            if(ifs.eof())
                break;
            // Leave an error msg, and return false.
            std::cerr << "Parse failure at line " << lineno << std::endl;
            return false;
        }
        switch(typechar) {
            case 'v': // Vertex line
                // Expecting three floats for a vertex
                float x, y, z;
                line >> x >> y >> z;
                if(line.fail()) {
                    std::cerr << "line " << lineno << ": faulty vertex!" << std::endl;
                    return false;
                }

                // Save in vertexList
                vertexList.push_back(Vertex(x, y, z));
                std::cout << "line " << lineno << ": got a vertex: " << x << ", " << y << ", " << z << std::endl;
                break;
            case 'f':
                // Expecting three index ints for a face
                int f1, f2, f3;
                line >> f1 >> f2 >> f3;
                if(line.fail()) {
                    std::cerr << "line " << lineno << ": faulty face!" << std::endl;
                    return false;
                }

                // Save in indexList
                indexList.push_back(f1 -1);
                indexList.push_back(f2 -1);
                indexList.push_back(f3 -1);
                std::cout << "line " << lineno << ": got a face: " << f1 << ", " << f2 << ", " << f3 << std::endl;
                break;

            case 'o': // Material / Object
                break;
            case 'g': // Material / Group
                break;
            case 'm': // Material / mtllib
                break;
            case 'u': // Material / usemtl
                break;
            case 's': // Smooth shading on/off... huh.
                break;
            case '#': // Comment line
                break;
            default: // Default means not implemented - shouldn't happen?
                std::cout << "line " << lineno << ": Undefined character, parse error: " << typechar << std::endl;
                return false;
        }

    }

    ifs.close();

    return true;

}
