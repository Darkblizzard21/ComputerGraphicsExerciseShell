#include "core/CameraNode.h"

// Konstruktor
CameraNode::CameraNode(GLFWwindow* window)
    : camera(window)
{
}

// update: ruft parent-update und camera.update() auf
void CameraNode::update(float dt) {
    SceneNode::update(dt);
    camera.update();
}

// Getter
Camera& CameraNode::getCamera() {
    return camera;
}

