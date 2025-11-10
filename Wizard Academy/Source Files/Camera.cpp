//
//  Camera.cpp
//  Wizard Academy
//
//  Created by Diksha Chottani on 11/7/25.
//
#include"Camera.h"

#include<cmath>
#include<iostream>

double lastTime = 0.0;
float deltaTime = 0.0f;

Camera::Camera(int width, int height, glm::vec3 position)
{
    Camera::width = width;
    Camera::height = height;
    Position = position;
    
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(Position, Position + Orientation, Up);
}

glm::mat4 Camera::getProjectionMatrix()
{
    return glm::perspective(glm::radians(FovDeg), (float)width/height, nearPlane, farPlane);
}

void Camera::Innputs(GLFWwindow *window)
{
    //Calculating Delta time
    double currentTime = glfwGetTime();
    deltaTime = (float)(currentTime - lastTime);
    lastTime = currentTime;
    
    //Adjusting the speed based onthe delta time
    float currentSpeed = speed * deltaTime * 100.0f;
    
    //Getting the keyboard input
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        Position += currentSpeed * Orientation;
    }
    
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        Position -= currentSpeed * Orientation;
    }
    
    //Moving Right
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Position += currentSpeed * glm::normalize(glm::cross(Orientation, Up));
    }
    
    //Moving Left
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        Position -= currentSpeed * glm::normalize(glm::cross(Orientation, Up));
    }
    
    //Moving up
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        Position += currentSpeed * Up;
    }
    
    //Moving down
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        Position -= currentSpeed * Up;
    }
    
    // Mouse Input control function
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        glfwSetInputMode(window,GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        
        if (firstClick)
            
        {
            glfwSetCursorPos(window,(width/2), (height/2));
            firstClick = false;
        }
        
        double mouseX;
        double mouseY;
        
        //Getting the current position for the mouse
        glfwGetCursorPos(window, &mouseX, &mouseY);
        
        //Calculating the roatatiom offset from the center
        float rotX = sensitivity * (float)(mouseY - (height/2)) / height;
        float rotY = sensitivity * (float)(mouseY - (height/2)) / height;
        
        //Calculating the new orientation after rotation around the X-axis
        //PITCH
        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation,Up)));
     
        //Putting constraint on Pitch so it doesn't flip the camera
        if(abs(glm::angle(newOrientation, Up)- glm::radians(90.0f)) <= glm::radians(85.0f))
        {
            Orientation = newOrientation;
        }
        
        //Calculate the rotation around Y-axis
        //YAW
        Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);
        
        //Setting the mouse position to the center of the window for continuos movement
        glfwSetCursorPos(window, (width/2), (height/2));
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }
}
