#include "Skybox.h"

void Skybox::initialize(std::string vert, std::string frag)
{
    // vertex shader
    std::fstream skyboxVertexSrc(vert);//"Shader/skybox.vert"
    std::stringstream skyboxVertexBuff;

    skyboxVertexBuff << skyboxVertexSrc.rdbuf();
    std::string skyboxVertexS = skyboxVertexBuff.str();

    const char* sky_v = skyboxVertexS.c_str();

    // fragment shader
    std::fstream skyboxFragSrc(frag);//"Shader/skybox.frag"
    std::stringstream skyboxFragBuff;

    skyboxFragBuff << skyboxFragSrc.rdbuf();
    std::string skyboxFragS = skyboxFragBuff.str();

    const char* sky_f = skyboxFragS.c_str();

    GLuint skyboxVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(skyboxVertexShader, 1, &sky_v, NULL);
    glCompileShader(skyboxVertexShader);

    GLuint skyboxFragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(skyboxFragShader, 1, &sky_f, NULL);
    glCompileShader(skyboxFragShader);

    skyboxShader = glCreateProgram();
    glAttachShader(skyboxShader, skyboxVertexShader);
    glAttachShader(skyboxShader, skyboxFragShader);

    glLinkProgram(skyboxShader);
    glDeleteShader(skyboxVertexShader);
    glDeleteShader(skyboxFragShader);

}

void Skybox::bindBuffers()
{
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

    std::string facesSkybox[]{ // fixed order
        "Skybox/skybox_right.png",
        "Skybox/skybox_left.png",
        "Skybox/skybox_up.png",
        "Skybox/skybox_down.png",
        "Skybox/skybox_front.png",
        "Skybox/skybox_back.png",
    };
    //
    // Skybox shader
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    // 3 because of xyz
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);

    // 36 bcs the length of the array is 36
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_INT) * 36, &skyboxIndices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    glGenTextures(1, &skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    // makes the skybox blurred instead of pixelated
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // clamps from edge to edge
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // 6 = faces of the cube
    for (unsigned int i = 0; i < 6; i++) {
        int w, h, skyCChannel;
        stbi_set_flip_vertically_on_load(false); // normally flipped vertically
        unsigned char* data = stbi_load(facesSkybox[i].c_str(), &w, &h, &skyCChannel, 0);
        if (data) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, // R,L,T,Bm,F,Bk
                0,
                GL_RGBA,
                w,
                h,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                data
            );
            // clean up
            stbi_image_free(data);
        }
    }
}

void Skybox::updateUniforms(glm::mat4 view, glm::mat4 projection)
{
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    glUseProgram(skyboxShader);
    glm::mat4 sky_view = glm::mat4(1.f);
    sky_view = glm::mat4(glm::mat3(view)); // removes translation

    unsigned int skyboxViewLoc = glGetUniformLocation(skyboxShader, "view");
    glUniformMatrix4fv(skyboxViewLoc, 1, GL_FALSE, glm::value_ptr(sky_view));

    unsigned int skyboxProjLoc = glGetUniformLocation(skyboxShader, "projection");
    glUniformMatrix4fv(skyboxProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}

void Skybox::deleteBuffers()
{
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteBuffers(1, &skyboxEBO);
}
