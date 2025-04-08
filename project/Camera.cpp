#include "Camera.h"

Camera::Camera(float aspect, float nearZ, float farZ, float FOVY) : aspect(aspect), nearZ(nearZ), farZ(farZ), FOVY(FOVY)
{
    UpdatePerspectiveMatrix();
}

void Camera::AdvanceFrameImpl(GLFWwindow* window, const float& deltaTime)
{
    updateSelfAndChild();

    // movement
    const glm::vec3 forward = -modelMatrix[2];
    const glm::vec3 up      = modelMatrix[1];
    const glm::vec3 right   = modelMatrix[0];

    // TODO: Add movement logic here
}

void Camera::scrollCallback(const double& xoffset, const double& yoffset)
{
    // TODO: Add scroll logic here (call back has to be bound to glfwSetScrollCallback for it to be called on scroll input)
}

glm::mat4 Camera::GetViewMatrix()
{
    // view matrix is the matrix from world to the local camera coordinate system
    // which is just the inverse of the model matrix:
    return glm::inverse(modelMatrix);
}

const glm::mat4& Camera::GetPerspectivMatrix()
{
    return perspective;
}

float Camera::GetAspect()
{
    return aspect;
}

float Camera::GetNearZ()
{
    return nearZ;
}

float Camera::GetFarZ()
{
    return farZ;
}

float Camera::GetFoV()
{
    return FOVY;
}

void Camera::SetAspect(float newAspect)
{
    if (aspect == newAspect) {
        return;
    }
    aspect = newAspect;
    UpdatePerspectiveMatrix();
}

void Camera::SetNearZ(float newNearZ)
{
    if (nearZ == newNearZ) {
        return;
    }
    nearZ = newNearZ;
    UpdatePerspectiveMatrix();
}

void Camera::SetFarZ(float newFarZ)
{
    if (farZ == newFarZ) {
        return;
    }
    farZ = newFarZ;
    UpdatePerspectiveMatrix();
}

void Camera::SetFoV(float newFOVY)
{
    if (FOVY == newFOVY) {
        return;
    }

    FOVY = newFOVY;
    UpdatePerspectiveMatrix();
}

void Camera::UpdatePerspectiveMatrix()
{
    perspective = glm::perspective(glm::radians(FOVY), aspect, nearZ, farZ);
}
