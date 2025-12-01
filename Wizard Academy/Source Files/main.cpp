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
    
    glEnable(GL_DEPTH_TEST); 
    
    // 4. Setup for the Cube Geometry using the classes
    
    //Setting up the shader
    Shader defaultShader("/Users/dchottani/Desktop/Wizard-s-Academy/Wizard Academy/shaders/basic.vert", "/Users/dchottani/Desktop/Wizard-s-Academy/Wizard Academy/shaders/basic.frag");
    

    //Setting up the skybox shaders
    Shader skyboxShader("/Users/dchottani/Desktop/Wizard-s-Academy/Wizard Academy/shaders/skybox.vert", "/Users/dchottani/Desktop/Wizard-s-Academy/Wizard Academy/shaders/skybox.frag");


    //Loading the model HEREEEEE

    Model castelModel("Wizard Academy/assets/Model/castle/de_haar_castle.obj");
    //Model wizardModel("Wizard Academy/assets/Model/wizard/elf_wizard.obj");
    //Model leaveModel("Wizard Academy/assets/Model/leaves/birch_leaf.obj");
    Model skyboxModel("Wizard Academy/assets/Model/skybox/free_skybox_fantasy_lands_cloudy_space.obj");
    Model statueModel("Wizard Academy/assets/Model/statue/pbr_stylized_statue_-_mage__wizard.obj");
    Model headWizardModel("Wizard Academy/assets/Model/headWizard/dumbledore.obj");

    //student wizard models 
    Model wizard1Model("Wizard Academy/assets/Model/wizard1/draco_malfoy_cos_ps2.obj");
    Model wizard2Model("Wizard Academy/assets/Model/wizard2/harry_potter_cos_ps2.obj");
    Model wizard3Model("Wizard Academy/assets/Model/wizard3/hermione_granger_cos_gamecubexbox.obj");
    Model wizard4Model("Wizard Academy/assets/Model/wizard4/ron_weasley_cos_ps2.obj");

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
        
        //Skybox model 
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
        glm::mat4 view = camera.getViewMatrix();
        
        skyboxShader.use();


        glm::mat4 skyboxMatrix = glm::mat4(1.0f);
        skyboxMatrix = glm::scale(skyboxMatrix, glm::vec3(500.0f));

        skyboxShader.setMat4("projection", projection);
        skyboxShader.setMat4("view", camera.getViewMatrix());
        skyboxShader.setMat4("model", skyboxMatrix);
        glDepthFunc(GL_LEQUAL);
        skyboxModel.Draw(skyboxShader);
        glDepthFunc(GL_LESS);

        //Other shaders to load othe models 
        defaultShader.use();
        defaultShader.setMat4("projection", projection);
        defaultShader.setMat4("view", view);

        glm::vec3 lightDir = glm::vec3(0.5f, -1.0f, -0.5f);
        glm::vec3 lightColor = glm::vec3(3.0f, 3.0f, 2.7f);

        defaultShader.setVec3("LightDirection", lightDir);
        defaultShader.setVec3("lightColor", lightColor);
        defaultShader.setVec3("viewPos", camera.Position);
        defaultShader.setFloat("shininess", 32.0f);

        // CASTLE FOR THE COURTYARD
        glm::mat4 castelMatrix = glm::mat4(1.0f);

        castelMatrix = glm::translate(castelMatrix, glm::vec3(0.0f,0.0f,0.0f));

        castelMatrix = glm::scale(castelMatrix, glm::vec3(1.0f));

        defaultShader.setMat4("model", castelMatrix);
        castelModel.Draw(defaultShader);

        //Loading the statue
        glm::mat4 statueMatrix = glm::mat4(1.0f);
        statueMatrix = glm::translate(statueMatrix, glm::vec3(-45.5f, 17.5f, 0.0f));
        statueMatrix = glm::rotate(statueMatrix,glm::radians(-90.0f), glm::vec3(0,1,0));
        statueMatrix = glm::scale(statueMatrix, glm::vec3(3.0f));

        defaultShader.setMat4("model",statueMatrix);
        statueModel.Draw(defaultShader);

        // Loading the head wizard
        glm::mat4 headWizardMatrix = glm::mat4(1.0f);
        headWizardMatrix = glm::translate(headWizardMatrix,glm::vec3(-53.0f, 10.0f, 0.0f));
        headWizardMatrix = glm::rotate(headWizardMatrix, glm::radians(-90.0f), glm::vec3(0,1,0));
        headWizardMatrix = glm::scale(headWizardMatrix, glm::vec3(4.0f));

        defaultShader.setMat4("model", headWizardMatrix);
        headWizardModel.Draw(defaultShader);

        //Loading the student wizards
        //wizard 1 
        
        glm::mat4 wizard1Matrix = glm::mat4(1.0f);
        wizard1Matrix = glm::translate(wizard1Matrix, glm::vec3(-64.0f, 10.0f, 5.0f));
        wizard1Matrix = glm::rotate(wizard1Matrix, glm::radians(-90.0f), glm::vec3(1,0,0));
        wizard1Matrix = glm::rotate(wizard1Matrix, glm::radians(110.0f), glm::vec3(0,0,1));
        wizard1Matrix = glm::scale(wizard1Matrix,glm::vec3(4.0f));

        defaultShader.setMat4("model",wizard1Matrix);
        wizard1Model.Draw(defaultShader);
        

        //wizard 2
        glm::mat4 wizard2Matrix = glm::mat4(1.0f);
        wizard2Matrix = glm::translate(wizard2Matrix, glm::vec3(-59.0f, 10.0f, 13.0f));
        wizard2Matrix = glm::rotate(wizard2Matrix, glm::radians(-90.0f), glm::vec3(1,0,0));
        wizard2Matrix = glm::rotate(wizard2Matrix, glm::radians(150.0f), glm::vec3(0,0,1));
        wizard2Matrix = glm::scale(wizard2Matrix, glm::vec3(4.0f));

        defaultShader.setMat4("model", wizard2Matrix);
        wizard2Model.Draw(defaultShader);

        //wizard 3 
        glm::mat4 wizard3Matrix = glm::mat4(1.0f);
        wizard3Matrix = glm::translate(wizard3Matrix, glm::vec3(-64.0f, 10.0f, -5.0f));
        wizard3Matrix = glm::rotate(wizard3Matrix, glm::radians(60.0f), glm::vec3(0,1,0));
        wizard3Matrix = glm::scale(wizard3Matrix, glm::vec3(4.0f));

        defaultShader.setMat4("model", wizard3Matrix);
        wizard3Model.Draw(defaultShader);

        //wizard 4
        //This is the bigger model 
        glm::mat4 wizard4Matrix = glm::mat4(1.0f);
        wizard4Matrix = glm::translate(wizard4Matrix,glm::vec3(-59.0f, 10.0f, -13.0f));
        wizard4Matrix = glm::rotate(wizard4Matrix, glm::radians(-90.0f), glm::vec3(1,0,0));
        wizard4Matrix = glm::rotate(wizard4Matrix, glm::radians(30.0f), glm::vec3(0,0,1));
        wizard4Matrix = glm::scale(wizard4Matrix, glm::vec3(4.0f));

        defaultShader.setMat4("model", wizard4Matrix);
        wizard4Model.Draw(defaultShader);

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

