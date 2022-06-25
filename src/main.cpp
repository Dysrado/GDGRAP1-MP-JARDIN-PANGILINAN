#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION



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
        "3D/Models/ship.obj",
        "3D/Textures/ship_diffuse.jpg",
        "rgb",
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
        "3D/Models/debris1.obj",
        "3D/Textures/debris1.png",
        "rgba",
        "Shaders/sample.vert",
        "Shaders/sample.frag",
        false, 1
    );
    debris1->initialize();

    // Create 2nd Debris
    Model3D* debris2 = new Model3D(
        "3D/Models/debris2.obj",
        "3D/Textures/debris2.png",
        "rgba",
        "Shaders/sample.vert",
        "Shaders/sample.frag",
        false, 1
    );
    debris2->initialize();

    // Create 3rd Debris
    Model3D* debris3 = new Model3D(
        "3D/Models/debris3.obj",
        "3D/Textures/debris3.png",
        "rgba",
        "Shaders/sample.vert",
        "Shaders/sample.frag",
        false, 1
    );
    debris3->initialize();

    // Create 4th Debris
    Model3D* debris4 = new Model3D(
        "3D/Models/debris4.obj",
        "3D/Textures/debris4.png",
        "rgba",
        "Shaders/sample.vert",
        "Shaders/sample.frag",
        false, 1
    );
    debris4->initialize();

    // Create 5th Debris
    Model3D* debris5 = new Model3D(
        "3D/Models/sus.obj",
        "3D/Textures/texture.jpg",
        "rgb",
        "Shaders/sample.vert",
        "Shaders/sample.frag",
        false, 1
    );
    debris5->initialize();


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
    int lightType = 0;
    PerspectiveCamera* pCam = new PerspectiveCamera();
    OrthoCamera* oCam = new OrthoCamera();

    // Object transform
    player->initVariables(glm::vec3(0, 0, 0), glm::vec3(1, -90, 1), glm::vec3(0.005f));
    planet->initVariables(glm::vec3(0, 7, 100), glm::vec3(0, 0, 0), glm::vec3(5.f));
    debris1->initVariables(glm::vec3(45, -5, -100), glm::vec3(0, 0, 0), glm::vec3(10.f));
    debris2->initVariables(glm::vec3(-150, -5, 75), glm::vec3(0, 0, 0), glm::vec3(10.f));
    debris3->initVariables(glm::vec3(50, -5, 60), glm::vec3(0, 0, 0), glm::vec3(10.f));
    debris4->initVariables(glm::vec3(150, -5,0), glm::vec3(0, 0, 0), glm::vec3(10.f));
    debris5->initVariables(glm::vec3(65, -5, 20), glm::vec3(0, 0, 0), glm::vec3(0.01f));

    // Initialize the values needed for the camera
    pCam->initialize(glm::vec3(0, 0, 0));
    oCam->initialize(glm::vec3(0, 0, 0));

    // Disables the Cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
        
        lightManager->update(player->getShader(), player->getFront(pCam->getCameraPos()), pCam->getCameraPos());
       
        // Draw Player
        player->render();

        // ==================================== Debris1 ====================================
        // Update Uniforms for the Debris Object
        debris1->updateUniforms();
        
        if (inPers) {
            pCam->updateUniforms(debris1->getShader());
        }
        else {
            oCam->updateUniforms(debris1->getShader());
        }

        // Draw debris1
        lightManager->update(debris1->getShader(),player->getFront(pCam->getCameraPos()), player->getPosition());
        debris1->render();

        // ==================================== Debris2 ====================================
        // Update Uniforms for the Debris Object
        debris2->updateUniforms();
        
        if (inPers) {
            pCam->updateUniforms(debris2->getShader());
        }
        else {
            oCam->updateUniforms(debris2->getShader());
        }

        // Draw debris2
        lightManager->update(debris2->getShader(),player->getFront(pCam->getCameraPos()), player->getPosition());
        debris2->render();

        
        // ==================================== Debris3 ====================================
        // Update Uniforms for the Debris Object
        debris3->updateUniforms();
        
        if (inPers) {
            pCam->updateUniforms(debris3->getShader());
        }
        else {
            oCam->updateUniforms(debris3->getShader());
        }

        // Draw debris3
        lightManager->update(debris3->getShader(),player->getFront(pCam->getCameraPos()), player->getPosition());
        debris3->render();
        
        // ==================================== Debris4 ====================================
        // Update Uniforms for the Debris Object
        debris4->updateUniforms();
        
        if (inPers) {
            pCam->updateUniforms(debris4->getShader());
        }
        else {
            oCam->updateUniforms(debris4->getShader());
        }

        // Draw debris4
        lightManager->update(debris4->getShader(),player->getFront(pCam->getCameraPos()), player->getPosition());
        debris4->render();

        
        // ==================================== Debris5 ====================================
        // Update Uniforms for the Debris Object
        debris5->updateUniforms();
        
        if (inPers) {
            pCam->updateUniforms(debris5->getShader());
        }
        else {
            oCam->updateUniforms(debris5->getShader());
        }

        // Draw debris5
        lightManager->update(debris5->getShader(),player->getFront(pCam->getCameraPos()), player->getPosition());
        debris5->render();


        // ==================================== Planet ====================================
       // Update Uniforms for the Planet Object
        planet->updateUniforms();
        if (inPers) {
            pCam->updateUniforms(planet->getShader());
        }
        else {
            oCam->updateUniforms(planet->getShader());
        }
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
        lightManager->updateInput(window);
       

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
    delete planet;
    delete debris1;
    delete debris2;
    delete debris3;
    delete debris4;
    delete debris5;
    delete pCam;
    delete oCam;
    delete lightManager;

    skybox->deleteBuffers();


    glfwTerminate();
    return 0;
}