//  main.cpp
//  Wizard Academy
//
//  Created by Diksha Chottani on 11/2/25.
//Still running the test program

#include<iostream>
#include <fstream>
#include <vector>
#include<algorithm>

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


//Magical floating
float wizardRiseDuration = 8.0f;    
float wizardMaxHeight = 3.0f;     
float floatAmplitude = 0.8f;      
float floatSpeed = 2.0f;            
float headWizardStartTime = 0.0f;   

class SimpleParticle {
public:
    unsigned int VAO, VBO;
    
    SimpleParticle() {
        float vertices[] = {
            -0.5f, -0.5f,    0.0f, 0.0f,
            0.5f, -0.5f,    1.0f, 0.0f,
            0.5f,  0.5f,    1.0f, 1.0f,
            -0.5f, -0.5f,    0.0f, 0.0f,
            0.5f,  0.5f,    1.0f, 1.0f,
            -0.5f,  0.5f,    0.0f, 1.0f
        };
        
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        // Position attribute (2D quad position)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        
        // Texture coordinate attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        
        glBindVertexArray(0);
    }
    
    void Draw() {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
    
    ~SimpleParticle() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
};

//Function Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow *window);

void DrawParticleSpell(
    Shader &shader,
    Model &particleModel,
    const glm::mat4 &parentModel,
    float time,
    int numLeaves = 90,
    float radius = 4.0f,
    float verticalAmp = 2.0f,
    float verticalSpeed = 5.0f,
    float orbitSpeed = 5.0f,
    float phasePerLeaf = 0.4f
);


void DrawSimpleParticleSpell(
    Shader &shader,
    SimpleParticle &particle,
    const glm::mat4 &parentModel,
    const glm::vec3 &cameraPos,
    const glm::vec3 &cameraRight,
    const glm::vec3 &cameraUp,
    float time,
    int numParticles = 90,
    float radius = 4.0f,
    float verticalAmp = 2.0f,
    float verticalSpeed = 5.0f,
    float orbitSpeed = 5.0f,
    float phasePerParticle = 0.4f,
    glm::vec3 color = glm::vec3(1.0f, 0.9f, 0.3f)
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    // 4. Setup for the Cube Geometry using the classes
    
    //Setting up the shader
    Shader defaultShader("/Users/dchottani/Desktop/Wizard-s-Academy/Wizard Academy/shaders/basic.vert", "/Users/dchottani/Desktop/Wizard-s-Academy/Wizard Academy/shaders/basic.frag");
    
    //Setting up the skybox shaders
    Shader skyboxShader("/Users/dchottani/Desktop/Wizard-s-Academy/Wizard Academy/shaders/skybox.vert", "/Users/dchottani/Desktop/Wizard-s-Academy/Wizard Academy/shaders/skybox.frag");

    //the particle shaders
    Shader particleShader("/Users/dchottani/Desktop/Wizard-s-Academy/Wizard Academy/shaders/particle.vert","/Users/dchottani/Desktop/Wizard-s-Academy/Wizard Academy/shaders/particle.frag");

    //Loading the model HEREEEEE

    Model castelModel("Wizard Academy/assets/Model/castle/de_haar_castle.obj");
    Model skyboxModel("Wizard Academy/assets/Model/skybox/free_skybox_fantasy_lands_cloudy_space.obj");
    Model statueModel("Wizard Academy/assets/Model/statue/pbr_stylized_statue_-_mage__wizard.obj");
    Model headWizardModel("Wizard Academy/assets/Model/headWizard/dumbledore.obj");
    Model particleModel("Wizard Academy/assets/Model/particle/uttm_glow.obj");

    //student wizard models 
    Model wizard1Model("Wizard Academy/assets/Model/wizard1/draco_malfoy_cos_ps2.obj");
    Model wizard2Model("Wizard Academy/assets/Model/wizard2/harry_potter_cos_ps2.obj");
    Model wizard3Model("Wizard Academy/assets/Model/wizard3/hermione_granger_cos_gamecubexbox.obj");
    Model wizard4Model("Wizard Academy/assets/Model/wizard4/ron_weasley_cos_ps2.obj");

    std::cout << "Model loaded successfully. Ready to enter." << std::endl;

    SimpleParticle simpleParticle;
    std::cout << "Particle system initialized!" << std::endl;

    // 5. Main Render Loop
    while(!glfwWindowShouldClose(window))
    {

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //input 
        processInput(window);


        //render heree
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

        float cycleSpeed = 0.1f; 
        float timeOfDay = sin(currentFrame * 0.05f) * 0.5f + 0.5f;

        glm::vec3 dayColor = glm::vec3(1.3f, 1.25f, 1.1f);
        glm::vec3 nightColor = glm::vec3(0.2f, 0.3f, 0.5f);
        glm::vec3 sunsetColor = glm::vec3(1.5f, 0.6f, 0.3f);

        //diksha -- fix this please 
        glm::vec3 lightColor;
        if (timeOfDay < 0.3f) {
            // Night to sunrise
            float t = timeOfDay / 0.3f;
            lightColor = glm::mix(nightColor, sunsetColor, t);
        } else if (timeOfDay < 0.5f) {
            // Sunrise to day
            float t = (timeOfDay - 0.3f) / 0.2f;
            lightColor = glm::mix(sunsetColor, dayColor, t);
        } else if (timeOfDay < 0.7f) {
            // Day to sunset
            float t = (timeOfDay - 0.5f) / 0.2f;
            lightColor = glm::mix(dayColor, sunsetColor, t);
        } else {
            // Sunset to night
            float t = (timeOfDay - 0.7f) / 0.3f;
            lightColor = glm::mix(sunsetColor, nightColor, t);
        }

        // light direction for sun position
        float sunAngle = currentFrame * cycleSpeed;
        glm::vec3 lightDir = glm::normalize(glm::vec3(
            cos(sunAngle) * 0.6f,
            -sin(sunAngle) * 0.8f,
            -0.3f
        ));

        // Update sky background color
        glm::vec3 skyDayColor = glm::vec3(0.529f, 0.808f, 0.922f);    
        glm::vec3 skyNightColor = glm::vec3(0.05f, 0.05f, 0.15f);     
        glm::vec3 skySunsetColor = glm::vec3(0.8f, 0.4f, 0.3f);       


        //fix this after the light color loop
        glm::vec3 skyColor;
        if (timeOfDay < 0.3f) {
            float t = timeOfDay / 0.3f;
            skyColor = glm::mix(skyNightColor, skySunsetColor, t);
        } else if (timeOfDay < 0.5f) {
            float t = (timeOfDay - 0.3f) / 0.2f;
            skyColor = glm::mix(skySunsetColor, skyDayColor, t);
        } else if (timeOfDay < 0.7f) {
            float t = (timeOfDay - 0.5f) / 0.2f;
            skyColor = glm::mix(skyDayColor, skySunsetColor, t);
        } else {
            float t = (timeOfDay - 0.7f) / 0.3f;
            skyColor = glm::mix(skySunsetColor, skyNightColor, t);
        }

        glClearColor(skyColor.r, skyColor.g, skyColor.b, 1.0f);


        defaultShader.setVec3("lightDirection", lightDir);
        defaultShader.setVec3("lightColor", lightColor);
        defaultShader.setVec3("viewPos", camera.Position);
        defaultShader.setFloat("shininess", 16.0f);

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
        
        //floating wizard
        float currentYOffset = 0.0f;
        
        if (headWizardStartTime == 0.0f) {
            headWizardStartTime = currentFrame; 
        }
        
        float animationTime = currentFrame - headWizardStartTime;

        if (animationTime < wizardRiseDuration) {
            float t = animationTime / wizardRiseDuration;
            float smoothT = 3.0f * t * t - 2.0f * t * t * t; 
            currentYOffset = smoothT * wizardMaxHeight;
            
        } else {
            float bobbingTime = animationTime - wizardRiseDuration;
            currentYOffset = wizardMaxHeight + floatAmplitude * sin(bobbingTime * floatSpeed);
        }
        
        
        glm::vec3 basePosition = glm::vec3(-53.0f, 10.0f, 0.0f);
        glm::vec3 finalPosition = basePosition + glm::vec3(0.0f, currentYOffset, 0.0f);
        
        // Apply Translation after the floating thing
        headWizardMatrix = glm::translate(headWizardMatrix, finalPosition);

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

        //adaptive blending
        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        particleShader.use();
        particleShader.setMat4("projection", projection);
        particleShader.setMat4("view", view);

        //DIKSHA - particle thing for all wizards 
        static bool debugPrinted = false;
        if (!debugPrinted) {
            std::cout << "Drawing particles..." << std::endl;
            debugPrinted = true;
        }
        
        // purple for head wizard 
        DrawSimpleParticleSpell(particleShader, simpleParticle, headWizardMatrix, 
                            camera.Position, camera.Right, camera.Up, currentFrame,
                            70, 4.0f, 2.0f, 2.0f, 2.0f, 0.4f, 
                            glm::vec3(0.8f, 0.3f, 1.0f));

        // Green here 
        DrawSimpleParticleSpell(particleShader, simpleParticle, wizard1Matrix,
                            camera.Position, camera.Right, camera.Up, currentFrame,
                            50, 3.0f, 1.5f, 2.5f, 2.2f, 0.5f,
                            glm::vec3(0.3f, 1.0f, 0.3f)); 
        
        //red here 
        DrawSimpleParticleSpell(particleShader, simpleParticle, wizard2Matrix,
                            camera.Position, camera.Right, camera.Up, currentFrame,
                            60, 3.5f, 1.8f, 2.3f, 2.1f, 0.45f,
                            glm::vec3(1.0f, 0.3f, 0.3f)); 
        
        //blue here 
        DrawSimpleParticleSpell(particleShader, simpleParticle, wizard3Matrix,
                            camera.Position, camera.Right, camera.Up, currentFrame,
                            55, 3.2f, 1.6f, 2.4f, 2.3f, 0.48f,
                            glm::vec3(0.3f, 0.6f, 1.0f));


        // gold at this one 
        DrawSimpleParticleSpell(particleShader, simpleParticle, wizard4Matrix,
                            camera.Position, camera.Right, camera.Up, currentFrame,
                            65, 3.8f, 2.0f, 2.2f, 2.0f, 0.42f,
                            glm::vec3(1.0f, 0.9f, 0.3f)); 

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);

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


}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    float xpos = static_cast<float>(xPos);

