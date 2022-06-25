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


#include "Camera/PerspectiveCamera.h"
#include "Camera/OrthoCamera.h"
#include "Skybox.h"
#include "Model3D.h"
#include "Player.h"
#include "Light.h"


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
    Light* lightManager = new Light();
    // Create Player
    // Parameters for the Model3D "obj path, texture path, rgba or rgb, .vert path, .frag path, isPlayer"
    Player* player = new Player(
        "3D/Models/UFO.obj",
        "3D/Textures/ufo_diffuse.png",
        "rgba",
        "Shaders/player.vert",
        "Shaders/player.frag",
        true, 1
    );// Model Source: https://www.turbosquid.com/3d-models/free-3ds-model-flying-saucer/1081073#
    player->initialize();

    Model3D* planet = new Model3D(
        "3D/Models/sphere.obj",
        "3D/Textures/planet.jpg",
        "rgb",
        "Shaders/sample.vert",
        "Shaders/sample.frag",
        false, 0
    );
    planet->initialize();

    // Create 1st Debris
    Model3D* debris1 = new Model3D(
        "3D/Models/sus.obj",
        "3D/Textures/texture.jpg",
        "rgb",
        "Shaders/sample.vert",
        "Shaders/sample.frag",
        false, 1
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
    player->initVariables(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(0.2f));
    planet->initVariables(glm::vec3(0, 7, -100), glm::vec3(0, 0, 0), glm::vec3(5.f));
    debris1->initVariables(glm::vec3(0, -5, 20), glm::vec3(0, 0, 0), glm::vec3(0.1f));

    // Initialize the values needed for the camera
    pCam->initialize(glm::vec3(0, 0, -1));
    oCam->initialize(glm::vec3(0, 0, -1));

  

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

        // ==================================== Skybox ====================================
        // Update Uniforms for the Skybox
        if (inPers) {
            skybox->updateUniforms(pCam->getView(), pCam->getProj());
        }
        else {
            skybox->updateUniforms(oCam->getView(), oCam->getProj());
        }
        

        // ==================================== Player ====================================
        // Update Uniforms for the Player Object
        player->updateUniforms();
        if (inPers) {
            pCam->updateUniforms(player->getShader());
        }
        else {
            oCam->updateUniforms(player->getShader());
        }
        
        lightManager->update(player->getShader(), window, player->getF());
       
        // Draw Player
        player->render();

        // ==================================== Debris1 ====================================
        // for the debris
        debris1->updateUniforms();
        
        if (inPers) {
            pCam->updateUniforms(debris1->getShader());
        }
        else {
            oCam->updateUniforms(debris1->getShader());
        }

        // Draw debris1
        lightManager->update(debris1->getShader(), window, player->getF());
        debris1->render();


        // ==================================== Planet ====================================
        planet->updateUniforms();
        if (inPers) {
            pCam->updateUniforms(planet->getShader());
        }
        else {
            oCam->updateUniforms(planet->getShader());
        }
        //lightManager->update(planet->getShader());
        planet->render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        if (cooldownTimer > lastCDTime + 0.5f) {
            if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
                lastCDTime = glfwGetTime();
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
   // delete planet;
    delete debris1;
    delete pCam;
    delete oCam;

    skybox->deleteBuffers();


    glfwTerminate();
    return 0;
}