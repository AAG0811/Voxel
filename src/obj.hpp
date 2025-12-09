#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

// class OBJModel {
// public:
//     std::vector<Vertex> vertices;
//     std::vector<unsigned int> indices;
//
//     bool load(const std::string& filename) {
//         std::ifstream file(filename);
//         if (!file.is_open()) {
//             std::cerr << "Failed to open file: " << filename << std::endl;
//             return false;
//         }
//
//         std::string line;
//         while (std::getline(file, line)) {
//             std::istringstream iss(line);
//             std::string prefix;
//             iss >> prefix;
//
//             if (prefix == "v") {
//                 // Read vertex position (v)
//                 Vertex vertex;
//                 iss >> vertex.x >> vertex.y >> vertex.z;
//                 // Set default normal values (these will be overwritten later)
//                 vertex.nx = vertex.ny = vertex.nz = 0.0f;
//                 // Set default texture coordinates (to be overwritten later)
//                 vertex.u = vertex.v = 0.0f;
//                 vertices.push_back(vertex);
//             }
//             else if (prefix == "vn") {
//                 // Read normal vector (vn)
//                 float nx, ny, nz;
//                 iss >> nx >> ny >> nz;
//                 // Update the last vertex with the normal
//                 if (!vertices.empty()) {
//                     vertices.back().nx = nx;
//                     vertices.back().ny = ny;
//                     vertices.back().nz = nz;
//                 }
//             }
//             else if (prefix == "vt") {
//                 // Read texture coordinates (vt)
//                 float u, v;
//                 iss >> u >> v;
//                 // Update the last vertex with the texture coordinates
//                 if (!vertices.empty()) {
//                     vertices.back().u = u;
//                     vertices.back().v = v;
//                 }
//             }
//             else if (prefix == "f") {
//                 // Read face indices (f) and store them in the indices array
//                 unsigned int vIndex[3], tIndex[3], nIndex[3];
//                 char slash;
//                 for (int i = 0; i < 3; i++) {
//                     iss >> vIndex[i] >> slash >> tIndex[i] >> slash >> nIndex[i];
//                     // OBJ indices are 1-based, so we need to subtract 1 to make them 0-based
//                     indices.push_back(vIndex[i] - 1);
//                 }
//             }
//         }
//
//         file.close();
//         return true;
//     }
//
//     void loadToVAO(GLuint& VAO, GLuint& VBO, GLuint& EBO) {
//         glGenVertexArrays(1, &VAO);
//         glGenBuffers(1, &VBO);
//         glGenBuffers(1, &EBO);
//
//         glBindVertexArray(VAO);
//
//         glBindBuffer(GL_ARRAY_BUFFER, VBO);
//         glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
//
//         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//         glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
//
//         // Position attribute
//         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//         glEnableVertexAttribArray(0);
//
//         // Normal attribute
//         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, nx));
//         glEnableVertexAttribArray(1);
//
//         // Texture coordinate attribute
//         glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
//         glEnableVertexAttribArray(2);
//
//         glBindVertexArray(0);
//     }
// };

inline bool loadOBJ(const std::string &filename, 
             std::vector<Vertex> &vertices, 
             std::vector<unsigned int> &indices) {

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open obj file: " << filename << std::endl;
        return false;
    }

    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_texCoords;
    std::vector<glm::vec3> temp_normals;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            glm::vec3 position;
            ss >> position.x >> position.y >> position.z;
            temp_positions.push_back(position);
        } 
        else if (prefix == "vt") {
            glm::vec2 texCoord;
            ss >> texCoord.x >> texCoord.y;
            texCoord.y = 1.0 - texCoord.y;
            temp_texCoords.push_back(texCoord);
        } 
        else if (prefix == "vn") {
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        } 
        else if (prefix == "f") {
            unsigned int vIndex[3], tIndex[3], nIndex[3];
            char slash;
            for (int i = 0; i < 3; ++i) {
                ss >> vIndex[i] >> slash >> tIndex[i] >> slash >> nIndex[i];
                // Indices in obj file are 1-based, OpenGL uses 0-based indices.
                vIndex[i]--; tIndex[i]--; nIndex[i]--;
            }

            // Create vertices from the faces
            for (int i = 0; i < 3; ++i) {
                Vertex vertex;
                vertex.position = temp_positions[vIndex[i]];
                vertex.normal = temp_normals[nIndex[i]];
                vertex.texCoords = temp_texCoords[tIndex[i]];
                vertices.push_back(vertex);
                indices.push_back(indices.size());  // Adding the index to the index buffer
            }
        }
    }

    file.close();
    return true;
}

inline unsigned int createVAO(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices) {
    unsigned int VAO, VBO, EBO;
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex Buffer Object (VBO)
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // Element Buffer Object (EBO)
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Vertex Position Attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    
    // Texture Coordinates Attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(2);
    
    // Normal Attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    return VAO;
}