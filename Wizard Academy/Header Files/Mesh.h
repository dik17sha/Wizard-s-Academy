//
//  Mesh.h
//  Wizard Academy
//
//  Created by Diksha Chottani on 11/2/25.
//
#ifndef MESH_H
#define MESH_H

#include<GLUT/glut.h>
#include<glm/glm.hpp>
#include<string>
#include<vector>
#include"Shader.h"

//The GLSL attributes, setting up DS for them
struct vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture{
    unsigned int id;
    std::string type;
    std::string path;
};

//Mesh Class - Setting up for the managing of VAO, VBO, EBO
class Mesh{
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int VAO;
    
    Mesh(std::vector<Vector> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        
        setupMesh();
        
    }
    
    void Draw(Shader& shader)
    {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.sizes()));
        glBindVertexArray(0);
    }
    
private:
    unsigned int VBO, EBO;
    
    void setupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        //Bind VAO (stores the state of VBO and EBO
        glBindVertexArray(VAO);
        
        //Bind and Fill VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof )
    }
}
