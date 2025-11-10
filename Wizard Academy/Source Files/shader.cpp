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
    // Provide a descriptive C++ exception instead of throwing a raw int.
    std::string message = std::string("Failed to open file: ") + filename + " (" + std::to_string(errno) + ")";
    throw std::runtime_error(message);
}

std::string get_file_contents(const char* filename);

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
    std::cout << "Loading shaders: " << vertexFile << " and " << fragmentFile << std::endl;
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);
    
    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();
    
    int success;
    char infoLog[1024];
    
    //Vertex Work
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    } else {
        std::cout << "Vertex shader compiled successfully." << std::endl;
    }
    checkCompileErrors(vertexShader,"VERTEX");
    
    //Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    } else {
        std::cout << "Fragment shader compiled successfully." << std::endl;
    }
    checkCompileErrors(fragmentShader, "FRAGMENT");
    
    //Creating shader program object and getting the reference
    ID = glCreateProgram();
    
    //Attaching both the shaders to the new shader program cretated above
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    
    //Linking all the shaders to the the program
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 1024, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    } else {
        std::cout << "Shader program linked successfully. Program ID: " << ID << std::endl;
    }
    
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

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if(type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        }
    }
}

void Shader::setBool(const std::string &name, bool value)const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    
}

void Shader::setInt(const std::string &name, int value)const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value)const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat)const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value)const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

