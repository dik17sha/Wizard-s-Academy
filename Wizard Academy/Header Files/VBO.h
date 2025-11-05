//
//  VBO.h
//  Wizard Academy
//
//  Created by Diksha Chottani on 11/4/25.
//
#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<GLUT/glut.h>

class VBO
{
public:
    //Refrence ID for VBO
    GLuint ID;
    
    //Constructor
    VBO(GLfloat* vertices, GLsizeiptr size);
    
    
    //helper functions
    void Bind();
    void Unbind();
    void Delete();
};

#endif
