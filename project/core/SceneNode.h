#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Model;

class SceneNode {
public:
    SceneNode();

    virtual ~SceneNode() = default;

    void setModel(std::shared_ptr<Model> model);
    void addChild(std::shared_ptr<SceneNode> child);

    // Setter und Getter für Rotationsgeschwindigkeit
    void setRotationSpeed(float speed);
    float getRotationSpeed() const;

    virtual void update(float deltaTime);
    virtual void draw(const glm::mat4& parentTransform, unsigned int shaderID);

    glm::mat4 transform;  // Lokale Transformation (Translation, Rotation etc.)

private:
    std::shared_ptr<Model> model;
    std::vector<std::shared_ptr<SceneNode>> children;

    // Beispiel für Rotation um eigene Achse
    float rotationSpeed = 0.0f; // Grad/Sekunde
    float currentRotation = 0.0f;
};
