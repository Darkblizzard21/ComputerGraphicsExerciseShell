#pragma once
#include "SceneGraph.h"

class Camera : public Entity {
public:
    Camera(float aspect, float nearZ = 0.1f, float farZ = 10000.f, float FOVY = 90);

    virtual void AdvanceFrameImpl(GLFWwindow* window, const float& deltaTime) override;
    void         scrollCallback(const double& xoffset, const double& yoffset);

    glm::mat4 GetViewMatrix();
    const glm::mat4& GetPerspectivMatrix();

    // Getter for the private variables
    float GetAspect();
    float GetNearZ();
    float GetFarZ();
    float GetFoV();

    // Setter functions for the private variables
    // They all call UpdatePerspectiveMatrix to keep 
    // the perspective matrix uptodate
    void SetAspect(float newAspect);
    void SetNearZ(float newNearZ);
    void SetFarZ(float newFarZ);
    void SetFoV(float newFOVY);

private:
    void  UpdatePerspectiveMatrix();

    glm::mat4 perspective = glm::identity<glm::mat4>();

    float aspect;
    float nearZ;
    float farZ;
    float FOVY;
};