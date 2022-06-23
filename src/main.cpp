#include <glad/glad.h>
#include <GLFW/glfw3.h>


#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

/*Linear Transformation*/
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*Shaders*/
#include <string>
#include <iostream>


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



int main(void)
{
    GLFWwindow* window;
    std::string path = "3D/plane.obj";

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;
    tinyobj::attrib_t attributes;
    std::vector<GLuint> mesh_indices;

    bool success = tinyobj::LoadObj(&attributes, &shapes, &material, &warning, &error, path.c_str());
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }
    //Vector to hold our tangents
    std::vector<glm::vec3> tangents;
    //Vector to hold our bittangents
    std::vector<glm::vec3> bitangents;
    
    //For loop that will go over all the vertics by 3
    for (int i = 0; i < shapes[0].mesh.indices.size(); i += 3) {
        //Get the 3 vertex data for the triangle
        tinyobj::index_t vData1 = shapes[0].mesh.indices[i];
        tinyobj::index_t vData2 = shapes[0].mesh.indices[i+1];
        tinyobj::index_t vData3 = shapes[0].mesh.indices[i+2];
       // Vertex1
        glm::vec3 v1 = glm::vec3(
            attributes.vertices[vData1.vertex_index * 3],
            attributes.vertices[(vData1.vertex_index * 3) + 1],
            attributes.vertices[(vData1.vertex_index * 3) + 2]
        );
       // Vertex2
        glm::vec3 v2 = glm::vec3(
            attributes.vertices[vData2.vertex_index * 3],
            attributes.vertices[(vData2.vertex_index * 3) + 1],
            attributes.vertices[(vData2.vertex_index * 3) + 2]
        );
       // Vertex3
        glm::vec3 v3 = glm::vec3(
            attributes.vertices[vData3.vertex_index * 3],
            attributes.vertices[(vData3.vertex_index * 3) + 1],
            attributes.vertices[(vData3.vertex_index * 3) + 2]
        );
        // UV Vertex1
        glm::vec2 uv1 = glm::vec2(
        attributes.texcoords[(vData1.texcoord_index * 2)],
        attributes.texcoords[(vData1.texcoord_index * 2)+1]
        
        );
        // UV Vertex2
        glm::vec2 uv2 = glm::vec2(
        attributes.texcoords[(vData2.texcoord_index * 2)],
        attributes.texcoords[(vData2.texcoord_index * 2)+1]
        
        );
        // UV Vertex3
        glm::vec2 uv3 = glm::vec2(
        attributes.texcoords[(vData3.texcoord_index * 2)],
        attributes.texcoords[(vData3.texcoord_index * 2)+1]
        
        );
        //Edges of the triangle : position delta
        glm::vec3 deltaPos1 = v2 - v1;
        glm::vec3 deltaPos2 = v3 - v1;
        //UV delta
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;
    
        float r = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.y - deltaPos1 * deltaUV2.y) * r;

        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);

    }
    
    std::vector<GLfloat> fullVertexData;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shapes[0].mesh.indices[i];
        fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3)]);
        fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3) + 1]);
        fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3) + 2]);

        fullVertexData.push_back(attributes.normals[(vData.normal_index * 3)]);
        fullVertexData.push_back(attributes.normals[(vData.normal_index * 3) + 1]);
        fullVertexData.push_back(attributes.normals[(vData.normal_index * 3) + 2]);

        fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2)]);
        fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2) + 1]);

        fullVertexData.push_back(tangents[i].x);
        fullVertexData.push_back(tangents[i].y);
        fullVertexData.push_back(tangents[i].z);
        fullVertexData.push_back(bitangents[i].x);
        fullVertexData.push_back(bitangents[i].y);
        fullVertexData.push_back(bitangents[i].z);


    }


    //Brickwall Texture load
    int img_width, img_height, colorChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* tex_bytes = stbi_load("3D/brickwall.jpg", &img_width, &img_height, &colorChannels, 0);
    

    //Brickwall Normals load
    int img_width2, img_height2, colorChannels2;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* normal_bytes = stbi_load("3D/brickwall_normal.jpg", &img_width2, &img_height2, &colorChannels2, 0);

    //Yae Texture load
    int img_width3, img_height3, colorChannels3;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* yae_bytes = stbi_load("3D/yae.png", &img_width3, &img_height3, &colorChannels3, 0);

    /*Vertex Shader Setup*/
    std::fstream vertStream1("Shaders/shader.vert");
    std::stringstream vertStrStream1;
    vertStrStream1 << vertStream1.rdbuf();
    std::string vertStr1 = vertStrStream1.str();
    const char* vertSrc1 = vertStr1.c_str();

    /*Fragment Shader Setup*/
    std::fstream fragStream1("Shaders/shader.frag");
    std::stringstream fragStrStream1;
    fragStrStream1 << fragStream1.rdbuf();
    std::string fragStr1 = fragStrStream1.str();
    const char* fragSrc1 = fragStr1.c_str();


    /*Skybox Shader Setup*/
    std::fstream skyboxVertSrc("Shaders/skybox.vert");
    std::stringstream skyboxVerBuff;
    skyboxVerBuff << skyboxVertSrc.rdbuf();
    std::string skyBoxVertS = skyboxVerBuff.str();
    const char* skyV = skyBoxVertS.c_str();

    std::fstream skyboxFragSrc("Shaders/skybox.frag");
    std::stringstream skyboxFragBuff;
    skyboxFragBuff << skyboxFragSrc.rdbuf();
    std::string skyBoxFragS = skyboxFragBuff.str();
    const char* skyF = skyBoxFragS.c_str();



    float width = 800;
    float height = 800;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    /*
  7--------6
 /|       /|
4--------5 |
| |      | |
| 3------|-2
|/       |/
0--------1
*/
//Vertices for the cube
    float skyboxVertices[]{
        -1.f, -1.f, 1.f, //0
        1.f, -1.f, 1.f,  //1
        1.f, -1.f, -1.f, //2
        -1.f, -1.f, -1.f,//3
        -1.f, 1.f, 1.f,  //4
        1.f, 1.f, 1.f,   //5
        1.f, 1.f, -1.f,  //6
        -1.f, 1.f, -1.f  //7
    };

    //Skybox Indices
    unsigned int skyboxIndices[]{
        1,2,6,
        6,5,1,

        0,4,7,
        7,3,0,

        4,5,6,
        6,7,4,

        0,3,2,
        2,1,0,

        0,1,5,
        5,4,0,

        3,7,6,
        6,2,3
    };
    std::string facesSkybox[]{
    "Skybox/rainbow_ft.png",
    "Skybox/rainbow_bk.png",
    "Skybox/rainbow_up.png",
    "Skybox/rainbow_dn.png",
    "Skybox/rainbow_rt.png",
    "Skybox/rainbow_lf.png",
    };


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Ramon Alvaro B. Jardin", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    //Texture 0 = Brick
    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);
    //Texture 1 = Brick_normal
    GLuint norm_tex;
    glGenTextures(1, &norm_tex);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, norm_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width2, img_height2, 0, GL_RGB, GL_UNSIGNED_BYTE, normal_bytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(normal_bytes);

    //Texture 2 = Yae Texture
    GLuint yae;
    glGenTextures(1, &yae);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, yae);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width3, img_height3, 0, GL_RGBA, GL_UNSIGNED_BYTE, yae_bytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(yae_bytes);
   

    glEnable(GL_DEPTH_TEST);

    GLuint vertShader1 = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader1, 1, &vertSrc1, NULL);
    glCompileShader(vertShader1);

    GLuint fragShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader1, 1, &fragSrc1, NULL);
    glCompileShader(fragShader1);

    GLuint shaderProgram1 = glCreateProgram();
    glAttachShader(shaderProgram1, vertShader1);
    glAttachShader(shaderProgram1, fragShader1);

    glLinkProgram(shaderProgram1);

    GLuint skyboxVShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(skyboxVShader, 1, &skyV, NULL);
    glCompileShader(skyboxVShader);

    GLuint skyboxFShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(skyboxFShader, 1, &skyF, NULL);
    glCompileShader(skyboxFShader);

    GLuint skyboxShader = glCreateProgram();
    glAttachShader(skyboxShader, skyboxVShader);
    glAttachShader(skyboxShader, skyboxFShader);
    glLinkProgram(skyboxShader);

    glDeleteShader(skyboxVShader);
    glDeleteShader(skyboxFShader);


    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * fullVertexData.size(), fullVertexData.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
    //UV starts at 6
    GLintptr uvPtr = 6 * sizeof(float);
   //norm starts at 3
    GLintptr normPtr = 3 * sizeof(GLfloat);
    //tangent starts at 8
    GLintptr tangetPtr = 8 * sizeof(GLfloat);
    //bitangent starts at 11
    GLintptr bitangetPtr = 11 * sizeof(GLfloat);


    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 14 * sizeof(float), (void*)normPtr);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)uvPtr);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)tangetPtr);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)bitangetPtr);

    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(1);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
   
    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_INT) * 36, &skyboxIndices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    unsigned int skyboxTex;
    glGenTextures(1, &skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    //Skybox wont pixelate
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++) {
        int w, h, skyChannel;
        stbi_set_flip_vertically_on_load(false);
        unsigned char* data = stbi_load(facesSkybox[i].c_str(), &w, &h, &skyChannel, 0);
        if (data) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, //R, L, T, B, F, B
                0,
                GL_RGB,
                w,
                h,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data
                );

            stbi_image_free(data);
        }
        else {
            std::cerr << "Error did not load properly\n";
        }
    }

    stbi_set_flip_vertically_on_load(true);

    /*Identity Matrix*/
    glm::mat4 identity = glm::mat4(1.0f);
   
    
    /*Perspective Project Matrix*/
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), height / width, 0.1f, 100.f);

    /*Camera Variables*/
    glm::vec3 movement(0.f, 0.f, 15.f);
    float rotation = 0;
    glm::vec3 scale(5.f);
    glm::vec3 cameraPos = glm::vec3(movement.x, movement.y, movement.z);
    glm::vec3 WorldUp = glm::vec3(0.f, 10.f, 0.f);
    glm::mat4 cameraPosMat = glm::translate(identity, cameraPos * -1.0f);
    glm::vec3 centerPos = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 F = glm::normalize(centerPos - cameraPos);
    glm::vec3 R = glm::normalize(glm::cross(F, WorldUp));
    glm::vec3 U = glm::normalize(glm::cross(R, F));
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + F, WorldUp);
  

    /*Transform Matrix*/
    glm::mat4 transform = glm::translate(identity, centerPos - glm::vec3(0,0,-1.f));
    transform = glm::scale(transform, scale);
    transform = glm::rotate(transform, glm::radians(90.f), glm::vec3(0, 0, -1));
   
    /*Clock Variables*/
    float lastTime = glfwGetTime();

    bool startup = true;
    /*Light Variables*/
    glm::vec3 lightPos = glm::vec3(-1, 2, 3.f);
    glm::vec3 lightColor = glm::vec3(1.f, 1.f, 1.f);

    float ambientStr = 5.f;
    glm::vec3 ambientColor = lightColor;

    float specStr = 0.5f;
    float specPhong = 16.f;
    /*Blending*/

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);
    
 
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        float pressed = glfwGetTime();  
        GLfloat currTime = glfwGetTime();
        float deltaTime = currTime - lastTime;
        
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        rotation = 30.f * deltaTime;
        transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0, -1, 0));
        
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        /*SkyBox*/
        glUseProgram(skyboxShader);
        glm::mat4 skyView = glm::mat4(1.f);
        skyView = glm::mat4(glm::mat3(view));

        unsigned int skyboxViewLoc = glGetUniformLocation(skyboxShader, "view");
        glUniformMatrix4fv(skyboxViewLoc, 1, GL_FALSE, glm::value_ptr(skyView));
        unsigned int skyboxProjLoc = glGetUniformLocation(skyboxShader, "projection");
        glUniformMatrix4fv(skyboxProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        /*ShaderProgam*/
        glUseProgram(shaderProgram1);

        unsigned int transformLoc = glGetUniformLocation(shaderProgram1, "transform"); /*Object*/
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram1, "projection"); /*Projection*/
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        unsigned int viewLoc = glGetUniformLocation(shaderProgram1, "view"); /*Camera*/
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        glActiveTexture(GL_TEXTURE0);
        GLuint tex0Address = glGetUniformLocation(shaderProgram1, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(tex0Address, 0);

        glActiveTexture(GL_TEXTURE1);
        GLuint norm_texLoc = glGetUniformLocation(shaderProgram1, "norm_tex");
        glBindTexture(GL_TEXTURE_2D, norm_tex);
        glUniform1i(norm_texLoc, 1);
        
        glActiveTexture(GL_TEXTURE2);
        GLuint yae_texLoc = glGetUniformLocation(shaderProgram1, "yae_tex");
        glBindTexture(GL_TEXTURE_2D, yae);
        glUniform1i(yae_texLoc, 2);

        GLuint lightAddress = glGetUniformLocation(shaderProgram1, "lightPos");
        glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));

        GLuint lightColorAddress = glGetUniformLocation(shaderProgram1, "lightColor");
        glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));
        
        GLuint ambientColorAddress = glGetUniformLocation(shaderProgram1, "ambientColor");
        glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));

        GLuint ambientStrAddress = glGetUniformLocation(shaderProgram1, "ambientStr");
        glUniform1f(ambientStrAddress, ambientStr);

        GLuint specStrAddress = glGetUniformLocation(shaderProgram1, "specStr");
        glUniform1f(specStrAddress, specStr);

        GLuint specPhongAddress = glGetUniformLocation(shaderProgram1, "specPhong");
        glUniform1f(specPhongAddress, specPhong);

        GLuint cameraPosAddress = glGetUniformLocation(shaderProgram1, "cameraPos");
        glUniform3fv(cameraPosAddress, 1, glm::value_ptr(cameraPos));
        
        glUseProgram(shaderProgram1);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 8);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        
        lastTime = currTime;
        
    }

    /*CLEANUP*/
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    

    glfwTerminate();
    return 0;
}