    float ypos = static_cast<float>(yPos);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 

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

void DrawParticleSpell(
    Shader &shader,
    Model &particleModel,
    const glm::mat4 &parentModel,
    float time,
    int numLeaves,
    float radius,
    float verticalAmp,
    float verticalSpeed,
    float orbitSpeed,
    float phasePerLeaf
)
{
    // Extracting world-space of head wizard 
    glm::vec3 parentPos = glm::vec3(
        parentModel[3].x,
        parentModel[3].y,
        parentModel[3].z
    );

    // parent loaction
    glm::mat3 parentRot = glm::mat3(
        glm::normalize(glm::vec3(parentModel[0])),
        glm::normalize(glm::vec3(parentModel[1])),
        glm::normalize(glm::vec3(parentModel[2]))
    );

    glm::vec3 right   = parentRot[0];
    glm::vec3 up      = parentRot[1];
    glm::vec3 forward = parentRot[2];

    // particle postion 
    std::vector<std::pair<float, glm::vec3>> particles;
    particles.reserve(numLeaves);

    for (int i = 0; i < numLeaves; ++i) {
        float angle = time * orbitSpeed + i * phasePerLeaf;
        
        //circular motion 
        glm::vec3 offset =
            right   * (radius * cos(angle)) +
            forward * (radius * sin(angle)) +
            up      * (verticalAmp * sin(time * verticalSpeed + i * 0.3f));
        
        glm::vec3 pos = parentPos + offset;
        float dist = glm::length(camera.Position - pos);
        particles.emplace_back(dist, pos);
    }

    // alpha blending for particle
    std::sort(particles.begin(), particles.end(),
        [](const auto &a, const auto &b) { return a.first > b.first; });

    // Rendering
    for (const auto &p : particles) {
        glm::vec3 pos = p.second;
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        model = glm::scale(model, glm::vec3(0.5f));
        
        shader.setMat4("model", model);
        shader.setVec3("cameraRight", camera.Right);
        shader.setVec3("cameraUp", camera.Up);
        shader.setFloat("size", 0.6f);
        
        particleModel.Draw(shader);
    }
}

void DrawSimpleParticleSpell(
    Shader &shader,
    SimpleParticle &particle,
    const glm::mat4 &parentModel,
    const glm::vec3 &cameraPos,
    const glm::vec3 &cameraRight,
    const glm::vec3 &cameraUp,
    float time,
    int numParticles,
    float radius,
    float verticalAmp,
    float verticalSpeed,
    float orbitSpeed,
    float phasePerParticle,
    glm::vec3 color
)
{
    // parent pos
    glm::vec3 parentPos = glm::vec3(parentModel[3]);
    
    // rotation
    glm::mat3 parentRot = glm::mat3(
        glm::normalize(glm::vec3(parentModel[0])),
        glm::normalize(glm::vec3(parentModel[1])),
        glm::normalize(glm::vec3(parentModel[2]))
    );
    
    glm::vec3 right = parentRot[0];
    glm::vec3 up = parentRot[1];
    glm::vec3 forward = parentRot[2];
    
    // Generate and sort particles
    std::vector<std::pair<float, glm::vec3>> particles;
    particles.reserve(numParticles);
    
    for (int i = 0; i < numParticles; ++i) {
        float angle = time * orbitSpeed + i * phasePerParticle;
        float verticalOffset = verticalAmp * sin(time * verticalSpeed + i * 0.3f);
        
        glm::vec3 offset = 
            right * (radius * cos(angle)) +
            forward * (radius * sin(angle)) +
            up * verticalOffset;
        
        glm::vec3 pos = parentPos + offset;
        float dist = glm::length(cameraPos - pos);
        particles.emplace_back(dist, pos);
    }
    
    std::sort(particles.begin(), particles.end(),
        [](const auto &a, const auto &b) { return a.first > b.first; });
    
    // Drawing particles now
    for (size_t i = 0; i < particles.size(); ++i) {
        glm::vec3 pos = particles[i].second;
        float alpha = 0.7f + 0.3f * sin(time * 3.0f + i * 0.2f);
        
        shader.setVec3("particlePos", pos);
        shader.setVec3("cameraRight", cameraRight);
        shader.setVec3("cameraUp", cameraUp);
        shader.setFloat("particleSize", 1.2f);
        shader.setVec4("particleColor", glm::vec4(color, alpha));
        
        particle.Draw();
    }
}