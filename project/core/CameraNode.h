#pragma once
#include "core/SceneNode.h"
#include "core/Camera.h"

class CameraNode : public SceneNode {
public:
    CameraNode(GLFWwindow* window);
    void update(float dt) override;
    Camera& getCamera();

private:
    Camera camera;
};
