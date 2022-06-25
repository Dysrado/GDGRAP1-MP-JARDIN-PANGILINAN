#include "Light.h"

Light::Light()
{
 
}

Light::~Light()
{
}

void Light::update(GLuint shaderProgram, glm::vec3 direction, glm::vec3 cameraPos)
{
    if (type == 0) {
        ambientStr = 0.5;
        GLuint lightTypeAddress = glGetUniformLocation(shaderProgram, "lightType");
        glUniform1i(lightTypeAddress, 0);
        
        GLuint lightPosAddress = glGetUniformLocation(shaderProgram, "lightPos");
        glUniform3fv(lightPosAddress, 1, glm::value_ptr(lightPos));
    }

    else if (type == 1) {
        ambientStr = 0.2;
        GLuint lightTypeAddress = glGetUniformLocation(shaderProgram, "lightType");
        glUniform1i(lightTypeAddress, 1);

        GLuint lightPosAddress = glGetUniformLocation(shaderProgram, "lightPos");
        glUniform3fv(lightPosAddress, 1, glm::value_ptr(cameraPos));
        GLuint lightDirectionAddress = glGetUniformLocation(shaderProgram, "lightDirection");
        glUniform3fv(lightDirectionAddress, 1, glm::value_ptr(direction));
    }
   

    GLuint lightColorAddress = glGetUniformLocation(shaderProgram, "lightColor");
    glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));

    GLuint  ambientColorAddress = glGetUniformLocation(shaderProgram, "ambientColor");
    glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));

    GLuint ambientStrAddress = glGetUniformLocation(shaderProgram, "ambientStr");
    glUniform1f(ambientStrAddress, ambientStr);

    GLuint  specStrAddress = glGetUniformLocation(shaderProgram, "specStr");
    glUniform1f(specStrAddress, specStr);

    GLuint  specPhongAddress = glGetUniformLocation(shaderProgram, "specPhong");
    glUniform1f(specPhongAddress, specPhong);
}

void Light::updateInput(GLFWwindow* window)
{
    float cooldownTimer = glfwGetTime();
    if (cooldownTimer > lastCDTime + 0.5f) {
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
            lastCDTime = glfwGetTime();
            if (type == 0) {
                type = 1;
            }
            else {
                type = 0;
            }
        }
    }
}


