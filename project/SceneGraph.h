#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Mesh.h"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>

struct Transform {
    /*SPACE INFORMATION*/
    // Local space information
    glm::vec3 pos      = {0.0f, 0.0f, 0.0f};
    glm::vec3 eulerRot = {0.0f, 0.0f, 0.0f};
    glm::vec3 scale    = {1.0f, 1.0f, 1.0f};

    glm::mat4 getLocalModelMatrix();
    glm::mat4 getGlobalModelMatrix(const glm::mat4& parentMatrix);
};

class Entity {
public:
    Transform transform = {};

    // Global space information concatenate in matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    Entity*                              parent   = nullptr;
    std::vector<std::unique_ptr<Entity>> children = {};

    std::vector<std::shared_ptr<Mesh>> meshes = {};

    // constructor.
    Entity(const std::vector<std::shared_ptr<Mesh>> meshes = {});
    // add vitual destructor to enshure the destructor of child classes are called correctly
    virtual ~Entity() = default;

    // add new child with std::make_unique and set this as parent
    void addChild(const std::vector<std::shared_ptr<Mesh>> meshes = {});
    // add new child which is moved into this entity
    void addChild(std::unique_ptr<Entity>&& child);

    // Calls update frame on this entity and all its children 
    // (parent first to update then depth first for the children)
    // GLFWwindow* window and delta time are passed through
    void         AdvanceFrame(GLFWwindow* window, const float& deltaTime);
    // Virtual (overridable function) which can be used to implement logic like 
    // animation, interaction or other 
    virtual void AdvanceFrameImpl(GLFWwindow* window, const float& deltaTime){};

    // update self and child modelMatrix 
    // (is called after AdvanceFrame)
    void updateSelfAndChild();
};