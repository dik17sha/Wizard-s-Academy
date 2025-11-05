//
//  shader.cpp
//  Wizard Academy
//
//  Created by Diksha Chottani on 11/4/25.
//

#include"shader.h"

std::string get_file_contents(const char* filename)
{
    std::ifstream in(filename, std::ios::binary);
    if(in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0,std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    
    throw(errno);
    
}

std::string get_file_contents(const char* filename);

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);
    
    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();
    
    //Vertex Work
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    
    //Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    
    //Creating shader program object and getting the reference
    ID = glCreateProgram();
    
    //Attaching both the shaders to the new shader program cretated above
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    
    //Linking all the shaders to the the program
    glLinkProgram(ID);
    
    //Deleting the vertex and fragment shader that are not in use
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
}

void Shader::Activate()
{
    glUseProgram(ID);
}

void Shader::Delete()
{
    glDeleteProgram(ID);
}

