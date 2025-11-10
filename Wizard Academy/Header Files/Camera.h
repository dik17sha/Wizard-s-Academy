//
//  Camera.h
//  Wizard Academy
//
//  Created by Diksha Chottani on 11/7/25.
//

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

class Camera{
public:
    
    //Storing the initial position
    glm::vec3 Position;
    
    
    //Storing the intial direction of the camera
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    
    
    //Storing the up direction
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    
    //Projection Properties
    float FovDeg = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    
    //Specifing Window Dimensions
    int width;
    int height;
    
    //Speed Settings
    float speed = 0.5f;
    float sensitivity = 100.0f;
    
    //Mouse State
    bool firstClick = true;
    
    //Constructor
    Camera(int width, int height, glm::vec3 position);
    
    //For calculating te View Matrix
    glm::mat4 getViewMatrix();
    
    //For calculating the Projection Matrix
    glm::mat4 getProjectionMatrix();
    
    //Function that will handle the user input
    void Innputs(GLFWwindow* window);
    
};
