//
//  EBO.h
//  Wizard Academy
//
//  Created by Diksha Chottani on 11/4/25.
//
#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glad/glad.h>

class EBO
{
public:
    //Reference ID of EBO
    GLuint ID;
    
    //Constructor for generating the EBO
    EBO(GLuint* indices, GLsizeiptr size);
    
    //Helper Functions
    void Bind();
    void Unbind();
    void Delete();
    
};

#endif
