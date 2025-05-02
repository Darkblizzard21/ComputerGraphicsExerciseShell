#include "SceneNode.h"
#include "Model.h"

SceneNode::SceneNode() {
   transform = glm::mat4(1.0f);
}

void SceneNode::setModel(std::shared_ptr<Model> model) {
   this->model = model;
}

void SceneNode::addChild(std::shared_ptr<SceneNode> child) {
   children.push_back(child);
}

void SceneNode::removeChild(std::shared_ptr<SceneNode> child) {
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        children.erase(it);
    }
}


void SceneNode::update(float deltaTime) {
    if (rotationSpeed != 0.0f) {
        currentRotation += rotationSpeed * deltaTime;
        if (currentRotation > 360.0f) currentRotation -= 360.0f;
    }

    for (auto& child : children) {
        child->update(deltaTime);
    }
}



void SceneNode::draw(const glm::mat4& parentTransform, unsigned int shaderID) {
    glm::mat4 rotation =
        glm::rotate(glm::mat4(1.0f),
            glm::radians(currentRotation),
            glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 globalTransform = parentTransform * transform * rotation;


   if (model) {
       model->draw(shaderID, globalTransform);
   }

   for (auto& child : children) {
       child->draw(globalTransform, shaderID);
   }
}

void SceneNode::setRotationSpeed(float speed) {
   rotationSpeed = speed;
}

float SceneNode::getRotationSpeed() const {
   return rotationSpeed;
}
