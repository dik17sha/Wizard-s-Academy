//  main.cpp
//  Wizard Academy
//
//  Created by Diksha Chottani on 11/2/25.
//Still running the test program
//the traingle


#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"shader.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include "Camera.h"

//Window Settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

//Calling Camera object
Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 3.0f));

//Creating Function Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xPos, double yPos);
//void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

//Test
// 8 vertices * (3 positions + 3 colors) = 48 floats

GLfloat cube_vertices[] = {
    //Positions                         //Colors(R,G,B)
    -0.5f, -0.5f, -0.5f,                1.0f, 0.0f, 0.0f,  //Back Bottom Left
    0.5f, -0.5f, -0.5f,                 1.0f, 0.0f, 0.0f,  //Back Bottom Right
    0.5f, 0.5f, -0.5f,                  1.0f, 0.0f, 0.0f,  //Back Top Right
    -0.5f, 0.5f, -0.5f,                 1.0f, 0.0f, 0.0f,  //Back Top Left
    
    -0.5f, 0.5f, 0.5f,                  0.0f, 1.0f, 0.0f,  //Front Bottom Left
    0.5f, -0.5f, 0.5f,                  0.0f, 1.0f, 0.0f,  //Front Bottom Right
    0.5f, 0.5f, 0.5f,                   0.0f, 1.0f, 0.0f,  //Front Top Right
    -0.5f, 0.5f, 0.5f,                  0.0f, 1.0f, 0.0f   //Front Top Left
    
};

//Indcies for the Cube (12 triangles * 3 indices = 36 indices)
GLuint cubeIndices[] = {
    //Back Face
    0, 1, 2,
    2, 3, 0,
    //Front Face
    4, 5, 6,
    6, 7, 4,
    //Right Face
    1, 5, 6,
    6, 2, 1,
    //Left Face
    0, 4, 7,
    7, 3, 0,
    //Top Face
    3, 2, 6,
    6, 7, 3,
    //Bottom Face
    0, 1, 5,
    5, 4, 1
};

int main()
{
    // 1. Initializing GLFW
    if(!glfwInit()){
        std::cerr << "Failed to initialize GLFW" << std ::endl;
        return -1;
    }
    
    //Setting up openGL verison -- only becaue of MAC
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // 2. Creating the Window and Glad loading
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Wizard's Academy Test", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    //Setting callback functions for resizing and mouse input
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetScrollCallback(window, scroll_callback);
    
    //Locking cursor to the center of the screen
    glfwSetInputMode(window, GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    
    //Loading GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // 3. Global OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    // 4. Setup for the Cube Geometry using the classes
    std::cout << "Setting up shader..." << std::endl;
    Shader defaultShader("basic.vs", "basic.fs");
    
    std::cout << "Setting up cube geometry..." << std::endl;
    VAO cubeVAO;
    cubeVAO.Bind();
    
    VBO cubeVBO(cube_vertices, sizeof(cube_vertices));
    EBO cubeEBO(cubeIndices, sizeof(cubeIndices));
    
    //Linking Position Attribute (layout 0)
    cubeVAO.LinkVBO(cubeVBO, 0);
    
    //Linking color Attribute
    cubeVAO.LinkVBO(cubeVBO, 1);
    
    //cubeVAO.LinkVBO(cubeVBO, 0, 3, GL_FLOAT, 6*sizeof(float),(void*)0);
    //cubeVAO.LinkVBO(cubeVBO, 1, 3, GL_FLOAT, 6*sizeof(float),(void*)0);
    
    cubeVAO.Unbind();
    cubeVBO.Unbind();
    cubeEBO.Unbind();
    
    std::cout << "Starting render loop..." << std::endl;
    int frame_count = 0;
    
    // 5. Main Render Loop
    while(!glfwWindowShouldClose(window))
    {
        // 5.1 Input Processing
        camera.Innputs(window);
        
        // 5.2 Clearing
        glClearColor(0.01f, 0.01f, 0.05f, 1.0f);  //Helps me create the night sky
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // 5.3 Drawing the Cube
        defaultShader.Activate();
        
        //Calculate and uploading the MVP matrices -- HEREEEE
        glm::mat4 model = glm::mat4(1.0f);
        
        //Rotation
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();
        
        defaultShader.setMat4("model", model);
        defaultShader.setMat4("view", view);
        defaultShader.setMat4("projection", projection);
        
        cubeVAO.Bind();
        
        //Drawing the cube using indicies
        glDrawElements(GL_TRIANGLES, sizeof(cubeIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
        
        if (frame_count++ < 5 || frame_count % 100 == 0) {
            std::cout << "Frame " << frame_count << ": drew " << (sizeof(cubeIndices) / sizeof(GLuint)) << " indices" << std::endl;
        }
        
        // 5.4 Swap and Poll
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
        
        // 6. Cleanup
        cubeVAO.Delete();
        cubeVBO.Delete();
        cubeEBO.Delete();
        defaultShader.Delete();
        
        glfwTerminate();
        return 0;
    
}

//Function Definations for Callback Functions

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    camera.width = width;
    camera.height = height;
}
/*
void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    //Passing empty function for now
}
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    //Will use this function to add zoom in/out with the scroll wheel of mouse 
}
*/