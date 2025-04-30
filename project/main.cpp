// OpenGL Scene: 3 Planets + Skybox + Rotation

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <stb_image.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <iostream>
#include <memory>
#include "core/Camera.h"
#include "render/Model.h"
#include "render/Shader.h"
#include "core/SceneNode.h"

unsigned int loadCubemap(const std::vector<std::string>& faces);
unsigned int skyboxVAO = 0, skyboxVBO = 0;

void drawSkyboxCube() {
    if (skyboxVAO == 0) {
        float skyboxVertices[] = {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    }
    glBindVertexArray(skyboxVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}





// Globals
bool isWireframe = false;
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

void framebuffer_size_callback(GLFWwindow* w, int width, int height) {
    glViewport(0, 0, width, height);
}

void setupImGui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void renderImGui(Camera& camera) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Settings");
    ImGui::SliderFloat3("Light Direction", glm::value_ptr(lightDirection), -1.0f, 1.0f);
    ImGui::Checkbox("Wireframe Mode", &isWireframe);
    if (ImGui::Button("Reset Camera")) camera.reset();
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}



// loadCubemap implementation
unsigned int loadCubemap(const std::vector<std::string>& faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cerr << "Cubemap-Textur konnte nicht geladen werden: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}




int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "3-Planet Scene", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);
    setupImGui(window);

    Camera camera(window);
    Shader modelShader("shaders/model.vert", "shaders/model.frag");
    Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");

    unsigned int skyboxTex = loadCubemap({
		"assets/right.png", "assets/left.png",
        "assets/top.png", "assets/bottom.png",
        "assets/front.png", "assets/back.png"
        });

    // Load models
    auto planet1 = std::make_shared<Model>("assets/planet1.glb");
    auto planet2 = std::make_shared<Model>("assets/planet2.glb");
    auto moon = std::make_shared<Model>("assets/moon.glb");

    auto rootNode = std::make_shared<SceneNode>();

    auto node1 = std::make_shared<SceneNode>();
    node1->setModel(planet1);
    node1->transform = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    rootNode->addChild(node1);

    auto orbitNode = std::make_shared<SceneNode>();
    orbitNode->setRotationSpeed(10.0f); // rotates around planet1
    rootNode->addChild(orbitNode);

    auto node2 = std::make_shared<SceneNode>();
    node2->setModel(planet2);
    node2->transform = glm::translate(glm::mat4(1.0f), glm::vec3(3, 0, 0));
    orbitNode->addChild(node2);

    auto moonOrbit = std::make_shared<SceneNode>();
    moonOrbit->setRotationSpeed(30.0f);
    node2->addChild(moonOrbit);

    auto moonNode = std::make_shared<SceneNode>();
    moonNode->setModel(moon);
    moonNode->transform = glm::translate(glm::mat4(1.0f), glm::vec3(1, 0, 0));
    moonOrbit->addChild(moonNode);

    float lastFrame = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window)) {
        float current = static_cast<float>(glfwGetTime());
        float delta = current - lastFrame;
        lastFrame = current;

        glfwPollEvents();
        camera.update();

        glPolygonMode(GL_FRONT_AND_BACK, isWireframe ? GL_LINE : GL_FILL);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 proj = camera.getProjectionMatrix(1280.0f / 720.0f);

        modelShader.use();
        modelShader.setVec3("lightDir", lightDirection);
        modelShader.setVec3("lightColor", glm::vec3(1.0f));
        modelShader.setVec3("viewPos", camera.getPosition());
        modelShader.setMat4("view", view);
        modelShader.setMat4("projection", proj);

        rootNode->update(delta);
        rootNode->draw(glm::mat4(1.0f), modelShader.ID);

        // Draw skybox
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        skyboxShader.setMat4("view", glm::mat4(glm::mat3(view)));
        skyboxShader.setMat4("projection", proj);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
        // drawSkyboxCube(); → eigene Cube-VAO Funktion nötig
        glDepthFunc(GL_LESS);

        renderImGui(camera);
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
