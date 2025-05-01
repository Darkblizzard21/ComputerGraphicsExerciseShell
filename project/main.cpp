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
#include <filesystem>
#include <memory>
#include "render/Model.h"
#include "render/Shader.h"
#include "core/SceneNode.h"
#include "core/CameraNode.h"

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

// Load Texture für skybox galaxy sphere
unsigned int loadTexture(const char* path) {
    unsigned int id;
    glGenTextures(1, &id);
    int w, h, c;
    unsigned char* data = stbi_load(path, &w, &h, &c, 0);
    GLenum format = (c == 4 ? GL_RGBA : GL_RGB);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    // Wrap/Filter nach Wunsch
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);
    return id;
}

int main() {
    // Debugging start
    // 1) Ausgabe des aktuellen Arbeitsverzeichnisses
    std::cout << "[DEBUG] CWD: " << std::filesystem::current_path() << std::endl;

    // 2) GLFW initialisieren
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "3-Planet Scene", nullptr, nullptr);
    if (!window) {
        std::cerr << "Window creation failed\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 3) GLAD laden (Funktionspointer für OpenGL)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "GLAD init failed\n";
        return -1;
    }

    // Änderung: Initialen Viewport setzen (erster Aufruf)
    int scrW, scrH;
    glfwGetFramebufferSize(window, &scrW, &scrH);
    glViewport(0, 0, scrW, scrH);

    // 4) **Depth-Test** aktivieren
    glEnable(GL_DEPTH_TEST);
    // Änderung: Back-Face Culling aktivieren
    glEnable(GL_CULL_FACE);               // Änderung: Back-Faces werden ausgefiltert
    glCullFace(GL_BACK);                  // Änderung: Welche Seite (Back)
    glFrontFace(GL_CCW);                  // Änderung: Vertex-Winding (Counter-Clockwise)

    setupImGui(window);

    // 5) Kamera und Shader anlegen
    auto rootNode = std::make_shared<SceneNode>();  
    auto cameraNode = std::make_shared<CameraNode>(window); 
    cameraNode->transform =
        glm::translate(glm::mat4(1.0f), glm::vec3(500.0f, 30, 50.0f));
    rootNode->addChild(cameraNode);                    


    Shader modelShader(
        "../../../../project/shaders/model.vert",
        "../../../../project/shaders/model.frag"
    );
    Shader sunShader(
        "../../../../project/shaders/sun.vert",
        "../../../../project/shaders/sun.frag"
    );

    // 6) Modelle laden und SceneGraph aufbauen
    auto alienPlanet = std::make_shared<Model>("../../../../project/models/planets/alien_planet.glb");
    auto purplePlanet = std::make_shared<Model>("../../../../project/models/planets/purple_planet.glb");
    auto shinyPlanet = std::make_shared<Model>("../../../../project/models/planets/shiny_planet.glb");

    // Alien Planet Node
    auto alienNode = std::make_shared<SceneNode>();
    alienNode->setModel(alienPlanet);
    alienNode->transform =
        glm::translate(glm::mat4(1.0f), glm::vec3(-8.0f, 0.0f, -12.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.2f));
    rootNode->addChild(alienNode);

    // Purple Planet Node
    auto purpleNode = std::make_shared<SceneNode>();
    purpleNode->setModel(purplePlanet);
    purpleNode->transform =
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -15.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.8f));
    rootNode->addChild(purpleNode);

    // Shiny Planet Node
    auto shinyNode = std::make_shared<SceneNode>();
    shinyNode->setModel(shinyPlanet);
    shinyNode->transform =
        glm::translate(glm::mat4(1.0f), glm::vec3(8.0f, 0.0f, -12.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
    rootNode->addChild(shinyNode);

    float lastFrame = static_cast<float>(glfwGetTime());

    // Galaxy Skybox Setup
    Shader skyShader(
        "../../../../project/shaders/sky.vert",
        "../../../../project/shaders/sky.frag");

    Model skySphere("../../../../project/models/galaxy_skybox/inside_galaxy.glb");
    std::cout << "[DEBUG] SkySphere MeshCount: " << skySphere.getMeshCount() << std::endl; // Model laden Check
    unsigned int skyTex = loadTexture(
        "../../../../project/models/galaxy_skybox/inside_galaxy.png"
    );

    while (!glfwWindowShouldClose(window)) {
        // 1) Delta-Time berechnen
        float current = static_cast<float>(glfwGetTime());
        float delta = current - lastFrame;
        lastFrame = current;

        // 2) Events abfragen
        glfwPollEvents();

        // 3) Scene Graph updaten (inkl. CameraNode)
        rootNode->update(delta);

        // 4) Buffers löschen
        glPolygonMode(GL_FRONT_AND_BACK, isWireframe ? GL_LINE : GL_FILL);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 5) View & Projection aus CameraNode holen
        glm::mat4 view = cameraNode->getCamera().getViewMatrix();
        glm::mat4 proj = cameraNode->getCamera().getProjectionMatrix(1280.0f / 720.0f);

        // 6) Modelle rendern
        modelShader.use();
        modelShader.setInt("texture_diffuse", 0);
        modelShader.setVec3("lightDir", lightDirection);
        modelShader.setVec3("lightColor", glm::vec3(1.0f));
        modelShader.setVec3("viewPos", cameraNode->getCamera().getPosition());
        modelShader.setMat4("view", view);
        modelShader.setMat4("projection", proj);
        rootNode->draw(glm::mat4(1.0f), modelShader.ID);

        // 7) Skybox rendern
        GLboolean wasCull = glIsEnabled(GL_CULL_FACE);
        glDisable(GL_CULL_FACE);
        glDepthMask(GL_FALSE);
        skyShader.use();
        skyShader.setMat4("view", glm::mat4(glm::mat3(view)));
        skyShader.setMat4("projection", proj);
        glm::mat4 skyModel =                                    // Skybox skallieren
            glm::scale(glm::mat4(1.0f), glm::vec3(1000.0f));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, skyTex);
        skyShader.setInt("equirectangularMap", 0);
        //skySphere.draw(skyShader.ID, glm::mat4(1.0f));
        skySphere.draw(skyShader.ID, skyModel);
        glDepthMask(GL_TRUE);
        if (wasCull) glEnable(GL_CULL_FACE);

        // 8) ImGui zeichnen (mit Camera aus dem Graph)
        renderImGui(cameraNode->getCamera());

        // 9) Buffer swap
        glfwSwapBuffers(window);
    }

    // 10) Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
