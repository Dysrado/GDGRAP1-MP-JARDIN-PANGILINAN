#include "Model3D.h"

Model3D::Model3D(std::string path, std::string texPath, std::string rgba, std::string vert, std::string frag, bool isPlayer)
{
    this->isPlayer = isPlayer;
    this->rgba = rgba;
    //std::cout << "Obj Path: " << path.c_str() << std::endl;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(&attributes,
        &shapes, &material,
        &warning, &error, path.c_str());

    // Mesh indices
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }

    if (this->isPlayer == true) {
        // Computation for the tangents
        for (int i = 0; i < shapes[0].mesh.indices.size(); i += 3) {
            tinyobj::index_t vData1 = shapes[0].mesh.indices[i];
            tinyobj::index_t vData2 = shapes[0].mesh.indices[i + 1];
            tinyobj::index_t vData3 = shapes[0].mesh.indices[i + 2];

            glm::vec3 v1 = glm::vec3(
                attributes.vertices[vData1.vertex_index * 3],
                attributes.vertices[(vData1.vertex_index * 3) + 1],
                attributes.vertices[(vData1.vertex_index * 3) + 2]
            );

            glm::vec3 v2 = glm::vec3(
                attributes.vertices[vData2.vertex_index * 3],
                attributes.vertices[(vData2.vertex_index * 3) + 1],
                attributes.vertices[(vData2.vertex_index * 3) + 2]
            );

            glm::vec3 v3 = glm::vec3(
                attributes.vertices[vData3.vertex_index * 3],
                attributes.vertices[(vData3.vertex_index * 3) + 1],
                attributes.vertices[(vData3.vertex_index * 3) + 2]
            );


            glm::vec2 uv1 = glm::vec2(
                attributes.texcoords[vData1.texcoord_index * 2],
                attributes.texcoords[(vData1.texcoord_index * 2) + 1]
            );

            glm::vec2 uv2 = glm::vec2(
                attributes.texcoords[vData2.texcoord_index * 2],
                attributes.texcoords[(vData2.texcoord_index * 2) + 1]
            );

            glm::vec2 uv3 = glm::vec2(
                attributes.texcoords[vData3.texcoord_index * 2],
                attributes.texcoords[(vData3.texcoord_index * 2) + 1]
            );

            glm::vec3 deltaPos1 = v2 - v1;
            glm::vec3 deltaPos2 = v3 - v1;

            glm::vec2 deltaUV1 = uv2 - uv1;
            glm::vec2 deltaUV2 = uv3 - uv1;

            float r = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV1.x));

            glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
            glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

            // for each vertex
            tangents.push_back(tangent);
            tangents.push_back(tangent);
            tangents.push_back(tangent);

            biTangents.push_back(bitangent);
            biTangents.push_back(bitangent);
            biTangents.push_back(bitangent);
        }

        // gets the vertices, texcoords, tangent and bitangent data
        for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
            tinyobj::index_t vData = shapes[0].mesh.indices[i];

            int pos_offset = vData.vertex_index * 3;
            int normal_offset = vData.normal_index * 3;
            int uv_offset = vData.texcoord_index * 2;

            fullVertexData.push_back(attributes.vertices[pos_offset]);// X
            fullVertexData.push_back(attributes.vertices[pos_offset + 1]); // Y
            fullVertexData.push_back(attributes.vertices[pos_offset + 2]); // Z

            fullVertexData.push_back(attributes.normals[normal_offset]); // normal
            fullVertexData.push_back(attributes.normals[normal_offset + 1]); // normal
            fullVertexData.push_back(attributes.normals[normal_offset + 2]); // normal

            fullVertexData.push_back(attributes.texcoords[uv_offset]); // U
            fullVertexData.push_back(attributes.texcoords[uv_offset + 1]); // V

            fullVertexData.push_back(tangents[i].x);
            fullVertexData.push_back(tangents[i].y);
            fullVertexData.push_back(tangents[i].z);

            fullVertexData.push_back(biTangents[i].x);
            fullVertexData.push_back(biTangents[i].y);
            fullVertexData.push_back(biTangents[i].z);

        }
    }
    else {
        // gets the vertices and texcoords data
        for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
            tinyobj::index_t vData = shapes[0].mesh.indices[i];

            int pos_offset = vData.vertex_index * 3;
            int normal_offset = vData.normal_index * 3;
            int uv_offset = vData.texcoord_index * 2;

            fullVertexData.push_back(attributes.vertices[pos_offset]);// X
            fullVertexData.push_back(attributes.vertices[pos_offset + 1]); // Y
            fullVertexData.push_back(attributes.vertices[pos_offset + 2]); // Z

            fullVertexData.push_back(attributes.normals[normal_offset]); // normal
            fullVertexData.push_back(attributes.normals[normal_offset + 1]); // normal
            fullVertexData.push_back(attributes.normals[normal_offset + 2]); // normal

            fullVertexData.push_back(attributes.texcoords[uv_offset]); // U
            fullVertexData.push_back(attributes.texcoords[uv_offset + 1]); // V

        }
    }

   

    // loads the texture into the .obj
    stbi_set_flip_vertically_on_load(true);
    tex_bytes = stbi_load(texPath.c_str(), &img_width, &img_height, &colorChannels, 0);
    //std::cout << "Texture Path: "  << texPath.c_str() << std::endl;

    if (this->isPlayer == true) {
        stbi_set_flip_vertically_on_load(true);
        norm_bytes = stbi_load("3D/Textures/ufo_normal.png", &img_width2, &img_height2, &colorChannels2, 0);
    }

    // vertex shader
    std::fstream verStream(vert);
    std::stringstream vertStrStream;

    vertStrStream << verStream.rdbuf();
    std::string vertStr = vertStrStream.str();

    const char* vertSrc = vertStr.c_str();

    // fragment shader
    std::fstream fragStream(frag);
    std::stringstream fragStrStream;

    fragStrStream << fragStream.rdbuf();
    std::string fragStr = fragStrStream.str();

    const char* fragSrc = fragStr.c_str();

    // vertex Shader
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertSrc, NULL);
    glCompileShader(vertShader);

    // fragment Shader
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragSrc, NULL);
    glCompileShader(fragShader);

    // shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);

    glLinkProgram(shaderProgram);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind Buffer 
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Data for the Vertices, Normals, and UVs
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * fullVertexData.size(), fullVertexData.data(), GL_DYNAMIC_DRAW);

    if (this->isPlayer == true) {
        GLintptr normPtr = 3 * sizeof(GLfloat);
        GLintptr uvPtr = 6 * sizeof(GLfloat);

        GLintptr tangentPtr = 8 * sizeof(GLfloat);
        GLintptr bitangentPtr = 11 * sizeof(GLfloat);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 14 * sizeof(float), (void*)normPtr);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)uvPtr);

        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)tangentPtr);

        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)bitangentPtr);

        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(0);
    }
    else {
        GLintptr normPtr = 3 * sizeof(GLfloat);
        GLintptr uvPtr = 6 * sizeof(GLfloat);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)normPtr);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)uvPtr);

        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Model3D::~Model3D()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Model3D::initialize()
{
    // initialize texture values
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    if (rgba == "RGBA" || rgba == "rgba") {
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            img_width,
            img_height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            tex_bytes
        );
    }
    else {
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            img_width,
            img_height,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            tex_bytes
        );
    }

    // mip map
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);

    if (this->isPlayer == true) {
        // Normal Map
        glGenTextures(1, &norm_tex);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, norm_tex);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            img_width2,
            img_height2,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            norm_bytes
        );

        // mip map
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(norm_bytes);
    }


}

