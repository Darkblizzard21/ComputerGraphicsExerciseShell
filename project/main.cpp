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
#include <filesystem>
#include <memory>
#include "core/Camera.h"
#include "render/Model.h"
#include "render/Shader.h"
#include "core/SceneNode.h"



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
    std::cout << "[DEBUG] CWD: "
        << std::filesystem::current_path()
        << std::endl;

    // 2) Prüfen, ob die Shader-Dateien existieren
    namespace fs = std::filesystem;
    const auto vsPath = "../../../../project/shaders/sky.vert";
    const auto fsPath = "../../../../project/shaders/sky.frag";
    std::cerr << "[DEBUG] exists(sky.vert)? "
        << fs::exists(vsPath)
        << "  at " << fs::absolute(vsPath) << std::endl;
    std::cerr << "[DEBUG] exists(sky.frag)? "
        << fs::exists(fsPath)
        << "  at " << fs::absolute(fsPath) << std::endl;
    // end 

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
    Shader modelShader(
        "../../../../project/shaders/model.vert",
        "../../../../project/shaders/model.frag"
    );
    Shader skyboxShader(
        "../../../../project/shaders/skybox.vert",
        "../../../../project/shaders/skybox.frag"
    );



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

    // Galaxy Skybox Setup 
    Shader skyShader(
        "../../../../project/shaders/sky.vert",
        "../../../../project/shaders/sky.frag"
    );
    Model skySphere(
        "../../../../project/models/galaxy_skybox/inside_galaxy.glb");
    unsigned int skyTex = loadTexture(
        "../../../../project/models/galaxy_skybox/inside_galaxy.png");
    // Prüfung ob Model korrekt geladen wird (Konsolenausgabe)
    std::cout << "Loaded meshes: " << skySphere.getMeshCount() << "\n";
    auto texIDs = skySphere.getTextureIDs();
    for (auto id : texIDs)
        std::cout << "  Texture ID: " << id << "\n";


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

        // --- Szene rendern ---
        modelShader.use();
        modelShader.setVec3("lightDir", lightDirection);
        modelShader.setVec3("lightColor", glm::vec3(1.0f));
        modelShader.setVec3("viewPos", camera.getPosition());
        modelShader.setMat4("view", view);
        modelShader.setMat4("projection", proj);

        rootNode->update(delta);
        rootNode->draw(glm::mat4(1.0f), modelShader.ID);

        // Galaxy Skybox 
        GLboolean wasCull = glIsEnabled(GL_CULL_FACE);  //Culling ausschalten, damit wir die Innenseiten der Kugel sehen
        if (wasCull) glDisable(GL_CULL_FACE);
        glDepthMask(GL_FALSE);                                         
        skyShader.use();                                               
        skyShader.setMat4("view", glm::mat4(glm::mat3(view)));         
        skyShader.setMat4("projection", proj);                        
        glActiveTexture(GL_TEXTURE0);                                 
        glBindTexture(GL_TEXTURE_2D, skyTex);                         
        skyShader.setInt("equirectangularMap", 0);                         
        skySphere.draw(skyShader.ID, glm::mat4(1.0f));                 
        glDepthMask(GL_TRUE);      
        if (wasCull) glEnable(GL_CULL_FACE); //Tiefen-Schreibungen wieder an

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
