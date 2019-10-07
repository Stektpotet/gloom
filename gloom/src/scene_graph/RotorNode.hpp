#pragma once

#include <glm/glm.hpp>
#include "SceneNode.hpp"

struct RotorNode : public SceneNode
{
    RotorNode(glm::vec3 rotationAxis = glm::vec3{ 1,0,0 }, float rotorSpeed = 10.0f);
    float rotorSpeed = 10.0f;
    glm::vec3 axis = { 1,0,0 };
    void update(float deltaTime) override;
};