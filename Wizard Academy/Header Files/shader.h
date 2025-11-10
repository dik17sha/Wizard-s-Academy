//
//  shader.h
//  Wizard Academy
//
//  Created by Diksha Chottani on 11/2/25.
//

#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>


std::string get_file_contents(const char* filename);

class Shader
{
public:
    GLuint ID;
    
    Shader(const char* vertexFile,  const char* fragmentFile);
    
    //helper functions
    void Activate();
    void Delete();
    
    
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value)const;
    void setFloat(const std::string &name, float value)const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
     
private:
    void checkCompileErrors(unsigned int shader, std::string type);
    
};

#endif 

/*
class Shader
{
public:
    unsigned int ID;
    
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        //Retrieve the vertex/fragment source code from the file path
        std::string vertexCode;
        std::string fragmentShader;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        
        //Ensure ifstream objects can throw expectations
        vShaderFile.expectations (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.expectations (std::ifstream::failbit | std::ifstream::badbit)
        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            
            std::stringstream vShaderStream, fShaderStream;
            
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            
            vShaderFile.close();
            fShaderFile.close();
            
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << "\nCheck file paths: " << vertexPath << "and" << fragmentPath << std::endl;
        }
        
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        
        // Compile shaders
        unsigned int vertex, fragment;
        
        //Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX")
        
        //Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment,"FRAGMENT");
        
        //Shader Program Linkeer
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        
        //Deleting the individual shaders after linking
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        
    }
    
    //Activates the shader program for use
    
    void use() const
    {
        glUseProgram(ID);
    }
    
    // Utility functions to set uniform variables on the GPU
    
    void setIN(const std::string &name, int value)const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    
    void setVec3(const std::string &name, const glm::vec3 &value)const
    {
        glUnifrom3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    
    void setMat4(const std::string &name, const glm::mat4 &mat)const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    
private:
    //Utility function for cheking shader compilation/linking errors.
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infolog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infolog << "\n -- ---------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infolog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infolog << "\n -- ---------------------------- -- " << std::endl;
            }
        }
            
    }
};

#endif

 */
