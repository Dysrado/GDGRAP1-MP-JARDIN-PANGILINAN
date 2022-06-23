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
#include "Skybox.h"
#include "Model3D.h"


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

    Skybox* skybox = new Skybox();

    // Parameters for the Model3D "obj path, texture path, rgba or rgb, .vert path, .frag path, isPlayer"
    Model3D* player = new Model3D(
        "3D/Models/UFO.obj",
        "3D/Textures/ufo_diffuse.png",
        "rgba",
        "Shaders/player.vert",
        "Shaders/player.frag",
        true
    );// Model Source: https://www.turbosquid.com/3d-models/free-3ds-model-flying-saucer/1081073#
    player->initialize();

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

    // Insert skybox here
    skybox->initialize("Shaders/skybox.vert", "Shaders/skybox.frag");
    skybox->bindBuffers();

    /* ======================= Initialize Values ======================= */
    float SPEED_MULTIPLIER = 60.f;
    float distance = 40.f;
    glm::mat4 identity = glm::mat4(1.f);

    PerspectiveCamera* pCam = new PerspectiveCamera();

    // the object transform
    player->initVariables(glm::vec3(0, 0, -1), glm::vec3(1, 1, 1), glm::vec3(0.2f));
    debris1->initVariables(glm::vec3(0, 7, -30), glm::vec3(0, -45, 0), glm::vec3(0.5f));

    pCam->initialize(glm::vec3(0, 0, -1));

    // light
    glm::vec3 lightPos = glm::vec3(-10, 10, 5);
    glm::vec3 lightColor = glm::vec3(1.f, 1.f, 1.f);

    float ambientStr = 0.1f;
    glm::vec3 ambientColor = lightColor;

    float specStr = 0.5f;
    float specPhong = 16.f;

    float lastTime = glfwGetTime();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Current Time */
        GLfloat currTime = glfwGetTime();

        /* Time that has passed */
        float deltaTime = currTime - lastTime;

        // Update Uniforms for the Skybox
        skybox->updateUniforms(pCam->getView(), pCam->getProj());

        // Update Uniforms for the Player Object
        player->updateUniforms();
        pCam->updateUniforms(player->getShader());

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
        pCam->updateUniforms(debris1->getShader());

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

        pCam->update(window, deltaTime);
        //// Update Values for View Based on the Yaw and Pitch
        //F.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        //F.y = sin(glm::radians(pitch));
        //F.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        //F = glm::normalize(F);
        //R = glm::normalize(glm::cross(F, WorldUp));
        //U = glm::normalize(glm::cross(R, F));

        //view = glm::lookAt(cameraPos, cameraPos + F, WorldUp);

        player->update(window, deltaTime);


        lastTime = currTime;
    }

    delete player;
    delete debris1;

    skybox->deleteBuffers();


    glfwTerminate();
    return 0;
}