void Model3D::initVariables(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
    entity = identity;
    this->pos = position;
    entity = glm::translate(identity, pos);
    this->scale = scale;
    entity = glm::scale(entity, scale);
    this->rotation = rotation;
    entity = glm::rotate(entity, glm::radians(rotation.x), glm::vec3(1,0,0));
    entity = glm::rotate(entity, glm::radians(rotation.y), glm::vec3(0,1,0));
    entity = glm::rotate(entity, glm::radians(rotation.z), glm::vec3(0,0,1));
}

void Model3D::update(GLFWwindow* window, float deltaTime)
{ 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { // Move Forward
        pos.z -= 20 * deltaTime;
        entity = glm::translate(identity, pos);
        entity = glm::scale(entity, scale);
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { // Move Forward
        pos.z += 20 * deltaTime;
        entity = glm::translate(identity, pos);
        entity = glm::scale(entity, scale);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { // Move Right
        pos.x += 20 * deltaTime;
        entity = glm::translate(identity, pos);
        entity = glm::scale(entity, scale);
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { // Move Left
        pos.x -= 20 * deltaTime;
        entity = glm::translate(identity, pos);
        entity = glm::scale(entity, scale);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { // Move Up
        pos.y += 20 * deltaTime;
        entity = glm::translate(identity, pos);
        entity = glm::scale(entity, scale);
    }
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { // Move Down
        pos.y -= 20 * deltaTime;
        entity = glm::translate(identity, pos);
        entity = glm::scale(entity, scale);
    }
}

void Model3D::updateUniforms()
{
    glUseProgram(shaderProgram);

    // Uniforms
    unsigned int translationLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(translationLoc, 1, GL_FALSE, glm::value_ptr(entity));

    glActiveTexture(GL_TEXTURE0);
    GLuint tex0Loc = glGetUniformLocation(shaderProgram, "tex0");
    glUniform1i(tex0Loc, 0);
    glBindTexture(GL_TEXTURE_2D, texture);

    if (this->isPlayer == true) {
        glActiveTexture(GL_TEXTURE1);
        GLuint norm_texLoc = glGetUniformLocation(shaderProgram, "norm_tex");
        glBindTexture(GL_TEXTURE_2D, norm_tex);
        glUniform1i(norm_texLoc, 1);
    }
    
}


void Model3D::render()
{
    // Bind
    glBindVertexArray(VAO);
    // Draw
    glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 8);
}

GLuint Model3D::getShader()
{
    return shaderProgram;
}