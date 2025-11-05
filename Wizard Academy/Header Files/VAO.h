//
//  VAO.h
//  Wizard Academy
//
//  Created by Diksha Chottani on 11/4/25.
//
#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<GLUT/glut.h>
#include"VBO.h"

class VAO
{
public:
    //Refrence ID for VAO
    GLuint ID;
    
    //Constructor
    VAO();
    
    //Linker for VBO and VAO
    void LinkVBO(VBO& VBO, GLuint layout);
    
    //helper functions
    void Bind();
    void Unbind();
    void Delete();

};

#endif

