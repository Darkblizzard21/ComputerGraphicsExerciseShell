#include "SceneGraph.h"

glm::mat4 Transform::getLocalModelMatrix()
{
    return getGlobalModelMatrix(glm::identity<glm::mat4>());
}

glm::mat4 Transform::getGlobalModelMatrix(const glm::mat4& parentMatrix)
{
    glm::mat4 result = parentMatrix;

    // 1. apply tranlation

    // 2. apply roations: Y * X * Z

    // 3. apply scale

    // translation * rotation * scale (also know as TRS matrix)
    return result;
}

Entity::Entity(const std::vector<std::shared_ptr<Mesh>> meshes) : meshes(meshes) {}

void Entity::addChild(const std::vector<std::shared_ptr<Mesh>> meshes)
{
    // Add new child
    // set parent pointer of child to this
}

void Entity::addChild(std::unique_ptr<Entity>&& child)
{
    // move entity to children
    // set parent pointer of child to this
}

void Entity::AdvanceFrame(GLFWwindow* window, const float& deltaTime)
{
    AdvanceFrameImpl(window, deltaTime);
    for (auto& child : children) {
        child->AdvanceFrame(window, deltaTime);
    }
}

void Entity::updateSelfAndChild()
{
    if (parent) {
        modelMatrix = transform.getGlobalModelMatrix(parent->modelMatrix);
    } else {
        // for the root node the global matix is the same as the local Matrix
        modelMatrix = transform.getLocalModelMatrix();
    }

    // recursivly update children
    // so they incooperate the new model matrix of the parent
    for (auto& child : children) {
        child->updateSelfAndChild();
    }
}
