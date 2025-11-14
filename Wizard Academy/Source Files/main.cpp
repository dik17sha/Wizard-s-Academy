//  main.cpp
//  Wizard Academy
//
//  Created by Diksha Chottani on 11/2/25.
//Still running the test program

#include<iostream>
#include <fstream>
#include <vector>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

#include"Mesh.h"
#include "Camera.h"
#include "Model.h"
#include"shader.h"

//Window Settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

//Calling Camera object
Camera camera(glm::vec3(15.0f, 15.0f, 70.0f));
float lastX = SCR_WIDTH/2.0f;
float lastY = SCR_HEIGHT/2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//Creating Function Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow *window);
void DrawLeafSwirl(
    Shader &shader,
    Model &leafModel,
    const glm::mat4 &parentModel,
    float time,
    int numLeaves = 70,
    float radius = 4.0f,
    float verticalAmp = 2.0f,
    float verticalSpeed = 2.0f,
    float orbitSpeed = 2.0f,
    float phasePerLeaf = 0.4f
);


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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Wizard's Academy", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    
    //Setting callback functions for resizing and mouse input
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    //Locking cursor to the center of the screen
    glfwSetInputMode(window, GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    
    //Loading GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    std::cout <<"OpenGl version: " << glGetString(GL_VERSION) << std::endl;
    
    // 3. Global OpenGL state
    //stbi_set_flip_vertically_on_load(true);
    
    glEnable(GL_DEPTH_TEST); 
    
    // 4. Setup for the Cube Geometry using the classes
    
    //Setting up the shader
    Shader defaultShader("/Users/dchottani/Desktop/Wizard-s-Academy/Wizard Academy/shaders/basic.vert", "/Users/dchottani/Desktop/Wizard-s-Academy/Wizard Academy/shaders/basic.frag");
    
    //Loading the model HEREEEEE

    Model castelModel("Wizard Academy/assets/Model/castle/de_haar_castle.obj");
    Model wizardModel("Wizard Academy/assets/Model/wizard/elf_wizard.obj");
    Model leaveModel("Wizard Academy/assets/Model/leaves/birch_leaf.obj");


    std::cout << "Model loaded successfully. Ready to enter." << std::endl;

    // 5. Main Render Loop
    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //input 
        processInput(window);


        //render
        glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        defaultShader.use();

        
        
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100000.0f);
        glm::mat4 view = camera.getViewMatrix();

        
        defaultShader.setMat4("projection", projection);
        defaultShader.setMat4("view", view);

        // CASTLE FOR THE COURTYARD
        glm::mat4 castelMatrix = glm::mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

        castelMatrix = glm::translate(castelMatrix, glm::vec3(0.0f,0.0f,0.0f));

        castelMatrix = glm::scale(castelMatrix, glm::vec3(1.0f));

        defaultShader.setMat4("model", castelMatrix);
        castelModel.Draw(defaultShader);

        // WIZARD //

        //rotation to face towards the screen
        float wizardRotation = 30.0f; 
        wizardRotation += deltaTime * 30.0f;
        if(wizardRotation > 360.0f)
            wizardRotation -= 360.0f;

        //wizard rising after some seconds
        float hoverStartTime = 5.0f;
        float wizardHeight = 10.0f;
        if(currentFrame > hoverStartTime)
        {
            wizardHeight += sin((currentFrame - hoverStartTime) * 2.0f) * 5.0f;

        }

        // --- WIZARD TRANSFORMATION ---

        float wizardFloatDelay = 4.0f;       // delay before floating starts
        float wizardBaseHeight = 20.0f;      // initial Y height
        float wizardFloatAmplitude = 6.0f;   // how high/low he floats
        float wizardFloatSpeed = 2.0f;       // floating speed

        // Rotate wizard continuously
        wizardRotation = currentFrame * 30.0f;   // rotates 30 deg per second

        // Start floating only after delay
        float wizardY = wizardBaseHeight;
        if (currentFrame > wizardFloatDelay)
        {
            float t = currentFrame - wizardFloatDelay;
            wizardY += sin(t * wizardFloatSpeed) * wizardFloatAmplitude;
        }

        glm::mat4 wizardMatrix = glm::mat4(1.0f);

        // Move wizard to world position
        wizardMatrix = glm::translate(wizardMatrix, glm::vec3(-50.0f, wizardY, 5.0f));

        // Rotate around Y axis (to face camera)
        wizardMatrix = glm::rotate(wizardMatrix, glm::radians(wizardRotation), glm::vec3(0,1,0));

        // Scale him
        wizardMatrix = glm::scale(wizardMatrix, glm::vec3(15.0f));

        // Send to shader + draw
        defaultShader.setMat4("model", wizardMatrix);
        wizardModel.Draw(defaultShader);


        DrawLeafSwirl(defaultShader, leaveModel, wizardMatrix, currentFrame);

        // 5.4 Swapping the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    // 6. Cleanup

    glfwTerminate();
    return 0;

}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);


}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    float xpos = static_cast<float>(xPos);
    std::cout << "this is xPos: " << xpos << std::endl;

    float ypos = static_cast<float>(yPos);
    std::cout << "this is yPos: " << ypos << std::endl;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    //xoffset += 0.2f;
    //yoffset += 0.2f;

    camera.ProcessMouseMovement(xoffset, yoffset);

}
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yOffset));
}

void DrawLeafSwirl(
    Shader &shader,
    Model &leafModel,
    const glm::mat4 &parentModel,
    float time,
    int numLeaves,
    float radius,        // fixed radius (independent of wizard scale)
    float verticalAmp,
    float verticalSpeed,
    float orbitSpeed,
    float phasePerLeaf
)
{
    // Extract world-space parent position (UNSCALED)
    glm::vec3 parentPos = glm::vec3(
        parentModel[3].x,
        parentModel[3].y,
        parentModel[3].z
    );

    // Extract parent's rotation ONLY (ignore scale)
    glm::mat3 parentRot = glm::mat3(glm::normalize(glm::vec3(parentModel[0])),
                                    glm::normalize(glm::vec3(parentModel[1])),
                                    glm::normalize(glm::vec3(parentModel[2])));

    glm::vec3 right   = parentRot[0];
    glm::vec3 up      = parentRot[1];
    glm::vec3 forward = parentRot[2];

    for(int i = 0; i < numLeaves; i++)
    {
        float angle = time * orbitSpeed + i * phasePerLeaf;

        glm::vec3 offset =
            right   * (radius * cos(angle)) +
            forward * (radius * sin(angle)) +
            up      * (verticalAmp * sin(time * verticalSpeed + i * 0.3f));

        glm::vec3 pos = parentPos + offset;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        model = glm::scale(model, glm::vec3(30.0f)); // stable small leaf size

        shader.setMat4("model", model);
        leafModel.Draw(shader);
    }
}
