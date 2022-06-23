//#include <glad/glad.h>
//#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
//#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include <string>
//#include <iostream>


//#include "Camera.h"
#include "Camera/PerspectiveCamera.h"
#include "Camera/OrthoCamera.h"
#include "Skybox.h"
#include "Model3D.h"
#include "Player.h"


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    float width = 800;
    float height = 800;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Jardin - Pangilinan", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    // Create Skybox
    Skybox* skybox = new Skybox();

    // Create Player
    // Parameters for the Model3D "obj path, texture path, rgba or rgb, .vert path, .frag path, isPlayer"
    Player* player = new Player(
        "3D/Models/UFO.obj",
        "3D/Textures/ufo_diffuse.png",
        "rgba",
        "Shaders/player.vert",
        "Shaders/player.frag",
        true
    );// Model Source: https://www.turbosquid.com/3d-models/free-3ds-model-flying-saucer/1081073#
    player->initialize();

    // Create 1st Debris
    Model3D* debris1 = new Model3D(
        "3D/Models/sphere.obj",
        "3D/Textures/planet.jpg",
        "rgb",
        "Shaders/sample.vert",
        "Shaders/sample.frag",
        false
    );
    debris1->initialize();

    // enables depth test
    glEnable(GL_DEPTH_TEST);

    // Initializes the variables and shaders for the skybox
    skybox->initialize("Shaders/skybox.vert", "Shaders/skybox.frag");
    skybox->bindBuffers();

    /* ======================= Initialize Values ======================= */
    float SPEED_MULTIPLIER = 60.f;
    float distance = 40.f;
    glm::mat4 identity = glm::mat4(1.f);

    // Initialize cameras
    bool inPers = true;
    PerspectiveCamera* pCam = new PerspectiveCamera();
    OrthoCamera* oCam = new OrthoCamera();

    // the object transform
    player->initVariables(glm::vec3(0, 0, -1), glm::vec3(1, 1, 1), glm::vec3(0.2f));
    debris1->initVariables(glm::vec3(0, 7, -30), glm::vec3(0, -45, 0), glm::vec3(0.5f));

    // Initialize the values needed for the camera
    pCam->initialize(glm::vec3(0, 0, -1));
    oCam->initialize(glm::vec3(0, 0, -1));

    // light
    glm::vec3 lightPos = glm::vec3(-10, 10, 5);
    glm::vec3 lightColor = glm::vec3(1.f, 1.f, 1.f);

    float ambientStr = 0.1f;
    glm::vec3 ambientColor = lightColor;

    float specStr = 0.5f;
    float specPhong = 16.f;

    float lastTime = glfwGetTime();
    float lastCDTime = glfwGetTime(); // Last time the camera was swaped

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Current Time */
        GLfloat currTime = glfwGetTime();
        float cooldownTimer = glfwGetTime(); // Used for the cooldown for swaping cameras

        /* Time that has passed */
        float deltaTime = currTime - lastTime;

        // Update Uniforms for the Skybox
        skybox->updateUniforms(pCam->getView(), pCam->getProj());

        // Update Uniforms for the Player Object
        player->updateUniforms();
        if (inPers) {
            pCam->updateUniforms(player->getShader());
        }
        else {
            oCam->updateUniforms(player->getShader());
        }

        // Replace this when light is added ===============================================================
        // Lighting
        GLuint lightPosAddress = glGetUniformLocation(player->getShader(), "lightPos");
        glUniform3fv(lightPosAddress, 1, glm::value_ptr(lightPos));

        GLuint lightColorAddress = glGetUniformLocation(player->getShader(), "lightColor");
        glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));

        GLuint ambientColorAddress = glGetUniformLocation(player->getShader(), "ambientColor");
        glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));

        GLuint ambientStrAddress = glGetUniformLocation(player->getShader(), "ambientStr");
        glUniform1f(ambientStrAddress, ambientStr);

        GLuint specStrAddress = glGetUniformLocation(player->getShader(), "specStr");
        glUniform1f(specStrAddress, specStr);

        GLuint specPhongAddress = glGetUniformLocation(player->getShader(), "specPhong");
        glUniform1f(specPhongAddress, specPhong);

        // Draw Player
        player->render();

        // for the debris
        debris1->updateUniforms();
        if (inPers) {
            pCam->updateUniforms(debris1->getShader());
        }
        else {
            oCam->updateUniforms(debris1->getShader());
        }

        // Replace this when light is added ===============================================================
        // Lighting
        GLuint lightPosAddress2 = glGetUniformLocation(debris1->getShader(), "lightPos");
        glUniform3fv(lightPosAddress2, 1, glm::value_ptr(lightPos));

        GLuint lightColorAddress2 = glGetUniformLocation(debris1->getShader(), "lightColor");
        glUniform3fv(lightColorAddress2, 1, glm::value_ptr(lightColor));

        GLuint ambientColorAddress2 = glGetUniformLocation(debris1->getShader(), "ambientColor");
        glUniform3fv(ambientColorAddress2, 1, glm::value_ptr(ambientColor));

        GLuint ambientStrAddress2 = glGetUniformLocation(debris1->getShader(), "ambientStr");
        glUniform1f(ambientStrAddress2, ambientStr);

        GLuint specStrAddress2 = glGetUniformLocation(debris1->getShader(), "specStr");
        glUniform1f(specStrAddress2, specStr);

        GLuint specPhongAddress2 = glGetUniformLocation(debris1->getShader(), "specPhong");
        glUniform1f(specPhongAddress2, specPhong);

        // Draw debris1
        debris1->render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        //cam->processInput(window);
        if (cooldownTimer > lastCDTime + 0.5f) { // in 0.5 seconds you can change cameras
            if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
                lastCDTime = glfwGetTime(); // Resets the timer
                if (inPers) {
                    inPers = false;
                }
                else {
                    inPers = true;
                }
            }
        }

        if (inPers) { // Allows movement during perspective mode
            pCam->update(window, deltaTime, player->getPosition());
            player->setF(pCam->getF());
            player->setR(pCam->getR());
            player->update(window, deltaTime, true);
        }
        else {
            oCam->update(window, deltaTime, player->getPosition());
        }

        lastTime = currTime;
    }

    delete player;
    delete debris1;
    delete pCam;
    delete oCam;

    skybox->deleteBuffers();


    glfwTerminate();
    return 0;
}